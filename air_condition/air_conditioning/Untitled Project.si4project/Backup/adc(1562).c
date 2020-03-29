
#include "adc.h"


extern ADC_HandleTypeDef hadc1;


//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
unsigned int Get_Adc(uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //ͨ��
    ADC1_ChanConf.Rank=1;                                       //��1�����У�����1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_41CYCLES_5;        //����ʱ��
    //ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //ͨ������
	
    HAL_ADC_Start(&hadc1);                               //����ADC
	
    HAL_ADC_PollForConversion(&hadc1,10);                //��ѯת��
 
//	return (unsigned int)HAL_ADC_GetValue(&hadc1);	        //�������һ��ADC1�������ת�����
}
//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ

adc_io_str adc_io;

void Get_Adc_Average(unsigned char times)
{
	uint32_t temp_val=0,adcBuf[6];
	unsigned char i,t;
	for(t=0;t<6;t++)
	{;			
		adcBuf[t]=0;
	}

	for(i=0;i<times;i++)
	{                               //����ADC
		//Get_Adc( ch);
		for(t=0;t<6;t++)
		{;
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1,0xffff);
			temp_val = HAL_ADC_GetValue(&hadc1); 			
			adcBuf[t]=temp_val + adcBuf[t];
			if(temp_val>5000)
				adcBuf[t] = 0;
		}
	}
		HAL_ADC_Stop(&hadc1);
	for(t=0;t<6;t++)
	{;			
		adcBuf[t]=adcBuf[t]/times;
	}

	for(t=0;t<5;t++)
	{;		
	
		adc_io.adc_result[t]=12*adcBuf[t]/adcBuf[5];//����10��
	}

	#if DEBUG_USER
		printf("adcBuf[%d] is:   %d\n",i,adcBuf[i]);
		#endif
	
	//return &adc_io.adc_result[0];
} 

unsigned int adc_process()
{
    unsigned int fault_status;
	fault_status = 0;
	Get_Adc_Average(100);
	if(adc_io.adc_result[0]>=MAX_TEMPER ||adc_io.adc_result[0]<=MIN_TEMPER)
	{
		adc_io.fault_status = adc_io.fault_status|0xe01;
	}
	else if(adc_io.adc_result[1]>=MAX_TEMPER ||adc_io.adc_result[1]<=MIN_TEMPER)
	{
		adc_io.fault_status = adc_io.fault_status|0xe02;
	}
	else if(adc_io.adc_result[2]>=MAX_TEMPER ||adc_io.adc_result[2]<=MIN_TEMPER)
	{
		adc_io.fault_status =adc_io.fault_status| 0xe04;
	}
	else if(adc_io.adc_result[3]>=MAX_TEMPER ||adc_io.adc_result[3]<=MIN_TEMPER)
	{
		adc_io.fault_status =adc_io.fault_status| 0xe08;
	}
	else if(adc_io.adc_result[4]>=MAX_TEMPER ||adc_io.adc_result[4]<=MIN_TEMPER)
	{
		adc_io.fault_status =adc_io.fault_status| 0xe10;
	}
	else 
		adc_io.fault_status = 0;

	return adc_io.fault_status;

}
