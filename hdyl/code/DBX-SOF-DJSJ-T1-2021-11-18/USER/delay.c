/******************** (C) COPYRIGHT 2014 ʢԪ���ӹ����� **************************
 * �ļ���  ��delay.c
 * ��    ����MODBUS����
 * ʵ��ƽ̨��STM32�ֳֻ�������
 * Ӳ�����ӣ���
 * ��汾  ��ST3.0.0
 * ����    ��С��
 * ��Ŀ˵����MODBUS
**********************************************************************************/
#include "delay.h" 
#include "usart1.h"
#include "driveboard.h"
#include "bsp_led.h"
#include "system.h"

__IO uint8_t Second_Flage=0;				// ���־
__IO uint8_t Msecond_Flage=0;				// �����־
__IO uint8_t Curent_Flage=0;				// ͳ�����ٱ�־
__IO uint16_t MSecond=0;						// ����
__IO uint8_t  HS_Flage=0;						// ����
__IO uint8_t  MS_Flage=1;						// �����־
__IO uint8_t  Second=0;							// ��
__IO uint8_t  Minute=0;							// ��ʱ����
__IO uint8_t  Hour=0;								// ��ʱ����
__IO uint8_t  Speak_Flage=1;				// ������      0 �ر�  1 ����   2 ������  3 ��ʾ��  4 ������
__IO uint8_t  Standby_Time=0;				// ����ģʽ��ʱʱ��
__IO uint8_t  Init_Falge=0;					// ��ʼ����־
__IO uint8_t Temp_File[500];				// �����ļ�
// SAVE F:\PID\TEMP0.28.HEX &Temp_File[0],&Temp_File[0]+300

const uint16_t Music_Code_1[MUSIC_NUM1]=			// ����
								{
									2000, 2000, 2000, 1800, 1800
								};		
const uint16_t Music_Code_2[MUSIC_NUM2]=			// ����
								{
									988,1760
								};	
const uint16_t Music_Code_3[MUSIC_NUM3]=			// ��ʾ��
								{
									2400, 2400, 2000, 1175, 1175
								};				
const uint16_t Music_Code_4[MUSIC_NUM4]=			// ����������
								{
									1800, 1800, 1800, 2000, 2000
								};	

/***********************************************************************************
 * ��������delay_System_MS
 * ����  ��System_MS��ʱ
 * ����  ����
 * ���  ����
************************************************************************************/
void delay_ms(__IO uint32_t nCount)
{
	uint32_t i;
	for(i=0;i<nCount;i++)
	{
		delay_us(1000);
	}
}
/***********************************************************************************
 * ��������delay_us
 * ����  ��us��ʱ
 * ����  ����
 * ���  ���� j=6ʵ��Ϊ1us 
************************************************************************************/
void delay_us(__IO uint32_t nCount)
{
	uint32_t i,j;
	for(i=0;i<nCount;i++)
	{
  	for(j=0;j<6;j++)
		{
			__ASM("NOP");
		}	
	}
}

/*************************************************************************************
 *	��������:	TIM3_Configuration(void)	
 *	�������:	
 *	�������:
 *	��������:	
*************************************************************************************/
void Set_Music_Value(uint16_t value)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  /* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Period = 1000000/value;
  TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM3->CCR4=(1000000/value)/2;										// speak delay
}

/*************************************************************************************
 *	��������:	TIM1_Configuration(void)	
 *	�������:	
 *	�������:
 *	��������:	TDS
*************************************************************************************/
void TIM1_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
	
	
	/* GPIOA RCC_APB2Periph_AFIO ��ʱ��	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//
  TIM_DeInit(TIM1);
	TIM_TimeBaseStructure.TIM_Period = 4000;		// 50us��������
	TIM_TimeBaseStructure.TIM_Prescaler = 71;				
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	//
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity =TIM_OCNPolarity_High; 		//TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;      
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;   
	TIM_OCInitStructure.TIM_Pulse = 2000;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	/* TIM1 enable counter */
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);   
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/*************************************************************************************
 *	��������:	TIM2_Configuration(void)	
 *	�������:	
 *	�������:
 *	��������:	���������ļ���ʱ
*************************************************************************************/
void TIM2_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
  /* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	/* Reset Time2 */
  TIM_DeInit(TIM2);
  /* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Period = 5000;				// 50ms
  TIM_TimeBaseStructure.TIM_Prescaler = (720-1);	// 10ns
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	
		
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE); 
}

/*************************************************************************************
 *	��������:	TIM3_Configuration(void)	
 *	�������:	
 *	�������:
 *	��������:	������
*************************************************************************************/
void TIM3_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	/* GPIOA RCC_APB2Periph_AFIO ��ʱ��	*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//
  TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = 366;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = TIM_OPMode_Repetitive;	/* �ı�ռ�ձ� */
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);						// �ú���ֻ���TIM3 TIM8
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);	
	TIM3->CCR4=0;																// speak	TIM4->CCR4=4000;		// speak	
}
/*************************************************************************************
 *	��������:	TIM4_Configuration(void)	
 *	�������:	
 *	�������:
 *	��������:	MOTOR
*************************************************************************************/
void TIM4_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	/* GPIOB RCC_APB2Periph_AFIO ��ʱ��	*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*
	TIM4 ����: ����1·PWM�ź�;
	TIM4CLK = 72 MHz, Prescaler = 0(����Ƶ), TIM4 counter clock = 72 MHz
	���㹫ʽ��
	PWM���Ƶ�� = TIM3 counter clock /(ARR + 1)

	������������Ϊ100Hz

	�������TIM3CLKԤ��Ƶ����ô�����ܵõ�100Hz��Ƶ��
	�������÷�Ƶ�� = 1000�� ��ô  TIM3 counter clock = 72KHz
	TIM_Period = 720 - 1;
	Ƶ���²�����
 */
  TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = 4000;	/* TIM_Period = TIM3 ARR Register */
	TIM_TimeBaseStructure.TIM_Prescaler = 1;	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = TIM_OPMode_Repetitive;	/* �ı�ռ�ձ� */
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);	
	TIM4->CCR1=0;				// 1L������
	TIM4->CCR2=0;				// 2L������
	TIM4->CCR3=0;				// 2L������
	TIM4->CCR4=4000;		// ����ѹ����
}

/***********************************************************************************
 * ��������SysTick_Init
 * ��  ��: ��
 * ��  ��: ��
 * ��  �ܣ�����ϵͳ�δ�ʱ�� SysTick0 10ms����һ��
************************************************************************************/
void SysTick_Init(void)
{
	if (SysTick_Config(72*10000))
  { 
    /* Capture error */ 
    while (1);
  }
}

/***********************************************************************************
 * ��������SysTick_ISR
 * ��  ��:
 * ��  ��:
 * ����˵����SysTick�жϷ������ÿ��10MS����1��
************************************************************************************/
void SysTick_ISR(void)
{
	static uint8_t ledstate=0;
	static uint16_t i=0;

	// ʱ�Ӻ���		
	if(MSecond++>99)				// 1��
	{
		MSecond=0;
		Second_Flage=1;	
		if(Second++>=59)			// 1����
		{
			Second=0;
			if(Minute++>=59)		// 1Сʱ
			{
				Minute=0;					
				if(Hour<24) Hour++;
			}
		}
		// LED
		if(ledstate==0)
		{
			ledstate=1;
			LED_Pow(1);
		}
		else
		{
			ledstate=0;
			LED_Pow(0);
		}
		// ����ģʽ�ж�
		if(Work_Step==0)
		{
			if(Standby_Time<STANDBY_NUM)	Standby_Time++;
		}
		// ��ʼ��ϵͳʱˢ����ʾ��ʱʱ��
		if(Second>2) Init_Falge=1;				// ��ʼ����־
		Curent_Flage=1;										// ����ͳ�Ʊ�־
	}	
	if(MSecond%50==0)
	{
		// �����¶ȼ�¼
		if(Work_Step==0) i=0;
		if(Work_Step==10&&i<490) Temp_File[i++]=Hmwater_Temp;
	}
	// �����־
	Msecond_Flage=1;
}



/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/
