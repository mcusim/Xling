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
#ifndef XLING_TASKS_H_
#define XLING_TASKS_H_ 1

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

/* Task information */
typedef struct xt_info_t {
	TaskHandle_t	task_handle;
	QueueHandle_t	queue_handle;
} xt_info_t;

/* Arguments to initialize a task */
typedef struct xt_args_t {
	xt_info_t	display_info;
	xt_info_t	battery_info;
	xt_info_t	sleep_info;
	xt_info_t	keyboard_info;
} xt_args_t;

/* Functions to initialize Xling tasks for the FreeRTOS scheduler. */
int xt_init_display(xt_args_t *args, UBaseType_t prio, TaskHandle_t *h);
int xt_init_battery_monitor(xt_args_t *args, UBaseType_t prio, TaskHandle_t *h);
int xt_init_sleep_mode(xt_args_t *args, UBaseType_t prio, TaskHandle_t *h);
int xt_init_keyboard(xt_args_t *args, UBaseType_t prio, TaskHandle_t *h);

#endif /* XLING_TASKS_H_ */
