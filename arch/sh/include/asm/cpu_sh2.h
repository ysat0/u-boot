/*
 * Copyright (C) 2007,2008 Nobuhiro Iwamatsu <iwamatsu@nigauri.org>
 * Copyright (C) 2008 Renesas Solutions Corp.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _ASM_CPU_SH2_H_
#define _ASM_CPU_SH2_H_

#if defined(CONFIG_SH2A)
#  include <asm/cpu_sh2a.h>
#endif
#define CACHE_OC_NUM_WAYS	4
#define CACHE_UPDATED		2

#if defined(CONFIG_SH2A)
#  include <asm/cpu_sh2a.h>
#endif
#define STBCR3		0xF80A0000
#if !defined(scif_enable)
#define scif_enable(ch)					 \
do { 							 \
	if (ch < 3) { 					 \
		unsigned char mask = 1 << (ch); 			 \
		writeb((readb(STBCR3) & ~mask), STBCR3); \
	} 						 \
} while (0)
#endif
#if !defined( cmt_clock_enable)
#define cmt_clock_enable() \
do { 							 \
		writeb((readb(STBCR3) & ~0x10), STBCR3); \
} while (0)
#endif
#if defined(CONFIG_CPU_SH7203)
# include <asm/cpu_sh7203.h>
#elif defined(CONFIG_CPU_SH7206)
# include <asm/cpu_sh7206.h>
#elif defined(CONFIG_CPU_SH7264)
# include <asm/cpu_sh7264.h>
#elif defined(CONFIG_CPU_SH7269)
# include <asm/cpu_sh7269.h>
#elif defined(CONFIG_CPU_SH7206)
# include <asm/cpu_sh7206.h>
#elif defined(CONFIG_CPU_SH7619)
# include <asm/cpu_sh7619.h>
#else
# error "Unknown SH2 variant"
#endif

#endif	/* _ASM_CPU_SH2_H_ */
