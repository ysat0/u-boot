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
	.global _h8300_flash_setup
	.global	_h83069_flash_write
	.global	_h83069_flash_wbuf

DRCRA	=	0xfee026
FCCS	=	0xfee0b0
FPCS	=	0xfee0b1
FECS	=	0xfee0b2
FKEY	=	0xfee0b4
FTDAR	=	0xfee0b6
TCSR	=	0xffff8c
TCNT	=	0xffff8c
RSTCSR	=	0xffff8e
	
DPFR	=	0xffbf20
INITENT	=	0xffbf20 + 32
MAINENT	=	0xffbf20 + 16

	.text
_h8300_flash_setup:
	mov.l	er4,@-sp
	mov.l	er5,@-sp
	mov.l	er6,@-sp
	mov.l	#__iram_text,er5
	mov.l	#__iram_top,er6
	mov.l	#__text2_end,er4
	sub.l	er6,er4
	eepmov.w
	mov.l	@sp+,er6
	mov.l	@sp+,er5
	mov.l	@sp+,er4
	rts
	
	.section	".text2","ax"
load_progs:
	mov.b	#0x03,r1l
	mov.b	r1l,@FTDAR
	mov.b	#0xa5,r1l
	mov.b	r1l,@FKEY
	mov.b	#1,r1l
	mov.b	r1l,@FCCS
	nop
	nop
	nop
	nop
	mov.b	#0x00,r1l
	mov.b	r1l,@FKEY
	mov.b	@DPFR,r1l
	beq	1f
	sub.l	er0,er0
	mov.b	r1l,r0l
	rts
1:	sub.l	er1,er1
	mov.l	@freq,er0
	jsr	@INITENT
	sub.w	e0,e0
	mov.b	#0,r0h
	rts

_h83069_flash_write:
	mov.l	er0,@freq
	mov.l	er1,@dest
	mov.b	#1,r3l
	mov.b	r3l,@FECS
	mov.l	er1,er3
	jsr	@load_progs
	mov.l	r0l,r0l
	beq	1f
	rts
1:	
	/* disable dram */
	/* can't return to caller */
	mov.l	#flash_stack,sp
	orc	#0x80,ccr
	mov.b	#0,r0l
	mov.b	r0l,@DRCRA
	/* erase block */
	mov.b	#0x5a,r0l
	mov.b	r0l,@FKEY
	mov.l	er3,er0
	mov.b	r0h,r0l
	shlr.b	r0l
	shlr.b	r0l
	shlr.b	r0l
	shlr.b	r0l
	jsr	@MAINENT
	mov.b	r0l,r0l
	bne	2f
	/* write block */
	mov.b	#1,r3l
	mov.b	r3l,@FPCS
	mov.l	er1,er3
	mov.l	er4,er0
	jsr	@load_progs
	mov.l	er0,er0
	bne	2f
	mov.b	#0x5a,r0l
	mov.b	r0l,@FKEY
	mov.l	#_h83069_flash_wbuf,er3
	mov.l	er3,@src
	mov.l	@dest,er1
1:	mov.l	@src,er0
	jsr	@MAINENT
	mov.b	r0l,r0l
	bne	2f
	mov.l	@dest,er1
	add.l	#128,er1
	mov.l	er1,@dest
	mov.l	@src,er3
	add.l	#128,er3
	mov.l	er3,@src
	sub.l	#128,er2
	bne	1b
2:	sub.w	r0,r0
	mov.b	r0l,@FKEY
	/* waiting recovery */
3:	dec.w	#1,r0
	bne	3b
	/* force restart */
	mov.w	#0x5aff,r0
	mov.w	r0,@TCNT
	mov.w	#0xa578,r0
	mov.w	r0,@TCSR
	bra	.
	
__text2_end:	
	
	.section	".bss2","w"
freq:	.space	4
src:	.space	4
dest:	.space	4
_h83069_flash_wbuf:
	.space	4096
	.space	256
flash_stack:	

	.end
