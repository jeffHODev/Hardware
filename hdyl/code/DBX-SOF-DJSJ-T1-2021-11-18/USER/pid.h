#ifndef __PID_H_
#define __PID_H_
#include "stm32f10x.h"

#define MAX_MOTOR_INIT_TIME 20											// �����ȫ��עˮʱ��

extern const uint16_t Set_Current_Table[];					// PHֵ��Ӧ�ĵ���ֵ����ѹ��أ�

void Start_Cur_PID(uint8_t temp);
void Cur_PID_Run(void);
void Cur_PID_Reset(void);

void Start_Motor_PID(uint8_t temp);
void Motor_PID_Run(void);
void Motor_PID_Reset(void);
void Reset_Motor_Value(void);

#endif

/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/
