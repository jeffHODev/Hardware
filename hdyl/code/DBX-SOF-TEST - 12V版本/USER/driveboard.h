#ifndef __DRIVEBOARD_H_
#define __DRIVEBOARD_H_
#include "stm32f10x.h"

#define USART_1  1
#define USART_2  2
#define USART_3  3										// ����3
#define DEBUGE   0										// ����ģʽ   ==0  ����ģʽ
#define ON       1
#define OFF      0

/* Ӳ����������	*/
// Ӳ������

// �̵���Y1	�µ�
#define JDQ_YD1(a)	do{if (a)\
										GPIO_ResetBits(GPIOB,GPIO_Pin_13);\
										else\
										GPIO_SetBits(GPIOB,GPIO_Pin_13);}while(0)

// �̵���Y2	�ϵ�
#define JDQ_YU2(a)	do{if (a)\
										GPIO_ResetBits(GPIOB,GPIO_Pin_15);\
										else\
										GPIO_SetBits(GPIOB,GPIO_Pin_15);}while(0)
													

void Board_GPIO_Config(void);					// Ӳ������ 
void IWDG_Config(void);								// Ӳ�����Ź�	
void NVIC_Configuration(void);				// �ж����ú���
uint8_t Init_Board(void);

										
#endif

/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/
