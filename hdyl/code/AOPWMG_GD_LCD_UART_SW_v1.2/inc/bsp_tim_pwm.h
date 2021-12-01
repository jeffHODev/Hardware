/*
*********************************************************************************************************
*
*	模块名称 : 利用STM32F4内部TIM输出PWM信号，顺便实现
*	文件名称 : bsp_tim_pwm.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2012-2013, 沈阳恒德医疗器械研发有限公司 ShenYang HengDe Medical Instruments Co.，Ltd.
*
*********************************************************************************************************
*/

#ifndef __BSP_TIM_PWM_H
#define __BSP_TIM_PWM_H

#include "main.h"

void bsp_SetTIMOutPWM( uint8_t _ucChannel,uint32_t _ulFreq, uint32_t _ulDutyCycle);
void bsp_SetTIMOutPWM_N( uint8_t _ucChannel,uint32_t _ulFreq, uint32_t _ulDutyCycle);
//void bsp_SetTIMforInt(TIM_TypeDef* TIMx, uint32_t _ulFreq, uint8_t _PreemptionPriority, uint8_t _SubPriority);

#endif

/***************************** 沈阳恒德医疗器械研发有限公司 ShenYang HengDe Medical Instruments Co.，Ltd. (END OF FILE) *********************************/
