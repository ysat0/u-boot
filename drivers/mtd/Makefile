#
# (C) Copyright 2000-2007
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# SPDX-License-Identifier:	GPL-2.0+
#

ifneq (,$(findstring y,$(CONFIG_MTD_DEVICE)$(CONFIG_CMD_NAND)$(CONFIG_CMD_ONENAND)))
obj-y += mtdcore.o
endif
obj-$(CONFIG_MTD_PARTITIONS) += mtdpart.o
obj-$(CONFIG_MTD_CONCAT) += mtdconcat.o
ifeq ($(CONFIG_HAS_DATAFLASH),y)
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
