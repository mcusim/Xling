/*
 * Copyright (c) 2017, 2018, The MCUSim Contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the MCUSim or its parts nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
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
 * Main file of the Xling firmware.
 */
#define F_CPU			8000000UL	/* CPU clock frequency */
#define TWI_CLOCK		400000L		/* TWI clock frequency */

#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "xling/twi.h"
#include "xling/luci.h"
#include "xling/hw/ssd1309.h"

#define SET_BIT(byte, bit)	((byte)|=(1UL<<(bit)))
#define CLEAR_BIT(byte, bit)	((byte)&=~(1UL<<(bit)))
#define IS_SET(byte, bit)	(((byte)&(1UL<<(bit)))>>(bit))

#define BTN1			PD0
#define BTN2			PD1
#define BTN3			PD2
#define OLED_RES		PD3
#define OLED_PWR		PD4

#define FRAME_ITER		5
#define FRAME_DIV		100000UL

/* Static variables */
static struct XG_TWI twi;
static struct XG_SSD1309 display;
static volatile uint32_t ms = 0;

/* Declarations of the local functions */
static void timer2_init(void);

int main(void)
{
	uint32_t i, j, frames;
	uint32_t delay_ms;
	char textbuf[32];

	/* Bit-bang TWI configuration */
	twi.port = (uint8_t *)0x28;	/* Offset to PORTC register */
	twi.ddr = (uint8_t *)0x27;	/* Offset to DDRC register */
	twi.sda = PC4;
	twi.scl = PC5;
	XG_TWIInit(&twi, 0x7A, F_CPU, TWI_CLOCK);

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
	XG_SSD1309TWIInit(&twi, &display);

	/* Setup Timer/Counter2 to count milliseconds */
	timer2_init();
	sei();

	i = 0;
	while (1) {
		XG_SSD1309SetPage(&display, 0);
		XG_SSD1309SetColumn(&display, 0);

		XG_TWIStart(&twi);
		XG_TWIWrite(&twi, 0x40);
		delay_ms = ms;
		for (j = 0; j < FRAME_ITER; j++) {
			for (i = 0; i < sizeof oled_luci; i++) {
				XG_TWIWrite(&twi, pgm_read_byte(&oled_luci[i]));
			}
		}
		delay_ms = ms-delay_ms;
		XG_TWIStop(&twi);

		snprintf(textbuf, sizeof textbuf, "%lu.%lu",
		         FRAME_DIV/delay_ms, FRAME_DIV%delay_ms);
		XG_SSD1309SetPage(&display, 7);
		XG_SSD1309SetColumn(&display, 0);
		XG_SSD1309Print(&display, &textbuf[0]);
		_delay_ms(300);
	}
	return 0;
}

static void timer2_init(void)
{
	/* CTC mode, WGM22:0 = 2 */
	TCCR2A |= (1<<WGM21);
	TCCR2A &= ~(1<<WGM20);
	TCCR2B &= ~(1<<WGM22);

	/* OCR2A is 124 to gain 1kHz with selected prescaler */
	TCNT2 = 0;
	OCR2A = 124;
	/* Enable interrupt */
	TIMSK2 |= (1<<OCIE2A);

	/* Start timer, prescaler to Fclk_io/32: CS21:0 = 6 */
	TCCR2B |= (1<<CS21) | (1<<CS20);
	TCCR2B &= ~(1<<CS22);
}

ISR(TIMER2_COMPA_vect)
{
	ms++;
}
