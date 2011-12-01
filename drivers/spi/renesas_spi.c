/*
 * Renesas SPI driver
 *
 * Copyright (C) 2011 Yoshinori Sato
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <asm/io.h>

struct rspi_slave {
	struct spi_slave spi;
	unsigned short cmd0;
	unsigned char brr;
};

int spi_init(void)
{
	int i;
	int offset;
	/* disable interfaces */
	for (i = 0, offset = 0; i < RSPI_CHANNEL; i++) { 
		writeb(0x00, RSPI_BASE + offset);
		offset += 0x20;
	}
	return 0;
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
				  unsigned int max_hz, unsigned int mode)
{
	struct rspi_slave *rs;
	int brr;
	int div;

	if (!spi_cs_is_valid(bus, cs))
		return NULL;

	rs = malloc(sizeof(struct rspi_slave));
	if (!rs)
		return NULL;

	rs->spi.bus = bus;
	rs->spi.cs = cs;
	brr = CONFIG_RSPI_CLK / max_hz;
	div = 1;
	while (brr < 256) {
		brr /= 2;
		div++;
	}
	rs->brr = brr + 1;
	rs->cmd0 = ((mode & 0x08) << 8) | 0x0700 | ((div - 1) << 2) | (mode & 3);
	return &(rs->spi);
}

void spi_free_slave(struct spi_slave *ss)
{
	free(ss);
}

int spi_claim_bus(struct spi_slave *slave)
{
	return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
	unsigned long offset = 0x20 * slave->bus;
	writeb(readb(CONFIG_RSPI_BASE + offset) & ~0x40, CONFIG_RSPI_BASE + offset);
}

int  spi_xfer(struct spi_slave *slave, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	struct rspi_slave *rs = (struct rspi_slave *)slave;
	unsigned long offset = 0x20 * slave->bus;
	
	if (flags & SPI_XFER_BEGIN) {
		/* RSPI initalize */
		writeb(0x00, CONFIG_RSPI_BASE + offset);
		writeb(0x00, CONFIG_RSPI_BASE + offset + 0x02);
		writeb(0x00, CONFIG_RSPI_BASE + offset + 0x08);
		writeb(0x20, CONFIG_RSPI_BASE + offset + 0x0b);
		writew(rs->cmd0, CONFIG_RSPI_BASE + offset + 0x10);
		writeb(rs->brr, CONFIG_RSPI_BASE + offset + 0x0a);
		writeb(0x49, CONFIG_RSPI_BASE + offset);

		spi_cs_activate(slave);
	}

	while (bitlen > 0) {
		writel(*dout++, CONFIG_RSPI_BASE + offset + 0x04);
		while((readb(CONFIG_RSPI_BASE + offset + 0x03) & 0x80) == 0) {
			if (ctrlc())
				return -1;
			usleep(10);
		}
		if (din)
			*din++ = readl(CONFIG_RSPI_BASE + offset + 0x04);
		else
			readl(CONFIG_RSPI_BASE + offset + 0x04);

		bitlen -= 8;
	}

	if (flags & SPI_XFER_END)
		spi_cs_deactivate(slave);

	return 0;
}

