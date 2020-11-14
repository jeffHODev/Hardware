#ifndef __RTC_H
#define __RTC_H
#include "main.h"
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
typedef struct{
 unsigned char RtcWakeUP;
 
}RTC_STRU;
unsigned char *getRTC(void);
void  setRtc(unsigned char* p,unsigned char item);
void SetWakeUp(unsigned char *p);
RTC_STRU *getRtcStatus(void);

extern RTC_HandleTypeDef RTC_Handler;  //RTC���
    
u8 RTC_Init(void);              //RTC��ʼ��
HAL_StatusTypeDef RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);      //RTCʱ������
HAL_StatusTypeDef RTC_Set_Date(u8 year,u8 month,u8 date,u8 week);	//RTC��������
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec); //��������ʱ��(����������,24Сʱ��)
void RTC_Set_WakeUp(u32 wksel,u16 cnt);             //�����Ի��Ѷ�ʱ������
#endif
