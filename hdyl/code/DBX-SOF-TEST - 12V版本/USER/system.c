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
#include "usart1.h"
#include "system.h"
#include "delay.h"
#include "adc.h"

/*****************************变量声明*******************************************/
__IO uint8_t Work_Mode=0;							// 工作模式
__IO uint8_t Chuang_Mode=0;						// 床垫类别 单人 双人
__IO float UP_Temp=0;									// 上垫温度
__IO float Down_Temp=0;								// 下垫温度
__IO float UP_Power=0;								// 上垫功率
__IO float Down_Power=0;							// 下垫功率
__IO float UP_Res=0;									// 上垫电阻
__IO float Down_Res=0;								// 下垫电阻
__IO float UP_Value=0;								// 上垫电压
__IO float Down_Value=0;							// 下垫电压
__IO float Now_Temp=0;								// 当前温度
__IO float IN_Value=12;								// 输入电压
__IO float Pref_Value=0;							// 基准电压


/*************	本地常量声明	**************/
const float NTC_Temp_Table[Ntc_Num]={32.919,31.27,29.715,28.246,26.858,25.547,24.307,23.135,22.026,20.977,
									19.987,19.044,18.154,17.31,16.51,15.752,15.034,14.352,13.705,13.09,
									12.507,11.953,11.427,10.927,10.452,10,9.57,9.161,8.771,8.401,8.048,
									7.712,7.391,7.086,6.795,6.518,6.254,6.001,5.761,5.531,5.311,5.102,
									4.902,4.71,4.528,4.353,4.186,4.026,3.874,3.728,3.588,3.454,3.326,
									3.203,3.085,2.973,2.865,2.761,2.662,2.567,2.476,2.388,2.304,2.223,
									2.146,2.072,2,1.932,1.866,1.803,1.742,1.684,1.627,1.573,1.521,1.471,
									1.423,1.377,1.332,1.289,1.248,1.208,1.17,1.133,1.097,1.063,1.03,0.998,
									0.968,0.938,0.909,0.882,0.855,0.829,0.805,0.781,0.758,0.735,0.714,0.693,
									0.673,0.653,0.635,0.616,0.599,0.582,0.565,0.55,0.534,0.519,0.505,0.491,
									0.478,0.465,0.452,0.44,0.428,0.416,0.405,0.395,0.384,0.374,0.364,0.355,0.345,0.337};


/***********************************************************************************
 * 函数名：void Get_Now_Temp(void)
 * 描述  ：获取温度数据
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
void Get_Now_Temp(void)
{
	static float avesumu,avesumd,avesumt;
	float temp,res;
	uint8_t j;
	
	temp=ReadADCAverageValue(0);	// 累计当前温度
	avesumt=(avesumt-avesumt/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	temp=ReadADCAverageValue(1);	// 累计下垫温度	
	avesumd=(avesumd-avesumd/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	temp=ReadADCAverageValue(2);	// 累计上垫温度	
	avesumu=(avesumu-avesumu/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	// 计算上垫温度平均值
	// 计算上垫温度
	res=(10.0*avesumu)/(4095.0-avesumu);	// 阻值计算
	for(j=0;j<Ntc_Num;j++)								// 电阻分度表
	{
		if((res<=NTC_Temp_Table[j])&&(res>=NTC_Temp_Table[j+1])) break;
	}
	if(j<Ntc_Num)
	{
		UP_Temp=(uint8_t)(j+(NTC_Temp_Table[j]-res)/(NTC_Temp_Table[j]-NTC_Temp_Table[j+1])+0.5);	
	}
	else
	{
		UP_Temp=0;
	}
	// 计算下垫温度平均值
	res=(10.0*avesumd)/(4095.0-avesumd);
	for(j=0;j<Ntc_Num;j++)
	{
		if((res<=NTC_Temp_Table[j])&&(res>=NTC_Temp_Table[j+1])) break;
	}
	if(j<Ntc_Num)
	{		
		Down_Temp=(uint8_t)(j+(NTC_Temp_Table[j]-res)/(NTC_Temp_Table[j]-NTC_Temp_Table[j+1])+0.5);	
	}
	else
	{
		Down_Temp=0;
	}
	// 计算当前温度
	res=(10.0*avesumt)/(4095.0-avesumt);
	for(j=0;j<Ntc_Num;j++)
	{
		if((res<=NTC_Temp_Table[j])&&(res>=NTC_Temp_Table[j+1])) break;
	}
	if(j<Ntc_Num)
	{				
		Now_Temp=(uint8_t)(j+(NTC_Temp_Table[j]-res)/(NTC_Temp_Table[j]-NTC_Temp_Table[j+1])+0.5);		
	}
	else
	{
		Now_Temp=0;
	}
}		

/***********************************************************************************
 * 函数名：void Get_Now_Value(void)
 * 描述  ：获取当前功率
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
void Get_Now_Value(void)
{
	static double avesumu,avesumd,avesumr,avesumvu,avesumvd,avesumv;
	float cup,cdow,temp,datau,datad;
	static float diffup=0,diffdow=0;
	// 累计
	temp=ReadADCAverageValue(3);			// 上垫电压
	avesumvu=(avesumvu-avesumvu/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	temp=ReadADCAverageValue(4);			// 下垫电压
	avesumvd=(avesumvd-avesumvd/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	temp=ReadADCAverageValue(5);			// 电源电压 1/10
	avesumv=(avesumv-avesumv/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	temp=ReadADCAverageValue(6);			// 基准源电压
	avesumr=(avesumr-avesumr/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	temp=ReadADCAverageValue(7);			// 累计下垫功率
	avesumd=(avesumd-avesumd/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	temp=ReadADCAverageValue(8);			// 累计上垫功率
	avesumu=(avesumu-avesumu/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	// 计算平均功率
	Pref_Value=2.5/avesumr;										// 获得基准电压系数
	IN_Value=avesumv*Pref_Value*10;						// 获得电源输入电压
	datau=avesumu*Pref_Value;									// 上垫电流
	datad=avesumd*Pref_Value;									// 下垫电流
	UP_Value=avesumvu*Pref_Value*6.1;					// 上垫电压
	Down_Value=avesumvd*Pref_Value*6.1;				// 下垫电压
	// 计算差值功率
	// 检测空载功率自动校正零点基准值
	cup=(datau-(VDD_VALUE/2))/0.18;						// 上垫电流
	if(Work_Mode==0&&UP_Temp==0)							// 上垫未链接
	{
		if(cup<0.5) diffup=cup;									// 上垫电流差值
	}
	cup=cup-diffup;
	// 检测空载功率
	cdow=(datad-(VDD_VALUE/2))/0.18;					// 下垫电阻
	if(Work_Mode==0&&Down_Temp==0)						// 下垫未链接
	{
		if(cdow<0.5) diffdow=cdow;							// 下垫电流差值
	}
	cdow=cdow-diffdow;
	// 计算垫功率
	if(Work_Mode!=4)
	{
		UP_Res=IN_Value/cup;
		UP_Power = 576/UP_Res;
	}
	else
	{
		UP_Res=0;
		UP_Power=0;		
	}
	if(Work_Mode!=4)
	{
		Down_Res=IN_Value/cdow;		
		Down_Power=576/Down_Res;
	}
	else
	{
		Down_Res=0;		
		Down_Power=0;		
	}	
UP_Power = (uint32_t) UP_Power;
Down_Power = (uint32_t) Down_Power;	
}


/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/
