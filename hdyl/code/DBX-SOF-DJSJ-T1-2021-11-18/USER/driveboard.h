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

#define ELE_Read    GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)	// �������ű�������ʼ���

#define VALUE_Out(a)do{if (a)\
										GPIO_SetBits(GPIOB,GPIO_Pin_5);\
										else\
										GPIO_ResetBits(GPIOB,GPIO_Pin_5);}while(0)	

#define LED_Pow(a)	do{if (a)\
										GPIO_SetBits(GPIOB,GPIO_Pin_12);\
										else\
										GPIO_ResetBits(GPIOB,GPIO_Pin_12);}while(0)		

// ���Ƶ������� Ĭ�����ӵ�����  ���������ӵ�����
#define ELE_ConP(a)	do{if (a)\
										GPIO_SetBits(GPIOC,GPIO_Pin_13);\
										else\
										GPIO_ResetBits(GPIOC,GPIO_Pin_13);}while(0)		

// ���Ƶ��۸��� Ĭ�����ӵ�����  ���������ӵ�����
#define ELE_ConN(a)	do{if (a)\
										GPIO_SetBits(GPIOC,GPIO_Pin_14);\
										else\
										GPIO_ResetBits(GPIOC,GPIO_Pin_14);}while(0)												
// �̵���Y1	PC15
#define JDQ_Heat(a)	do{if (a)\
										GPIO_SetBits(GPIOC,GPIO_Pin_15);\
										else\
										GPIO_ResetBits(GPIOC,GPIO_Pin_15);}while(0)
// TDS-P
#define TDS_P(a)		do{if (a)\
										GPIO_SetBits(GPIOA,GPIO_Pin_8);\
										else\
										GPIO_ResetBits(GPIOA,GPIO_Pin_8);}while(0)
// TDS-N
#define TDS_N(a)		do{if (a)\
										GPIO_SetBits(GPIOB,GPIO_Pin_13);\
										else\
										GPIO_ResetBits(GPIOB,GPIO_Pin_13);}while(0)
		

#define SENSER_YSX GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)			 // ԭˮ��ˮλ			
#define SENSER_SSX GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)			 // ��ˮ��ˮλ												
#define SENSER_SXG GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)			 // ˮ���
	
													
void Board_GPIO_Config(void);					// Ӳ������ 
void IWDG_Config(void);								// Ӳ�����Ź�	
void NVIC_Configuration(void);				// �ж����ú���
uint8_t Init_Board(void);

										
#endif

/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/
