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
obj-$(CONFIG_AT91_DATAFLASH) += at45.o at91dataflash.o
obj-$(CONFIG_FLASH_CFI_DRIVER) += cfi_flash.o
obj-$(CONFIG_FLASH_CFI_MTD) += cfi_mtd.o
obj-$(CONFIG_FTSMC020) += ftsmc020.o
obj-$(CONFIG_FLASH_CFI_LEGACY) += jedec_flash.o
obj-$(CONFIG_MW_EEPROM) += mw_eeprom.o
obj-$(CONFIG_ST_SMI) += st_smi.o
obj-$(CONFIG_CPU_H83069) += h3069dataflash.o h83069dataflash_low.o
obj-$(CONFIG_RX600) += rxdataflash.o
