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
#include <limits.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * A battery monitor task. It allows to estimate a power left in the battery in
 * order to prevent deep discharges and overcharges.
 *
 * This task configures ADC interrupt, calculates the battery power left and
 * battery status (charging or not), and informs the display task via its queue.
 *
 * NOTE: The ADC interrupt will be occupied by the task.
 */

/* FreeRTOS headers. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Xling headers. */
#include "xling/tasks.h"

/* Local macros. */
#define SET_BIT(byte, bit)	((byte) |= (1U << (bit)))
#define CLEAR_BIT(byte, bit)	((byte) &= (uint8_t) ~(1U << (bit)))
#define TNAME			"Battery Monitor Task"
#define STSZ			(configMINIMAL_STACK_SIZE)
#define BAT_MAX			(700) /* Raw ADC value measured manually. */
#define BAT_MIN			(545) /* Raw ADC value measured manually. */
#define BAT_PERCENT(v)		((v)-BAT_MIN)/((BAT_MAX-BAT_MIN)/100.0)

/* Local variables. */
static volatile uint16_t _bat_lvl;	/* Raw battery voltage from ADC. */
static volatile uint8_t _bat_stat;	/* Battery status pin value. */
static volatile TaskHandle_t _task_handle;

/* Local functions. */
static void init_adc(void);
static void batmon_task(void *) __attribute__((noreturn));

int
XG_InitBatteryMonitorTask(XG_TaskArgs_t *arg, UBaseType_t prior,
                          TaskHandle_t *task_handle)
{
	BaseType_t stat;
	TaskHandle_t th;
	int rc = 0;

	/*
	 * Configure ADC to measure a battery voltage and sample the battery
	 * status pin. We don't have to worry about interrupts and FreeRTOS
	 * scheduler - they shouldn't be active at the moment.
	 */
	init_adc();

	/* Create the battery monitor task. */
	stat = xTaskCreate(batmon_task, TNAME, STSZ, arg, prior, &th);

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
batmon_task(void *arg)
{
	const XG_TaskArgs_t * const args = (XG_TaskArgs_t *) arg;
	XG_Msg_t msg;
	BaseType_t status;

	/* Task loop */
	while (1) {
		/* Receive all of the messages from the queue. */
		while (1) {
			/*
			 * Attempt to receive a message from the queue.
			 *
			 * NOTE: This task won't be blocked because its main
			 * purpose is to control the battery voltage and let
			 * the other tasks know about the actual values.
			 */
			status = xQueueReceive(args->batmon_ti.queue_hdl,
			                       &msg, 0);

			/* Message has been received. */
			if (status == pdPASS) {
				switch (msg.type) {
				case XG_MSG_TASKSUSP_REQ:
					/* Let the task to suspend itself. */
					vTaskSuspend(NULL);
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

		/* Send the battery level message. */
		msg.type = XG_MSG_BATLVL;
		msg.value = BAT_PERCENT(_bat_lvl);
		status = xQueueSendToBack(args->display_ti.queue_hdl, &msg,
		                          portMAX_DELAY);

		if (status != pdPASS) {
			/*
			 * We weren't able to send a message to the queue, so
			 * give up and start a new messages sending iteration
			 * again.
			 */
			continue;
		}

		/* Send the battery status pin message. */
		msg.type = XG_MSG_BATSTATPIN;
		msg.value = _bat_stat;
		status = xQueueSendToBack(args->display_ti.queue_hdl, &msg,
		                          portMAX_DELAY);

		if (status != pdPASS) {
			/*
			 * We weren't able to send a message to the queue, so
			 * give up and start a new messages sending iteration
			 * again.
			 */
			continue;
		}

		/* Be silent for some time. */
		vTaskDelay(pdMS_TO_TICKS(100));
	}

	/*
	 * The task must be deleted before reaching the end of its
	 * implementating function.
	 *
	 * NOTE: This point shouldn't be reached.
	 */
	vTaskDelete(NULL);
}

static void
init_adc(void)
{
	/* Select Vref = 1.1 V */
	SET_BIT(ADMUX, REFS1);
	CLEAR_BIT(ADMUX, REFS0);

	/*
	 * ADC clock frequency should be between 50 kHz and 200 kHz to achieve
	 * maximum resolution. Let's set an ADC clock prescaler to 64 at 12 MHz
	 * MCU clock frequency:
	 *
	 *     12,000,000 / 64 = 187.5 kHz
	 */
	SET_BIT(ADCSRA, ADPS2);
	SET_BIT(ADCSRA, ADPS1);
	CLEAR_BIT(ADCSRA, ADPS0);

	/*
	 * Select ADC3 as a single ended input.
	 */
	CLEAR_BIT(ADMUX, MUX4);
	CLEAR_BIT(ADMUX, MUX3);
	CLEAR_BIT(ADMUX, MUX2);
	SET_BIT(ADMUX, MUX1);
	SET_BIT(ADMUX, MUX0);

	/*
	 * Select a Free Running mode. It means that the ADC Interrupt Flag
	 * will be used as a trigger source and a new conversion will start
	 * as soon as the ongoing conversion has finished.
	 *
	 * NOTE: Such a continues monitoring of the battery voltage and status
	 * isn't necessary in most cases and may be changed in future to save
	 * power, for instance.
	 */
	CLEAR_BIT(ADCSRB, ADTS2);
	CLEAR_BIT(ADCSRB, ADTS1);
	CLEAR_BIT(ADCSRB, ADTS0);

	/*
	 * Enable ADC, its interrupt and auto trigger. Configuration is
	 * performed within a critical section, so we won't need to worry about
	 * interrupts here.
	 */
	SET_BIT(ADCSRA, ADEN);
	SET_BIT(ADCSRA, ADIE);
	SET_BIT(ADCSRA, ADATE);

	/* Run the first conversion. */
	SET_BIT(ADCSRA, ADSC);
}

ISR(ADC_vect)
{
	const uint16_t lvl_low = ADCL;
	const uint16_t lvl_high = (ADCH << 8) & 0x0300;

	/* Sample the battery voltage level. */
	_bat_lvl = (uint16_t)((lvl_high) | (lvl_low));

	/* Sample the battery status pin. */
	_bat_stat = PINA & 1U;
}
