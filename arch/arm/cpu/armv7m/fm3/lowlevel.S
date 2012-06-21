/*
 * (C) Copyright 2012 Yoshinori Sato <ysato@users.sourceforge.jp>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <config.h>

	.code	16
	.syntax	unified
	.thumb_func
.globl lowlevel_init
lowlevel_init:
	/* WDT disable */
	ldr	r0, =0x40011000
	ldr	r1, =0x1acce551
	ldr	r2, =0x00000c00
	str	r1, [r0, r2]
	ldr	r1, =0xe5331aae
	str	r1, [r0, r2]
	eors	r1, r1
	str	r1, [r0, #0x08]
	ldr	r0, =0x4003323c
	movs	r1, #0x08
	ldr	r0, =0x4003373c
	strb	r1, [r0]
	ldr	r0, =0x4003323c
	strb	r1, [r0]
	movs	r1, #0x08
	ldr	r0, =0x4003343c
	strb	r1, [r0]
	
	/* clock setup */
	ldr	r2, =0x40010030
	movs	r3, #0x4d
	strb	r3, [r2]
	ldr	r2, =0x40010000
	movs	r3, #2
	strb	r3, [r2]
1:	
	ldr	r3, =0x40010004
	ldrb	r3, [r3]
	lsrs	r3, #2
	bcc	1b
	ldr	r2, =0x40010034
	movs	r3, #6
	strb	r3, [r2]
	ldr	r2, =0x40010038
	movs	r3, #1
	strb	r3, [r2]
	ldr	r2, =0x4001003c
	movs	r3, #35
	strb	r3, [r2]
	ldr	r2, =0x40010000
	movs	r3, #18
	strb	r3, [r2]
2:	
	ldr	r3, =0x40010004
	ldrb	r3, [r3]
	lsrs	r3, #5
	bcc	2b
	ldr	r2, =0x40010010
	movs	r3, #0
	strb	r3, [r2]
	ldr	r2, =0x40010014
	movs	r3, #0x02
	strb	r3, [r2]
	ldr	r2, =0x40010018
	movs	r3, #0x82
	strb	r3, [r2]
	ldr	r2, =0x4001001c
	movs	r3, #0x82
	strb	r3, [r2]
	ldr	r2, =0x40010000
	movs	r3, #82
	strb	r3, [r2]
	movs	r1, #0x00
	mov	pc, lr
	

	.align	5
.globl reset_cpu
reset_cpu:
	ldr	r0, =0xe000ed0c
	ldr	r1, =0x05fa0001
	str	r1, [r0]
1:
	b	1b
	