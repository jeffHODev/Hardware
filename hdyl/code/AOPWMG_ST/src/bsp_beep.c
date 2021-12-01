/*
*********************************************************************************************************
*
*	ģ������ : ����������ģ��
*	�ļ����� : bsp_beep.c
*	��    �� : V1.1
*	˵    �� : ����������.
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2014-10-20 Liujingtao  ��ʽ����
*		V1.1    2015-08-30 Liujingtao  �����޸ķ�����Ƶ�ʵĹ��� BEEP_Start() �������Ƶ���β�
*
*	Copyright (C), 2015-2016, �������ҽ����е�з����޹�˾ ShenYang HengDe Medical Instruments Co.��Ltd.
*
*********************************************************************************************************
*/

#include "bsp_beep.h"
extern TIM_HandleTypeDef htim1;
//#define BEEP_HAVE_POWER		/* ������б�ʾ��Դ��������ֱ��ͨ��GPIO����, ����PWM */

#if	1		/* ��Դ������ */

//	/* PA11 */
///*	#define GPIO_RCC_BEEP   RCC_APB2Periph_GPIOA
//	#define GPIO_PORT_BEEP	GPIOA
//	#define GPIO_PIN_BEEP		GPIO_Pin_11

//	#define BEEP_ENABLE()		GPIO_PORT_BEEP->BSRR = GPIO_PIN_BEEP		/* ʹ�ܷ��������� */
//	/*#define BEEP_DISABLE()	GPIO_PORT_BEEP->BRR = GPIO_PIN_BEEP			/* ��ֹ���������� */
///*#else	*/	/* ��Դ������ */
//	/* PA11/TIM1_CH4 ---> TIM1_CH4 */

	/* 1500��ʾƵ��1.5KHz��5000��ʾ50.00%��ռ�ձ� */
	#define BEEP_ENABLE()	bsp_SetTIMOutPWM( g_tBeep.uiFreq, 5000);

	/* ��ֹ���������� */
	#define BEEP_DISABLE()	bsp_SetTIMOutPWM(1500, 0);
#endif

static BEEP_T g_tBeep;		/* ���������ȫ�ֽṹ����� */


/*
*********************************************************************************************************
*	�� �� ��: bsp_SetTIMOutPWM
*	����˵��: �������������PWM�źŵ�Ƶ�ʺ�ռ�ձ�.  ��Ƶ��Ϊ0������ռ��Ϊ0ʱ���رն�ʱ����GPIO���0��
*			  ��Ƶ��Ϊ0��ռ�ձ�Ϊ100%ʱ��GPIO���1.
*	��    ��: _ulFreq : PWM�ź�Ƶ�ʣ���λHz  (ʵ�ʲ��ԣ�������Ƶ��Ϊ 168M / 4 = 42M��. 0 ��ʾ��ֹ���
*			  _ulDutyCycle : PWM�ź�ռ�ձȣ���λ�����֮һ����5000����ʾ50.00%��ռ�ձ�
*	�� �� ֵ: ��
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
		//TIM_Cmd(TIMx, DISABLE);		/* �ر�PWM��� */
		//bsp_ConfigGpioOut(GPIOx, GPIO_Pin);	/* ����GPIOΪ������� */		
		//GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_RESET);	/* PWM = 0 */		
		return;
	}
	else if (_ulDutyCycle == 10000)
	{
	   
		//TIM_Cmd(TIMx, DISABLE);		/* �ر�PWM��� */
#if CPU == ST
	   __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, 65535);
#else
	   timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_4,65535);

#endif

		///bsp_ConfigGpioOut(GPIOx, GPIO_Pin);	/* ����GPIOΪ������� */		
		//GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_SET);	/* PWM = 1 */	
		return;
	}
#if CPU == ST
		   __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, (_ulDutyCycle * usPeriod) / 10000);
#else
		   timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_4,(_ulDutyCycle * usPeriod) / 10000);
	
#endif


	
	//__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, (_ulDutyCycle * usPeriod) / 10000);
	//bsp_ConfigTimGpio(GPIOx, GPIO_Pin, TIMx, _ucChannel);	/* ʹ��GPIO��TIMʱ�ӣ�������TIMͨ����GPIO */
	
    /*-----------------------------------------------------------------------
		system_stm32f4xx.c �ļ��� void SetSysClock(void) ������ʱ�ӵ��������£�

		HCLK = SYSCLK / 1     (AHB1Periph)
		PCLK2 = HCLK / 2      (APB2Periph)
		PCLK1 = HCLK / 4      (APB1Periph)

		��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
		��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIMxCLK = PCLK2 x 2 = SystemCoreClock;

		APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM6, TIM12, TIM13,TIM14
		APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11

	----------------------------------------------------------------------- */

		uiTIMxCLK = SystemCoreClock;	// SystemCoreClock / 2;

	if (_ulFreq < 100)
	{
		usPrescaler = 10000 - 1;					/* ��Ƶ�� = 10000 */
		usPeriod =  (uiTIMxCLK / 10000) / _ulFreq  - 1;		/* �Զ���װ��ֵ */
	}
	else if (_ulFreq < 3000)
	{
		usPrescaler = 100 - 1;					/* ��Ƶ�� = 100 */
		usPeriod =  (uiTIMxCLK / 100) / _ulFreq  - 1;		/* �Զ���װ��ֵ */
	}
	else	/* ����4K��Ƶ�ʣ������Ƶ */
	{
		usPrescaler = 0;					/* ��Ƶ�� = 1 */
		usPeriod = uiTIMxCLK / _ulFreq - 1;	/* �Զ���װ��ֵ */
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
*	�� �� ��: BEEP_InitHard
*	����˵��: ��ʼ��������Ӳ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BEEP_InitHard(void)
{
#ifdef	BEEP_HAVE_POWER		/* ��Դ������ */
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIO��ʱ�� */
	RCC_APB2PeriphClockCmd(GPIO_RCC_BEEP, ENABLE);

	BEEP_DISABLE();

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* �������ģʽ */	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_BEEP;
	GPIO_Init(GPIO_PORT_BEEP, &GPIO_InitStructure);
#else
	;	/* ��Դ������ */
	
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: BEEP_Start
*	����˵��: ������������
*	��    �Σ�_uiFreq : Ƶ�� (Hz)
*			  _usBeepTime : ����ʱ�䣬��λ10ms; 0 ��ʾ������
*			  _usStopTime : ֹͣʱ�䣬��λ10ms; 0 ��ʾ��������
*			 _usCycle : ���д����� 0 ��ʾ��������
*	�� �� ֵ: ��
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
	g_tBeep.ucEnalbe = 1;	/* ������ȫ�ֲ�������ʹ�ܷ�����־ */

	BEEP_ENABLE();			/* ��ʼ���� */
}

/*
*********************************************************************************************************
*	�� �� ��: BEEP_Stop
*	����˵��: ֹͣ��������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BEEP_Stop(void)
{
	g_tBeep.ucEnalbe = 0;

	BEEP_DISABLE();	/* ����������Ʊ�־����ֹͣ����������ֹͣ�����ж����ֿ��� */
}

/*
*********************************************************************************************************
*	�� �� ��: BEEP_KeyTone
*	����˵��: ���Ͱ������� �̶� 1.5KHz
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BEEP_KeyTone(void)
{
	BEEP_Start(1500, 5, 1, 1);	/* ����50ms��ͣ10ms�� 1�� */
}

/*
*********************************************************************************************************
*	�� �� ��: BEEP_Pro
*	����˵��: ÿ��10ms����1�θú��������ڿ��Ʒ������������ú����� bsp_timer.c �б����á�
*	��    �Σ���
*	�� �� ֵ: ��
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
		if (g_tBeep.usStopTime > 0)	/* ��Ϸ��� */
		{
			if (++g_tBeep.usCount >= g_tBeep.usBeepTime)
			{
				BEEP_DISABLE();		/* ֹͣ���� */
				g_tBeep.usCount = 0;
				g_tBeep.ucState = 1;
			}
		}
		else
		{
			;	/* �����κδ����������� */
		}
	}
	else if (g_tBeep.ucState == 1)
	{
		if (++g_tBeep.usCount >= g_tBeep.usStopTime)
		{
			/* ��������ʱ��ֱ������stopֹͣΪֹ */
			if (g_tBeep.usCycle > 0)
			{
				if (++g_tBeep.usCycleCount >= g_tBeep.usCycle)
				{
					/* ѭ����������ֹͣ���� */
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

			BEEP_ENABLE();			/* ��ʼ���� */
		}
	}
}

/***************************** �������ҽ����е�з����޹�˾ ShenYang HengDe Medical Instruments Co.��Ltd. (END OF FILE) *********************************/
