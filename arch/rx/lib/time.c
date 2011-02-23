/*
 * Copyright (C) 2010 Yoshinori Sato <ysato@users.sourceforge.jp>
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

#define TCR0  (CONFIG_SYS_TIMER_BASE + 0)
#define TCR1  (CONFIG_SYS_TIMER_BASE + 1)
#define TCSR0 (CONFIG_SYS_TIMER_BASE + 2)
#define TCSR1 (CONFIG_SYS_TIMER_BASE + 3)
#define TCNT0 (CONFIG_SYS_TIMER_BASE + 8)
#define TCNT1 (CONFIG_SYS_TIMER_BASE + 9)

static unsigned long long tick;
static unsigned short last;

int timer_init(void)
{
	outb(0x04, TCR0);	/* 16bit free running */
	outb(0x01, TCR1);	/* clk / 8 */
	return 0;
}

unsigned long long get_ticks(void)
{
	unsigned short now = inw(TCNT0);
	if (now < last)
		tick += (0x10000 - last) + now;
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
	outw((u16) t, TCNT0);
}

void reset_timer(void)
{
	last = 0;
	outb(0x00, TCR1);
	outw(0, TCNT0);
	outb(0x01, TCR1);
}

void __udelay(unsigned long usec)
{
	unsigned long long end = get_ticks() + (usec * (CONFIG_SYS_HZ / 8000000));

	while (get_ticks() < end);
}

unsigned long get_tbclk(void)
{
	return CONFIG_SYS_HZ / 8;
}
