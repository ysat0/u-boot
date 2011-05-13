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
	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff

	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff

	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff
	.long	__ram_vec + (20 * 4)	/* 20 privillege exception */
	.long	0xffffffff
	.long	0xffffffff
	.long	__ram_vec + (23 * 4)	/* 23 illigal instruction */

	.long	0xffffffff
	.long	__ram_vec + (25 * 4)	/* 25: floating point exception */
	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff
	.long	0xffffffff
	.long	__ram_vec + (30 * 4)	/* 30: nmi */
	.long	_start

	.text
	.align	2
_start:
	mov.l	#__stack, r0
	bsr	lowlevel_init

	mov.l	#_etext,r2
	mov.l	#_fdata ,r1
	mov.l	#__u_boot_cmd_end - _fdata,r3
	smovf
	mov.l	#bss_start,r1
	mov.l	#bss_end,r3
	sub	r1,r3
	mov.l	#0,r2
	sstr.b
	mov.l	#bss_end,r13		/* global data */
	bsr	rx_generic_init
	
	bra	.

	.end

