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
#define F_CPU			12000000UL	/* CPU 12 MHz */

#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>

/*
 * Entry point of the Xling firmware.
 *
 * An initial configuration of the microcontroller will be performed. Tasks and
 * FreeRTOS scheduler will be created and started here.
 *
 * NOTE: An interrupt of the 16-bit Timer 1 and its compare-match channel A is
 * used to generate a tick interrupt for the FreeRTOS scheduler.
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

/* Local macros. */
#define SET_BIT(byte, bit)	((byte) |= (1U << (bit)))
#define CLEAR_BIT(byte, bit)	((byte) &= (uint8_t) ~(1U << (bit)))
#define IS_SET(byte, bit)	(((byte) & (1U << (bit))) >> (bit))

/* Local variables. */
static XG_TaskArgs_t _display_args;
static XG_TaskArgs_t _batmon_args;
static XG_TaskArgs_t _slpmod_args;
static uint8_t _mcusr_mirror __attribute__ ((section (".noinit")));

/* Local functions declarations. */
static void disable_wdt(void)
	__attribute__((naked))
	__attribute__((section(".init3")));

/* Entry point. */
int
main(void)
{
	TaskHandle_t *th;
	int rc = 0;

	/* Configure PORTC pins as output. */
	DDRC = 0xFF;

	/* Configure PORTA pins as input. */
	DDRA = 0x00;
	PORTA = 0x00;

	/*
	 * Pull PB4 (SS, Slave Select bit) high to ensure Master SPI operation.
	 *
	 * PB4 -> as input pin, write logic one.
	 */
	CLEAR_BIT(DDRB, PB4);
	SET_BIT(PORTB, PB4);

	/* Initialize the tasks arguments. */
	_display_args.display_ti.queue_hdl = xQueueCreate(3, sizeof(XG_Msg_t)),
	_display_args.batmon_ti.queue_hdl = xQueueCreate(3, sizeof(XG_Msg_t)),
	_display_args.slpmod_ti.queue_hdl = xQueueCreate(3, sizeof(XG_Msg_t)),
	_batmon_args = _display_args;
	_slpmod_args = _display_args;

	/* Create and initialize Xling tasks. */
	do {
		th = &_display_args.display_ti.task_hdl;
		rc = XG_InitDisplayTask(&_display_args, 1, th);
		if (rc != 0) {
			/* Task couldn't be initialized successfully. */
			break;
		} else {
			/* Copy the task handler. */
			_batmon_args.display_ti.task_hdl = *th;
			_slpmod_args.display_ti.task_hdl = *th;
		}

		th = &_batmon_args.batmon_ti.task_hdl;
		rc = XG_InitBatteryMonitorTask(&_batmon_args, 3, th);
		if (rc != 0) {
			/* Task couldn't be initialized successfully. */
			break;
		} else {
			/* Copy the task handler. */
			_display_args.batmon_ti.task_hdl = *th;
			_slpmod_args.batmon_ti.task_hdl = *th;
		}

		th = &_slpmod_args.slpmod_ti.task_hdl;
		rc = XG_InitSleepModeTask(&_slpmod_args, 3, th);
		if (rc != 0) {
			/* Task couldn't be initialized successfully. */
			break;
		} else {
			/* Copy the task handler. */
			_display_args.slpmod_ti.task_hdl = *th;
			_batmon_args.slpmod_ti.task_hdl = *th;
		}
	} while (0);
	/* END Create and initialize Xling tasks. */

	/* Start the FreeRTOS scheduler. */
	if (rc == 0) {
		vTaskStartScheduler();
	}

	/* Stop the display driver. */
	MSIM_SH1106__drvStop();

	/*
	 * If all is well then main() will never reach here as the scheduler
	 * will now be running the tasks. If main() does reach here then it is
	 * likely that there was insufficient heap memory available for the
	 * display queue or tasks to be created.
	 */
	while (1);

	return 0;
}

/*
 * Note that for effectively any AVR that has the option to generate WDT
 * interrupts, the watchdog timer remains active even after a system reset
 * (except a power-on condition), using the fastest prescaler value
 * (approximately 15 ms).
 *
 * It is therefore required to turn off the watchdog early during program
 * startup.
 *
 * NOTE: https://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html
 */
static void
disable_wdt(void)
{
	/*
	 * Saving the value of MCUSR in mcusr_mirror is only needed if the
	 * application later wants to examine the reset source, but in
	 * particular, clearing the watchdog reset flag before disabling the
	 * watchdog is required, according to the datasheet.
	 */
	_mcusr_mirror = MCUSR;
	MCUSR = 0;

	wdt_disable();
}

/*
 * Disable interrupts and sit still in case of a stask overflow.
 *
 * NOTE: This hook will be optimized out by the compiler unless you have
 * a "configCHECK_FOR_STACK_OVERFLOW" option enabled in the FreeRTOSConfig.h.
 */
void
vApplicationStackOverflowHook(TaskHandle_t *pxTask, signed char *pcTaskName)
{
	taskDISABLE_INTERRUPTS();
	while(1);
}
