#ifndef __DELAY_H
#define	__DELAY_H

#include "stm32f10x.h"

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */

extern uint8_t Second_Flage;
extern uint8_t Msecond_Flage;
extern uint8_t Msad_Flage;

void delay_ms(__IO uint32_t nCount);			// ����ʱ
void delay_us(__IO uint32_t nCount);			// ����ʱ

void SysTick_Init(void);									// ��ʼ����શ�ʱ��
void SysTick_ISR(void);										// ��શ�ʱ���жϴ�����

#endif /* DELAY THE END */

/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/
