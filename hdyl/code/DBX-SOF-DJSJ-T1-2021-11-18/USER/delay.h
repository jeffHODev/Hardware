#ifndef __DELAY_H
#define	__DELAY_H

#include "stm32f10x.h"

#define	Cur_OUTPUT 			TIM4->CCR4				// 电解电压控制
#define PUM2L_OUTPUT		TIM4->CCR2				// 2L自吸泵
#define PUM1L_OUTPUT		TIM4->CCR1				// 1L自吸泵
#define PUM_MINVALUE2L  1400							// 2L最小占空比 未开电解时最小PWM  2019-04-10  1450

#define MUSIC_NUM1  5											// 最小值为2
#define MUSIC_NUM2  2											// 最小值为2
#define MUSIC_NUM3  5											// 最小值为2
#define MUSIC_NUM4  5											// 最小值为2
#define STANDBY_NUM	45										// 锁屏延时

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */

extern __IO uint8_t Second_Flage;					// 秒
extern __IO uint8_t Msecond_Flage;				// 毫秒
extern __IO uint8_t Curent_Flage;					// 统计流速标志
extern __IO uint8_t  Speak_Flage;					// 蜂鸣器      0 关闭  1 开机   2 按键音  3 结束音  4 报警音
extern const uint16_t Music_Code_1[MUSIC_NUM1];				// 
extern const uint16_t Music_Code_2[MUSIC_NUM2];				// 
extern const uint16_t Music_Code_3[MUSIC_NUM3];				// 
extern const uint16_t Music_Code_4[MUSIC_NUM4];				// 
extern __IO uint8_t  Standby_Time;				// 待机模式延时时间
extern __IO uint8_t  Init_Falge;					// 初始化标志
extern __IO uint8_t  Value_Delay_Time;		// 电磁阀关闭延时时间

void delay_ms(__IO uint32_t nCount);			// 短延时
void delay_us(__IO uint32_t nCount);			// 短延时

void SysTick_Init(void);									// 初始化嘀嗒定时器
void SysTick_ISR(void);										// 嘀嗒定时器中断处理函数
void TIM4_Configuration(void);						//
void TIM3_Configuration(void);						//
void TIM2_Configuration(void);
void TIM1_Configuration(void);
void Set_Music_Value(uint16_t value);			//
#endif /* DELAY THE END */

/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/
