// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2015 Google, Inc
 *
 * (C) Copyright 2008-2014 s3c2440 Electronics
 * Peter, Software Engineering, <superpeter.cai@gmail.com>.
 */

#include <common.h>
#include <dm.h>
#include <syscon.h>
#include <linux/errno.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <dm/pinctrl.h>

enum {
	s3c2440_GPIOS_PER_BANK		= 32,
};

#define OFFSET_TO_BIT(bit)	(1UL << (bit))

struct s3c2440_gpio_regs {
	u32 gpio_con;
	u32 gpio_dat;
	u32 gpio_up;
};

struct s3c2440_gpio_priv {
	struct s3c2440_gpio_regs *regs;
	struct udevice *pinctrl;
	int bank;
	char name[2];
};

static int s3c2440_gpio_direction_input(struct udevice *dev, unsigned offset)
{
	struct s3c2440_gpio_priv *priv = dev_get_priv(dev);
	struct s3c2440_gpio_regs *regs = priv->regs;
	u32 temp_value;

	temp_value = readl(&regs->gpio_con);
	writel(temp_value & ~(0x3 << (offset *2)), &regs->gpio_con);

	return 0;
}

static int s3c2440_gpio_direction_output(struct udevice *dev, unsigned offset,
					  int value)
{
	struct s3c2440_gpio_priv *priv = dev_get_priv(dev);
	struct s3c2440_gpio_regs *regs = priv->regs;
	u32 temp_value;

	temp_value = readl(&regs->gpio_con);
	writel(temp_value & ~(0x3 << (offset * 2)), &regs->gpio_con);

	temp_value = readl(&regs->gpio_con);
	writel(temp_value | (0x1 << (offset * 2)), &regs->gpio_con);

	return 0;
}

static int s3c2440_gpio_get_value(struct udevice *dev, unsigned offset)
{
	struct s3c2440_gpio_priv *priv = dev_get_priv(dev);
	struct s3c2440_gpio_regs *regs = priv->regs;

	return readl(&regs->gpio_dat) & (1 << offset) ? 1 : 0;
}

static int s3c2440_gpio_set_value(struct udevice *dev, unsigned offset,
				   int value)
{
	struct s3c2440_gpio_priv *priv = dev_get_priv(dev);
	struct s3c2440_gpio_regs *regs = priv->regs;
	u32 temp_value;

	temp_value = readl(&regs->gpio_dat);

	if (value)
		writel(temp_value | (1 << offset), &regs->gpio_dat);
	else
		writel(temp_value & ~(1 << offset), &regs->gpio_dat);

	return 0;
}

static int s3c2440_gpio_get_function(struct udevice *dev, unsigned offset)
{
	struct s3c2440_gpio_priv *priv = dev_get_priv(dev);
	struct s3c2440_gpio_regs *regs = priv->regs;
	bool is_output;
	int ret;

	ret = pinctrl_get_gpio_mux(priv->pinctrl, priv->bank, offset);
	if (ret)
		return ret;
	is_output = readl(&regs->gpio_con) & OFFSET_TO_BIT(offset * 2);

	return is_output ? GPIOF_OUTPUT : GPIOF_INPUT;
}

static int s3c2440_gpio_probe(struct udevice *dev)
{
	struct gpio_dev_priv *uc_priv = dev_get_uclass_priv(dev);
	struct s3c2440_gpio_priv *priv = dev_get_priv(dev);
	char *end;
	int ret;

	priv->regs = dev_read_addr_ptr(dev);
	printf("%s(): priv->regs = 0x%x\n", __func__, priv->regs);

	ret = uclass_first_device_err(UCLASS_PINCTRL, &priv->pinctrl);
	if (ret)
		return ret;

	uc_priv->gpio_count = s3c2440_GPIOS_PER_BANK;
	// end = strrchr(dev->name, '@');
	// priv->bank = trailing_strtoln(dev->name, end);
	// priv->name[0] = 'A' + priv->bank;
	// uc_priv->bank_name = priv->name;
	uc_priv->bank_name = dev->name;
	// printf("%s(): dev->name = %s, end = %s, priv->bank = %d,"
	// 		"priv->name[0] = %c, uc_priv->bank_name = %s\n",
	// 		__func__, dev->name, end, priv->bank, priv->name[0], uc_priv->bank_name);

	return 0;
}

static const struct dm_gpio_ops gpio_s3c2440_ops = {
	.direction_input	= s3c2440_gpio_direction_input,
	.direction_output	= s3c2440_gpio_direction_output,
	.get_value		= s3c2440_gpio_get_value,
	.set_value		= s3c2440_gpio_set_value,
	.get_function		= s3c2440_gpio_get_function,
};

static const struct udevice_id s3c2440_gpio_ids[] = {
	{ .compatible = "samsung,s3c2440-gpio-bank" },
	{ }
};

U_BOOT_DRIVER(gpio_s3c2440) = {
	.name	= "gpio_s3c2440",
	.id	= UCLASS_GPIO,
	.of_match = s3c2440_gpio_ids,
	.ops	= &gpio_s3c2440_ops,
	.priv_auto_alloc_size = sizeof(struct s3c2440_gpio_priv),
	.probe	= s3c2440_gpio_probe,
};
