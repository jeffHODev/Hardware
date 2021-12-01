/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名  ：adc.c
 * 描述    ：DMA方式读取ADC值应用函数库         
 * 库版本  ：ST3.0.0
 *
 * 作者    ：fire  QQ: 313303034 
 * 博客    ：firestm32.blog.chinaunix.net
**********************************************************************************/
#include "adc.h"
#include "delay.h" 
#include "system.h"
#include "pid.h"
#include "iic.h"
#include "driveboard.h"
#include "bsp_led.h"

float IC_Temp=0;
float IC_Vdd=0;
float POW_Value=0;
uint16_t ADC_ConvertedValue[SAMPLE_NUM][CHANNEL_NUM];					// ADC采样数据池
const float NTC_Temp_Table10K[Ntc_Num]={ 32.919,31.27,29.715,28.246,26.858,25.547,24.307,23.135,22.026,20.977,
                                      19.987,19.044,18.154,17.31,16.51,15.752,15.034,14.352,13.705,13.09,
                                      12.507,11.953,11.427,10.927,10.452,10,9.57,9.161,8.771,8.401,8.048,
                                      7.712,7.391,7.086,6.795,6.518,6.254,6.001,5.761,5.531,5.311,5.102,
                                      4.902,4.71,4.528,4.353,4.186,4.026,3.874,3.728,3.588,3.454,3.326,
                                      3.203,3.085,2.973,2.865,2.761,2.662,2.567,2.476,2.388,2.304,2.223,
                                      2.146,2.072,2,1.932,1.866,1.803,1.742,1.684,1.627,1.573,1.521,1.471,
                                      1.423,1.377,1.332,1.289,1.248,1.208,1.17,1.133,1.097,1.063,1.03,0.998,
                                      0.968,0.938,0.909,0.882,0.855,0.829,0.805,0.781,0.758,0.735,0.714,0.693,
                                      0.673,0.653,0.635,0.616,0.599,0.582,0.565,0.55,0.534,0.519,0.505,0.491,
                                      0.478,0.465,0.452,0.44,0.428,0.416,0.405,0.395,0.384,0.374,0.364,0.355,0.345,0.337};// 0-126摄氏度


// 20℃下TDS测试值该值为ADC采样过来的值																			
const uint16_t TDS_Test_Vaule[TDS_NUM]={	4075,4062,4057,4033,4011,3992,3969,3943,3928,3907,
																					3890,3871,3847,3829,3813,3785,3773,3755,3746,3722,
																					3699,3664,3635,3630,3623,3607,3589,3573,3564,3540,
																					3521,3511,3503,3485,3460,3457,3442,3421,3395,3392,
																					3378,3363,3344,3312,3310,3290,3267,3256,3237,3248,
																					3219,3201,3169,3150,3133,3117,3103,3088,3077,3065,
																					3053,3037,3034,3021,3010,2972,2943,2926,2888,2881,
																					2854,2823,2810,2797,2778,2758,2744,2722,2710,2701,
																					2694,2682,2674,2664,2657,2642,2630,2620,2601,2588,
																					2579,2570,2562,2556,2544,2536,2526,2520,2512,2510,
																					2491,2486,2472,2464,2458,2453,2444,2438,2422,2415,
																					2403,2393,2385,2377,2365,2356,2349,2344,2335,2329,
																					2319,2311,2305,2298,2290,2282,2275,2269,2263,2256,
																					2248,2239,2231,2224,2216,2206,2200,2192,2184,2177,
																					2168,2164,2157,2146,2141,2135,2126,2120,2114,2107,
																					2103,2095,2087,2078,2070,2062,2056,2048,2042,2033,
																					2028,2021,2014,2007,1998,1993,1987,1979,1972,1968,
																					1959,1951,1945,1939,1931,1924,1920,1917,1909,1904,
																					1894,1888,1883,1876,1868,1864,1858,1846,1841,1836,
																					1829,1822,1819,1814,1808,1804,1800,1794,1787,1782,
																					1777,1773,1766,1761,1755,1747,1744,1736,1732,1727,
																					1724,1718,1712,1706,1700,1697,1692,1686,1682,1674,
																					1672,1668,1666,1662,1655};

// 20℃下TDS值该值为TDS标定值
const uint16_t TDS_Vaule[TDS_NUM]=			{	   0,		1,	 3,		4,	 5,	  7,	 8,	  9,	11,	 12,
																						13,	 15,	16,	 17,  19,  20,  21,  23,  24,  25,
																						26,  28,  29,  30,  31,  33,  34,  35,  36,  37,
																						38,  40,  41,  42,  43,  44,  46,  47,  48,  49,
																						50,  51,  52,  53,  54,  56,  57,  58,  59,  61,
																						62,  65,  66,  67,  69,  70,  72,  73,  75,  76,
																						77,  78,  79,  80,  81,  82,  85,  86,  89,  90,
																						92,  93,  95,  96,  99, 101, 102, 104, 105, 106,
																					 108, 109, 110, 111, 112, 113, 114, 117, 119, 120,
																					 121, 122, 123, 124, 126, 127, 128, 129, 130, 131,
																					 132, 133, 134, 135, 136, 137, 138, 139, 140, 141,
																					 144, 145, 146, 148, 149, 150, 152, 153, 154, 155,
																					 156, 157, 158, 159, 161, 162, 163, 164, 165, 166,
																				   167, 169, 170, 171, 174, 175, 176, 177, 178, 179,
																					 180, 182, 183, 184, 185, 187, 188, 189, 190, 191,
																					 192, 194, 196, 198, 199, 200, 201, 203, 205, 206,
																					 207, 209, 210, 211, 212, 214, 215, 216, 217, 219,
																					 221, 223, 224, 225, 227, 229, 230, 231, 233, 235,
																					 236, 238, 239, 240, 242, 243, 244, 248, 249, 250,
																					 252, 253, 254, 255, 257, 259, 260, 262, 263, 264,
																					 266, 267, 269, 270, 271, 273, 275, 277, 278, 279,
																				   280, 282, 285, 286, 287, 289, 290, 291, 292, 295,
																					 296,297,298,299,300};																			 

/* 函数名：ADC1_Mode_Config
 * 描述  ：配置ADC1的工作模式为MDA模式
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
void ADC1_Mode_Config(void)
{
  GPIO_InitTypeDef 	GPIO_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure;
	ADC_InitTypeDef 	ADC_InitStructure;

  /* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* ADC1 DeInit */  
	ADC_DeInit(ADC1);
  /* Configure ADC Channel11 and channel10 as analog input */
  /* Configure PB.00  as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);						
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  /* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = CHANNEL_NUM;
  ADC_Init(ADC1, &ADC_InitStructure);
  /* ADC1 regular channel11 configuration */ 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_TempSensorVrefintCmd(ENABLE);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_13Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_13Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_13Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_13Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16,6, ADC_SampleTime_13Cycles5);		// TEMP
	ADC_RegularChannelConfig(ADC1, ADC_Channel_17,7, ADC_SampleTime_13Cycles5);		// VREF
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	// 配置DMA寄存器
  /* DMA1 clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* DMA1 Channel1 Config */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = SAMPLE_NUM*CHANNEL_NUM;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  /* DMA1 Channel1 enable */
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

/* 函数名：ADC1_Mode_Config
 * 描述  ：配置ADC1的工作模式为MDA模式
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
void ADC2_Mode_Config(void)
{
  GPIO_InitTypeDef 	GPIO_InitStructure;
	ADC_InitTypeDef 	ADC_InitStructure;

  /* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  /* ADC2 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	/* ADC2 DeInit */  
	ADC_DeInit(ADC2);
  /* Configure ADC Channel11 and channel10 as analog input */
  /* Configure PB.00  as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);						
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  /* Configure the ADC2 in continuous mode withe a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC2, &ADC_InitStructure);
  /* ADC2 regular channel11 configuration */ 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_TempSensorVrefintCmd(ENABLE);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_13Cycles5);
  /* Enable ADC2 */
  ADC_Cmd(ADC2, ENABLE);
  /* Enable ADC2 reset calibaration register */   
  ADC_ResetCalibration(ADC2);
  /* Check the end of ADC2 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC2));
  /* Start ADC2 calibaration */
  ADC_StartCalibration(ADC2);
  /* Check the end of ADC2 calibration */
  while(ADC_GetCalibrationStatus(ADC2));     
  /* Start ADC2 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC2, DISABLE);
}

/***********************************************************************************
 * 函数名：uint16_t ReadADCAverageValue(uint8_t Channel)
 * 描述  ：ReadADCAverageValue
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
float ReadADCAverageValue(uint8_t channel)
{
 uint8_t i;
 float sum = 0;
 for(i=0; i<SAMPLE_NUM; i++)
 {
  sum+=ADC_ConvertedValue[i][channel];
 }
 return (sum/SAMPLE_NUM);
}


/***********************************************************************************
 * 函数名：void Get_Now_Value(void)
 * 描述  ：获取当前功率
 * 输入  ：无
 * 输出  ：无  ADC采样周期150us
************************************************************************************/
void Get_Now_Value(void)
{
	static double  averys=2048,avercs=2048,averhm=2048,avercur=3120,avertem=1450,averpow=1600,avertds=2800;
	static uint8_t curflage=0;									//
	static uint16_t loopnum=0,orpvalue=0;				//
	static float curzero=0,tempvalue=0;					//
	float value,res;														//
	uint16_t orptemp,tdsvalue;									//
  uint16_t j=0;
	
	// 循环周期计数
	loopnum++;																	// 循环周期计数
	// 获取AD
	// value=ADC_GetConversionValue(ADC2);			// 获取TDS
	value=ReadADCAverageValue(6);								// 获取内部温度
	avertem=avertem-avertem/FIR_NUM+value/FIR_NUM;
	value=ReadADCAverageValue(0);								// 厚膜温度
	averhm=averhm-averhm/5+value/5;
	value=ReadADCAverageValue(3);								// 原水温度
	averys=averys-averys/FIR_NUM+value/FIR_NUM;
	value=ReadADCAverageValue(1);								// 出水温度
	avercs=avercs-avercs/FIR_NUM+value/FIR_NUM;
	value=ReadADCAverageValue(2);								// 电解电流
	avercur=avercur-avercur/FIR_NUM+value/FIR_NUM;
	value=ReadADCAverageValue(5);								// 供电电压
	averpow=averpow-averpow/FIR_NUM+value/FIR_NUM;
	// 计算电解电流
	value=(avercur*PRE_VALUE > 2.5 ? (avercur*PRE_VALUE-2.5) : (2.5-avercur*PRE_VALUE))/0.000185;
	if(curflage==0&&Init_Falge==1)
	{
		curzero=value;curflage=1;
	}
	Now_Elecr_Current=value-curzero > 0 ? (value-curzero) : 0;
	value=(float)(Now_Elecr_Current-Set_Current_Table[Set_PH_Num])/(float)(Set_Current_Table[Set_PH_Num])*Set_ORP_Table[Set_PH_Num];
	orptemp=Set_ORP_Table[Set_PH_Num]+value*0.1;
	// if(Set_PH_Num==0
	if(orptemp!=orpvalue&&ELE_Read==1&&loopnum%50==0)			// 电解模式下  最快每500ms变化一次
	{
		orpvalue=orptemp;
		Display_Ramb[0]=orpvalue/100;
		Display_Ramb[1]=(orpvalue%100)/10;
		Display_Ramb[2]=orpvalue%10;	
	}
	// 计算出水温度值
//	res=(10.0*avercs)/(4096.0-avercs);
//	for(j=0;j<Ntc_Num;j++)
//	{
//		 if((res<=NTC_Temp_Table10K[j])&&(res>=NTC_Temp_Table10K[j+1])) break;
//	}
//	Cswater_Temp=(uint8_t)(j+(NTC_Temp_Table10K[j]-res)/(NTC_Temp_Table10K[j]-NTC_Temp_Table10K[j+1])+0.5);	
	
	// 计算厚膜温度值
	res=(10.0*averhm)/(4096.0-averhm);
	for(j=0;j<Ntc_Num;j++)
	{
		 if((res<=NTC_Temp_Table10K[j])&&(res>=NTC_Temp_Table10K[j+1])) break;
	}
	Hmwater_Temp=(j+(NTC_Temp_Table10K[j]-res)/(NTC_Temp_Table10K[j]-NTC_Temp_Table10K[j+1]));	
	// 估算出水温度值	{0,65,80,90}--{0,45,65,80}
	if(Hmwater_Temp<=65) Cswater_Temp=Hmwater_Temp/65.0*(45.0);													// 显示0-45℃
	else if(Hmwater_Temp<=80) Cswater_Temp=(45)+(Hmwater_Temp-65.0)/(80.0-65.0)*(20.0);	// 显示45-65℃
	else Cswater_Temp=(65)+(Hmwater_Temp-80.0)/(90.0-80.0)*(15.0);											// 显示80-100℃
	if(Cswater_Temp>90)	Cswater_Temp=90;	
//	Cswater_Temp=Hmwater_Temp;

	// 计算原水温度值
	res=(10.0*averys)/(4096.0-averys);
	for(j=0;j<Ntc_Num;j++)
	{
		 if((res<=NTC_Temp_Table10K[j])&&(res>=NTC_Temp_Table10K[j+1])) break;
	}
	value=(j+(NTC_Temp_Table10K[j]-res)/(NTC_Temp_Table10K[j]-NTC_Temp_Table10K[j+1]));
	if(tempvalue>value)
	{
		if(tempvalue-value>0.5) tempvalue=value;
	}
	else
	{
		if(value-tempvalue>0.5) tempvalue=value;
	}
	// 2019-11-28 修改温度传感器 保证在5-38度之间
	if(tempvalue<3)  tempvalue=5;
	if(tempvalue>35) tempvalue=35;
	// 2019-11-28 修改温度传感器 保证在5-38度之间
	Yswater_Temp=(uint8_t)tempvalue;
	Set_Temp_Table[0]=Yswater_Temp;
	// TDS  TDS  TDS
	// TDS 脉冲及ADC (13.5+12.5)*8*5=1040采样周期 合计74.2us 采样延时100us
	// TDS 脉冲 
	if(Tds_Test_Flage==1)												// 工作模式 
	{
		TDS_P(1);TDS_N(0);												// 开始TDS脉冲采样
		delay_us(5);
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);
		while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));
		value=ADC_GetConversionValue(ADC2);
		ADC_SoftwareStartConvCmd(ADC2,DISABLE);  
		TDS_P(0);TDS_N(0);												// 电极倒极/
		delay_us(10);								
		TDS_P(0);TDS_N(1);												// 电极倒极
		delay_us(7);	
		TDS_P(0);TDS_N(0);												// 电极倒极/	
		// 计算部分
		avertds=avertds-avertds/FIR_NUM+value/FIR_NUM;		
		if(Yswater_Tds>MAX_TDS_VALUE)							// 原水TDS异常
		{
			LED_TDS=1;															// 报警
			ELE_ConP(0);														// 链接到正极
			ELE_ConN(0);														// 链接到负极	
		}
		else																			// 
		{
			LED_TDS=0;															// 解除报警
		}
		// 根据温度计算TDS温度补偿系数
		if(Yswater_Temp>20)												// 计算温度补偿
		{
			for(j=0;j<Yswater_Temp-20;j++)
			{
				tdsvalue=avertds/TDS_TEMP_FACTOR;
			}
		}
		else if(Yswater_Temp<20)
		{
			for(j=0;j<20-Yswater_Temp;j++)
			{
				tdsvalue=avertds*TDS_TEMP_FACTOR;
			}	
		}
		else tdsvalue=avertds;
		// 计算原水TDS值 y=3E-05x2 - 0.2618x + 530.42
		if(tdsvalue>=TDS_Test_Vaule[0])
		{
			Yswater_Tds=TDS_Vaule[0];
		}
		else
		{
			for(j=0;j<TDS_NUM-1;j++)
			{
				if((tdsvalue>=TDS_Test_Vaule[j+1])&&(tdsvalue<TDS_Test_Vaule[j]))
				{
					break;
				}
			}	
			if(j>=TDS_NUM) j=TDS_NUM-2;
			Yswater_Tds=TDS_Vaule[j]+(TDS_Vaule[j+1]-TDS_Vaule[j])*((float)(tdsvalue-TDS_Test_Vaule[j+1])/(float)(TDS_Test_Vaule[j]-TDS_Test_Vaule[j+1]));
		}
	}	
	// 执行PID调节
	if(loopnum%10==0)							// 每100ms秒执行一次
	{
		Cur_PID_Run();							// 
		Motor_PID_Run();						// 
	}
	// 重新初始化ADC1
	ADC1_Mode_Config();						// 
}


/******************* (C) COPYRIGHT 2011 野火嵌入式开发工作室 *****END OF FILE****/

