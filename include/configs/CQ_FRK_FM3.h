/*
 * Configuation settings for CQ-FRK-FM3 with LF-CQ1
 *
 * Copyright (C) 2011 Yoshinori Sato
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

#ifndef __CQ_FRK_FM3_H
#define __CQ_FRK_FM3_H

#define CONFIG_ARMV7M		1
#define CONFIG_SYS_THUMB_BUILD

#include <config_cmd_default.h>
#undef CONFIG_CMD_BDI
#undef CONFIG_CMD_FLASH
#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_IMI
#undef CONFIG_CMD_IMLS
#undef CONFIG_CMD_SETGETDCR
#undef CONFIG_CMD_EDITENV
#undef CONFIG_CMD_NET
#undef CONFIG_CMD_NFS
#define CONFIG_BAUDRATE		115200

#define CONFIG_VERSION_VARIABLE
#undef  CONFIG_SHOW_BOOT_PROGRESS

/* system */
#define CONFIG_SYS_TEXT_BASE	0x00000000
#define CONFIG_SYS_LONGHELP		/* undef to save memory	*/
#define CONFIG_SYS_PROMPT	"=> "	/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE	256	/* Buffer size for input from the Console */
#define CONFIG_SYS_PBSIZE	256	/* Buffer size for Console output */
#define CONFIG_SYS_MAXARGS	16	/* max args accepted for monitor commands */
/* List of legal baudrate settings for this board */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600,14400,19200,38400,57600,115200 }
#define CONFIG_SYS_HZ		144000000

/* UART */
#define CONFIG_FM3_SERIAL
#define CONFIG_SYS_FM3_UART_CH	4
#define CONFIG_APB2_CLK_FREQ		(CONFIG_SYS_HZ / 4)
#define CONFIG_BOARD_EARLY_INIT_F

/* memory */
#define CONFIG_SYS_SRAM_BASE		0x1fff0000
#define CONFIG_SYS_SRAM_SIZE		0x00020000
#define CONFIG_SYS_INIT_SP_ADDR		0x2000f000
#define CONFIG_SYS_SDRAM_BASE		0x62000000
#define CONFIG_SYS_SDRAM_SIZE		(4 * 1024 * 1024)
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		CONFIG_SYS_SDRAM_SIZE
#define CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_USE_SRAM

#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_MEMTEST_START)
#define CONFIG_SYS_MONITOR_BASE		(CONFIG_SYS_TEXT_BASE)
#define CONFIG_SYS_MONITOR_LEN		(256 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(64 * 1024)
#define CONFIG_SYS_GBL_DATA_SIZE	256
#define CONFIG_SYS_BOOTMAPSZ		(8 * 1024 * 1024)
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_SIZE			1024
#define CONFIG_ENV_IS_NOWHERE

#define CONFIG_NVIC_TIMER

#endif	/* __RXMEGA_H */
