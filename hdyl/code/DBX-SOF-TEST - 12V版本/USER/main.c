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
#include "usart1.h"
#include "usart3.h"
#include "driveboard.h"
#include "delay.h" 
#include <stdio.h>
#include <string.h>

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  : 无
 * 输出  ：无
 */
int main(void)
{  
	uint8_t num=0,jdvalue=0;
	uint8_t tempu=0,tempd=0,poweru=0,powerd=0;
	uint16_t delayms=0;
	uint8_t temp;
	static char str1[100];
	static char str2[50];
	char *str3="";
	char *str4="";
	
	/* 配置系统时钟为 72M */      
  SystemInit();
  Init_Board();

  while (1)
  {
		// 获取当前温度
		if(Msad_Flage==1)
		{
			Msad_Flage=0;
			Get_Now_Temp();				
			Get_Now_Value();	//UP_Temp=20;		
		}
		// 检测功率
		switch(Work_Mode)		// 根据工作模式
		{
			case 0:	// 床垫未链接时
							if((UP_Temp!=0)||(Down_Temp!=0))		// 床垫已经链接
							{	
								delay_us(10);
								if(delayms++>10000)
								{
									if(UP_Temp!=0&&Down_Temp!=0)		// 床垫已经链接进入检测状态
									{									
										strcpy (str1,"双人床垫");
										strcpy (str2,"正在预热");
										Usart3_Print("双人床垫");
										JDQ_YD1(1);	JDQ_YU2(1);		
										Work_Mode=1; 		
										Chuang_Mode=2;								// 双人
									}
									else if(Down_Temp!=0) 					// 
									{
										strcpy (str1,"单人床垫");
										strcpy (str2,"正在预热");										
										Usart3_Print("单人床垫");			
										JDQ_YD1(1);	JDQ_YU2(0);				// 下垫为单人
										Work_Mode=1;		
										Chuang_Mode=1;								// 单人
									}
									else 
									{
										JDQ_YD1(0);	JDQ_YU2(0);
										strcpy (str1,"床垫异常");
										strcpy (str2,"类型异常");							
										printf("床检测异常");
										Work_Mode=2;
										Chuang_Mode=0;								// 异常
									}															
								}
							}			
							else	{	delayms=0;	}
							break;
			case 1:	// 已经链接准备进入测量状态
							if(UP_Temp==0&&Down_Temp==0)			// 
							{
								Work_Mode=0;										// 床垫未链接
								strcpy (str1,"未链接床垫");
								strcpy (str2,"无");									
								tempu=0;tempd=0;poweru=0;powerd=0;
								jdvalue=0;
								JDQ_YD1(0);	JDQ_YU2(0);
							}		
							// 准备测量
							if(Second_Flage==1)
							{
								Second_Flage=0;
								jdvalue+=20;
								if(jdvalue>=100)
								{
									Work_Mode=2;		
									tempu=0;tempd=0;poweru=0;powerd=0;
									Usart3_Print("开始测试");
									strcpy (str2,"开始测试");		
									jdvalue=0;
								}							
							}
							break;
			case 2:	// 已经链接开始测量功率
							if(UP_Temp==0&&Down_Temp==0)			// 
							{
								Work_Mode=0;										// 床垫未链接
								strcpy (str1,"未链接床垫");
								strcpy (str2,"无");			
								tempu=0;tempd=0;poweru=0;powerd=0;
								jdvalue=0;
								JDQ_YD1(0);	JDQ_YU2(0);								
							}	
							// 开始测量
							if(Second_Flage==1)
							{
								Second_Flage=0;
								jdvalue+=25;
								if(jdvalue>=100)
								{
									num=0;
									Work_Mode=3;
									jdvalue=0;
								}							
							}
							// 检测结果
							// 上垫温度传感器检测
							if(Chuang_Mode==2)
							{
								if(Now_Temp>=UP_Temp)							
								{
									if(((Now_Temp-UP_Temp)>TEMP_DIFF))			// 只有双人床垫才检测	
									{
										tempu=1;num=0;
										Work_Mode=3;
									}
								}
								else
								{
									if(((UP_Temp-Now_Temp)>TEMP_DIFF))			// 只有双人床垫才检测		
									{
										tempu=1;num=0;
										Work_Mode=3;
									}							
								}	
								// 上下垫间温度测试
								if(Down_Temp>=UP_Temp)							
								{
									if(((Down_Temp-UP_Temp)>TEMP_DIFF))			// 只有双人床垫才检测	
									{
										tempd=1;tempu=1;num=0;
										Work_Mode=3;
									}
								}
								else
								{
									if(((UP_Temp-Down_Temp)>TEMP_DIFF))			// 只有双人床垫才检测		
									{
										tempd=1;tempu=1;num=0;
										Work_Mode=3;
									}							
								}	
								// 上垫功率检测
								if(UP_Power>=SRSTAND_POWER)							// 功率上限
								{
									if(((UP_Power-SRSTAND_POWER)>SRPOWER_DIFFU))					// 只有双人床垫才检测			
									{								
										poweru=1;num=0;
										Work_Mode=3;
									}
								}
								else																	// 功率下限
								{
									if(((SRSTAND_POWER-UP_Power)>SRPOWER_DIFFD))					// 只有双人床垫才检测			
									{
										poweru=1;num=0;
										Work_Mode=3;
									}							
								}
								// 下垫温度传感器检测
								if(Now_Temp>=Down_Temp)							
								{
									if((Now_Temp-Down_Temp)>TEMP_DIFF)		
									{
										tempd=1;num=0;
										Work_Mode=3;
									}
								}
								else
								{
									if((Down_Temp-Now_Temp)>TEMP_DIFF)		
									{
										tempd=1;num=0;
										Work_Mode=3;
									}							
								}									
								// 下垫功率检测
								if(Down_Power>=SRSTAND_POWER)					// 功率上限							
								{
									if((Down_Power-SRSTAND_POWER)>SRPOWER_DIFFU)
									{
										powerd=1;num=0;
										Work_Mode=3;
									}
								}
								else																// 功率下限
								{
									if((SRSTAND_POWER-Down_Power)>SRPOWER_DIFFD)
									{
										powerd=1;num=0;
										Work_Mode=3;
									}							
								}									
							}
							else if(Chuang_Mode==1)
							{
								// 下垫温度传感器检测
								if(Now_Temp>=Down_Temp)							
								{
									if((Now_Temp-Down_Temp)>TEMP_DIFF)		
									{
										tempd=1;num=0;
										Work_Mode=3;
									}
								}
								else
								{
									if((Down_Temp-Now_Temp)>TEMP_DIFF)		
									{
										tempd=1;num=0;
										Work_Mode=3;
									}							
								}									
								// 下垫功率检测
								if(Down_Power>=DRSTAND_POWER)					// 功率上限							
								{
									if((Down_Power-DRSTAND_POWER)>DRPOWER_DIFFU)
									{
										powerd=1;num=0;
										Work_Mode=3;
									}
								}
								else																// 功率下限
								{
									if((DRSTAND_POWER-Down_Power)>DRPOWER_DIFFD)
									{
										powerd=1;num=0;
										Work_Mode=3;
									}							
								}															
							}
							else if(Chuang_Mode==0)
							{
								Work_Mode=4;
								JDQ_YD1(0);	JDQ_YU2(0);									
								Usart3_Print("检测异常");					
							}
							break;
			case 3:	// 测量结束显示测试结果
							if(UP_Temp==0&&Down_Temp==0)			// 
							{
								Work_Mode=0;										// 床垫未链接
								strcpy (str1,"未链接床垫");
								strcpy (str2,"无");		
								tempu=0;tempd=0;poweru=0;powerd=0;
								jdvalue=0;
								JDQ_YD1(0);	JDQ_YU2(0);								
							}	
							// 显示测量结果温度传感器
							if(tempu==1&&tempd==1){str3="上下垫传感器异常";}
							else if(tempu==1){str3="上垫传感器异常";}
								else if(tempd==1){str3="下垫传感器异常";}
									else {str3="传感器正常";}
							// 显示测量结果功率						
							if(poweru==1&&powerd==1) {str4="上下垫功率异常";}
							else if(poweru==1)	{str4="上垫功率异常";}
								else if(powerd==1)	{str4="下垫功率异常";}
									else {str4="功率正常";}
							// 字符拼接
							strcpy(str1,str3);		
							strcat(str1,str4);			
							// 报告测试结果
							if(tempu==0&&tempd==0&&poweru==0&&powerd==0)
							{
								strcpy (str2,"合格");	
							}
							else
							{
								strcpy (str2,"不合格");	
							}
							// 退出条件 语音播放测试结果
							if(num>=8)
							{
								Work_Mode=4;
								JDQ_YD1(0);	JDQ_YU2(0);									
								if(tempu==0&&tempd==0&&poweru==0&&powerd==0)
								{
									Usart3_Print("通过检测");
								}
								else
								{
									Usart3_Print("检测异常");
								}								
							}						
							break;
			case 4: // 测量结束显示测量值
							if(UP_Temp==0&&Down_Temp==0)			// 
							{
								Work_Mode=0;										// 床垫未链接
								strcpy (str1,"未链接床垫");
								strcpy (str2,"无");		
								tempu=0;tempd=0;poweru=0;powerd=0;
								jdvalue=0;
								JDQ_YD1(0);	JDQ_YU2(0);										
							}								
							break;
			default : break;
		}
			
		// 刷新显示
		if(Msecond_Flage==1&&Work_Mode!=4)
		{
			Msecond_Flage=0;
			
			if(num==0)												// 上垫功率
			{
				temp=(uint8_t)UP_Power;
				if(temp<5) temp=0;
				Usart1_Print("%s=%d","nsg.val",temp);						
			}
			if(num==1) 												// 下垫功率
			{
				temp=(uint8_t)Down_Power;
				Usart1_Print("%s=%d","nxg.val",temp);	
			}
			if(num==2) 												// 上垫温度
			{
				temp=(uint8_t)UP_Temp;
				Usart1_Print("%s=%d","nsw.val",temp);
			}
			if(num==3) 												// 下垫温度
			{
				temp=(uint8_t)Down_Temp;
				Usart1_Print("%s=%d","nxw.val",temp);
			}
			if(num==4) 												// 当前温度
			{
				temp=(uint8_t)Now_Temp;
				Usart1_Print("%s=%d","nwd.val",temp);		
			}
			// 字符串
			if(num==5) Usart1_Print("%s=\"%s\"","tzt.txt",str1);	// 显示床垫状态
			if(num==6) Usart1_Print("%s=\"%s\"","tjg.txt",str2);	// 显示床垫状态
			if(num==7) Usart1_Print("%s=%d","jd.val",jdvalue); 		// 进度条

			if(num++>=10) num=0;
		}
  }
}


/******************* (C) COPYRIGHT 2011 野火嵌入式开发工作室 *****END OF FILE****/
