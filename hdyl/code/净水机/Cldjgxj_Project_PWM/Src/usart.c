/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "feeprom.h"
#include "system.h"
#include "filter.h"
#include "adc.h"

__IO uint8_t Usart1_Ready_Flage=0;					// ����1�յ����ݱ�־
__IO uint8_t Usart3_Ready_Flage=0;					// ����1�յ����ݱ�־
__IO uint8_t Usart1_Send_Flage=0;						// ����1���ڷ�������
__IO uint8_t Usart3_Send_Flage=0;						// ����1���ڷ�������
uint16_t Usart1_Rx_Size=0;									// ���ڽ�������
uint16_t Usart3_Rx_Size=0;									// ���ڽ�������
uint8_t Usart1_NewFax[USART1_BUFF_SIZE];		// ���ڻ�����
uint8_t Usart1_SendFax[USART1_BUFF_SIZE];		// ���ڻ�����
uint8_t Usart3_NewFax[USART3_BUFF_SIZE];		// ���ڻ�����
uint8_t Usart3_SendFax[USART3_BUFF_SIZE];		// ���ڻ�����

static uint8_t Usart1_Rxbuf[USART1_BUFF_SIZE];			// ���ڻ�����
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart1,Usart1_Rxbuf,USART1_BUFF_SIZE);	
	Usart1_Send_Flage=USART_DMA_SENDOVER;
  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 DMA Init */
    /* USART3_RX Init */
    hdma_usart3_rx.Instance = DMA1_Channel3;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_NORMAL;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);

    /* USART3_TX Init */
    hdma_usart3_tx.Instance = DMA1_Channel2;
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart3_tx);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */
	Usart3_Send_Flage=USART_DMA_SENDOVER;
  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void print(const char *format,...)
{
	uint16_t ulen;
	uint8_t temp;
	
	va_list arg;
	va_start(arg,format);
	ulen = vsnprintf((char*)Usart3_SendFax,sizeof(Usart3_SendFax),(char*)format,arg);
	va_end(arg);
	// ���ڷ�������
	temp=Uart3_SendData(&huart3,Usart3_SendFax,ulen);
	if(temp!=3)				// ���ڷ����쳣���ʼ������
	{
		HAL_UART_DeInit(&huart3);
		MX_USART3_UART_Init();		
	}	
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)  
{  
     __HAL_DMA_DISABLE(huart->hdmatx); 
		if(	huart->Instance == huart1.Instance) Usart1_Ready_Flage=USART_DMA_SENDOVER;
		if(	huart->Instance == huart3.Instance) Usart3_Ready_Flage=USART_DMA_SENDOVER;
}

/***** UART_IDLE_Callback ****/
void UART_IDLE_Callback(UART_HandleTypeDef *huart)
{
	uint8_t flage;
	uint8_t damrxsize=0;

	flage = __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
	
	if(flage!=RESET)												// ��־����λ
	{
		__HAL_UART_CLEAR_IDLEFLAG(huart);			// ����жϱ�־λ
		// ���жϺ�����رշ����ж� ��˲���������ʹ��
		//HAL_UART_DMAStop(huart); 						// ֹͣMDA�����ж�
		//HAL_UART_AbortReceive(huart);				// ����DMA�����ж�
		HAL_UART_Abort(huart);								// ��������
		if(huart->Instance == USART1)					// �����жϵ���USART1
		{
			damrxsize = USART1_BUFF_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);	// 
		}
		if(damrxsize != 0)										//
		{
			memcpy(Usart1_NewFax,Usart1_Rxbuf,damrxsize);
			Usart1_Rx_Size = damrxsize;					// 
			Usart1_Ready_Flage=1;								// �յ�һ֡����
		}
		/************��ֹDMA���������д����յ�����**************/
		__HAL_UART_CLEAR_IDLEFLAG(huart);			// ����жϱ�־λ
		HAL_UART_Receive_DMA(&huart1,Usart1_Rxbuf,USART1_BUFF_SIZE);
	}
}

uint8_t Uart1_SendData(UART_HandleTypeDef *huart,uint8_t *buf,uint16_t length)
{
	if(length == 0 )return 0;
	if(length>USART1_BUFF_SIZE) length=USART1_BUFF_SIZE;
	memcpy(Usart1_SendFax,buf,length);
	if(Usart1_Ready_Flage!=USART_DMA_SENDING)
	{
		Usart1_Send_Flage=USART_DMA_SENDING;
		HAL_UART_Transmit_DMA(&huart1,Usart1_SendFax,length);
		return 1;
	}
	
	return 0;
}

uint8_t Uart3_SendData(UART_HandleTypeDef *huart,uint8_t *buf,uint16_t length)
{
	if(length == 0 )return 0;
	if(length>USART3_BUFF_SIZE) length=USART3_BUFF_SIZE;
	memcpy(Usart3_SendFax,buf,length);	
	if(Usart3_Ready_Flage!=USART_DMA_SENDING)
	{
		Usart3_Send_Flage=USART_DMA_SENDING;
		HAL_UART_Transmit_DMA(&huart3,Usart3_SendFax,length);
		return 3;
	}	
	
	return 0;
}


/*****Send_Work infomation*****/
void Send_System_Info(void)
{
	// ���͹�������
	print("**********ϵͳ����**********/\r\n"
				"����ʱ��:%d:%d:%d ��Դ��ѹ:%2.3fV\r\n"
				"����ѹ:%2.3fV������:%1.3fA Ԥ��ֵ:%d\r\n"
				"������Ƶ��:%3.1fHz �������:%1.3fL/m\r\n"
				"TDSת��ֵ:%d TDSֵ:%3.3f\r\n"		
				"��ǰ����:%3.3fL �ۼ�����:%5.2f\r\n"
				"��о����:PP:%d C1:%d UF:%d C2:%d\r\n"
				"��ϴ״̬:%d �Ӻ�:%d�� ����:%d�� ��ϴ:%d�� �ۼ�:%3.2fL ��ֵ:%dL �ܼ�:%d��\r\n"
				,Hour,Minute,Second,SYS_Value
				,ELE_Value,ELE_Current,TLP0501_Value
				,Flow_Now_Fre,Flow_Now_Value
				,TDS_ADC_Value[0],TDS_Value
				,Last_Float_Flow,Toatl_Float_Flow
				,PP_Filter_Life,C1_Filter_Life,UDF_Filter_Life,C2_Filter_Life
				,Fanxi_Mode,Fanxi_Delay_Time,Fanxi_Continue_Time,Chongxi_Continue_Time,Fanxi_Float_Flow,Auto_Fanxi_Value,Fanxi_Num
				);
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
