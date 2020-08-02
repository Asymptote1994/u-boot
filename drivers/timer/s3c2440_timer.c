// SPDX-License-Identifier: GPL-2.0
/*
 * Samsung s3c2440 timer driver
 *
 * Copyright (C) 2020 Asymptote
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <clk.h>
#include <timer.h>
#include <asm/io.h>

struct s3c2440_timer_regs {
	u32 tcfg0;
	u32 tcfg1;
	u32 tcon;
	
	u32 tcntb0;
	u32 tcmpb0;
	u32 tcnto0;
	
	u32 tcntb1;
	u32 tcmpb1;
	u32 tcnto1;
	
	u32 tcntb2;
	u32 tcmpb2;
	u32 tcnto2;

	u32 tcntb3;
	u32 tcmpb3;
	u32 tcnto3;

	u32 tcntb4;
	u32 tcnto4;
};

struct s3c2440_timer_priv {
	struct s3c2440_timer_regs *reg;
	struct clk clk;
};

static int s3c2440_timer_get_count(struct udevice *dev, u64 *count)
{
	struct s3c2440_timer_priv *priv = dev_get_priv(dev);
	struct timer_dev_priv *uc_priv = dev_get_uclass_priv(dev);
	ulong now = readl(&priv->reg->tcnto4) & 0xffff;
	unsigned long timer_rate_hz;
	unsigned int tbu;
	static unsigned int tbl = 0;
	static unsigned long lastinc = 0;

	timer_rate_hz = uc_priv->clock_rate;
	tbu = timer_rate_hz / 100;
	debug("%s(): timer_rate_hz = %ld, tbu = %d\n", __func__, timer_rate_hz, tbu);

	if (lastinc >= now) {
		/* normal mode */
		tbl += lastinc - now;
	} else {
		/* we have an overflow ... */
		tbl += lastinc + tbu - now;
	}
	lastinc = now;

	*count = timer_conv_64(tbl);
	debug("%s(): tbl = %d, *count = %ld\n", __func__, tbl, *count);

	return 0;
}

static int s3c2440_timer_probe(struct udevice *dev)
{
	struct timer_dev_priv *uc_priv = dev_get_uclass_priv(dev);
	struct s3c2440_timer_priv *priv = dev_get_priv(dev);
	int ret;
	ulong tmr;

	priv->reg = dev_read_addr_ptr(dev);
	if (!priv->reg)
		return -ENOENT;

	ret = clk_get_by_name(dev, "timer_clk", &priv->clk);
	if (ret) {
		printf("%s(): get timer clk failed!\n", __func__);
		return -ENODEV;
	}

	ret = clk_enable(&priv->clk);
	if (ret) {
		printf("%s(): enable timer clk failed!\n", __func__);
		return -EAGAIN;
	}

	/* 
     * Use PWM Timer 4 because it has no output 
	 */
	/* Prescaler for Timer 4 is 16 */
	writel(0x0f00, &priv->reg->tcfg0);

	/* Load value for 10 ms timeout */
	uc_priv->clock_rate = clk_get_rate(&priv->clk) / (2 * 16);
	debug("%s(): uc_priv->clock_rate = %ld\n", __func__, uc_priv->clock_rate);
	if (!uc_priv->clock_rate)
		return -EINVAL;

	writel(uc_priv->clock_rate / 100, &priv->reg->tcntb4);

	/* Auto load, manual update of timer 4 */
	tmr = (readl(&priv->reg->tcon) & ~0x0700000) | 0x0600000;
	writel(tmr, &priv->reg->tcon);

	/* Auto load, start timer 4 */
	tmr = (tmr & ~0x0700000) | 0x0500000;
	writel(tmr, &priv->reg->tcon);

	return 0;
}

static const struct timer_ops s3c2440_timer_ops = {
	.get_count = s3c2440_timer_get_count,
};

static const struct udevice_id s3c2440_timer_ids[] = {
	{ .compatible = "samsung,s3c2440-timer" },
	{ }
};

U_BOOT_DRIVER(s3c2440_timer) = {
	.name = "s3c2440_timer",
	.id = UCLASS_TIMER,
	.of_match = s3c2440_timer_ids,
	.priv_auto_alloc_size = sizeof(struct s3c2440_timer_priv),
	.probe = s3c2440_timer_probe,
	.ops = &s3c2440_timer_ops,
	// .flags = DM_FLAG_PRE_RELOC,
};
