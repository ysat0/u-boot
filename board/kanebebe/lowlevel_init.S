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
	mov.w	#0x00fe,e5	/* 0xfee0xx */
1:
	mov.w	@er4+,r5
	beq	3f
	mov.w	@er4+,r0
	mov.b	r0l,@er5
	bra	1b
3:
	mov.l	#420000,er0	/* wait 50ms */
4:	dec.l	#1,er0
	bpl	4b
	rts

	.align	2
init_table:
	.word	0xe02a,9	/* RTCOR */
	.word	0xe028,0x30	/* RTMCSR */
	.word	0xe027,0x98	/* DRCRB */
	.word	0xe026,0x78	/* DRCRA */
	.word	0xe021,0xf3	/* ASTCR */
	.word	0xe022,0xff	/* WCRH */
	.word	0xe023,0x08	/* WCRL */
	.word	0xe01f,0x08	/* CSCR */
	.word	0xe000,0xff	/* P1DDR */
	.word	0xe001,0xff	/* P2DDR */
	.word	0xe004,0xf1	/* P5DDR */
	.word	0xe007,0xee	/* P8DDR */
	.word	0xe009,0xff	/* PADDR */
	.word	0

	.end
