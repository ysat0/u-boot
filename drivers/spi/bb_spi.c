/*
 * Copyright (C) 2014 Yoshinori Sato <ysato@users.sourceforge.jp>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <asm/io.h>
#include <bb_spi.h>

struct bb_spi {
	struct spi_slave	slave;
	struct bb_spi_dev	*dev;
};

static LIST_HEAD(bb_spi_devs);

static inline struct bb_spi *to_bb_spi(struct spi_slave *slave)
{
	return container_of(slave, struct bb_spi, slave);
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode)
{
	struct bb_spi *spi;
	struct list_head *entry;
	struct bb_spi_dev *dev;

	spi = spi_alloc_slave(struct bb_spi, bus, cs);
	if (!spi)
		return NULL;
	list_for_each(entry, &bb_spi_devs) {
		dev = list_entry(entry, struct bb_spi_dev, link);
		if((dev->_bus == bus) && (dev->_cs == cs)) {
			dev->mode = mode;
			dev->max_hz = max_hz;
			if (dev->setup(spi) >= 0) {
				spi->dev = dev;
				return &spi->slave;
			} else
				break;
		}
	}
	free(spi);
	return NULL;
}

void spi_free_slave(struct spi_slave *slave)
{
	struct bb_spi *spi = to_bb_spi(slave);
	if(spi->dev->free)
		spi->dev->free(slave);
	free(spi);
}

int spi_claim_bus(struct spi_slave *slave)
{
	struct bb_spi *spi = to_bb_spi(slave);
	if (spi->dev->bus)
		return spi->dev->bus(slave, 1);
	else
		return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
	struct bb_spi *spi = to_bb_spi(slave);
	if (spi->dev->bus)
		spi->dev->bus(slave, 0);
}

void spi_set_speed(struct spi_slave *slave, unsigned int hz)
{
	struct bb_spi *spi = to_bb_spi(slave);
	if (spi->dev->set_speed)
		spi->dev->set_speed(slave, hz);
}

static unsigned char xfer_byte(struct spi_slave *slave, unsigned char val)
{
	struct bb_spi *spi = to_bb_spi(slave);
	int i;
	unsigned char ret = 0;
	int clk = (spi->dev->mode & 0x02)?0:1;
	
	for (i = 0; i < 8; i++) {
		ret <<= 1;
		if (spi->dev->mode & 1) {
			spi->dev->clock(slave, clk);
			spi->dev->clock(slave, (~clk) & 1);
		}
		if (val & 0x80)
			ret |= spi->dev->dio(slave, 1);
		else
			ret |= spi->dev->dio(slave, 0);
		if (!(spi->dev->mode & 1)) {
			spi->dev->clock(slave, clk);
			spi->dev->clock(slave, (~clk) & 1);
		}
		val <<= 1;
	}
	return ret;
}

int  spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *dout,
		void *din, unsigned long flags)
{
	struct bb_spi *spi = to_bb_spi(slave);
	const unsigned char *tx_data = dout;
	unsigned char *rx_data = din;
	unsigned int len = bitlen / 8;

	if ((flags & SPI_XFER_BEGIN) && spi->dev->startstop)
		spi->dev->startstop(slave, 1);
	for (; len > 0; len--) {
		unsigned char d;
		d = (tx_data)?*tx_data++:0xff;
		d = xfer_byte(slave, d);
		if (rx_data) {
			*rx_data++ = d;
		}
	}
	if ((flags & SPI_XFER_END) && spi->dev->startstop)
		spi->dev->startstop(slave, 0);
	return 0;
}

void spi_cs_activate(struct spi_slave *slave)
{
	struct bb_spi *spi = to_bb_spi(slave);
	if (spi->dev->cs)
		spi->dev->cs(slave, 1);
}
void spi_cs_deactivate(struct spi_slave *slave)
{
	struct bb_spi *spi = to_bb_spi(slave);
	if (spi->dev->cs)
		spi->dev->cs(slave, 0);
}

int  spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	struct list_head *entry;
	struct bb_spi_dev *dev;

	list_for_each(entry, &bb_spi_devs) {
		dev = list_entry(entry, struct bb_spi_dev, link);
		if((dev->_bus == bus) && (dev->_cs == cs)) {
			return 1;
		}
	}
	return 0;
}

void bb_spi_register_dev(struct bb_spi_dev *dev)
{
	list_add_tail(&dev->link, &bb_spi_devs);
}
