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
//                   E:/沈阳多宝星科技有限公司/电解水机清洗机/自研电解水杯/电解水杯程序/Project/Output/GPIO.bin E:/沈阳多宝星科技有限公司/电解水机清洗机/自研电解水杯/电解水杯程序/Project/Output/GPIO.bin
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
uint8_t equipment =0; //设备控制 bit   7           6       5      4     3       2     1    0
//             None         switch   None   LED   Pump   Green  Red  Blue
extern uint32_t systick;
uint8_t Led_Ram;//数码管控制

uint8_t vbat_warn_flag;//0正常；1低压
uint8_t DEBUG=0;
uint16_t ADC_Value[8] = {0, 1, 2, 3, 4, 5, 6, 7}; // 各通道采样值
double Vcc_value = 5;                                           // 电源电压值
double Vbat_value =3.7;                                            // 电池电压值
float Ele_value = 24;                                               // 电解电压值
float Now_current =25;                                                // 当前温度值
uint8_t Speak_Flage =0;                                        // 蜂鸣器      0 关闭  1 开机   2 按键音  3 电解完成提示音  4 低电量报警音  5 其他报警音
uint8_t Key_Status =0;                                                 // 按键状态 喷雾按键状态
uint8_t Work_Mode =0;                                              // 工作模式    0 开机自检模式  1 电解模式 2
//uint8_t Set_Time =2;                                               // 设置电解时长 3-4-5分钟
uint8_t Charge_Mode =0;                                        // 充电模式 未充电 0 充电 1 充满 2
//uint8_t Erro_Code =0;                                              // 故障码 0 无故障 1 电源电压低 2 原水温度高  3水质异常  4清洗电磁阀
uint8_t Ele_Flage =0;                                              // 开始电解标志 0 未电解 1 开始电解 2 已经电解 3 电解结束
uint16_t Ele_Time = 0;                                              // 电解时间
uint16_t All_Use_Time = 0;                                      // 累计使用次数
uint16_t Temp_Alarm_Num = 0;                                // 温度报警数值
uint16_t TDS_Alarm_Num = 0;                                     // TDS报警数值
//uint8_t LED_Con = 0;                                                // LED指示灯
uint8_t pump_flag = 0;
/* 自定义宏 */
double  Bandgap_Voltage;                                        //please always use "double" mode for this
code unsigned char  table[] = {0xc0, 0xf9, 0xa4, 0xb0, 0xff,0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e}; //数码管段码

// 蜂鸣器
// 2500, 3000, 2500, 3000, 3500 开机
// 2000, 2100, 2300, 2500, 3500 电解完成
// 5000, 0, 0, 5000, 0, 0,5000  电量低
// 3500, 3000, 2500, 3500, 2500 报警不工作
code uint16_t Music_Code_1[MUSIC_NUM1] =        // 开/关机
{
    2200, 2500, 3400
};
code uint16_t Music_Code_2[MUSIC_NUM2] =        // 按键
{
    2700, 0, 0
};
code uint16_t Music_Code_3[MUSIC_NUM3] =        // 电解完成
{
    2000, 2100, 2300, 2500, 3500
};
code uint16_t Music_Code_4[MUSIC_NUM4] =        // 低电量提示音
{
    5000, 0, 0, 5000, 0, 0, 5000
};
code uint16_t Music_Code_5[MUSIC_NUM5] =        // 报警不工作
{
    3500, 3000, 2500, 3500, 2500
};
code uint16_t Music_Code_6[MUSIC_NUM6] =        // ISP烧写程序
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
    P05_PushPull_Mode;                      // EN_OUT 电解电源使能

    // P10_PushPull_Mode;                      // VALUE
    P11_PushPull_Mode;                      // CLK
    P12_PushPull_Mode;                      // CE
    P13_PushPull_Mode;                      // OE
    P14_PushPull_Mode;                      // DAT
    P15_PushPull_Mode;                      // EN_POWER

    // 上电后先保持
    Pow_Con = 1;                                    // 开机上电高电平保持上电状态
    DJ_Con = 0;                                         // 电解器不工作
    Clear_Con = 1;                                  // LED禁止显示
    // LED_Con = 0;                                    // 控制LED
    //Value_Con=0;                                  // 控制电磁阀
}

// ISP 软件复位
void ISP_Rest(void)
{
    Speak_Flage = 6;
    DJ_Con = 0;                                         // 电解器关闭
    //LED_Con = 0;                                    // 控制LED点亮
    //Value_Con=0;                                  // 关阀
    delay_ms(200);
    TA = 0xAA;
    TA = 0x55;
    CHPCON = 0x02;                //set boot from AP
    TA = 0xAA;
    TA = 0x55;
    CHPCON = 0x82;                //software reset enable

    /* Trap the CPU */
}

// 读取带隙电压出厂标定值
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

// PWM初始化
void Init_pwm_code(uint16_t fpwm, uint16_t duty)
{
    uint16_t pwm;

    PWM3_P04_OUTPUT_ENABLE;
    PWM_IMDEPENDENT_MODE;
    PWM_CLOCK_DIV_2;                        // 16/2=8

    pwm = 8000000 / fpwm;
    PWMPH = pwm / 256;                      // PWM计时器T1高位
    PWMPL = pwm % 256;                      // PWM计时器T1低位
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
    Enable_ADC_AIN0;                // 电解电压
    clr_ADCF;
    set_ADCS;
    while (ADCF == 0);
    ADC_Value[0] = (ADCRH << 4) + ADCRL;
    Disable_ADC;

    Enable_ADC_AIN1;                // 温度或者电流
    clr_ADCF;
    set_ADCS;
    while (ADCF == 0);
    ADC_Value[1] = (ADCRH << 4) + ADCRL;
    Disable_ADC;

    Enable_ADC_AIN6;                // 电池电压
    clr_ADCF;
    set_ADCS;
    while (ADCF == 0);
    ADC_Value[2] = (ADCRH << 4) + ADCRL;
    Disable_ADC;

    Enable_ADC_BandGap;         // 参考源
    clr_ADCF;
    set_ADCS;
    while (ADCF == 0);
    ADC_Value[3] = (ADCRH << 4) + ADCRL;
    Disable_ADC;
    CKDIV = 0x00;
    // 计算采样数据

    // *****************计算温度******************************


    // ***************** 根据带隙电压计算供电电压******************************
    value = (4096.0 / ADC_Value[3]) * Bandgap_Voltage / 1.0;
    value =  value /1000;
    if (init == 0)
        Vcc_value = value;              // 未初始化

    Vcc_value = Vcc_value - Vcc_value / FIR_NUM + value / FIR_NUM; //电源

    value = (ADC_Value[1]) * Vcc_value /4095;//电解电流对应电压
    // 未初始化
    if (init == 0)
        Now_current = value/11/RS;
    value = value/11/RS;
    Now_current = Now_current - Now_current / FIR_NUM + value / FIR_NUM;
    //Now_current = Now_current;
    // ***************** 根据带隙电压计算电池电压******************************
    value = (ADC_Value[2]) * Vcc_value /4095*2;
    if (init == 0)
        Vbat_value = value;             // 未初始化
    Vbat_value = Vbat_value - Vbat_value / FIR_NUM + value / FIR_NUM; //电池电压

    // ***************** 计算电解电压******************************
    res = ADC_Value[0] * Vcc_value / 4095.0 * 11.0;
    if (init == 0)
        Ele_value = res;
    //	res = res*11;
    Ele_value = Ele_value - Ele_value / FIR_NUM + res / FIR_NUM;
    Ele_value = Ele_value;
    // 初始化状态结束
    init = 1;
}

// 蜂鸣器间隔定时器
void Timer1_init(void)
{
    TIMER1_MODE1_ENABLE;

    clr_T1M;                            // 1/12 system clock
    //set_T1M;                      // system clock

    TH1 = TIMER_DIV12_VALUE_10ms / 256;             // 延时10ms
    TL1 = TIMER_DIV12_VALUE_10ms % 256;             // 延时10ms

    set_ET1;              // enable Timer1 interrupt
    set_EA;               // enable interrupts
    set_TR1;              // Timer1 run

    Speak_Flage = 0;            // 开机声
}
/*******************************************************************
功能：控制设备的开关
参数：dev：设备类型
			LED_BLUE=1
			LED_RED=4
			LED_GREEN=2
			PUMP=8
			DIA1=0x10
			SWITCH=0x40
	level:电平


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
// 蜂鸣器鸣响
void Speak_value(void)
{
    static uint8_t codenum = 0;
    static uint16_t music = 0;

    // 蜂鸣器
    switch (Speak_Flage)
    {
    case 0: // 关闭
        Init_pwm_code(2000, 0);
        codenum = 0;
        break;
    case 1: // 开机音
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
    case 2: // 按键音
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
    case 3: // 电解结束
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
    case 4: // 电量低
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
    case 5: // 异常报警
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
    // 计数器增加

}
static uint32_t key_time_start = 0,key_time_star2 = 0;   // 按键检测时间     START   设定按键
unsigned char update=0,update2;   // 按键检测时间     START   设定按键
unsigned char key_hit_status=2;
/***********************************************************************************
 * 函数名：Button_Read(void)
 * 描述  ：读取按键数据
 * 输入  ：无1
 * 输出  ：无
************************************************************************************/
uint8_t Button_Read(unsigned char flag)
{
    //static uint32_t tick_tmp;


    // 按键滤波时间计时

//    static unsigned char key_hit_flag;

    if (Key_Set == 1 && Key_Pump == 1)//同时按下无效
    {
        return KEY_START_UP;
    }
    else
    {

        // START按键按下
        if(Key_Set==1)												// SET按键按下
        {
            if(key_hit_status==0)
                key_hit_status = 1;
            if((Key_Status & 0x0f)==KEY_START_HOLD)				// 长按键
            {
                update=1;
                Key_Status = Key_Status & 0xf0;
                Key_Status = Key_Status | KEY_START_HOLD; 		  //				//
            }
            else if((Key_Status & 0x0f)==KEY_START_DOWN||(Key_Status & 0x0f)==KEY_START_MID)   // 前一次是按键按下
            {
                if(key_time_start++>BUTTON_LONG_TIME)				// 检测到长按键
                {
                    update=1;
                    Key_Status = Key_Status & 0xf0;
                    Key_Status = Key_Status | KEY_START_HOLD; 		  //
                }
                else if(key_time_start++>BUTTON_LONG_TIME*3)				// 检测到长按键
                {
                    update=1;
                    Key_Status = Key_Status & 0xf0;
                    Key_Status = Key_Status | KEY_START_MID; 		  //
                }
            }
            else 																// 前一次没有按键按下
            {
                if(key_time_start++>BUTTON_FILTER_TIME)			// 按键通过滤波检测
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
            // Key_Status=KEY_START_UP;							// 按键抬起
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

        // START按键按下
        if(Key_Pump==1)												// SET按键按下
        {
            if((Key_Status & 0xf0)==KEY_HSTART_HOLD)				// 长按键
            {
                update2=1;
                Key_Status = Key_Status & 0x0f;
                Key_Status = Key_Status | KEY_HSTART_HOLD; 		  //				//
            }
            else if((Key_Status & 0xf0)==KEY_HSTART_DOWN)   // 前一次是按键按下
            {
                if(key_time_star2++>BUTTON_LONG_TIME)				// 检测到长按键
                {
                    update2=1;
                    Key_Status = Key_Status & 0x0f;
                    Key_Status = Key_Status | KEY_HSTART_HOLD; 		  //
                }
            }
            else 																// 前一次没有按键按下
            {
                if(key_time_star2++>BUTTON_FILTER_TIME)			// 按键通过滤波检测
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

            if((Key_Status & 0xf0)==KEY_HSTART_HOLD)				// 长按键
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


//        if (Key_Pump == 1)                                           // 水泵按键
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
 * 函数名：Button_Read(void)
 * 描述  ：读取按键数据
 * 输入  ：无1
 * 输出  ：无
************************************************************************************/
uint8_t Button_Read2(unsigned char flag)
{
    //static uint32_t tick_tmp;


    // 按键滤波时间计时

//    static unsigned char key_hit_flag;

    if (Key_Set == 1 && Key_Pump == 1)//同时按下无效
    {
        return KEY_START_UP;
    }
    else
    {

        // START按键按下
        if(Key_Set==1)												// SET按键按下
        {

            if(key_time_start++>BUTTON_FILTER_TIME)			// 按键通过滤波检测
            {


                key_down_flag = 1;
            }
            if(key_time_start++>BUTTON_LONG_TIME)			// 按键通过滤波检测
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

        // START按键按下
        if(Key_Pump==1)												// SET按键按下
        {

            if(key_time_star2++>BUTTON_FILTER_TIME) 		// 按键通过滤波检测
            {


                key_down_flag2 = 1;
            }
            if(key_time_star2++>BUTTON_LONG_TIME)			// 按键通过滤波检测
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
    Key_Status = Key_Status | KEY_START_UP; // 按键抬起
    key_time_start = 0;
    Key_Status = Key_Status & 0x0f;
    Key_Status = Key_Status | KEY_HSTART_UP; // 按键抬起
    key_time_star2 = 0;
}
unsigned char Charge_Process()
{
    static uint32_t charge_time = 0;
    static uint32_t stdby_time = 0;
    static unsigned char red_flag;
    static unsigned char charge_status;
    // 检测充电模式
    if (Charge_Set == 1)//充电模式
    {
        Charge_Mode = 5;
        if(charge_status==1)
        {
            if(charge_time%100 == 0)
            {
                if(red_flag == 0)//充电时，定时刷新红灯
                    red_flag = 1;
                else
                    red_flag = 0;
                Equipment_Ctrl_Set(LED_RED, red_flag);//红灯闪
                Equipment_Ctrl_Set(LED_GREEN, 1);//绿灯灭
            }

            if (charge_time++ > CHARGE_LONG_TIME) //
            {
                charge_time = 0;         //
            }

        }
        if (Vbat_value <= MAX_VBAT && Vbat_value >= MIN_VBAT)
        {
            charge_status = 1;
            Charge_Mode = 1;   // 充电
        }
        else if (Vbat_value > MAX_VBAT)
        {
            if (stdby_time++ > FCHARGE_LONG_TIME)// 充满
            {
                Equipment_Ctrl_Set(LED_RED, 1);//红灯灭
                Equipment_Ctrl_Set(LED_GREEN, 0);//绿灯亮
                charge_status = 0;
                stdby_time = BUTTON_LONG_TIME;
                Charge_Mode = 2;// 充满;
            }
            else
            {
                charge_status = 1;
                Charge_Mode = 1;// 充电
            }
        }
        else
        {
            Charge_Mode = 3; //故障
            stdby_time = 0;
            charge_time = 0;
            charge_status = 0;
        }

    }
    else
    {
        stdby_time = 0;
        charge_time = 0;
        Charge_Mode = 0;//没充电;
    }
    return Charge_Mode;

}


//发送一个字节数据给595再并行输出,data1控制数码管显示，data2控制外部设备
void SendTo595(uint8_t data1, uint8_t data2)
{
    char i = 0;
    //Clear_Con = 1;
    for (i; i < 16; i++)
    {

        if(i<8)
        {

            if(0x80 & data1)
                DAT_Con = 1; //&为按位运算符，即全1为1，有0为0，上式也就是 （1000 0000）&（1111 1111）=1000 0000，若高位为1则是1高位为0则这个式子为0
            else
                DAT_Con = 0;
            data1 = data1 <<1; //左移一位 将高位补给低位，如果二进制数为01010101 那么_crol_(1) 为10101010
            CLK_Con = 1;          //上升沿让串行输入时钟变成高电平 并延时一个时钟周期
            _nop_();
        }
        else
        {
            CLK_Con = 0;
            if(0x80 & data2)
                DAT_Con = 1; //&为按位运算符，即全1为1，有0为0，上式也就是 （1000 0000）&（1111 1111）=1000 0000，若高位为1则是1高位为0则这个式子为0
            else
                DAT_Con = 0;
            data2 = data2 <<1; //左移一位 将高位补给低位，如果二进制数为01010101 那么_crol_(1) 为10101010

        }
        CLK_Con = 0;
        _nop_();
        CLK_Con = 1;          //上升沿让串行输入时钟变成高电平 并延时一个时钟周期
        _nop_();

    }

    /*位移寄存器完毕，转移到存储寄存器*/
    RCK_Con = 0;         //上升沿，存储寄存器变为高电平 延迟两个时钟周期
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
 *74HC595 发送一个字节
 *即往74HC595的DS引脚发送一个字节
*/
void HC595_Send_Byte(unsigned char byte)
{
    unsigned char i;
    for (i = 0; i < 8; i ++)  //一个字节8位，传输8次，一次一位，循环8次，刚好移完8位
    {
        /****  步骤1：将数据传到DS引脚    ****/
        if (byte & 0x80)        //先传输高位，通过与运算判断第八是否为1
            HC595_Data_High();    //如果第八位是1，则与 595 DS连接的引脚输出高电平
        else                    //否则输出低电平
            HC595_Data_Low();

        /*** 步骤2：SHCP每产生一个上升沿，当前的bit就被送入移位寄存器 ***/
        HC595_SHCP_Low();   // SHCP拉低
        _nop_();
        _nop_();;           // 适当延时
        HC595_SHCP_High();  // SHCP拉高， SHCP产生上升沿
        _nop_();
        _nop_();;

        byte <<= 1;		// 左移一位，将低位往高位移，通过	if (byte & 0x80)判断低位是否为1
    }
}

/**
 *74HC595输出锁存 使能
**/
void HC595_CS(void)
{
    /**  步骤3：STCP产生一个上升沿，移位寄存器的数据移入存储寄存器  **/
    HC595_STCP_Low();   // 将STCP拉低
    _nop_();
    _nop_();
    _nop_();           // 适当延时
    HC595_STCP_High();  // 再将STCP拉高，STCP即可产生一个上升沿
    _nop_();
    _nop_();
    _nop_();
}

/**
 *发送多个字节
 *便于级联时数据的发送
 *级联N级，就需要发送N个字节控制HC595
***/
void HC595_Send_Multi_Byte(uint8_t data1, uint8_t data2)
{
    unsigned char i;
    unsigned char buf[2];
    buf[0] = data1;
    buf[1] = data2;
    Clear_Con = 0;
    for (i = 0; i < 2; i ++ ) // len 个字节
    {
        HC595_Send_Byte(buf[i]);
    }

    HC595_CS(); //先把所有字节发送完，再使能输出
}

/***********************************************************************************
 * 函数名：void Shutdown_Mode(void)
 * 描述  ：关机
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
void Shutdown_Mode(void)
{
    Led_Ram = 0xff;
    Set_Speak_Value(1);
    Equipment_Ctrl_Set(0, 6);
    //Led_Display();                                  // LED显示
    DJ_Con = 0;                                             // 关闭电解
    Pow_Con = 0;                                        // 关闭电源

    // 保存数据
    All_Use_Time++;                                 // 累计使用次数
    Write_DATAFLASH_BYTE();                 // 保存数据
    Equipment_Ctrl_Set(DIA1, 0);//阀门开
    Led_Ram = 0x04;
    HC595_Send_Multi_Byte(table[Led_Ram], equipment);
    delay_ms(200);


}

void display_init(void)
{

    Set_Speak_Value(1);
    Equipment_Ctrl_Set(0, 6);
    //Led_Display();                                  // LED显示
    DJ_Con = 0;                                             // 关闭电解                                        // 关闭电源
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
//Equipment_Ctrl_Set(DIA1, 1);//阀门开
    Clear_Con = 0;
    //equipment = 0x02;
    // Equipment_Ctrl_Set(DIA1, 1);//阀门开
//Led_Ram=2;
    HC595_Send_Multi_Byte(table[Led_Ram], equipment);
}



// 用户操作
static unsigned char key_Count;
void Work_In_Set(uint8_t keycode)               //
{
    static uint8_t key = 0;//按键状态临时获取
    static uint8_t init_time = 0;
    static uint8_t key_num = 0;                       // 连续按键检测 5次点按 一次常按键后进入ISP
    static uint16_t NoWaterTick;
    static uint8_t key_hit_flag;
    static uint32_t timeout,timeout2 ;
    static uint32_t light_flag;
    float tmpa;
    unsigned char tmpb;
    // 主程序
    switch (Work_Mode)
    {
    case 0://开机自检
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
            else if(Vbat_value <=BATTER_ALARM)//低压报警
            {
                if((systick-timeout)>=500)
                {

                    Equipment_Ctrl_Set(PUMP|SWITCH|DIA1|LED_BLUE, 0);//泵关
                    Work_Mode = 4;//异常工作模式
                    if(Charge_Set == 0)
                    {
                        Equipment_Ctrl_Set(LED_GREEN, 1);//泵关
                        Equipment_Ctrl_Set(LED_RED, 0);//红灯亮
                        Set_Speak_Value(4);//
                    }
                    timeout = systick;
                    Led_Ram = 0x04;
                }
            }
            else//正常启动
            {
                Equipment_Ctrl_Set(PUMP|SWITCH|LED_RED|DIA1|LED_BLUE, 0);//泵关
                Equipment_Ctrl_Set(LED_GREEN, 0);//绿灯亮
                Led_Ram = 0x04;
                Work_Mode = 1;//正常工作模式
                //Set_Speak_Value(1);//
            }
        }
        key = keycode&0x0f;
//            //******************** 开关机按键，短按电解，长按开关机***************************

        if(key==KEY_START_HOLD) //关机
        {
            Shutdown_Mode();
            update =0;
            Send_Data_To_UART0(3,DEBUG);
        }

    }
    break;
    case 1: //正常工作模式
    {

        if(Charge_Mode == 0)
        {
            key = keycode&0x0f;
//            //******************** 开关机按键，短按电解，长按开关机***************************
            if(update==1)
            {
                update =0;
                if(key==KEY_START_DOWN)//短按一次
                {
                    //if(Key_Set ==0)
                    {
                        key_Count ++;

                        if(Ele_Flage==0&&vbat_warn_flag ==0)//开始电解
                        {
                            if( pump_flag == 0)
                            {
                                equipment = equipment|0x01|0x80;
                                Ele_Time = 0;
                                Ele_Flage = 1;
                                DJ_Con = 1;//电解
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
                                Equipment_Ctrl_Set(0x80, 1);//灯开
                                Equipment_Ctrl_Set(0x01, 1);//灯开
                                Equipment_Ctrl_Set(DIA1, 1);//阀门开
                                Equipment_Ctrl_Set(SWITCH, 0);//阀门关
                            }

                        }
                        else if(Ele_Flage==1)
                        {

                            {

                                Ele_Time = 0;
                                Ele_Flage = 0;
                                Send_Data_To_UART0(2,DEBUG);
                                Led_Ram = 0;
                                DJ_Con = 0;//取消电解
                                Led_Ram = 0x04;
                                Equipment_Ctrl_Set(0x80, 0);//阀门开
                                Equipment_Ctrl_Set(0x01, 0);//阀门开

                                Equipment_Ctrl_Set(SWITCH, 0);//阀门关											 }

                            }
                        }
                        //  Ele_Flage = 0;
                        //显示
                    }

                }
                else if(key==KEY_START_HOLD) //关机
                {
                    Shutdown_Mode();
                    Send_Data_To_UART0(3,DEBUG);
                }
            }
            if(update2==1)
            {
                update2 = 0;
                //******************** 喷雾排水按键，短按喷雾**************************
                key = keycode&0xf0;
                if(key==KEY_HSTART_DOWN||key==KEY_HSTART_HOLD)//短按一次
                {
                    if(pump_flag == 1)//正在喷雾
                    {

                        if(key==KEY_HSTART_HOLD)
                        {
                            Key_Status = Key_Status & 0x0f;
                            Key_Status = Key_Status | KEY_HSTART_UP;
                        }
												else
												{
                        Equipment_Ctrl_Set(PUMP, 0);//泵关;
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
                            Equipment_Ctrl_Set(PUMP, 1);//泵开;
                        }

                    }
                }
                else if(key==KEY_HSTART_HOLD) //喷雾
                {
                    if(pump_flag == 0)
                    {
                        if(Ele_Flage == 0&&vbat_warn_flag ==0)
                        {
                            pump_flag = 1;
                            Send_Data_To_UART0(6,DEBUG);
                            Equipment_Ctrl_Set(PUMP, 1);//泵开;
                        }
                    }
//										else
//									                    {
//                        Equipment_Ctrl_Set(PUMP, 0);//泵关;
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

    if(Charge_Mode != 0)//充电待机不工作，只能下载程序
    {

        vbat_warn_flag = 1;
        Work_Mode = 6;
        key = keycode&0x0f;
        if(key==KEY_START_DOWN)//短按一次
        {
            key_hit_flag = 1;
            if(timeout2 <=1000)
            {
                key_num ++;
                if(key_num>=10)//超过5次进入升级模式
                {
                    Send_Data_To_UART0(7,DEBUG);
                    Set_Speak_Value(6);//升级模式声音
                    ISP_Rest();//进入ldrom升级模式
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
        if(Charge_Mode == 2) //充满
        {
            Work_Mode = 4;
            // Shutdown_Mode();//关机
            // Charge_Mode = 0;
        }
        if(Charge_Mode == 3) //故障
        {
            Work_Mode = 4;
            Ele_Flage=0;
            Shutdown_Mode();//关机
            // Charge_Mode = 0;
        }
    }
    else //正常模式
    {
        key_hit_flag = 0;
        //timeout = 0;
        if(Ele_Flage == 1)//电解计时
        {
            Ele_Time++;
            if(Ele_Time%6000==0)//1分钟刷新一次
            {
                if(Led_Ram>0)
                    Led_Ram = Led_Ram - 1;
            }

            if(Ele_Time>=ELE_PERIOD)//电解时间到
            {
                light_flag = 1;
                Ele_Flage = 0;
                Ele_Time = 0;
                Led_Ram = 0;
                DJ_Con = 0;
                Equipment_Ctrl_Set(0x80, 0);//灯关
                Equipment_Ctrl_Set(0x01, 0);//灯关
                Equipment_Ctrl_Set(SWITCH, 1);//阀门开
                Led_Ram = 0x04;
                if(Ele_value>MIN_TDSVALUE)  // 水质差
                {

                    //  Send_Data_To_UART0(8,DEBUG);
                    Set_Speak_Value(5);//报警
                    TDS_Alarm_Num++;                    // TDS报警数值累计增加
                }
                else
                {
                    //  Send_Data_To_UART0(9,DEBUG);

                    Set_Speak_Value(3);//电解结束
                }
            }

            if(Now_current<=MIN_CURRENT||(Ele_Time>=4000&&Now_current<=0.8))//无水检测
            {
                NoWaterTick++;
                if(NoWaterTick>=1100)//超过10s
                {
                    Led_Ram = 0x04;
                    NoWaterTick = 0;
                    Ele_Flage = 0;
                    Ele_Time = 0;
                    DJ_Con = 0;
                    Equipment_Ctrl_Set(0x80, 0);//灯开
                    Equipment_Ctrl_Set(0x01, 0);//灯开
                    //Send_Data_To_UART0(10,DEBUG);
                    Equipment_Ctrl_Set(SWITCH, 0);//阀门关
                    Set_Speak_Value(5);//异常报警
                }
            }
            else  if(Now_current>=MAX_CURRENT)//短路保护
            {
                NoWaterTick++;
                if(NoWaterTick>=500)//超过10s
                {
                    Led_Ram = 0x04;
                    NoWaterTick = 0;
                    Ele_Flage = 0;
                    Ele_Time = 0;
                    DJ_Con = 0;
                    Equipment_Ctrl_Set(0x80, 0);//灯开
                    Equipment_Ctrl_Set(0x01, 0);//灯开
                    // Send_Data_To_UART0(11,DEBUG);
                    Equipment_Ctrl_Set(SWITCH, 0);//阀门关
                    Set_Speak_Value(5);//异常报警
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
                    Equipment_Ctrl_Set(SWITCH, 0);//阀门关
                    Ele_Time = 0;
                }
            }
            else
                Equipment_Ctrl_Set(SWITCH, 0);//阀门关
        }

    }

    if(Charge_Mode != 0)//充电模式停机工作
    {
        vbat_warn_flag = 1;
        Ele_Flage = 0;
        Ele_Time = 0;
        Led_Ram = 0;
        DJ_Con = 0;
        Led_Ram = 0x04;
        Equipment_Ctrl_Set(PUMP|SWITCH|DIA1|LED_BLUE, 0);//设备都关
        // Equipment_Ctrl_Set( LED_GREEN, 0);//设备都关
        // Equipment_Ctrl_Set( LED_RED, 1);//设备都关
    }


    //非初始化模式电量检测
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
        else if(Vbat_value <=BATTER_ALARM)//低压报警
        {
            if((systick-timeout)>=3000)
            {
                vbat_warn_flag = 1;

                //  Send_Data_To_UART0(13,DEBUG);
                // Led_Ram = 0x04;
                //  Equipment_Ctrl_Set(PUMP|SWITCH|DIA1|LED_BLUE, 0);//泵关
                Equipment_Ctrl_Set(LED_RED, 0);//红灯亮
                Equipment_Ctrl_Set(LED_GREEN, 1);//绿灯灭
                // Set_Speak_Value(4);// 电量低
                //    Work_Mode = 4;//异常工作模式
                timeout = systick;
            }
        }
        else
        {

            timeout = systick;
            if(Charge_Mode == 0)
            {
                vbat_warn_flag = 0;
                Equipment_Ctrl_Set(LED_RED, 1);//红灯灭
                Equipment_Ctrl_Set(LED_GREEN, 0);//绿灯亮
            }


        }

    }

}
// 发送工作参数到检测设备
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
    // 写入用户数据
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
