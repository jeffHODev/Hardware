/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���  ��main.c
 * ����    ����c���е�printf()����ʵ�ֵ�����1(USART1)���������ǾͿ�����printf()��
 *           ������Ϣͨ�����ڴ�ӡ�������ϡ�         
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.0.0
 *
 * ����    ��fire  QQ: 313303034 
 * ����    ��firestm32.blog.chinaunix.net
**********************************************************************************/

#include "stm32f10x.h"
#include "system.h"
#include "usart3.h"
#include "driveboard.h"
#include "iic.h"
#include "delay.h" 
#include <stdio.h>
#include <string.h>
#include "bsp_led.h"

static uint8_t RFID_Setp=0;						 	// ��ȡID����
static uint8_t Read_Num=0; 							// ��ȡID����
static uint8_t Last_Setp=0;							// �ϴζ�ȡID����

	/*
 * ��������main
 * ����  ��������
 * ����  : ��
 * ���  ����
 */
int main(void)													// ������
{  
	/* ����ϵͳʱ��Ϊ 72M */      
  SystemInit();													// ��ʼʱ��
  Init_Board();													// ��ʼ��Ӳ��
	Refresh_LED_Display();								// ˢ����ʾ		
	
  while (1)
  {
		if(Hmwater_Temp>=105)								// ���±��� ����¶Ȳ�����105��
		{
			JDQ_Heat(0);											// �Ͽ�������
		}			
		// ��ȡ����
		if(Msecond_Flage==1)								// 10������ʱ								
		{
			Msecond_Flage=0;									// ��λ��־			
			Get_Now_Value();									// ��ȡ�¶�/TDS/��ADֵ
			Senser_Value_Read();							// ��ȡ����������״̬					
			Button_Read();										// ��ȡ����״̬		
			Work_In_Set();										// �û�����			
			if(Init_Falge==1) 								// ��ʼ����־(�ϵ��ʼ����־)
			{
				Refresh_LED_Display();					// ˢ����ʾ		
			}
			else
			{
				Tds_Test_Flage=1;								// �ϵ�����TDS
			}
			IWDG_ReloadCounter();							// ι��
		}
		// ÿ���Ӷ�ȡһ��RFID��
		if(Second_Flage==1)									// ���ӱ�־
		{
			Second_Flage=0;										// �����־λ								
			RF_Read_Info(&RFID_Setp);					// ��ȡ����֡
			if(Last_Setp==RFID_Setp)					// ��ζ����쳣����
			{
				if(Read_Num++>10)								// �ۼ�10�ζ��쳣
				{
					Read_Num=0;										// ��λID����
					RFID_Setp=1;									// ��λID����
					Last_Setp=1;									// ��λID����
				}
			}
			else
			{
				Last_Setp=RFID_Setp;						// �����ϴμ�¼
				Read_Num=0;											// ��λID����
			}
		}
		// ��⴮������֡
		if(Usart3_Receive_Flage==1)					// �������֡
		{
			Usart3_Receive_Flage=0;						// �����־λ
			EM4305_Get_NewFAX(&RFID_Setp);		// ��ȡ����
		}
  }
}


/******************* (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� *****END OF FILE****/
