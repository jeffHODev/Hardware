/****************************************************************************************
 
*��������:IAR for stm8 v6.5.3
 
*Ӳ��ƽ̨:STM8L-DISCOVERY
 
*����˵��:ʹ��STM8L-DISCOVERYҺ������ʾһ��6λ����
 
*��    �ߣ�����
 
****************************************************************************************/
 
#include"iostm8l152c6.h"
 
#include"stdint.h"
 
/*  =========================================================================
 
                                 LCD MAPPING
 
    =========================================================================
 
            A
 
     _  ----------
 
COL |_| |\   |J  /|
 
       F| H  |  K |B
 
     _  |  \ | /  |
 
COL |_| --G-- --M--
 
        |   /| \  |
 
       E|  Q |  N |C
 
     _  | /  |P  \|   
 
DP  |_| -----------  
 
            D      
 
*/
 
 
#define a 0x01
 
#define b 0x02
 
#define c 0x04
 
#define d 0x08
 
#define e 0x10
 
#define f 0x20
 
#define g 0x40
 
#define m 0x80
 
 
const uint8_t LCD_Tab[10] = {
 
                a + b + c + d + e + f,                        // Displays "0"
 
                b + c,                                        // Displays "1"
 
                a + b + m + g + e + d,                        // Displays "2"
 
                a + b + m + g + c + d,                        // Displays "3"
 
                f + g + m + b + c,                        // Displays "4"
 
                a + f + g + m + c +d,                        // Displays "5"
 
                a + f + e + d + c + g + m ,                // Displays "6"
 
                a + b + c,                                // Displays "7"
 
                a + b + c + d + e + f + g + m,                // Displays "8"
 
                a + b + c + d + f + g + m                // Displays "9"
 
 
};
 
 
/******************************************************************************************************
 
*  ��    �ƣ�void LCD_Config(void)
 
*  ��    �ܣ�����DAC,����DMA,��ʹ��TIM4����,Ҳ�����������,д��DHR�����ݱ���������DOR�Ĵ���,
 
*            ���������Ӧ��ѹ
 
*  ��ڲ�������
 
*  ���ڲ�������
 
*  ˵    ����STM8L152C6�����е�����MCU,ֻ��һ·DAC���,�������ΪPF0
 
*  ��    ������
 
******************************************************************************************************/
 
void LCD_Config(void)
 
{ 
 
//------��LCD/RTCʱ��------
 
  CLK_PCKENR2_PCKEN22=1;//��RTCʱ��,LCDˢ��Ƶ�����ʱ���й�
 
  CLK_PCKENR2_PCKEN23=1;//��LCDʱ��,��дLCD�Ĵ����õ���ʱ��
 
  
 
//---ѡ��LSE��ΪRTCʱ��---
 
  CLK_CRTCR_RTCSEL0=0;
 
  CLK_CRTCR_RTCSEL1=0;
 
  CLK_CRTCR_RTCSEL2=0;
 
  CLK_CRTCR_RTCSEL3=1;
 
/* 0000: No clock selected
 
   0001: HSI clock used as RTC clock source
 
   0010: LSI clock used as RTC clock source
 
   0100: HSE clock used as RTC clock source
 
   1000: LSE clock used as RTC clock sourc*/
 
  
 
//----����RTCʱ�ӷ�Ƶֵ----
 
  CLK_CRTCR_RTCDIV0=0;
 
  CLK_CRTCR_RTCDIV1=0;
 
  CLK_CRTCR_RTCDIV2=0;
 
/*000: RTC clock source/1
 
  001: RTC clock source /2
 
  010: RTC clock source /4
 
  011: RTC clock source /8
 
  100: RTC clock source /16
 
  101: RTC clock source /32
 
  110: RTC clock source /64
 
  111: RTC clock source /128*/
 
  
 
//----����LCDԤ��Ƶֵ----  
 
  LCD_FRQ_PS0=0;// 2^PS[3:0]
 
  LCD_FRQ_PS1=0;//��ƵֵΪ1
 
  LCD_FRQ_PS2=0;
 
  LCD_FRQ_PS3=0;
 
  
 
//----����LCD��Ƶֵ----  
 
  LCD_FRQ_DIV0=1;//DIV[3:0]+16
 
  LCD_FRQ_DIV1=1;//��ƵֵΪ15+16=31
 
  LCD_FRQ_DIV2=1;
 
  LCD_FRQ_DIV3=1;  
 
 
 
//���Ϸ�Ƶֵ������,��Ϊ�˵õ��ʺϵ�LCD��ˢ��Ƶ��,��������Ƶֵ,�ᵼ��
 
//LCDˢ��Ƶ�ʱ��,�ῴ��LCD��ʾ������˸
 
//����,���ǽ�PS[3:0]����Ϊ0011,�ῴ��Һ����˸  
 
  
 
//----1/4 duty----  
 
  LCD_CR1_DUTY0=1;//1/4 duty
 
  LCD_CR1_DUTY1=1;
 
/* Duty ratio selection
 
   00: Static duty
 
   01: 1/2 duty
 
   10: 1/3 duty
 
   11: 1/4 duty      */
 
 
//----1/3 bias----     
 
  LCD_CR1_B2=0;//1/3 bias
 
/* 0: 1/3 bias
 
   1: 1/2 bias  */
 
 
//----�ڲ���ѹԴ----   
 
  LCD_CR2_VSEL=0;
 
  
 
//----�����ŵ�SEG����----     
 
// LCD_PM0=0xFF;//ͷ�ļ�����ط��������,�޷�ֱ����LCD_PM0д������
 
// LCD_PM1=0xFF;//PM0�Ĵ����������,����PM1Ҳ�޷�ֱ��д��
 
// LCD_PM2=0xFF;//PM0�Ĵ����������,����PM2Ҳ�޷�ֱ��д��
 
  *((uint8_t *)0x5404)=0xFF;//ֱ����LCD_PM0�Ĵ����ĵ�ַд������
 
  *((uint8_t *)0x5405)=0xFF;//ֱ����LCD_PM1�Ĵ����ĵ�ַд������
 
  *((uint8_t *)0x5406)=0xFF;//ֱ����LCD_PM2�Ĵ����ĵ�ַд������
 
  
 
//----To set contrast to mean value----   
 
  LCD_CR2_CC0=0;//�Աȶ�
 
  LCD_CR2_CC1=1;
 
  LCD_CR2_CC2=0;
 
/*  000: VLCD0  2.6V
 
    001: VLCD1  2.7V
 
    010: VLCD2  2.8V
 
    011: VLCD3  2.9V
 
    100: VLCD4  3.0V
 
    101: VLCD5  3.1V
 
    110: VLCD6  3.2V
 
    111: VLCD7    */
 
        
 
//----Dead time 0----         
 
  LCD_CR3_DEAD0=0;//no dead time
 
  LCD_CR3_DEAD1=0;  
 
  LCD_CR3_DEAD2=0;
 
//----LCD_PulseOnDuration_1----
 
  LCD_CR2_PON0=1; 
 
  LCD_CR2_PON1=0;  
 
  LCD_CR2_PON2=0;          
 
/*  000: 0 CLKps pulses
 
    001: 1 CLKps pulses
 
    010: 2 CLKps pulses
 
    011: 3 CLKps pulses
 
    100: 4 CLKps pulses
 
    101: 5 CLKps pulses
 
    110: 6 CLKps pulses
 
    111: 7 CLKps pulses  */
 
        
 
//----Enable LCD peripheral----        
 
  LCD_CR3_LCDEN=1;
 
}
 
 
/******************************************************************************************************
 
*  ��          �ƣ�LCD_DisplayNum(uint8_t number)
 
*  ��            �ܣ����ƶ�ʽҺ������������ʾ����
 
*  ��ڲ�����number:Ҫ��ʾ������
 
*  ���ڲ�������
 
*  ˵          �����������ֵĳ��ȣ��ж�Ҫ��ʾ�ĳ���,���ȴ���6λ,ֻ��ʾ����λ
 
*  ��          ������
 
******************************************************************************************************/
 
void LCD_DisplayNum(uint32_t number)
 
{
 
  uint8_t cnts=0,tmp=0;
 
  
 
  if(number<10)cnts=1;
  else if(number<100)cnts=2;
  else if(number<1000)cnts=3;
  else if(number<10000)cnts=4;
  else if(number<100000)cnts=5;
  else if(number<(uint32_t) 1000000)cnts=6;
  else cnts=6;
  //�ж���Ҫ��ʾ���ֵĳ��ȣ�ȷ����LCD������Ҫ��λ��
  switch(cnts)
  {
    case 6:
           tmp = LCD_Tab[number%1000000/100000];
           ((tmp&m)==0)?(LCD_RAM0&=~0x02):(LCD_RAM0 |=0x02) ;
           ((tmp&e)==0)?(LCD_RAM0&=~0x01):(LCD_RAM0 |=0x01) ;
           ((tmp&g)==0)?(LCD_RAM2&=~0x80):(LCD_RAM2 |=0x80) ;
           ((tmp&b)==0)?(LCD_RAM2&=~0x40):(LCD_RAM2 |=0x40) ;
           ((tmp&f)==0)?(LCD_RAM6&=~0x08):(LCD_RAM6 |=0x08) ;
           ((tmp&a)==0)?(LCD_RAM6&=~0x04):(LCD_RAM6 |=0x04) ;
           ((tmp&c)==0)?(LCD_RAM3&=~0x20):(LCD_RAM3 |=0x20) ;
           ((tmp&d)==0)?(LCD_RAM3&=~0x10):(LCD_RAM3 |=0x10) ;                      
    case 5:
           tmp = LCD_Tab[number%100000/10000];
           ((tmp&m)==0)?(LCD_RAM0&=~0x08):(LCD_RAM0 |=0x08) ;
           ((tmp&e)==0)?(LCD_RAM0&=~0x04):(LCD_RAM0 |=0x04) ;
           ((tmp&g)==0)?(LCD_RAM2&=~0x20):(LCD_RAM2 |=0x20) ;
           ((tmp&b)==0)?(LCD_RAM2&=~0x10):(LCD_RAM2 |=0x10) ;
           ((tmp&f)==0)?(LCD_RAM6&=~0x02):(LCD_RAM6 |=0x02) ;
           ((tmp&a)==0)?(LCD_RAM6&=~0x01):(LCD_RAM6 |=0x01) ;
           ((tmp&c)==0)?(LCD_RAM3&=~0x80):(LCD_RAM3 |=0x80) ;
           ((tmp&d)==0)?(LCD_RAM3&=~0x40):(LCD_RAM3 |=0x40) ;           
           
    case 4:
           tmp = LCD_Tab[number%10000/1000];
           ((tmp&m)==0)?(LCD_RAM0&=~0x20):(LCD_RAM0 |=0x20) ;
           ((tmp&e)==0)?(LCD_RAM0&=~0x10):(LCD_RAM0 |=0x10) ;
           ((tmp&g)==0)?(LCD_RAM2&=~0x08):(LCD_RAM2 |=0x08) ;
           ((tmp&b)==0)?(LCD_RAM2&=~0x04):(LCD_RAM2 |=0x04) ;
           ((tmp&f)==0)?(LCD_RAM5&=~0x80):(LCD_RAM5 |=0x80) ;
           ((tmp&a)==0)?(LCD_RAM5&=~0x40):(LCD_RAM5 |=0x40) ;
           ((tmp&c)==0)?(LCD_RAM4&=~0x02):(LCD_RAM4 |=0x02) ;
           ((tmp&d)==0)?(LCD_RAM4&=~0x01):(LCD_RAM4 |=0x01) ;           
    case 3:
           tmp = LCD_Tab[number%1000/100];
           ((tmp&m)==0)?(LCD_RAM0&=~0x80):(LCD_RAM0 |=0x80) ;
           ((tmp&e)==0)?(LCD_RAM0&=~0x40):(LCD_RAM0 |=0x40) ;
           ((tmp&g)==0)?(LCD_RAM2&=~0x02):(LCD_RAM2 |=0x02) ;
           ((tmp&b)==0)?(LCD_RAM2&=~0x01):(LCD_RAM2 |=0x01) ;
           ((tmp&f)==0)?(LCD_RAM5&=~0x20):(LCD_RAM5 |=0x20) ;
           ((tmp&a)==0)?(LCD_RAM5&=~0x10):(LCD_RAM5 |=0x10) ;
           ((tmp&c)==0)?(LCD_RAM4&=~0x08):(LCD_RAM4 |=0x08) ;
           ((tmp&d)==0)?(LCD_RAM4&=~0x04):(LCD_RAM4 |=0x04) ;
   case 2:
           tmp = LCD_Tab[number%100/10];
           ((tmp&m)==0)?(LCD_RAM1&=~0x02):(LCD_RAM1 |=0x02) ;
           ((tmp&e)==0)?(LCD_RAM1&=~0x01):(LCD_RAM1 |=0x01) ;
           ((tmp&g)==0)?(LCD_RAM1&=~0x80):(LCD_RAM1 |=0x80) ;
           ((tmp&b)==0)?(LCD_RAM1&=~0x40):(LCD_RAM1 |=0x40) ;
           ((tmp&f)==0)?(LCD_RAM5&=~0x08):(LCD_RAM5 |=0x08) ;
           ((tmp&a)==0)?(LCD_RAM5&=~0x04):(LCD_RAM5 |=0x04) ;
           ((tmp&c)==0)?(LCD_RAM4&=~0x20):(LCD_RAM4 |=0x20) ;
           ((tmp&d)==0)?(LCD_RAM4&=~0x10):(LCD_RAM4 |=0x10) ;
   case 1: 
           tmp = LCD_Tab[number%10];
           ((tmp&m)==0)?(LCD_RAM1&=~0x08):(LCD_RAM1 |=0x08) ;
           ((tmp&e)==0)?(LCD_RAM1&=~0x04):(LCD_RAM1 |=0x04) ;
           ((tmp&g)==0)?(LCD_RAM1&=~0x20):(LCD_RAM1 |=0x20) ;
           ((tmp&b)==0)?(LCD_RAM1&=~0x10):(LCD_RAM1 |=0x10) ;
           ((tmp&f)==0)?(LCD_RAM5&=~0x02):(LCD_RAM5 |=0x02) ;
           ((tmp&a)==0)?(LCD_RAM5&=~0x01):(LCD_RAM5 |=0x01) ;
           ((tmp&c)==0)?(LCD_RAM4&=~0x80):(LCD_RAM4 |=0x80) ;
           ((tmp&d)==0)?(LCD_RAM4&=~0x40):(LCD_RAM4 |=0x40) ;
    break;
    default:break;
  }
}
void main(void)
{
  LCD_Config();
  LCD_DisplayNum(201609);
//  asm("rim");               //enable interrupts
  while(1)
  {
    asm("wfi");
  }
}

