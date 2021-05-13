#include "rtc.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//RTC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//RTC_HandleTypeDef RTC_Handler;  //RTC���
extern RTC_HandleTypeDef hrtc;
//RTCʱ������
//hour,min,sec:Сʱ,����,����
//ampm:@RTC_AM_PM_Definitions:RTC_HOURFORMAT12_AM/RTC_HOURFORMAT12_PM
//����ֵ:SUCEE(1),�ɹ�
//       ERROR(0),�����ʼ��ģʽʧ�� 
HAL_StatusTypeDef RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RTC_TimeStructure.Hours=hour;
	RTC_TimeStructure.Minutes=min;
	RTC_TimeStructure.Seconds=sec;
	RTC_TimeStructure.TimeFormat=ampm;
	RTC_TimeStructure.DayLightSaving=RTC_DAYLIGHTSAVING_NONE;
    RTC_TimeStructure.StoreOperation=RTC_STOREOPERATION_RESET;
	return HAL_RTC_SetTime(&hrtc,&RTC_TimeStructure,RTC_FORMAT_BIN);	
}

//RTC��������
//year,month,date:��(0~99),��(1~12),��(0~31)
//week:����(1~7,0,�Ƿ�!)
//����ֵ:SUCEE(1),�ɹ�
//       ERROR(0),�����ʼ��ģʽʧ�� 
HAL_StatusTypeDef RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	RTC_DateTypeDef RTC_DateStructure;
    
	RTC_DateStructure.Date=date;
	RTC_DateStructure.Month=month;
	RTC_DateStructure.WeekDay=week;
	RTC_DateStructure.Year=year;
	return HAL_RTC_SetDate(&hrtc,&RTC_DateStructure,RTC_FORMAT_BIN);
}

//RTC��ʼ��
//����ֵ:0,��ʼ���ɹ�;
//       2,�����ʼ��ģʽʧ��;
u8 RTC_Init(void)
{      

	
	hrtc.Instance=RTC;
    hrtc.Init.HourFormat=RTC_HOURFORMAT_24;//RTC����Ϊ24Сʱ��ʽ 
    hrtc.Init.AsynchPrediv=0X7F;           //RTC�첽��Ƶϵ��(1~0X7F)
    hrtc.Init.SynchPrediv=0XFF;            //RTCͬ����Ƶϵ��(0~7FFF)   
    hrtc.Init.OutPut=RTC_OUTPUT_DISABLE;     
    hrtc.Init.OutPutPolarity=RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType=RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&hrtc)!=HAL_OK) return 2;
     // HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0X5051);//����Ѿ���ʼ������
    if(HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR0)!=0X5050)//�Ƿ��һ������
    { 
        RTC_Set_Time(13,59,56,RTC_HOURFORMAT12_PM);	        //����ʱ�� ,����ʵ��ʱ���޸�
		RTC_Set_Date(15,12,27,7);		                    //��������
        HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0X5050);//����Ѿ���ʼ������
    }
    return 0;
}



RTC_STRU rtc_usr;

 RTC_DateTypeDef sdatestructure;
 RTC_TimeTypeDef stimestructure;
 RTC_STRU *getRtcStatus(void)
 {
	return &rtc_usr;
 }

void SetWakeUp(uint32_t p)
{
   RTC_HandleTypeDef hrtc;
   unsigned int WakeUpCounter;
   WakeUpCounter = ((u16)(p))<<8+p;
   WakeUpCounter = WakeUpCounter*60;   
   HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, WakeUpCounter, RTC_WAKEUPCLOCK_CK_SPRE_16BITS,0);

}

void  setRtc(unsigned char* p,unsigned char item)
{
	/* USER CODE BEGIN RTC_Init 0 */
	
	  /* USER CODE END RTC_Init 0 */
	if(item <=3)
	{
		RTC_HandleTypeDef hrtc;
		RTC_TimeTypeDef sTime = {0};
        RTC_DateTypeDef sDate = {0};
		sDate.Year = p[0];
		sDate.Year = p[1];
		sDate.Year = p[2];
		sTime.Hours = p[3];
		sTime.Minutes = p[4];
		sTime.Seconds = p[5];

		HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);


	}
}

void RTC_WAKEUP_Init(uint32_t p)
{

	   __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);//���RTC WAKE UP�ı�־
	    HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
        /*rtc_usr.wakeup_period = p[0];
        rtc_usr.wakeup_period = rtc_usr.wakeup_period<<8;
        rtc_usr.wakeup_period = rtc_usr.wakeup_period + p[1];
        rtc_usr.wakeup_period = rtc_usr.wakeup_period*60;*/

//        rtc_usr.wakeup_period = p[0];
//        rtc_usr.wakeup_period = rtc_usr.wakeup_period<<8;
//        rtc_usr.wakeup_period = rtc_usr.wakeup_period + p[1];
//        // rtc_usr.wakeup_period = rtc_usr.wakeup_period*20;
//        rtc_usr.wakeup_period = rtc_usr.wakeup_period*60;

        //HAL_RTCEx_SetWakeUpTimer_IT(&hrtc,  rtc_usr.wakeup_period, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
        SetWakeUp(p);
}

unsigned char *getRTC()
{
    unsigned char p[6],i;
//	RTC_HandleTypeDef hrtc;	
	HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN); 
	p[i++] = sdatestructure.Year;
	p[i++] = sdatestructure.Month;
	p[i++] = sdatestructure.Date;
	p[i++] = stimestructure.Hours;
	p[i++] = stimestructure.Minutes;
	p[i++] = stimestructure.Seconds;
	return p;

}

void RTC_Calibration(unsigned char *p)
{
	if(rtc_usr.calibration == 0)
	{
		rtc_usr.calibration =1;
		RTC_Init();
	}
}