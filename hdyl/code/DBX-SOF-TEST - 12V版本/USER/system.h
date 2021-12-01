#ifndef __SYSTEM_H_
#define __SYSTEM_H_
#include "stm32f10x.h"

#define Ntc_Num 126													// �¶ȴ��������·�Χ
#define AVER_SAMPLE_NUM 100									// ƽ������
#define INPUT_FILTER_TIME 10								// �����˲�ʱ��50ms, ��λ10ms 	ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
#define INPUT_LONG_TIME   100								// ����1000ms����Ϊ�����¼� 
#define TEMP_DIFF 5													// �¶�ƫ��

#define DRPOWER_DIFFD 11										// ����ƫ������
#define DRPOWER_DIFFU 11											// ����ƫ������
#define DRSTAND_POWER 110										// ��׼����

#define SRPOWER_DIFFD 11										// ����ƫ������
#define SRPOWER_DIFFU 11											// ����ƫ������
#define SRSTAND_POWER 120										// ��׼����

#define VDD_VALUE 5.0												// ��Դ��ѹ
#define UP_VALUE  2.5												// �ϵ��޸���ֵ
#define DOW_VALUE 2.5												// �µ��޸���ֵ

extern __IO uint8_t Work_Mode;							// ����ģʽ
extern __IO uint8_t Chuang_Mode;						// ������� ���� ˫��
extern __IO float UP_Temp;									// �ϵ��¶�
extern __IO float Down_Temp;								// �µ��¶�
extern __IO float Now_Temp;									// ��ǰ�¶�
extern __IO float IN_Value;									// �����ѹ
extern __IO float UP_Power;									// �ϵ湦��
extern __IO float Down_Power;								// �µ湦��

uint8_t SW_Posmode_Read(void);
void Postion_GPIO_Config(void);
void Get_Now_Temp(void);
void Get_Now_Value(void);

#endif

/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/
