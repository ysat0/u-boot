/*
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

/*
 * Jump to cache disabled area
 * When handling caches, we need to do it from non-cache area.
 */
#define jump_to_uncacheable()			\
do {					\
	unsigned long __dummy;		\
	__asm__ __volatile__(		\
		"mov.l  1f, %0\n\t"	\
		"or     %1, %0\n\t"	\
		"jmp    @%0\n\t"	\
		" nop\n\t"		\
		".balign 4\n"		\
		"1:     .long 2f\n"	\
		"2:"			\
		: "=&r" (__dummy)	\
		: "r" (0x20000000));	\
} while (0)

/*
 * Back to cache area.
 */
#define back_to_cacheable()			\
do {					\
	unsigned long __dummy;		\
	__asm__ __volatile__(		\
		"nop;nop;nop;nop;nop;nop;nop\n\t"	\
		"mov.l  1f, %0\n\t"	\
		"jmp    @%0\n\t"	\
		" nop\n\t"		\
		".balign 4\n"		\
		"1:     .long 2f\n"	\
		"2:"			\
		: "=&r" (__dummy));	\
} while (0)

static inline void cache_wback_all(void)
{
	unsigned long addr, data, i, j;

	for (i = 0; i < CACHE_OC_NUM_ENTRIES; i++){
		for (j = 0; j < CACHE_OC_NUM_WAYS; j++) {
			addr = CACHE_OC_ADDRESS_ARRAY | (j << CACHE_OC_WAY_SHIFT)
				| (i << CACHE_OC_ENTRY_SHIFT);
			data = inl(addr);
			if (data & CACHE_UPDATED) {
				data &= ~CACHE_UPDATED;
				outl(data, addr);
			}
		}
	}
}
void flush_cache(unsigned long addr, unsigned long size)
{
	unsigned long entry;
	unsigned long tag;
	jump_to_uncacheable();
	while(size > 0) {
		entry = addr & 0x000003ff0;
		tag = addr & 0x1ffff0000;
		/* I-Cache flush */
		outl(tag, 0xf0000008 | entry);
		/* D-Cache flush with wb */
		outl(tag, 0xf0800008 | entry);
		addr += 4;
		size -= 4;
	}
	back_to_cacheable();
}

void icache_enable(void)
{
	unsigned long ccr;
	ccr = readl(CCR1);
	ccr |= 0x00000900;
	jump_to_uncacheable();
	writel(ccr, CCR1);
}

void icache_disable(void)
{
	unsigned long ccr;
	ccr = readl(CCR1);
	ccr &= ~0x00000100;
	jump_to_uncacheable();
	writel(ccr, CCR1);
}

int icache_status(void)
{
	unsigned long ccr;
	ccr = readl(CCR1);
	return ((ccr & 0x00000100) != 0);
}

void dcache_enable(void)
{
	unsigned long ccr;
	ccr = readl(CCR1);
	ccr |= 0x00000009;
	jump_to_uncacheable();
	writel(ccr, CCR1);
}

void dcache_disable(void)
{
	unsigned long ccr;
	ccr = readl(CCR1);
	ccr &= ~0x00000001;
	jump_to_uncacheable();
	cache_wback_all();
	writel(ccr, CCR1);
}

int dcache_status(void)
{
	unsigned long ccr;
	ccr = readl(CCR1);
	return ((ccr & 0x00000001) != 0);
}
