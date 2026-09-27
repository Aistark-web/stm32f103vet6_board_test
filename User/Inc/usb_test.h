#ifndef _USB_TEST_H_
#define _USB_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

uint8_t UsbTest_Echo(uint8_t *buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif
