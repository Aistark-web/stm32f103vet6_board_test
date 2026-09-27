#include "usb_test.h"
#include "usbd_cdc_if.h"
#include <string.h>

#define USB_TEST_TX_SIZE  256

static uint8_t usb_tx_buf[USB_TEST_TX_SIZE];

/* 把收到的数据拷到独立发送缓冲后原样回传，返回 USBD_OK / USBD_BUSY */
uint8_t UsbTest_Echo(uint8_t *buf, uint16_t len)
{
	uint16_t n = (len > USB_TEST_TX_SIZE) ? USB_TEST_TX_SIZE : len;

	memcpy(usb_tx_buf, buf, n);
	return CDC_Transmit_FS(usb_tx_buf, n);
}
