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

	.h8300s

	.section	.vector,"ax"
	.long	0x200
	.long	0x200
vector	=	2
	.rept	126
	.long	__ram_vec + (vector * 4)
vector	=	vector + 1
	.endr

	.global	_start
	.text
	.align	2
_start:
	mov.l	#__init_stack,sp
	bsr	1f
1:
	mov.l	@sp+,er0
	mov.l	#_lowlevel_init,er1
	sub.l	#1b,er1
	add.l	er1,er0
	jsr	@er0

#if !defined(CONFIG_H8300_SIM)
#if defined(CONFIG_SYS_NO_FLASH)
	/* .data setup */
	mov.l	#__fdata,er5
	mov.l	#__ram_data,er6
	mov.l	#__sizeof_data,er4
	eepmov.w
#else
	/* relocate */
	bsr	1f
1:	
	mov.l	@sp+,er5
	sub.l	#1b-_start,er5
	mov.l	#CONFIG_SYS_TEXT_BASE,er6
	mov.l	#_bss_start,er4
	sub.l	er6,er4
	shlr	#2,er4
1:	mov.l	@er5,er0
	mov.l	er0,@er6
	adds	#4,er5
	adds	#4,er6
	dec.l	#1,er4
	bne	1b
#endif
	mov.l	#_bss_start,er4
	mov.l	#_bss_end,er5
	sub	er4,er5
	shlr.l	#2,er5
	mov	#0,er0
1:	mov.l	er0,@er4		/* bss clear */
	adds	#4,er4
	dec.l	#1,er5
	bne	1b
#endif
	mov.l	#CONFIG_SYS_LOAD_ADDR - CONFIG_SYS_GBL_DATA_SIZE - CONFIG_SYS_MALLOC_LEN,er0
	mov.l	er0,sp
	jsr	@_h8300_generic_init
	bra	.

	.end

