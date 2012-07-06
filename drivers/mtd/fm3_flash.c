/*
 * FM3 On-Chip Flash memory driver
 * Copyright (C) 2012 Yoshinori Sato <ysato@users.sourceforge.jp>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

/* The DEBUG define must be before common to enable debugging */
/* #define DEBUG	*/

#include <common.h>
#include <environment.h>
#include <flash.h>
#include <malloc.h>

extern int erase_sector(void *sector_top);
extern int write_sector(void *flash, uchar *src, int size);
static int flash_verbose = 0;

flash_info_t flash_info[] = {
	{
		.size = 0x2000,
		.sector_count = 0,
		.flash_id = FLASH_FM3,
		.start[0] = 0x00000000,
		.protect[0] = 0,
	},
	{
		.size = 0x2000,
		.sector_count = 1,
		.flash_id = FLASH_FM3,
		.start[0] = 0x00002000,
		.protect[0] = 0,
	},
	{
		.size = 0x2000,
		.sector_count = 2,
		.flash_id = FLASH_FM3,
		.start[0] = 0x00004000,
		.protect[0] = 0,
	},
	{
		.size = 0x2000,
		.sector_count = 3,
		.flash_id = FLASH_FM3,
		.start[0] = 0x00006000,
		.protect[0] = 0,
	},
	{
		.size = 0xc000,
		.sector_count = 4,
		.flash_id = FLASH_FM3,
		.start[0] = 0x00008000,
		.protect[0] = 0,
	},
	{
		.size = 0xc000,
		.sector_count = 5,
		.flash_id = FLASH_FM3,
		.start[0] = 0x00014000,
		.protect[0] = 0,
	},
	{
		.size = 0x10000,
		.sector_count = 6,
		.flash_id = FLASH_FM3,
		.start[0] = 0x00020000,
		.protect[0] = 0,
	},
	{
		.size = 0x10000,
		.sector_count = 7,
		.flash_id = FLASH_FM3,
		.start[0] = 0x00030000,
		.protect[0] = 0,
	},
	{
		.size = 0x10000,
		.sector_count = 8,
		.flash_id = FLASH_FM3,
		.start[0] = 0x00040000,
		.protect[0] = 0,
	},
	{
		.size = 0x10000,
		.sector_count = 9,
		.flash_id = FLASH_FM3,
		.start[0] = 0x00050000,
		.protect[0] = 0,
	},
	{
		.size = 0x10000,
		.sector_count = 10,
		.flash_id = FLASH_FM3,
		.start[0] = 0x00060000,
		.protect[0] = 0,
	},
	{
		.size = 0x10000,
		.sector_count = 11,
		.flash_id = FLASH_FM3,
		.start[0] = 0x00070000,
		.protect[0] = 0,
	},
	{
		.size = 0x10000,
		.sector_count = 12,
		.flash_id = FLASH_FM3,
		.start[0] = 0x00080000,
		.protect[0] = 0,
	},
	{
		.size = 0x10000,
		.sector_count = 13,
		.flash_id = FLASH_FM3,
		.start[0] = 0x00090000,
		.protect[0] = 0,
	},
	{
		.size = 0x10000,
		.sector_count = 14,
		.flash_id = FLASH_FM3,
		.start[0] = 0x000a0000,
		.protect[0] = 0,
	},
	{
		.size = 0x10000,
		.sector_count = 15,
		.flash_id = FLASH_FM3,
		.start[0] = 0x000b0000,
		.protect[0] = 0,
	},
	{
		.size = 0x10000,
		.sector_count = 16,
		.flash_id = FLASH_FM3,
		.start[0] = 0x000c0000,
		.protect[0] = 0,
	},
	{
		.size = 0x10000,
		.sector_count = 17,
		.flash_id = FLASH_FM3,
		.start[0] = 0x000d0000,
		.protect[0] = 0,
	},
	{
		.size = 0x10000,
		.sector_count = 18,
		.flash_id = FLASH_FM3,
		.start[0] = 0x000e0000,
		.protect[0] = 0,
	},
	{
		.size = 0x10000,
		.sector_count = 19,
		.flash_id = FLASH_FM3,
		.start[0] = 0x000f0000,
		.protect[0] = 0,
	},
};

/*-----------------------------------------------------------------------
 */
#if defined(CONFIG_ENV_IS_IN_FLASH) || defined(CONFIG_ENV_ADDR_REDUND) || (CONFIG_SYS_MONITOR_BASE >= CONFIG_SYS_FLASH_BASE)
flash_info_t *flash_get_info(ulong base)
{
	int i;
	flash_info_t *info = NULL;

	for (i = 0; i < CONFIG_SYS_MAX_FLASH_BANKS; i++) {
		info = & flash_info[i];
		if (info->size && info->start[0] <= base &&
		    base <= info->start[0] + info->size - 1)
			break;
	}

	return info;
}
#endif

unsigned long flash_sector_size(flash_info_t *info, flash_sect_t sect)
{
	if (sect != (info->sector_count - 1))
		return info->start[sect + 1] - info->start[sect];
	else
		return info->start[0] + info->size - info->start[sect];
}

/*-----------------------------------------------------------------------
 */
int flash_erase (flash_info_t * info, int s_first, int s_last)
{
	int ret;
	__asm__ volatile("mov r0, %1\n\t"
			 "mov lr, pc\n\t"
			 "add lr, #4\n\t"
			 "mov pc, %2\n\t"
			 "mov %0, r0\n\t"
			 :"=r"(ret):"r"(info->start[0]),"r"(erase_sector):"r0","lr");
	if (ret)
		return 2;
	if (flash_verbose)
		putc ('.');

	if (flash_verbose)
		puts (" done\n");

	return 0;
}

void flash_print_info (flash_info_t * info)
{
	printf("Sector %d: %ldKB (%08lx - %08lx)", info->sector_count, info->size / 1024,
	       info->start[0], info->start[0] + info->size -1);
	if (info->sector_count >= CONFIG_SYS_MAX_FLASH_BANKS)
		printf("\n");
}

/*-----------------------------------------------------------------------
 * Copy memory to flash, returns:
 * 0 - OK
 * 1 - write timeout
 * 2 - Flash not erased
 */
int write_buff (flash_info_t * info, uchar * src, ulong addr, ulong cnt)
{
	int i;
	uchar *buf = NULL;
	int unaligned;
	int size;
	int ret = 0;
	if (addr >= info->start[0]) {
		unaligned = addr - info->start[0];
		if (unaligned > 0) {
			buf = malloc(info->size);
			if (buf == NULL)
				return 2;
			memcpy(buf, (void *)info->start[0], info[i].size - unaligned);
		}
		size = cnt - unaligned;
		if (size < 0) {
			size = cnt;
			memcpy(buf + cnt, (void *)(info->start[0] + cnt), info->size - cnt);
		}
		if (buf != NULL) {
			memcpy(buf + unaligned, src, size);
			__asm__ volatile("mov r0, %1\n\t"
					 "mov r1, %2\n\t"
					 "mov r2, %3\n\t"
					 "mov lr, pc\n\t"
					 "add lr, #4\n\t"
					 "mov pc, %4\n\t"
					 "mov %0, r0\n\t"
					 :"=r"(ret)
					 :"r"(info->start[0]),"r"(buf),"r"(info->size),"r"(write_sector)
					 :"r0", "lr");

			free(buf);
			if (ret)
				return ret;
		} else {
			__asm__ volatile("mov r0, %1\n\t"
					 "mov r1, %2\n\t"
					 "mov r2, %3\n\t"
					 "mov lr, pc\n\t"
					 "add lr, #4\n\t"
					 "mov pc, %4\n\t"
					 "mov %0, r0\n\t"
					 :"=r"(ret)
					 :"r"(info->start[0]),"r"(src),"r"(info->size),"r"(write_sector)
					 :"r0", "lr");
			if (ret)
				return ret;
		}
	}
	return 0;
}

/*-----------------------------------------------------------------------
 */

void flash_protect_default(void)
{
	/* Flash protect not supported. */
}

unsigned long flash_init(void)
{
	return CONFIG_SYS_FLASH_SIZE;
}
