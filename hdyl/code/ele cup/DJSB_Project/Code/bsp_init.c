/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2017 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp.
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Apr/21/2017
//***********************************************************************************************************
//  --bin -o E:/Keil_Project/OneNET.bin E:/Keil_Project/OneNET.axf
//                   E:/�����౦�ǿƼ����޹�˾/���ˮ����ϴ��/���е��ˮ��/���ˮ������/Project/Output/GPIO.bin E:/�����౦�ǿƼ����޹�˾/���ˮ����ϴ��/���е��ˮ��/���ˮ������/Project/Output/GPIO.bin
//***********************************************************************************************************
//  File Function: N76E003 PWM Output demo code
//***********************************************************************************************************
#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "eepromf.h"
#include "Bsp_init.h"

//*****************  The Following is in define in Fucntion_define.h  ***************************
//****** Always include Function_define.h call the define you want, detail see main(void) *******
//***********************************************************************************************
uint8_t equipment =0; //�豸���� bit   7           6       5      4     3       2     1    0
//             None         switch   None   LED   Pump   Green  Red  Blue
extern uint32_t systick;
uint8_t Led_Ram;//����ܿ���

uint8_t vbat_warn_flag;//0������1��ѹ
uint8_t DEBUG=0;
uint16_t ADC_Value[8] = {0, 1, 2, 3, 4, 5, 6, 7}; // ��ͨ������ֵ
double Vcc_value = 5;                                           // ��Դ��ѹֵ
double Vbat_value =3.7;                                            // ��ص�ѹֵ
float Ele_value = 24;                                               // ����ѹֵ
float Now_current =25;                                                // ��ǰ�¶�ֵ
uint8_t Speak_Flage =0;                                        // ������      0 �ر�  1 ����   2 ������  3 ��������ʾ��  4 �͵���������  5 ����������
uint8_t Key_Status =0;                                                 // ����״̬ ������״̬
uint8_t Work_Mode =0;                                              // ����ģʽ    0 �����Լ�ģʽ  1 ���ģʽ 2
//uint8_t Set_Time =2;                                               // ���õ��ʱ�� 3-4-5����
uint8_t Charge_Mode =0;                                        // ���ģʽ δ��� 0 ��� 1 ���� 2
//uint8_t Erro_Code =0;                                              // ������ 0 �޹��� 1 ��Դ��ѹ�� 2 ԭˮ�¶ȸ�  3ˮ���쳣  4��ϴ��ŷ�
uint8_t Ele_Flage =0;                                              // ��ʼ����־ 0 δ��� 1 ��ʼ��� 2 �Ѿ���� 3 ������
uint16_t Ele_Time = 0;                                              // ���ʱ��
uint16_t All_Use_Time = 0;                                      // �ۼ�ʹ�ô���
uint16_t Temp_Alarm_Num = 0;                                // �¶ȱ�����ֵ
uint16_t TDS_Alarm_Num = 0;                                     // TDS������ֵ
//uint8_t LED_Con = 0;                                                // LEDָʾ��
uint8_t pump_flag = 0;
/* �Զ���� */
double  Bandgap_Voltage;                                        //please always use "double" mode for this
code unsigned char  table[] = {0xc0, 0xf9, 0xa4, 0xb0, 0xff,0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e}; //����ܶ���

// ������
// 2500, 3000, 2500, 3000, 3500 ����
// 2000, 2100, 2300, 2500, 3500 ������
// 5000, 0, 0, 5000, 0, 0,5000  ������
// 3500, 3000, 2500, 3500, 2500 ����������
code uint16_t Music_Code_1[MUSIC_NUM1] =        // ��/�ػ�
{
    2200, 2500, 3400
};
code uint16_t Music_Code_2[MUSIC_NUM2] =        // ����
{
    2700, 0, 0
};
code uint16_t Music_Code_3[MUSIC_NUM3] =        // ������
{
    2000, 2100, 2300, 2500, 3500
};
code uint16_t Music_Code_4[MUSIC_NUM4] =        // �͵�����ʾ��
{
    5000, 0, 0, 5000, 0, 0, 5000
};
code uint16_t Music_Code_5[MUSIC_NUM5] =        // ����������
{
    3500, 3000, 2500, 3500, 2500
};
code uint16_t Music_Code_6[MUSIC_NUM6] =        // ISP��д����
{
    4000, 2000, 4000, 2000, 4000
};

void delay_10ms(uint32_t tmp)
{
    static uint32_t tick_t;
    if(tmp>0)
    {
        tick_t = systick;
        while((systick-tick_t)<tmp)
            ;

    }
}
/************************************************************************************************************
*    Main function
************************************************************************************************************/
void Board_gpio_init(void)
{

    P10_Input_Mode;
    P00_Input_Mode;                             // KEY
    P01_Input_Mode;                             // CHARG
    // P03_Input_Mode;                             // STDBY
    P05_PushPull_Mode;                      // EN_OUT ����Դʹ��

    // P10_PushPull_Mode;                      // VALUE
    P11_PushPull_Mode;                      // CLK
    P12_PushPull_Mode;                      // CE
    P13_PushPull_Mode;                      // OE
    P14_PushPull_Mode;                      // DAT
    P15_PushPull_Mode;                      // EN_POWER

    // �ϵ���ȱ���
    Pow_Con = 1;                                    // �����ϵ�ߵ�ƽ�����ϵ�״̬
    DJ_Con = 0;                                         // �����������
    Clear_Con = 1;                                  // LED��ֹ��ʾ
    // LED_Con = 0;                                    // ����LED
    //Value_Con=0;                                  // ���Ƶ�ŷ�
}

// ISP �����λ
void ISP_Rest(void)
{
    Speak_Flage = 6;
    DJ_Con = 0;                                         // ������ر�
    //LED_Con = 0;                                    // ����LED����
    //Value_Con=0;                                  // �ط�
    delay_ms(200);
    TA = 0xAA;
    TA = 0x55;
    CHPCON = 0x02;                //set boot from AP
    TA = 0xAA;
    TA = 0x55;
    CHPCON = 0x82;                //software reset enable

    /* Trap the CPU */
}

// ��ȡ��϶��ѹ�����궨ֵ
void READ_BANDGAP(void)
{
    UINT8 BandgapHigh, BandgapLow, BandgapMark;
    double Bandgap_Value, Bandgap_Voltage_Temp;

    set_IAPEN;
    IAPCN = READ_UID;
    IAPAL = 0x0d;
    IAPAH = 0x00;
    set_IAPGO;
    BandgapLow = IAPFD;
    BandgapMark = BandgapLow & 0xF0;

    if (BandgapMark == 0x80)
    {
        BandgapLow = BandgapLow & 0x0F;
        IAPAL = 0x0C;
        IAPAH = 0x00;
        set_IAPGO;
        BandgapHigh = IAPFD;
        Bandgap_Value = (BandgapHigh << 4) + BandgapLow;
        Bandgap_Voltage_Temp = Bandgap_Value * 3 / 4;
        Bandgap_Voltage = Bandgap_Voltage_Temp -
                          33;            //the actually banggap voltage value is similar this value.
    }
    if (BandgapMark == 0x00)
    {
        BandgapLow = BandgapLow & 0x0F;
        IAPAL = 0x0C;
        IAPAH = 0x00;
        set_IAPGO;
        BandgapHigh = IAPFD;
        Bandgap_Value = (BandgapHigh << 4) + BandgapLow;
        Bandgap_Voltage = Bandgap_Value * 3 / 4;
    }
    if (BandgapMark == 0x90)
    {
        IAPAL = 0x0E;
        IAPAH = 0x00;
        set_IAPGO;
        BandgapHigh = IAPFD;
        IAPAL = 0x0F;
        IAPAH = 0x00;
        set_IAPGO;
        BandgapLow = IAPFD;
        BandgapLow = BandgapLow & 0x0F;
        Bandgap_Value = (BandgapHigh << 4) + BandgapLow;
        Bandgap_Voltage = Bandgap_Value * 3 / 4;
    }
    clr_IAPEN;
}

// PWM��ʼ��
void Init_pwm_code(uint16_t fpwm, uint16_t duty)
{
    uint16_t pwm;

    PWM3_P04_OUTPUT_ENABLE;
    PWM_IMDEPENDENT_MODE;
    PWM_CLOCK_DIV_2;                        // 16/2=8

    pwm = 8000000 / fpwm;
    PWMPH = pwm / 256;                      // PWM��ʱ��T1��λ
    PWMPL = pwm % 256;                      // PWM��ʱ��T1��λ
    /**********************************************************************
        PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV>
                                    = (16MHz/2)/(0x7CF + 1)
                                    = 2KHz (1ms)
    ***********************************************************************/
    PWM3H = duty / 256;
    PWM3L = duty % 256;

//      set_SFRPAGE;                                //PWM4 and PWM5 duty seting is in SFP page 1
//      PWM4H = 0x03;
//      PWM4L = 0xCF;
//      clr_SFRPAGE;
// PWM output
    PWM3_OUTPUT_NORMAL;
//      PWM4_OUTPUT_NORMAL;
//-------- PWM start run--------------
    set_LOAD;
    set_PWMRUN;
}

void Init_adc_read(void)
{
    static uint8_t init = 0;
    float res, value;
//    uint8_t j;

    CKDIV = 0x02;                                       // IMPORTANT!! Modify system clock to 4MHz ,then add the ADC sampling clock base to add the sampling timing.
    Enable_ADC_AIN0;                // ����ѹ
    clr_ADCF;
    set_ADCS;
    while (ADCF == 0);
    ADC_Value[0] = (ADCRH << 4) + ADCRL;
    Disable_ADC;

    Enable_ADC_AIN1;                // �¶Ȼ��ߵ���
    clr_ADCF;
    set_ADCS;
    while (ADCF == 0);
    ADC_Value[1] = (ADCRH << 4) + ADCRL;
    Disable_ADC;

    Enable_ADC_AIN6;                // ��ص�ѹ
    clr_ADCF;
    set_ADCS;
    while (ADCF == 0);
    ADC_Value[2] = (ADCRH << 4) + ADCRL;
    Disable_ADC;

    Enable_ADC_BandGap;         // �ο�Դ
    clr_ADCF;
    set_ADCS;
    while (ADCF == 0);
    ADC_Value[3] = (ADCRH << 4) + ADCRL;
    Disable_ADC;
    CKDIV = 0x00;
    // �����������

    // *****************�����¶�******************************


    // ***************** ���ݴ�϶��ѹ���㹩���ѹ******************************
    value = (4096.0 / ADC_Value[3]) * Bandgap_Voltage / 1.0;
    value =  value /1000;
    if (init == 0)
        Vcc_value = value;              // δ��ʼ��

    Vcc_value = Vcc_value - Vcc_value / FIR_NUM + value / FIR_NUM; //��Դ

    value = (ADC_Value[1]) * Vcc_value /4095;//��������Ӧ��ѹ
    // δ��ʼ��
    if (init == 0)
        Now_current = value/11/RS;
    value = value/11/RS;
    Now_current = Now_current - Now_current / FIR_NUM + value / FIR_NUM;
    //Now_current = Now_current;
    // ***************** ���ݴ�϶��ѹ�����ص�ѹ******************************
    value = (ADC_Value[2]) * Vcc_value /4095*2;
    if (init == 0)
        Vbat_value = value;             // δ��ʼ��
    Vbat_value = Vbat_value - Vbat_value / FIR_NUM + value / FIR_NUM; //��ص�ѹ

    // ***************** �������ѹ******************************
    res = ADC_Value[0] * Vcc_value / 4095.0 * 11.0;
    if (init == 0)
        Ele_value = res;
    //	res = res*11;
    Ele_value = Ele_value - Ele_value / FIR_NUM + res / FIR_NUM;
    Ele_value = Ele_value;
    // ��ʼ��״̬����
    init = 1;
}

// �����������ʱ��
void Timer1_init(void)
{
    TIMER1_MODE1_ENABLE;

    clr_T1M;                            // 1/12 system clock
    //set_T1M;                      // system clock

    TH1 = TIMER_DIV12_VALUE_10ms / 256;             // ��ʱ10ms
    TL1 = TIMER_DIV12_VALUE_10ms % 256;             // ��ʱ10ms

    set_ET1;              // enable Timer1 interrupt
    set_EA;               // enable interrupts
    set_TR1;              // Timer1 run

    Speak_Flage = 0;            // ������
}
/*******************************************************************
���ܣ������豸�Ŀ���
������dev���豸����
			LED_BLUE=1
			LED_RED=4
			LED_GREEN=2
			PUMP=8
			DIA1=0x10
			SWITCH=0x40
	level:��ƽ


*******************************************************************/
void Equipment_Ctrl_Set(unsigned char dev, unsigned char level)
{
    if(dev&0x01)
    {
        if (level == 0)
        {
            equipment = equipment & 0xfe;
        }
        else
        {
            equipment = equipment | 0x01;
        }

    }
    if(dev&0x02)
    {
        if (level == 0)
        {
            equipment = equipment & 0xfd;
        }
        else
        {
            equipment = equipment | 0x02;
        }

    }
    if(dev&0x04)
    {
        if (level == 0)
        {
            equipment = equipment & 0xfb;
        }
        else
        {
            equipment = equipment | 0x04;
        }

    }
    if(dev&0x08)
    {
        if (level == 0)
        {
            equipment = equipment & 0xf7;
        }
        else
        {
            equipment = equipment | 0x08;
        }

    }
    if(dev&0x10)
    {
        if (level == 0)
        {
            equipment = equipment & 0xef;
        }
        else
        {
            equipment = equipment | 0x10;
        }

    }
    if(dev&0x20)
    {
        if (level == 0)
        {
            equipment = equipment & 0xdf;
        }
        else
        {
            equipment = equipment | 0x20;
        }

    }
    if(dev&0x40)
    {
        if (level == 0)
        {
            equipment = equipment & 0xbf;
        }
        else
        {
            equipment = equipment | 0x40;
        }

    }
    if(dev&0x80)
    {
        if (level == 0)
        {
            equipment = equipment & 0x7f;
        }
        else
        {
            equipment = equipment | 0x80;
        }

    }
    if((dev&0xff)==0)
    {
        equipment = 0x06;
        DJ_Con = 0;
    }

}

void Set_Speak_Value(uint8_t flage)
{
    Speak_Flage = flage;
}
void testt()
{
    Init_pwm_code(100,20);
}
// ����������
void Speak_value(void)
{
    static uint8_t codenum = 0;
    static uint16_t music = 0;

    // ������
    switch (Speak_Flage)
    {
    case 0: // �ر�
        Init_pwm_code(2000, 0);
        codenum = 0;
        break;
    case 1: // ������
        if (codenum >= MUSIC_NUM1)
        {
            Speak_Flage = 0;
            codenum = 0;
            music = 0;
        }
        else
        {
            if (music != Music_Code_1[codenum])
            {
                music = Music_Code_1[codenum];
                Init_pwm_code(music, music / 10);
            }
            codenum++;
        }
        break;
    case 2: // ������
        if (codenum >= MUSIC_NUM2)
        {
            Speak_Flage = 0;
            codenum = 0;
            music = 0;
        }
        else
        {
            if (music != Music_Code_2[codenum])
            {
                music = Music_Code_2[codenum];
                Init_pwm_code(music, music / 10);
            }
            codenum++;
        }
        break;
    case 3: // ������
        if (codenum >= MUSIC_NUM3)
        {
            Speak_Flage = 0;
            codenum = 0;
            music = 0;
        }
        else
        {
            if (music != Music_Code_3[codenum])
            {
                music = Music_Code_3[codenum];
                Init_pwm_code(music, music / 10);
            }
            codenum++;
        }
        break;
    case 4: // ������
        if (codenum > MUSIC_NUM4)
        {
            Speak_Flage = 0;
            codenum = 0;
            music = 0;
        }
        else
        {
            if (music != Music_Code_4[codenum])
            {
                music = Music_Code_4[codenum];
                Init_pwm_code(music, music / 10);
            }
            codenum++;
        }
        break;
    case 5: // �쳣����
        if (codenum >= MUSIC_NUM5)
        {
            Speak_Flage = 0;
            codenum = 0;
            music = 0;
        }
        else
        {
            if (music != Music_Code_5[codenum])
            {
                music = Music_Code_5[codenum];
                Init_pwm_code(music, music / 10);
            }
            codenum++;
        }
        break;
    case 6: // ISP
        if (codenum >= MUSIC_NUM6)
        {
            Speak_Flage = 0;
            codenum = 0;
            music = 0;
        }
        else
        {
            if (music != Music_Code_6[codenum])
            {
                music = Music_Code_6[codenum];
                Init_pwm_code(music, music / 10);
            }
            codenum++;
        }
        break;
    default :
        break;
    }
    // ����������

}
static uint32_t key_time_start = 0,key_time_star2 = 0;   // �������ʱ��     START   �趨����
unsigned char update=0,update2;   // �������ʱ��     START   �趨����
unsigned char key_hit_status=2;
/***********************************************************************************
 * ��������Button_Read(void)
 * ����  ����ȡ��������
 * ����  ����1
 * ���  ����
************************************************************************************/
uint8_t Button_Read(unsigned char flag)
{
    //static uint32_t tick_tmp;


    // �����˲�ʱ���ʱ

//    static unsigned char key_hit_flag;

    if (Key_Set == 1 && Key_Pump == 1)//ͬʱ������Ч
    {
        return KEY_START_UP;
    }
    else
    {

        // START��������
        if(Key_Set==1)												// SET��������
        {
            if(key_hit_status==0)
                key_hit_status = 1;
            if((Key_Status & 0x0f)==KEY_START_HOLD)				// ������
            {
                update=1;
                Key_Status = Key_Status & 0xf0;
                Key_Status = Key_Status | KEY_START_HOLD; 		  //				//
            }
            else if((Key_Status & 0x0f)==KEY_START_DOWN||(Key_Status & 0x0f)==KEY_START_MID)   // ǰһ���ǰ�������
            {
                if(key_time_start++>BUTTON_LONG_TIME)				// ��⵽������
                {
                    update=1;
                    Key_Status = Key_Status & 0xf0;
                    Key_Status = Key_Status | KEY_START_HOLD; 		  //
                }
                else if(key_time_start++>BUTTON_LONG_TIME*3)				// ��⵽������
                {
                    update=1;
                    Key_Status = Key_Status & 0xf0;
                    Key_Status = Key_Status | KEY_START_MID; 		  //
                }
            }
            else 																// ǰһ��û�а�������
            {
                if(key_time_start++>BUTTON_FILTER_TIME)			// ����ͨ���˲����
                {
                    update=1;
                    Key_Status = Key_Status & 0xf0;
                    Key_Status = Key_Status | KEY_START_DOWN;
                    if(flag == 0)
                        Set_Speak_Value(2);
                    //printf("key code = down");
                }
                else
                {
                    update=0;
                    Key_Status = Key_Status & 0xf0;
                    Key_Status = Key_Status | KEY_START_UP;
                    // if(flag == 0)
                    //   Set_Speak_Value(2);
                    //printf("key code = down");
                }
            }
        }
        else
        {

            update=0;
            Key_Status = Key_Status & 0xf0;
            Key_Status = Key_Status | KEY_START_UP; 		  //				//
            // Key_Status=KEY_START_UP;							// ����̧��
            key_time_start=0;
        }
//           while(Key_Set==1)
//            {
//                key_time_start++;
//                if(key_time_start > BUTTON_LONG_TIME)
//                {

//                    Key_Status = Key_Status & 0xf0;
//                    Key_Status = Key_Status | KEY_START_HOLD; 		  //
//
//                    break;
//                }
//            }

//            if(key_time_start<(BUTTON_LONG_TIME))
//            {
//                Key_Status = Key_Status & 0xf0;
//                Key_Status = Key_Status | KEY_START_DOWN;
//                if(flag == 0)
//                    Set_Speak_Value(2);
//            }
//            key_time_start = 0;
//
//        }

        // START��������
        if(Key_Pump==1)												// SET��������
        {
            if((Key_Status & 0xf0)==KEY_HSTART_HOLD)				// ������
            {
                update2=1;
                Key_Status = Key_Status & 0x0f;
                Key_Status = Key_Status | KEY_HSTART_HOLD; 		  //				//
            }
            else if((Key_Status & 0xf0)==KEY_HSTART_DOWN)   // ǰһ���ǰ�������
            {
                if(key_time_star2++>BUTTON_LONG_TIME)				// ��⵽������
                {
                    update2=1;
                    Key_Status = Key_Status & 0x0f;
                    Key_Status = Key_Status | KEY_HSTART_HOLD; 		  //
                }
            }
            else 																// ǰһ��û�а�������
            {
                if(key_time_star2++>BUTTON_FILTER_TIME)			// ����ͨ���˲����
                {
                    update2=1;
                    Key_Status = Key_Status & 0x0f;
                    Key_Status = Key_Status | KEY_HSTART_DOWN;
                    if(flag == 0)
                        Set_Speak_Value(2);
                    //printf("key code = down");
                }
//                else
//                {
//                    update2=0;
//									key_time_star2=0;
//                    Key_Status = Key_Status & 0x0f;
//                    Key_Status = Key_Status | KEY_HSTART_UP;
//                    // if(flag == 0)
//                    //   Set_Speak_Value(2);
//                    //printf("key code = down");
//                }
            }
        }
        else
        {

            if((Key_Status & 0xf0)==KEY_HSTART_HOLD)				// ������
            {
                update2=1;
                //update2=1;
                Key_Status = Key_Status & 0x0f;
                Key_Status = Key_Status | KEY_HSTART_LHOLD;		  //				//
            }
            else  if((Key_Status & 0xf0)!=KEY_HSTART_LHOLD)
            {

                Key_Status = Key_Status & 0x0f;
                Key_Status = Key_Status | KEY_HSTART_UP;
                update2=0;
            }
            key_time_star2=0;

        }


//        if (Key_Pump == 1)                                           // ˮ�ð���
//        {
//            while(Key_Pump==1)
//            {
//                key_time_star2++;
//                if(key_time_star2 > BUTTON_LONG_TIME)
//                {

//                    Key_Status = Key_Status & 0x0f;
//                    Key_Status = Key_Status | KEY_HSTART_HOLD;           //
//                    break;
//                }

//            }
//            if(key_time_star2<BUTTON_LONG_TIME)
//            {
//                Key_Status = Key_Status & 0x0f;
//                Key_Status = Key_Status | KEY_HSTART_DOWN;
//                Set_Speak_Value(2);

//            }
//            key_time_star2 = 0;

//        }
    }

    return Key_Status;
}
static unsigned char key_down_flag,key_down_flag2;
/***********************************************************************************
 * ��������Button_Read(void)
 * ����  ����ȡ��������
 * ����  ����1
 * ���  ����
************************************************************************************/
uint8_t Button_Read2(unsigned char flag)
{
    //static uint32_t tick_tmp;


    // �����˲�ʱ���ʱ

//    static unsigned char key_hit_flag;

    if (Key_Set == 1 && Key_Pump == 1)//ͬʱ������Ч
    {
        return KEY_START_UP;
    }
    else
    {

        // START��������
        if(Key_Set==1)												// SET��������
        {

            if(key_time_start++>BUTTON_FILTER_TIME)			// ����ͨ���˲����
            {


                key_down_flag = 1;
            }
            if(key_time_start++>BUTTON_LONG_TIME)			// ����ͨ���˲����
            {
                if(key_down_flag == 1)
                {
                    update=1;
                    key_time_start = 0;
                    key_down_flag = 2;
                    Key_Status = Key_Status & 0xf0;
                    Key_Status = Key_Status | KEY_START_HOLD;
                }

            }
        }

        else
        {
            if( key_down_flag == 1)
            {
                            if(flag == 0)
                    Set_Speak_Value(2);
                update=1;
                key_down_flag = 0;
                Key_Status = Key_Status & 0xf0;
                Key_Status = Key_Status | KEY_START_DOWN;

            }
            else
            {
                Key_Status = Key_Status & 0xf0;
                Key_Status = Key_Status | KEY_START_UP;

            }
            key_down_flag = 0;
            key_time_start = 0;
        }

        // START��������
        if(Key_Pump==1)												// SET��������
        {

            if(key_time_star2++>BUTTON_FILTER_TIME) 		// ����ͨ���˲����
            {


                key_down_flag2 = 1;
            }
            if(key_time_star2++>BUTTON_LONG_TIME)			// ����ͨ���˲����
            {
                if(key_down_flag2 == 1)
                {
                    key_time_star2 = 0;
                    update2=1;
                    Key_Status = Key_Status & 0x0f;
                    Key_Status = Key_Status | KEY_HSTART_HOLD;		  //

                }

            }
        }

        else
        {
            if( key_down_flag2 == 1)
            {             
            if(flag == 0)
                    Set_Speak_Value(2);
                key_down_flag2 = 0;
                update2=1;
                Key_Status = Key_Status & 0x0f;
                Key_Status = Key_Status | KEY_HSTART_DOWN;


            }
            else
            {
                Key_Status = Key_Status & 0x0f;
                Key_Status = Key_Status | KEY_HSTART_UP;

            }
            key_down_flag2 = 0;
            key_time_star2 = 0;
        }
    }

    return Key_Status;



}
void key_reset()
{

    Key_Status = Key_Status & 0xf0;
    Key_Status = Key_Status | KEY_START_UP; // ����̧��
    key_time_start = 0;
    Key_Status = Key_Status & 0x0f;
    Key_Status = Key_Status | KEY_HSTART_UP; // ����̧��
    key_time_star2 = 0;
}
unsigned char Charge_Process()
{
    static uint32_t charge_time = 0;
    static uint32_t stdby_time = 0;
    static unsigned char red_flag;
    static unsigned char charge_status;
    // �����ģʽ
    if (Charge_Set == 1)//���ģʽ
    {
        Charge_Mode = 5;
        if(charge_status==1)
        {
            if(charge_time%100 == 0)
            {
                if(red_flag == 0)//���ʱ����ʱˢ�º��
                    red_flag = 1;
                else
                    red_flag = 0;
                Equipment_Ctrl_Set(LED_RED, red_flag);//�����
                Equipment_Ctrl_Set(LED_GREEN, 1);//�̵���
            }

            if (charge_time++ > CHARGE_LONG_TIME) //
            {
                charge_time = 0;         //
            }

        }
        if (Vbat_value <= MAX_VBAT && Vbat_value >= MIN_VBAT)
        {
            charge_status = 1;
            Charge_Mode = 1;   // ���
        }
        else if (Vbat_value > MAX_VBAT)
        {
            if (stdby_time++ > FCHARGE_LONG_TIME)// ����
            {
                Equipment_Ctrl_Set(LED_RED, 1);//�����
                Equipment_Ctrl_Set(LED_GREEN, 0);//�̵���
                charge_status = 0;
                stdby_time = BUTTON_LONG_TIME;
                Charge_Mode = 2;// ����;
            }
            else
            {
                charge_status = 1;
                Charge_Mode = 1;// ���
            }
        }
        else
        {
            Charge_Mode = 3; //����
            stdby_time = 0;
            charge_time = 0;
            charge_status = 0;
        }

    }
    else
    {
        stdby_time = 0;
        charge_time = 0;
        Charge_Mode = 0;//û���;
    }
    return Charge_Mode;

}


//����һ���ֽ����ݸ�595�ٲ������,data1�����������ʾ��data2�����ⲿ�豸
void SendTo595(uint8_t data1, uint8_t data2)
{
    char i = 0;
    //Clear_Con = 1;
    for (i; i < 16; i++)
    {

        if(i<8)
        {

            if(0x80 & data1)
                DAT_Con = 1; //&Ϊ��λ���������ȫ1Ϊ1����0Ϊ0����ʽҲ���� ��1000 0000��&��1111 1111��=1000 0000������λΪ1����1��λΪ0�����ʽ��Ϊ0
            else
                DAT_Con = 0;
            data1 = data1 <<1; //����һλ ����λ������λ�������������Ϊ01010101 ��ô_crol_(1) Ϊ10101010
            CLK_Con = 1;          //�������ô�������ʱ�ӱ�ɸߵ�ƽ ����ʱһ��ʱ������
            _nop_();
        }
        else
        {
            CLK_Con = 0;
            if(0x80 & data2)
                DAT_Con = 1; //&Ϊ��λ���������ȫ1Ϊ1����0Ϊ0����ʽҲ���� ��1000 0000��&��1111 1111��=1000 0000������λΪ1����1��λΪ0�����ʽ��Ϊ0
            else
                DAT_Con = 0;
            data2 = data2 <<1; //����һλ ����λ������λ�������������Ϊ01010101 ��ô_crol_(1) Ϊ10101010

        }
        CLK_Con = 0;
        _nop_();
        CLK_Con = 1;          //�������ô�������ʱ�ӱ�ɸߵ�ƽ ����ʱһ��ʱ������
        _nop_();

    }

    /*λ�ƼĴ�����ϣ�ת�Ƶ��洢�Ĵ���*/
    RCK_Con = 0;         //�����أ��洢�Ĵ�����Ϊ�ߵ�ƽ �ӳ�����ʱ������
    Clear_Con = 0;
    _nop_();
    _nop_();
    RCK_Con = 1;
}

void HC595_Data_High()
{
    DAT_Con = 1;
}
void HC595_Data_Low()
{
    DAT_Con = 0;
}
void HC595_SHCP_High()
{
    CLK_Con = 1;
}
void HC595_SHCP_Low()
{
    CLK_Con = 0;
}
void HC595_STCP_High()
{
    RCK_Con = 1;
}
void HC595_STCP_Low()
{
    RCK_Con = 0;
}
/***
 *74HC595 ����һ���ֽ�
 *����74HC595��DS���ŷ���һ���ֽ�
*/
void HC595_Send_Byte(unsigned char byte)
{
    unsigned char i;
    for (i = 0; i < 8; i ++)  //һ���ֽ�8λ������8�Σ�һ��һλ��ѭ��8�Σ��պ�����8λ
    {
        /****  ����1�������ݴ���DS����    ****/
        if (byte & 0x80)        //�ȴ����λ��ͨ���������жϵڰ��Ƿ�Ϊ1
            HC595_Data_High();    //����ڰ�λ��1������ 595 DS���ӵ���������ߵ�ƽ
        else                    //��������͵�ƽ
            HC595_Data_Low();

        /*** ����2��SHCPÿ����һ�������أ���ǰ��bit�ͱ�������λ�Ĵ��� ***/
        HC595_SHCP_Low();   // SHCP����
        _nop_();
        _nop_();;           // �ʵ���ʱ
        HC595_SHCP_High();  // SHCP���ߣ� SHCP����������
        _nop_();
        _nop_();;

        byte <<= 1;		// ����һλ������λ����λ�ƣ�ͨ��	if (byte & 0x80)�жϵ�λ�Ƿ�Ϊ1
    }
}

/**
 *74HC595������� ʹ��
**/
void HC595_CS(void)
{
    /**  ����3��STCP����һ�������أ���λ�Ĵ�������������洢�Ĵ���  **/
    HC595_STCP_Low();   // ��STCP����
    _nop_();
    _nop_();
    _nop_();           // �ʵ���ʱ
    HC595_STCP_High();  // �ٽ�STCP���ߣ�STCP���ɲ���һ��������
    _nop_();
    _nop_();
    _nop_();
}

/**
 *���Ͷ���ֽ�
 *���ڼ���ʱ���ݵķ���
 *����N��������Ҫ����N���ֽڿ���HC595
***/
void HC595_Send_Multi_Byte(uint8_t data1, uint8_t data2)
{
    unsigned char i;
    unsigned char buf[2];
    buf[0] = data1;
    buf[1] = data2;
    Clear_Con = 0;
    for (i = 0; i < 2; i ++ ) // len ���ֽ�
    {
        HC595_Send_Byte(buf[i]);
    }

    HC595_CS(); //�Ȱ������ֽڷ����꣬��ʹ�����
}

/***********************************************************************************
 * ��������void Shutdown_Mode(void)
 * ����  ���ػ�
 * ����  ����
 * ���  ����
************************************************************************************/
void Shutdown_Mode(void)
{
    Led_Ram = 0xff;
    Set_Speak_Value(1);
    Equipment_Ctrl_Set(0, 6);
    //Led_Display();                                  // LED��ʾ
    DJ_Con = 0;                                             // �رյ��
    Pow_Con = 0;                                        // �رյ�Դ

    // ��������
    All_Use_Time++;                                 // �ۼ�ʹ�ô���
    Write_DATAFLASH_BYTE();                 // ��������
    Equipment_Ctrl_Set(DIA1, 0);//���ſ�
    Led_Ram = 0x04;
    HC595_Send_Multi_Byte(table[Led_Ram], equipment);
    delay_ms(200);


}

void display_init(void)
{

    Set_Speak_Value(1);
    Equipment_Ctrl_Set(0, 6);
    //Led_Display();                                  // LED��ʾ
    DJ_Con = 0;                                             // �رյ��                                        // �رյ�Դ
    Led_Ram = 0x04;
    if(Charge_Set == 0)
        Equipment_Ctrl_Set(LED_GREEN, 0);
    else
    {
        Equipment_Ctrl_Set(LED_GREEN, 1);
        Equipment_Ctrl_Set(LED_RED, 0);
    }
    HC595_Send_Multi_Byte(table[Led_Ram], equipment);
    delay_10ms(10);


}
void Display_Process()
{
//Equipment_Ctrl_Set(DIA1, 1);//���ſ�
    Clear_Con = 0;
    //equipment = 0x02;
    // Equipment_Ctrl_Set(DIA1, 1);//���ſ�
//Led_Ram=2;
    HC595_Send_Multi_Byte(table[Led_Ram], equipment);
}



// �û�����
static unsigned char key_Count;
void Work_In_Set(uint8_t keycode)               //
{
    static uint8_t key = 0;//����״̬��ʱ��ȡ
    static uint8_t init_time = 0;
    static uint8_t key_num = 0;                       // ����������� 5�ε㰴 һ�γ����������ISP
    static uint16_t NoWaterTick;
    static uint8_t key_hit_flag;
    static uint32_t timeout,timeout2 ;
    static uint32_t light_flag;
    float tmpa;
    unsigned char tmpb;
    // ������
    switch (Work_Mode)
    {
    case 0://�����Լ�
    {
        key_num = 0;
        if(init_time++>=50)
        {
            init_time = 1;
            if(Vbat_value <=BATTER_DS )
            {
                if((systick-timeout)>=20000)
                {
                    timeout = systick;
                    Shutdown_Mode();
                }
            }
            else if(Vbat_value <=BATTER_ALARM)//��ѹ����
            {
                if((systick-timeout)>=500)
                {

                    Equipment_Ctrl_Set(PUMP|SWITCH|DIA1|LED_BLUE, 0);//�ù�
                    Work_Mode = 4;//�쳣����ģʽ
                    if(Charge_Set == 0)
                    {
                        Equipment_Ctrl_Set(LED_GREEN, 1);//�ù�
                        Equipment_Ctrl_Set(LED_RED, 0);//�����
                        Set_Speak_Value(4);//
                    }
                    timeout = systick;
                    Led_Ram = 0x04;
                }
            }
            else//��������
            {
                Equipment_Ctrl_Set(PUMP|SWITCH|LED_RED|DIA1|LED_BLUE, 0);//�ù�
                Equipment_Ctrl_Set(LED_GREEN, 0);//�̵���
                Led_Ram = 0x04;
                Work_Mode = 1;//��������ģʽ
                //Set_Speak_Value(1);//
            }
        }
        key = keycode&0x0f;
//            //******************** ���ػ��������̰���⣬�������ػ�***************************

        if(key==KEY_START_HOLD) //�ػ�
        {
            Shutdown_Mode();
            update =0;
            Send_Data_To_UART0(3,DEBUG);
        }

    }
    break;
    case 1: //��������ģʽ
    {

        if(Charge_Mode == 0)
        {
            key = keycode&0x0f;
//            //******************** ���ػ��������̰���⣬�������ػ�***************************
            if(update==1)
            {
                update =0;
                if(key==KEY_START_DOWN)//�̰�һ��
                {
                    //if(Key_Set ==0)
                    {
                        key_Count ++;

                        if(Ele_Flage==0&&vbat_warn_flag ==0)//��ʼ���
                        {
                            if( pump_flag == 0)
                            {
                                equipment = equipment|0x01|0x80;
                                Ele_Time = 0;
                                Ele_Flage = 1;
                                DJ_Con = 1;//���
//                            nop_;
//                            nop_;
//                            nop_;
//                            nop_;
//                            nop_;
//                            nop_;
//                            nop_;
                                tmpa = ELE_TIME*10;
                                tmpb = (unsigned char)tmpa;
                                if(tmpb%10>=5)

                                    Led_Ram = ELE_TIME+1;
                                else
                                    Led_Ram = ELE_TIME;
                                // Send_Data_To_UART0(1,DEBUG);
                                Equipment_Ctrl_Set(0x80, 1);//�ƿ�
                                Equipment_Ctrl_Set(0x01, 1);//�ƿ�
                                Equipment_Ctrl_Set(DIA1, 1);//���ſ�
                                Equipment_Ctrl_Set(SWITCH, 0);//���Ź�
                            }

                        }
                        else if(Ele_Flage==1)
                        {

                            {

                                Ele_Time = 0;
                                Ele_Flage = 0;
                                Send_Data_To_UART0(2,DEBUG);
                                Led_Ram = 0;
                                DJ_Con = 0;//ȡ�����
                                Led_Ram = 0x04;
                                Equipment_Ctrl_Set(0x80, 0);//���ſ�
                                Equipment_Ctrl_Set(0x01, 0);//���ſ�

                                Equipment_Ctrl_Set(SWITCH, 0);//���Ź�											 }

                            }
                        }
                        //  Ele_Flage = 0;
                        //��ʾ
                    }

                }
                else if(key==KEY_START_HOLD) //�ػ�
                {
                    Shutdown_Mode();
                    Send_Data_To_UART0(3,DEBUG);
                }
            }
            if(update2==1)
            {
                update2 = 0;
                //******************** ������ˮ�������̰�����**************************
                key = keycode&0xf0;
                if(key==KEY_HSTART_DOWN||key==KEY_HSTART_HOLD)//�̰�һ��
                {
                    if(pump_flag == 1)//��������
                    {

                        if(key==KEY_HSTART_HOLD)
                        {
                            Key_Status = Key_Status & 0x0f;
                            Key_Status = Key_Status | KEY_HSTART_UP;
                        }
												else
												{
                        Equipment_Ctrl_Set(PUMP, 0);//�ù�;
                        pump_flag = 0;
                        Send_Data_To_UART0(4,DEBUG);												
												}
                    }
                    else
                    {
                        if(Ele_Flage == 0&&vbat_warn_flag ==0)
                        {
                            pump_flag = 1;
                            Send_Data_To_UART0(5,DEBUG);
                            Equipment_Ctrl_Set(PUMP, 1);//�ÿ�;
                        }

                    }
                }
                else if(key==KEY_HSTART_HOLD) //����
                {
                    if(pump_flag == 0)
                    {
                        if(Ele_Flage == 0&&vbat_warn_flag ==0)
                        {
                            pump_flag = 1;
                            Send_Data_To_UART0(6,DEBUG);
                            Equipment_Ctrl_Set(PUMP, 1);//�ÿ�;
                        }
                    }
//										else
//									                    {
//                        Equipment_Ctrl_Set(PUMP, 0);//�ù�;
//                        pump_flag = 0;
//                        Send_Data_To_UART0(4,DEBUG);

//                    }
                }
            }


        }
    }

    break;
    case 4:
    {
        if(Charge_Mode == 0)
        {
            Work_Mode = 0;
            key_num = 0;
        }

    }
    break;
    case 6:
    {
        if(Charge_Mode == 0)
        {
            Shutdown_Mode();
            Work_Mode = 0;
            key_num = 0;
        }
    }
    break;
    }

    if(Charge_Mode != 0)//��������������ֻ�����س���
    {

        vbat_warn_flag = 1;
        Work_Mode = 6;
        key = keycode&0x0f;
        if(key==KEY_START_DOWN)//�̰�һ��
        {
            key_hit_flag = 1;
            if(timeout2 <=1000)
            {
                key_num ++;
                if(key_num>=10)//����5�ν�������ģʽ
                {
                    Send_Data_To_UART0(7,DEBUG);
                    Set_Speak_Value(6);//����ģʽ����
                    ISP_Rest();//����ldrom����ģʽ
                    key_num = 0;
                }
            }
            else
            {
                key_hit_flag = 0;
                key_num = 0;
                timeout2 = 0;
            }
        }
        if(key_hit_flag == 1)
        {
            timeout2++;

        }
        if(Charge_Mode == 2) //����
        {
            Work_Mode = 4;
            // Shutdown_Mode();//�ػ�
            // Charge_Mode = 0;
        }
        if(Charge_Mode == 3) //����
        {
            Work_Mode = 4;
            Ele_Flage=0;
            Shutdown_Mode();//�ػ�
            // Charge_Mode = 0;
        }
    }
    else //����ģʽ
    {
        key_hit_flag = 0;
        //timeout = 0;
        if(Ele_Flage == 1)//����ʱ
        {
            Ele_Time++;
            if(Ele_Time%6000==0)//1����ˢ��һ��
            {
                if(Led_Ram>0)
                    Led_Ram = Led_Ram - 1;
            }

            if(Ele_Time>=ELE_PERIOD)//���ʱ�䵽
            {
                light_flag = 1;
                Ele_Flage = 0;
                Ele_Time = 0;
                Led_Ram = 0;
                DJ_Con = 0;
                Equipment_Ctrl_Set(0x80, 0);//�ƹ�
                Equipment_Ctrl_Set(0x01, 0);//�ƹ�
                Equipment_Ctrl_Set(SWITCH, 1);//���ſ�
                Led_Ram = 0x04;
                if(Ele_value>MIN_TDSVALUE)  // ˮ�ʲ�
                {

                    //  Send_Data_To_UART0(8,DEBUG);
                    Set_Speak_Value(5);//����
                    TDS_Alarm_Num++;                    // TDS������ֵ�ۼ�����
                }
                else
                {
                    //  Send_Data_To_UART0(9,DEBUG);

                    Set_Speak_Value(3);//������
                }
            }

            if(Now_current<=MIN_CURRENT||(Ele_Time>=4000&&Now_current<=0.8))//��ˮ���
            {
                NoWaterTick++;
                if(NoWaterTick>=1100)//����10s
                {
                    Led_Ram = 0x04;
                    NoWaterTick = 0;
                    Ele_Flage = 0;
                    Ele_Time = 0;
                    DJ_Con = 0;
                    Equipment_Ctrl_Set(0x80, 0);//�ƿ�
                    Equipment_Ctrl_Set(0x01, 0);//�ƿ�
                    //Send_Data_To_UART0(10,DEBUG);
                    Equipment_Ctrl_Set(SWITCH, 0);//���Ź�
                    Set_Speak_Value(5);//�쳣����
                }
            }
            else  if(Now_current>=MAX_CURRENT)//��·����
            {
                NoWaterTick++;
                if(NoWaterTick>=500)//����10s
                {
                    Led_Ram = 0x04;
                    NoWaterTick = 0;
                    Ele_Flage = 0;
                    Ele_Time = 0;
                    DJ_Con = 0;
                    Equipment_Ctrl_Set(0x80, 0);//�ƿ�
                    Equipment_Ctrl_Set(0x01, 0);//�ƿ�
                    // Send_Data_To_UART0(11,DEBUG);
                    Equipment_Ctrl_Set(SWITCH, 0);//���Ź�
                    Set_Speak_Value(5);//�쳣����
                }
            }
            else
            {
                if(NoWaterTick>0)
                    NoWaterTick--;
            }

        }
        else
        {
            if(light_flag == 1)
            {
                if((Ele_Time++)>=SWITCH_PERIOD)
                {
                    // Send_Data_To_UART0(12,DEBUG);
                    light_flag = 0;
                    Equipment_Ctrl_Set(SWITCH, 0);//���Ź�
                    Ele_Time = 0;
                }
            }
            else
                Equipment_Ctrl_Set(SWITCH, 0);//���Ź�
        }

    }

    if(Charge_Mode != 0)//���ģʽͣ������
    {
        vbat_warn_flag = 1;
        Ele_Flage = 0;
        Ele_Time = 0;
        Led_Ram = 0;
        DJ_Con = 0;
        Led_Ram = 0x04;
        Equipment_Ctrl_Set(PUMP|SWITCH|DIA1|LED_BLUE, 0);//�豸����
        // Equipment_Ctrl_Set( LED_GREEN, 0);//�豸����
        // Equipment_Ctrl_Set( LED_RED, 1);//�豸����
    }


    //�ǳ�ʼ��ģʽ�������
    if(Work_Mode!=0)
    {
        if(Vbat_value <= BATTER_DS)
        {

            if((systick-timeout)>=30000)
            {
                vbat_warn_flag = 1;
                timeout = systick;
                Shutdown_Mode();
                //Send_Data_To_UART0(12,DEBUG);
            }
        }
        else if(Vbat_value <=BATTER_ALARM)//��ѹ����
        {
            if((systick-timeout)>=3000)
            {
                vbat_warn_flag = 1;

                //  Send_Data_To_UART0(13,DEBUG);
                // Led_Ram = 0x04;
                //  Equipment_Ctrl_Set(PUMP|SWITCH|DIA1|LED_BLUE, 0);//�ù�
                Equipment_Ctrl_Set(LED_RED, 0);//�����
                Equipment_Ctrl_Set(LED_GREEN, 1);//�̵���
                // Set_Speak_Value(4);// ������
                //    Work_Mode = 4;//�쳣����ģʽ
                timeout = systick;
            }
        }
        else
        {

            timeout = systick;
            if(Charge_Mode == 0)
            {
                vbat_warn_flag = 0;
                Equipment_Ctrl_Set(LED_RED, 1);//�����
                Equipment_Ctrl_Set(LED_GREEN, 0);//�̵���
            }


        }

    }

}
// ���͹�������������豸
void Send_Info(void)
{
    static uint8_t sendnum = 0;
    uint8_t buffer[20];
    uint16_t ecc, i;

    if (sendnum++ < 50)
    {
        return;
    }
    else
    {
        sendnum = 0;
    }
    buffer[0] = 'D';
    buffer[1] = 'B';
    buffer[2] = 'X';
    buffer[3] = Vcc_value * 10;
    buffer[4] = Ele_value * 10;
    buffer[5] = Now_current;
    // д���û�����
    buffer[6] = All_Use_Time / 256;
    buffer[7] = All_Use_Time % 256;
    buffer[8] = Temp_Alarm_Num / 256;
    buffer[9] = Temp_Alarm_Num % 256;
    buffer[10] = TDS_Alarm_Num / 256;
    buffer[11] = TDS_Alarm_Num % 256;

    for (i = 0; i < 12; i++)
    {
        if (i == 0) ecc = buffer[0];
        ecc += buffer[i];
    }
    buffer[12] = ecc / 256;
    buffer[13] = ecc % 256;
    buffer[14] = 0x47;
    buffer[15] = 0x10;
    for (i = 0; i < 16; i++)
    {
        Send_Data_To_UART0(buffer[i],0);  // UART0 send ascii "U"
    }
}
