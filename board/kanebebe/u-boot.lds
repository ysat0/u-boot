/*
 * (C) Copyright 2000, 2001
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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

OUTPUT_ARCH(h8300h)
ENTRY(_start)

MEMORY
{
	vector : ORIGIN = 0x000000, LENGTH = 4K
	config : ORIGIN = 0x001000, LENGTH = 28K
	irom : ORIGIN = 0x008000, LENGTH = 480K
	dram : ORIGIN = 0x7C0000, LENGTH = 256K
	/* 0xffbf20 - 0xffc720 using flash writer */
	iram : ORIGIN = 0xffcf20, LENGTH = 12K
}

SECTIONS
{

	.vector :
	{
		*(.vector)
	} > vector
	.config :
	{
		*(.config)
	} > config		
	PROVIDE (_start = .);
	PROVIDE (_ftext = .);
	.text :
	{
		arch/h8300/cpu/h8300h/start.o		(.text)
		. = ALIGN(4);
		*(.text)
		. = ALIGN(4);
	} > irom
	PROVIDE (_ecode = .);
	.rodata :
	{
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
		. = ALIGN(4);
#include <u-boot.lst>
	} > irom
	PROVIDE (_etext = .);

	PROVIDE (_fdata = .);
	.data :
	{
		PROVIDE (_ram_data = .);
		PROVIDE (__u_boot_cmd_start = .);
		*(.u_boot_cmd)
		. = ALIGN(4);
		PROVIDE (__u_boot_cmd_end = .);
		*(.data)
		. = ALIGN(4);
	} >dram AT>irom
	PROVIDE (_sizeof_data = SIZEOF(.data));
	PROVIDE (_edata = .);
	PROVIDE (_iram_text = _fdata + SIZEOF(.data));

	PROVIDE (bss_start = .);
	PROVIDE (__bss_start = .);
	.bss :
	{
		*(.bss)
		. = ALIGN(4);
	} > dram
	PROVIDE (bss_end = .);

	PROVIDE (_end = .);
	.iram_text :
	{
		PROVIDE (_iram_top = .);
		*(.text2)
	} >iram AT>irom
	.iram_bss :
	{
		*(.bss2)
	} >iram AT>irom

	PROVIDE (_init_stack = 0xfffe20);
	PROVIDE (_ram_vec =    0xfffe20);
}
