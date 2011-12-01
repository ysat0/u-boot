/*
 * Configuation settings for rxsim (RX simulator)
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

#ifndef __TKDN_RX62N_H
#define __TKDN_RX62N_H

#define CONFIG_RX		1
#define CONFIG_RX600		1
#define CONFIG_CPU_RX62X	1
#define CONFIG_CPU_RX62N	1

#include <config_cmd_default.h>
#undef CONFIG_CMD_BDI
#undef CONFIG_CMD_FLASH
#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_IMI
#undef CONFIG_CMD_IMLS
#undef CONFIG_CMD_SETGETDCR
#define CONFIG_CMD_SPI
#define CONFIG_CMD_SF

#define CONFIG_BAUDRATE		38400
#define CONFIG_BOOTARGS		"console=ttySC0,38400"

#define CONFIG_VERSION_VARIABLE
#undef  CONFIG_SHOW_BOOT_PROGRESS

/*#define CONFIG_TKDN_RX620_RAM*/

/* system */
#define CONFIG_SYS_TEXT_BASE	0xfff80000
/*#define CONFIG_SYS_TEXT_BASE	0x08000000*/
#define CONFIG_SYS_LONGHELP		/* undef to save memory	*/
#define CONFIG_SYS_PROMPT	"=> "	/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE	256	/* Buffer size for input from the Console */
#define CONFIG_SYS_PBSIZE	256	/* Buffer size for Console output */
#define CONFIG_SYS_MAXARGS	16	/* max args accepted for monitor commands */
/* List of legal baudrate settings for this board */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600,14400,19200,38400,57600,115200 }

#define CONFIG_ENV_IS_IN_DATAFLASH	1
#define CONFIG_ENV_SIZE 4096
#define CONFIG_ENV_ADDR 0x100000

/* SCI */
#define CONFIG_SCI		1
#define CONFIG_SCIF_CONSOLE	1
#define CONFIG_CONS_SCIF0	1

/* memory */
#define CONFIG_SYS_SRAM_BASE		0x00008000
#define CONFIG_SYS_SRAM_SIZE		0x00010000
#define CONFIG_SYS_SDRAM_BASE		0x08000000
#define CONFIG_SYS_SDRAM_SIZE		(16 * 1024 * 1024)
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		CONFIG_SYS_SDRAM_SIZE

#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_MEMTEST_START)
#define CONFIG_SYS_MONITOR_BASE		(CONFIG_SYS_TEXT_BASE)
#define CONFIG_SYS_MONITOR_LEN		(256 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(64 * 1024)
#define CONFIG_SYS_GBL_DATA_SIZE	256
#define CONFIG_SYS_BOOTMAPSZ		(8 * 1024 * 1024)
#define CONFIG_HAS_DATAFLASH		1
#define CONFIG_SYS_NO_FLASH

#define CONFIG_SYS_CLK_FREQ 48000000
#define CONFIG_SYS_HZ	1000

#define CONFIG_SYS_TIMER_BASE (0x00088200)

#define CONFIG_NET_MULTI 1
#define CONFIG_SH_ETHER 1
#define CONFIG_SH_ETHER_USE_PORT (1)
#define CONFIG_SH_ETHER_PHY_ADDR (0x00)
#define CONFIG_RX62N_RMII 1

#define CONFIG_RENESAS_SPI
#define CONFIG_RSPI_CLK (CONFIG_SYS_CLK_FREQ)
#define CONFIG_RSPI_BASE (0x00088380)
#define CONFIG_RSPI_CHANNEL (2)
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_SST

#endif	/* __TKDN_RX62N */
