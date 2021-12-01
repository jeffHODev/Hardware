#include "uart.h"
extern UART_HandleTypeDef huart6;
unsigned char uart6_rxbuf[BUFFER_SIZE];
void uartInit()
{
	HAL_UART_Receive_DMA(&huart6, uart6_rxbuf,BUFFER_SIZE);
}
