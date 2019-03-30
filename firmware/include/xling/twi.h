#ifndef XLING_TWI_H_
#define XLING_TWI_H_ 1

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Structure to deal with TWI.*/
struct XG_TWI {
	uint32_t clk;		/* Clock frequency of TWI */
	uint8_t addr;		/* Address of a slave device */

	/* The next fields are for bit-bang TWI only. */
	volatile uint8_t *port;	/* TWI port (PORTC, PORTD, etc.) */
	volatile uint8_t *ddr;	/* TWI data direction (DDRC, DDRD, etc.) */
	uint8_t sda;		/* SDA pin number of the TWI port */
	uint8_t scl;		/* SCL pin number of the TWI port */
};

/* Function to initialize TWI interface using address of a device,
 * CPU clock and TWI clock frequencies. */
void XG_TWIInit(struct XG_TWI *twi, uint8_t addr, uint32_t cpu_clk,
                uint32_t twi_clk);

/* Function to prepare TWI to transmit data. */
int XG_TWIStart(struct XG_TWI *twi);

/* Function to stop TWI transmittion. */
int XG_TWIStop(struct XG_TWI *twi);

/* Function to write a byte to TWI. */
int XG_TWIWrite(struct XG_TWI *twi, uint8_t v);

/* Function to write array of bytes to TWI. */
int XG_TWIWriteData(struct XG_TWI *twi, uint8_t *data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* XLING_TWI_H_ */
