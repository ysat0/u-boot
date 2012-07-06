/*
 * FM3 On-Chip Flash memory driver 
 * Copyright (C) 2012 Yoshinori Sato <ysato@users.sourceforge.jp>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

	.section	.text2, "ax"
	.syntax	unified
	.thumb_func
.globl erase_sector
erase_sector:
	push	{r1, r2, r3}
	ldr	r1, =0x40000000
	movs	r2, #0x01
	str	r2, [r1]
	ldr	r2, [r1]
	movw	r1, 0x1550
	movs	r2, #0xaa
	strh	r2, [r1]
	lsrs	r1, r1, #1
	lsrs	r2, r2, #1
	strh	r2, [r1]
	lsls	r1, r1, #1
	movs	r3, #0x80
	strh	r3, [r1]
	lsls	r2, r2, #1
	strh	r2, [r1]	
	lsrs	r1, r1, #1
	lsrs	r2, r2, #1
	strh	r2, [r1]
	movs	r3, #0x30
	strh	r3, [r0, r4]
	ldrh	r3, [r0, r4]
2:	ldrh	r1, [r0, r4]
	ldrh	r2, [r0, r4]
	eors	r1, r2
	lsrs	r1, r1, #7
	bcc	3f
	lsrs	r2, r2, #4
	bcc	2b
	/* Erase Timeout */
	movs	r0, #2
	b	4f
3:
	/* Erase Complete */
	subs	r0, r0
4:
	ldr	r1, =0x40000000
	movs	r2, #0x02
	str	r2, [r1]
	ldr	r2, [r1]
	pop	{r1, r2, r3}
	mov	pc, lr

	.thumb_func
.globl write_sector
write_sector:
	push	{r1, r2, r3, r4, r5}
	ldr	r3, =0x40000000
	movs	r4, #0x01
	str	r4, [r3]
	ldr	r4, [r3]
1:
	ldrh	r3, [r1]
	add	r3, r3, #1
	movt	r3, #0
	tst	r3, r3
	beq	3f
	movw	r3, 0x1550
	movs	r4, #0xaa
	strh	r4, [r3]
	lsrs	r3, r3, #1
	lsrs	r4, r4, #1
	strh	r4, [r3]
	lsls	r3, r3, #1
	movs	r4, #0xa0
	strh	r4, [r3]
	ldrh	r4, [r1]
	strh	r4, [r0]
	ldrh	r3, [r0]
2:
	ldrh	r3, [r0]
	cmp	r3, r4
	beq	3f
	tst	r3, #0x0020
	beq	2b
	ldrh	r4, [r1]
	cmp	r3,r4
	beq	3f
	movs	r0, #1
	b	4f
3:
	adds	r0, r0, #2
	adds	r1, r1, #2
	subs	r2, r2, #2
	bne	1b
	subs	r0, r0, r0
4:	
	ldr	r1, =0x40000000
	movs	r2, #0x02
	str	r2, [r1]
	ldr	r2, [r1]
	pop	{r1, r2, r3, r4, r5}
	mov	pc, lr

	.end
