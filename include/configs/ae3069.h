/*
 * Configuation settings for the Akizukidenshi AE-3069
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

#ifndef __AE3069_H
#define __AE3069_H

#undef DEBUG
#define CONFIG_H8300		1
#define CONFIG_H8300H		1
#define CONFIG_CPU_H83069	1
#define CONFIG_AE3069	1

#undef CONFIG_CMD_FLASH
#undef CONFIG_CMD_NET
#undef CONFIG_CMD_NFS
#undef CONFIG_CMD_PING
#undef CONFIG_CMD_SAVEENV
#undef CONFIG_CMD_SDRAM
#define CONFIG_CMD_MEMORY
#undef CONFIG_CMD_CACHE

#define CONFIG_BAUDRATE		38400
#define CONFIG_BOOTARGS		"console=ttySC0,115200"
#define CONFIG_LOADADDR		0x0C100000 /* RSK7203_SDRAM_BASE + 1MB */

#define CONFIG_VERSION_VARIABLE
#undef	CONFIG_SHOW_BOOT_PROGRESS

/* MEMORY */
#define AE3069_DRAM_BASE	0x400000

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

#define CONFIG_SYS_MEMTEST_START	AE3069_DRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + (2 * 1024 * 1024))

#define CONFIG_SYS_DRAM_BASE	        AE3069_DRAM_BASE
#define CONFIG_SYS_DRAM_SIZE		(2 * 1024 * 1024)

#define CONFIG_SYS_LOAD_ADDR		0x500000

#define CONFIG_SYS_MALLOC_LEN		(256 * 1024)
#define CONFIG_SYS_GBL_DATA_SIZE	256

#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_SIZE			1024
#define CONFIG_ENV_IS_NOWHERE

/* Board Clock */
#define CONFIG_SYS_HZ			20000000

#endif	/* __AE3069_H */
