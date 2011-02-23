/*
 * Copyright (C) 2011 Yoshinori Sato <ysato@users.sourceforge.jp>

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

	.section	".vector","ax"
	.global	_start
vector	=	1
	.rept	31
	.long	__ram_vec + (vector * 4)
vector	=	vector + 1
	.endr
	.long	_start

	.text
	.align	2
_start:
	mov.l	#bss_end, r0
	bsr	lowlevel_init

	mov.l	#_etext,r1
	mov.l	#_fdata ,r2
	mov.l	#_edata - _fdata,r3
	/*smovf*/
	mov.l	#bss_start,r1
	mov.l	#bss_end,r3
	sub	r1,r3
	mov.l	#0,r2
	sstr.b
	mov.l	#0x1000, r0
	mov.l	#bss_end,r13		/* global data */
	bsr	rx_generic_init
	
	bra	.

	.end

