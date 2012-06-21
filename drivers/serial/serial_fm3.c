/*
 * Fujitsu FM3 UART Driver
 * Copyright (C) 2012 Yoshinori Sato <ysato@users.sourceforge.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <common.h>
#include <asm/io.h>
#include <asm/processor.h>

#define BGR_VALUE(baud, freq) ((freq) / (baud))

#define UART_CH (CONFIG_SYS_FM3_UART_CH)
#define UART_BASE (0x40038000 + UART_CH * 0x100)
#define SMR (UART_BASE + 0x00)
#define SCR (UART_BASE + 0x01)
#define ESCR (UART_BASE + 0x04)
#define SSR (UART_BASE + 0x05)
#define TDR (UART_BASE + 0x08)
#define RDR (UART_BASE + 0x08)
#define BGR (UART_BASE + 0x0C)
#if UART_CH >= 4
#define FCR0 (UART_BASE + 0x14)
#define FCR1 (UART_BASE + 0x15)
#define FBYTE1 (UART_BASE + 0x18)
#define FBYTE2 (UART_BASE + 0x19)
#endif


void serial_setbrg(void)
{
	DECLARE_GLOBAL_DATA_PTR;
	writew(BGR_VALUE(gd->baudrate, CONFIG_APB2_CLK_FREQ), BGR);
}

int serial_init(void)
{
	
	writeb(0x80, SCR);
	writeb(0x00, SCR);
	writeb(0x00, SMR);
	writeb(0x00, ESCR);
	serial_setbrg();
#if defined(FCR1)
	writeb(0x0c, FCR0);
	writeb(0x0c, FCR1);
	writeb(0x03, FCR0);
#endif
	writeb(0x01, SMR);
	writeb(0x03, SCR);
	return 0;
}

void serial_raw_putc(const char c)
{
#if defined(FCR1)
	while (!(readb(FCR1) & 0x04)) {};
#else
	while(!readb(SSR) & 0x02) {};
#endif
	writeb(c, TDR);
}

void serial_putc(const char c)
{
	if (c == '\n')
		serial_raw_putc('\r');
	serial_raw_putc(c);
}

void serial_puts(const char *s)
{
	char c;
	while ((c = *s++) != 0)
		serial_putc(c);
	return 0;
}

int serial_tstc(void)
{
	return readb(FBYTE2) > 0;
}

void handle_error(void)
{
	writeb(0x80, SSR);
}

int serial_getc_check(void)
{
	uchar status;

	status = readb(SSR);

	if (status & 0x38)
		handle_error();
	return status & 0x04;
}

int serial_getc(void)
{
	uchar status;
	char ch;

	while (!serial_getc_check()) {};

	ch = readb(RDR);
	status = readb(RDR);

	if (status & 0x38)
		handle_error();

	return ch;
}
