#include "bsp_led.h"
#include "main.h"
#include "system.h"
#include "filter.h"

uint8_t LED_ZS=0;												// 制水
uint8_t LED_DJ=0;												// 电解
uint8_t LED_CX=0;												// 冲洗
uint8_t LED_QS=0;												// 缺水
uint8_t LED_Brightness=7;	  						// 亮度调节
uint8_t Refresh_LED=1;									// 强制刷新LCD
__IO uint8_t Standby_Time_Delay=0;			// 待机延时时间
/***********************************************************************************
 * 函数名：void TM1640_Refresh_Start(void)	// 开始刷新数据	
 * 描述  ：刷新DAC通道
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
static void TM1640_Refresh_Start(void)	// 开始刷新数据
{
	TM_SCK_SET(1);												// 拉高CLK引脚
	TM_DIN_SET(1);												// 拉高DINA引脚
	delay_us(10);													// 短延时
	TM_DIN_SET(0);												// 拉低DINA引脚
	delay_us(10);													// 短延时
	TM_SCK_SET(0);												// 拉低CLK引脚
	delay_us(10);													// 短延时
}

/***********************************************************************************
 * 函数名：TM1640_Refresh_Stop(void)		
 * 描述  ：停止数据数据
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
static void TM1640_Refresh_Stop(void)		// 停止刷新数据
{
	TM_SCK_SET(0);												// 拉低CLK引脚
	TM_DIN_SET(0);												// 拉低DINA引脚
	delay_us(10);													// 短延时
	TM_SCK_SET(1);												// 拉高CLK引脚
	delay_us(10);													// 短延时
	TM_DIN_SET(1);												// 拉高DINA引脚
	delay_us(10);													// 短延时
}

/***********************************************************************************
 * 函数名：void TM1640_Write_Cmd(uint8_t data)
 * 描述  ：向TM1640写入数据
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
static void TM1640_Write_Cmd(uint8_t temp)
{
	uint8_t i;	

	for(i=0;i<8;i++)						// 写设置数据  地址自动加1	 0100 0000  0x40
	{
		TM_SCK_SET(0);						// 拉低CLK引脚
		delay_us(10);							// 短延时	
		if(temp&0x01)							// 高电平
		{
			TM_DIN_SET(1);					// 拉高DINA引脚
		}
		else											// 低电平
		{
			TM_DIN_SET(0);					// 拉低DINA引脚	
		}
		delay_us(10);							// 短延时	
		TM_SCK_SET(1);						// 拉高CLK引脚
		delay_us(10);							// 短延时
		temp=temp>>1;							// 向右移动1位
	}
}

/***********************************************************************************
 * 函数名：TM1640_Write_Data(uint8_t data1,uint8_t data2,uint8_t data3,uint8_t data4)
 * 描述  ：向TM1640写入数据
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
static void TM1640_Write_Data(uint8_t tempa)
{
	uint8_t i;

	for(i=0;i<8;i++)					// 写设置数据  地址自动加1	 1100 0000  0xC0	 起始位置为0
	{
		TM_SCK_SET(0);					// 拉低CLK引脚
		// 写显示模块a
		if(tempa&0x01)					// 高电平
		{
			TM_DIN_SET(1);				// 拉高DINA引脚	
		}
		else										// 低电平
		{
			TM_DIN_SET(0);				// 拉低DINA引脚	
		}
		delay_us(10);						// 短延时
		TM_SCK_SET(1);					// 拉高CLK引脚
		delay_us(10);						// 短延时
		tempa=tempa>>1;					// 向右移动1位
	}
}

/***********************************************************************************
 * 函数名：Refresh_DAC_OUT(uint8_t channel)			
 * 描述  ：刷新DAC通道
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
void Refresh_LED_Display(void)	// 刷新显示
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
	
	// 设置数据命令
	TM1640_Refresh_Start();			// 使能数据输入 
	TM1640_Write_Cmd(0x40);			// 连续数据地址写入
	TM1640_Refresh_Stop();			// 停止数据输入
	// 设置地址命令
	TM1640_Refresh_Start();			// 使能数据输入 
	TM1640_Write_Cmd(0xC0);			// 开始显示地址(0xc0) 
	/*****写入显示数据*****/
	TM1640_Write_Data(0xFF);		// SM1 0xc0 
	TM1640_Write_Data(0xFF);		// SM2 0xc1
	TM1640_Write_Data(0xFF);		// SM3 0xc2
	TM1640_Write_Data(0xFF);		// SM4 0xc3
	// PP滤芯
	temp=0;
	for(i=0;i<LED_PP;i++)				// 滤芯寿命
	{
		temp|=0x01<<i;						// 
	}
	if(Probe_PP==1) temp|=0xc0;	// TI
	else temp&=0x3F;						//
	TM1640_Write_Data(temp);		// SM5 0xc4
	// C1滤芯
	temp=0;
	for(i=0;i<LED_C1;i++)				// 滤芯寿命
	{
		temp|=0x01<<i;						// 
	}
	if(Probe_C1==1) temp|=0xc0;	// C1
	else temp&=0x3F;						//
	TM1640_Write_Data(temp);		// SM6 0xc5
	// UDF  
	temp=0;
	for(i=0;i<LED_UDF;i++)				// 滤芯寿命
	{
		temp|=0x01<<i;						// 
	}
	if(Probe_UDF==1) temp|=0xc0;	// UDF
	else temp&=0x3F;						//	
	TM1640_Write_Data(temp);		// SM7 0xc6	
	// C2滤芯	
	temp=0;
	for(i=0;i<LED_C2;i++)				// 滤芯寿命
	{
		temp|=0x01<<i;						// 
	}
	if(Probe_C2==1) temp|=0xc0;	// C2
	else temp&=0x3F;						//	
	TM1640_Write_Data(temp);		// SM8 0xc7
	// 电解模式
	if(ELE_Strength==0)					// 净水
	{
		temp=0x03;
	}
	if(ELE_Strength==1)					// 弱
	{
		temp=0x0C;
	}	
	if(ELE_Strength==2)					// 中
	{
		temp=0x30;
	}
	if(ELE_Strength==3)					// 强
	{
		temp=0xC0;
	}
	TM1640_Write_Data(temp);		// SM9  0xc8
	// 工作模式
	temp=0;										
	if(LED_ZS==1) temp|=0x03;		// 制水
	else temp&=0xFC;	
	if(LED_DJ==1) temp|=0x0C;		// 电解
	else temp&=0xF3;	
	if(LED_CX==1) temp|=0x30;		// 冲洗
	else temp&=0xCF;	
	if(LED_QS==1) temp|=0xC0;		// 缺水
	else temp&=0x3F;	
	TM1640_Write_Data(temp);		// SM10 0xc9
	
	TM1640_Write_Data(0xFF);		// SM11 0xca 
	TM1640_Write_Data(0xFF);		// SM12 0xcb
	TM1640_Write_Data(0xFF);		// SM13 0xcc
	TM1640_Write_Data(0xFF);		// SM14 0xcd
	TM1640_Write_Data(0xFF);		// SM15 0xce
	/*****写入显示数据*****/
	TM1640_Refresh_Stop();			// 停止数据输入
	// 显示亮度设置
	TM1640_Refresh_Start();			// 使能数据输入 
	if(LED_Brightness>7)  LED_Brightness=7;
	TM1640_Write_Cmd(0x88|LED_Brightness);					
	TM1640_Refresh_Stop();			// 停止数据输入
}

