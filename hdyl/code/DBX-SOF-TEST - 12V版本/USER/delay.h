#ifndef __DELAY_H
#define	__DELAY_H

#include "stm32f10x.h"

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */

extern uint8_t Second_Flage;
extern uint8_t Msecond_Flage;
extern uint8_t Msad_Flage;

void delay_ms(__IO uint32_t nCount);			// 短延时
void delay_us(__IO uint32_t nCount);			// 短延时

void SysTick_Init(void);									// 初始化嘀嗒定时器
void SysTick_ISR(void);										// 嘀嗒定时器中断处理函数

#endif /* DELAY THE END */

/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/
