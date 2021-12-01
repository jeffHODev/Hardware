#ifndef _TIMER_H
#define _TIMER_H
#include "main.h"



void TIM3_CH3_Cap_Init(uint32_t arr,unsigned int psc);
float *GetCapture(void);
void HAL_TIM_IC_CaptureCallback(void);
uint32_t *GetCapture_cnt(void);
#endif

