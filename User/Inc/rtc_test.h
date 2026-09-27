#ifndef _RTC_TEST_H_
#define _RTC_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rtc.h"

void RtcTest_Init(void);
HAL_StatusTypeDef RtcTest_Print(uint8_t uart_index);

#ifdef __cplusplus
}
#endif

#endif
