#include "rtc_test.h"
#include "usart.h"
#include <stdio.h>

void RtcTest_Init(void)
{
	/* 若启用掉电保持（见 rtc.c 的 Check_RTC_BKUP 改动），此处无需再初始化时间 */
}

HAL_StatusTypeDef RtcTest_Print(uint8_t uart_index)
{
	char msg[64];
	int len;
	UART_HandleTypeDef *huart;
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	switch (uart_index)
	{
		case 1:  huart = &huart1; break;
		case 2:  huart = &huart2; break;
		case 3:  huart = &huart3; break;
		case 4:  huart = &huart4; break;
		default: return HAL_ERROR;
	}

	if (huart->gState != HAL_UART_STATE_READY)
		return HAL_BUSY;

	/* 顺序必须是先 GetTime 再 GetDate：读日期会解锁影子寄存器，否则时间不刷新 */
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	len = snprintf(msg, sizeof(msg),
		"20%02u-%02u-%02u %02u:%02u:%02u W%u\r\n",
		date.Year, date.Month, date.Date,
		time.Hours, time.Minutes, time.Seconds, date.WeekDay);

	return HAL_UART_Transmit(huart, (uint8_t *)msg, (uint16_t)len, 100);
}
