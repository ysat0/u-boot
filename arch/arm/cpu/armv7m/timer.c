/*
 * Copyright (C) 2012 Yoshinori Sato
 * 
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
#include <asm/io.h>
#include <asm/processor.h>

DECLARE_GLOBAL_DATA_PTR;

#define SYSTICK_CTRL	0xe000e010
#define SYSTICK_RELOAD	0xe000e014
#define SYSTICK_CURRENT	0xe000e018
#define SYSTICK_REF	0xe000e01c

#define TICKS_TO_HZ(tick) ((tick) / 18000000) 
#define READ_TIMER() readl(SYSTICK_CURRENT)

/* Configure a free-running, auto-wrap counter with /16 prescaler */
int timer_init(void)
{
	writel(0xffffff, SYSTICK_RELOAD);
	writel(0x05, SYSTICK_CTRL);
	return 0;
}

ulong get_timer_masked(void)
{
	/* current tick value */
	ulong now = TICKS_TO_HZ(READ_TIMER());
	ushort status = readl(SYSTICK_CTRL);

	if (!(status & 0x8000))	/* normal (non rollover) */
		gd->tbl += (now - gd->lastinc);
	else			/* rollover */
		gd->tbl += (TICKS_TO_HZ(0xffffff) - gd->lastinc) + now;
	gd->lastinc = now;
	return gd->tbl;
}

/* Delay x useconds */
void __udelay(ulong usec)
{
	long tmo = usec * (CONFIG_SYS_HZ / 1000) / 1000;
	ulong now, last = READ_TIMER();
	ushort status;

	while (tmo > 0) {
		now = READ_TIMER();
		status = readl(SYSTICK_CTRL);
		if (!(status & 0x8000))	/* normal (non rollover) */
			tmo -= last - now;
		else		/* rollover */
			tmo -= last + 0xffffff - now;
		last = now;
	}
}

ulong get_timer(ulong base)
{
	return get_timer_masked() - base;
}

/*
 * Emulation of Power architecture long long timebase.
 *
 * TODO: Support gd->tbu for real long long timebase.
 */
unsigned long long get_ticks(void)
{
	return get_timer(0);
}

/*
 * Emulation of Power architecture timebase.
 * NB: Low resolution compared to Power tbclk.
 */
ulong get_tbclk(void)
{
	return 100 * readl(1800000);
}

