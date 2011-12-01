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
	*(volatile unsigned long *)0x00080010 &= ~0x00008000;
	*(volatile unsigned long *)0x00080014 &= ~0xc0000000;
	*(volatile unsigned char *)0x0008c062 |= 0x02;
	*(volatile unsigned char *)0x0008c10f |= 0x02;
	*(volatile unsigned char *)0x0008c06b |= 0x04;
#if defined(CONFIG_SH_ETHER)
	*(volatile unsigned long *)0x00080014 &= ~0x00008000;
#endif
#if defined(CONFIG_SPI_FLASH) || defined(COFIG_MMC_SPI)
	*(volatile unsigned char *)0x0008c06c |= 0x80;
	*(volatile unsigned long *)0x00080014 &= ~0x00020000;
	*(volatile unsigned char *)0x0008c110 |= 0x0e;
#endif
#if defined(CONFIG_SPI_FLASH)
	*(volatile unsigned char *)0x0008c02c |= 0x04;
	*(volatile unsigned char *)0x0008c00c |= 0x04;
#endif
#if defined(CONFIG_MMC_SPI)
	*(volatile unsigned char *)0x0008c02c |= 0x08;
	*(volatile unsigned char *)0x0008c00c |= 0x08;
#endif
	return 0;
}

int sdram_init(void)
{
#if !defined(CONFIG_TKDN_RX620_RAM)
	*(volatile unsigned char *)0x00083c00 = 0x00;
	*(volatile unsigned char *)0x00083c10 = 0x00;
	*(volatile unsigned char *)0x00083c16 = 0x00;
	*(volatile unsigned short *)0x00083c24 = 0x0588;
	*(volatile unsigned char *)0x00083c20 = 0x01;
	while(*(volatile unsigned char *)0x00083c50 & 0x08);

	*(volatile unsigned char *)0x00083c00 = 0x10;
	*(volatile unsigned short *)0x00083c48 = 0x0080;
	while(*(volatile unsigned char *)0x00083c50 & 0x01);
	
	*(volatile unsigned long *)0x00083c44 = 0x00020202;
	*(volatile unsigned char *)0x00083c40 = 0x00;

	*(volatile unsigned short *)0x00083c14 = 0x32e0;
	*(volatile unsigned char *)0x00083c02 = 0x01;
	*(volatile unsigned char *)0x00083c16 = 0x01;
	*(volatile unsigned char *)0x00083c10 = 0x00;

	*(volatile unsigned char *)0x00083c00 = 0x11;
#endif
	return 0;
}

#if defined(CONFIG_SPI_FLASH) || defined(COFIG_MMC_SPI)
int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	return (bus == 0) && (cs == 0 || cs == 1);
}

void spi_cs_activate(struct spi_slave *slave)
{
	unsigned char cs = 1 << (slave->cs + 2);
	*(volatile unsigned char *)0x0008c02c &= ~cs;
}

void spi_cs_deactivate(struct spi_slave *slave)
{
	unsigned char cs = 1 << (slave->cs + 2);
	*(volatile unsigned char *)0x0008c02c |= cs;
}
#endif
