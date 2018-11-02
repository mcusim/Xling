#define F_CPU			8000000UL	/* CPU clock frequency */
#define TWI_CLOCK		400000L		/* TWI clock frequency */
#define I2CPORT			PORTC		/* TWI port */
#define I2CDDR			DDRC		/* TWI data direction port */
#define BB_SDA			PC4		/* SDA pin of TWI port */
#define BB_SCL			PC5		/* SCL pin of TWI port */

#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "xling/twi.h"
#include "xling/font.h"
#include "xling/tits.h"
#include "xling/luci.h"

#define SET_BIT(byte, bit)	((byte)|=(1UL<<(bit)))
#define CLEAR_BIT(byte, bit)	((byte)&=~(1UL<<(bit)))
#define IS_SET(byte, bit)	(((byte)&(1UL<<(bit)))>>(bit))

#define BTN1				PD0
#define BTN2				PD1
#define BTN3				PD2
#define OLED_RES			PD3
#define OLED_PWR			PD4

#define FRAME_ITER			20
#define FRAME_DIV			100000UL

#define SSD1306_SETCONTRAST		0x81
#define SSD1306_DISPLAYALLON_RESUME	0xA4
#define SSD1306_DISPLAYALLON		0xA5
#define SSD1306_NORMALDISPLAY		0xA6
#define SSD1306_INVERTDISPLAY		0xA7
#define SSD1306_DISPLAYOFF		0xAE
#define SSD1306_DISPLAYON		0xAF
#define SSD1306_SETDISPLAYOFFSET	0xD3
#define SSD1306_SETCOMPINS		0xDA
#define SSD1306_SETVCOMDETECT		0xDB
#define SSD1306_SETDISPLAYCLOCKDIV	0xD5
#define SSD1306_SETPRECHARGE		0xD9
#define SSD1306_SETMULTIPLEX		0xA8
#define SSD1306_SETLOWCOLUMN		0x00
#define SSD1306_SETHIGHCOLUMN		0x10
#define SSD1306_SETSTARTLINE		0x40
#define SSD1306_MEMORYMODE		0x20
#define SSD1306_COLUMNADDR		0x21
#define SSD1306_PAGEADDR		0x22
#define SSD1306_COMSCANINC		0xC0
#define SSD1306_COMSCANDEC		0xC8
#define SSD1306_SEGREMAP		0xA0
#define SSD1306_CHARGEPUMP		0x8D
#define SSD1306_SWITCHCAPVCC		0x02
#define SSD1306_NOP			0xE3

static struct XL_TWI twi;
static uint8_t buf[1024];
static volatile uint32_t ms;

static void timer2_init();
static void oled_init(void);
static void send_framebuffer(unsigned char *buf, int len);
static void send_command(unsigned char cmd);
static void print_text(const char *text);

int main(void)
{
	uint32_t i, frames;
	uint32_t j;
	uint32_t start_ms, stop_ms;
	uint8_t luci;
	uint8_t textbuf[32];

	DDRD = 0x00;
	/* Configure pins as input/output ones. */
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

	/* Setup Timer/Counter2 to count milliseconds */
	ms = 0;
	timer2_init();
	sei();

	/* Setup OLED display */
	oled_init();
	/* Show tits! */
	XL_TWIStart(&twi);
	XL_TWIWrite(&twi, 0x40);
	for (j = 0; j < sizeof oled_tits; j++)
		XL_TWIWrite(&twi, pgm_read_byte(&oled_tits[j]));
	XL_TWIStop(&twi);
	_delay_ms(100);

	i = 0;
	luci = 0;
	while (1) {
		start_ms = ms;
		XL_TWIStart(&twi);
		XL_TWIWrite(&twi, 0x40);
		if (luci == 1U) {
			for (j = 0; j < FRAME_ITER; j++) {
				for (i = 0; i < sizeof oled_luci; i++) {
					XL_TWIWrite(&twi,
					            pgm_read_byte(
					                    &oled_luci[i]));
				}
			}
			luci = 0;
		} else {
			for (j = 0; j < FRAME_ITER; j++) {
				for (i = 0; i < sizeof oled_tits; i++) {
					XL_TWIWrite(&twi,
					            pgm_read_byte(
					                    &oled_tits[i]));
				}
			}
			luci = 1;
		}
		XL_TWIStop(&twi);
		stop_ms = ms;

		sprintf(textbuf, "%d.%d", FRAME_DIV/(stop_ms-start_ms),
		        FRAME_DIV%(stop_ms-start_ms));
		start_ms = 0;
		stop_ms = 0;
		print_text(textbuf);
		_delay_ms(500);
	}
	return 0;
}

static void timer2_init()
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

static void print_text(const char *text)
{
	unsigned int i;

	send_command(SSD1306_COLUMNADDR);
	send_command(0x00);
	send_command(0x7F);
	for (i = 0; i < strlen(text); i++) {
		switch (text[i]) {
		case '0':
			send_framebuffer(mark_zero, sizeof mark_zero);
			break;
		case '1':
			send_framebuffer(mark_one, sizeof mark_one);
			break;
		case '2':
			send_framebuffer(mark_two, sizeof mark_two);
			break;
		case '3':
			send_framebuffer(mark_three, sizeof mark_three);
			break;
		case '4':
			send_framebuffer(mark_four, sizeof mark_four);
			break;
		case '5':
			send_framebuffer(mark_five, sizeof mark_five);
			break;
		case '6':
			send_framebuffer(mark_six, sizeof mark_six);
			break;
		case '7':
			send_framebuffer(mark_seven, sizeof mark_seven);
			break;
		case '8':
			send_framebuffer(mark_eight, sizeof mark_eight);
			break;
		case '9':
			send_framebuffer(mark_nine, sizeof mark_nine);
			break;
		case '.':
			send_framebuffer(mark_dot, sizeof mark_dot);
			break;
		}
	}
	send_command(SSD1306_COLUMNADDR);
	send_command(0x00);
	send_command(0x7F);
}

static void oled_init(void)
{
	XL_TWIInit(&twi, 0x7A, F_CPU, TWI_CLOCK);

	send_command(SSD1306_DISPLAYOFF);
	send_command(SSD1306_SETDISPLAYCLOCKDIV);
	send_command(0xF0);
	send_command(SSD1306_SETDISPLAYOFFSET);
	send_command(0x00);
	send_command(SSD1306_SETSTARTLINE | 0x00);
	send_command(SSD1306_MEMORYMODE);
	send_command(0x00);			/* Horizontal memory mode */
	send_command(SSD1306_SETCONTRAST);
	send_command(0x50);
	send_command(SSD1306_NORMALDISPLAY);	/* Non-inverted display */
	send_command(SSD1306_DISPLAYON);
}

static void send_framebuffer(unsigned char *buf, int len)
{
	XL_TWIStart(&twi);
	XL_TWIWrite(&twi, 0x40);
	XL_TWIWriteData(&twi, buf, len);
	XL_TWIStop(&twi);
}

static void send_command(unsigned char cmd)
{
	XL_TWIStart(&twi);
	XL_TWIWrite(&twi, 0x00);
	XL_TWIWrite(&twi, cmd);
	XL_TWIStop(&twi);
}
