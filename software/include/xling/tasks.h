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
#ifndef XG_TASKS_H_
#define XG_TASKS_H_ 1

/*
 * The Xling tasks for FreeRTOS scheduler are declared here. You'll find helper
 * data types (like "XG_TaskArgs_t") in this file also.
 *
 * NOTE: A list of tasks is a good point for studing routines available in the
 * firmware and helper types - to see how they communicate to each other.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

typedef struct XG_TaskInfo_t {
	TaskHandle_t task_hdl;
	QueueHandle_t queue_hdl;
} XG_TaskInfo_t;

/*
 * A "task arguments" type which is passed to all of the Xling tasks.
 */
typedef struct XG_TaskArgs_t {
	XG_TaskInfo_t	display_ti;	/* Display task info. */
	XG_TaskInfo_t	batmon_ti;	/* Battery monitor task info. */
	XG_TaskInfo_t	slpmod_ti;	/* Sleep mode task info. */
} XG_TaskArgs_t;

/*
 * Types of the messages transmitted via queues.
 */
typedef enum XG_MsgType_t {
	XG_MSG_BATLVL,			/* Current battery level, in %. */
	XG_MSG_BATSTATPIN,		/* Battery status pin value. */
	XG_MSG_BATCHARGING,		/* Battery started charging. */
	XG_MSG_BATSTOPCHARGING,		/* Battery stopped charging. */

	/*
	 * These messages are intended to be used by the SleepModeTask and all
	 * of the other tasks in order to prepare the device to be switched to
	 * sleep mode.
	 *
	 * It's done by suspending all of the tasks by the SleeModeTask in order
	 * to let the Idle task to be the only one which is able to run.
	 */
	XG_MSG_TASKSUSP_REQ,		/* A request to suspend the task. */
} XG_MsgType_t;

/*
 * Message to be transmitted via queues.
 */
typedef struct XG_Msg_t {
	uint16_t	value;		/* Message value, depends on a type. */
	XG_MsgType_t	type;		/* Type of the message. */
} XG_Msg_t;

/* Functions to initialize Xling tasks for the FreeRTOS scheduler. */
int	XG_InitDisplayTask(XG_TaskArgs_t *, UBaseType_t, TaskHandle_t *);
int	XG_InitBatteryMonitorTask(XG_TaskArgs_t *, UBaseType_t, TaskHandle_t *);
int	XG_InitSleepModeTask(XG_TaskArgs_t *, UBaseType_t, TaskHandle_t *);

#endif /* XG_TASKS_H_ */
