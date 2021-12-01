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
#include "usart3.h"		
#include "system.h"
#include "iic.h"
#include "bsp_led.h"
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

	// Heat_Com ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);
	PWR_BackupAccessCmd( ENABLE );
	RCC_LSEConfig( RCC_LSE_OFF );
	BKP_TamperPinCmd(DISABLE); 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13  | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	// Value ��ϴ��ŷ� 	PB5  LED PB12 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	// Senser A ԭˮ��ˮλ 	PB0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	// Senser B ˮ��ǰ�λ	PA7
	// Senser C ��ˮ��ˮλ	PA6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	// TDS-P
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	// TDS-N
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	// ��ʼ��IO״̬
	JDQ_Heat(0);
	ELE_ConP(0);
	ELE_ConN(0);
	VALUE_Out(0);
	TDS_P(0);
	TDS_N(0);
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

	// Enable DMA Channel3 Interrupt 	USART3_RX
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	//Enable DMA Channel2 Interrupt   USART3_TX
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

  //����USART3�ж�(RS232ͨѶ)
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	// ��ռ���ȼ�Ϊ2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				// �����ȼ�Ϊ1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						// �ж�ͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);														// ��ʼ��NVIC�ṹ��	
	
	// TIMER1
//	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);		
	// TIMER2
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
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
	/* д��0x5555,�����������Ĵ���д�빦�� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* ����ʱ�ӷ�Ƶ,40K/256=156HZ(6.4ms)*/
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
	/* ι��ʱ�� 1.2s/6.4MS=200 .ע�ⲻ�ܴ���0xfff*/
	IWDG_SetReload(200);
	
	/* ι��*/
	IWDG_ReloadCounter();
	
	/* ʹ�ܹ���*/
	IWDG_Enable();
}

/***********************************************************************************
 * ��������Init_Board
 * ��  ��:
 * ��  ��:
 * ����˵������ʼ��������Ӳ��
************************************************************************************/
uint8_t Init_Board(void)
{
	uint8_t data[2];
	
	// Ӳ����ʼ��
	Board_GPIO_Config();										// GPIO
	Init_GT208L();													// ��������
	bsp_led_init();													// LEDָʾ��
	SysTick_Init();													// ��ʼ��ϵͳ��શ�ʱ��s
	ADC1_Mode_Config();											// ADC1
	ADC2_Mode_Config();											// ADC2
//	TIM1_Configuration();										// TDS
	TIM2_Configuration();										// ������������ʱ
	TIM3_Configuration();										// ����������
	TIM4_Configuration();										// ������ �����ǿ�ȿ���
	USART3_Config();												// ��ʼ��USART1
	NVIC_Configuration();										// �ж�
	// ��ȡ��о����
	EEP_Read_Dates(RFID_CODE_ADDR,4,RF_ID_Code);						// ͻ��ʽ��ȡEEPROM 
	EEP_Read_Dates(RFID_CURRENT_SPEED,2,data);							// ͻ��ʽ��ȡEEPROM
	Filter_Count=data[1]*10+data[0];												// �ָ�����
	data[0]=EEP_Read_Date(CLERA_FLAGE_ADDR);								// ͻ��ʽ��ȡEEPROM
	if(data[0]==0xAA) New_Filter_Flage=0x01;								// ����о��ϴ��־
	data[0]=EEP_Read_Date(TEST_FILTER_ADDR);								// ͻ��ʽ��ȡEEPROM
	if(data[0]==0xBB) Test_Filter_Flage=0x01;								// ������о
	EEP_Read_Dates(FILTER_LIFT_ADDR,2,data);								// ͻ��ʽ��ȡEEPROM
	Filter_Life=data[1]*10+data[0];													// ��о����
	if(Filter_Life==0xFFFF) Filter_Life=0;									// ��о����
	// �ж���о״̬
	if(Filter_Life>335) LED_LX=5;														// ��о����
	else if(Filter_Life>166) LED_LX=4;											// ��о����
	else if(Filter_Life>50) LED_LX=3;												// ��оһ��
	else if(Filter_Life>5) LED_LX=2;												// ��о���
	else LED_LX=1;																					// ��о�����˸
	IWDG_Config();																					// ���Ź�
	return 0;
}


/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/
