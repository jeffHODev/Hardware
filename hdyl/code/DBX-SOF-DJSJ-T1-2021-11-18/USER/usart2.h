#ifndef __USART2_H
#define	__USART2_H

#include "stm32f10x.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "misc.h"

/*****************************宏定义区********************************************/
#define	 USART2_TX_BUFFER				0xff										// 串口1发送缓存
#define	 USART2_RX_BUFFER				0xff										// 串口1接收缓存A

/**************************全局变量声明区*****************************************/
extern uint8_t Usart2_Tx_Buffer[USART2_TX_BUFFER];			// 串口2发送缓存
extern uint8_t Usart2_Rx_Buffer[USART2_RX_BUFFER];			// 串口2接收缓存A
extern uint8_t Usart2_NewFax[USART2_RX_BUFFER];	 				// 串口2获取一帧新数据缓存
extern uint8_t In_Usart2_RxBuf;									 				// 串口2接收指针入						 
extern uint8_t Out_Usart2_RxBuf;									 			// 串口2接收指针出 
extern uint8_t Usart2_Receive_Flage;										// 串口2发现新报文	
extern uint8_t Usart2_RX_Overtime;											// 串口2超时计数器  
extern uint8_t Usart2_RX_State;													// 串口2串口工作标志  =0 未工作  =1 正在工作
extern uint8_t Usart2_TX_State;													// 串口2串口工作标志  =0 未工作  =1 正在工作
/**************************全局函数声明区*****************************************/
void USART2_Config(void);																// 串口2配置函数
uint8_t  USART2_SendBuffer(char *buffer, uint16_t n);		//
uint8_t  USART2_SendDatas(uint16_t n);
void Usart2_ISR(void);

#endif /* __USART1_H */
