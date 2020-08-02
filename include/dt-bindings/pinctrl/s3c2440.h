/*
 * This header provides constants for most S3C2440 pinctrl bindings.
 *
 * Copyright (C) 2013 Jean-Christophe PLAGNIOL-VILLARD <plagnioj@jcrosoft.com>
 *
 * GPLv2 only
 */

#ifndef __DT_BINDINGS_S3C2440_PINCTRL_H__
#define __DT_BINDINGS_S3C2440_PINCTRL_H__

#define S3C2440_GPIOA	0
#define S3C2440_GPIOB	1
#define S3C2440_GPIOC	2
#define S3C2440_GPIOD	3
#define S3C2440_GPIOE	4
#define S3C2440_GPIOF	5
#define S3C2440_GPIOG	6
#define S3C2440_GPIOH	7
#define S3C2440_GPIOJ	8

#define S3C2440_MUX_GPIO	        0
#define S3C2440_MUX_PERIPH_A		1
#define S3C2440_MUX_PERIPH_B		2

#define S3C2440_PINCTRL_NONE		0
#define S3C2440_PINCTRL_PULL_UP		1

#endif /* __DT_BINDINGS_S3C2440_PINCTRL_H__ */
