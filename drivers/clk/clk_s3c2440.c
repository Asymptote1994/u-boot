// SPDX-License-Identifier: GPL-2.0
/*
 * Samsung s3c2440 clock driver
 *
 * Copyright (C) 2020 Asymptote
 */

#define DEBUG

#include <common.h>
#include <clk-uclass.h>
#include <dm.h>
#include <errno.h>
#include <asm/io.h>
#include <dt-bindings/clock/s3c2440-clock.h>

#define MPLL 0
#define UPLL 1

#define CLK_ENABLE		1
#define CLK_DISABLE		0

struct s3c2440_clk_priv {
	struct s3c2440_clk_regs *reg;
	ulong xtal_frequency;
	ulong fclk;
	ulong hclk;
	ulong pclk;
	ulong uclk;
};

struct s3c2440_clk_regs {
	u32 locktime;
	u32 mpllcon;
	u32 upllcon;
	u32 clkcon;
	u32 clkslow;
	u32 clkdivn;
	u32 camdivn;
};

static void s3c2440_clk_set_clkcon(struct s3c2440_clk_priv *priv, unsigned long id, bool flag)
{
	u32 value;

	value = readl(&priv->reg->clkcon);

	if (flag == CLK_ENABLE)
		writel(value | (1 << id), &priv->reg->clkcon);
	else if (flag == CLK_DISABLE)
		writel(value & ~(1 << id), &priv->reg->clkcon);
}

static int s3c2440_clk_enable(struct clk *clk)
{
	struct s3c2440_clk_priv *priv = dev_get_priv(clk->dev);

	debug("enter %s(): clk->id = %ld\n", __func__, clk->id);

	switch (clk->id) {
	case HCLK_NAND_FLASH:
		s3c2440_clk_set_clkcon(priv, HCLK_NAND_FLASH, CLK_ENABLE);
		break;
	case HCLK_LCDC:
		s3c2440_clk_set_clkcon(priv, HCLK_LCDC, CLK_ENABLE);
		break;
	case HCLK_USB_HOST:
		s3c2440_clk_set_clkcon(priv, HCLK_USB_HOST, CLK_ENABLE);
		break;
	case PCLK_USB_DEVICE:
		s3c2440_clk_set_clkcon(priv, PCLK_USB_DEVICE, CLK_ENABLE);
		break;
	case PCLK_PWMTIMER:
		s3c2440_clk_set_clkcon(priv, PCLK_PWMTIMER, CLK_ENABLE);
		break;
	case PCLK_SDI:
		s3c2440_clk_set_clkcon(priv, PCLK_SDI, CLK_ENABLE);
		break;
	case PCLK_UART0:
		s3c2440_clk_set_clkcon(priv, PCLK_UART0, CLK_ENABLE);
		break;
	case PCLK_UART1:
		s3c2440_clk_set_clkcon(priv, PCLK_UART1, CLK_ENABLE);
		break;
	case PCLK_UART2:
		s3c2440_clk_set_clkcon(priv, PCLK_UART2, CLK_ENABLE);
		break;
	case PCLK_GPIO:
		s3c2440_clk_set_clkcon(priv, PCLK_GPIO, CLK_ENABLE);
		break;
	case PCLK_RTC:
		s3c2440_clk_set_clkcon(priv, PCLK_RTC, CLK_ENABLE);
		break;
	case PCLK_ADC:
		s3c2440_clk_set_clkcon(priv, PCLK_ADC, CLK_ENABLE);
		break;
	case PCLK_IIC:
		s3c2440_clk_set_clkcon(priv, PCLK_IIC, CLK_ENABLE);
		break;
	case PCLK_IIS:
		s3c2440_clk_set_clkcon(priv, PCLK_IIS, CLK_ENABLE);
		break;
	case PCLK_SPI:
		s3c2440_clk_set_clkcon(priv, PCLK_SPI, CLK_ENABLE);
		break;
	case HCLK_CAMERA:
		s3c2440_clk_set_clkcon(priv, HCLK_CAMERA, CLK_ENABLE);
		break;
	case PCLK_AC97:
		s3c2440_clk_set_clkcon(priv, PCLK_AC97, CLK_ENABLE);
		break;
	default:
		return -ENOENT;
	}

	return 0;
}

static int s3c2440_clk_disable(struct clk *clk)
{
	struct s3c2440_clk_priv *priv = dev_get_priv(clk->dev);

	debug("enter %s(): clk->id = %ld\n", __func__, clk->id);

	switch (clk->id) {
	case HCLK_NAND_FLASH:
		s3c2440_clk_set_clkcon(priv, HCLK_NAND_FLASH, CLK_DISABLE);
		break;
	case HCLK_LCDC:
		s3c2440_clk_set_clkcon(priv, HCLK_LCDC, CLK_DISABLE);
		break;
	case HCLK_USB_HOST:
		s3c2440_clk_set_clkcon(priv, HCLK_USB_HOST, CLK_DISABLE);
		break;
	case PCLK_USB_DEVICE:
		s3c2440_clk_set_clkcon(priv, PCLK_USB_DEVICE, CLK_DISABLE);
		break;
	case PCLK_PWMTIMER:
		s3c2440_clk_set_clkcon(priv, PCLK_PWMTIMER, CLK_DISABLE);
		break;
	case PCLK_SDI:
		s3c2440_clk_set_clkcon(priv, PCLK_SDI, CLK_DISABLE);
		break;
	case PCLK_UART0:
		s3c2440_clk_set_clkcon(priv, PCLK_UART0, CLK_DISABLE);
		break;
	case PCLK_UART1:
		s3c2440_clk_set_clkcon(priv, PCLK_UART1, CLK_DISABLE);
		break;
	case PCLK_UART2:
		s3c2440_clk_set_clkcon(priv, PCLK_UART2, CLK_DISABLE);
		break;
	case PCLK_GPIO:
		s3c2440_clk_set_clkcon(priv, PCLK_GPIO, CLK_DISABLE);
		break;
	case PCLK_RTC:
		s3c2440_clk_set_clkcon(priv, PCLK_RTC, CLK_DISABLE);
		break;
	case PCLK_ADC:
		s3c2440_clk_set_clkcon(priv, PCLK_ADC, CLK_DISABLE);
		break;
	case PCLK_IIC:
		s3c2440_clk_set_clkcon(priv, PCLK_IIC, CLK_DISABLE);
		break;
	case PCLK_IIS:
		s3c2440_clk_set_clkcon(priv, PCLK_IIS, CLK_DISABLE);
		break;
	case PCLK_SPI:
		s3c2440_clk_set_clkcon(priv, PCLK_SPI, CLK_DISABLE);
		break;
	case HCLK_CAMERA:
		s3c2440_clk_set_clkcon(priv, HCLK_CAMERA, CLK_DISABLE);
		break;
	case PCLK_AC97:
		s3c2440_clk_set_clkcon(priv, PCLK_AC97, CLK_DISABLE);
		break;
	default:
		return -ENOENT;
	}

	return 0;
}

static ulong s3c2440_clk_get_rate(struct clk *clk)
{
	struct s3c2440_clk_priv *priv = dev_get_priv(clk->dev);
	ulong rate;

	debug("enter %s(): clk->id = %ld\n", __func__, clk->id);

	switch (clk->id) {
	case PCLK_USB_DEVICE:
	case PCLK_PWMTIMER:
	case PCLK_SDI:
	case PCLK_UART0:
	case PCLK_UART1:
	case PCLK_UART2:
	case PCLK_GPIO:
	case PCLK_RTC:
	case PCLK_ADC:
	case PCLK_IIC:
	case PCLK_IIS:
	case PCLK_SPI:
	case PCLK_AC97:
		rate = priv->pclk;
		break;
	case HCLK_NAND_FLASH:
	case HCLK_LCDC:
	case HCLK_USB_HOST:
	case HCLK_CAMERA:
		rate = priv->hclk;
		break;
	default:
		return -ENOENT;
	}

	return rate;
}

#if 0
static ulong s3c2440_clk_set_rate(struct clk *clk, ulong rate)
{
	return 0;
}
#endif

struct clk_ops s3c2440_clk_ops = {
	.enable = s3c2440_clk_enable,
	.disable = s3c2440_clk_disable,
	.get_rate = s3c2440_clk_get_rate,
	// .set_rate	= s3c2440_clk_set_rate,
};

static ulong s3c2440_get_pllclk(struct s3c2440_clk_priv *priv, int pllreg)
{
	ulong r, m, p, s;

	if (pllreg == MPLL)
		r = readl(&priv->reg->mpllcon);
	else if (pllreg == UPLL)
		r = readl(&priv->reg->upllcon);

	m = ((r & 0xFF000) >> 12) + 8;
	p = ((r & 0x003F0) >> 4) + 2;
	s = r & 0x3;

	if (pllreg == MPLL)
		return 2 * m * (priv->xtal_frequency / (p << s));

	return (priv->xtal_frequency * m) / (p << s);
}

static ulong s3c2440_get_fclk(struct s3c2440_clk_priv *priv)
{
	return s3c2440_get_pllclk(priv, MPLL);
}

static ulong s3c2440_get_hclk(struct s3c2440_clk_priv *priv)
{
	switch (readl(&priv->reg->clkdivn) & 0x6) {
	default:
	case 0:
		return s3c2440_get_fclk(priv);
	case 2:
		return s3c2440_get_fclk(priv) / 2;
	case 4:
		return (readl(&priv->reg->camdivn) & (1 << 9)) ?
			s3c2440_get_fclk(priv) / 8 : s3c2440_get_fclk(priv) / 4;
	case 6:
		return (readl(&priv->reg->camdivn) & (1 << 8)) ?
			s3c2440_get_fclk(priv) / 6 : s3c2440_get_fclk(priv) / 3;
	}
}

static ulong s3c2440_get_pclk(struct s3c2440_clk_priv *priv)
{
	return (readl(&priv->reg->clkdivn) & 1) ? s3c2440_get_hclk(priv) / 2 : s3c2440_get_hclk(priv);
}

static ulong s3c2440_get_uclk(struct s3c2440_clk_priv *priv)
{
	return s3c2440_get_pllclk(priv, UPLL);
}

int s3c2440_clk_probe(struct udevice *dev)
{
	struct s3c2440_clk_priv *priv = dev_get_priv(dev);

	priv->reg = dev_read_addr_ptr(dev);
	if (!priv->reg)
		return -ENOMEM;

	priv->xtal_frequency = dev_read_u32_default(dev, "xtal-frequency", 12000000);
	debug("%s(): xtal_frequency = %ld\n", __func__, priv->xtal_frequency);

	/* Get FCLK */
	priv->fclk = s3c2440_get_fclk(priv);
	/* Get HCLK */
	priv->hclk = s3c2440_get_hclk(priv);
	/* Get PCLK */
	priv->pclk = s3c2440_get_pclk(priv);
	/* Get UCLK */
	priv->uclk = s3c2440_get_uclk(priv);

	debug("%s(): FCLK = %ld, HCLK = %ld, PCLK = %ld, UCLK = %ld\n",
			__func__, priv->fclk, priv->hclk, priv->pclk, priv->uclk);
	
	/* Disable all clock output */
	// writel(0, &priv->reg->clkcon);
	// writel(readl(&priv->reg->clkcon) & ~(1 << 10), &priv->reg->clkcon);
	// writel(readl(&priv->reg->clkcon) & ~(1 << 13), &priv->reg->clkcon);
	debug("%s(): clkcon = 0x%x\n", __func__, readl(&priv->reg->clkcon));

	return 0;
}

static const struct udevice_id s3c2440_clk_ids[] = {
	{ .compatible = "samsung,s3c2440-clk" },
	{ }
};

U_BOOT_DRIVER(clk_s3c2440) = {
	.name		= "clk_s3c2440",
	.id		= UCLASS_CLK,
	.of_match	= s3c2440_clk_ids,
	.priv_auto_alloc_size	= sizeof(struct s3c2440_clk_priv),
	.ops		= &s3c2440_clk_ops,
	.probe		= s3c2440_clk_probe,
};
