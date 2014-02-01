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
       from : ORIGIN = 0x000000, LENGTH = 512K
       sdram : ORIGIN = 0x400000, LENGTH = 2M
       iram : ORIGIN = 0xffbf20, LENGTH = 16K
}

SECTIONS
{
	PROVIDE (_start = .);
	PROVIDE (_ftext = .);

	.text :
	{
		*(.vector)
		arch/h8300/cpu/h8300h/start.o		(.text)
		. = ALIGN(4);
		*(.text)
		. = ALIGN(4);
	} > from
	PROVIDE (_ecode = .);
	.rodata :
	{
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
		. = ALIGN(4);
#include "u-boot.lst"
	} > from
	PROVIDE (_etext = .);

	PROVIDE (_fdata = .);
	.data :
	{
		PROVIDE (_ram_data = .);
	
		*(.data)
		. = ALIGN(4);
	} >sdram AT>from
	PROVIDE (_sizeof_data = SIZEOF(.data));
	PROVIDE (_edata = .);

	PROVIDE (bss_start = .);
	PROVIDE (__bss_start = .);
	.bss :
	{
		*(.bss)
		. = ALIGN(4);
	} > sdram
	PROVIDE (bss_end = .);

	PROVIDE (_end = .);
	.iram :
	{
	}
	PROVIDE (_ram_vec =    0xffbe00);
	PROVIDE (_init_stack = 0xffc000);
}
