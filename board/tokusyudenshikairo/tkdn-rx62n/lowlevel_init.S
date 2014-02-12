/*
 * (C) Copyright 2011 Yoshinori Sato
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

	.global	lowlevel_init

	.text
	.align	2

lowlevel_init:
#if !defined(CONFIG_TKDN_RX620_RAM)
	mov.l	#3f,r1
	mov.l	#0x00080000,r2
1:	
	mov.l	[r1+],r3
	tst	r3,r3
	bn	2f
	mov.l	[r1+],r4
	shlr	#16,r3,r5
	and	#0xffff,r3
	bra	r5
	mov.b	r4,[r3,r2]
	bra	1b
	mov.w	r4,[r3,r2]
	bra	1b
	mov.l	r4,[r3,r2]
	bra	1b
2:
#endif
	/* dataflash read enable */
	mov.l	#0x7fc440,r1
	mov.w	#0x2dff,[r1]
	mov.w	#0xd2ff,2[r1]
	rts

3:
#define BYTE 0
#define	WORD 1
#define LONG 2
#define REGINIT(offset, size, val) .long ((2 + size * 5) << 16)|(offset >> size),val
	/* ICLK:96MHz, BCLK:48MHz, PCLK:48MHz */
	/* SDCLK and BCLK enable */
	REGINIT(0x0020,LONG,0x00010100)
	/* External Bus enable */
	REGINIT(0x0006,WORD,0x5a03)
	REGINIT(0xC104,BYTE,0xff)
	REGINIT(0xC105,BYTE,0x30)
	/* SDCLK enable */
	REGINIT(0xC106,BYTE,0xd0)
	.long	0xffffffff
	
	.align 2
