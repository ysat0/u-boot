/*
 * Copyright 2010 Yoshinori Sato <ysato@users.sourceforge.jp>
 *
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <asm/processor.h>

#define _8TCR0  0xffff80
#define _8TCR1  0xffff81
#define _8TCSR0 0xffff82
#define _8TCSR1 0xffff83
#define _8TCNT0 0xffff88
#define _8TCNT1 0xffff89

static unsigned long long tick;
static unsigned short last;

int timer_init(void)
{
	outb(0x04, _8TCR0);	/* 16bit free running */
	outb(0x01, _8TCR1);	/* clk / 8 */
	return 0;
}

unsigned long long get_ticks(void)
{
	unsigned short now = inw(_8TCNT0);
	if (now < last)
		tick += 0x10000 + (0x10000 - last) + now;
	else
		tick += now - last;
	last = now;
	return tick;
}

ulong get_timer(ulong base)
{
	return get_ticks() / (CONFIG_SYS_HZ / 1000) - base;
}

void set_timer(ulong t)
{
	outw((u16) t, _8TCNT0);
}

void reset_timer(void)
{
	last = 0;
	outb(0x00, _8TCR1);
	outw(0, _8TCNT0);
	outb(0x01, _8TCR1);
}

void __udelay(unsigned long usec)
{
	unsigned long long end = get_ticks() + usec * (CONFIG_SYS_HZ / 1000000);

	while (get_ticks() < end);
}

unsigned long get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}
