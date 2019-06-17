/*
 * Copyright 2017-2019 The MCUSim Project.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the MCUSim or its parts nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Entry point of the Xling firmware.
 */
#define F_CPU			12000000UL	/* CPU 12 MHz */
#define F_TWI			400000L		/* TWI 400 kHz */

#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "xling/graphics/luci.h"
#include "xling/oled/sh1106.h"
#include "xling/twi/twi.h"

#define SET_BIT(byte, bit)	((byte)|=(1U<<(bit)))
#define CLEAR_BIT(byte, bit)	((byte)&=(uint8_t)~(1U<<(bit)))
#define IS_SET(byte, bit)	(((byte)&(1U<<(bit)))>>(bit))

#define BTN1			PD0
#define BTN2			PD1
#define BTN3			PD2
#define OLED_RES		PD3
#define OLED_PWR		PD4
#define FRAME_ITER		5
#define FRAME_DIV		100000UL

static volatile uint32_t ms;
static volatile uint32_t dummy;
static struct SH1106 display = {
	.twi = {
		.cpuclk = F_CPU,		/* CPU frequency */
		.clk = F_TWI,			/* TWI frequency */
		.addr = 0x7A,			/* Display TWI address */
		.port = (uint8_t *)0x28,	/* Bit-bang port (PORTC) */
		.ddr = (uint8_t *)0x27,		/* Bit-bang ddr (DDRC) */
		.sda = PC4,			/* Bit-bang SDA pin */
		.scl = PC5,			/* Bit-bang SCL pin */
	},
};

static void timer2_init(void);

int main(void)
{
	uint8_t textbuf[32];

	/* Configure pins as input/output ones. */
	DDRD = 0x00;
	SET_BIT(DDRD, OLED_RES);		/* output */
	SET_BIT(DDRD, OLED_PWR);		/* output */
	CLEAR_BIT(DDRD, BTN1);			/* input */
	CLEAR_BIT(DDRD, BTN2);			/* input */
	CLEAR_BIT(DDRD, BTN3);			/* input */

	/* Set initial values. */
	SET_BIT(PORTD, OLED_RES);
	CLEAR_BIT(PORTD, OLED_PWR);
	_delay_ms(1);

	/* We've to switch OLED on. */
	CLEAR_BIT(PORTD, OLED_RES);
	_delay_ms(1);
	SET_BIT(PORTD, OLED_RES);
	_delay_ms(1);
	SET_BIT(PORTD, OLED_PWR);
	_delay_ms(100);

	/* Setup OLED display */
	SH1106_TWIInit(&display);

	/* Setup Timer/Counter2 to count milliseconds */
	timer2_init();
	sei();

	while (1) {
		uint32_t delay_ms = ms;

		for (uint32_t j = 0; j < FRAME_ITER; j++) {
			for (uint32_t i = 0; i < (sizeof oled_luci)/128; i++) {
				SH1106_Purge(&display);
				SH1106_SetPage(&display, i);
				SH1106_SetColumn(&display, 0);

				SH1106_WriteControlByte(&display, CB_LASTRAM);
				for (uint32_t k = 0; k < 132; k++) {
					if ((k < 2) || (k >= 130)) {
						SH1106_WriteDisplayData(
							&display, 0xFF);
					} else {
						SH1106_WriteDisplayData(
							&display, pgm_read_byte(&oled_luci[i*128+k-2]));
					}
				}
				SH1106_Send(&display);
			}
		}
		delay_ms = ms - delay_ms;

		snprintf((char *)&textbuf[0], sizeof textbuf, "%lu.%lu",
		         FRAME_DIV/delay_ms, FRAME_DIV%delay_ms);

		SH1106_Purge(&display);
		SH1106_SetPage(&display, 7);
		SH1106_SetColumn(&display, 2);
		SH1106_Send(&display);

		SH1106_Print(&display, &textbuf[0]);

		_delay_ms(1000);
	}

	return 0;
}

static void timer2_init(void)
{
	/* CTC mode, WGM22:0 = 2 */
	TCCR2A |= (1<<WGM21);
	TCCR2A &= (uint8_t)(~(1<<WGM20));
	TCCR2B &= (uint8_t)(~(1<<WGM22));

	/* OCR2A is 124 to gain 1kHz with selected prescaler */
	TCNT2 = 0;
	OCR2A = 124;
	/* Enable interrupt */
	TIMSK2 |= (1<<OCIE2A);

	/* Start timer, prescaler to Fclk_io/32: CS21:0 = 6 */
	TCCR2B |= (1<<CS21) | (1<<CS20);
	TCCR2B &= (uint8_t)(~(1<<CS22));
}

ISR(TIMER2_COMPA_vect)
{
	ms++;
}

ISR(TIMER2_COMPB_vect)
{
	dummy++;
}
