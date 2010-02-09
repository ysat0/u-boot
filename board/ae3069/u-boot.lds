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
ENTRY(__start)

MEMORY
{
       irom : ORIGIN = 0x000000, LENGTH = 512K
       dram : ORIGIN = 0x400000, LENGTH = 2M
       iram : ORIGIN = 0xffbf20, LENGTH = 16K
}

SECTIONS
{
	PROVIDE (__start = .);
	PROVIDE (__ftext = .);

	.text :
	{
		cpu/h8300h/start.o		(.text)
		. = ALIGN(4);
		*(.text)
		. = ALIGN(4);
	} > irom
	PROVIDE (__ecode = .);
	.rodata :
	{
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
		. = ALIGN(4);
	} > irom
	PROVIDE (__etext = .);

	PROVIDE (___u_boot_cmd_start = .);
	.u_boot_cmd :
	{
		*(.u_boot_cmd)
		. = ALIGN(4);
	} > irom
	PROVIDE (___u_boot_cmd_end = .);

	PROVIDE (__fdata = .);
	.data :
	{
		PROVIDE (__ram_data = .);
	
		*(.data)
		. = ALIGN(4);
	} >dram AT>irom
	PROVIDE (__sizeof_data = SIZEOF(.data));
	PROVIDE (__edata = .);

	PROVIDE (_bss_start = .);
	PROVIDE (___bss_start = .);
	.bss :
	{
		*(.bss)
		. = ALIGN(4);
	} > dram
	PROVIDE (_bss_end = .);

	PROVIDE (__end = .);
	.iram :
	{
	}
	PROVIDE (__ram_vec =    0xfffe20);
	PROVIDE (__init_stack = 0xffff20);
}
