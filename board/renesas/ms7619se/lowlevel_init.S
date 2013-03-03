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
	// Set up the Pin Function Controler
	mov.l	$PBDRL,r0
	mov.w	@r0,r1
	mov	#0x40,r2
	shll	r2
	or	r2,r1
	mov.w	r1,@r0			// PB7 <- "H"
	mova	PFC_settings_table,r0
3:	mov.l	@r0+,r3		       // Address (or zero)
	tst	r3,r3
	bt	4f
	mov.l	@r0+,r1		       // data
	bra	3b
	 mov.w	r1,@r3
4:
	// Set up the Bus Controler
	mova	BSC_settings_table,r0
5:	mov.l	@r0+,r3		       // Address (or zero)
	tst	r3,r3
	bt	6f
	mov.l	@r0+,r1		       // data
	bra	5b
	 mov.l	r1,@r3

6:
	mov.l	$delay_count,r0
7:
	dt	r0
	bf	7b

	mov.l	$SDMR,r1		// Turns on SDRAM controller
	mov.l	$SDMR_val,r2
	rts
	 mov.l	r2,@r1


	.align	4
$delay_count:
	.long	2000
$SDMR:	.long	0xf8fd58c0
$SDMR_val:
	.long	0
$PBDRL:
	.long	PBDRL
CPG_settings_table:
	.long	WTCSR,0xA500
	.long	WTCNT,0x5A00
	.long	WTCSR,0xA51E
	.long	FRQCR,0x1103	/* input:31.25MHz I:B:P=4:2:1=125MHz:62.5MHz:31.25MHz */
	.long	0

BSC_settings_table:
	.long	CMNCR,  0x00000010
	.long	CS0BCR, 0x24920400
	.long	CS3BCR, 0x12494600
	.long	CS0WCR, 0x00000300
	.long	CS3WCR, 0x0000190a
	.long	SDCR,   0x00000811
	.long	RTCNT,  0xa55a0000
	.long	RTCOR,  0xa55a007a
	.long	RTCSR,  0xa55a0008
	.long	0

PFC_settings_table:
	.long	PAIORH, 0x0055
	.long	PACRH2, 0x5555
	.long	PBCRL1, 0x0551
	.long	PBCRL2, 0x1551
	.long	PCCRH2, 0x0000
	.long	PCCRL1, 0x0000
	.long	PCCRL2, 0xff00
        .long	PCIORH, 0x001f
        .long	PCIORL, 0xfffe
	.long	PDCRL2, 0xa900
	.long	PECRH1, 0x0003
	.long	PECRH2, 0xffff
	.long	PECRL1, 0xfffc
	.long	PECRL2, 0x0000
	.long	0
