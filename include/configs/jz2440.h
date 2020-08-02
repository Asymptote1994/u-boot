/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2020 Asymptote
 *
 * Configuation settings for the SAMSUNG JZ2440 board.
 */

#ifndef __CONFIG_JZ2440_H
#define __CONFIG_JZ2440_H

/* Do not use lowlevel_init function */
#define CONFIG_SKIP_LOWLEVEL_INIT_ONLY

#define CONFIG_SYS_LOAD_ADDR		0x30800000
#define CONFIG_SYS_MALLOC_LEN		(4 * 1024 * 1024)

/* SDRAM */
#define PHYS_SDRAM_1				0x30000000 /* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE			0x04000000 /* 64 MB */
// #define CONFIG_NR_DRAM_BANKS        1
#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM_1

/* Stack address */
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x1000 - GENERATED_GBL_DATA_SIZE)

/* Timer unit */
#define CONFIG_SYS_HZ				1000

#define CONFIG_BOOTCOMMAND \
	   "mmc dev 0;" \
	   "if mmc rescan; then " \
	   		"fatload mmc 0:1 0x30008000 uImage; " \
	   		"fatload mmc 0:1 0x32000000 jz2440.dtb; " \
			"bootm 0x30008000 - 0x32000000;" \
	   "fi"

/* Enable passing of ATAGs */
// #define CONFIG_CMDLINE_TAG
// #define CONFIG_SETUP_MEMORY_TAGS
// #define CONFIG_INITRD_TAG

#endif /* __CONFIG_JZ2440_H */
