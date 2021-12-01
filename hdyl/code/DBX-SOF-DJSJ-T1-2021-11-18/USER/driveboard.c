/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************
 * 文件名  ：system.c
 * 描    述：MODBUS开发
 * 实验平台：STM32通讯转换
 * 硬件连接：无
 * 库版本  ：ST3.0.0
 * 作者    ：小奕
 * 项目说明：MODBUS
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
 * 函数名：void Output_GPIO_Config(void)	 												
 * 描述  ： I/O配置	   														
 * 输入  ：无																	
 * 输出  ：无																	
 * 调用  ：内部调用																
*********************************************************************************/
void Board_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

	// Heat_Com 电解
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);
	PWR_BackupAccessCmd( ENABLE );
	RCC_LSEConfig( RCC_LSE_OFF );
	BKP_TamperPinCmd(DISABLE); 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13  | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	// Value 清洗电磁阀 	PB5  LED PB12 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	// Senser A 原水箱水位 	PB0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	// Senser B 水箱盖板位	PA7
	// Senser C 酸水箱水位	PA6
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

	// 初始化IO状态
	JDQ_Heat(0);
	ELE_ConP(0);
	ELE_ConN(0);
	VALUE_Out(0);
	TDS_P(0);
	TDS_N(0);
}

/***********************************************************************************
 * 函数名：NVIC_Configuration(void)
 * 描述  ：中断配置函数
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						// 配置优先级组为第2组(抢占式优先级有2位，副优先级有2位)

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

  //配置USART3中断(RS232通讯)
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	// 抢占优先级为2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				// 副优先级为1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						// 中断通道使能
  NVIC_Init(&NVIC_InitStructure);														// 初始化NVIC结构体	
	
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
 *	函数名称:	void IWDG_Config(void)		
 *	输入变量:	
 *	输出变量:
 *	功能描述:	用于配置硬件	看门狗	
*************************************************************************************/
void IWDG_Config(void)
{
	/* 写入0x5555,用于允许狗狗寄存器写入功能 */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* 狗狗时钟分频,40K/256=156HZ(6.4ms)*/
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
	/* 喂狗时间 1.2s/6.4MS=200 .注意不能大于0xfff*/
	IWDG_SetReload(200);
	
	/* 喂狗*/
	IWDG_ReloadCounter();
	
	/* 使能狗狗*/
	IWDG_Enable();
}

/***********************************************************************************
 * 函数名：Init_Board
 * 输  入:
 * 输  出:
 * 功能说明：初始化驱动板硬件
************************************************************************************/
uint8_t Init_Board(void)
{
	uint8_t data[2];
	
	// 硬件初始化
	Board_GPIO_Config();										// GPIO
	Init_GT208L();													// 触摸按键
	bsp_led_init();													// LED指示灯
	SysTick_Init();													// 初始化系统嘀嗒定时器s
	ADC1_Mode_Config();											// ADC1
	ADC2_Mode_Config();											// ADC2
//	TIM1_Configuration();										// TDS
	TIM2_Configuration();										// 蜂鸣器节拍延时
	TIM3_Configuration();										// 蜂鸣器节奏
	TIM4_Configuration();										// 自吸泵 及电解强度控制
	USART3_Config();												// 初始化USART1
	NVIC_Configuration();										// 中断
	// 读取滤芯数据
	EEP_Read_Dates(RFID_CODE_ADDR,4,RF_ID_Code);						// 突发式读取EEPROM 
	EEP_Read_Dates(RFID_CURRENT_SPEED,2,data);							// 突发式读取EEPROM
	Filter_Count=data[1]*10+data[0];												// 恢复流速
	data[0]=EEP_Read_Date(CLERA_FLAGE_ADDR);								// 突发式读取EEPROM
	if(data[0]==0xAA) New_Filter_Flage=0x01;								// 新滤芯反洗标志
	data[0]=EEP_Read_Date(TEST_FILTER_ADDR);								// 突发式读取EEPROM
	if(data[0]==0xBB) Test_Filter_Flage=0x01;								// 测试滤芯
	EEP_Read_Dates(FILTER_LIFT_ADDR,2,data);								// 突发式读取EEPROM
	Filter_Life=data[1]*10+data[0];													// 滤芯寿命
	if(Filter_Life==0xFFFF) Filter_Life=0;									// 滤芯寿命
	// 判断滤芯状态
	if(Filter_Life>335) LED_LX=5;														// 滤芯三格
	else if(Filter_Life>166) LED_LX=4;											// 滤芯二格
	else if(Filter_Life>50) LED_LX=3;												// 滤芯一格
	else if(Filter_Life>5) LED_LX=2;												// 滤芯红灯
	else LED_LX=1;																					// 滤芯红灯闪烁
	IWDG_Config();																					// 看门狗
	return 0;
}


/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/
