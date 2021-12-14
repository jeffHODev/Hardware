/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名  ：main.c
 * 描述    ：将c库中的printf()函数实现到串口1(USART1)。这样我们就可以用printf()将
 *           调试信息通过串口打印到电脑上。         
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.0.0
 *
 * 作者    ：fire  QQ: 313303034 
 * 博客    ：firestm32.blog.chinaunix.net
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

static uint8_t RFID_Setp=0;						 	// 读取ID步骤
static uint8_t Read_Num=0; 							// 读取ID次数
static uint8_t Last_Setp=0;							// 上次读取ID步骤

	/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  : 无
 * 输出  ：无
 */
int main(void)													// 主程序
{  
	/* 配置系统时钟为 72M */      
  SystemInit();													// 初始时钟
  Init_Board();													// 初始化硬件
	Refresh_LED_Display();								// 刷新显示		
	
  while (1)
  {
		if(Hmwater_Temp>=105)								// 高温保护 最高温度不超过105℃
		{
			JDQ_Heat(0);											// 断开发热体
		}			
		// 读取按键
		if(Msecond_Flage==1)								// 10毫秒延时								
		{
			Msecond_Flage=0;									// 复位标志			
			Get_Now_Value();									// 获取温度/TDS/等AD值
			Senser_Value_Read();							// 读取各个传感器状态					
			Button_Read();										// 获取按键状态		
			Work_In_Set();										// 用户设置			
			if(Init_Falge==1) 								// 初始化标志(上电初始化标志)
			{
				Refresh_LED_Display();					// 刷新显示		
			}
			else
			{
				Tds_Test_Flage=1;								// 上电后测试TDS
			}
			IWDG_ReloadCounter();							// 喂狗
		}
		// 每秒钟读取一次RFID卡
		if(Second_Flage==1)									// 秒钟标志
		{
			Second_Flage=0;										// 清除标志位								
			RF_Read_Info(&RFID_Setp);					// 读取数据帧
			if(Last_Setp==RFID_Setp)					// 多次读卡异常计数
			{
				if(Read_Num++>10)								// 累计10次读异常
				{
					Read_Num=0;										// 复位ID步骤
					RFID_Setp=1;									// 复位ID步骤
					Last_Setp=1;									// 复位ID步骤
				}
			}
			else
			{
				Last_Setp=RFID_Setp;						// 保存上次记录
				Read_Num=0;											// 复位ID步骤
			}
		}
		// 检测串口数据帧
		if(Usart3_Receive_Flage==1)					// 检测数据帧
		{
			Usart3_Receive_Flage=0;						// 清除标志位
			EM4305_Get_NewFAX(&RFID_Setp);		// 获取数据
		}
  }
}


/******************* (C) COPYRIGHT 2011 野火嵌入式开发工作室 *****END OF FILE****/
