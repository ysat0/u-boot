/*
 * Configuation settings for the csun Kane Bebe
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

#ifndef __KANEBEBE_H
#define __KANEBEBE_H

#undef DEBUG
#define CONFIG_H8300		1
#define CONFIG_H8300H		1
#define CONFIG_CPU_H83069	1
#define CONFIG_KANEBEBE		1

#include <config_cmd_default.h>
#undef CONFIG_CMD_BDI
#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_IMI
#undef CONFIG_CMD_IMLS
#undef CONFIG_CMD_NFS
#undef CONFIG_CMD_SDRAM
#undef CONFIG_CMD_CACHE

#define CONFIG_NET_MULTI

#define CONFIG_BAUDRATE		38400
#define CONFIG_BOOTARGS		"console=ttySC0,115200"

#undef CONFIG_VERSION_VARIABLE
#undef	CONFIG_SHOW_BOOT_PROGRESS

/* MEMORY */
#define KANEBEBE_DRAM_BASE	0x400000

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
#define CONFIG_SCI		1
#define CONFIG_SCIF_CONSOLE	1
#define CONFIG_CONS_SCIF1	1

/* TIMER */
#define CONFIG_TIMER_BASE	0xffff80	/* CH0-1 */

#define CONFIG_WATCHDOG

#define CONFIG_SYS_MEMTEST_START	KANEBEBE_DRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + (4 * 1024 * 1024))

#define CONFIG_SYS_DRAM_BASE	        KANEBEBE_DRAM_BASE
#define CONFIG_SYS_DRAM_SIZE		(4 * 1024 * 1024)

#define CONFIG_SYS_SRAM_BASE		0xffc720
#define CONGIG_SYS_SRAM_SIZE		(14 * 1024)

#define CONFIG_SYS_LOAD_ADDR		0x400000

#define CONFIG_SYS_MALLOC_LEN		8192
#define CONFIG_SYS_GBL_DATA_SIZE	64

#define CONFIG_ENV_SIZE			1024
#define CONFIG_ENV_IS_IN_FLASH		1
#define CONFIG_ENV_ADDR			0x001000

#define CONFIG_SYS_RX_ETH_BUFFER	1

#define CONFIG_SYS_MAX_FLASH_BANKS	1
#define CONFIG_SYS_MAX_FLASH_SECT	7
#define CONFIG_SYS_FLASH_BASE		0x001000

/* Board Clock */
#define CONFIG_SYS_HZ			25000000
#define CONFIG_SYS_CLK_FREQ		CONFIG_SYS_HZ

#define CONFIG_DRIVER_RTL8019		1
#define CONFIG_DRIVER_RTL8019_BASE	0x200000

#endif
