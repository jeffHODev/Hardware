#include "bsp_led.h"
#include "main.h"
#include "system.h"
#include "filter.h"

uint8_t LED_ZS=0;												// ��ˮ
uint8_t LED_DJ=0;												// ���
uint8_t LED_CX=0;												// ��ϴ
uint8_t LED_QS=0;												// ȱˮ
uint8_t LED_Brightness=7;	  						// ���ȵ���
uint8_t Refresh_LED=1;									// ǿ��ˢ��LCD
__IO uint8_t Standby_Time_Delay=0;			// ������ʱʱ��
/***********************************************************************************
 * ��������void TM1640_Refresh_Start(void)	// ��ʼˢ������	
 * ����  ��ˢ��DACͨ��
 * ����  ����
 * ���  ����
************************************************************************************/
static void TM1640_Refresh_Start(void)	// ��ʼˢ������
{
	TM_SCK_SET(1);												// ����CLK����
	TM_DIN_SET(1);												// ����DINA����
	delay_us(10);													// ����ʱ
	TM_DIN_SET(0);												// ����DINA����
	delay_us(10);													// ����ʱ
	TM_SCK_SET(0);												// ����CLK����
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
	TM_SCK_SET(0);												// ����CLK����
	TM_DIN_SET(0);												// ����DINA����
	delay_us(10);													// ����ʱ
	TM_SCK_SET(1);												// ����CLK����
	delay_us(10);													// ����ʱ
	TM_DIN_SET(1);												// ����DINA����
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
		TM_SCK_SET(0);						// ����CLK����
		delay_us(10);							// ����ʱ	
		if(temp&0x01)							// �ߵ�ƽ
		{
			TM_DIN_SET(1);					// ����DINA����
		}
		else											// �͵�ƽ
		{
			TM_DIN_SET(0);					// ����DINA����	
		}
		delay_us(10);							// ����ʱ	
		TM_SCK_SET(1);						// ����CLK����
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
		TM_SCK_SET(0);					// ����CLK����
		// д��ʾģ��a
		if(tempa&0x01)					// �ߵ�ƽ
		{
			TM_DIN_SET(1);				// ����DINA����	
		}
		else										// �͵�ƽ
		{
			TM_DIN_SET(0);				// ����DINA����	
		}
		delay_us(10);						// ����ʱ
		TM_SCK_SET(1);					// ����CLK����
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
	uint8_t i,temp=0,refresh=0;
	static uint8_t elestr=0;
	static uint8_t probet1,probet2,probet3,probet4;
	static uint8_t ledt1,ledt2,ledt3,ledt4;
	static uint8_t ledzs,leddj,ledcx,ledqs;
	static uint8_t ledbright;
	
	if(Standby_Time_Delay==0) LED_Brightness=2;
	else LED_Brightness=7;
	if(ledbright!=LED_Brightness) {ledbright=LED_Brightness;}
	if(elestr!=ELE_Strength)	{	elestr=ELE_Strength; refresh=1;}
	if(probet1!=Probe_PP)			{	probet1=Probe_PP; refresh=1;}
	if(probet2!=Probe_C1)			{	probet2=Probe_C1; refresh=1;}
	if(probet3!=Probe_UDF)		{	probet3=Probe_UDF; refresh=1;}
	if(probet4!=Probe_C2)			{	probet4=Probe_C2; refresh=1;}
	if(ledt1!=LED_PP)					{	ledt1=LED_PP; refresh=1;}
	if(ledt2!=LED_C1)					{	ledt2=LED_C1; refresh=1;}
	if(ledt3!=LED_UDF)				{	ledt3=LED_UDF; refresh=1;}
	if(ledt4!=LED_C2)					{	ledt4=LED_C2; refresh=1;}
	if(ledzs!=LED_ZS)					{	ledzs=LED_ZS; refresh=1;}
	if(leddj!=LED_DJ)					{	leddj=LED_DJ; refresh=1;}
	if(ledcx!=LED_CX)					{	ledcx=LED_CX; refresh=1;}
	if(ledqs!=LED_QS)					{	ledqs=LED_QS; refresh=1;}
	if(Refresh_LED==1)        {	Refresh_LED=0; refresh=1;	}
	if(refresh==0) return;
	
	// ������������
	TM1640_Refresh_Start();			// ʹ���������� 
	TM1640_Write_Cmd(0x40);			// �������ݵ�ַд��
	TM1640_Refresh_Stop();			// ֹͣ��������
	// ���õ�ַ����
	TM1640_Refresh_Start();			// ʹ���������� 
	TM1640_Write_Cmd(0xC0);			// ��ʼ��ʾ��ַ(0xc0) 
	/*****д����ʾ����*****/
	TM1640_Write_Data(0xFF);		// SM1 0xc0 
	TM1640_Write_Data(0xFF);		// SM2 0xc1
	TM1640_Write_Data(0xFF);		// SM3 0xc2
	TM1640_Write_Data(0xFF);		// SM4 0xc3
	// PP��о
	temp=0;
	for(i=0;i<LED_PP;i++)				// ��о����
	{
		temp|=0x01<<i;						// 
	}
	if(Probe_PP==1) temp|=0xc0;	// TI
	else temp&=0x3F;						//
	TM1640_Write_Data(temp);		// SM5 0xc4
	// C1��о
	temp=0;
	for(i=0;i<LED_C1;i++)				// ��о����
	{
		temp|=0x01<<i;						// 
	}
	if(Probe_C1==1) temp|=0xc0;	// C1
	else temp&=0x3F;						//
	TM1640_Write_Data(temp);		// SM6 0xc5
	// UDF  
	temp=0;
	for(i=0;i<LED_UDF;i++)				// ��о����
	{
		temp|=0x01<<i;						// 
	}
	if(Probe_UDF==1) temp|=0xc0;	// UDF
	else temp&=0x3F;						//	
	TM1640_Write_Data(temp);		// SM7 0xc6	
	// C2��о	
	temp=0;
	for(i=0;i<LED_C2;i++)				// ��о����
	{
		temp|=0x01<<i;						// 
	}
	if(Probe_C2==1) temp|=0xc0;	// C2
	else temp&=0x3F;						//	
	TM1640_Write_Data(temp);		// SM8 0xc7
	// ���ģʽ
	if(ELE_Strength==0)					// ��ˮ
	{
		temp=0x03;
	}
	if(ELE_Strength==1)					// ��
	{
		temp=0x0C;
	}	
	if(ELE_Strength==2)					// ��
	{
		temp=0x30;
	}
	if(ELE_Strength==3)					// ǿ
	{
		temp=0xC0;
	}
	TM1640_Write_Data(temp);		// SM9  0xc8
	// ����ģʽ
	temp=0;										
	if(LED_ZS==1) temp|=0x03;		// ��ˮ
	else temp&=0xFC;	
	if(LED_DJ==1) temp|=0x0C;		// ���
	else temp&=0xF3;	
	if(LED_CX==1) temp|=0x30;		// ��ϴ
	else temp&=0xCF;	
	if(LED_QS==1) temp|=0xC0;		// ȱˮ
	else temp&=0x3F;	
	TM1640_Write_Data(temp);		// SM10 0xc9
	
	TM1640_Write_Data(0xFF);		// SM11 0xca 
	TM1640_Write_Data(0xFF);		// SM12 0xcb
	TM1640_Write_Data(0xFF);		// SM13 0xcc
	TM1640_Write_Data(0xFF);		// SM14 0xcd
	TM1640_Write_Data(0xFF);		// SM15 0xce
	/*****д����ʾ����*****/
	TM1640_Refresh_Stop();			// ֹͣ��������
	// ��ʾ��������
	TM1640_Refresh_Start();			// ʹ���������� 
	if(LED_Brightness>7)  LED_Brightness=7;
	TM1640_Write_Cmd(0x88|LED_Brightness);					
	TM1640_Refresh_Stop();			// ֹͣ��������
}

