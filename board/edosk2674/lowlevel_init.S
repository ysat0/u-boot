/*
 * Copyright 2010 Yoshinori Sato <ysato@users.sourceforge.jp>
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

	.h8300s
	.global	_lowlevel_init

	.text
	.align	2

_lowlevel_init:
	bsr	1f
1:	mov.l	@sp+,er4
	mov.l	#init_table,er5
	sub.l	#1b,er5
	add.l	er5,er4
	mov.w	#0xffff,e5
2:	mov.w	@er4+,r5
	beq	3f
	mov.w	@er4+,r0
	mov.b	r0l,@er5
	bra	2b
3:
	sub.w	r0,r0
	mov.w	r0,@0xfed2:16
	mov.w	#0x188,r0
	mov.w	r0,@0xfed4:16
	mov.w	#0x85b4,r0
	mov.w	r0,@0xfed0:16
	mov.b	#0,r1l
	mov.b	r1l,@0x400040
	mov.w	#0x84b4,r0
	mov.w	r0,@0xfed0:16
	rts

	.align	2
init_table:
	.word	0xfec1,0x00fb
	.word	0xfec6,0x0000
	.word	0xfec0,0x0080
	.word	0xfec2,0x0027
	.word	0xfec3,0x0077
	.word	0xfec4,0x0071
	.word	0xfec5,0x0022
	.word	0xfec8,0x0080
	.word	0xfec9,0x0080
	.word	0xfeca,0x00a0
	.word	0xfecb,0x00a0
	.word	0xfe22,0x003a
	.word	0xfe3c,0x0006
	.word	0xfe29,0x00ff
	.word	0xfe2e,0x00fe
	.word	0xfe2f,0x000f
	.word	0xff74,0x000f
	.word	0xfe32,0x00ff
	.word	0xfe34,0x000d
	.word	0xfe31,0x0028
	.word	0xfe16,0x00ff
	.word	0xfe17,0x00ff
	.word	0xfe1c,0x0055
	.word	0xfe1d,0x0055
	.word	0

puthex:
	mov.l	#0xff4008,er1
	mov.b	#8,r2h
1:	mov.b	r0l,r2l
	and.b	#0x0f,r2l
	add.b	#'0',r2l
	cmp	#'9',r2l
	ble	2f
	add	#7,r2l
2:	subs	#1,er1
	mov.b	r2l,@er1
	shlr.l	#2,er0
	shlr.l	#2,er0
	dec.b	r2h
	bne	1b
	mov.l	#0xff4000,er1
	mov.b	#8,r2h
3:	btst	#7,@0x8c:8
	beq	3b
	mov.b	@er1+,r0l
	mov.b	r0l,@0x8b:8
	bclr	#7,@0x8c:8
	dec.b	r2h
	bne	3b
	rts

	.end
