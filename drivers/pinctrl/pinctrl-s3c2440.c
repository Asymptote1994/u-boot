// SPDX-License-Identifier: GPL-2.0
/*
 * Samsung s3c2440 pinctrl driver
 *
 * Copyright (C) 2020 Asymptote
 */

#define DEBUG

#include <common.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <linux/io.h>
#include <linux/err.h>
#include <dt-bindings/pinctrl/s3c2440.h>

#define MAX_GPIO_BANKS			9
#define MAX_NB_GPIO_PER_BANK	32

#define MAX_PINMUX_ENTRIES		200

struct s3c2440_pinctrl_priv {
	u32 mux_reg_base[MAX_GPIO_BANKS];
	u32 conf_reg_base[MAX_GPIO_BANKS];
	u32 nbanks;
};

static int s3c2440_set_pinmux(struct udevice *dev, u32 bank, u32 pin, u32 mux)
{
	u32 temp_value;
	struct s3c2440_pinctrl_priv *priv = dev_get_priv(dev);
	
	debug("%s(): bank = %d, pin = %d, mux = %d\n", __func__, bank, pin, mux);
	debug("%s(): reg_base = 0x%x\n", __func__, priv->mux_reg_base[bank]);

	temp_value = readl(priv->mux_reg_base[bank]) & ~(0x3 << (pin * 2));

	switch (mux) {
	case S3C2440_MUX_GPIO:
		// writel(temp_value | (0x2 << (pin * 2)), bank);
		debug("%s(): S3C2440_MUX_GPIO\n", __func__);
		break;
	case S3C2440_MUX_PERIPH_A:
		writel(temp_value | (0x2 << (pin * 2)), priv->mux_reg_base[bank]);
		debug("%s(): S3C2440_MUX_PERIPH_A\n", __func__);
		break;
	case S3C2440_MUX_PERIPH_B:
		writel(temp_value | (0x3 << (pin * 2)), priv->mux_reg_base[bank]);
		debug("%s(): S3C2440_MUX_PERIPH_B\n", __func__);
		break;
	}

	return 0;
}

static int s3c2440_set_pinconf(struct udevice *dev, u32 bank, u32 pin, u32 config)
{
	u32 temp_value;
	struct s3c2440_pinctrl_priv *priv = dev_get_priv(dev);

	debug("%s(): bank = %d, pin = %d, config = %d\n", __func__, bank, pin, config);
	debug("%s(): reg_base = 0x%x\n", __func__, priv->conf_reg_base[bank]);

	temp_value = readl(priv->conf_reg_base[bank]);

	switch (config) {
	case S3C2440_PINCTRL_NONE:
		writel(temp_value | (1 << pin), priv->conf_reg_base[bank]);
		debug("%s(): S3C2440_PINCTRL_NONE\n", __func__);
		break;
	case S3C2440_PINCTRL_PULL_UP:
		writel(temp_value & ~(1 << pin), priv->conf_reg_base[bank]);
		debug("%s(): S3C2440_PINCTRL_PULL_UP\n", __func__);
		break;
	}
	
	return 0;
}

static int s3c2440_pin_check_config(struct udevice *dev, u32 bank, u32 pin)
{
	struct s3c2440_pinctrl_priv *priv = dev_get_priv(dev);

	if (bank >= priv->nbanks) {
		debug("%s(): pin conf bank %d >= nbanks %d\n", __func__, bank, priv->nbanks);
		return -EINVAL;
	}

	if (pin >= MAX_NB_GPIO_PER_BANK) {
		debug("%s(): pin conf pin %d >= %d\n", __func__, pin, MAX_NB_GPIO_PER_BANK);
		return -EINVAL;
	}

	return 0;
}

static int s3c2440_pinctrl_set_state(struct udevice *dev, struct udevice *config)
{
	struct s3c2440_pinctrl_priv *priv = dev_get_priv(dev);
	const void *blob = gd->fdt_blob;
	int node = dev_of_offset(config);
	u32 cells[MAX_PINMUX_ENTRIES];
	const u32 *list = cells;
	u32 bank, pin, mux, conf;
	u32 size, count;
	int i, ret;

	/*
	 * the binding format is s3c2440,pins = <bank pin mux CONFIG ...>,
	 * do sanity check and calculate pins number
	 */
	size = fdtdec_get_int_array_count(blob, node, "s3c2440,pins",
					  cells, ARRAY_SIZE(cells));

	/* we do not check return since it's safe node passed down */
	count = size >> 2;

	debug("%s(): size = %d, count = %d\n", __func__, size, count);

	if (!count)
		return -EINVAL;

	for (i = 0; i < count; i++) {
		bank = *list++;
		pin = *list++;
		mux = *list++;
		conf = *list++;
		
		ret = s3c2440_pin_check_config(dev, bank, pin);
		if (ret)
			return ret;

		ret = s3c2440_set_pinmux(dev, bank, pin, mux);
		if (ret)
			return ret;

		ret = s3c2440_set_pinconf(dev, bank, pin, conf);
		if (ret)
			return ret;
	}

	return 0;
}

const struct pinctrl_ops s3c2440_pinctrl_ops  = {
	.set_state = s3c2440_pinctrl_set_state,
};

static int s3c2440_pinctrl_probe(struct udevice *dev)
{
	struct s3c2440_pinctrl_priv *priv = dev_get_priv(dev);
	int index;
	u32 mux_reg_base[MAX_GPIO_BANKS] = {0x56000000, 0x56000010, 0x56000020, 0x56000030,
			0x56000040, 0x56000050, 0x56000060, 0x56000070, 0x560000d0};
	/* GPIOA have no Pull-up option */
	u32 conf_reg_base[MAX_GPIO_BANKS] = {0x56000000, 0x56000018, 0x56000028, 0x56000038,
			0x56000048, 0x56000058, 0x56000068, 0x56000078, 0x560000d8};

	debug("enter %s()\n", __func__);

	for (index = 0; index < MAX_GPIO_BANKS; index++) {
		priv->mux_reg_base[index] = mux_reg_base[index];
		priv->conf_reg_base[index] = conf_reg_base[index];
	}

	priv->nbanks = index;

	return 0;
}

static const struct udevice_id s3c2440_pinctrl_match[] = {
	{ .compatible = "samsung,s3c2440-pinctrl" },
	{}
};

U_BOOT_DRIVER(s3c2440_pinctrl) = {
	.name = "pinctrl_s3c2440",
	.id = UCLASS_PINCTRL,
	.of_match = s3c2440_pinctrl_match,
	.probe = s3c2440_pinctrl_probe,
	.priv_auto_alloc_size = sizeof(struct s3c2440_pinctrl_priv),
	.ops = &s3c2440_pinctrl_ops,
};
