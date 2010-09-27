/*
 * Internal flash access functions for H8/3069
 *
 * Copyright (C) 2010 Yoshinori Sato
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

	.h8300h
	.global	__3069_load_progs
	.global	__3069_erase_block
	.global	__3069_write_block

FCCS	=	0xfee0b0
FPCS	=	0xfee0b1
FECS	=	0xfee0b2
FKEY	=	0xfee0b4
FTDAR	=	0xfee0b6

DPFR	=	0xffbf20
INITENT	=	0xffbf20 + 32
MAINENT	=	0xffbf20 + 16
	
	.section	".text2","ax"
__3069_load_progs:
	mov.b	#1,r3l
	mov.b	r3l,@er1
	mov.b	r3l,@FCCS
	nop
	nop
	nop
	nop
	mov.b	@DPFR,r0l
	bne	1f
	sub.l	er1,er1
	jsr	@INITENT
1:	sub.w	e0,e0
	mov.b	#0,r0h
	rts

__3069_erase_block:
__3069_write_block:
	jsr	@MAINENT
	sub.w	e0,e0
	mov.b	#0,r0h
	rts

	.end
