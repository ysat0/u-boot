/*
 * Realtek 8019AS Ethernet
 * (C) Copyright 2002-2003
 * Xue Ligong(lgxue@hotmail.com),Wang Kehao, ESLAB, whut.edu.cn
 * 
 * Copyright (C) 2010 Yoshinori Sato <ysato@users.sourceforge.jp>
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

/*
 * This code works in 8bit mode.
 * If you need to work in 16bit mode, PLS change it!
 */

#include <common.h>
#include <command.h>
#include "rtl8019.h"
#include <net.h>

/* packet page register access functions */

static inline unsigned char get_reg (struct eth_device *dev, unsigned int regno)
{
	return (*(unsigned char *) (dev->iobase + regno));
}

static inline void put_reg (struct eth_device *dev, unsigned int regno, unsigned char val)
{
	*(volatile unsigned char *) (dev->iobase + regno) = val;
}

static inline void remote_read (struct eth_device *dev, unsigned short address, unsigned short len, u8 *buffer)
{
	put_reg (dev, RSAL, address & 0xff);
	put_reg (dev, RSAH, address >> 8);
	put_reg (dev, RBCL, len & 0xff);
	put_reg (dev, RBCH, len >> 8);
	put_reg (dev, CR, CR_RDMA | CR_START);
	for (; len > 0; --len)
		*buffer++ = get_reg (dev, DATA);
}

static inline void remote_write (struct eth_device *dev, unsigned short address, unsigned short len, u8 *buffer)
{
	put_reg (dev, RSAL, address & 0xff);
	put_reg (dev, RSAH, address >> 8);
	put_reg (dev, RBCL, len & 0xff);
	put_reg (dev, RBCH, len >> 8);
	put_reg (dev, CR, CR_WDMA | CR_START);
	for (; len > 0; --len)
		put_reg (dev, DATA, *buffer++);
}

static int rtl8019_halt (struct eth_device *dev)
{
	put_reg (dev, CR, CR_STOP);
	return 0;
}

static int rtl8019_init(struct eth_device *dev, bd_t *bis)
{
	static const struct rtl8019_commands init_seq[] = {
		{CR,     CR_PAGE0 | CR_NODMA |CR_STOP},
		{DCR,    DCR_LS | DCR_FIFO_4},
		{RBCH,   0},
		{RBCL,   0},
		{RCR,    RCR_MON},
		{TCR,    TCR_LOCAL},
		{ISR,    0xff},
		{IMR,    0x7f},
		{TPSR,   TXPAGE},
		{PSTART, RXPAGE},
		{BNDRY,  0x7f},
		{PSTOP,  0x80},
		{CR,     CR_NODMA | CR_PAGE1 | CR_STOP},
		{CURP,   RXPAGE},
		{CR,     CR_NODMA | CR_PAGE0 | CR_STOP},
		{TCR,    TCR_NORMAL},
		{RCR,    RCR_AB}
	};
	int i;

	put_reg (dev, CR, CR_NODMA | CR_PAGE1);
	for (i = 0; i < 6; i++)
		put_reg(dev, PAR0 + i, dev->enetaddr[i]);
	for (i = 0; i < 8; i++)
		put_reg (dev, MAR0 + i, 0x00);
	for (i = 0; i < sizeof(init_seq) / sizeof(init_seq[0]); i++)
		put_reg(dev, init_seq[i].offset, init_seq[i].value);
	return 0;
}

static int rtl8019_rx (struct eth_device *dev)
{
	u8 header[4];
	unsigned short rxlen;
	unsigned char cur,bndry;
	unsigned char isr, cr;
	int resend = 0;

	while (1) {
		if (((isr = get_reg (dev, ISR)) & (ISR_OFLW | ISR_RXP | ISR_RXE)) == 0) {
			put_reg (dev, ISR, isr);
			return 0;
		}
		if (isr & ISR_OFLW) {
			printf(DRIVERNAME ": overflow\n");
			/* overflow */
			cr = get_reg (dev, CR);
			
			put_reg (dev, CR, CR_STOP | CR_NODMA);
			udelay(1600);
			
			put_reg (dev, RBCL, 0);
			put_reg (dev, RBCL, 0);
			
			resend = ((cr & CR_TXPKT) && (isr & (ISR_TXP | ISR_TXE)));
			
			put_reg (dev, TCR, TCR_LOCAL);
			put_reg (dev, cr, CR_NODMA | CR_START);
		}

		put_reg (dev, CR, CR_PAGE1 | CR_NODMA | CR_START);
		cur = get_reg(dev, CURP);
		put_reg (dev, CR, CR_PAGE0 | CR_NODMA | CR_START);
		bndry = get_reg(dev, BNDRY);
		bndry = (++bndry == ENDPAGE)?RXPAGE:bndry;
		if (bndry == cur) {
			bndry = (--bndry < RXPAGE)?ENDPAGE -1:bndry;
			put_reg(dev, BNDRY, bndry);
			goto end;
		}

		remote_read(dev, bndry << 8, sizeof(header), header);
		if (header[0] != 0) {
			rxlen = (header[3] << 8 | header[2]) - sizeof(header);
			remote_read(dev, (bndry << 8) +  sizeof(header), rxlen, NetRxPackets[0]);
			NetReceive (NetRxPackets[0], rxlen);
			bndry = header[1];
			bndry = (--bndry < RXPAGE)?ENDPAGE - 1:bndry;
			put_reg (dev, BNDRY, bndry);
		} else {
			put_reg (dev, CR, CR_NODMA | CR_PAGE1 | CR_START);
			put_reg (dev, CURP, RXPAGE);
			put_reg (dev, CR, CR_NODMA | CR_PAGE0 | CR_START);
			put_reg (dev, BNDRY, ENDPAGE - 1);
		}

	}
end:
	if ((isr & ISR_OFLW) && resend)
		put_reg (dev, CR, CR_NODMA | CR_TXPKT |CR_START);
	put_reg (dev, ISR, isr);
	return 0;
}

/* Send a data block via Ethernet. */
static int rtl8019_send (struct eth_device *dev, volatile void *packet, int length)
{
	while (get_reg (dev, CR) & CR_TXPKT);

	put_reg (dev, ISR, 0x40);
	remote_write(dev, TXPAGE << 8, length, (char *)packet);

	if (length < 60) {
		put_reg (dev, RSAL, length);
		put_reg (dev, RSAH, TXPAGE);
		put_reg (dev, RBCL, (60 - length));
		put_reg (dev, RBCH, 0);
		put_reg (dev, CR, CR_WDMA | CR_START);
		for (; length < 60; length++)
			put_reg (dev, DATA, 0);
	}
	while (!(get_reg (dev, ISR)) & ISR_RDC);

	put_reg (dev, ISR, ISR_RDC | ISR_TXP |ISR_TXE);
	put_reg (dev, TBCL, (length & 0xff));
	put_reg (dev, TBCH, ((length >> 8) & 0xff));
	put_reg (dev, TPSR, TXPAGE);
	put_reg (dev, CR, CR_NODMA | CR_TXPKT | CR_START);
	return 0;
}

static int read_address(struct eth_device *dev)
{
	u8 prom[32];
	int i;
	const static struct rtl8019_commands init_seq[] = {
		{DCR,  0x48},
		{RBCH, 0},
		{RBCL, 0},
		{ISR,  0xFF},
		{IMR,  0x00},
		{RCR,  0x20},
		{TCR,  0x02},
	};
	
	/* reset */
	put_reg(dev, RESET, get_reg(dev, RESET));
	udelay (2000);		/* wait for reset complete */
	if (! (get_reg(dev, ISR) & 0x80))
		return 0;	/* reset failed */
	put_reg(dev, ISR, 0xff);

	for(i = 0; i < sizeof(init_seq) / sizeof(init_seq[0]); i++)
		put_reg(dev, init_seq[i].offset, init_seq[i].value);

	/* read prom */
	remote_read(dev, 0x0000, 32, prom);
	if ((prom[0] == 0xff) && (prom[2] == 0xff) && (prom[4] == 0xff))
		return 0;	/* invalid data */

	for (i = 0;  i < 6; i++)
		dev->enetaddr[i] = prom[i * 2];
	return 1;
}

int rtl8019_initialize(bd_t *bis, int base_adr)
{
	struct eth_device *dev;
	
	dev = malloc(sizeof(*dev));
	if (dev == NULL)
		return 0;

	dev->iobase = base_adr;

	/* read address */
	if (!read_address(dev)) {
		return 0;	/* not found */
	}
	
	dev->init = rtl8019_init;
	dev->halt = rtl8019_halt;
	dev->send = rtl8019_send;
	dev->recv = rtl8019_rx;
	sprintf(dev->name, DRIVERNAME);

	eth_register(dev);
	return 1;
}
