#ifndef __DRIVEBOARD_H_
#define __DRIVEBOARD_H_
#include "stm32f10x.h"

#define USART_1  1
#define USART_2  2
#define USART_3  3										// 串口3
#define DEBUGE   0										// 调试模式   ==0  工作模式
#define ON       1
#define OFF      0

/* 硬件连接设置	*/
// 硬件连接

#define ELE_Read    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)	// 当该引脚被驱动后开始电解

#define VALUE_Out(a)do{if (a)\
										GPIO_SetBits(GPIOB,GPIO_Pin_5);\
										else\
										GPIO_ResetBits(GPIOB,GPIO_Pin_5);}while(0)	

#define LED_Pow(a)	do{if (a)\
										GPIO_SetBits(GPIOB,GPIO_Pin_12);\
										else\
										GPIO_ResetBits(GPIOB,GPIO_Pin_12);}while(0)		

// 控制电解槽正极 默认链接到正极  驱动后链接到负极
#define ELE_ConP(a)	do{if (a)\
										GPIO_SetBits(GPIOC,GPIO_Pin_13);\
										else\
										GPIO_ResetBits(GPIOC,GPIO_Pin_13);}while(0)		

// 控制电解槽负极 默认链接到正极  驱动后链接到负极
#define ELE_ConN(a)	do{if (a)\
										GPIO_SetBits(GPIOC,GPIO_Pin_14);\
										else\
										GPIO_ResetBits(GPIOC,GPIO_Pin_14);}while(0)												
// 继电器Y1	PC15
#define JDQ_Heat(a)	do{if (a)\
										GPIO_SetBits(GPIOC,GPIO_Pin_15);\
										else\
										GPIO_ResetBits(GPIOC,GPIO_Pin_15);}while(0)
// TDS-P
#define TDS_P(a)		do{if (a)\
										GPIO_SetBits(GPIOA,GPIO_Pin_8);\
										else\
										GPIO_ResetBits(GPIOA,GPIO_Pin_8);}while(0)
// TDS-N
#define TDS_N(a)		do{if (a)\
										GPIO_SetBits(GPIOB,GPIO_Pin_13);\
										else\
										GPIO_ResetBits(GPIOB,GPIO_Pin_13);}while(0)
		

#define SENSER_YSX GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)			 // 原水箱水位			
#define SENSER_SSX GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)			 // 酸水箱水位												
#define SENSER_SXG GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)			 // 水箱盖
	
													
void Board_GPIO_Config(void);					// 硬件配置 
void IWDG_Config(void);								// 硬件看门狗	
void NVIC_Configuration(void);				// 中断配置函数
uint8_t Init_Board(void);

										
#endif

/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/
