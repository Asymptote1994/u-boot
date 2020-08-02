/*
 * S3C2440 SD/MMC driver
 *
 * Based on OpenMoko S3C24xx driver by Harald Welte <laforge@openmoko.org>
 *
 * Copyright (C) 2014 Marek Vasut <marex@denx.de>
 * Copyright (C) 2020 Asymptote
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <malloc.h>
#include <mmc.h>
#include <clk.h>
#include <reset.h>
#include <asm/io.h>
#include <asm-generic/gpio.h>
#include <asm/unaligned.h>

#define S3C2440_SDICON_SDRESET		(1 << 8)
#define S3C2410_SDICON_FIFORESET	(1 << 1)
#define S3C2410_SDICON_CLOCKTYPE	(1 << 0)

#define S3C2410_SDICMDCON_LONGRSP	(1 << 10)
#define S3C2410_SDICMDCON_WAITRSP	(1 << 9)
#define S3C2410_SDICMDCON_CMDSTART	(1 << 8)
#define S3C2410_SDICMDCON_SENDERHOST	(1 << 6)
#define S3C2410_SDICMDCON_INDEX		0x3f

#define S3C2410_SDICMDSTAT_CRCFAIL	(1 << 12)
#define S3C2410_SDICMDSTAT_CMDSENT	(1 << 11)
#define S3C2410_SDICMDSTAT_CMDTIMEOUT	(1 << 10)
#define S3C2410_SDICMDSTAT_RSPFIN	(1 << 9)

#define S3C2440_SDIDCON_DS_WORD		(2 << 22)
#define S3C2410_SDIDCON_TXAFTERRESP	(1 << 20)
#define S3C2410_SDIDCON_RXAFTERCMD	(1 << 19)
#define S3C2410_SDIDCON_BLOCKMODE	(1 << 17)
#define S3C2410_SDIDCON_WIDEBUS		(1 << 16)
#define S3C2440_SDIDCON_DATSTART	(1 << 14)
#define S3C2410_SDIDCON_XFER_RXSTART	(2 << 12)
#define S3C2410_SDIDCON_XFER_TXSTART	(3 << 12)
#define S3C2410_SDIDCON_BLKNUM		0x7ff

#define S3C2410_SDIDSTA_FIFOFAIL	(1 << 8)
#define S3C2410_SDIDSTA_CRCFAIL		(1 << 7)
#define S3C2410_SDIDSTA_RXCRCFAIL	(1 << 6)
#define S3C2410_SDIDSTA_DATATIMEOUT	(1 << 5)
#define S3C2410_SDIDSTA_XFERFINISH	(1 << 4)

#define S3C2410_SDIFSTA_TFHALF		(1 << 11)
#define S3C2410_SDIFSTA_COUNTMASK	0x7f

struct s3c2440_mmc {
	u32 sdicon;			/* 0x00 control */
	u32 sdipre;			/* 0x04 baud rate prescaler */
	u32 sdicarg;		/* 0x08 command argument */
	u32 sdiccon;		/* 0x0c command control */
	u32 sdicsta;		/* 0x10 command status */
	u32 sdirsp0;		/* 0x14 response 0 */
	u32 sdirsp1;		/* 0x18 response 1 */
	u32 sdirsp2;		/* 0x1c response 2 */
	u32 sdirsp3;		/* 0x20 response 3 */	
	u32 sdidtimer;		/* 0x24 data/busy timer */
	u32 sdibsize;		/* 0x28 block size */
	u32 sdidcon;		/* 0x2c data control */
	u32 sdidcnt;		/* 0x30 data remain counter */
	u32 sdidsta;		/* 0x34 data status */
	u32 sdifsta;		/* 0x38 FIFO status */
	u32 sdiimsk;		/* 0x3c interrupt mask */
	u32 sdidat;			/* 0x40 data */
};

struct s3c2440_mmc_plat {
	struct mmc_config cfg;
	struct mmc mmc;
};

struct s3c2440_mmc_priv {
	struct clk clk;
	struct gpio_desc cd_gpio;
	struct s3c2440_mmc *reg;
	struct mmc_config cfg;
};

/*
 * WARNING: We only support one SD IP block.
 * NOTE: It's not likely there will ever exist an S3C24xx with two,
 *       at least not in this universe all right.
 */
static int s3c2440_mmc_send_cmd(struct udevice *dev, struct mmc_cmd *cmd,
			      struct mmc_data *data)
{
	struct s3c2440_mmc_plat *plat = dev_get_platdata(dev);
	struct s3c2440_mmc_priv *priv = dev_get_priv(dev);
	struct mmc *mmc = &plat->mmc;

	unsigned int timeout = 100000;
	int ret = 0, xfer_len, data_offset = 0;
	uint32_t sdiccon, sdicsta, sdidcon, sdidsta, sdidat, sdifsta;
	uint32_t sdicsta_wait_bit = S3C2410_SDICMDSTAT_CMDSENT;
	const uint32_t sdidsta_err_mask = S3C2410_SDIDSTA_FIFOFAIL |
		S3C2410_SDIDSTA_CRCFAIL | S3C2410_SDIDSTA_RXCRCFAIL |
		S3C2410_SDIDSTA_DATATIMEOUT;

	writel(0xffffffff, &priv->reg->sdicsta);
	writel(0xffffffff, &priv->reg->sdidsta);
	writel(0xffffffff, &priv->reg->sdifsta);

	/* Set up data transfer (if applicable). */
	if (data) {
		writel(data->blocksize, &priv->reg->sdibsize);

		sdidcon = data->blocks & S3C2410_SDIDCON_BLKNUM;
		sdidcon |= S3C2410_SDIDCON_BLOCKMODE;
		sdidcon |= S3C2440_SDIDCON_DS_WORD | S3C2440_SDIDCON_DATSTART;

		if (mmc->bus_width == 4)
			sdidcon |= S3C2410_SDIDCON_WIDEBUS;

		if (data->flags & MMC_DATA_READ) {
			sdidcon |= S3C2410_SDIDCON_RXAFTERCMD;
			sdidcon |= S3C2410_SDIDCON_XFER_RXSTART;
		} else {
			sdidcon |= S3C2410_SDIDCON_TXAFTERRESP;
			sdidcon |= S3C2410_SDIDCON_XFER_TXSTART;
		}

		writel(sdidcon, &priv->reg->sdidcon);
	}

	/* Write CMD arg. */
	writel(cmd->cmdarg, &priv->reg->sdicarg);

	/* Write CMD index. */
	sdiccon = cmd->cmdidx & S3C2410_SDICMDCON_INDEX;
	sdiccon |= S3C2410_SDICMDCON_SENDERHOST;
	sdiccon |= S3C2410_SDICMDCON_CMDSTART;

	/* Command with short response. */
	if (cmd->resp_type & MMC_RSP_PRESENT) {
		sdiccon |= S3C2410_SDICMDCON_WAITRSP;
		sdicsta_wait_bit = S3C2410_SDICMDSTAT_RSPFIN;
	}

	/* Command with long response. */
	if (cmd->resp_type & MMC_RSP_136)
		sdiccon |= S3C2410_SDICMDCON_LONGRSP;

	/* Start the command. */
	writel(sdiccon, &priv->reg->sdiccon);

	/* Wait for the command to complete or for response. */
	for (timeout = 100000; timeout; timeout--) {
		sdicsta = readl(&priv->reg->sdicsta);
		if (sdicsta & sdicsta_wait_bit)
			break;

		if (sdicsta & S3C2410_SDICMDSTAT_CMDTIMEOUT)
			timeout = 1;
	}

	/* Clean the status bits. */
	writel(readl(&priv->reg->sdicsta) | (0xf << 9), &priv->reg->sdicsta);
    // setbits_le32(&priv->reg->sdicsta, 0xf << 9);

	if (!timeout) {
		printf("S3C SDI: Command timed out!\n");
		ret = -ETIMEDOUT;
		goto error;
	}

	/* Read out the response. */
	if (cmd->resp_type & MMC_RSP_136) {
		cmd->response[0] = readl(&priv->reg->sdirsp0);
		cmd->response[1] = readl(&priv->reg->sdirsp1);
		cmd->response[2] = readl(&priv->reg->sdirsp2);
		cmd->response[3] = readl(&priv->reg->sdirsp3);
	} else {
		cmd->response[0] = readl(&priv->reg->sdirsp0);
	}

	/* If there are no data, we're done. */
	if (!data)
		return 0;

	xfer_len = data->blocksize * data->blocks;

	while (xfer_len > 0) {
		sdidsta = readl(&priv->reg->sdidsta);
		sdifsta = readl(&priv->reg->sdifsta);

		if (sdidsta & sdidsta_err_mask) {
			printf("S3C SDI: Data error (sdta=0x%08x)\n", sdidsta);
			ret = -EIO;
			goto error;
		}

		if (data->flags & MMC_DATA_READ) {
			if ((sdifsta & S3C2410_SDIFSTA_COUNTMASK) < 4)
				continue;
			sdidat = readl(&priv->reg->sdidat);
			put_unaligned_le32(sdidat, data->dest + data_offset);
		} else {	/* Write */
			/* TX FIFO half full. */
			if (!(sdifsta & S3C2410_SDIFSTA_TFHALF))
				continue;

			/* TX FIFO is below 32b full, write. */
			sdidat = get_unaligned_le32(data->src + data_offset);
			writel(sdidat, &priv->reg->sdidat);
		}
		data_offset += 4;
		xfer_len -= 4;
	}

	/* Wait for the command to complete or for response. */
	for (timeout = 100000; timeout; timeout--) {
		sdidsta = readl(&priv->reg->sdidsta);
		if (sdidsta & S3C2410_SDIDSTA_XFERFINISH)
			break;

		if (sdidsta & S3C2410_SDIDSTA_DATATIMEOUT)
			timeout = 1;
	}

	/* Clear status bits. */
	writel(0x6f8, &priv->reg->sdidsta);

	if (!timeout) {
		printf("S3C SDI: Command timed out!\n");
		ret = -ETIMEDOUT;
		goto error;
	}

	writel(0, &priv->reg->sdidcon);

	return 0;
error:
	return ret;
}

static int s3c2440_mmc_set_ios(struct udevice *dev)
{
	struct s3c2440_mmc_plat *plat = dev_get_platdata(dev);
	struct s3c2440_mmc_priv *priv = dev_get_priv(dev);
	struct mmc *mmc = &plat->mmc;
    uint32_t divider = 0;
	u32 sdmmc_clk;

	printf("set ios: bus_width: %x, clock: %d\n",
	      mmc->bus_width, mmc->clock);

	if (!mmc->clock)
		return -1;

	sdmmc_clk = clk_get_rate(&priv->clk);
	printf("%s(): sdmmc_clk = %d\n", __func__, sdmmc_clk);

	divider = DIV_ROUND_UP(sdmmc_clk, mmc->clock);
	if (divider)
		divider--;

	writel(divider, &priv->reg->sdipre);
	mdelay(125);

    return 0;
}

static int s3c2440_mmc_getcd(struct udevice *dev)
{
	struct s3c2440_mmc_priv *priv = dev_get_priv(dev);
	int value, ret;

	ret = gpio_request_by_name(dev, "cd-gpio", 0, &priv->cd_gpio, GPIOD_IS_IN);
	if (ret) {
		printf("%s(): gpio_request_by_name failed!\n", __func__);
		return ret;
	}
	
	if (!dm_gpio_is_valid(&priv->cd_gpio)) {
		printf("%s(): dm_gpio_is_valid failed!\n", __func__);
		return ret;
	}

	value = dm_gpio_get_value(&priv->cd_gpio);
	printf("%s(): value = %d\n", __func__, value);

	dm_gpio_free(dev, &priv->cd_gpio);
	
	return value;
}

static int s3c2440_mmc_getwp(struct udevice *dev)
{
	/* There is allways no write-protected */
	return 0;
}

static const struct dm_mmc_ops s3c2440_mmc_ops = {
	.send_cmd	= s3c2440_mmc_send_cmd,
	.set_ios	= s3c2440_mmc_set_ios,
	.get_cd		= s3c2440_mmc_getcd,
	.get_wp		= s3c2440_mmc_getwp,
};

static int s3c2440_mmc_probe(struct udevice *dev)
{
	struct mmc_uclass_priv *upriv = dev_get_uclass_priv(dev);
	struct s3c2440_mmc_plat *plat = dev_get_platdata(dev);
	struct s3c2440_mmc_priv *priv = dev_get_priv(dev);
	struct mmc_config *cfg = &plat->cfg;
	int bus_width, ret;
	u32 sdmmc_clk;

	priv->reg = (void *)dev_read_addr(dev);
	bus_width = dev_read_u32_default(dev, "bus-width", 1);

	ret = clk_get_by_name(dev, "sdmmc_clk", &priv->clk);
	if (ret) {
		printf("%s(): get sdmmc clk failed!\n", __func__);
		return -ENODEV;
	}

	ret = clk_enable(&priv->clk);
	if (ret) {
		printf("%s(): enable sdmmc clk failed!\n", __func__);
		return -EAGAIN;
	}

	sdmmc_clk = clk_get_rate(&priv->clk);
	printf("%s(): sdmmc_clk = %d\n", __func__, sdmmc_clk);

	cfg->name = dev->name;
	cfg->voltages = MMC_VDD_32_33 | MMC_VDD_33_34;
	cfg->host_caps = MMC_MODE_4BIT | MMC_MODE_HS;
	cfg->f_min = 400000;
	cfg->f_max = sdmmc_clk / 2;
	cfg->b_max = 0x80;

	upriv->mmc = &plat->mmc;

	writel(S3C2440_SDICON_SDRESET, &priv->reg->sdicon);
	mdelay(10);
	writel(0x7fffff, &priv->reg->sdidtimer);

	writel(MMC_MAX_BLOCK_LEN, &priv->reg->sdibsize);
	writel(0x0, &priv->reg->sdiimsk);

	writel(S3C2410_SDICON_FIFORESET | S3C2410_SDICON_CLOCKTYPE,
	       &priv->reg->sdicon);

	mdelay(125);

	return 0;
}

static int s3c2440_mmc_bind(struct udevice *dev)
{
	struct s3c2440_mmc_plat *plat = dev_get_platdata(dev);

	return mmc_bind(dev, &plat->mmc, &plat->cfg);
}

static const struct udevice_id s3c2440_mmc_ids[] = {
	{ .compatible = "samsung,s3c2440-mmc"},
	{ /* sentinel */ }
};

U_BOOT_DRIVER(s3c2440_mmc_drv) = {
	.name		= "s3c2440_mmc",
	.id			= UCLASS_MMC,
	.of_match	= s3c2440_mmc_ids,
	.bind		= s3c2440_mmc_bind,
	.probe		= s3c2440_mmc_probe,
	.ops		= &s3c2440_mmc_ops,
	.platdata_auto_alloc_size = sizeof(struct s3c2440_mmc_plat),
	.priv_auto_alloc_size = sizeof(struct s3c2440_mmc_priv),
};
