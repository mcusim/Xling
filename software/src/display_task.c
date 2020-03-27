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
#include "xling/graphics/xling.h"
#include "xling/graphics/luci.h"

/* Local macros. */
#define SET_BIT(byte, bit)	((byte) |= (1U << (bit)))
#define CLEAR_BIT(byte, bit)	((byte) &= (uint8_t) ~(1U << (bit)))
#define TNAME			"Display Task"
#define STSZ			(configMINIMAL_STACK_SIZE)
#define OLED_DC			PC5		/* OLED Data/Command pin. */
#define OLED_RST		PC6		/* OLED Reset pin. */
#define OLED_CS			PC7		/* OLED Chip Select pin. */
#define OLED_MOSI		PB5		/* OLED SPI pin. */
#define OLED_MISO		PB6		/* OLED SPI pin. */
#define OLED_SCK		PB7		/* OLED SPI pin. */
#define BLINK_DELAY		49
#define LOOK_AT_DELAY		19

/* Local variables. */
static const MSIM_SH1106DrvConf_t _driver_conf = {
	.port_spi = &PORTB,
	.ddr_spi = &DDRB,
	.mosi = OLED_MOSI,
	.miso = OLED_MISO,
	.sck = OLED_SCK,
};
static const MSIM_SH1106Conf_t _display_conf = {
	.rst_port = &PORTC,
	.rst_ddr = &DDRC,
	.cs_port = &PORTC,
	.cs_ddr = &DDRC,
	.dc_port = &PORTC,
	.dc_ddr = &DDRC,
	.rst = OLED_RST,
	.cs = OLED_CS,
	.dc = OLED_DC,
};
static volatile TaskHandle_t _task_handle;

/* Local functions declarations. */
static void	init_timer3(void);
static void	stop_timer3(void);
static void	start_timer3(void);
static void	display_task(void *) __attribute__((noreturn));

int
XG_InitDisplayTask(XG_TaskArgs_t *arg, UBaseType_t prior,
                   TaskHandle_t *task_handle)
{
	BaseType_t stat;
	TaskHandle_t th;
	int rc = 0;

	/*
	 * Configure the display and its driver. We don't have to worry about
	 * interrupts and FreeRTOS scheduler - they shouldn't be active at the
	 * moment.
	 */
	{
		/* Keep CS high. Display isn't selected by default. */
		SET_BIT(PORTC, OLED_CS);

		/* Power On the display. */
		CLEAR_BIT(PORTC, OLED_RST);
		_delay_ms(1);
		SET_BIT(PORTC, OLED_RST);
		_delay_ms(10);

		/* Start the driver for SH1106-based displays. */
		MSIM_SH1106__drvStart(&_driver_conf);

		/* Setup Timer 3 to resume the display task. */
		init_timer3();
	}

	/* Create the display task. */
	stat = xTaskCreate(display_task, TNAME, STSZ, arg, prior, &th);

	if (stat != pdPASS) {
		/* Sleep mode task couldn't be created. */
		rc = 1;
	} else {
		/* Task has been created successfully. */
		if (task_handle != NULL) {
			(*task_handle) = th;
		}
		_task_handle = th;
	}

	return rc;
}

static void
display_task(void *arg)
{
	const XG_TaskArgs_t * const args = (XG_TaskArgs_t *) arg;
	const uint8_t *ptr;
	MSIM_SH1106_t * const display = MSIM_SH1106_Init(&_display_conf);
	uint8_t frame_id = 1;
	uint8_t change_frame = 1;
	uint8_t col = 0;
	uint8_t skip_bat_lvl = 0;
	uint16_t bat_lvl = UINT16_MAX;
	uint16_t bat_stat = 5;
	uint16_t blink_delay = BLINK_DELAY;
	uint16_t look_at_delay = LOOK_AT_DELAY;
	char textbuf[32];
	TickType_t delay;
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

	/* Clear the display. */
	for (uint32_t i = 0; i < 8; i++) {
		MSIM_SH1106_bufClear(display);
		MSIM_SH1106_SetPage(display, (uint8_t) i);
		MSIM_SH1106_SetColumn(display, 2);
		MSIM_SH1106_bufSend(display);

		MSIM_SH1106_bufClear(display);
		for (uint32_t j = 0; j < 128; j++) {
			MSIM_SH1106_bufAppend(display, 0);
		}
		MSIM_SH1106_bufSend(display);
	}

	/* Task loop */
	while (1) {
		/* Remember a moment in time. */
		delay = xTaskGetTickCount();

		/* Receive all of the messages from the queue. */
		while (1) {
			/* Attempt to receive the next message. */
			status = xQueueReceive(args->display_ti.queue_hdl, &msg, 0);

			/* Message has been received. */
			if (status == pdPASS) {
				switch (msg.type) {
				case XG_MSG_BATLVL:
					/* Skip several values initially. */
					if (skip_bat_lvl > 0) {
						skip_bat_lvl--;
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
					/*
					 * Do not resume the task by the timer
					 * before going to sleep.
					 */
					stop_timer3();

					/* Switch the display off. */
					MSIM_SH1106_bufClear(display);
					MSIM_SH1106_DisplayOff(display);
					MSIM_SH1106_bufSend(display);
					MSIM_SH1106_Wait(display);

					/* Let the task to suspend itself. */
					vTaskSuspend(NULL);

					/*
					 * Skip the first battery level messages
					 * after awake.
					 */
					skip_bat_lvl = 5;

					/* Switch the display back on. */
					MSIM_SH1106_bufClear(display);
					MSIM_SH1106_DisplayOn(display);
					MSIM_SH1106_bufSend(display);
					MSIM_SH1106_Wait(display);

					/*
					 * Let's resume the task by the timer
					 * again.
					 */
					start_timer3();

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

		/*
		 * Draw the next animation frame.
		 */

		/* Choose an animation frame. */
		if (change_frame == 1) {
			/* Clean a part of the display. */
//			for (uint32_t i = 0; i < 8; i++) {
//				MSIM_SH1106_bufClear(display);
//				MSIM_SH1106_SetPage(display, (uint8_t) i);
//				MSIM_SH1106_SetColumn(display, col);
//				MSIM_SH1106_bufSend(display);
//
//				MSIM_SH1106_bufClear(display);
//				for (uint32_t k = 0; k < 5; k++) {
//					MSIM_SH1106_bufAppend(display, 0);
//				}
//				MSIM_SH1106_bufSend(display);
//			}

			/* Update a sprite position. */
//			col = (uint8_t)((col >= 130) ? 5 : (col + 5));
			col = 75;

			switch (frame_id) {
			case 0:
				ptr = exy_inbottle_01;
				if (look_at_delay == 0) {
					frame_id = 1;
					look_at_delay = LOOK_AT_DELAY;
				} else {
					look_at_delay--;
				}
				break;
			case 1:
				ptr = exy_inbottle_02;
				if (blink_delay == 0) {
					frame_id++;
				} else {
					blink_delay--;
					if ((1 + (rand()/((RAND_MAX + 1u)/20))) > 19) {
						frame_id = 0;
					}
				}
				break;
			case 2:
				ptr = exy_inbottle_03;
				frame_id++;
				break;
			case 3:
				ptr = exy_inbottle_04;
				frame_id++;
				break;
			case 4:
				ptr = exy_inbottle_05;
				frame_id++;
				break;
			case 5:
				ptr = exy_inbottle_06;
				frame_id++;
				break;
			default:
				ptr = exy_inbottle_02;
				frame_id = 1;
				blink_delay = BLINK_DELAY;
				break;
			}

			/* Draw the next frame. */
			for (uint32_t i = 0; i < 8; i++) {
				MSIM_SH1106_bufClear(display);
				MSIM_SH1106_SetPage(display, (uint8_t) i);
				MSIM_SH1106_SetColumn(display, col);
				MSIM_SH1106_bufSend(display);

				MSIM_SH1106_bufClear(display);
				MSIM_SH1106_bufAppendLast_PF(
				        display, &ptr[i * 37], 37);
				MSIM_SH1106_bufSend(display);
			}
			change_frame = 0;
		} else {
			change_frame = 1;
		}

		if (change_frame == 0) {
			/*
			 * Calculate a delay to receive a message from the queue and
			 * draw an animation frame.
			 */
			delay = xTaskGetTickCount() - delay;

			/* Draw the delay. */
			snprintf(&textbuf[0], sizeof textbuf, "frame: %4lu ms",
			         delay);

			MSIM_SH1106_bufClear(display);
			MSIM_SH1106_SetPage(display, 0);
			MSIM_SH1106_SetColumn(display, 2);
			MSIM_SH1106_bufSend(display);
			MSIM_SH1106_Print(display, &textbuf[0]);

			/* Draw the battery level. */
			snprintf(&textbuf[0], sizeof textbuf, "battery: %3d%%",
			         bat_lvl);

			MSIM_SH1106_bufClear(display);
			MSIM_SH1106_SetPage(display, 1);
			MSIM_SH1106_SetColumn(display, 2);
			MSIM_SH1106_bufSend(display);
			MSIM_SH1106_Print(display, &textbuf[0]);

			/* Draw the battery level. */
			snprintf(&textbuf[0], sizeof textbuf, ((bat_stat == 1) ?
			         "charged:  yes" : "charged:   no"));

			MSIM_SH1106_bufClear(display);
			MSIM_SH1106_SetPage(display, 2);
			MSIM_SH1106_SetColumn(display, 2);
			MSIM_SH1106_bufSend(display);
			MSIM_SH1106_Print(display, &textbuf[0]);
		}

		/* Let the task to suspend itself until the next wake. */
		vTaskSuspend(NULL);
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
init_timer3(void)
{
	/*
	 * Configure Timer 3 to generate a 24 Hz frequency at the Output Compare
	 * Channel A. It can be achieved by dividing the MCU clock frequency
	 * by 8 to get 1.5 MHz, and by 62,500 to get the desired one:
	 *
	 *     12,000,000 / 8 = 1,500,000 Hz
	 *     1,500,000 / 62,500 = 24 Hz
	 */

	/* CTC mode, WDM33:0 = 4 */
	CLEAR_BIT(TCCR3B, WGM33);
	SET_BIT(TCCR3B, WGM32);
	CLEAR_BIT(TCCR3A, WGM31);
	CLEAR_BIT(TCCR3A, WGM30);

	/*
	 * Divide the timer frequency by 62,500 to get 24 Hz at channel A.
	 */
	TCNT3 = 0x00;
	OCR3A = 62499;

	/* Enable Output-Compare interrupt (Channel A). */
	SET_BIT(TIMSK3, OCIE3A);

	/* Start timer, prescaler to 8: CS32:0 = 2. */
	CLEAR_BIT(TCCR3B, CS32);
	SET_BIT(TCCR3B, CS31);
	CLEAR_BIT(TCCR3B, CS30);
}

static void
stop_timer3(void)
{
	uint8_t byte = TCCR3B;

	taskENTER_CRITICAL();

	/* Stop the timer: CS32:0 = 0. */
	CLEAR_BIT(byte, CS32);
	CLEAR_BIT(byte, CS31);
	CLEAR_BIT(byte, CS30);
	TCCR3B = byte;

	taskEXIT_CRITICAL();
}

static void
start_timer3(void)
{
	uint8_t byte = TCCR3B;

	taskENTER_CRITICAL();

	/* Start timer, prescaler to 8: CS32:0 = 2. */
	CLEAR_BIT(byte, CS32);
	SET_BIT(byte, CS31);
	CLEAR_BIT(byte, CS30);
	TCCR3B = byte;

	taskEXIT_CRITICAL();
}

/*
 * A Timer 3 Compare-Match (Channel A) ISR which is used to resume the display
 * updating task.
 */
ISR(TIMER3_COMPA_vect)
{
	/* Resume the suspended task. */
	const BaseType_t yield = xTaskResumeFromISR(_task_handle);

	if (yield == pdTRUE) {
		/*
		 * A context switch should now be performed so the ISR returns
		 * directly to the resumed task. This is because the resumed
		 * task had a priority that was equal to or higher than the task
		 * that is currently in the Running state.
		 */
		portYIELD_FROM_ISR();
	}
}
