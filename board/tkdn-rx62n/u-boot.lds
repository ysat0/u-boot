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

OUTPUT_FORMAT("elf32-rx-linux")
OUTPUT_ARCH(rx)
ENTRY(_start)

SECTIONS
{
	. = 0xfff80000;

	PROVIDE (_ftext = .);
	PROVIDE (_fcode = .);
	PROVIDE (_start = .);

	.text :
	{
		arch/rx/cpu/rx600/start.o		(.text)
		*(.text)
		. = ALIGN(4);
	} = 0xff
	PROVIDE (_ecode = .);
	.rodata :
	{
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
		. = ALIGN(4);
	}
	PROVIDE (_etext = .);

	. = 0xffffff80;
	.vector :
	{
		*(.vector);
	}

	PROVIDE (__ram_vec = 0x00000000);

	. = 0x00001000;
	PROVIDE (_fdata = .);
	.data : AT(_etext)
	{
		*(.data)
		. = ALIGN(4);
	}
	PROVIDE (_edata = .);

	PROVIDE (__u_boot_cmd_start = .);
	.u_boot_cmd :
	{
		*(.u_boot_cmd)
		. = ALIGN(4);
	}
	PROVIDE (__u_boot_cmd_end = .);

	PROVIDE (bss_start = .);
	PROVIDE (__bss_start = .);
	.bss :
	{
		*(.bss)
		. = ALIGN(4);
	}
	PROVIDE (bss_end = .);

	PROVIDE (__stack = 0x00018000);

	PROVIDE (_end = .);
}
