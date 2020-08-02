// SPDX-License-Identifier: GPL-2.0+
/*
 *  Copyright (C) 2008-2009 Samsung Electronics
 *  Minkyu Kang <mk7.kang@samsung.com>
 *  Kyungmin Park <kyungmin.park@samsung.com>
 */

#include <common.h>
#include <init.h>
#include <asm/io.h>
#include <asm/mach-types.h>

DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
	// gd->bd->bi_arch_number = MACH_TYPE_JZ2440;
	// gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

	return 0;
}

int print_cpuinfo(void)
{
	printf("hello, u-boot!\n");
	
	return 0;
}

/*
 * reset the cpu by setting up the watchdog timer and let him time out
 */
void reset_cpu(ulong ignored)
{
#define	WTCON		0x53000000
#define	WTCNT		0x53000008

	/* Disable watchdog */
	writel(0x0000, WTCON);

	/* Initialize watchdog timer count register */
	writel(0x0001, WTCNT);

	/* Enable watchdog timer; assert reset at timer timeout */
	writel(0x0021, WTCON);

	while (1)
		/* loop forever and wait for reset to happen */;

	/*NOTREACHED*/
}

int dram_init(void)
{
	gd->ram_size = get_ram_size((long *)PHYS_SDRAM_1, PHYS_SDRAM_1_SIZE);

	return 0;
}

int dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

	return 0;
}
