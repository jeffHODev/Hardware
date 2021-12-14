#ifndef __DELAY_H
#define	__DELAY_H

#include "stm32f10x.h"

#define	Cur_OUTPUT 			TIM4->CCR4				// ����ѹ����
#define PUM2L_OUTPUT		TIM4->CCR2				// 2L������
#define PUM1L_OUTPUT		TIM4->CCR1				// 1L������
#define PUM_MINVALUE2L  1400							// 2L��Сռ�ձ� δ�����ʱ��СPWM  2019-04-10  1450

#define MUSIC_NUM1  5											// ��СֵΪ2
#define MUSIC_NUM2  2											// ��СֵΪ2
#define MUSIC_NUM3  5											// ��СֵΪ2
#define MUSIC_NUM4  5											// ��СֵΪ2
#define STANDBY_NUM	45										// ������ʱ

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */

extern __IO uint8_t Second_Flage;					// ��
extern __IO uint8_t Msecond_Flage;				// ����
extern __IO uint8_t Curent_Flage;					// ͳ�����ٱ�־
extern __IO uint8_t  Speak_Flage;					// ������      0 �ر�  1 ����   2 ������  3 ������  4 ������
extern const uint16_t Music_Code_1[MUSIC_NUM1];				// 
extern const uint16_t Music_Code_2[MUSIC_NUM2];				// 
extern const uint16_t Music_Code_3[MUSIC_NUM3];				// 
extern const uint16_t Music_Code_4[MUSIC_NUM4];				// 
extern __IO uint8_t  Standby_Time;				// ����ģʽ��ʱʱ��
extern __IO uint8_t  Init_Falge;					// ��ʼ����־
extern __IO uint8_t  Value_Delay_Time;		// ��ŷ��ر���ʱʱ��

void delay_ms(__IO uint32_t nCount);			// ����ʱ
void delay_us(__IO uint32_t nCount);			// ����ʱ

void SysTick_Init(void);									// ��ʼ����શ�ʱ��
void SysTick_ISR(void);										// ��શ�ʱ���жϴ�����
void TIM4_Configuration(void);						//
void TIM3_Configuration(void);						//
void TIM2_Configuration(void);
void TIM1_Configuration(void);
void Set_Music_Value(uint16_t value);			//
#endif /* DELAY THE END */

/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/
