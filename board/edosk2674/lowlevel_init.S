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
#if CONFIG_SYS_TEXT_BASE == 0
	mov.l	#init_table,er4
	mov.w	#0xffff,e5
1:	mov.w	@er4+,r5
	beq	2f
	mov.w	@er4+,r0
	mov.b	r0l,@er5
	bra	1b
2:
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
#endif
	rts

	.align	2
init_table:
	.word	0xfec1,0xfb
	.word	0xfec6,0x00
	.word	0xfec0,0x80
	.word	0xfec2,0x27
	.word	0xfec3,0x77
	.word	0xfec4,0x71
	.word	0xfec5,0x22
	.word	0xfec8,0x80
	.word	0xfec9,0x80
	.word	0xfeca,0xa0
	.word	0xfecb,0xa0
	.word	0xfe22,0x3a
	.word	0xfe3c,0x06
	.word	0xfe29,0xff
	.word	0xfe2e,0xfe
	.word	0xfe2f,0x0f
	.word	0xff74,0x0f
	.word	0xfe32,0xff
	.word	0xfe34,0x0d
	.word	0

	.end
