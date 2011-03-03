/* H8300 flash driver
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

extern void h8300_flash_setup(void);
extern int h83069_flash_write(int freq, unsigned int addr, unsigned int size);
extern char h83069_flash_wbuf[];

#define AREA_START (0x1000)
#define AREA_END   (0x1fff)

void dataflash_print_info (void)
{
	printf("H8/3069 Internal Flash\n");
	printf("0x%06x - 0x%06x\n", AREA_START, AREA_END);
}

int addr_dataflash (unsigned long addr)
{
	return (addr >= AREA_START) && (addr <= AREA_END);
}

int dataflash_real_protect (int flag, unsigned long start_addr,
				unsigned long end_addr)
{
	/* Proect not supported */
	return -1;
}

int read_dataflash (unsigned long addr, unsigned long size, char *result)
{
	memcpy(result, (char *)addr, size);
	return 0;
}

int write_dataflash (unsigned long addr_dest, unsigned long addr_src,
			unsigned long size)
{
	if (size > (AREA_END - AREA_START + 1) || !addr_dataflash(addr_dest)) {
		dataflash_perror(ERR_INVAL);
		return 1;
	}
	if ((__raw_readb(0xfee0b0) & 0x80) == 0) {
		dataflash_perror(ERR_PROTECTED);
		return 1;
	}
	memcpy(h83069_flash_wbuf, (char *)addr_src, size);
	puts("Force restart after writing\n");
	h8300_flash_setup();
	return h83069_flash_write(CONFIG_SYS_HZ/10000, addr_dest, size);
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
