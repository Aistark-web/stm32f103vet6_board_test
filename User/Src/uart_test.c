#include "uart_test.h"
#include <string.h>

#define UART_TEST_RX_SIZE  128

static uint8_t uart1_rx_buf[UART_TEST_RX_SIZE];
static uint8_t uart1_tx_buf[UART_TEST_RX_SIZE];

static uint8_t uart2_rx_buf[UART_TEST_RX_SIZE];
static uint8_t uart2_tx_buf[UART_TEST_RX_SIZE];

static uint8_t uart3_rx_buf[UART_TEST_RX_SIZE];
static uint8_t uart3_tx_buf[UART_TEST_RX_SIZE];

static uint8_t uart4_rx_buf[UART_TEST_RX_SIZE];
static uint8_t uart4_tx_buf[UART_TEST_RX_SIZE];

void UartTest_Init(void)
{
//	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart1_rx_buf, UART_TEST_RX_SIZE);			
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, uart2_rx_buf, UART_TEST_RX_SIZE);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, uart3_rx_buf, UART_TEST_RX_SIZE);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart4, uart4_rx_buf, UART_TEST_RX_SIZE);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	uint8_t *rx_buf = NULL;
	uint8_t *tx_buf = NULL;

	if(huart->Instance == USART1){
		rx_buf = uart1_rx_buf;
		tx_buf = uart1_tx_buf;
	}
	else if(huart->Instance == USART2) {
		rx_buf = uart2_rx_buf;
		tx_buf = uart2_tx_buf;
	}
	else if(huart->Instance == USART3) {
		rx_buf = uart3_rx_buf;
		tx_buf = uart3_tx_buf;
	}
	else if(huart->Instance == UART4) {
		rx_buf = uart4_rx_buf;
		tx_buf = uart4_tx_buf;
	}
	else
		return;

	switch (HAL_UARTEx_GetRxEventType(huart))
	{
		case HAL_UART_RXEVENT_IDLE:
		case HAL_UART_RXEVENT_TC:
		if (Size > 0U && huart->gState == HAL_UART_STATE_READY)
		{
			memcpy(tx_buf, rx_buf, Size);               // 拷出，避免发送途中被接收 DMA 覆盖
			HAL_UART_Transmit_DMA(huart, tx_buf, Size); // 非阻塞原样发回
		}
		HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_buf, UART_TEST_RX_SIZE); // 重新武装接收
		break;

		case HAL_UART_RXEVENT_HT:
		default:
				break;   // 半满不处理，等待 IDLE/TC
	}
}
