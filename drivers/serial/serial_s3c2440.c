/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2020 Asymptote
 */

// #define DEBUG

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <fdtdec.h>
#include <linux/compiler.h>
#include <asm/io.h>
#include <serial.h>
#include <clk.h>

struct s3c2440_serial {
	u32 ulcon;
	u32 ucon;
	u32 ufcon;
	u32 umcon;
	u32 utrstat;
	u32 uerstat;
	u32 ufstat;
	u32 umstat;
	u32 utxh;
	u32 urxh;
	u32 ubrdiv;
};

struct s3c2440_serial_priv {
	struct clk clk;
	struct s3c2440_serial *reg;
};

int s3c2440_serial_setbrg(struct udevice *dev, int baudrate)
{
	struct s3c2440_serial_priv *priv = dev_get_priv(dev);
	u32 val, uart_clk;

	/* 使用pclk时钟，默认大小为50MHz */
	uart_clk = clk_get_rate(&priv->clk);
	debug("%s(): uart_clk = %d\n", __func__, uart_clk);

	val = uart_clk / baudrate;
	writel(val / 16 - 1, &priv->reg->ubrdiv);

	return 0;
}

static int s3c2440_serial_getc(struct udevice *dev)
{
	struct s3c2440_serial_priv *priv = dev_get_priv(dev);

	if (!(readl(&priv->reg->utrstat) & (1 << 0)))
		return -EAGAIN;

	return (int)(readb(&priv->reg->urxh) & 0xff);
}

static int s3c2440_serial_putc(struct udevice *dev, const char ch)
{
	struct s3c2440_serial_priv *priv = dev_get_priv(dev);

	if (!(readl(&priv->reg->utrstat) & (1 << 2)))
		return -EAGAIN;

	writeb(ch, &priv->reg->utxh);

	return 0;
}

static int s3c2440_serial_pending(struct udevice *dev, bool input)
{
	struct s3c2440_serial_priv *priv = dev_get_priv(dev);
	uint32_t utrstat;

	utrstat = readl(&priv->reg->utrstat);
	
	if (input)
		return (utrstat & (1 << 0));
	else
		return (utrstat & (1 << 2));
}

static const struct dm_serial_ops s3c2440_serial_ops = {
	.putc = s3c2440_serial_putc,
	.pending = s3c2440_serial_pending,
	.getc = s3c2440_serial_getc,
	.setbrg = s3c2440_serial_setbrg,
};

#define GPHCON  (*(volatile unsigned long *)0x56000070)
#define GPHUP	(*(volatile unsigned long *)0x56000078)

static int s3c2440_serial_probe(struct udevice *dev)
{
	struct s3c2440_serial_priv *priv = dev_get_priv(dev);
	int ret;

	priv->reg = (void *)dev_read_addr(dev);

	ret = clk_get_by_name(dev, "uart0_clk", &priv->clk);
	if (ret) {
		printf("%s(): get uart clk failed!\n", __func__);
		return -ENODEV;
	}

	ret = clk_enable(&priv->clk);
	if (ret) {
		printf("%s(): enable uart clk failed!\n", __func__);
		return -EAGAIN;
	}
	
 	/* GPH2,GPH3用作TXD0,RXD0 */
    GPHCON |= 0xa0;   
    /* GPH2,GPH3内部上拉 */
	GPHUP = 0x0c;     

    /* 8N1(8个数据位，无较验，1个停止位) */
	writel(0x03, &priv->reg->ulcon);
    /* 查询方式，UART时钟源为PCLK */
	writel(0x05, &priv->reg->ucon);
    /* 不使用FIFO */
	writel(0x00, &priv->reg->ufcon);
    /* 不使用流控 */
	writel(0x00, &priv->reg->umcon);

	return 0;
}

static const struct udevice_id s3c2440_serial_ids[] = {
	{ .compatible = "samsung,s3c2440-uart" },
	{ }
};

U_BOOT_DRIVER(serial_s3c2440) = {
	.name = "serial_s3c2440",
	.id	= UCLASS_SERIAL,
	.of_match = s3c2440_serial_ids,
	.probe = s3c2440_serial_probe,
	.ops = &s3c2440_serial_ops,
	.priv_auto_alloc_size = sizeof(struct s3c2440_serial_priv),
	// .flags = DM_FLAG_PRE_RELOC,
};
