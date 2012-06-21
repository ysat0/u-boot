/*
 * Copyright (C) 2012 Yoshinori Sato
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
#include <net.h>
#include <netdev.h>
#include <asm/io.h>
#include <asm/processor.h>

DECLARE_GLOBAL_DATA_PTR;

#define GPIO_PFR0	0x40033000
#define GPIO_PFR1	0x40033004
#define GPIO_PFR2	0x40033008
#define GPIO_PFR3	0x4003300C
#define GPIO_PFR4	0x40033010
#define GPIO_PFR5	0x40033014
#define GPIO_PFR7	0x4003301C
#define GPIO_PFR9	0x40033024
#define GPIO_PFRF	0x4003303C
#define GPIO_DDRF	0x4003323C
#define GPIO_PDORF	0x4003343C
#define GPIO_ADE	0x40033500
#define GPIO_EPFR07	0x4003361C
#define GPIO_EPFR08	0x40033620
#define GPIO_EPFR10	0x40033628
#define GPIO_EPFR11	0x4003362C
#define GPIO_PZRF	0x4003373C

#define BUS_MODE0	0x4003F000
#define BUS_MODE1	0x4003F004
#define BUS_MODE2	0x4003F008
#define BUS_MODE3	0x4003F00C
#define BUS_TIM0	0x4003F020
#define BUS_TIM1	0x4003F024
#define BUS_TIM2	0x4003F028
#define BUS_TIM3	0x4003F02C
#define BUS_AREA0	0x4003F040
#define BUS_AREA1	0x4003F044
#define BUS_AREA2	0x4003F048
#define BUS_AREA3	0x4003F04C
#define BUS_DCLKR	0x4003F300

#define RMW0(val, adr) writel(readl(adr) & (val), adr)
#define RMW1(val, adr) writel(readl(adr) | (val), adr)

int checkboard(void)
{
	return 0;
}

int board_early_init_f(void)
{
	/* Use MFS CH4 */
	RMW1(0x000000F0, GPIO_EPFR08);
	RMW1(0x00000060, GPIO_PFR0);
}

int board_init(void)
{
	printf("BOARD: CQ Publishing CQ-FRK-FM3 w/ LF-CQ1\n");
	return 0;
}

int dram_init(void)
{
	static const struct {
		unsigned int adr;
		unsigned int val;
	} pfrset[] = {
		{ .adr = GPIO_PFR1, .val = 0x0000FFF0 },
		{ .adr = GPIO_PFR2, .val = 0x000003F1 },
		{ .adr = GPIO_PFR3, .val = 0x000001C0 },
		{ .adr = GPIO_PFR4, .val = 0x00007C00 },
		{ .adr = GPIO_PFR5, .val = 0x00000C2D },
		{ .adr = GPIO_PFR7, .val = 0x000007FF },
		{ .adr = GPIO_PFR9, .val = 0x0000001F },
		{ .adr = GPIO_PZRF, .val = 0x00000008 },
		{ .adr = GPIO_DDRF, .val = 0x00000008 },
	};
	static const struct {
		unsigned long adr;
		unsigned long val;
	} busset[] = {
		{ .adr = BUS_DCLKR, .val = 0x00000013 },
		{ .adr = BUS_MODE0, .val = 0x00000809 },
		{ .adr = BUS_TIM0,  .val = 0x00020012 },
		{ .adr = BUS_AREA0, .val = 0x000F0000 },
		{ .adr = BUS_MODE1, .val = 0x00000849 },
		{ .adr = BUS_TIM1,  .val = 0x01030013 },
		{ .adr = BUS_AREA1, .val = 0x000F0010 },
		{ .adr = BUS_MODE2, .val = 0x00000809 },
		{ .adr = BUS_TIM2,  .val = 0x05170027 },
		{ .adr = BUS_AREA2, .val = 0x000F0020 },
		{ .adr = BUS_MODE3, .val = 0x00000848 },
		{ .adr = BUS_TIM3,  .val = 0x055FF00F },
		{ .adr = BUS_AREA3, .val = 0x000F0030 },
	};
	int i;
	/* Initialize external bus */
	writel(0x7FFFC3B7, GPIO_EPFR10);
	writel(0x01FFFFFE, GPIO_EPFR11);
	RMW0(0xC0FF000F, GPIO_ADE);
	RMW0(~0x0000008, GPIO_PFRF);
	for (i = 0; i < sizeof(pfrset)/sizeof(pfrset[0]); i++)
		RMW1(pfrset[i].val, pfrset[i].adr);
	for (i = 0; i < sizeof(busset)/sizeof(busset[0]); i++)
		RMW1(busset[i].val, busset[i].adr);
	
	gd->ram_size = CONFIG_SYS_SDRAM_SIZE;
	return 0;
}

void led_set_state(unsigned short value)
{
#if 0
	unsigned int output;
	output = readl(GPIO_PDORF);
	if (value)
		output &= ~0x00000008;
	else
		output |= 0x00000008;
	writel(output, GPIO_PDORF);
#endif
}

