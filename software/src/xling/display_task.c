/*-
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This file is part of a firmware for Xling, a tamagotchi-like toy.
 *
 * Copyright (c) 2019 Dmitry Salychev
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
#include "mcusim/drivers/avr-gcc/avr/display/sh1106/sh1106_graphics.h"

/* Xling headers. */
#include "xling/tasks.h"
#include "xling/graphics.h"

/* Local macros. */
#define SET_BIT(byte, bit)	((byte) |= (1U << (bit)))
#define CLEAR_BIT(byte, bit)	((byte) &= (uint8_t) ~(1U << (bit)))
#define TASK_NAME		"Display Task"
#define STACK_SZ		(configMINIMAL_STACK_SIZE)
#define TASK_PERIOD		(42)				/* ms */
#define TASK_DELAY		(pdMS_TO_TICKS(TASK_PERIOD))	/* ticks */

/* Local variables. */
static const MSIM_SH1106DrvConf_t _driver_conf = {
	.port_spi = &PORTB,
	.ddr_spi = &DDRB,
	.mosi = PB5,			/* OLED SPI pin. */
	.miso = PB6,			/* OLED SPI pin. */
	.sck = PB7,			/* OLED SPI pin. */
};
static const MSIM_SH1106Conf_t _display_conf = {
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
static volatile TaskHandle_t _task_handle;
static uint8_t _display_buffer[1024];
static MSIM_SH1106Canvas_t _canvas = {
	.data = &_display_buffer[0],
	.width = 128,
	.height = 64,
	.data_size = 8,
};

/* Local functions declarations. */
static void	display_task(void *) __attribute__((noreturn));

int
XG_InitDisplayTask(XG_TaskArgs_t *arg, UBaseType_t priority,
                   TaskHandle_t *task_handle)
{
	BaseType_t stat;
	int rc = 0;

	/*
	 * Configure the display and its driver. We don't have to worry about
	 * interrupts and FreeRTOS scheduler - they shouldn't be active at the
	 * moment.
	 */
	{
		/* Keep CS high. Display isn't selected by default. */
		SET_BIT(PORTC, _display_conf.cs);

		/* Power On the display. */
		CLEAR_BIT(PORTC, _display_conf.rst);
		_delay_ms(1);
		SET_BIT(PORTC, _display_conf.rst);
		_delay_ms(10);

		/* Start the driver for SH1106-based displays. */
		MSIM_SH1106__drvStart(&_driver_conf);
	}

	/* Create the display task. */
	stat = xTaskCreate(display_task, TASK_NAME, STACK_SZ,
	                   arg, priority, task_handle);

	if (stat != pdPASS) {
		/* Sleep mode task couldn't be created. */
		rc = 1;
	} else {
		/* Task has been created successfully. */
		_task_handle = (*task_handle);
	}

	return rc;
}

static void
display_task(void *arg)
{
	const XG_TaskArgs_t * const args = (XG_TaskArgs_t *) arg;
	MSIM_SH1106_t * const display = MSIM_SH1106_Init(&_display_conf);
	uint8_t bat_lvl_skip = 0;
	uint16_t bat_lvl = UINT16_MAX;
	uint16_t bat_stat = 5;
	uint16_t x_base = 0;
	uint16_t y_base = 0;
	TickType_t delay;
	TickType_t last_wake_time;
	BaseType_t status;
	XG_Msg_t msg;

	/* Use current time as seed for random generator. */
	srand((unsigned int) time(NULL));

	/* Setup an OLED display. */
	MSIM_SH1106_DisplayOff(display);
	MSIM_SH1106_SetContrast(display, 0x75);
	MSIM_SH1106_DisplayNormal(display);
	MSIM_SH1106_SetScanDirection(display, 0);
	MSIM_SH1106_DisplayOn(display);
	MSIM_SH1106_bufSend(display);

	/* Remember the last wake time. */
	last_wake_time = xTaskGetTickCount();

	/* Task loop */
	while (1) {
		/* Wait for the next task tick. */
		vTaskDelayUntil(&last_wake_time, TASK_DELAY);

		/* Remember a moment in time. */
		delay = xTaskGetTickCount();

		/* Receive all of the messages from the queue. */
		while (1) {
			/* Attempt to receive the next message. */
			status = xQueueReceive(args->display_info.queue_handle,
			                       &msg, 0);

			/* Message has been received. */
			if (status == pdPASS) {
				switch (msg.type) {
				case XG_MSG_BATLVL:
					/* Skip several values initially. */
					if (bat_lvl_skip > 0) {
						bat_lvl_skip--;
					} else if (msg.value > 100) {
						bat_lvl = 100;
					} else {
						bat_lvl = msg.value;
					}

					break;
				case XG_MSG_BATSTATPIN:
					bat_stat = msg.value;
					break;
				case XG_MSG_TASKSUSP_REQ:
					/* Switch the display off. */
					MSIM_SH1106_bufClear(display);
					MSIM_SH1106_DisplayOff(display);
					MSIM_SH1106_bufSend(display);
					MSIM_SH1106_Wait(display);

					/*
					 * Block the task indefinitely to wait
					 * for a notification.
					 */
					xTaskNotifyWait(0, 0, NULL,
					                portMAX_DELAY);

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

		/* Prepare random coordinates. */
		x_base = 1 + rand() / ((RAND_MAX + 1u) / _canvas.width);
		y_base = 1 + rand() / ((RAND_MAX + 1u) / _canvas.height);

		/* Draw image on a canvas at random coordinates. */
		MSIM_SH1106_Draw_PF(&_canvas, &XG_IMG_EXY_IN_BOTTLE_02,
		                    x_base, y_base);

		/* Transfer canvas buffer to the display. */
		for (uint32_t i = 0; i < 8; i++) {
			MSIM_SH1106_bufClear(display);
			MSIM_SH1106_SetPage(display, (uint8_t) i);
			MSIM_SH1106_SetColumn(display, 2);
			MSIM_SH1106_bufSend(display);

			MSIM_SH1106_bufClear(display);
			MSIM_SH1106_bufAppendLast(
			        display, &_display_buffer[i * 128],
			        128);
			MSIM_SH1106_bufSend(display);
		}

		/*
		 * Calculate a delay to receive a message from the queue and
		 * draw an animation frame.
		 */
		delay = xTaskGetTickCount() - delay;
	}

	/*
	 * The task must be deleted before reaching the end of its
	 * implementating function.
	 *
	 * NOTE: Shouldn't reach this point!
	 */
	vTaskDelete(NULL);
}
