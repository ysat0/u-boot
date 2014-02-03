#
# (C) Copyright 2000-2007
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#
<<<<<<< HEAD
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LIB	:= $(obj)libmtd.o

COBJS-$(CONFIG_MTD_DEVICE) += mtdcore.o
COBJS-$(CONFIG_MTD_PARTITIONS) += mtdpart.o
COBJS-$(CONFIG_MTD_CONCAT) += mtdconcat.o
COBJS-$(CONFIG_FLASH_CFI_DRIVER) += cfi_flash.o
COBJS-$(CONFIG_FLASH_CFI_MTD) += cfi_mtd.o
ifneq ($(CONFIG_HAS_DATAFLASH),)
COBJS-$(CONFIG_AT91) += at45.o dataflash.o
COBJS-$(CONFIG_H8300H) += h8dataflash.o
SOBJS-$(CONFIG_H8300H) += 3069flash_low.o
endif
COBJS-$(CONFIG_FLASH_CFI_LEGACY) += jedec_flash.o
COBJS-$(CONFIG_MW_EEPROM) += mw_eeprom.o
COBJS-$(CONFIG_ST_SMI) += st_smi.o

COBJS	:= $(COBJS-y)
SOBJS	:= $(SOBJS-y)
SRCS	:= $(COBJS:.o=.c) $(SOBJS:.o=.S)
OBJS	:= $(addprefix $(obj),$(COBJS) $(SOBJS))

all:	$(LIB)

$(LIB):	$(obj).depend $(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend
=======
>>>>>>> origin

ifneq (,$(findstring y,$(CONFIG_MTD_DEVICE)$(CONFIG_CMD_NAND)$(CONFIG_CMD_ONENAND)))
obj-y += mtdcore.o
endif
obj-$(CONFIG_MTD_PARTITIONS) += mtdpart.o
obj-$(CONFIG_MTD_CONCAT) += mtdconcat.o
ifneq ($(CONFIG_HAS_DATAFLASH),)
obj-$(CONFIG_AT91) += at45.o dataflash.o
obj-$(CONFIG_CPU_H83069) += h8dataflash.o 3069flash_low.o
endif
obj-$(CONFIG_FLASH_CFI_DRIVER) += cfi_flash.o
obj-$(CONFIG_FLASH_CFI_MTD) += cfi_mtd.o
obj-$(CONFIG_HAS_DATAFLASH) += dataflash.o
obj-$(CONFIG_FTSMC020) += ftsmc020.o
obj-$(CONFIG_FLASH_CFI_LEGACY) += jedec_flash.o
obj-$(CONFIG_MW_EEPROM) += mw_eeprom.o
obj-$(CONFIG_ST_SMI) += st_smi.o
