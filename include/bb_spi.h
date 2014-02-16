/*
 * Copyright (C) 2014 Yoshinori Sato <ysato@users.sourceforge.jp>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __BB_SPI_H
#define __BB_SPI_H

#include <spi.h>
#include <linux/list.h>

struct bb_spi;

struct bb_spi_dev {
	struct list_head link;
	int _bus;
	int _cs;
	int mode;
	int max_hz;
	int hz;
	int (*setup)(struct bb_spi *spi);
	int (*free)(struct spi_slave *spi);
	int (*bus)(struct spi_slave *slave, int req);
	void (*set_speed)(struct spi_slave *slave, unsigned int hz);
	int (*dio)(struct spi_slave *slave, unsigned int b);
	void (*clock)(struct spi_slave *slave, unsigned int c);
	int (*startstop)(struct spi_slave *slave, unsigned int s);
	void (*cs)(struct spi_slave *slave, int en);
};

void bb_spi_register_dev(struct bb_spi_dev *dev);

#endif
