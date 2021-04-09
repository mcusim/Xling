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
#include <avr/io.h>

/*
 * A task to process keyboard events.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "xling/tasks.h"
#include "xling/msg.h"

/* Local macros. */
#define TASK_NAME		"Keyboard Task"
#define STACK_SIZE		(configMINIMAL_STACK_SIZE)
#define TASK_PERIOD		(10)				/* ms */
#define TASK_DELAY		(pdMS_TO_TICKS(TASK_PERIOD))	/* ticks */
#define NO_DELAY		(0)

/* Local variables. */
static xm_btn_state_t _keyboard[] = {
	XM_BTN_LEFT_RELEASED,   /* 0 - Left button. */
	XM_BTN_CENTER_RELEASED, /* 1 - Center button. */
	XM_BTN_RIGHT_RELEASED,  /* 2 - Right button. */
};
static TaskHandle_t _task_handle;

/* Local functions. */
static void keyboard_task(void *) __attribute__((noreturn));

int
xt_init_keyboard(xt_args_t *args, UBaseType_t prio, TaskHandle_t *task_handle)
{
	BaseType_t status;
	int rc = 0;

	/* Create the keyboard task. */
	status = xTaskCreate(keyboard_task, TASK_NAME, STACK_SIZE,
	    args, prio, task_handle);

	if (status != pdPASS) {
		/* Task couldn't be created. */
		rc = 1;
	} else {
		/* Task has been created successfully. */
		_task_handle = (*task_handle);
	}

	return rc;
}

static void
keyboard_task(void *arg)
{
	const xt_args_t * const args = (xt_args_t *) arg;
	const QueueHandle_t display_queue = args->display_info.queue_handle;
	const QueueHandle_t sleep_queue = args->sleep_info.queue_handle;
	const QueueHandle_t keyboard_queue = args->keyboard_info.queue_handle;
	BaseType_t status;
	TickType_t last_wake_time;
	xm_msg_t msg;

	/* Initialize the last wake time. */
	last_wake_time = xTaskGetTickCount();

	/* Task loop */
	while (1) {
		/* Wait for the next task tick. */
		vTaskDelayUntil(&last_wake_time, TASK_DELAY);

		/* Receive all of the messages from the queue. */
		while (1) {
			/*
			 * Attempt to receive a message from the queue.
			 *
			 * NOTE: This task won't be blocked because its main
			 * purpose is to generate the keyboard events for the
			 * other tasks.
			 */
			status = xQueueReceive(keyboard_queue, &msg, 0);

			/* Message has been received. */
			if (status == pdPASS) {
				switch (msg.type) {
				case XM_MSG_TASKSUSP_REQ:
					/*
					 * Block the task indefinitely to wait
					 * for a notification.
					 */
					xTaskNotifyWait(0, 0, NULL,
					                portMAX_DELAY);
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

		/* Scan buttons and send messages. */

		/* Read left button, PD3. */
		if (_keyboard[0] == XM_BTN_LEFT_RELEASED) {
			if (((PIND & 8U) >> 3) == 0U) {
				_keyboard[0] = XM_BTN_LEFT_PRESSED;

				msg.type = XM_MSG_KEYBOARD;
				msg.value = XM_BTN_LEFT_PRESSED;

				status = xQueueSendToBack(
				        display_queue, &msg, 0);
				status = xQueueSendToBack(
				        sleep_queue, &msg, 0);
			}
		} else if (_keyboard[0] == XM_BTN_LEFT_PRESSED) {
			if (((PIND & 8U) >> 3) == 1U) {
				_keyboard[0] = XM_BTN_LEFT_RELEASED;

				msg.type = XM_MSG_KEYBOARD;
				msg.value = XM_BTN_LEFT_RELEASED;

				status = xQueueSendToBack(
				        display_queue, &msg, 0);
				status = xQueueSendToBack(
				        sleep_queue, &msg, 0);
			}
		} else {
			/* Nothing to do in this case. */
		}

		/* Read center button, PD2. */
		if (_keyboard[1] == XM_BTN_CENTER_RELEASED) {
			if (((PIND & 4U) >> 2) == 0U) {
				_keyboard[1] = XM_BTN_CENTER_PRESSED;

				msg.type = XM_MSG_KEYBOARD;
				msg.value = XM_BTN_CENTER_PRESSED;

				status = xQueueSendToBack(
				        display_queue, &msg, 0);
				status = xQueueSendToBack(
				        sleep_queue, &msg, 0);
			}
		} else if (_keyboard[1] == XM_BTN_CENTER_PRESSED) {
			if (((PIND & 4U) >> 2) == 1U) {
				_keyboard[1] = XM_BTN_CENTER_RELEASED;

				msg.type = XM_MSG_KEYBOARD;
				msg.value = XM_BTN_CENTER_RELEASED;

				status = xQueueSendToBack(
				        display_queue, &msg, 0);
				status = xQueueSendToBack(
				        sleep_queue, &msg, 0);
			}
		} else {
			/* Nothing to do in this case. */
		}

		/* Read right button, PB2. */
		if (_keyboard[2] == XM_BTN_RIGHT_RELEASED) {
			if (((PINB & 4U) >> 2) == 0U) {
				_keyboard[2] = XM_BTN_RIGHT_PRESSED;

				msg.type = XM_MSG_KEYBOARD;
				msg.value = XM_BTN_RIGHT_PRESSED;

				status = xQueueSendToBack(
				        display_queue, &msg, 0);
				status = xQueueSendToBack(
				        sleep_queue, &msg, 0);
			}
		} else if (_keyboard[2] == XM_BTN_RIGHT_PRESSED) {
			if (((PINB & 4U) >> 2) == 1U) {
				_keyboard[2] = XM_BTN_RIGHT_RELEASED;

				msg.type = XM_MSG_KEYBOARD;
				msg.value = XM_BTN_RIGHT_RELEASED;

				status = xQueueSendToBack(
				        display_queue, &msg, 0);
				status = xQueueSendToBack(
				        sleep_queue, &msg, 0);
			}
		} else {
			/* Nothing to do in this case. */
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
