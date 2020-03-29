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
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * Sleep Mode task is supposed to monitor the device activity during a selected
 * period of time and suspend all of the tasks if the device hasn't been used.
 *
 * NOTE: External interrupts of INT0:2 are occupied here. They're necessary to
 * resume the task after an awake from the MCU sleep in Tickelss Idle mode of
 * FreeRTOS. See for details:
 *
 *     https://forums.freertos.org/t/awake-from-sleep-tickless-idle-mode
 *
 * NOTE: An interrupt of the 8-bit Timer 0 and its Compare-Match channel A will
 * be occupied by the task.
 */

/* FreeRTOS headers. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Xling headers. */
#include "xling/tasks.h"

/* Local macros. */
#define TNAME			"Sleep Mode Task"
#define SET_BIT(byte, bit)	((byte) |= (1U << (bit)))
#define CLEAR_BIT(byte, bit)	((byte) &= (uint8_t) ~(1U << (bit)))
#define STSZ			(configMINIMAL_STACK_SIZE)
#define TIMEOUT_MS		(29000)
#define TIMEOUT_TICKS		((uint16_t)(TIMEOUT_MS / 2.666667))

/* Local data types. */
typedef enum wake_source_e {
	WAKE_FROM_TIMER,
	WAKE_FROM_EXTINT,
} wake_source_e;

/* Local variables. */
static volatile TaskHandle_t _task_handle;

/* Number of the timer ticks passed. */
static volatile uint16_t _tick_val = 0;
/* Task shouldn't be woken from external interrupts initially. */
static volatile uint8_t _task_woken_from_extint = 1;
/* Task should be woken from its timer initially. */
static volatile uint8_t _task_woken_from_timer = 0;
static volatile wake_source_e _wake_source = WAKE_FROM_TIMER;
/* END Local variables. */

/* Local functions. */
static void	sleepmod_task(void *) __attribute__((noreturn));
static void	ask_tasks_wait(const XG_TaskArgs_t *arg);
static void	notify_tasks(const XG_TaskArgs_t *arg);
static void	init_timer0(void);
static void	stop_timer0(void);
static void	start_timer0(void);
static void	enable_extint(void);
static void	disable_extint(void);

int
XG_InitSleepModeTask(XG_TaskArgs_t *arg, UBaseType_t prior,
                     TaskHandle_t *task_handle)
{
	BaseType_t stat;
	TaskHandle_t th;
	int rc = 0;

	/*
	 * Configure Timer 0 and its Compare Match channel A to wake the
	 * Sleep Mode task after a timeout. We don't have to worry about
	 * interrupts and FreeRTOS scheduler - they shouldn't be active at the
	 * moment.
	 */
	init_timer0();

	/* Create the sleep mode task. */
	stat = xTaskCreate(sleepmod_task, TNAME, STSZ, arg, prior, &th);

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
sleepmod_task(void *arg)
{
	const XG_TaskArgs_t * const args = (XG_TaskArgs_t *) arg;

	/* Task loop */
	while (1) {
		/*
		 * Block the task indefinitely to wait for a notification
		 * (interrupt from a timer or an external one).
		 */
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

		switch (_wake_source) {
		case WAKE_FROM_TIMER:
			/* Stop the Timer 0. */
			stop_timer0();

			/* Enable external interrupts. */
			enable_extint();

			/* Reset the timer ticks. */
			_tick_val = 0;

			/*
			 * Timeout expired. Let's ask all of the tasks to
			 * suspend.
			 */
			ask_tasks_wait(args);

			/* Toggle the switch. */
			_task_woken_from_extint = 0;

			break;
		case WAKE_FROM_EXTINT:
			/* Disable external interrupts. */
			disable_extint();

			/* Start the Timer 0. */
			start_timer0();

			/* Reset the timer ticks. */
			_tick_val = 0;

			/*
			 * There was an external interrupt. Let's notify all of
			 * the tasks.
			 */
			notify_tasks(args);

			/* Toggle the switch. */
			_task_woken_from_timer = 0;

			break;
		default:
			break;
		}
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
ask_tasks_wait(const XG_TaskArgs_t *arg)
{
	const QueueHandle_t q[] = {
		arg->batmon_ti.queue_hdl,
		arg->display_ti.queue_hdl,
	};
	const size_t qnum = sizeof(q) / sizeof(q[0]);
	XG_Msg_t msg = {
		.type = XG_MSG_TASKSUSP_REQ
	};

	/* Send suspend requests to the tasks. */
	for (size_t i = 0; i < qnum; i++) {
		xQueueSendToBack(q[i], &msg, portMAX_DELAY);
	}
}

static void
notify_tasks(const XG_TaskArgs_t *arg)
{
	const TaskHandle_t t[] = {
		arg->batmon_ti.task_hdl,
		arg->display_ti.task_hdl,
	};
	const size_t tnum = sizeof(t) / sizeof(t[0]);

	/* Resume all of the tasks. */
	for (size_t i = 0; i < tnum; i++) {
		xTaskNotify(t[i], 0, eNoAction);
	}
}

static void
enable_extint(void)
{
	taskENTER_CRITICAL();

	/*
	 * Clear Interrupt Enable bits in the EIMSK register to disable external
	 * INTn interrupts.
	 */
	CLEAR_BIT(EIMSK, INT2);
	CLEAR_BIT(EIMSK, INT1);
	CLEAR_BIT(EIMSK, INT0);

	/*
	 * Adjust configuration flags to generate external interrupts
	 * asynchronously on the falling edges of the INTn pins.
	 *
	 * ISCn1    ISCn0    Description
	 * -----------------------------
	 * 0        0        The low level of INTn generates an IRQ.
	 * 0        1        Any edge of INTn generates an IRQ (async).
	 * 1        0        The falling edge of INTn generates an IRQ (async).
	 * 1        1        The rising edge of INTn generates and IRQ (async).
	 */
	SET_BIT(EICRA, ISC21);
	CLEAR_BIT(EICRA, ISC20);
	SET_BIT(EICRA, ISC11);
	CLEAR_BIT(EICRA, ISC10);
	SET_BIT(EICRA, ISC01);
	CLEAR_BIT(EICRA, ISC00);

	/* Clear INTn interrupt flags in the EIFR register. */
	SET_BIT(EIFR, INTF2);
	SET_BIT(EIFR, INTF1);
	SET_BIT(EIFR, INTF0);

	/* Re-enable external INTn interrupts. */
	SET_BIT(EIMSK, INT2);
	SET_BIT(EIMSK, INT1);
	SET_BIT(EIMSK, INT0);

	taskEXIT_CRITICAL();
}

static void
disable_extint(void)
{
	taskENTER_CRITICAL();

	/* Clear INTn interrupt flags in the EIFR register. */
	SET_BIT(EIFR, INTF2);
	SET_BIT(EIFR, INTF1);
	SET_BIT(EIFR, INTF0);

	/*
	 * Clear Interrupt Enable bits in the EIMSK register to disable external
	 * INTn interrupts.
	 */
	CLEAR_BIT(EIMSK, INT2);
	CLEAR_BIT(EIMSK, INT1);
	CLEAR_BIT(EIMSK, INT0);

	taskEXIT_CRITICAL();
}

static void
init_timer0(void)
{
	/*
	 * Configure Timer 0 to generate a 375 Hz frequency at the Output
	 * Compare Channel A. It can be achieved by dividing the MCU clock
	 * frequency by 256 to get 46.875 kHz, and by 125 to get the
	 * desired one:
	 *
	 *     12,000,000 / 256 = 46.875 kHz
	 *     46,875 / 125 = 375 Hz
	 */

	/* CTC mode, WDM02:0 = 2 */
	CLEAR_BIT(TCCR0B, WGM02);
	SET_BIT(TCCR0A, WGM01);
	CLEAR_BIT(TCCR0A, WGM00);

	/*
	 * Divide the timer frequency by 62,500 to get 24 Hz at channel A.
	 */
	TCNT0 = 0x00;
	OCR0A = 124;

	/* Enable Output-Compare interrupt (Channel A). */
	SET_BIT(TIMSK0, OCIE0A);

	/* Start timer, prescaler to 256: CS02:0 = 4. */
	SET_BIT(TCCR0B, CS02);
	CLEAR_BIT(TCCR0B, CS01);
	CLEAR_BIT(TCCR0B, CS00);
}

static void
stop_timer0(void)
{
	taskENTER_CRITICAL();

	/* Stop the timer: CS02:0 = 0. */
	CLEAR_BIT(TCCR0B, CS02);
	CLEAR_BIT(TCCR0B, CS01);
	CLEAR_BIT(TCCR0B, CS00);

	taskEXIT_CRITICAL();
}

static void
start_timer0(void)
{
	taskENTER_CRITICAL();

	/* Start timer, prescaler to 256: CS02:0 = 4. */
	SET_BIT(TCCR0B, CS02);
	CLEAR_BIT(TCCR0B, CS01);
	CLEAR_BIT(TCCR0B, CS00);

	taskEXIT_CRITICAL();
}

ISR(TIMER0_COMPA_vect)
{
	BaseType_t higher_prior_task_woken = pdFALSE;

	if (_tick_val == (TIMEOUT_TICKS - 1)) {
		/* Reset the timer ticks. */
		_tick_val = 0;

		if (_task_woken_from_timer == 0) {
			/* Set the task wake source. */
			_wake_source = WAKE_FROM_TIMER;

			/* Toggle the switch. */
			_task_woken_from_timer = 1;

			xTaskNotifyFromISR(_task_handle, 0, eNoAction,
			                   &higher_prior_task_woken);

			if (higher_prior_task_woken == pdTRUE) {
				/*
				 * A context switch should now be performed so
				 * the ISR returns directly to the resumed task.
				 *
				 * This is because the resumed task had a
				 * priority that was equal to or higher than
				 * the task that is currently in the Running
				 * state.
				 */
				portYIELD_FROM_ISR();
			}
		}
	} else {
		_tick_val++;
	}
}

ISR(INT0_vect)
{
	BaseType_t higher_prior_task_woken = pdFALSE;

	if (_task_woken_from_extint == 0) {
		/* Set the task wake source. */
		_wake_source = WAKE_FROM_EXTINT;

		/* Toggle the switch. */
		_task_woken_from_extint = 1;

		xTaskNotifyFromISR(_task_handle, 0, eNoAction,
		                   &higher_prior_task_woken);

		if (higher_prior_task_woken == pdTRUE) {
			/*
			 * A context switch should now be performed so the
			 * ISR returns directly to the resumed task.
			 *
			 * This is because the resumed task had a priority
			 * that was equal to or higher than the task
			 * that is currently in the Running state.
			 */
			portYIELD_FROM_ISR();
		}
	}
}

ISR(INT1_vect, ISR_ALIASOF(INT0_vect));
ISR(INT2_vect, ISR_ALIASOF(INT0_vect));
