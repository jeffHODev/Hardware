#include "bsp_led.h"
#include "system.h"
#include "delay.h"

__IO uint8_t LED_QS=1;						// 原水箱缺水     0 1 2
__IO uint8_t LED_MS=1;						// 酸水箱水满			0 1 2
__IO uint8_t LED_TDS=1;						// 水质TDS指示灯  0 1 2
__IO uint8_t LED_LX=5;						// 滤芯已满指示		0 无滤芯(全闪烁) 1 滤芯到期(红灯闪烁0L)  2 滤芯寿命低(红灯亮30L)   3滤芯1格(大于50)  4滤芯2格(大于166)  5滤芯3格(大于335)
__IO uint8_t LED_GW=1;						// 原水水温高     0 1 2
__IO uint8_t LED_QX=0;						// 清洗模式
																
__IO uint8_t LED_Brightness=7;	  // 亮度调节
__IO uint8_t Display_Rama[3]={0x12,0x12,0x13};		// 显示缓冲区 PH   10pH
__IO uint8_t Display_Ramb[3]={0x12,0x12,0x12};		// 显示缓冲区 ORP  -385mv
//__IO uint8_t Display_Ramc[2]={0x12,0x12};				// 显示缓冲区 温度 88  ℃
__IO uint8_t Display_Ramc[2]={0x13,0x13};					// 2021 06 04 刘景涛改

// LedSegs	DB	03FH, 006H, 05BH, 04FH, 066H, 06DH, 07DH, 007H, 07FH, 06FH, 077H, 07CH, 039H, 05EH, 079H, 071H
// LED显示段码定义
const uint8_t Segcode[]=		// LED段码
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
  0x00,		// 无 0x13
	0x5C,		// o  0x14
	0x04,		// i  0x15
};

/********************************************************************************
 * 函数名：void Output_GPIO_Config(void)	 												
 * 描述  ： I/O配置	   														
 * 输入  ：无																	
 * 输出  ：无																	
 * 调用  ：内部调用																
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
 * 函数名：void TM1640_Refresh_Start(void)					// 开始刷新数据	
 * 描述  ：刷新DAC通道
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
static void TM1640_Refresh_Start(void)	// 开始刷新数据
{
	TM1640_SCLK(1);												// 拉高CLK引脚
	TM1640_DIN(1);												// 拉高DINA引脚
	delay_us(10);													// 短延时
	TM1640_DIN(0);												// 拉低DINA引脚
	delay_us(10);													// 短延时
	TM1640_SCLK(0);												// 拉低CLK引脚
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
	TM1640_SCLK(0);												// 拉低CLK引脚
	TM1640_DIN(0);												// 拉低DINA引脚
	delay_us(10);													// 短延时
	TM1640_SCLK(1);												// 拉高CLK引脚
	delay_us(10);													// 短延时
	TM1640_DIN(1);												// 拉高DINA引脚
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
		TM1640_SCLK(0);						// 拉低CLK引脚
		delay_us(10);							// 短延时	
		if(temp&0x01)							// 高电平
		{
			TM1640_DIN(1);					// 拉高DINA引脚
		}
		else											// 低电平
		{
			TM1640_DIN(0);					// 拉低DINA引脚	
		}
		delay_us(10);							// 短延时	
		TM1640_SCLK(1);						// 拉高CLK引脚
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
		TM1640_SCLK(0);					// 拉低CLK引脚
		// 写显示模块a
		if(tempa&0x01)					// 高电平
		{
			TM1640_DIN(1);				// 拉高DINA引脚	
		}
		else										// 低电平
		{
			TM1640_DIN(0);				// 拉低DINA引脚	
		}
		delay_us(10);						// 短延时
		TM1640_SCLK(1);					// 拉高CLK引脚
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
	uint8_t temp=0x00;	
	static uint8_t shift=0;
	
	if(Init_Falge==1) 													// 初始化标志(上电初始化标志)
	{
//		Display_Ramc[1]=Set_Temp_Table[Set_Temp_Num]/10;		// 显示温度
//		Display_Ramc[0]=Set_Temp_Table[Set_Temp_Num]%10;		// 显示温度
		Display_Ramc[1]=0x13;		// 2021 06 04 刘景涛改
		Display_Ramc[0]=0x13;		// 2021 06 04 刘景涛改
	}
	else
	{
//		Display_Ramc[1]=0x12;											//
//		Display_Ramc[0]=0x12;											//
		Display_Ramc[1]=0x13;		// 2021 06 04 刘景涛改
		Display_Ramc[0]=0x13;		// 2021 06 04 刘景涛改
	}
	
	// 判断显示TDS还是ORP
	if(Show_TDS_Value==1)												// 显示调试信息
	{
		Display_Ramb[0]=(Yswater_Tds%1000)/100;		// 显示TDS
		Display_Ramb[1]=(Yswater_Tds%100)/10;
		Display_Ramb[2]=Yswater_Tds%10;		
		if(Set_Temp_Num!=0&&Work_Step==10)				// 只有出热水时才会变化
		{
//			Display_Ramc[1]=Cswater_Temp/10;				// 出水温度
//			Display_Ramc[0]=Cswater_Temp%10;
			Display_Ramc[1]=0x13;		// 2021 06 04 刘景涛改
			Display_Ramc[0]=0x13;		// 2021 06 04 刘景涛改
		}
	}	
	// 设置数据命令
	TM1640_Refresh_Start();			// 使能数据输入 
	TM1640_Write_Cmd(0x40);			// 连续数据地址写入
	TM1640_Refresh_Stop();			// 停止数据输入
	// 设置地址命令
	TM1640_Refresh_Start();			// 使能数据输入 
	TM1640_Write_Cmd(0xC0);			// 开始显示地址(0xc0) 	
	// 写入告警标志1	0xC0
	temp=0x00;
	// 原水箱缺水
	if(LED_QS==1) temp=temp|0x03;
	else temp=temp&0xFC;
	// 原水水温高
	if(LED_GW==1) temp=temp|0x0C;
	else temp=temp&0xF3;	
	// 原水TDS异常
	if(LED_TDS==1) temp=temp|0x30;
	else temp=temp&0xCF;		
	// 酸水箱满了
	if(LED_MS==1) temp=temp|0xC0;
	else temp=temp&0x3F;		
	TM1640_Write_Data(temp);
	// 写入告警标志2	0xC1
	temp=0x00;
	if(LED_QX==1) temp=temp|0x03;
	else temp=temp&0xFC;
	TM1640_Write_Data(temp);
	// 写入告警标志3	0xC2 
	// 滤芯状态
	switch(LED_LX)
	{
		case 0:	// 滤芯无
						temp=0x00;
						break;
		case 1:	// 红色灯闪烁
						if(shift++>20)
						{
							temp=0x0E;
							if(shift>40) shift=0;
						}
						else temp=0x8E;
						break;
		case 2:	// 红色灯
						temp=0x8E;
						break;
		case 3: // 滤芯还有1格
						temp=0x4E;
						break;
		case 4:	// 滤芯还有2格
						temp=0x6E;
						break;
		case 5:	// 滤芯还有3格
						temp=0x7E;
						break;
		default : break;
	}
	TM1640_Write_Data(temp);
	// 写入温度			0xc3-0xc4
	TM1640_Write_Data(Segcode[Display_Ramc[0]]);				// 3
	TM1640_Write_Data(Segcode[Display_Ramc[1]]);				// 4
	// 写入ORP    	0xc5-0xc6-0xc7
	TM1640_Write_Data(Segcode[Display_Ramb[2]]);				// 5
	// 与温度有关		
	temp=0x00;	
	if(Set_Temp_Num==0&&Work_Step==10) temp=0x80;				// 
	TM1640_Write_Data(Segcode[Display_Ramb[1]]|temp);		// 6  蓝色
	TM1640_Write_Data(Segcode[Display_Ramb[0]]|0x80);		// 7 
	// 写入PH				0xc8-0xc9-0xc10	
	temp=0x00;	
	if(Set_Temp_Num==1||Set_Temp_Num==2) 								// 
	{
		if(Work_Step==10)	temp=0x80;	
	}
	TM1640_Write_Data(Segcode[Display_Rama[0]]|temp);		// 8	绿色
	TM1640_Write_Data(Segcode[Display_Rama[1]]);				// 9
	temp=0x00;
	if(Set_Temp_Num==3||Set_Temp_Num==2)
	{
		if(Work_Step==10)	temp=0x80;	
	}
	TM1640_Write_Data(Segcode[Display_Rama[2]]|temp);		// 10 红色
	// 写入指示灯 1 PH指示 0xC11	
	if(Set_Temp_Num==3)					//高温指示灯亮
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
	// 写入指示灯 2 温度指示 0xC12	
	temp=0xF0;							
	if(Set_Temp_Num==0)						// 常温
	{
		temp&=0xF0;
	}
	else if(Set_Temp_Num==1)			// 低温
	{
		temp&=0xF0;
		temp|=0x0C;
	}
	else if(Set_Temp_Num==2)			// 中温
	{
		temp&=0xF0;
		temp|=0x03;
	}
	else if(Set_Temp_Num==3)			// 高温
	{
		temp&=0xF0;
	}
	TM1640_Write_Data(temp);		
	// 写入指示灯 3 滤芯边框指示 0xC13	
	if(Work_Step==0) TM1640_Write_Data(0xFF);	
	else TM1640_Write_Data(0x1F);	
	// 写入触摸按键 1 0xC14	
	if(Work_Step==0) TM1640_Write_Data(0xFF);	
	else TM1640_Write_Data(0xFC);	
	// 写入触摸按键 2 0xC15	
	if(Work_Step==0) TM1640_Write_Data(0xFF);	
	else TM1640_Write_Data(0x07);	
	// 停止
	TM1640_Refresh_Stop();						// 停止数据输入
	// 显示亮度设置
	TM1640_Refresh_Start();						// 使能数据输入 
	if(Standby_Time>=STANDBY_NUM)			// 待机模式
	{
		LED_Brightness=2;								// 最大亮度
		Standby_Mode=1;									// 进入待机模式
	} // if(Standby_Time>=200)
	else
	{
		LED_Brightness=7;								// 最大亮度
		
	}	// if(Standby_Time>=200) else
	TM1640_Write_Cmd(0x88|LED_Brightness);					
	TM1640_Refresh_Stop();						// 停止数据输入
}

