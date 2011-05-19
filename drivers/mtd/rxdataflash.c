/* RX data flash driver
 * Copyright 2011 Yoshinori Sato
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
 *
 */

#include <common.h>
#include <config.h>
#include <dataflash.h>
#include <asm/io.h>

static int fcu_initdone = 0;

static void fcu_init(void)
{
	if (fcu_initdone && !(__raw_readb(0x7fffb1) & 0x80))
		return ;
	__raw_writew(0, 0x7fffb2);
	__raw_writew(0xc401, 0x7fc454);
	memcpy(0x7f8000, 0xfeffe000, 0x2000);
	fcu_initdone = 1;
}

static int wait_frdy(void)
{
	int i;
	for (i = 0; i < 10; i++) {
		if (__raw_readb(0x7fffb0) & 0x80)
			return 0;
		udelay(500);
	}
	return -1;
}
	
static int dataflash_pe_mode(unsigned long addr)
{
	unsigned char status;
	__raw_writew(0xaa00, 0x7fffb2);
	__raw_writew(0xaa80, 0x7fffb2);
	if (__raw_readb(0x7fffb0) & 0x40) {
		if (__raw_readb(0x7fc410) != 0x10)
			__raw_writeb(0x10, 0x7fc410);
	}
	__raw_writeb(0x50, addr);
	__raw_writeb(0x01, 0x0008c289);
	__raw_writew(CONFIG_SYS_CLK_FREQ / 1000000, 0x7fffe8);
	__raw_writeb(0xe9, addr);
	__raw_writeb(0x03, addr);
	__raw_writew(0x0f0f, addr);
	__raw_writew(0x0f0f, addr);
	__raw_writew(0x0f0f, addr);
	__raw_writeb(0xd0, addr);
	if(wait_frdy() < 0)
		return -1;
	__raw_writew(0x2d00, 0x7fc440);
	__raw_writew(0x1eff, 0x7fc450);
	__raw_writew(0xd200, 0x7fc442);
	__raw_writew(0xe1ff, 0x7fc452);
	return 0;
}

void dataflash_print_info (void)
{
	printf("RX internal data flash\n");
	printf("0x%06x - 0x%06x\n", DATAFLASH_START, DATAFLASH_END);
}

int addr_dataflash (unsigned long addr)
{
	return (addr >= DATAFLASH_START) && (addr <= DATAFLASH_END);
}

int dataflash_real_protect (int flag, unsigned long start_addr,
				unsigned long end_addr)
{
	unsigned char bflg[2] = {0, 0};
	unsigned char dflwe;
	int bank;
	int reg;
	
	/* addr to bank */
	start_addr >>=11;
	end_addr = (end_addr + 0x800) >> 11;
	if (start_addr <= end_addr)
		return -1;

	for (bank = start_addr; bank < end_addr; bank++)
		bflg[bank / 8] != 1 << (bank % 8);
	for (reg = 0; reg < 2; reg++) {
		dflwe = __raw_readw(0x7fc540 + reg * 2) & 0xff;
		if (flag)
			dflwe &= ~bflg[reg];	/* protect */
		else
			dflwe |= bflg[reg];	/* unprotect */
		__raw_writew(0xe100 | dflwe, 0x7fc540 + reg * 2);
	}
	return end_addr - start_addr;
}

static int is_protect(unsigned long addr, unsigned int size)
{
	unsigned short dflwe;
	dflwe = (__raw_readw(0x7fc542) & 0xff) << 8 |
		(__raw_readw(0x7fc540) & 0xff);
	addr >>= 11;
	size = (size >> 11) + 1;
	for (;size > 0; size--) {
		if ((dflwe & (1 << addr)) == 0)
			return -1;
	}
	return 0;
}

static int erase (unsigned long addr, unsigned int size)
{
	int i;
	size = (size >> 11) + 1;
	for(; size > 0; size--) {
		__raw_writeb(0x20, addr);
		__raw_writeb(0xd0, addr);
		if (wait_frdy() < 0)
			return -1;
		addr += 0x800;
	}
	return 0;
}

static int write (unsigned long dst, unsigned long src, unsigned int size)
{
	unsigned short *p = (unsigned short *)src;
	int s;
	while(size > 0) {
		__raw_writeb(0xe8, dst);
		__raw_writeb(0x40, dst);
		for (s = 0; s < 64; s++) {
			if (size > 0)
				__raw_writew(*p++, dst);
			else
				__raw_writew(0xffff, dst);
			dst += 2;
			size -= 2;
		}
		__raw_writeb(0xd0, dst);
		if (wait_frdy() < 0)
			return -1;
	}
	return 0;
}

int read_dataflash (unsigned long addr, unsigned long size, char *result)
{
	memcpy(result, (char *)addr, size);
	return 0;
}

int write_dataflash (unsigned long addr_dest, unsigned long addr_src,
			unsigned long size)
{
	if (size > (DATAFLASH_END - DATAFLASH_START + 1) ||
	    !addr_dataflash(addr_dest)) {
		dataflash_perror(ERR_INVAL);
		return 1;
	}
	fcu_init();
	if(dataflash_pe_mode(addr_dest) < 0) {	
		dataflash_perror(ERR_PROG_ERROR);
		return 1;
	}
	if (erase(addr_dest, size) < 0) {
		dataflash_perror(ERR_PROG_ERROR);
		return 1;
	}
	if (write(addr_dest, addr_src, size) < 0) {
		dataflash_perror(ERR_PROG_ERROR);
		return 1;
	}
	__raw_writeb(0x02, 0x0008c289);
	__raw_writew(0xaa00, 0x7fffb2);

	return 0;
}

void dataflash_perror (int err)
{
	switch (err) {
	case ERR_OK:
		break;
	case ERR_PROTECTED:
		printf("Can't write to protected/invalid DataFlash sectors\n");
		break;
	case ERR_INVAL:
		printf("Outside available DataFlash\n");
		break;
	case ERR_PROG_ERROR:
		printf("General DataFlash Programming Error\n");
		break;
	default:
		printf("%s[%d] FIXME: rc=%d\n", __FILE__, __LINE__, err);
		break;
	}
}
