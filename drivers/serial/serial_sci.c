/*
 * Renesas generic SCI device driver.
 * Copyright (C) 2010 Yoshinori Sato <ysato@users.sourceforge.jp>
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

#define SCIBASE 0xffffb0

#define SMR (SCIBASE + 0x00)
#define BRR (SCIBASE + 0x01)
#define SCR (SCIBASE + 0x02)
#define TDR (SCIBASE + 0x03)
#define SSR (SCIBASE + 0x04)
#define RDR (SCIBASE + 0x05)

/*----------------------------------------------------------------------*/

# define BRR_VALUE(bps, clk) ((clk + 16 * bps) / (32 * bps) - 1)

void serial_setbrg(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	outb(BRR_VALUE(gd->baudrate, CONFIG_SYS_HZ), BRR);
}

int serial_init(void)
{
	outb(0, SCR);
	outb(0, SMR);
	serial_setbrg();
	outb(0x30, SCR);

	return 0;
}

void serial_raw_putc(const char c)
{
	while(!(inb(SSR) & 0x80))
	      ;
	outb(c, TDR);
	outb(0x78, SSR);
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
}

int serial_tstc(void)
{
	return (inb(SSR) & 0x40 != 0);
}

void handle_error(void)
{
	inb(SSR);
	outb(0xc4, SSR);
}

int serial_getc_check(void)
{
	unsigned char ssr = inb(SSR);
	if (ssr & 0x37)
		handle_error();
	return ssr & 0x40;
}

int serial_getc(void)
{
	char ch;

	while (!serial_getc_check())
		;

	ch = inb(RDR);
	outb(inb(SSR) & ~0x40, SSR);

	return ch;
}
