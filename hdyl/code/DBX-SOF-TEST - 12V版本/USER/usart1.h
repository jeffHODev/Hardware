#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "misc.h"

/*****************************�궨����********************************************/
#define	 USART1_TX_BUFFER				0xff												// ����1���ͻ���
#define	 USART1_RX_BUFFER				0xff												// ����1���ջ���A
#define  USART_NUM1             1
#define  USART_NUM2             2
#define  USART_NUM3             3
/**************************ȫ�ֱ���������******************************************/
extern uint8_t Usart1_Tx_Buffer[USART1_TX_BUFFER];					// ����1���ͻ���
extern uint8_t Usart1_Rx_Buffer[USART1_RX_BUFFER];					// ����1���ջ���A
extern uint8_t Usart1_NewFax[USART1_RX_BUFFER];							// ��ȡһ֡������
extern uint8_t In_Usart1_RxBuf;															// ����1����ָ����						 
extern uint8_t Out_Usart1_RxBuf;														// ����1����ָ��� 
extern uint8_t Usart1_Receive_Flage;												// �����±���	
extern uint8_t Usart1_RX_Overtime;													// ����1��ʱ������  
extern uint8_t Usart1_TX_State;															// ����1���ڹ�����־  =0 δ����  =1 ���ڹ���	
extern uint8_t Usart1_RX_State;															// ����1���ڹ�����־  =0 δ����  =1 ���ڹ���
/**************************�ⲿ����������******************************************/

#define TRXD_SET(a)	do{if (a)\
										GPIO_SetBits(GPIOA,GPIO_Pin_8);\
										else\
										GPIO_ResetBits(GPIOA,GPIO_Pin_8);}while(0)

void NVIC_Configuration(void);
void USART1_Config(void);																		// ����1���ú���
uint8_t Usart1_Print(char *Data,...);												// ��ӡ
uint8_t USART1_SendBuffer(char *buffer, uint16_t n);				// ͨ������1��������
uint8_t  USART1_SendDatas(uint16_t n);											// ��������
void Usart1_ISR(void);
										//
#endif /* __USART1_H */

/******************** (C) COPYRIGHT 2014 ʢԪ���ӹ����� **************************/
