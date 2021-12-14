/******************** (C) COPYRIGHT 2014 盛元电子工作室 **************************
 * 文件名  ：delay.c
 * 描    述：MODBUS开发
 * 实验平台：STM32手持机开发板
 * 硬件连接：无
 * 库版本  ：ST3.0.0
 * 作者    ：小奕
 * 项目说明：MODBUS
**********************************************************************************/
#include "delay.h" 
#include "usart1.h"
#include "driveboard.h"
#include "bsp_led.h"
#include "system.h"

__IO uint8_t Second_Flage=0;				// 秒标志
__IO uint8_t Msecond_Flage=0;				// 毫秒标志
__IO uint8_t Curent_Flage=0;				// 统计流速标志
__IO uint16_t MSecond=0;						// 毫秒
__IO uint8_t  HS_Flage=0;						// 半秒
__IO uint8_t  MS_Flage=1;						// 毫秒标志
__IO uint8_t  Second=0;							// 秒
__IO uint8_t  Minute=0;							// 计时函数
__IO uint8_t  Hour=0;								// 计时函数
__IO uint8_t  Speak_Flage=1;				// 蜂鸣器      0 关闭  1 开机   2 按键音  3 提示音  4 报警音
__IO uint8_t  Standby_Time=0;				// 待机模式延时时间
__IO uint8_t  Init_Falge=0;					// 初始化标志
__IO uint8_t Temp_File[500];				// 数据文件
// SAVE F:\PID\TEMP0.28.HEX &Temp_File[0],&Temp_File[0]+300

const uint16_t Music_Code_1[MUSIC_NUM1]=			// 开机
								{
									2000, 2000, 2000, 1800, 1800
								};		
const uint16_t Music_Code_2[MUSIC_NUM2]=			// 按键
								{
									988,1760
								};	
const uint16_t Music_Code_3[MUSIC_NUM3]=			// 提示音
								{
									2400, 2400, 2000, 1175, 1175
								};				
const uint16_t Music_Code_4[MUSIC_NUM4]=			// 报警不工作
								{
									1800, 1800, 1800, 2000, 2000
								};	

/***********************************************************************************
 * 函数名：delay_System_MS
 * 描述  ：System_MS延时
 * 输入  ：无
 * 输出  ：无
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
 * 函数名：delay_us
 * 描述  ：us延时
 * 输入  ：无
 * 输出  ：无 j=6实际为1us 
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
 *	函数名称:	TIM3_Configuration(void)	
 *	输入变量:	
 *	输出变量:
 *	功能描述:	
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
 *	函数名称:	TIM1_Configuration(void)	
 *	输入变量:	
 *	输出变量:
 *	功能描述:	TDS
*************************************************************************************/
void TIM1_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
	
	
	/* GPIOA RCC_APB2Periph_AFIO 的时钟	*/
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
	TIM_TimeBaseStructure.TIM_Period = 4000;		// 50us采样周期
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
 *	函数名称:	TIM2_Configuration(void)	
 *	输入变量:	
 *	输出变量:
 *	功能描述:	嗡鸣器节拍间延时
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
 *	函数名称:	TIM3_Configuration(void)	
 *	输入变量:	
 *	输出变量:
 *	功能描述:	蜂鸣器
*************************************************************************************/
void TIM3_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	/* GPIOA RCC_APB2Periph_AFIO 的时钟	*/
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
	TIM_OCInitStructure.TIM_Pulse = TIM_OPMode_Repetitive;	/* 改变占空比 */
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);						// 该函数只针对TIM3 TIM8
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);	
	TIM3->CCR4=0;																// speak	TIM4->CCR4=4000;		// speak	
}
/*************************************************************************************
 *	函数名称:	TIM4_Configuration(void)	
 *	输入变量:	
 *	输出变量:
 *	功能描述:	MOTOR
*************************************************************************************/
void TIM4_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	/* GPIOB RCC_APB2Periph_AFIO 的时钟	*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*
	TIM4 配置: 产生1路PWM信号;
	TIM4CLK = 72 MHz, Prescaler = 0(不分频), TIM4 counter clock = 72 MHz
	计算公式：
	PWM输出频率 = TIM3 counter clock /(ARR + 1)

	我们期望设置为100Hz

	如果不对TIM3CLK预分频，那么不可能得到100Hz低频。
	我们设置分频比 = 1000， 那么  TIM3 counter clock = 72KHz
	TIM_Period = 720 - 1;
	频率下不来。
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
	TIM_OCInitStructure.TIM_Pulse = TIM_OPMode_Repetitive;	/* 改变占空比 */
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
	TIM4->CCR1=0;				// 1L自吸泵
	TIM4->CCR2=0;				// 2L自吸泵
	TIM4->CCR3=0;				// 2L自吸泵
	TIM4->CCR4=4000;		// 电解电压控制
}

/***********************************************************************************
 * 函数名：SysTick_Init
 * 输  入: 无
 * 输  出: 无
 * 功  能：启动系统滴答定时器 SysTick0 10ms进入一次
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
 * 函数名：SysTick_ISR
 * 输  入:
 * 输  出:
 * 功能说明：SysTick中断服务程序，每隔10MS进入1次
************************************************************************************/
void SysTick_ISR(void)
{
	static uint8_t ledstate=0;
	static uint16_t i=0;

	// 时钟函数		
	if(MSecond++>99)				// 1秒
	{
		MSecond=0;
		Second_Flage=1;	
		if(Second++>=59)			// 1分钟
		{
			Second=0;
			if(Minute++>=59)		// 1小时
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
		// 待机模式判断
		if(Work_Step==0)
		{
			if(Standby_Time<STANDBY_NUM)	Standby_Time++;
		}
		// 初始化系统时刷新显示延时时间
		if(Second>2) Init_Falge=1;				// 初始化标志
		Curent_Flage=1;										// 流量统计标志
	}	
	if(MSecond%50==0)
	{
		// 保存温度记录
		if(Work_Step==0) i=0;
		if(Work_Step==10&&i<490) Temp_File[i++]=Hmwater_Temp;
	}
	// 毫秒标志
	Msecond_Flage=1;
}



/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/
