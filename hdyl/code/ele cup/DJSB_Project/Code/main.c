/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2017 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Jan/21/2017
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: N76E003 GPIO demo code
//***********************************************************************************************************
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "eepromf.h"
#include "Common.h"
#include "Delay.h"
#include "Bsp_init.h"

uint8_t  Ms_Flage=0,dis_flag;						// 10ms��־
//uint8_t  Second=0;							// ��
//uint8_t  Minute=0;							// ��ʱ����
//uint8_t  Hour=0;								// ��ʱ����
//uint8_t  MSecond=0;							// 10ms����

//*****************  The Following is in define in Fucntion_define.h  ***************************
//****** Always include Function_define.h call the define you want, detail see main(void) *******
//***********************************************************************************************

/************************************************************************************************************
*    TIMER 1 interrupt subroutine
************************************************************************************************************/
//code uint16_t Ele_Time_Table[6]= {10,60,120,180,240,300};
uint32_t systick;

void Timer1_ISR (void) interrupt 3  //interrupt address is 0x001B
{
    static uint16_t time=0;
    uint8_t temp=0;
    systick++;
    TH1 = TIMER_DIV12_VALUE_10ms/256;
    TL1 = TIMER_DIV12_VALUE_10ms%256;
    // ������
    if(time++>=3)						// 30msִ������
    {
        time=0;
        Speak_value();
			if(dis_flag)
			{
			dis_flag = 0;
				Display_Process();
			}
			 
    }
   	
    // 10ms��־
    Ms_Flage=1;
}

/***********************************************************************
	WDT CONFIG enable
	warning : this macro is only when ICP not enable CONFIG WDT function
	copy this marco code to you code to enable WDT reset.
************************************************************************/
void WDT_Config_Enable(void)
{
    TA=0xAA;
    TA=0x55;
    WDCON|=0x03;			// Setting WDT prescale 102ms
    set_WDCLR;												// Clear WDT timer
    while((WDCON|~SET_BIT6)==0xFF);		// confirm WDT clear is ok before into power down mode
    set_WDTR;													// WDT run
}
uint32_t start_tick;
extern uint8_t Charge_Mode ;
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void)
{
     uint8_t keycode=0;								// ����
    uint8_t init;											// ��ʼ��ADC
    unsigned char init_flag;
    init_flag = 1;
    Set_All_GPIO_Quasi_Mode;					// Define in Function_define.h

    Board_gpio_init();								// ��ʼ��IO
    Timer1_init();										// ��ʼ����ʱ��

    if(Charge_Process())
        goto next;
    while((Button_Read(1)&KEY_START_HOLD)!=KEY_START_HOLD)
        key_reset();
next:
    key_reset();


    WDT_Config_Enable();							// WDT run
    InitialUART0_Timer3(9600);				// ��ʼ������1
    Read_EEprom();										// ��ȡʹ������
    READ_BANDGAP();										// ��϶��ѹ�����궨ֵ
    for(init=0; init<50; init++) 				// ����ص�ѹ
    {
        Init_adc_read();								// ��ʼ��ADC
        //	set_WDCLR;											// Clear WDT timer
    }
    display_init();
    init_flag = 0;
    // ��ȡADC����ֵ
    while(1)
    {
		//	testt();
        //if(Charge_Set == 0)
        keycode=Button_Read(init_flag);				// ��ȡ����
        if(Ms_Flage==1)									// 10ms��ʱ��־λ
        {
            set_WDCLR;										// Clear WDT timer
            // �����־//22

            Charge_Process();
            Work_In_Set(keycode);					// ����������ģʽ��
            Init_adc_read();							// ��ȡADC����ֵ
            Ms_Flage=0;
            key_reset();
					  dis_flag = 1;
			//Send_Info();									// �������ݵ�����
        }

        // ˢ����ʾ
      //  Display_Process();								  // LED��ʾ

    }
}

