/*
*********************************************************************************************************
*
*	模块名称 : 蜂鸣器驱动模块
*	文件名称 : bsp_beep.c
*	版    本 : V1.1
*	说    明 : 驱动蜂鸣器.
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2014-10-20 Liujingtao  正式发布
*		V1.1    2015-08-30 Liujingtao  增加修改蜂鸣器频率的功能 BEEP_Start() 函数添加频率形参
*
*	Copyright (C), 2015-2016, 沈阳恒德医疗器械研发有限公司 ShenYang HengDe Medical Instruments Co.，Ltd.
*
*********************************************************************************************************
*/

#include "bsp_beep.h"
extern TIM_HandleTypeDef htim1;
//#define BEEP_HAVE_POWER		/* 定义此行表示有源蜂鸣器，直接通过GPIO驱动, 无需PWM */

#if	1		/* 有源蜂鸣器 */

//	/* PA11 */
///*	#define GPIO_RCC_BEEP   RCC_APB2Periph_GPIOA
//	#define GPIO_PORT_BEEP	GPIOA
//	#define GPIO_PIN_BEEP		GPIO_Pin_11

//	#define BEEP_ENABLE()		GPIO_PORT_BEEP->BSRR = GPIO_PIN_BEEP		/* 使能蜂鸣器鸣叫 */
//	/*#define BEEP_DISABLE()	GPIO_PORT_BEEP->BRR = GPIO_PIN_BEEP			/* 禁止蜂鸣器鸣叫 */
///*#else	*/	/* 无源蜂鸣器 */
//	/* PA11/TIM1_CH4 ---> TIM1_CH4 */

	/* 1500表示频率1.5KHz，5000表示50.00%的占空比 */
	#define BEEP_ENABLE()	bsp_SetTIMOutPWM( g_tBeep.uiFreq, 5000);

	/* 禁止蜂鸣器鸣叫 */
	#define BEEP_DISABLE()	bsp_SetTIMOutPWM(1500, 0);
#endif

static BEEP_T g_tBeep;		/* 定义蜂鸣器全局结构体变量 */


/*
*********************************************************************************************************
*	函 数 名: bsp_SetTIMOutPWM
*	功能说明: 设置引脚输出的PWM信号的频率和占空比.  当频率为0，并且占空为0时，关闭定时器，GPIO输出0；
*			  当频率为0，占空比为100%时，GPIO输出1.
*	形    参: _ulFreq : PWM信号频率，单位Hz  (实际测试，最大输出频率为 168M / 4 = 42M）. 0 表示禁止输出
*			  _ulDutyCycle : PWM信号占空比，单位：万分之一。如5000，表示50.00%的占空比
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetTIMOutPWM( uint32_t _ulFreq, uint32_t _ulDutyCycle)	 
{

	uint16_t usPeriod;
	uint16_t usPrescaler;
	uint32_t uiTIMxCLK;

	if (_ulDutyCycle == 0)
	{	

       #if CPU == ST
	   __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, 0);
	   #else
	   timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_4,0);

	   #endif
		//TIM_Cmd(TIMx, DISABLE);		/* 关闭PWM输出 */
		//bsp_ConfigGpioOut(GPIOx, GPIO_Pin);	/* 配置GPIO为推挽输出 */		
		//GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_RESET);	/* PWM = 0 */		
		return;
	}
	else if (_ulDutyCycle == 10000)
	{
	   
		//TIM_Cmd(TIMx, DISABLE);		/* 关闭PWM输出 */
#if CPU == ST
	   __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, 65535);
#else
	   timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_4,65535);

#endif

		///bsp_ConfigGpioOut(GPIOx, GPIO_Pin);	/* 配置GPIO为推挽输出 */		
		//GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_SET);	/* PWM = 1 */	
		return;
	}
#if CPU == ST
		   __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, (_ulDutyCycle * usPeriod) / 10000);
#else
		   timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_4,(_ulDutyCycle * usPeriod) / 10000);
	
#endif


	
	//__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, (_ulDutyCycle * usPeriod) / 10000);
	//bsp_ConfigTimGpio(GPIOx, GPIO_Pin, TIMx, _ucChannel);	/* 使能GPIO和TIM时钟，并连接TIM通道到GPIO */
	
    /*-----------------------------------------------------------------------
		system_stm32f4xx.c 文件中 void SetSysClock(void) 函数对时钟的配置如下：

		HCLK = SYSCLK / 1     (AHB1Periph)
		PCLK2 = HCLK / 2      (APB2Periph)
		PCLK1 = HCLK / 4      (APB1Periph)

		因为APB1 prescaler != 1, 所以 APB1上的TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
		因为APB2 prescaler != 1, 所以 APB2上的TIMxCLK = PCLK2 x 2 = SystemCoreClock;

		APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM6, TIM12, TIM13,TIM14
		APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11

	----------------------------------------------------------------------- */

		uiTIMxCLK = SystemCoreClock;	// SystemCoreClock / 2;

	if (_ulFreq < 100)
	{
		usPrescaler = 10000 - 1;					/* 分频比 = 10000 */
		usPeriod =  (uiTIMxCLK / 10000) / _ulFreq  - 1;		/* 自动重装的值 */
	}
	else if (_ulFreq < 3000)
	{
		usPrescaler = 100 - 1;					/* 分频比 = 100 */
		usPeriod =  (uiTIMxCLK / 100) / _ulFreq  - 1;		/* 自动重装的值 */
	}
	else	/* 大于4K的频率，无需分频 */
	{
		usPrescaler = 0;					/* 分频比 = 1 */
		usPeriod = uiTIMxCLK / _ulFreq - 1;	/* 自动重装的值 */
	}





#if CPU == ST

	 /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = usPrescaler;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = usPeriod;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = (_ulDutyCycle * usPeriod) / 10000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);
  #else
  rcu_periph_clock_enable(RCU_GPIOA);
		 
		 /*Configure PB3 PB10 PB11(TIMER1 CH1 CH2 CH3) as alternate function*/
		 gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
		 gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_11);
  
	  
		 gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_11);
  
		 /* -----------------------------------------------------------------------
			TIMER1 configuration: generate 3 PWM signals with 3 different duty cycles:
			TIMER1CLK = SystemCoreClock / 200 = 1MHz
		 
			TIMER1 channel1 duty cycle = (4000/ 16000)* 100  = 25%
			TIMER1 channel2 duty cycle = (8000/ 16000)* 100  = 50%
			TIMER1 channel3 duty cycle = (12000/ 16000)* 100 = 75%
			----------------------------------------------------------------------- */
			timer_oc_parameter_struct timer_ocintpara;
			timer_parameter_struct timer_initpara;
		 
			rcu_periph_clock_enable(RCU_TIMER1);
			rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
		 
			timer_deinit(TIMER1);
		 
			/* TIMER1 configuration */
			timer_initpara.prescaler		 = 0;
			timer_initpara.alignedmode		 = TIMER_COUNTER_EDGE;
			timer_initpara.counterdirection  = TIMER_COUNTER_UP;
			timer_initpara.period			 = 65535;
			timer_initpara.clockdivision	 = TIMER_CKDIV_DIV1;
			timer_initpara.repetitioncounter = 0;
			timer_init(TIMER1,&timer_initpara);
		 
			/* CH1,CH2 and CH3 configuration in PWM mode */
			timer_ocintpara.ocpolarity	= TIMER_OC_POLARITY_HIGH;
			timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
			timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
			timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
			timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
			timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
		 
			timer_channel_output_config(TIMER1,TIMER_CH_1,&timer_ocintpara);
  
		 
			/* CH1 configuration in PWM mode1,duty cycle 25% */
			timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_4,65535/2);
			timer_channel_output_mode_config(TIMER1,TIMER_CH_4,TIMER_OC_MODE_PWM0);
			timer_channel_output_shadow_config(TIMER1,TIMER_CH_4,TIMER_OC_SHADOW_DISABLE);
		 
			/* auto-reload preload enable */
			timer_auto_reload_shadow_enable(TIMER1);
			/* auto-reload preload enable */
			timer_enable(TIMER1);

  #endif
}




/*
*********************************************************************************************************
*	函 数 名: BEEP_InitHard
*	功能说明: 初始化蜂鸣器硬件
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void BEEP_InitHard(void)
{
#ifdef	BEEP_HAVE_POWER		/* 有源蜂鸣器 */
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIO的时钟 */
	RCC_APB2PeriphClockCmd(GPIO_RCC_BEEP, ENABLE);

	BEEP_DISABLE();

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* 推挽输出模式 */	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_BEEP;
	GPIO_Init(GPIO_PORT_BEEP, &GPIO_InitStructure);
#else
	;	/* 无源蜂鸣器 */
	
#endif
}

/*
*********************************************************************************************************
*	函 数 名: BEEP_Start
*	功能说明: 启动蜂鸣音。
*	形    参：_uiFreq : 频率 (Hz)
*			  _usBeepTime : 蜂鸣时间，单位10ms; 0 表示不鸣叫
*			  _usStopTime : 停止时间，单位10ms; 0 表示持续鸣叫
*			 _usCycle : 鸣叫次数， 0 表示持续鸣叫
*	返 回 值: 无
*********************************************************************************************************
*/
void BEEP_Start(uint32_t _uiFreq, uint16_t _usBeepTime, uint16_t _usStopTime, uint16_t _usCycle)
{
	if (_usBeepTime == 0)
	{
		return;
	}

	g_tBeep.uiFreq = _uiFreq;
	g_tBeep.usBeepTime = _usBeepTime;
	g_tBeep.usStopTime = _usStopTime;
	g_tBeep.usCycle = _usCycle;
	g_tBeep.usCount = 0;
	g_tBeep.usCycleCount = 0;
	g_tBeep.ucState = 0;
	g_tBeep.ucEnalbe = 1;	/* 设置完全局参数后再使能发声标志 */

	BEEP_ENABLE();			/* 开始发声 */
}

/*
*********************************************************************************************************
*	函 数 名: BEEP_Stop
*	功能说明: 停止蜂鸣音。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void BEEP_Stop(void)
{
	g_tBeep.ucEnalbe = 0;

	BEEP_DISABLE();	/* 必须在清控制标志后再停止发声，避免停止后在中断中又开启 */
}

/*
*********************************************************************************************************
*	函 数 名: BEEP_KeyTone
*	功能说明: 发送按键音， 固定 1.5KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void BEEP_KeyTone(void)
{
	BEEP_Start(1500, 5, 1, 1);	/* 鸣叫50ms，停10ms， 1次 */
}

/*
*********************************************************************************************************
*	函 数 名: BEEP_Pro
*	功能说明: 每隔10ms调用1次该函数，用于控制蜂鸣器发声。该函数在 bsp_timer.c 中被调用。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void BEEP_Pro(void)
{
	if ((g_tBeep.ucEnalbe == 0) || (g_tBeep.usStopTime == 0))
	{
		return;
	}

	if (g_tBeep.ucState == 0)
	{
		if (g_tBeep.usStopTime > 0)	/* 间断发声 */
		{
			if (++g_tBeep.usCount >= g_tBeep.usBeepTime)
			{
				BEEP_DISABLE();		/* 停止发声 */
				g_tBeep.usCount = 0;
				g_tBeep.ucState = 1;
			}
		}
		else
		{
			;	/* 不做任何处理，连续发声 */
		}
	}
	else if (g_tBeep.ucState == 1)
	{
		if (++g_tBeep.usCount >= g_tBeep.usStopTime)
		{
			/* 连续发声时，直到调用stop停止为止 */
			if (g_tBeep.usCycle > 0)
			{
				if (++g_tBeep.usCycleCount >= g_tBeep.usCycle)
				{
					/* 循环次数到，停止发声 */
					g_tBeep.ucEnalbe = 0;
				}

				if (g_tBeep.ucEnalbe == 0)
				{
					g_tBeep.usStopTime = 0;
					return;
				}
			}

			g_tBeep.usCount = 0;
			g_tBeep.ucState = 0;

			BEEP_ENABLE();			/* 开始发声 */
		}
	}
}

/***************************** 沈阳恒德医疗器械研发有限公司 ShenYang HengDe Medical Instruments Co.，Ltd. (END OF FILE) *********************************/
