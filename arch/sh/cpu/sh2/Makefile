#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# Copyright (C) 2007,2008 Nobuhiro Iwamatsu <iwamatsu@nigauri.org>
# Copyright (C) 2008 Renesas Solutions Corp.
# Copyright (C) 2013 Yoshinori Sato <ysato@users.sourceforge.jp>
#
# SPDX-License-Identifier:	GPL-2.0+
#

extra-y	= start.o
obj-y	= cpu.o interrupts.o watchdog.o
obj-$(CONFIG_CPU_SH7619) += cache-sh7619.o
obj-$(CONFIG_SH2A) += cache-sh2a.o
else
obj-y	+= nocache.o
endif
