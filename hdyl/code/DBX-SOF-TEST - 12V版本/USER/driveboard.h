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

// 继电器Y1	下垫
#define JDQ_YD1(a)	do{if (a)\
										GPIO_ResetBits(GPIOB,GPIO_Pin_13);\
										else\
										GPIO_SetBits(GPIOB,GPIO_Pin_13);}while(0)

// 继电器Y2	上垫
#define JDQ_YU2(a)	do{if (a)\
										GPIO_ResetBits(GPIOB,GPIO_Pin_15);\
										else\
										GPIO_SetBits(GPIOB,GPIO_Pin_15);}while(0)
													

void Board_GPIO_Config(void);					// 硬件配置 
void IWDG_Config(void);								// 硬件看门狗	
void NVIC_Configuration(void);				// 中断配置函数
uint8_t Init_Board(void);

										
#endif

/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/
