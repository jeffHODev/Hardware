/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************
 * �ļ���  ��system.c
 * ��    ����MODBUS����
 * ʵ��ƽ̨��STM32ͨѶת��
 * Ӳ�����ӣ���
 * ��汾  ��ST3.0.0
 * ����    ��С��
 * ��Ŀ˵����MODBUS
**********************************************************************************/
#include "stm32f10x.h"
#include "driveboard.h"
#include "delay.h"
#include "usart1.h"		
#include "usart3.h"		
#include "system.h"
#include "adc.h"

/********************************************************************************
 * ��������void Output_GPIO_Config(void)	 												
 * ����  �� I/O����	   														
 * ����  ����																	
 * ���  ����																	
 * ����  ���ڲ�����																
*********************************************************************************/
void Board_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

	// JDQ_Y5 JDQ_Y6 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

//	// ��ʼ��IO״̬
	JDQ_YD1(0);JDQ_YU2(0);
}

/***********************************************************************************
 * ��������NVIC_Configuration(void)
 * ����  ���ж����ú���
 * ����  ����
 * ���  ����
************************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						// �������ȼ���Ϊ��2��(��ռʽ���ȼ���2λ�������ȼ���2λ)

  //����USART1�ж�(RS232ͨѶ)
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	// ��ռ���ȼ�Ϊ2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				// �����ȼ�Ϊ1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						// �ж�ͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);														// ��ʼ��NVIC�ṹ��

  //����USART2�ж�(GSMģ��)
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	// ��ռ���ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				// �����ȼ�Ϊ2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						// �ж�ͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);														// ��ʼ��NVIC�ṹ��

  //����USART3�ж�(RS485ͨѶ)
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	// ��ռ���ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				// �����ȼ�Ϊ2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						// �ж�ͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);														// ��ʼ��NVIC�ṹ��

	// Enable DMA Channel2 Interrupt 	USART3_TX
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	//Enable DMA Channel3 Interrupt  	USART3_RX 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

	// Enable DMA Channel4 Interrupt 	USART1_TX
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	//Enable DMA Channel5 Interrupt   USART1_RX
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	//Enable DMA Channel6 Interrupt   USART2_RX
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	
	//Enable DMA Channel7 Interrupt 	USART2_TX
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}

/*************************************************************************************
 *	��������:	void IWDG_Config(void)		
 *	�������:	
 *	�������:
 *	��������:	��������Ӳ��	���Ź�	
*************************************************************************************/
void IWDG_Config(void)
{
//	/* д��0x5555,�����������Ĵ���д�빦�� */
//	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
//	
//	/* ����ʱ�ӷ�Ƶ,40K/256=156HZ(6.4ms)*/
//	IWDG_SetPrescaler(IWDG_Prescaler_256);
//	
//	/* ι��ʱ�� 1.2s/6.4MS=781 .ע�ⲻ�ܴ���0xfff*/
//	IWDG_SetReload(200);
//	
//	/* ι��*/
//	IWDG_ReloadCounter();
//	
//	/* ʹ�ܹ���*/
//	IWDG_Enable();
}

/***********************************************************************************
 * ��������Init_Board
 * ��  ��:
 * ��  ��:
 * ����˵������ʼ��������Ӳ��
************************************************************************************/
uint8_t Init_Board(void)
{
	uint8_t adcerro=0;
	
	Board_GPIO_Config();										//
	SysTick_Init();													// ��ʼ��ϵͳ��શ�ʱ��
	USART1_Config();												// ��ʼ��USART1
	USART3_Config();												// ��ʼ��USART1
	NVIC_Configuration();										// �ж�
	ADC1_Mode_Config();											// ADC
	return adcerro;
}


/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/
