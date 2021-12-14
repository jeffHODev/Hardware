#ifndef __PID_H_
#define __PID_H_
#include "stm32f10x.h"

#define MAX_MOTOR_INIT_TIME 20											// 主电机全速注水时间

extern const uint16_t Set_Current_Table[];					// PH值对应的电流值（电压相关）

void Start_Cur_PID(uint8_t temp);
void Cur_PID_Run(void);
void Cur_PID_Reset(void);

void Start_Motor_PID(uint8_t temp);
void Motor_PID_Run(void);
void Motor_PID_Reset(void);
void Reset_Motor_Value(void);

#endif

/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/
