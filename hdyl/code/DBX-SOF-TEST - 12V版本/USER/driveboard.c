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
#include "usart1.h"		
#include "usart3.h"		
#include "system.h"
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

	// JDQ_Y5 JDQ_Y6 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

//	// 初始化IO状态
	JDQ_YD1(0);JDQ_YU2(0);
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

  //配置USART1中断(RS232通讯)
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	// 抢占优先级为2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				// 副优先级为1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						// 中断通道使能
  NVIC_Init(&NVIC_InitStructure);														// 初始化NVIC结构体

  //配置USART2中断(GSM模块)
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	// 抢占优先级为0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				// 副优先级为2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						// 中断通道使能
  NVIC_Init(&NVIC_InitStructure);														// 初始化NVIC结构体

  //配置USART3中断(RS485通讯)
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	// 抢占优先级为0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				// 副优先级为2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						// 中断通道使能
  NVIC_Init(&NVIC_InitStructure);														// 初始化NVIC结构体

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
 *	函数名称:	void IWDG_Config(void)		
 *	输入变量:	
 *	输出变量:
 *	功能描述:	用于配置硬件	看门狗	
*************************************************************************************/
void IWDG_Config(void)
{
//	/* 写入0x5555,用于允许狗狗寄存器写入功能 */
//	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
//	
//	/* 狗狗时钟分频,40K/256=156HZ(6.4ms)*/
//	IWDG_SetPrescaler(IWDG_Prescaler_256);
//	
//	/* 喂狗时间 1.2s/6.4MS=781 .注意不能大于0xfff*/
//	IWDG_SetReload(200);
//	
//	/* 喂狗*/
//	IWDG_ReloadCounter();
//	
//	/* 使能狗狗*/
//	IWDG_Enable();
}

/***********************************************************************************
 * 函数名：Init_Board
 * 输  入:
 * 输  出:
 * 功能说明：初始化驱动板硬件
************************************************************************************/
uint8_t Init_Board(void)
{
	uint8_t adcerro=0;
	
	Board_GPIO_Config();										//
	SysTick_Init();													// 初始化系统嘀嗒定时器
	USART1_Config();												// 初始化USART1
	USART3_Config();												// 初始化USART1
	NVIC_Configuration();										// 中断
	ADC1_Mode_Config();											// ADC
	return adcerro;
}


/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/
