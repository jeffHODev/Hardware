#include "bsp_led.h"
#include "system.h"
#include "delay.h"

__IO uint8_t LED_QS=1;						// ԭˮ��ȱˮ     0 1 2
__IO uint8_t LED_MS=1;						// ��ˮ��ˮ��			0 1 2
__IO uint8_t LED_TDS=1;						// ˮ��TDSָʾ��  0 1 2
__IO uint8_t LED_LX=5;						// ��о����ָʾ		0 ����о(ȫ��˸) 1 ��о����(�����˸0L)  2 ��о������(�����30L)   3��о1��(����50)  4��о2��(����166)  5��о3��(����335)
__IO uint8_t LED_GW=1;						// ԭˮˮ�¸�     0 1 2
__IO uint8_t LED_QX=0;						// ��ϴģʽ
																
__IO uint8_t LED_Brightness=7;	  // ���ȵ���
__IO uint8_t Display_Rama[3]={0x12,0x12,0x13};		// ��ʾ������ PH   10pH
__IO uint8_t Display_Ramb[3]={0x12,0x12,0x12};		// ��ʾ������ ORP  -385mv
//__IO uint8_t Display_Ramc[2]={0x12,0x12};				// ��ʾ������ �¶� 88  ��
__IO uint8_t Display_Ramc[2]={0x13,0x13};					// 2021 06 04 �����θ�

// LedSegs	DB	03FH, 006H, 05BH, 04FH, 066H, 06DH, 07DH, 007H, 07FH, 06FH, 077H, 07CH, 039H, 05EH, 079H, 071H
// LED��ʾ���붨��
const uint8_t Segcode[]=		// LED����
{
  0x3F,		// 0
  0x06,		// 1
  0x5B,		// 2
  0x4F,		// 3
  0x66,		// 4
  0x6D,		// 5
  0x7D,		// 6	
  0x07,		// 7
  0x7F,		// 8
  0x6F,		// 9
  0x77,		// A	0x0a
  0x7C,		// B	0x0b
  0x39,		// C	0x0c
  0x5E,		// D	0x0d
  0x79,		// E	0x0e	
  0x71,		// F	0x0f	
  0x76,		// H  0x10
  0x38,		// L  0x11
  0x40,		// -  0x12
  0x00,		// �� 0x13
	0x5C,		// o  0x14
	0x04,		// i  0x15
};

/********************************************************************************
 * ��������void Output_GPIO_Config(void)	 												
 * ����  �� I/O����	   														
 * ����  ����																	
 * ���  ����																	
 * ����  ���ڲ�����																
*********************************************************************************/
void bsp_led_init(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;

  /* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

/***********************************************************************************
 * ��������void TM1640_Refresh_Start(void)					// ��ʼˢ������	
 * ����  ��ˢ��DACͨ��
 * ����  ����
 * ���  ����
************************************************************************************/
static void TM1640_Refresh_Start(void)	// ��ʼˢ������
{
	TM1640_SCLK(1);												// ����CLK����
	TM1640_DIN(1);												// ����DINA����
	delay_us(10);													// ����ʱ
	TM1640_DIN(0);												// ����DINA����
	delay_us(10);													// ����ʱ
	TM1640_SCLK(0);												// ����CLK����
	delay_us(10);													// ����ʱ
}

/***********************************************************************************
 * ��������TM1640_Refresh_Stop(void)		
 * ����  ��ֹͣ��������
 * ����  ����
 * ���  ����
************************************************************************************/
static void TM1640_Refresh_Stop(void)		// ֹͣˢ������
{
	TM1640_SCLK(0);												// ����CLK����
	TM1640_DIN(0);												// ����DINA����
	delay_us(10);													// ����ʱ
	TM1640_SCLK(1);												// ����CLK����
	delay_us(10);													// ����ʱ
	TM1640_DIN(1);												// ����DINA����
	delay_us(10);													// ����ʱ
}

/***********************************************************************************
 * ��������void TM1640_Write_Cmd(uint8_t data)
 * ����  ����TM1640д������
 * ����  ����
 * ���  ����
************************************************************************************/
static void TM1640_Write_Cmd(uint8_t temp)
{
	uint8_t i;	

	for(i=0;i<8;i++)						// д��������  ��ַ�Զ���1	 0100 0000  0x40
	{
		TM1640_SCLK(0);						// ����CLK����
		delay_us(10);							// ����ʱ	
		if(temp&0x01)							// �ߵ�ƽ
		{
			TM1640_DIN(1);					// ����DINA����
		}
		else											// �͵�ƽ
		{
			TM1640_DIN(0);					// ����DINA����	
		}
		delay_us(10);							// ����ʱ	
		TM1640_SCLK(1);						// ����CLK����
		delay_us(10);							// ����ʱ
		temp=temp>>1;							// �����ƶ�1λ
	}
}

/***********************************************************************************
 * ��������TM1640_Write_Data(uint8_t data1,uint8_t data2,uint8_t data3,uint8_t data4)
 * ����  ����TM1640д������
 * ����  ����
 * ���  ����
************************************************************************************/
static void TM1640_Write_Data(uint8_t tempa)
{
	uint8_t i;

	for(i=0;i<8;i++)					// д��������  ��ַ�Զ���1	 1100 0000  0xC0	 ��ʼλ��Ϊ0
	{
		TM1640_SCLK(0);					// ����CLK����
		// д��ʾģ��a
		if(tempa&0x01)					// �ߵ�ƽ
		{
			TM1640_DIN(1);				// ����DINA����	
		}
		else										// �͵�ƽ
		{
			TM1640_DIN(0);				// ����DINA����	
		}
		delay_us(10);						// ����ʱ
		TM1640_SCLK(1);					// ����CLK����
		delay_us(10);						// ����ʱ
		tempa=tempa>>1;					// �����ƶ�1λ
	}
}

/***********************************************************************************
 * ��������Refresh_DAC_OUT(uint8_t channel)			
 * ����  ��ˢ��DACͨ��
 * ����  ����
 * ���  ����
************************************************************************************/
void Refresh_LED_Display(void)	// ˢ����ʾ
{
	uint8_t temp=0x00;	
	static uint8_t shift=0;
	
	if(Init_Falge==1) 													// ��ʼ����־(�ϵ��ʼ����־)
	{
//		Display_Ramc[1]=Set_Temp_Table[Set_Temp_Num]/10;		// ��ʾ�¶�
//		Display_Ramc[0]=Set_Temp_Table[Set_Temp_Num]%10;		// ��ʾ�¶�
		Display_Ramc[1]=0x13;		// 2021 06 04 �����θ�
		Display_Ramc[0]=0x13;		// 2021 06 04 �����θ�
	}
	else
	{
//		Display_Ramc[1]=0x12;											//
//		Display_Ramc[0]=0x12;											//
		Display_Ramc[1]=0x13;		// 2021 06 04 �����θ�
		Display_Ramc[0]=0x13;		// 2021 06 04 �����θ�
	}
	
	// �ж���ʾTDS����ORP
	if(Show_TDS_Value==1)												// ��ʾ������Ϣ
	{
		Display_Ramb[0]=(Yswater_Tds%1000)/100;		// ��ʾTDS
		Display_Ramb[1]=(Yswater_Tds%100)/10;
		Display_Ramb[2]=Yswater_Tds%10;		
		if(Set_Temp_Num!=0&&Work_Step==10)				// ֻ�г���ˮʱ�Ż�仯
		{
//			Display_Ramc[1]=Cswater_Temp/10;				// ��ˮ�¶�
//			Display_Ramc[0]=Cswater_Temp%10;
			Display_Ramc[1]=0x13;		// 2021 06 04 �����θ�
			Display_Ramc[0]=0x13;		// 2021 06 04 �����θ�
		}
	}	
	// ������������
	TM1640_Refresh_Start();			// ʹ���������� 
	TM1640_Write_Cmd(0x40);			// �������ݵ�ַд��
	TM1640_Refresh_Stop();			// ֹͣ��������
	// ���õ�ַ����
	TM1640_Refresh_Start();			// ʹ���������� 
	TM1640_Write_Cmd(0xC0);			// ��ʼ��ʾ��ַ(0xc0) 	
	// д��澯��־1	0xC0
	temp=0x00;
	// ԭˮ��ȱˮ
	if(LED_QS==1) temp=temp|0x03;
	else temp=temp&0xFC;
	// ԭˮˮ�¸�
	if(LED_GW==1) temp=temp|0x0C;
	else temp=temp&0xF3;	
	// ԭˮTDS�쳣
	if(LED_TDS==1) temp=temp|0x30;
	else temp=temp&0xCF;		
	// ��ˮ������
	if(LED_MS==1) temp=temp|0xC0;
	else temp=temp&0x3F;		
	TM1640_Write_Data(temp);
	// д��澯��־2	0xC1
	temp=0x00;
	if(LED_QX==1) temp=temp|0x03;
	else temp=temp&0xFC;
	TM1640_Write_Data(temp);
	// д��澯��־3	0xC2 
	// ��о״̬
	switch(LED_LX)
	{
		case 0:	// ��о��
						temp=0x00;
						break;
		case 1:	// ��ɫ����˸
						if(shift++>20)
						{
							temp=0x0E;
							if(shift>40) shift=0;
						}
						else temp=0x8E;
						break;
		case 2:	// ��ɫ��
						temp=0x8E;
						break;
		case 3: // ��о����1��
						temp=0x4E;
						break;
		case 4:	// ��о����2��
						temp=0x6E;
						break;
		case 5:	// ��о����3��
						temp=0x7E;
						break;
		default : break;
	}
	TM1640_Write_Data(temp);
	// д���¶�			0xc3-0xc4
	TM1640_Write_Data(Segcode[Display_Ramc[0]]);				// 3
	TM1640_Write_Data(Segcode[Display_Ramc[1]]);				// 4
	// д��ORP    	0xc5-0xc6-0xc7
	TM1640_Write_Data(Segcode[Display_Ramb[2]]);				// 5
	// ���¶��й�		
	temp=0x00;	
	if(Set_Temp_Num==0&&Work_Step==10) temp=0x80;				// 
	TM1640_Write_Data(Segcode[Display_Ramb[1]]|temp);		// 6  ��ɫ
	TM1640_Write_Data(Segcode[Display_Ramb[0]]|0x80);		// 7 
	// д��PH				0xc8-0xc9-0xc10	
	temp=0x00;	
	if(Set_Temp_Num==1||Set_Temp_Num==2) 								// 
	{
		if(Work_Step==10)	temp=0x80;	
	}
	TM1640_Write_Data(Segcode[Display_Rama[0]]|temp);		// 8	��ɫ
	TM1640_Write_Data(Segcode[Display_Rama[1]]);				// 9
	temp=0x00;
	if(Set_Temp_Num==3||Set_Temp_Num==2)
	{
		if(Work_Step==10)	temp=0x80;	
	}
	TM1640_Write_Data(Segcode[Display_Rama[2]]|temp);		// 10 ��ɫ
	// д��ָʾ�� 1 PHָʾ 0xC11	
	if(Set_Temp_Num==3)					//����ָʾ����
	{
		temp=0xC0;
	}
	else 
	{
		temp=0x00;
	}
	if(Set_PH_Num==0)						// 7
	{
		temp&=0xC0;
	}
	else if(Set_PH_Num==1)			// 8
	{
		temp&=0xC0;
		temp|=0x30;
	}
	else if(Set_PH_Num==2)			// 9
	{
		temp&=0xC0;
		temp|=0x0C;
	}
	else if(Set_PH_Num==3)			// 10
	{
		temp&=0xC0;
		temp|=0x03;
	}
	TM1640_Write_Data(temp);	
	// д��ָʾ�� 2 �¶�ָʾ 0xC12	
	temp=0xF0;							
	if(Set_Temp_Num==0)						// ����
	{
		temp&=0xF0;
	}
	else if(Set_Temp_Num==1)			// ����
	{
		temp&=0xF0;
		temp|=0x0C;
	}
	else if(Set_Temp_Num==2)			// ����
	{
		temp&=0xF0;
		temp|=0x03;
	}
	else if(Set_Temp_Num==3)			// ����
	{
		temp&=0xF0;
	}
	TM1640_Write_Data(temp);		
	// д��ָʾ�� 3 ��о�߿�ָʾ 0xC13	
	if(Work_Step==0) TM1640_Write_Data(0xFF);	
	else TM1640_Write_Data(0x1F);	
	// д�봥������ 1 0xC14	
	if(Work_Step==0) TM1640_Write_Data(0xFF);	
	else TM1640_Write_Data(0xFC);	
	// д�봥������ 2 0xC15	
	if(Work_Step==0) TM1640_Write_Data(0xFF);	
	else TM1640_Write_Data(0x07);	
	// ֹͣ
	TM1640_Refresh_Stop();						// ֹͣ��������
	// ��ʾ��������
	TM1640_Refresh_Start();						// ʹ���������� 
	if(Standby_Time>=STANDBY_NUM)			// ����ģʽ
	{
		LED_Brightness=2;								// �������
		Standby_Mode=1;									// �������ģʽ
	} // if(Standby_Time>=200)
	else
	{
		LED_Brightness=7;								// �������
		
	}	// if(Standby_Time>=200) else
	TM1640_Write_Cmd(0x88|LED_Brightness);					
	TM1640_Refresh_Stop();						// ֹͣ��������
}

