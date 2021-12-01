#ifndef __USART2_H
#define	__USART2_H

#include "stm32f10x.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "misc.h"

/*****************************�궨����********************************************/
#define	 USART2_TX_BUFFER				0xff										// ����1���ͻ���
#define	 USART2_RX_BUFFER				0xff										// ����1���ջ���A

/**************************ȫ�ֱ���������*****************************************/
extern uint8_t Usart2_Tx_Buffer[USART2_TX_BUFFER];			// ����2���ͻ���
extern uint8_t Usart2_Rx_Buffer[USART2_RX_BUFFER];			// ����2���ջ���A
extern uint8_t Usart2_NewFax[USART2_RX_BUFFER];	 				// ����2��ȡһ֡�����ݻ���
extern uint8_t In_Usart2_RxBuf;									 				// ����2����ָ����						 
extern uint8_t Out_Usart2_RxBuf;									 			// ����2����ָ��� 
extern uint8_t Usart2_Receive_Flage;										// ����2�����±���	
extern uint8_t Usart2_RX_Overtime;											// ����2��ʱ������  
extern uint8_t Usart2_RX_State;													// ����2���ڹ�����־  =0 δ����  =1 ���ڹ���
extern uint8_t Usart2_TX_State;													// ����2���ڹ�����־  =0 δ����  =1 ���ڹ���
/**************************ȫ�ֺ���������*****************************************/
void USART2_Config(void);																// ����2���ú���
uint8_t  USART2_SendBuffer(char *buffer, uint16_t n);		//
uint8_t  USART2_SendDatas(uint16_t n);
void Usart2_ISR(void);

#endif /* __USART1_H */
