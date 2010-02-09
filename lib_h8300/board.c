/*
 * Copyright 2010 Yoshinori Sato <ysato@users.sourceforge.jp>
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

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <stdio_dev.h>
#include <timestamp.h>
#include <version.h>
#include <watchdog.h>
#include <net.h>
#include <environment.h>

extern int cpu_init(void);
extern int board_init(void);
extern int dram_init(void);
extern int timer_init(void);

const char version_string[] = U_BOOT_VERSION" ("U_BOOT_DATE" - "U_BOOT_TIME")";

#if defined(CONFIG_WATCHDOG)
extern int watchdog_init(void);
extern int watchdog_disable(void);
# define INIT_FUNC_WATCHDOG_INIT	watchdog_init,
# define WATCHDOG_DISABLE       	watchdog_disable
#else
# define INIT_FUNC_WATCHDOG_INIT
# define WATCHDOG_DISABLE
#endif /* CONFIG_WATCHDOG */

#if defined(CONFIG_CMD_IDE)
# include <ide.h>
# define INIT_FUNC_IDE_INIT	ide_init,
#else
# define INIT_FUNC_IDE_INIT
#endif /* CONFIG_CMD_IDE */

#if defined(CONFIG_CMD_NET)
static int sh_net_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;
	gd->bd->bi_ip_addr = getenv_IPaddr("ipaddr");
	return 0;
}
#endif

typedef int (init_fnc_t) (void);

static init_fnc_t *init_sequence[] =
{
	cpu_init,		/* basic cpu dependent setup */
	board_init,		/* basic board dependent setup */
	interrupt_init,	/* set up exceptions */
	env_init,		/* event init */
	serial_init,	/* SCIF init */
	INIT_FUNC_WATCHDOG_INIT	/* watchdog init */
	console_init_f,
	display_options,
	checkcpu,
	checkboard,		/* Check support board */
	dram_init,
	timer_init,
	stdio_init,
	console_init_r,
	interrupt_init,
	NULL			/* Terminate this list */
};

gd_t *gd;

void h8300_generic_init(gd_t *_gd)
{
	DECLARE_GLOBAL_DATA_PTR;

	bd_t *bd;
	init_fnc_t **init_fnc_ptr;
	gd = _gd;

	memset(gd, 0, CONFIG_SYS_GBL_DATA_SIZE);

	gd->flags |= GD_FLG_RELOC;	/* tell others: relocation done */

	gd->bd = (bd_t *)(gd + 1);	/* At end of global data */
	gd->baudrate = CONFIG_BAUDRATE;

	gd->cpu_clk = CONFIG_SYS_HZ;

	bd = gd->bd;
	bd->bi_memstart	= CONFIG_SYS_DRAM_BASE;
	bd->bi_memsize = CONFIG_SYS_DRAM_SIZE;
#if defined(CONFIG_SYS_SRAM_BASE) && defined(CONFIG_SYS_SRAM_SIZE)
	bd->bi_sramstart = CONFIG_SYS_SRAM_BASE;
	bd->bi_sramsize	= CONFIG_SYS_SRAM_SIZE;
#endif
	bd->bi_baudrate	= CONFIG_BAUDRATE;

	mem_malloc_init(CONFIG_SYS_LOAD_ADDR - CONFIG_SYS_GBL_DATA_SIZE -
			CONFIG_SYS_MALLOC_LEN, CONFIG_SYS_MALLOC_LEN - 16);
	env_relocate();

	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) {
		WATCHDOG_RESET();
		if ((*init_fnc_ptr) () != 0)
			hang();
	}

#ifdef CONFIG_WATCHDOG
	/* disable watchdog if environment is set */
	{
		char *s = getenv("watchdog");
		if (s != NULL)
			if (strncmp(s, "off", 3) == 0)
				WATCHDOG_DISABLE();
	}
#endif /* CONFIG_WATCHDOG*/

#if defined(CONFIG_CMD_NET)
	{
		char *s;
		puts("Net:   ");
		eth_initialize(gd->bd);

		s = getenv("bootfile");
		if (s != NULL)
			copy_filename(BootFile, s, sizeof(BootFile));
	}
#endif /* CONFIG_CMD_NET */

	while (1) {
		WATCHDOG_RESET();
		main_loop();
	}
}

/***********************************************************************/

void hang(void)
{
	puts("Board ERROR\n");
	for (;;)
		;
}
