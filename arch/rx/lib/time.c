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

#define TICK ((CONFIG_SYS_CLK_FREQ/32)/1000000)

static unsigned long long tick;
static unsigned short last;

int timer_init(void)
{
	outw(0xffff, 0x00088006);
	outw(0x0000, 0x00088004);
	outw(0x0001, 0x00088002);
	outw(0x0001, 0x00088000);

	return 0;
}

unsigned long long get_ticks(void)
{
	unsigned short now = inw(0x00088004);
	if (now < last)
		tick += (0x10000 - last) + now;
	else
		tick += now - last;
	last = now;
	return tick;
}

ulong get_timer(ulong base)
{
	return get_ticks() / ((CONFIG_SYS_CLK_FREQ/32)/CONFIG_SYS_HZ) -  base;
}

void set_timer(ulong t)
{
	outw((u16) t * ((CONFIG_SYS_CLK_FREQ/32)/CONFIG_SYS_HZ), 0x00088004);
}

void reset_timer(void)
{
	last = 0;
	outw(0x0000, 0x00088004);
}

void __udelay(unsigned long usec)
{
	unsigned long long end = get_ticks() + usec;

	while (get_ticks() < end);
}

unsigned long get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}
