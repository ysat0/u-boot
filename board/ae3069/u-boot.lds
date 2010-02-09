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
	} =0xFF
	PROVIDE (__ecode = .);
	.rodata :
	{
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
		. = ALIGN(4);
	}
	PROVIDE (__etext = .);


	PROVIDE (__fdata = .);
	.data :
	{
		*(.data)
		. = ALIGN(4);
	}
	PROVIDE (__edata = .);

	PROVIDE (__fgot = .);
	.got :
	{
		*(.got)
		. = ALIGN(4);
	}
	PROVIDE (__egot = .);

	PROVIDE (___u_boot_cmd_start = .);
	.u_boot_cmd :
	{
		*(.u_boot_cmd)
		. = ALIGN(4);
	}
	PROVIDE (___u_boot_cmd_end = .);

	PROVIDE (reloc_dst_end = .);

	PROVIDE (_bss_start = .);
	PROVIDE (___bss_start = .);
	.bss :
	{
		*(.bss)
		. = ALIGN(4);
	}
	PROVIDE (_bss_end = .);

	PROVIDE (__end = .);
}
