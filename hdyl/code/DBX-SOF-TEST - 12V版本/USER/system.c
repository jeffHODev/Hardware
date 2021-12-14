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
#include "usart1.h"
#include "system.h"
#include "delay.h"
#include "adc.h"

/*****************************��������*******************************************/
__IO uint8_t Work_Mode=0;							// ����ģʽ
__IO uint8_t Chuang_Mode=0;						// ������� ���� ˫��
__IO float UP_Temp=0;									// �ϵ��¶�
__IO float Down_Temp=0;								// �µ��¶�
__IO float UP_Power=0;								// �ϵ湦��
__IO float Down_Power=0;							// �µ湦��
__IO float UP_Res=0;									// �ϵ����
__IO float Down_Res=0;								// �µ����
__IO float UP_Value=0;								// �ϵ��ѹ
__IO float Down_Value=0;							// �µ��ѹ
__IO float Now_Temp=0;								// ��ǰ�¶�
__IO float IN_Value=12;								// �����ѹ
__IO float Pref_Value=0;							// ��׼��ѹ


/*************	���س�������	**************/
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
 * ��������void Get_Now_Temp(void)
 * ����  ����ȡ�¶�����
 * ����  ����
 * ���  ����
************************************************************************************/
void Get_Now_Temp(void)
{
	static float avesumu,avesumd,avesumt;
	float temp,res;
	uint8_t j;
	
	temp=ReadADCAverageValue(0);	// �ۼƵ�ǰ�¶�
	avesumt=(avesumt-avesumt/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	temp=ReadADCAverageValue(1);	// �ۼ��µ��¶�	
	avesumd=(avesumd-avesumd/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	temp=ReadADCAverageValue(2);	// �ۼ��ϵ��¶�	
	avesumu=(avesumu-avesumu/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	// �����ϵ��¶�ƽ��ֵ
	// �����ϵ��¶�
	res=(10.0*avesumu)/(4095.0-avesumu);	// ��ֵ����
	for(j=0;j<Ntc_Num;j++)								// ����ֶȱ�
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
	// �����µ��¶�ƽ��ֵ
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
	// ���㵱ǰ�¶�
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
 * ��������void Get_Now_Value(void)
 * ����  ����ȡ��ǰ����
 * ����  ����
 * ���  ����
************************************************************************************/
void Get_Now_Value(void)
{
	static double avesumu,avesumd,avesumr,avesumvu,avesumvd,avesumv;
	float cup,cdow,temp,datau,datad;
	static float diffup=0,diffdow=0;
	// �ۼ�
	temp=ReadADCAverageValue(3);			// �ϵ��ѹ
	avesumvu=(avesumvu-avesumvu/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	temp=ReadADCAverageValue(4);			// �µ��ѹ
	avesumvd=(avesumvd-avesumvd/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	temp=ReadADCAverageValue(5);			// ��Դ��ѹ 1/10
	avesumv=(avesumv-avesumv/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	temp=ReadADCAverageValue(6);			// ��׼Դ��ѹ
	avesumr=(avesumr-avesumr/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	temp=ReadADCAverageValue(7);			// �ۼ��µ湦��
	avesumd=(avesumd-avesumd/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	temp=ReadADCAverageValue(8);			// �ۼ��ϵ湦��
	avesumu=(avesumu-avesumu/AVER_SAMPLE_NUM)+temp/AVER_SAMPLE_NUM;
	// ����ƽ������
	Pref_Value=2.5/avesumr;										// ��û�׼��ѹϵ��
	IN_Value=avesumv*Pref_Value*10;						// ��õ�Դ�����ѹ
	datau=avesumu*Pref_Value;									// �ϵ����
	datad=avesumd*Pref_Value;									// �µ����
	UP_Value=avesumvu*Pref_Value*6.1;					// �ϵ��ѹ
	Down_Value=avesumvd*Pref_Value*6.1;				// �µ��ѹ
	// �����ֵ����
	// �����ع����Զ�У������׼ֵ
	cup=(datau-(VDD_VALUE/2))/0.18;						// �ϵ����
	if(Work_Mode==0&&UP_Temp==0)							// �ϵ�δ����
	{
		if(cup<0.5) diffup=cup;									// �ϵ������ֵ
	}
	cup=cup-diffup;
	// �����ع���
	cdow=(datad-(VDD_VALUE/2))/0.18;					// �µ����
	if(Work_Mode==0&&Down_Temp==0)						// �µ�δ����
	{
		if(cdow<0.5) diffdow=cdow;							// �µ������ֵ
	}
	cdow=cdow-diffdow;
	// ����湦��
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


/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/
