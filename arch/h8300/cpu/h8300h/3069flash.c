/*
 * Internal flash access functions for H8/3069
 *
 * Copyright (C) 2010 Yoshinori Sato
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

#include <common.h>
#include <flash.h>
#include <malloc.h>

#define DRCRA (*(volatile unsigned char *)0xfee026)
#define DPFR (*(volatile unsigned char *)0xffbf20)
#define FKEY (*(volatile unsigned char *)0xfee0b4)
#define FTDAR (*(volatile unsigned char *)0xfee0b6)

#define ERASE 0xfee0b2
#define WRITE 0xfee0b1

/* low-level functions */
int _3069_load_progs(int hz, unsigned long progsel);
int _3069_erase_block(int block);
int _3069_write_block(unsigned char *data, unsigned long addr);

flash_info_t flash_info[] = {
	{0x7000,
	 7,
	 0,
	 {0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000},
	 {0, 0, 0, 0, 0, 0, 0, }
	},
};

int flash_erase (flash_info_t * info, int s_first, int s_last)
{
	int block;
	unsigned char drcra;
	unsigned char ccr;
	int rc;

	/* supported block is 1 - 7 */
	if (s_first > s_last || s_first <1 || s_last > 7)
		return 1;

	DPFR = 0xff;
	FKEY = 0xa5;
	FTDAR = 0x03;
	rc = _3069_load_progs(CONFIG_SYS_HZ / 1000, ERASE);
	if (rc == 0) {
		/* diable interrput */
		__asm__ volatile("stc ccr,%w0\n\torc #0x80,ccr" : "=r"(ccr));

		/* disable refresh */
		drcra = DRCRA;
		DRCRA = 0;
		for (block = s_first; block <= s_last; block++) {
			rc = _3069_erase_block(block);
			if (rc != 0)
				break;
		}
		/* restore setting */
		DRCRA = drcra;
		__asm__ volatile("ldc %w0,ccr" :: "r"(ccr));
	} else {
		rc = ERR_PROG_ERROR;
	}

	return rc;
}

int write_buff (flash_info_t * info, uchar * src, ulong addr, ulong cnt)
{
	int rc;
	unsigned char drcra;
	unsigned char ccr;

	DPFR = 0xff;
	FKEY = 0xa5;
	FTDAR = 0x03;
	rc = _3069_load_progs(CONFIG_SYS_HZ / 1000, WRITE);
	if (rc == 0) {
		FKEY = 0x5a;
		/* diable interrput */
		__asm__ volatile("stc ccr,%w0\n\torc #0x80,ccr" : "=r"(ccr));
		/* disable refresh */
		drcra = DRCRA;
		DRCRA = 0;
		while (cnt >= 128) {
			rc = _3069_write_block(src, addr);
			if (rc != 0)
				break;
			src += 128;
			addr += 128;
			cnt -= 128;
		}
		/* restore setting */
		DRCRA = drcra;
		__asm__ volatile("ldc %w0,ccr" :: "r"(ccr));
	} else {
		rc = ERR_PROG_ERROR;
	}

	return rc;
}

void flash_print_info(flash_info_t *info)
{
	int i;
	printf("H8/3069 Internal flash\n");
	printf("  Size: %d KB in %ld Sectors\n",
	       info->size >> 10, info->sector_count);

	printf("  Sector Start Address:\n");
	for(i = 0; i < info->sector_count; i++) {
		printf("%06lX ", info->start[i]);
	}
	printf("\n");
}

unsigned long flash_init(void)
{
	/* nothing do */
	return 0;
}
