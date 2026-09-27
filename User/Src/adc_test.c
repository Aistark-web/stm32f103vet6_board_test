#include "adc_test.h"
#include "usart.h"
#include <stdio.h>

/* VREF+ 由板载 TL431 精密基准提供，实际电压请用万用表实测 VREF+ 引脚后修改此值 */
#define ADC_TEST_VREF_MV   3300U

/* DMA 写入目标：adc_dma_buf[0] = PA0(ADC1_IN0)，adc_dma_buf[1] = PA1(ADC1_IN1) */
static uint16_t adc_dma_buf[ADC_TEST_CH_NUM];

void AdcTest_Init(void)
{
	/* ADC1 已配置为 扫描 + 连续转换 + DMA 循环，启动后缓冲会一直自动刷新 */
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_dma_buf, ADC_TEST_CH_NUM);
}

uint16_t AdcTest_GetRaw(uint8_t ch)
{
	if (ch >= ADC_TEST_CH_NUM)
		return 0U;
	return adc_dma_buf[ch];
}

uint32_t AdcTest_GetMv(uint8_t ch)
{
	if (ch >= ADC_TEST_CH_NUM)
		return 0U;
	return ((uint32_t)adc_dma_buf[ch] * ADC_TEST_VREF_MV) / 4095U;
}

HAL_StatusTypeDef AdcTest_Print(uint8_t uart_index)
{
	char msg[64];
	int len;
	UART_HandleTypeDef *huart;

	switch (uart_index)
	{
		case 1:  huart = &huart1; break;
		case 2:  huart = &huart2; break;
		case 3:  huart = &huart3; break;
		case 4:  huart = &huart4; break;
		default: return HAL_ERROR;
	}

	if (huart->gState != HAL_UART_STATE_READY)   // TX 被占用（比如回显 DMA 正在发）
		return HAL_BUSY;                          // 本帧跳过，下轮再打

	len = snprintf(msg, sizeof(msg),
		"PA0=%4u (%4umV)  PA1=%4u (%4umV)\r\n",
		AdcTest_GetRaw(0), AdcTest_GetMv(0),
		AdcTest_GetRaw(1), AdcTest_GetMv(1));

	return HAL_UART_Transmit(huart, (uint8_t *)msg, (uint16_t)len, 100);
}
