#ifndef XLING_TWI_H_
#define XLING_TWI_H_ 1

#ifdef __cplusplus
extern "C" {
#endif

struct XL_TWI {
	unsigned int clk;		/* Clock frequency of TWI */
	unsigned char addr;		/* Address of a slave device */
};

/* Function to initialize TWI interface using address of a device,
 * CPU clock and TWI clock frequencies. */
void XL_TWIInit(struct XL_TWI *twi, unsigned char addr,
                unsigned int cpu_clk, unsigned int twi_clk);

/* Function to prepare TWI to transmit data. */
int XL_TWIStart(struct XL_TWI *twi);

/* Function to stop TWI transmittion. */
int XL_TWIStop(struct XL_TWI *twi);

/* Function to write a byte to TWI. */
int XL_TWIWrite(struct XL_TWI *twi, unsigned char v);

/* Function to write array of bytes to TWI. */
int XL_TWIWriteData(struct XL_TWI *twi, unsigned char *data, int len);

#ifdef __cplusplus
}
#endif

#endif /* XLING_TWI_H_ */
