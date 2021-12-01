#ifndef __USART3_H
#define	__USART3_H

#include "stm32f10x.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "misc.h"

/*****************************宏定义区********************************************/
#define	 USART3_TX_BUFFER				0xff												// 串口1发送缓存
#define	 USART3_RX_BUFFER				0xff												// 串口1接收缓存A

/*****************************全局变量声明区********************************************/
extern uint8_t Usart3_Tx_Buffer[USART3_TX_BUFFER];					// 串口3发送缓存
extern uint8_t Usart3_Rx_Buffer[USART3_RX_BUFFER];					// 串口3接收缓存A
extern uint8_t Usart3_NewFax[USART3_RX_BUFFER];							// 串口3获取一帧新数据缓存
extern uint8_t In_Usart3_RxBuf;															// 串口3接收指针入						 
extern uint8_t Out_Usart3_RxBuf;														// 串口3接收指针出 
extern uint8_t Usart3_Receive_Flage;												// 串口3发现新报文	
extern uint8_t Usart3_RX_Overtime;													// 串口3超时计数器  
extern uint8_t Usart3_RX_State;															// 串口3串口工作标志  =0 未工作  =1 正在工作
extern uint8_t Usart3_TX_State;															// 串口3串口工作标志  =0 未工作  =1 正在工作
/* RS485_TRXD		---	PB15				*/
#define RS485_TRXD(a) 	 do{if (a)\
														GPIO_SetBits(GPIOB,GPIO_Pin_15);\
														else\
														GPIO_ResetBits(GPIOB,GPIO_Pin_15);}while(0)


/*****************************外部函数声明区********************************************/
void USART3_Config(void);																// 串口2配置函数
uint8_t  USART3_SendBuffer(char *buffer, uint16_t n);		//
uint8_t USART3_SendDatas(uint16_t n);
void Usart3_ISR(void);
uint8_t Usart3_Print(char *Data,...);
#endif /* __USART1_H */
