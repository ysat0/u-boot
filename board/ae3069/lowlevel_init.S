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

	.h8300h
	.global	_lowlevel_init

	.text
	.align	2

_lowlevel_init:
	mov.l	#init_table,er4
1:
	mov.w	@er4,r5
	beq	3f
	mov.w	#0x00ff,e5	/* 0xffffxx */
	cmp.w	#0xff00,r5
	bcc	2f
	dec.w	#1,e5		/* 0xfee0xx */
2:	mov.w	@er4+,r0
	mov.b	r0l,@er5
	bra	1b
3:
	mov.l	#420000,er0	/* wait 50ms */
4:	dec.l	#1,er0
	bpl	4b
	rts

	.align	2
init_table:
	.word	0xe029,9
	.word	0xe02a,0x30
	.word	0xe027,0x98
	.word	0xe026,0x38
	.word	0xe021,0xfb
	.word	0xe022,0xff
	.word	0xe023,0xc8
	.word	0xe000,0xff
	.word	0xe001,0xff
	.word	0xe004,0xf1
	.word	0xe007,0xee
	.word	0

	.end
