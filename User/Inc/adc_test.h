#ifndef _ADC_TEST_H_
#define _ADC_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "adc.h"

#define ADC_TEST_CH_NUM   2U

void AdcTest_Init(void);
uint16_t AdcTest_GetRaw(uint8_t ch);
uint32_t AdcTest_GetMv(uint8_t ch);
HAL_StatusTypeDef AdcTest_Print(uint8_t uart_index);

#ifdef __cplusplus
}
#endif

#endif