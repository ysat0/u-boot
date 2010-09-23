/*
 * Copyright (C) 2010 Yoshinori Sato <ysato@users.sourceforge.jp>

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

	.h8300h
	.text
	.align	2

	.global	_start
	.long	_start
vector	=	1
	.rept	63
	.long	__ram_vec + (vector * 4)
vector	=	vector + 1
	.endr

_start:
	mov.l	#__init_stack,sp
#if !defined(CONFIG_H8300_SIM)
	jsr	@_lowlevel_init

	mov.l	#__fdata,er5
	mov.l	#__ram_data,er6
	mov.l	#__sizeof_data,er4
	eepmov.w
	mov.l	#_bss_start,er4
	mov.l	#_bss_end,er5
	sub	er4,er5
	mov	#0,er0
1:	mov.l	er0,@er4		/* bss clear */
	adds	#4,er4
	dec.l	#1,er5
	bne	1b
#endif
	mov.l	#CONFIG_SYS_LOAD_ADDR - CONFIG_SYS_GBL_DATA_SIZE - CONFIG_SYS_MALLOC_LEN,sp
	mov.l	sp,er0		/* global data */
	jsr	@_h8300_generic_init
	
	bra	.

	.end

