#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "misc.h"

/*****************************宏定义区********************************************/
#define	 USART1_TX_BUFFER				0xff												// 串口1发送缓存
#define	 USART1_RX_BUFFER				0xff												// 串口1接收缓存A
#define  USART_NUM1             1
#define  USART_NUM2             2
#define  USART_NUM3             3
/**************************全局变量声明区******************************************/
extern uint8_t Usart1_Tx_Buffer[USART1_TX_BUFFER];					// 串口1发送缓存
extern uint8_t Usart1_Rx_Buffer[USART1_RX_BUFFER];					// 串口1接收缓存A
extern uint8_t Usart1_NewFax[USART1_RX_BUFFER];							// 获取一帧新数据
extern uint8_t In_Usart1_RxBuf;															// 串口1接收指针入						 
extern uint8_t Out_Usart1_RxBuf;														// 串口1接收指针出 
extern uint8_t Usart1_Receive_Flage;												// 发现新报文	
extern uint8_t Usart1_RX_Overtime;													// 串口1超时计数器  
extern uint8_t Usart1_TX_State;															// 串口1串口工作标志  =0 未工作  =1 正在工作	
extern uint8_t Usart1_RX_State;															// 串口1串口工作标志  =0 未工作  =1 正在工作
/**************************外部函数声明区******************************************/

#define TRXD_SET(a)	do{if (a)\
										GPIO_SetBits(GPIOA,GPIO_Pin_8);\
										else\
										GPIO_ResetBits(GPIOA,GPIO_Pin_8);}while(0)

void NVIC_Configuration(void);
void USART1_Config(void);																		// 串口1配置函数
uint8_t Usart1_Print(char *Data,...);												// 打印
uint8_t USART1_SendBuffer(char *buffer, uint16_t n);				// 通过串口1发送数据
uint8_t  USART1_SendDatas(uint16_t n);											// 发送数据
void Usart1_ISR(void);
										//
#endif /* __USART1_H */

/******************** (C) COPYRIGHT 2014 盛元电子工作室 **************************/
