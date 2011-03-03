/*
 * Copyright 2010 Yoshinori Sato <ysato@users.sourceforge.jp>
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
#include <net.h>
#include <netdev.h>
#include <asm/io.h>
#include <asm/processor.h>

int checkboard(void)
{
	puts("BOARD: csun KaneBebe\n");
	return 0;
}

int board_init(void)
{
	__raw_writeb(0xff, 0xfee009);
	__raw_writeb(0xc0, 0xffffd9);
	return 0;
}

int dram_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gd->bd->bi_memstart = CONFIG_SYS_DRAM_BASE;
	gd->bd->bi_memsize = CONFIG_SYS_DRAM_SIZE;
	printf("DRAM:  %dMB\n", CONFIG_SYS_DRAM_SIZE / (1024 * 1024));
	return 0;
}

void led_set_state(unsigned short value)
{
	unsigned char padr;
	padr = __raw_readb(0xffffd9);
	padr &= 0x3f;
	padr |= (value & 0x03) << 6;
	__raw_writeb(padr, 0xffffd9);
}

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	return rtl8019_initialize(bis, CONFIG_DRIVER_RTL8019_BASE);
}
#endif
