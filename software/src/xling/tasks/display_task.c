/*-
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This file is part of a firmware for Xling, a tamagotchi-like toy.
 *
 * Copyright (c) 2019, 2020 Dmitry Salychev
 *
 * Xling firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Xling firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

/*
 * A display task.
 *
 * It is supposed to be the only task which updates the display.
 *
 * NOTE: An interrupt of the 16-bit Timer 3 and its compare-match channel A is
 * used to resume the display updating task.
 */

/* FreeRTOS headers. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* SH1106 driver headers. */
#include "mcusim/drivers/avr-gcc/avr/display/sh1106/sh1106.h"

/* Xling headers. */
#include "xling/tasks.h"
#include "xling/graphics.h"
#include "xling/msg.h"
#include "xling/scenes/scenes.h"
#include "xling/font/Alagard_12pt.h"

/******************************************************************************
 * Local macros.
 ******************************************************************************/

#define SET_BIT(byte, bit)	((byte) |= (1U << (bit)))
#define CLEAR_BIT(byte, bit)	((byte) &= (uint8_t) ~(1U << (bit)))
#define TASK_NAME		"Display Task"
#define TASK_PERIOD		(42) /* in ms (~ 23.80 Hz) */
//#define TASK_PERIOD		(83) /* in ms (~ 12.05 Hz) */
#define TASK_DELAY		(pdMS_TO_TICKS(TASK_PERIOD))
#define TEXT_BUFSZ		(128)

/*
 * Bigger stack size is necessary to draw text and images to the canvas which
 * will be moved to the display memory.
 */
#define STACK_SZ		(configMINIMAL_STACK_SIZE * 2)

/******************************************************************************
 * Local variables.
 ******************************************************************************/
/* Configuration of the OLED display driver. */
static const MSIM_SH1106DrvConf_t driver_conf = {
	.port_spi = &PORTB,		/* I/O port with SPI. */
	.ddr_spi = &DDRB,		/* DDR for the I/O port with SPI. */
	.mosi = PB5,			/* OLED SPI pin. */
	.miso = PB6,			/* OLED SPI pin. */
	.sck = PB7,			/* OLED SPI pin. */
};

/* Configuration of the OLED display. */
static const MSIM_SH1106Conf_t display_conf = {
	.rst_port = &PORTC,
	.rst_ddr = &DDRC,
	.cs_port = &PORTC,
	.cs_ddr = &DDRC,
	.dc_port = &PORTC,
	.dc_ddr = &DDRC,
	.rst = PC6,			/* OLED Reset pin. */
	.cs = PC7,			/* OLED Chip Select pin. */
	.dc = PC5,			/* OLED Data/Command pin. */
};

static volatile TaskHandle_t thandle;

/*
 * NOTE: 1024 bytes is enough to describe a monochrome image for OLED display
 *       which resolution is 128x64 px.
 */
static uint8_t display_buffer[1024];
static uint8_t cache_buffer[1024];
static xg_canvas_t canvas = {
	.data = display_buffer, .width = 128, .height = 64, .data_size = 8,
};
static xg_canvas_t cache_canvas = {
	.data = cache_buffer, .width = 128, .height = 64, .data_size = 8,
};
static char text_buf[TEXT_BUFSZ];
static xg_text_t text = {
	.font = &XG_FONT_Alagard_12pt,
	.text = &text_buf[0],
	.text_sz = TEXT_BUFSZ,
};
static xg_scene_ctx_t scene_ctx = {
	//.scene = &XG_SCN_walking_01,
	//.scene = &XG_SCN_smoking_02,
	//.scene = &XG_SCN_test_brick,
	.scene = &XG_SCN_peasant_house,
	//.scene = &XG_SCN_forest,
	//.scene = &XG_SCN_town_street,
	//.scene = &XG_SCN_dungeon,
	.canvas = &canvas,
	.text = &text,
	.frame_delay = 0,
	.bat_lvl = 100,
	.bat_stat = 0,
	.scene_mode = XG_SM_SCENE
};

/******************************************************************************
 * Prototypes of the local functions.
 ******************************************************************************/
static void	display_task(void *arg) __attribute__((noreturn));
static void	receive_msgs(const QueueHandle_t q, MSIM_SH1106_t *display,
    xg_scene_ctx_t *scene_ctx);

/******************************************************************************
 * Implementation.
 ******************************************************************************/
int
xt_init_display(xt_args_t *args, UBaseType_t prio, TaskHandle_t *task_handle)
{
	BaseType_t stat;
	int rc = 0;

	/*
	 * Configure the display and its driver. Don't have to worry about
	 * interrupts and FreeRTOS scheduler - they shouldn't be active at the
	 * moment.
	 */
	{
		/* Keep CS high. Display isn't selected by default. */
		SET_BIT(PORTC, display_conf.cs);

		/* Power On the display. */
		CLEAR_BIT(PORTC, display_conf.rst);
		_delay_ms(1);
		SET_BIT(PORTC, display_conf.rst);
		_delay_ms(10);

		/* Start the driver for SH1106-based displays. */
		MSIM_SH1106__drvStart(&driver_conf);
	}

	/* Create the display task. */
	stat = xTaskCreate(display_task, TASK_NAME, STACK_SZ,
	                   args, prio, task_handle);

	if (stat != pdPASS) {
		/* Display task couldn't be created. */
		rc = 1;
	} else {
		/* Task has been created successfully. */
		thandle = (*task_handle);
	}

	return rc;
}

static void
display_task(void *arg)
{
	const xt_args_t * const args = (xt_args_t *) arg;
	MSIM_SH1106_t * const display = MSIM_SH1106_Init(&display_conf);
	TickType_t ticks;
	TickType_t last_wake;

	/* Use current time as a seed for random generator. */
	srand((unsigned int) time(NULL));

	/* Setup an OLED display. */
	MSIM_SH1106_DisplayOff(display);

	MSIM_SH1106_SetDivFreq(display, 0xB0);
	MSIM_SH1106_SetPumpVoltage(display, 0x32);
	MSIM_SH1106_SetChargePeriod(display, 0x22);
	MSIM_SH1106_SetVCOMDeselectLevel(display, 0x35);
	MSIM_SH1106_SetContrast(display, 0x75);

	MSIM_SH1106_DisplayNormal(display);
	MSIM_SH1106_SetScanDirection(display, 0);
	MSIM_SH1106_DisplayOn(display);
	MSIM_SH1106_bufSend(display);

	/* Remember the last wake time. */
	last_wake = xTaskGetTickCount();

	/* Setup a canvas for cache */
	xg_cache_canvas(&cache_canvas);

	/* Task loop */
	while (1) {
		/* Wait for the next task tick. */
		vTaskDelayUntil(&last_wake, TASK_DELAY);

		/* Remember a moment in time. */
		ticks = xTaskGetTickCount();

		/*
		 * Receive and process all of the messages available in the
		 * display queue at the moment.
		 */
		receive_msgs(args->display_info.queue_handle, display,
		    &scene_ctx);

		/* Clear the canvas. */
		if (scene_ctx.scene_mode == XG_SM_SCENE) {
			memset(display_buffer, 0x00, 1024);
		}

		/* Process keyboard events. */
		if (scene_ctx.scene->kbd_cbk != NULL) {
			scene_ctx.scene->kbd_cbk(&scene_ctx);
		}

		switch (scene_ctx.scene_mode) {
		case XG_SM_SCENE:
			xg_draw_scene(&canvas, scene_ctx.scene);
			break;
		case XG_SM_SPEECH:
			xg_draw_speech(&canvas, scene_ctx.text);
			break;
		default:
			/* Shouldn't reach here. */
			break;
		}

		/* Transfer canvas buffer to the display. */
		xg_transfer_canvas(display, &canvas);

		/*
		 * Calculate a delay to receive a message from the queue and
		 * draw an animation frame.
		 */
		scene_ctx.frame_delay = xTaskGetTickCount() - ticks;
	}

	/*
	 * The task must be deleted before reaching the end of its
	 * implementating function.
	 *
	 * NOTE: Shouldn't reach this point!
	 */
	vTaskDelete(NULL);
}

static void
receive_msgs(const QueueHandle_t q, MSIM_SH1106_t * const display,
             xg_scene_ctx_t * const ctx)
{
	static uint8_t bat_lvl_skip = 5;
	xm_msg_t msg;
	BaseType_t status;

	/* Receive all of the messages from the queue. */
	while (1) {
		/* An attempt to receive the next message. */
		status = xQueueReceive(q, &msg, 0);

		/* Message has been received. */
		if (status == pdPASS) {
			switch (msg.type) {
			case XM_MSG_BATLVL:
				/* Skip several values initially. */
				if (bat_lvl_skip > 0) {
					bat_lvl_skip--;
				} else if (msg.value > 100) {
					ctx->bat_lvl = 100;
				} else {
					ctx->bat_lvl = msg.value;
				}

				break;
			case XM_MSG_BATSTATPIN:
				ctx->bat_stat = msg.value;
				break;
			case XM_MSG_TASKSUSP_REQ:
				/* Switch the display off. */
				MSIM_SH1106_bufClear(display);
				MSIM_SH1106_DisplayOff(display);
				MSIM_SH1106_bufSend(display);
				MSIM_SH1106_Wait(display);

				/*
				 * Block the task indefinitely to wait
				 * for a notification.
				 */
				xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

				/* Switch the display back on. */
				MSIM_SH1106_bufClear(display);
				MSIM_SH1106_DisplayOn(display);
				MSIM_SH1106_bufSend(display);
				MSIM_SH1106_Wait(display);

				/*
				 * Skip the first battery level
				 * messages after awake.
				 */
				bat_lvl_skip = 5;

				break;
			case XM_MSG_KEYBOARD:
				ctx->btn_stat = (xm_btn_state_t) msg.value;
				break;
			default:
				/* Ignore other messages silently. */
				break;
			}
		} else {
			/*
			 * Queue is empty. There is no need to receive
			 * new messages anymore in this frame cycle.
			 */
			break;
		}
	}
}
