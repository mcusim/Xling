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
 * Declarations for two-wire interface (TWI).
 */
#ifndef XLING_TWI_H_
#define XLING_TWI_H_ 1

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct TWI {
	uint32_t cpuclk;	/* CPU clock frequency */
	uint32_t clk;		/* Clock frequency of TWI */
	uint8_t addr;		/* Address of a slave device */
	volatile uint8_t *port;	/* TWI port (PORTC, PORTD, etc.) */
	volatile uint8_t *ddr;	/* TWI data direction (DDRC, DDRD, etc.) */
	uint8_t sda;		/* SDA pin number of the TWI port */
	uint8_t scl;		/* SCL pin number of the TWI port */
};

void TWI_Init(struct TWI *twi);
int TWI_Start(struct TWI *twi);
int TWI_Stop(struct TWI *twi);
int TWI_Write(struct TWI *twi, uint8_t byte);
int TWI_WriteData(struct TWI *twi, uint8_t *data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* XLING_TWI_H_ */
