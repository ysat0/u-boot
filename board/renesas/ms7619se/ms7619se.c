/*
 * Copyright (C) 2013 Yoshinori Sato <ysato@users.sourceforge.jp>
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

DECLARE_GLOBAL_DATA_PTR;

#if defined(CONFIG_CMD_NET)
static inline void write_bit(int b)
{
	if (b)
		__raw_writew(0x000a, PCDRL);
	else
		__raw_writew(0x0008, PCDRL);
	udelay(2);
	if (b)
		__raw_writew(0x000e, PCDRL);
	else
		__raw_writew(0x000c, PCDRL);
	udelay(2);
}

static inline int read_bit(void)
{
	__raw_writew(0x0008, PCDRL);
	udelay(2);
	__raw_writew(0x000c, PCDRL);
	udelay(2);
	return __raw_readw(PCDRL) & 1;
}

static void read_e2(int addr, ushort *dst, int len)
{
	int i;
	ushort d = 0;

	/* CS=H */
	__raw_writew(0x0008, PCDRL);
	udelay(10);

	/* read command */
	write_bit(1);
	write_bit(1);
	write_bit(0);
	for(i = 0; i < 10; i++) {
		if (addr & 0x200)
			write_bit(1);
		else
			write_bit(0);
		addr <<= 1;
	}
	while(len > 0) {
		for (i = 0; i < 16; i++) {
			d <<= 1;
			d |= read_bit();
		}
		*dst++ = d;
		len--;
	}
	/* CS=L */
	__raw_writew(0x0000, PCDRL);
}

int board_late_init(void)
{
	uchar enetaddr[6];
	if (!eth_getenv_enetaddr("ethaddr", enetaddr)) {
		read_e2(0, (ushort *)enetaddr, 3);
		eth_setenv_enetaddr("ethaddr", enetaddr);
	}
	return 0;
}
#else
int board_late_init(void)
{
	return 0;
}

#endif

int checkboard(void)
{
	puts("BOARD: Renesas Technology MS7619SE\n");
	return 0;
}

int board_init(void)
{
	return 0;
}

int dram_init(void)
{
	gd->bd->bi_memstart = CONFIG_SYS_SDRAM_BASE;
	gd->bd->bi_memsize = CONFIG_SYS_SDRAM_SIZE;
	printf("DRAM:  %dMB\n", CONFIG_SYS_SDRAM_SIZE / (1024 * 1024));
	return 0;
}

void led_set_state(unsigned short value)
{
}

