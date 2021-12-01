#ifndef __USART3_H
#define	__USART3_H

#include "stm32f10x.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "misc.h"

/*****************************�궨����********************************************/
#define	 USART3_TX_BUFFER				0xff												// ����1���ͻ���
#define	 USART3_RX_BUFFER				0xff												// ����1���ջ���A

/*****************************ȫ�ֱ���������********************************************/
extern uint8_t Usart3_Tx_Buffer[USART3_TX_BUFFER];					// ����3���ͻ���
extern uint8_t Usart3_Rx_Buffer[USART3_RX_BUFFER];					// ����3���ջ���A
extern uint8_t Usart3_NewFax[USART3_RX_BUFFER];							// ����3��ȡһ֡�����ݻ���
extern uint8_t In_Usart3_RxBuf;															// ����3����ָ����						 
extern uint8_t Out_Usart3_RxBuf;														// ����3����ָ��� 
extern uint8_t Usart3_Receive_Flage;												// ����3�����±���	
extern uint8_t Usart3_RX_Overtime;													// ����3��ʱ������  
extern uint8_t Usart3_RX_State;															// ����3���ڹ�����־  =0 δ����  =1 ���ڹ���
extern uint8_t Usart3_TX_State;															// ����3���ڹ�����־  =0 δ����  =1 ���ڹ���
/* RS485_TRXD		---	PB15				*/
#define RS485_TRXD(a) 	 do{if (a)\
														GPIO_SetBits(GPIOB,GPIO_Pin_15);\
														else\
														GPIO_ResetBits(GPIOB,GPIO_Pin_15);}while(0)


/*****************************�ⲿ����������********************************************/
void USART3_Config(void);																// ����2���ú���
uint8_t  USART3_SendBuffer(char *buffer, uint16_t n);		//
uint8_t USART3_SendDatas(uint16_t n);
void Usart3_ISR(void);
uint8_t Usart3_Print(char *Data,...);
#endif /* __USART1_H */
