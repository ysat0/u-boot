/*
 *	start.S - armv7m startup
 *
 * Copyright (c) 2012 Yoshinori Sato <ysato@users.sourceforge.jp>
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
 */

#include <asm-offsets.h>
#include <config.h>
#include <version.h>
#include <asm/system.h>
#include <linux/linkage.h>
	
	.global	halt
	.global gd_init
	.global	lowlevel_init
	.global	board_init_f
	.global _start

	/*.code	16*/
	.syntax	unified

	.section	".vector", "a"
	.word	CONFIG_SYS_INIT_SP_ADDR
	.word	_start
	.word	halt
	.word	halt

	.text
	.thumb_func
_start:
	bl	lowlevel_init
	ldr	r1, =__relocation_start
	ldr	r2, =CONFIG_SYS_SRAM_BASE
	ldr	r3, =__u_boot_cmd_end
	subs	r3, r3, r2
1:	subs	r3, r3,#4
	ldr	r4, [r1, r3]
	str	r4, [r2, r3]
	tst	r3, r3
	bne	1b
	ldr	r0, =__bss_start
	ldr	r1, =__bss_end__
	subs	r1, r1, r0
	subs	r2, r2, r2
2:	str	r2, [r0]
	adds	r0, #4
	subs	r1, #4
	bne	2b
	b	board_init_f
10:	.word	lowlevel_init
	
ENTRY(relocate_code)
	mov	sp, r0
	mov	r0, r1
	mov	r1, r2
	b	board_init_r

.globl _TEXT_BASE
_TEXT_BASE:
	.word	CONFIG_SYS_TEXT_BASE

.globl _bss_start_ofs
_bss_start_ofs:
	.word __bss_start - _start

.global	_image_copy_end_ofs
_image_copy_end_ofs:
	.word 	0

.globl _bss_end_ofs
_bss_end_ofs:
	.word	0

.globl _end_ofs
_end_ofs:
	.word _end - _start
/* IRQ stack memory (calculated at run-time) + 8 bytes */
.globl IRQ_STACK_START_IN
IRQ_STACK_START_IN:
	.word	0x0badc0de

	.end
