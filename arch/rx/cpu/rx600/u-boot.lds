/*
 * Copyright (C) 2011 Yoshinori Sato
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

#include <config.h>
OUTPUT_FORMAT("elf32-rx-le")
OUTPUT_ARCH(rx)
ENTRY(_start)

MEMORY {
	iram : ORIGIN = 0x0000100, LENGTH = 96K - 256
	sdram : ORIGIN = 0x08000000, LENGTH = 16M
	/* 0xffbf20 - 0xffc720 using flash writer */
	irom : ORIGIN = 0xfff80000, LENGTH = 512K - 128
	vector : ORIGIN = 0xffffff80, LENGTH = 128
}

SECTIONS
{
#if !defined(CONFIG_RX600_DEBUG_SDRAM)
	. = 0xfff80000;
#else
	. = 0x08000000;
#endif
	PROVIDE (_ftext = .);
	PROVIDE (_fcode = .);
	PROVIDE (_start = .);

	.text :
	{
		KEEP(arch/rx/cpu/rx600/start.o (.text))
		*(.text)
		. = ALIGN(4);
	} > irom
	PROVIDE (_ecode = .);
	.rodata :
	{
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
		. = ALIGN(4);
	} > irom
	.u_boot_list : {
		KEEP(*(SORT(.u_boot_list*)));
	} > irom
	PROVIDE (_etext = .);

#if !defined(CONFIG_RX600_DEBUG_SDRAM)
	. = 0xffffff80;
	.vector :
	{
		*(.vector);
	} > vector

	PROVIDE (__ram_vec = 0x00000000);

#endif
	.data : AT(_etext)
	{
		PROVIDE (_fdata = .);
		*(.data)
		. = ALIGN(4);
		PROVIDE (_edata = .);
	} > iram

	PROVIDE (bss_start = .);
	PROVIDE (__bss_start = .);
	.bss :
	{
		*(.bss)
		. = ALIGN(4);
	} > iram
	PROVIDE (bss_end = .);

	PROVIDE (__stack = 0x00018000);

	PROVIDE (_end = .);
}
