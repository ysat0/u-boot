/*
 * Configuation settings for the Renesas EDOSK2674R
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

#ifndef __EDOSK2674_H
#define __EDOSK2674_H

#undef DEBUG
#define CONFIG_H8300		1
#define CONFIG_H8300S		1
#define CONFIG_CPU_H8S2678	1
#define CONFIG_EDOSK2674	1

#define CONFIG_SYS_NO_FLASH	1

#include <config_cmd_default.h>
#undef CONFIG_CMD_BDI
#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_IMI
#undef CONFIG_CMD_IMLS
#undef CONFIG_CMD_NFS
#undef CONFIG_CMD_SDRAM
#undef CONFIG_CMD_CACHE
#undef CONFIG_CMD_NET

#define CONFIG_BAUDRATE		38400
#define CONFIG_BOOTARGS		"console=ttySC0,115200"

#define CONFIG_VERSION_VARIABLE
#undef	CONFIG_SHOW_BOOT_PROGRESS

/* MEMORY */
#define EDOSK2674_SDRAM_BASE	0x400000

#define CONFIG_SYS_LONGHELP		/* undef to save memory	*/
#define CONFIG_SYS_PROMPT	"=> "	/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE	256	/* Buffer size for input from the Console */
#define CONFIG_SYS_PBSIZE	256	/* Buffer size for Console output */
#define CONFIG_SYS_MAXARGS	16	/* max args accepted for monitor commands */
/* Buffer size for Boot Arguments passed to kernel */
#define CONFIG_SYS_BARGSIZE	512
/* List of legal baudrate settings for this board */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 38400 }

/* SCI */
#define CONFIG_SCI_SERIAL	1
#define CONFIG_SCI_BASE		0xffff88	/* SCI2 */	

/* TIMER */
#define CONFIG_TIMER_BASE	0xffffb0	/* CH0-1 */

#define CONFIG_SYS_MEMTEST_START	EDOSK2674_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + (2 * 1024 * 1024))

#define CONFIG_SYS_DRAM_BASE	        EDOSK2674_SDRAM_BASE
#define CONFIG_SYS_DRAM_SIZE		(8 * 1024 * 1024)
#define CONFIG_SYS_SDRAM_BASE	        EDOSK2674_SDRAM_BASE
#define CONFIG_SYS_SDRAM_SIZE		(8 * 1024 * 1024)

#define CONFIG_SYS_LOAD_ADDR		0x500000

#define CONFIG_SYS_MALLOC_LEN		(256 * 1024)
#define CONFIG_SYS_GBL_DATA_SIZE	256

#define CONFIG_ENV_SIZE			1024
#define CONFIG_ENV_IS_NOWHERE

/* Board Clock */
#define CONFIG_SYS_HZ			33333333

#endif	/* __EDOSK2674_H */
