/*
 * (C) Copyright 2011 Yoshinori Sato
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
#include <version.h>

#include <asm/processor.h>

	.global	lowlevel_init

	.text
	.align	2

lowlevel_init:
	mov.l	#0x00080020,r1
	mov.l	#0x00010100,[r1]
	mov.l	#0x00080010,r1
	mov.l	#0x4fffffbf,[r1]
	mov.l	#0x00080014,r1
	mov.l	#0x7fffffff,[r1]
	mov.l	#0x0008c062,r1
	bset	#1,[r1].b
	rts
	.align 2
