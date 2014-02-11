/*
 * Copyright (C) 2007,2008 Nobuhiro Iwamatsu <iwamatsu@nigauri.org>
 * Copyright (C) 2008 Renesas Solutions Corp.
 * Copyright (C) 2013 Yoshinori Sato <ysato@users.sourceforge.jp>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <asm/processor.h>
#include <asm/io.h>

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
int checkcpu(void)
{
	puts("CPU: SH2\n");
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

#if defined(CONFIG_CPU_SH7619)
static void phy_init(void)
	int i;
	unsigned d;
	while (__raw_readw(0xf8490010) & 0x8000);
	__raw_writew(CONFIG_SH_ETHER_PHY_ADDR, 0xf849000c);
	__raw_writew(0xa007, 0xf8490000);
	udelay(1000);
	__raw_writew(0xe007, 0xf8490000);
	udelay(10000);
#endif
int cpu_eth_init(bd_t *bis)
#ifdef CONFIG_CPU_SH7619
	phy_init();
#endif
#ifdef CONFIG_SH_ETHER
	sh_eth_initialize(bis);
#endif