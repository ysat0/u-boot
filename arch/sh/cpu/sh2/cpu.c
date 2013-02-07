/*
 * Copyright (C) 2007,2008 Nobuhiro Iwamatsu <iwamatsu@nigauri.org>
 * Copyright (C) 2008 Renesas Solutions Corp.
 * Copyright (C) 2013 Yoshinori Sato <ysato@users.sourceforge.jp>
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

#include <common.h>
#include <command.h>
#include <asm/processor.h>
#include <asm/io.h>

int checkcpu(void)
{
#if defined(CONFIG_SH2A)
	puts("CPU: SH2A\n");
#else
	puts("CPU: SH2\n");
#endif
	return 0;
}

#if defined(CONFIG_CONS_SCIF0)
# define CH 0
#elif defined(CONFIG_CONS_SCIF1)
# define CH 1
#elif defined(CONFIG_CONS_SCIF2)
# define CH 2
#elif defined(CONFIG_CONS_SCIF3)
# define CH 3
#else
# error "Default SCIF doesn't set....."
#endif

int cpu_init(void)
{
	/* SCIF enable */
	scif_enable(CH);
	/* CMT clock enable */
	cmt_clock_enable() ;
	return 0;
}

int cleanup_before_linux(void)
{
	disable_interrupts();
	return 0;
}

int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	disable_interrupts();
	reset_cpu(0);
	return 0;
}

