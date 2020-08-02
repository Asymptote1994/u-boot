// SPDX-License-Identifier: GPL-2.0+
/*
 * linux/drivers/usb/gadget/s3c2410_udc.h
 * Samsung on-chip full speed USB device controllers
 *
 * Copyright (C) 2004-2007 Herbert Pötzl - Arnaud Patard
 *	Additional cleanups by Ben Dooks <ben-linux@fluff.org>
 */

#ifndef _S3C2410_UDC_H
#define _S3C2410_UDC_H

/* USB Device port */
#define S3C2410_PA_USBDEV	(0x52000000)
#define S3C24XX_SZ_USBDEV	SZ_1M

#define S3C2410_USBDREG(x) (x)

#define S3C2410_UDC_FUNC_ADDR_REG	S3C2410_USBDREG(0x0140)
#define S3C2410_UDC_PWR_REG		S3C2410_USBDREG(0x0144)
#define S3C2410_UDC_EP_INT_REG		S3C2410_USBDREG(0x0148)

#define S3C2410_UDC_USB_INT_REG		S3C2410_USBDREG(0x0158)
#define S3C2410_UDC_EP_INT_EN_REG	S3C2410_USBDREG(0x015c)

#define S3C2410_UDC_USB_INT_EN_REG	S3C2410_USBDREG(0x016c)

#define S3C2410_UDC_FRAME_NUM1_REG	S3C2410_USBDREG(0x0170)
#define S3C2410_UDC_FRAME_NUM2_REG	S3C2410_USBDREG(0x0174)

#define S3C2410_UDC_EP0_FIFO_REG	S3C2410_USBDREG(0x01c0)
#define S3C2410_UDC_EP1_FIFO_REG	S3C2410_USBDREG(0x01c4)
#define S3C2410_UDC_EP2_FIFO_REG	S3C2410_USBDREG(0x01c8)
#define S3C2410_UDC_EP3_FIFO_REG	S3C2410_USBDREG(0x01cc)
#define S3C2410_UDC_EP4_FIFO_REG	S3C2410_USBDREG(0x01d0)

#define S3C2410_UDC_EP1_DMA_CON		S3C2410_USBDREG(0x0200)
#define S3C2410_UDC_EP1_DMA_UNIT	S3C2410_USBDREG(0x0204)
#define S3C2410_UDC_EP1_DMA_FIFO	S3C2410_USBDREG(0x0208)
#define S3C2410_UDC_EP1_DMA_TTC_L	S3C2410_USBDREG(0x020c)
#define S3C2410_UDC_EP1_DMA_TTC_M	S3C2410_USBDREG(0x0210)
#define S3C2410_UDC_EP1_DMA_TTC_H	S3C2410_USBDREG(0x0214)

#define S3C2410_UDC_EP2_DMA_CON		S3C2410_USBDREG(0x0218)
#define S3C2410_UDC_EP2_DMA_UNIT	S3C2410_USBDREG(0x021c)
#define S3C2410_UDC_EP2_DMA_FIFO	S3C2410_USBDREG(0x0220)
#define S3C2410_UDC_EP2_DMA_TTC_L	S3C2410_USBDREG(0x0224)
#define S3C2410_UDC_EP2_DMA_TTC_M	S3C2410_USBDREG(0x0228)
#define S3C2410_UDC_EP2_DMA_TTC_H	S3C2410_USBDREG(0x022c)

#define S3C2410_UDC_EP3_DMA_CON		S3C2410_USBDREG(0x0240)
#define S3C2410_UDC_EP3_DMA_UNIT	S3C2410_USBDREG(0x0244)
#define S3C2410_UDC_EP3_DMA_FIFO	S3C2410_USBDREG(0x0248)
#define S3C2410_UDC_EP3_DMA_TTC_L	S3C2410_USBDREG(0x024c)
#define S3C2410_UDC_EP3_DMA_TTC_M	S3C2410_USBDREG(0x0250)
#define S3C2410_UDC_EP3_DMA_TTC_H	S3C2410_USBDREG(0x0254)

#define S3C2410_UDC_EP4_DMA_CON		S3C2410_USBDREG(0x0258)
#define S3C2410_UDC_EP4_DMA_UNIT	S3C2410_USBDREG(0x025c)
#define S3C2410_UDC_EP4_DMA_FIFO	S3C2410_USBDREG(0x0260)
#define S3C2410_UDC_EP4_DMA_TTC_L	S3C2410_USBDREG(0x0264)
#define S3C2410_UDC_EP4_DMA_TTC_M	S3C2410_USBDREG(0x0268)
#define S3C2410_UDC_EP4_DMA_TTC_H	S3C2410_USBDREG(0x026c)

#define S3C2410_UDC_INDEX_REG		S3C2410_USBDREG(0x0178)

/* indexed registers */

#define S3C2410_UDC_MAXP_REG		S3C2410_USBDREG(0x0180)

#define S3C2410_UDC_EP0_CSR_REG		S3C2410_USBDREG(0x0184)

#define S3C2410_UDC_IN_CSR1_REG		S3C2410_USBDREG(0x0184)
#define S3C2410_UDC_IN_CSR2_REG		S3C2410_USBDREG(0x0188)

#define S3C2410_UDC_OUT_CSR1_REG	S3C2410_USBDREG(0x0190)
#define S3C2410_UDC_OUT_CSR2_REG	S3C2410_USBDREG(0x0194)
#define S3C2410_UDC_OUT_FIFO_CNT1_REG	S3C2410_USBDREG(0x0198)
#define S3C2410_UDC_OUT_FIFO_CNT2_REG	S3C2410_USBDREG(0x019c)

#define S3C2410_UDC_FUNCADDR_UPDATE	(1 << 7)

#define S3C2410_UDC_PWR_ISOUP		(1 << 7) /* R/W */
#define S3C2410_UDC_PWR_RESET		(1 << 3) /* R   */
#define S3C2410_UDC_PWR_RESUME		(1 << 2) /* R/W */
#define S3C2410_UDC_PWR_SUSPEND		(1 << 1) /* R   */
#define S3C2410_UDC_PWR_ENSUSPEND	(1 << 0) /* R/W */

#define S3C2410_UDC_PWR_DEFAULT		(0x00)

#define S3C2410_UDC_INT_EP4		(1 << 4) /* R/W (clear only) */
#define S3C2410_UDC_INT_EP3		(1 << 3) /* R/W (clear only) */
#define S3C2410_UDC_INT_EP2		(1 << 2) /* R/W (clear only) */
#define S3C2410_UDC_INT_EP1		(1 << 1) /* R/W (clear only) */
#define S3C2410_UDC_INT_EP0		(1 << 0) /* R/W (clear only) */

#define S3C2410_UDC_USBINT_RESET	(1 << 2) /* R/W (clear only) */
#define S3C2410_UDC_USBINT_RESUME	(1 << 1) /* R/W (clear only) */
#define S3C2410_UDC_USBINT_SUSPEND	(1 << 0) /* R/W (clear only) */

#define S3C2410_UDC_INTE_EP4		(1 << 4) /* R/W */
#define S3C2410_UDC_INTE_EP3		(1 << 3) /* R/W */
#define S3C2410_UDC_INTE_EP2		(1 << 2) /* R/W */
#define S3C2410_UDC_INTE_EP1		(1 << 1) /* R/W */
#define S3C2410_UDC_INTE_EP0		(1 << 0) /* R/W */

#define S3C2410_UDC_USBINTE_RESET	(1 << 2) /* R/W */
#define S3C2410_UDC_USBINTE_SUSPEND	(1 << 0) /* R/W */

#define S3C2410_UDC_INDEX_EP0		(0x00)
#define S3C2410_UDC_INDEX_EP1		(0x01)
#define S3C2410_UDC_INDEX_EP2		(0x02)
#define S3C2410_UDC_INDEX_EP3		(0x03)
#define S3C2410_UDC_INDEX_EP4		(0x04)

#define S3C2410_UDC_ICSR1_CLRDT		(1 << 6) /* R/W */
#define S3C2410_UDC_ICSR1_SENTSTL	(1 << 5) /* R/W (clear only) */
#define S3C2410_UDC_ICSR1_SENDSTL	(1 << 4) /* R/W */
#define S3C2410_UDC_ICSR1_FFLUSH	(1 << 3) /* W   (set only) */
#define S3C2410_UDC_ICSR1_UNDRUN	(1 << 2) /* R/W (clear only) */
#define S3C2410_UDC_ICSR1_PKTRDY	(1 << 0) /* R/W (set only) */

#define S3C2410_UDC_ICSR2_AUTOSET	(1 << 7) /* R/W */
#define S3C2410_UDC_ICSR2_ISO		(1 << 6) /* R/W */
#define S3C2410_UDC_ICSR2_MODEIN	(1 << 5) /* R/W */
#define S3C2410_UDC_ICSR2_DMAIEN	(1 << 4) /* R/W */

#define S3C2410_UDC_OCSR1_CLRDT		(1 << 7) /* R/W */
#define S3C2410_UDC_OCSR1_SENTSTL	(1 << 6) /* R/W (clear only) */
#define S3C2410_UDC_OCSR1_SENDSTL	(1 << 5) /* R/W */
#define S3C2410_UDC_OCSR1_FFLUSH	(1 << 4) /* R/W */
#define S3C2410_UDC_OCSR1_DERROR	(1 << 3) /* R   */
#define S3C2410_UDC_OCSR1_OVRRUN	(1 << 2) /* R/W (clear only) */
#define S3C2410_UDC_OCSR1_PKTRDY	(1 << 0) /* R/W (clear only) */

#define S3C2410_UDC_OCSR2_AUTOCLR	(1 << 7) /* R/W */
#define S3C2410_UDC_OCSR2_ISO		(1 << 6) /* R/W */
#define S3C2410_UDC_OCSR2_DMAIEN	(1 << 5) /* R/W */

#define S3C2410_UDC_EP0_CSR_OPKRDY	(1 << 0)
#define S3C2410_UDC_EP0_CSR_IPKRDY	(1 << 1)
#define S3C2410_UDC_EP0_CSR_SENTSTL	(1 << 2)
#define S3C2410_UDC_EP0_CSR_DE		(1 << 3)
#define S3C2410_UDC_EP0_CSR_SE		(1 << 4)
#define S3C2410_UDC_EP0_CSR_SENDSTL	(1 << 5)
#define S3C2410_UDC_EP0_CSR_SOPKTRDY	(1 << 6)
#define S3C2410_UDC_EP0_CSR_SSE		(1 << 7)

#define S3C2410_UDC_MAXP_8		(1 << 0)
#define S3C2410_UDC_MAXP_16		(1 << 1)
#define S3C2410_UDC_MAXP_32		(1 << 2)
#define S3C2410_UDC_MAXP_64		(1 << 3)

#define USB_EP_CAPS_TYPE_CONTROL     0x01
#define USB_EP_CAPS_TYPE_ISO         0x02
#define USB_EP_CAPS_TYPE_BULK        0x04
#define USB_EP_CAPS_TYPE_INT         0x08
#define USB_EP_CAPS_TYPE_ALL \
	(USB_EP_CAPS_TYPE_ISO | USB_EP_CAPS_TYPE_BULK | USB_EP_CAPS_TYPE_INT)
#define USB_EP_CAPS_DIR_IN           0x01
#define USB_EP_CAPS_DIR_OUT          0x02
#define USB_EP_CAPS_DIR_ALL  (USB_EP_CAPS_DIR_IN | USB_EP_CAPS_DIR_OUT)

#define USB_EP_CAPS(_type, _dir) \
	{ \
		.type_control = !!(_type & USB_EP_CAPS_TYPE_CONTROL), \
		.type_iso = !!(_type & USB_EP_CAPS_TYPE_ISO), \
		.type_bulk = !!(_type & USB_EP_CAPS_TYPE_BULK), \
		.type_int = !!(_type & USB_EP_CAPS_TYPE_INT), \
		.dir_in = !!(_dir & USB_EP_CAPS_DIR_IN), \
		.dir_out = !!(_dir & USB_EP_CAPS_DIR_OUT), \
	}

enum s3c2410_udc_cmd_e {
	S3C2410_UDC_P_ENABLE	= 1,	/* Pull-up enable        */
	S3C2410_UDC_P_DISABLE	= 2,	/* Pull-up disable       */
	S3C2410_UDC_P_RESET	= 3,	/* UDC reset, in case of */
};

struct s3c2410_ep {
	struct list_head		queue;
	unsigned long			last_io;	/* jiffies timestamp */
	struct usb_gadget		*gadget;
	struct s3c2410_udc		*dev;
	struct usb_ep			ep;
	u8				num;

	unsigned short			fifo_size;
	u8				bEndpointAddress;
	u8				bmAttributes;

	unsigned			halted : 1;
	unsigned			already_seen : 1;
	unsigned			setup_stage : 1;
};


/* Warning : ep0 has a fifo of 16 bytes */
/* Don't try to set 32 or 64            */
/* also testusb 14 fails  wit 16 but is */
/* fine with 8                          */
#define EP0_FIFO_SIZE		 8
#define EP_FIFO_SIZE		64
#define DEFAULT_POWER_STATE	0x00

#define S3C2440_EP_FIFO_SIZE	128

static const char ep0name [] = "ep0";

static const char *const ep_name[] = {
	ep0name,                                /* everyone has ep0 */
	/* s3c2410 four bidirectional bulk endpoints */
	"ep1-bulk", "ep2-bulk", "ep3-bulk", "ep4-bulk",
};

#define S3C2410_ENDPOINTS       ARRAY_SIZE(ep_name)

struct s3c2410_request {
	struct list_head		queue;		/* ep's requests */
	struct usb_request		req;
};

enum ep0_state {
        EP0_IDLE,
        EP0_IN_DATA_PHASE,
        EP0_OUT_DATA_PHASE,
        EP0_END_XFER,
        EP0_STALL,
};

static const char *ep0states[]= {
        "EP0_IDLE",
        "EP0_IN_DATA_PHASE",
        "EP0_OUT_DATA_PHASE",
        "EP0_END_XFER",
        "EP0_STALL",
};

struct s3c2410_udc {
	spinlock_t			lock;

	struct s3c2410_ep		ep[S3C2410_ENDPOINTS];
	int				address;
	struct usb_gadget		gadget;
	struct usb_gadget_driver	*driver;
	struct s3c2410_request		fifo_req;
	u8				fifo_buf[EP_FIFO_SIZE];
	u16				devstatus;

	u32				port_status;
	int				ep0state;

	unsigned			got_irq : 1;

	unsigned			req_std : 1;
	unsigned			req_config : 1;
	unsigned			req_pending : 1;
	u8				vbus;
	struct dentry			*regs_info;
};
#define to_s3c2410(g)	(container_of((g), struct s3c2410_udc, gadget))

#endif
