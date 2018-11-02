#include <stdint.h>
#include <util/twi.h>
#include "xling/twi.h"

/* I2C port */
#ifndef I2CPORT
#define I2CPORT		PORTC
#endif
#ifndef I2CDDR
#define I2CDDR		DDRC
#endif
/* I2C pins */
#ifndef BB_SDA
#define BB_SDA		PC4
#endif
#ifndef BB_SCL
#define BB_SCL		PC5
#endif

static void twi_byteout(unsigned char b);
static void twi_dataout(unsigned char *data, int len);

void XL_TWIInit(struct XL_TWI *twi, unsigned char addr,
                unsigned int cpu_clk, unsigned int twi_clk)
{
	twi->addr = addr;
	/* twi->clk = twi_clk; */
	I2CDDR &= ~(1 << BB_SDA);
	I2CDDR &= ~(1 << BB_SCL); /* Let them float high */
	I2CPORT |= (1 << BB_SDA); /* Set both lines to get pulled up */
	I2CPORT |= (1 << BB_SCL);
}

int XL_TWIWrite(struct XL_TWI *twi, unsigned char v)
{
	twi_byteout(v);
	return 0;
}

int XL_TWIWriteData(struct XL_TWI *twi, unsigned char *data, int len)
{
	twi_dataout(data, len);
	return 0;
}

int XL_TWIStart(struct XL_TWI *twi)
{
	I2CPORT |= ((1 << BB_SDA) + (1 << BB_SCL));
	I2CDDR |= ((1 << BB_SDA) + (1 << BB_SCL));
	I2CPORT &= ~(1 << BB_SDA); /* Data line low first, */
	I2CPORT &= ~(1 << BB_SCL); /* then clock line low is a START signal */
	twi_byteout(twi->addr); /* Send the slave address */
	return 0;
}

int XL_TWIStop(struct XL_TWI *twi)
{
	I2CPORT &= ~(1 << BB_SDA);
	I2CPORT |= (1 << BB_SCL);
	I2CPORT |= (1 << BB_SDA);

	/* Let the lines float (tri-state) */
	I2CDDR &= ((1 << BB_SDA) | (1 << BB_SCL));
	return 0;
}

static void twi_byteout(unsigned char b)
{
	unsigned char i;
	unsigned char bOld = I2CPORT & ~((1 << BB_SDA)|(1 << BB_SCL));

	for (i = 0; i < 8; i++) {
		bOld &= ~(1 << BB_SDA);
		if (b & 0x80)
			bOld |= (1 << BB_SDA);
		I2CPORT = bOld;
		I2CPORT |= (1 << BB_SCL);
		I2CPORT = bOld;
		b <<= 1;
	}
	/* Ack bit */
	I2CPORT = bOld & ~(1 << BB_SDA);	/* set data low */
	I2CPORT |= (1 << BB_SCL);		/* toggle clock */
	I2CPORT = bOld;
}

static void twi_dataout(unsigned char *data, int len)
{
	uint8_t i, b;
	uint8_t bOld = I2CPORT & ~((1 << BB_SDA) | (1 << BB_SCL));

	while (len--) {
		b = *data++;
		/* Special case can save time */
		if (b == 0 || b == 0xff) {
			bOld &= ~(1 << BB_SDA);
			if (b & 0x80)
				bOld |= (1 << BB_SDA);
			I2CPORT = bOld;

			for (i = 0; i < 8; i++) {
				/* just toggle SCL, SDA stays the same */
				I2CPORT |= (1 << BB_SCL);
				I2CPORT = bOld;
			}
		} else { /* normal byte needs every bit tested */
			for (i = 0; i < 8; i++) {
				bOld &= (uint8_t)(~(1<<BB_SDA));
				if (b & 0x80)
					bOld |= (1 << BB_SDA);
				I2CPORT = bOld;
				I2CPORT |= (1 << BB_SCL);
				I2CPORT = bOld;
				b <<= 1;
			}
		}

		/* ACK bit seems to need to be set to 0, but SDA line
		 * doesn't need to be tri-state */
		I2CPORT &= (uint8_t)(~(1<<BB_SDA));
		I2CPORT |= (1 << BB_SCL); /* toggle clock */
		I2CPORT &= (uint8_t)(~(1<<BB_SCL));
	}
}
