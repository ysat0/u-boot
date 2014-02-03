/*
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2014
 * Yoshinori Sato <ysato@Users.sourceforge.jp>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef	__ASM_H8300_GLOBALDATA_H_
#define __ASM_H8300_GLOBALDATA_H_

struct arch_global_data
{
};

#include <asm-generic/global_data.h>

#define DECLARE_GLOBAL_DATA_PTR extern gd_t *gd;

#endif /* __ASM_H8300_GLOBALDATA_H_ */
