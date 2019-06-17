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
 * Bit-bang implementation of the two-wire interface (TWI).
 */
#include <stdint.h>
#include <util/twi.h>

#include "xling/twi/twi.h"

static void write_byte(struct TWI *twi, uint8_t b);
static void write_data(struct TWI *twi, uint8_t *data, uint32_t len);

/* Initialize TWI interface. */
void
TWI_Init(struct TWI *twi)
{
	/* Let them float high */
	*twi->ddr &= (uint8_t)(~(1<<twi->sda));
	*twi->ddr &= (uint8_t)(~(1<<twi->scl));

	/* Pull up both lines */
	*twi->port |= (uint8_t)(1<<twi->sda);
	*twi->port |= (uint8_t)(1<<twi->scl);
}

/* Send a single byte using TWI */
int
TWI_Write(struct TWI *twi, uint8_t byte)
{
	write_byte(twi, byte);
	return 0;
}

/* Send an array of bytes using TWI */
int
TWI_WriteData(struct TWI *twi, uint8_t *data, uint32_t len)
{
	write_data(twi, data, len);
	return 0;
}

/* Prepare TWI to transmit data. */
int
TWI_Start(struct TWI *twi)
{
	*twi->port |= (uint8_t)((1<<twi->sda) + (1<<twi->scl));
	*twi->ddr |= (uint8_t)((1<<twi->sda) + (1<<twi->scl));

	/* Data line low first, then... */
	*twi->port &= (uint8_t)(~(1<<twi->sda));
	/* ...clock line low is a START signal */
	*twi->port &= (uint8_t)(~(1<<twi->scl));
	/* Send the slave address */
	write_byte(twi, twi->addr);

	return 0;
}

/* Stop TWI transmission. */
int
TWI_Stop(struct TWI *twi)
{
	*twi->port &= (uint8_t)(~(1<<twi->sda));
	*twi->port |= (uint8_t)(1<<twi->scl);
	*twi->port |= (uint8_t)(1<<twi->sda);

	/* Let the lines float (tri-state) */
	*twi->ddr &= (uint8_t)((1<<twi->sda) | (1<<twi->scl));
	return 0;
}

static void
write_byte(struct TWI *twi, uint8_t b)
{
	uint8_t i;
	uint8_t bOld = *twi->port & ~((1 << twi->sda)|(1 << twi->scl));

	for (i = 0; i < 8; i++) {
		bOld &= ~(1 << twi->sda);
		if (b & 0x80) {
			bOld |= (1 << twi->sda);
		}
		*twi->port = bOld;
		*twi->port |= (1 << twi->scl);
		*twi->port = bOld;
		b <<= 1;
	}

	/* Ack bit */
	*twi->port = bOld & ~(1 << twi->sda);	/* set data low */
	*twi->port |= (1 << twi->scl);		/* toggle clock */
	*twi->port = bOld;
}

static void
write_data(struct TWI *twi, uint8_t *data, uint32_t len)
{
	uint8_t i, b;
	uint8_t bOld = *twi->port & ~((1 << twi->sda) | (1 << twi->scl));

	while (len--) {
		b = *data++;
		if (b == 0 || b == 0xff) {
			/* Special case can save time */
			bOld &= ~(1 << twi->sda);
			if (b & 0x80)
				bOld |= (1 << twi->sda);
			*twi->port = bOld;

			for (i = 0; i < 8; i++) {
				/* just toggle SCL, SDA stays the same */
				*twi->port |= (1 << twi->scl);
				*twi->port = bOld;
			}
		} else {
			/* Normal byte needs every bit tested */
			for (i = 0; i < 8; i++) {
				bOld &= (uint8_t)(~(1<<twi->sda));
				if (b & 0x80)
					bOld |= (1 << twi->sda);
				*twi->port = bOld;
				*twi->port |= (1 << twi->scl);
				*twi->port = bOld;
				b <<= 1;
			}
		}

		/* ACK bit seems to need to be set to 0, but SDA line
		 * doesn't need to be tri-state */
		*twi->port &= (uint8_t)(~(1<<twi->sda));
		*twi->port |= (uint8_t)(1 << twi->scl); /* toggle clock */
		*twi->port &= (uint8_t)(~(1<<twi->scl));
	}
}
