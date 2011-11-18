/*
 * (C) Copyright 2003
 * Data Flash Atmel Description File
 * Author : Hamid Ikdoumi (Atmel)
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

/* File Name		: dataflash.h					*/
/* Object		: Data Flash Atmel Description File		*/
/* Translator		:						*/
/*									*/
/* 1.0 03/04/01 HI	: Creation					*/
/* 1.2 20/10/02 FB	: Adapatation Service and Lib v3		*/
/*----------------------------------------------------------------------*/

#ifndef _DataFlash_h
#define _DataFlash_h


#include "config.h"

/*number of protected area*/
#define NB_DATAFLASH_AREA		5

#ifdef CONFIG_SYS_NO_FLASH

/*-----------------------------------------------------------------------
 * return codes from flash_write():
 */
# define ERR_OK				0
# define ERR_TIMOUT			1
# define ERR_NOT_ERASED			2
# define ERR_PROTECTED			4
# define ERR_INVAL			8
# define ERR_ALIGN			16
# define ERR_UNKNOWN_FLASH_VENDOR	32
# define ERR_UNKNOWN_FLASH_TYPE		64
# define ERR_PROG_ERROR			128

/*-----------------------------------------------------------------------
 * Protection Flags for flash_protect():
 */
# define FLAG_PROTECT_SET		0x01
# define FLAG_PROTECT_CLEAR		0x02
# define FLAG_PROTECT_INVALID		0x03

/* DataFlash return value */
#define DATAFLASH_BUSY			0x00
#define DATAFLASH_OK			0x01
#define DATAFLASH_ERROR			0x02
#define DATAFLASH_MEMORY_OVERFLOW	0x03
#define DATAFLASH_BAD_COMMAND		0x04
#define DATAFLASH_BAD_ADDRESS		0x05

/*-----------------------------------------------------------------------
 * Set Environment according to label:
 */
# define	FLAG_SETENV		0x80
#endif /* CONFIG_SYS_NO_FLASH */

#include <asm/arch/dataflash.h>

extern int addr2ram(ulong addr);
extern int dataflash_real_protect (int flag, unsigned long start_addr, unsigned long end_addr);
extern int addr_dataflash (unsigned long addr);
extern int read_dataflash (unsigned long addr, unsigned long size, char *result);
extern int write_dataflash (unsigned long addr, unsigned long dest, unsigned long size);
extern void dataflash_print_info (void);
extern void dataflash_perror (int err);

#endif
