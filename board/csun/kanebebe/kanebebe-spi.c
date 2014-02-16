/*
 * Copyright (C) 2014 Yoshinori Sato <ysato@users.sourceforge.jp>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <malloc.h>
#include <bb_spi.h>

#define MSTCRH 0xfee01c
#define P6DDR 0xfee005
#define P6DR  0xffffd5
#define PBDDR 0xfee00a
#define PBDR  0xffffda

#define DO (1 << 6)
#define DI_BIT 7
#define CLK (1 << 5)
#define CS (1 << 2)

static 	struct bb_spi_dev *dev;

static int setup(struct bb_spi *spi)
{
	return 0;
}

static int dio(struct spi_slave *spi, unsigned int b)
{
	unsigned char d;
	d = readb(PBDR);
	if (b)
		writeb(d | DO, PBDR);
	else
		writeb(d & ~DO, PBDR);
	return (d >> DI_BIT) & 0x01;
}

static void clock(struct spi_slave *spi, unsigned int c)
{
	unsigned char d;
	d = readb(PBDR);
	if (c)
		writeb(d | CLK, PBDR);
	else
		writeb(d & ~CLK, PBDR);
}

static void cs(struct spi_slave *spi, int en)
{
	unsigned char d;
	d = readb(P6DR);
	if (en)
		writeb(d | CS, P6DR);
	else
		writeb(d & ~CS, P6DR);
}

int board_mmc_init(bd_t *bis)
{
	dev = malloc(sizeof(*dev));
	if (dev == NULL)
		return -1;
	memset(dev, 0, sizeof(*dev));
	dev->setup = setup;
	dev->dio = dio;
	dev->clock = clock;
	dev->cs = cs;
	bb_spi_register_dev(dev);
	return 0;
}
