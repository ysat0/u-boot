/*
 * Copyright (C) 2011 Yoshinori Sato
 * (C) Copyright 2000-2003
 *     Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * board/tkdn-rx62n/tkdn-rx62n.c
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

int checkboard(void)
{
	puts("BOARD: TKDN-RX62N-BRD\n");
	return 0;
}

int board_init(void)
{
	return 0;
}

int dram_init(void)
{
	return 0;
}

void led_set_state(unsigned short value)
{

}
