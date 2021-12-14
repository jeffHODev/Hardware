/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define USART1_BUFF_SIZE 0xFF
#define USART3_BUFF_SIZE 0x1FF
#define USART_DMA_SENDOVER 0
#define USART_DMA_SENDING  1	 
	 
extern __IO uint8_t Usart1_Ready_Flage;							// 串口1收到数据标志
extern __IO uint8_t Usart3_Ready_Flage;							// 串口1收到数据标志
extern uint8_t Usart1_NewFax[USART1_BUFF_SIZE];			// 串口缓存区
extern uint8_t Usart3_NewFax[USART3_BUFF_SIZE];			// 串口缓存区
extern uint8_t Usart1_SendFax[USART1_BUFF_SIZE];		// 串口缓存区
extern uint8_t Usart3_SendFax[USART3_BUFF_SIZE];		// 串口缓存区
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void print(const char *format,...);
void Send_System_Info(void);
void UART_IDLE_Callback(UART_HandleTypeDef *huart);
uint8_t Uart1_SendData(UART_HandleTypeDef *huart,uint8_t *buf,uint16_t Size);
uint8_t Uart3_SendData(UART_HandleTypeDef *huart,uint8_t *buf,uint16_t Size);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
