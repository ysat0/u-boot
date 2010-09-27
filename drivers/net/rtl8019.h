/*
 * Realtek 8019AS Ethernet
 * (C) Copyright 2002-2003
 * Xue Ligong(lgxue@hotmail.com),Wang Kehao, ESLAB, whut.edu.cn
 *
 * Copyright (C) 2010 Yoshinori Sato <ysato@users.sourceforge.jp>
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

/*
 * This code works in 8bit mode.
 * If you need to work in 16bit mode, PLS change it!
 */

#include <asm/types.h>
#include <config.h>

#ifdef CONFIG_DRIVER_RTL8019

#define DRIVERNAME "rtl8019"

#define TXPAGE 0x40
#define RXPAGE 0x48
#define ENDPAGE 0x80

/* RTL8019 registers */
/* page 0 */
#define CR       0x00
#define CLDA0    0x01
#define PSTART   0x01
#define CLDA1    0x02
#define PSTOP    0x02
#define BNDRY    0x03
#define TSR      0x04
#define TPSR     0x04
#define NCR      0x05
#define TBCL     0x05
#define FIFO     0x06
#define TBCH     0x06
#define ISR      0x07
#define CRDA0    0x08
#define RSAL     0x08
#define CRDA1    0x09
#define RSAH     0x09
#define RBCL     0x0a
#define RBCH     0x0b
#define RSR      0x0c
#define RCR      0x0c
#define FER      0x0d
#define TCR      0x0d
#define CER      0x0e
#define DCR      0x0e
#define MISSED   0x0f
#define IMR      0x0f
#define DATA     0x10
#define RESET    0x1f

/* page 1 */
#define PAR0     0x01
#define PAR1     0x02
#define PAR2     0x03
#define PAR3     0x04
#define PAR4     0x05
#define PAR5     0x06
#define CURP     0x07
#define MAR0     0x08
#define MAR1     0x09
#define MAR2     0x0a
#define MAR3     0x0b
#define MAR4     0x0c
#define MAR5     0x0d
#define MAR6     0x0e
#define MAR7     0x0f

/* page 2*/
#define CR       0x00
#define PSTART   0x01
#define CLDA0    0x01
#define PSTOP    0x02
#define CLDA1    0x02
#define RNPP     0x03
#define TPSR     0x04
#define LNPP     0x05
#define ACH      0x06
#define ACL      0x07

/* interrupt status */
#define ISR_RXP    0x01
#define ISR_TXP    0x02
#define ISR_RXE    0x04
#define ISR_TXE    0x08
#define ISR_OFLW   0x10
#define ISR_CNT    0x20
#define ISR_RDC    0x40
#define ISR_RESET  0x80

/* command */
#define CR_STOP    0x01
#define CR_START   0x02
#define CR_TXPKT   0x04
#define CR_RDMA    0x08
#define CR_WDMA    0x10
#define CR_SEND    0x18
#define CR_NODMA   0x20
#define CR_PAGE0   0x00
#define CR_PAGE1   0x40
#define CR_PAGE2   0x80

// data configuration */
#define DCR_WTS    0x01
#define DCR_BOS    0x02
#define DCR_LAS    0x04
#define DCR_LS     0x08
#define DCR_ARM    0x10
#define DCR_FIFO_1 0x00
#define DCR_FIFO_2 0x20
#define DCR_FIFO_4 0x40
#define DCR_FIFO_6 0x60

/* receive control */
#define RCR_SEP    0x01
#define RCR_AR     0x02
#define RCR_AB     0x04
#define RCR_AM     0x08
#define RCR_PROM   0x10
#define RCR_MON    0x20

/* receive status */
#define RSR_RxP    0x01
#define RSR_CRC    0x02
#define RSR_FRAME  0x04
#define RSR_FO     0x08
#define RSR_MISS   0x10
#define RSR_PHY    0x20
#define RSR_DIS    0x40
#define RSR_DFR    0x80

/* transmit control */
#define TCR_NOCRC   0x01
#define TCR_NORMAL  0x00
#define TCR_LOCAL   0x02
#define TCR_INLOOP  0x04
#define TCR_OUTLOOP 0x08
#define TCR_ATD     0x10
#define TCR_OFFSET  0x20

/* transmit status */
#define TSR_TXP    0x01
#define TSR_COL    0x04
#define TSR_ABT    0x08
#define TSR_CRS    0x10
#define TSR_FU     0x20
#define TSR_CDH    0x40
#define TSR_OWC    0x80

struct rtl8019_commands {
	u8 offset;
	u8 value;
};

#endif /*end of CONFIG_DRIVER_RTL8019*/
