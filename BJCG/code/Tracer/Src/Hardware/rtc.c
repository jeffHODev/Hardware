#include "rtc.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//RTC驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//RTC_HandleTypeDef RTC_Handler;  //RTC句柄
extern RTC_HandleTypeDef hrtc;
//RTC时间设置
//hour,min,sec:小时,分钟,秒钟
//ampm:@RTC_AM_PM_Definitions:RTC_HOURFORMAT12_AM/RTC_HOURFORMAT12_PM
//返回值:SUCEE(1),成功
//       ERROR(0),进入初始化模式失败 
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

//RTC日期设置
//year,month,date:年(0~99),月(1~12),日(0~31)
//week:星期(1~7,0,非法!)
//返回值:SUCEE(1),成功
//       ERROR(0),进入初始化模式失败 
HAL_StatusTypeDef RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	RTC_DateTypeDef RTC_DateStructure;
    
	RTC_DateStructure.Date=date;
	RTC_DateStructure.Month=month;
	RTC_DateStructure.WeekDay=week;
	RTC_DateStructure.Year=year;
	return HAL_RTC_SetDate(&hrtc,&RTC_DateStructure,RTC_FORMAT_BIN);
}

//RTC初始化
//返回值:0,初始化成功;
//       2,进入初始化模式失败;
u8 RTC_Init(void)
{      

	
	  /*##-1- Configure the RTC peripheral #######################################*/
	  /* Configure RTC prescaler and RTC data registers */
	  /* RTC configured as follows:
		  - Hour Format    = Format 24
		  - Asynch Prediv  = Value according to source clock
		  - Synch Prediv   = Value according to source clock
		  - OutPut		   = Output Disable
		  - OutPutPolarity = High Polarity
		  - OutPutType	   = Open Drain */

#define WAKEUP_TIMER_ENABLE 0x32F2	

	

	  
	  /*##-4- Write 'wakeup timer enabled' tag in RTC Backup data Register 1 #######*/
	//	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, WAKEUP_TIMER_ENABLE);
	
	hrtc.Instance=RTC;
    hrtc.Init.HourFormat=RTC_HOURFORMAT_24;//RTC设置为24小时格式 
    hrtc.Init.AsynchPrediv=0X7F;           //RTC异步分频系数(1~0X7F)
    hrtc.Init.SynchPrediv=0XFF;            //RTC同步分频系数(0~7FFF)   
    hrtc.Init.OutPut=RTC_OUTPUT_DISABLE;     
    hrtc.Init.OutPutPolarity=RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType=RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&hrtc)!=HAL_OK) return 2;

	 /* Calendar ultra-low power mode */
	 if (HAL_RTCEx_SetLowPowerCalib(&hrtc, RTC_LPCAL_SET) != HAL_OK)
	 {
	   Error_Handler();
	 }
	 
	 /*##-2- Check if data stored in BackUp register1: Wakeup timer enable #######*/
	 /* Read the Back Up Register 1 Data */
	 if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) == WAKEUP_TIMER_ENABLE)
	 {
	   /* if the wakeup timer is enabled then deactivate it to disable the wakeup timer interrupt */
	   if(HAL_RTCEx_DeactivateWakeUpTimer(&hrtc) != HAL_OK)
	   {
		 /* Initialization Error */
		 Error_Handler(); 
	   }
	 }
	

     // HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0X5051);//标记已经初始化过了
   // if(HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR0)!=0X5050)//是否第一次配置
    { 
    //    RTC_Set_Time(13,59,56,RTC_HOURFORMAT12_PM);	        //设置时间 ,根据实际时间修改
	//	RTC_Set_Date(15,12,27,7);		                    //设置日期
    //    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0X5050);//标记已经初始化过了
    }
    return 0;
}

u8 RTC_InitSetting(unsigned char *p)
{      

	
	hrtc.Instance=RTC;
    hrtc.Init.HourFormat=RTC_HOURFORMAT_24;//RTC设置为24小时格式 
    hrtc.Init.AsynchPrediv=0X7F;           //RTC异步分频系数(1~0X7F)
    hrtc.Init.SynchPrediv=0XFF;            //RTC同步分频系数(0~7FFF)   
    hrtc.Init.OutPut=RTC_OUTPUT_DISABLE;     
    hrtc.Init.OutPutPolarity=RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType=RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&hrtc)!=HAL_OK) return 2;
     // HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0X5051);//标记已经初始化过了
    //if(HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR0)!=0X5050)//是否第一次配置
    { 
        RTC_Set_Time(p[3],p[4],p[5],RTC_HOURFORMAT12_PM);	        //设置时间 ,根据实际时间修改
		RTC_Set_Date(p[0],p[1],p[2],7);		                    //设置日期
        HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0X5050);//标记已经初始化过了
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
  // RTC_HandleTypeDef hrtc;
   unsigned int WakeUpCounter;
   WakeUpCounter = (u16)(p);
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

    #if 0
	   __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);//清除RTC WAKE UP的标志
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
	#else if 1
	//HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
	if(RTC_Init()==0)	 
	SetWakeUp(p);
	#endif
	
}
unsigned char p[6];
RTC_TimeTypeDef *getRTCTime()
{
	HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN);  
	

	return &stimestructure;
}
RTC_DateTypeDef *getRTCDATE()
{
	HAL_RTC_GetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN); 
	return &sdatestructure;
}

void RTC_Calibration(unsigned char *p)
{
	//if(rtc_usr.calibration == 0)
	{
	    unsigned char i,j;
		unsigned char pbuff[6];
		unsigned char len;
		len = strlen(p);
		for(j=0;j<len;j++)
		{
			if(p[j]=='+'&&p[j+4]=='K')
				break;
		}
		j = j +8;
		i = 0;
	    p[i++] = p[j++]-0x30;//yearH
	    p[i++] = p[j++]-0x30;//yearL
		j++;               //  '/'
	    p[i++] = p[j++]-0x30; //monthH
	    p[i++] = p[j++]-0x30;//monthL
		j++;               // '/'
	    p[i++] = p[j++]-0x30;//date H
	    p[i++] = p[j++]-0x30;//date L
        j++;               // 'L'
	    p[i++] = p[j++]-0x30;//hours H
	    p[i++] = p[j++]-0x30;//hours L
        j++;		 //':'
 	    p[i++] = p[j++]-0x30;//minute H
	    p[i++] = p[j++]-0x30;//minute L
        j++;		 //':'
	    p[i++] = p[j++]-0x30;//seconds H
	    p[i++] = p[j++]-0x30;//seconds L
        j++;		 //'+'
        i = 0;j = 0;
		pbuff[j++] = p[i]*10+p[i+1];i = i+2;//year
		pbuff[j++] = p[i]*10+p[i+1];i = i+2;//month
		pbuff[j++] = p[i]*10+p[i+1];i = i+2;//date
		pbuff[j++] = p[i]*10+p[i+1];i = i+2;//hours
		pbuff[j++] = p[i]*10+p[i+1];i = i+2;//minutes
		pbuff[j++] = p[i]*10+p[i+1];//seconds		
		rtc_usr.calibration =1;
		RTC_InitSetting(pbuff);
	}
}
