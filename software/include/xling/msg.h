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
#ifndef XG_MSG_H_
#define XG_MSG_H_ 1

/*
 * Types of the messages transmitted via queues.
 */
typedef enum XG_MsgType_t {
	XG_MSG_BATLVL,			/* Current battery level, in %. */
	XG_MSG_BATSTATPIN,		/* Battery status pin value. */
	XG_MSG_BATCHARGING,		/* Battery started charging. */
	XG_MSG_BATSTOPCHARGING,		/* Battery stopped charging. */

	/*
	 * Messages of this type are intended to be used by the Sleep Mode task
	 * and all of the others in order to prepare the device to be switched
	 * to the sleep mode.
	 *
	 * It's done by suspending all of the tasks by the Sleep Mode task
	 * in order to let Idle to be the only one which is able to run.
	 */
	XG_MSG_TASKSUSP_REQ,		/* Request to suspend the task. */

	XG_MSG_KEYBOARD,		/* Keyboard events. */
} XG_MsgType_t;

/*
 * Message to be transmitted via queues.
 */
typedef struct XG_Msg_t {
	uint16_t	value;		/* Message value, depends on a type. */
	XG_MsgType_t	type;		/* Type of the message. */
} XG_Msg_t;

/*
 * State of the buttons.
 *
 * NOTE: Button states are sent by the keyboard task as message values
 * together with the keyboard message type.
 */
typedef enum XG_ButtonState_e {
	XG_BTN_LEFT_RELEASED = 75,
	XG_BTN_LEFT_PRESSED,
	XG_BTN_LEFT_HOLD,
	XG_BTN_CENTER_RELEASED,
	XG_BTN_CENTER_PRESSED,
	XG_BTN_CENTER_HOLD,
	XG_BTN_RIGHT_RELEASED,
	XG_BTN_RIGHT_PRESSED,
	XG_BTN_RIGHT_HOLD,
	/* Additional states */
	XG_BTN_LEFT_DBLPRESSED,
	XG_BTN_CENTER_DBLPRESSED,
	XG_BTN_RIGHT_DBLPRESSED,
} XG_ButtonState_e;

#endif /* XG_MSG_H_ */
