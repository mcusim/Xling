/*-
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This file is part of a firmware for Xling, a tamagotchi-like toy.
 *
 * Copyright (c) 2020 Dmitry Salychev
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
#ifndef XLING_MSG_H_
#define XLING_MSG_H_ 1

/* Types of the messages transmitted between tasks. */
typedef enum xm_msg_type_t {
	XM_MSG_BATLVL,			/* Current battery level, in %. */
	XM_MSG_BATSTATPIN,		/* Battery status pin value. */
	XM_MSG_BATCHARGING,		/* Battery started charging. */
	XM_MSG_BATSTOPCHARGING,		/* Battery stopped charging. */

	/*
	 * Messages of this type are intended to be used by the Sleep Mode task
	 * and all of the others in order to prepare the device to be switched
	 * to the sleep mode.
	 *
	 * It's done by suspending all of the tasks by the Sleep Mode task
	 * in order to let Idle to be the only one which is able to run.
	 */
	XM_MSG_TASKSUSP_REQ,		/* Request to suspend the task. */

	XM_MSG_KEYBOARD,		/* Keyboard events. */
} xm_msg_type_t;

/* Message to be transmitted between tasks. */
typedef struct xm_msg_t {
	uint16_t	value;		/* Message value, depends on a type. */
	xm_msg_type_t	type;		/* Type of the message. */
} xm_msg_t;

/* State of a button. */
typedef enum xm_btn_state_t {
	XM_BTN_LEFT_RELEASED = 75,
	XM_BTN_LEFT_PRESSED,
	XM_BTN_LEFT_HOLD,
	XM_BTN_CENTER_RELEASED,
	XM_BTN_CENTER_PRESSED,
	XM_BTN_CENTER_HOLD,
	XM_BTN_RIGHT_RELEASED,
	XM_BTN_RIGHT_PRESSED,
	XM_BTN_RIGHT_HOLD,
	/* Additional states */
	XM_BTN_LEFT_DBLPRESSED,
	XM_BTN_CENTER_DBLPRESSED,
	XM_BTN_RIGHT_DBLPRESSED,
} xm_btn_state_t;

#endif /* XLING_MSG_H_ */
