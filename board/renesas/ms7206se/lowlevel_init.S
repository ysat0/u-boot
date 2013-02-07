/*
 * Copyright (C) 2013 Yoshinori Sato <ysato@users.sourceforge.jp>
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
#include <version.h>

#include <asm/processor.h>
#include <asm/macro.h>

	.global	lowlevel_init

	.text
	.align	2

lowlevel_init:
	mova	CPG_settings_table,r0
1:	mov.l	@r0+,r3		// Address (or zero)
	tst	r3,r3
	bt	2f
	mov.l	@r0+,r1		// data
	bra	1b
	 mov.w	r1,@r3
2:
	mov.l	.MCLKCR,r0	// MCLKCR setting
	mov	#0x40,r1
	mov.b	r1,@r0

	// Set up the Pin Function Controler
	mova	PFC_settings_table,r0
3:	mov.l	@r0+,r3		       // Address (or zero)
	tst	r3,r3
	bt	4f
	mov.l	@r0+,r1		       // data
	bra	3b
	 mov.w	r1,@r3

	// Set up the Bus Controler
4:
	mova	BSC_settings_table,r0
5:	mov.l	@r0+,r3		       // Address (or zero)
	tst	r3,r3
	bt	6f
	mov.l	@r0+,r1		       // data
	bra	5b
	 mov.l	r1,@r3

6:
	movi20	#2000,r0
7:
	dt	r0
	bf	7b

	mov.l	.SDMR ,r1		// Turns on SDRAM controller
	mov	#0,r2
	mov.l	r2,@r1

	// Initialize FPGA INTC 
	mov	#0,r0
	mov.l	.INTC,r2
	mov.w	r0,@r2			// INTMSK0
	mov.w	r0,@(2,r2)		// INTMSK1
	mov.w	r0,@(4,r2)		// INTSEL

	rts
	  nop

.SDMR:	.long	0xfffc5880
.INTC:	.long	0x31180000
.MCLKCR:	.long	0xfffe410

CPG_settings_table:
	.long	WTCSR, 0xA500
	.long	WTCNT, 0x5A00
	.long	WTCSR, 0xA51E
	.long	FRQCR, 0x120c	/* input:33.3333MHz I:B:P=6:2:1=200MHz:66MHz:33MHz */
	.long	0

PFC_settings_table:
	.long	PACRH3, 0x0022
	.long	PACRH2, 0x1111
	.long	PACRH1, 0x1115
	.long	PACRL4, 0x1111
	.long	PACRL3, 0x1125
	.long	PACRL2, 0x2255
	.long	PACRL1, 0x5500
	.long	PBCR3,  0x0022
	.long	PBCR2,  0x2244
	.long	PBCR1,  0x1111
	.long	PCCRL1, 0x1111
	.long	PDCRH4, 0x1111
	.long	PDCRH3, 0x1111
	.long	PDCRH2, 0x1111
	.long	PDCRH1, 0x1111
	.long	PDCRL4, 0x1111
	.long	PDCRL3, 0x1111
	.long	PECRH1, 0x0005
	.long	PECRL4, 0x3223
	.long	PECRL3, 0x3030
	.long	PECRL2, 0x3550
	.long	IFCR,   0x0003
	.long	0

BSC_settings_table:
	# Settings from Hitachi docs for SE7206
	.long	CMNCR,  0x00001010
	.long	CS0BCR, 0x10000600
	.long	CS2BCR, 0x10000600
	.long	CS4BCR, 0x10000400
	.long	CS5BCR, 0x10000400
	.long	CS3BCR, 0x10004600
	.long	CS0WCR, 0x00000380;	/* wait:4cycle				*/
	.long	CS2WCR, 0x00000380;
	.long	CS4WCR, 0x00000380;
	.long	CS5WCR, 0x00000380;
	.long	CS3WCR, 0x00004892;	/* pre->actv:1cycle, actv->read/write:2cycle, write->pre:1cycle, ref->actv:4cycle	*/
	.long	SDCR,   0x00000811;
	.long	RTCNT,  0xa55a0083;
	.long	RTCOR,  0xa55a0083;
	.long	RTCSR,  0xa55a0008;	/* reflesh count=B¦Õ/4	*/
	.long	0

