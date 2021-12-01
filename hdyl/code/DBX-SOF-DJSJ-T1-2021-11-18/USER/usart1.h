#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "misc.h"

// PAGE0  USER						0x44 0x12 0x07 0x19    D 2018-07-29
// PAGE1  UID
// PAGE2  password
// PAGE3  USER	
// PAGE4  CONFIG
// PAGE5  USER	
// PAGE6  USER	
// PAGE7  USER	
// PAGE8  USER	
// PAGE9  USER	
// PAGE10 USER	
// PAGE11 USER
// PAGE12 USER
// PAGE13 USER
// PAGE14 USER
// PAGE15 USER
/*****************************宏定义区********************************************/
#define	 USART1_TX_BUFFER				0xff												// 串口1发送缓存
#define	 USART1_RX_BUFFER				0xff												// 串口1接收缓存A
#define  USART_NUM1             1
#define  USART_NUM2             2
#define  USART_NUM3             3
#define  RF_Write_UserInfo      0x84												// 写卡数据信息
#define  RF_Read_UserInfo				0x85												// 读卡数据信息
#define  RF_Wake_UP							0x86												// 登陆
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

typedef enum
{
    RF_ENOERR,                  /*!< no error. */
		RF_CMDERR,									/*命令执行失败*/
		RF_DATANOERR=0x80,					/*标识参数设置成功*/
    RF_WRITEERR,                /*写卡失败*/
    RF_READERR,                 /*读卡失败*/
    RF_NOEXISTENT,              /*卡不存在*/
    RF_MISMATCH,                /*卡不匹配*/
    RF_PARERR,                  /*参数错误*/
    RF_ERR=0x87,               	/*输入参数错误 */
    RF_REGERR=0x8F,             /*输入指令不存在*/
		RF_IOERR,										/*io错误*/
}
rfErro_Code;
/**************************外部函数声明区******************************************/

void NVIC_Configuration(void);
void USART1_Config(void);																		// 串口1配置函数
uint8_t Usart1_Print(char *Data,...);												// 打印
uint8_t USART1_SendBuffer(uint8_t *buffer, uint16_t n);				// 通过串口1发送数据
uint8_t  USART1_SendDatas(uint16_t n);											// 发送数据
void Usart1_ISR(void);
rfErro_Code RF_Read_Page(uint8_t page);
rfErro_Code RF_Write_Page(uint8_t page,uint8_t *buffer);
rfErro_Code RF_Land(uint8_t page,uint8_t *buffer);
										//
#endif /* __USART1_H */

/******************** (C) COPYRIGHT 2014 盛元电子工作室 **************************/
