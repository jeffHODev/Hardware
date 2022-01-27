#include "sensor.h"
#include "ADS129x.h"
#include "spo.h"
#include "bsp.h"
#include "table.h"

sensors_stru sensors_usr;
uint16_t find_from_ntc_table( uint16_t adcv )
{
  uint16_t i,len;
  len = 205;
  for( i = 0; i < len; i ++ ) // 200�����Լ����Ԫ����Ŀ����
  if( res_table[i] <=adcv )break;
  return i;
}

void temperature_proc()
{
    float temp;
	uint16_t res_index;
	if(getAdcBuf()->adc_value[1]>=getAdcBuf()->adc_value[0])
	{
	
		sensors_usr.ta =getAdcBuf()->adc_value[0]*3.3/4095;
		sensors_usr.tb =getAdcBuf()->adc_value[1]*3.3/4095;
		sensors_usr.vol=sensors_usr.tb-sensors_usr.ta;
	    sensors_usr.res_cal =(sensors_usr.vol)*560;//sensors_usr.ta/1000 ;
     // temp = getAdcBuf()->adc_value[1]*300/getAdcBuf()->adc_value[0];
	  sensors_usr.res = sensors_usr.res +sensors_usr.res_cal/FIR_NUM-sensors_usr.res/FIR_NUM;
	  res_index = find_from_ntc_table(sensors_usr.res );

      if(res_table[res_index]<temp)
      	{
			if(res_index>0)
			{
				sensors_usr.temperature = (temp_table[res_index]+temp_table[res_index-1])/2.0;

			}
			else
			{
			sensors_usr.temperature = temp_table[0];

			}
	  }
	  else
	  {
	  	//sensors_usr.temperature = temp_table[0];
     sensors_usr.temperature = (temp_table[res_index]+temp_table[res_index+1])/2.0;
	  }
	 
	  
	}

}
/*adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_10, ADC_SAMPLETIME_15);//ta
adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_11, ADC_SAMPLETIME_15);//tb
adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_13, ADC_SAMPLETIME_15);//vbat
adc_regular_channel_config(ADC0, 3, ADC_CHANNEL_14, ADC_SAMPLETIME_15);//spo
adc_regular_channel_config(ADC0, 4, ADC_CHANNEL_15, ADC_SAMPLETIME_15);//br
adc_regular_channel_config(ADC0, 5, ADC_CHANNEL_4, ADC_SAMPLETIME_15);//hea*/

void br_proc()
{
      uint16_t temp;
      temp = getAdcBuf()->adc_value[4];
	  temp = temp*3.3/4095;
	  sensors_usr.br = sensors_usr.br +temp/FIR_NUM-sensors_usr.br/FIR_NUM;
}
void vbat_proc()
{
    uint16_t temp;
      temp = getAdcBuf()->adc_value[2];
	  temp =temp*3.3/4095;
	  temp = temp*10/3; 
	  sensors_usr.vbat = sensors_usr.vbat +temp/FIR_NUM-sensors_usr.vbat/FIR_NUM;
}
sensors_stru *getSensor(void)
{
	return &sensors_usr;
}
void key_proc(void)
{
    static uint32_t key_tick;
	
	key_tick = HAL_GetTick();
    while(gpio_input_bit_get(KEY1_GPIO_Port, KEY1_Pin)==1)
    {
		if((HAL_GetTick()-key_tick)>=4000)
		{
			sensors_usr.key_status = 2;
			break;
		}
		else
		{
			sensors_usr.key_status = 1;

		}
	}
	
	
}
void sensor_proc(void)
{
	if(*getstate()==SEND_BULE)
	{
	    temperature_proc();
	    br_proc();
	    vbat_proc();
	 	spo_proc();


	}
	else
	{
		
	}
}

