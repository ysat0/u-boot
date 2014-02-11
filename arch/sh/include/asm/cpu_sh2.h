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
#if defined(CONFIG_CPU_SH7203)
# include <asm/cpu_sh7203.h>
#elif defined(CONFIG_CPU_SH7206)
# include <asm/cpu_sh7206.h>
#elif defined(CONFIG_CPU_SH7264)
# include <asm/cpu_sh7264.h>
#elif defined(CONFIG_CPU_SH7269)
# include <asm/cpu_sh7269.h>
#else
# error "Unknown SH2 variant"
#endif

#endif	/* _ASM_CPU_SH2_H_ */
