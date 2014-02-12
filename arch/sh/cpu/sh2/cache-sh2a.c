/*
 * Copyright (C) 2013 Yoshinori Sato <ysato@users.sourceforge.jp>
 *
 * SPDX-License-Identifier:	GPL-2.0+
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

#define CACHE_OC_NUM_ENTRIES 128
#define CACHE_OC_NUM_WAYS 4
#define CACHE_OC_ADDRESS_ARRAY 0xf0800000
#define CACHE_OC_WAY_SHIFT 11
#define CACHE_OC_ENTRY_SHIFT 2
#define CACHE_UPDATED 0x02

static inline void cache_wback_all(void)
{
	unsigned long addr, data, i, j;

	for (i = 0; i < CACHE_OC_NUM_ENTRIES; i++) {
		for (j = 0; j < CACHE_OC_NUM_WAYS; j++) {
			addr = CACHE_OC_ADDRESS_ARRAY
				| (j << CACHE_OC_WAY_SHIFT)
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
	size = (size + 3) & ~3;
	jump_to_uncacheable();
	while (size > 0) {
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
	back_to_cacheable();
}

void icache_disable(void)
{
	unsigned long ccr;
	ccr = readl(CCR1);
	ccr &= ~0x00000100;
	jump_to_uncacheable();
	writel(ccr, CCR1);
	back_to_cacheable();
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
	back_to_cacheable();
}

void dcache_disable(void)
{
	unsigned long ccr;
	ccr = readl(CCR1);
	ccr &= ~0x00000001;
	jump_to_uncacheable();
	cache_wback_all();
	writel(ccr, CCR1);
	back_to_cacheable();
}

int dcache_status(void)
{
	unsigned long ccr;
	ccr = readl(CCR1);
	return ((ccr & 0x00000001) != 0);
}
