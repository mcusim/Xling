#ifndef XLING_HW_SSD1309_H_
#define XLING_HW_SSD1309_H_ 1

#include <stdint.h>

#include "xling/twi.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Structure to work with SSD1309-based display. */
struct XG_SSD1309 {
	struct XG_TWI *twi;
};

/* Initialize an SSD1309-based display connected using TWI
 * (two-wire interface). */
void XG_SSD1309TWIInit(struct XG_TWI *twi, struct XG_SSD1309 *dev);

/* Print text on the first line of the display. */
void XG_SSD1309Print(struct XG_SSD1309 *dev, const uint8_t *text);

void XG_SSD1309SetPage(struct XG_SSD1309 *dev, uint8_t page);

void XG_SSD1309SetColumn(struct XG_SSD1309 *dev, uint8_t col);

#ifdef __cplusplus
}
#endif

#endif /* XLING_HW_SSD1309_H_ */
