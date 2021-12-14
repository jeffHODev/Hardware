/******************** (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� ********************
 * �ļ���  ��main.c
 * ����    ����c���е�printf()����ʵ�ֵ�����1(USART1)���������ǾͿ�����printf()��
 *           ������Ϣͨ�����ڴ�ӡ�������ϡ�         
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.0.0
 *
 * ����    ��fire  QQ: 313303034 
 * ����    ��firestm32.blog.chinaunix.net
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
 * ��������main
 * ����  ��������
 * ����  : ��
 * ���  ����
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
	
	/* ����ϵͳʱ��Ϊ 72M */      
  SystemInit();
  Init_Board();

  while (1)
  {
		// ��ȡ��ǰ�¶�
		if(Msad_Flage==1)
		{
			Msad_Flage=0;
			Get_Now_Temp();				
			Get_Now_Value();	//UP_Temp=20;		
		}
		// ��⹦��
		switch(Work_Mode)		// ���ݹ���ģʽ
		{
			case 0:	// ����δ����ʱ
							if((UP_Temp!=0)||(Down_Temp!=0))		// �����Ѿ�����
							{	
								delay_us(10);
								if(delayms++>10000)
								{
									if(UP_Temp!=0&&Down_Temp!=0)		// �����Ѿ����ӽ�����״̬
									{									
										strcpy (str1,"˫�˴���");
										strcpy (str2,"����Ԥ��");
										Usart3_Print("˫�˴���");
										JDQ_YD1(1);	JDQ_YU2(1);		
										Work_Mode=1; 		
										Chuang_Mode=2;								// ˫��
									}
									else if(Down_Temp!=0) 					// 
									{
										strcpy (str1,"���˴���");
										strcpy (str2,"����Ԥ��");										
										Usart3_Print("���˴���");			
										JDQ_YD1(1);	JDQ_YU2(0);				// �µ�Ϊ����
										Work_Mode=1;		
										Chuang_Mode=1;								// ����
									}
									else 
									{
										JDQ_YD1(0);	JDQ_YU2(0);
										strcpy (str1,"�����쳣");
										strcpy (str2,"�����쳣");							
										printf("������쳣");
										Work_Mode=2;
										Chuang_Mode=0;								// �쳣
									}															
								}
							}			
							else	{	delayms=0;	}
							break;
			case 1:	// �Ѿ�����׼���������״̬
							if(UP_Temp==0&&Down_Temp==0)			// 
							{
								Work_Mode=0;										// ����δ����
								strcpy (str1,"δ���Ӵ���");
								strcpy (str2,"��");									
								tempu=0;tempd=0;poweru=0;powerd=0;
								jdvalue=0;
								JDQ_YD1(0);	JDQ_YU2(0);
							}		
							// ׼������
							if(Second_Flage==1)
							{
								Second_Flage=0;
								jdvalue+=20;
								if(jdvalue>=100)
								{
									Work_Mode=2;		
									tempu=0;tempd=0;poweru=0;powerd=0;
									Usart3_Print("��ʼ����");
									strcpy (str2,"��ʼ����");		
									jdvalue=0;
								}							
							}
							break;
			case 2:	// �Ѿ����ӿ�ʼ��������
							if(UP_Temp==0&&Down_Temp==0)			// 
							{
								Work_Mode=0;										// ����δ����
								strcpy (str1,"δ���Ӵ���");
								strcpy (str2,"��");			
								tempu=0;tempd=0;poweru=0;powerd=0;
								jdvalue=0;
								JDQ_YD1(0);	JDQ_YU2(0);								
							}	
							// ��ʼ����
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
							// �����
							// �ϵ��¶ȴ��������
							if(Chuang_Mode==2)
							{
								if(Now_Temp>=UP_Temp)							
								{
									if(((Now_Temp-UP_Temp)>TEMP_DIFF))			// ֻ��˫�˴���ż��	
									{
										tempu=1;num=0;
										Work_Mode=3;
									}
								}
								else
								{
									if(((UP_Temp-Now_Temp)>TEMP_DIFF))			// ֻ��˫�˴���ż��		
									{
										tempu=1;num=0;
										Work_Mode=3;
									}							
								}	
								// ���µ���¶Ȳ���
								if(Down_Temp>=UP_Temp)							
								{
									if(((Down_Temp-UP_Temp)>TEMP_DIFF))			// ֻ��˫�˴���ż��	
									{
										tempd=1;tempu=1;num=0;
										Work_Mode=3;
									}
								}
								else
								{
									if(((UP_Temp-Down_Temp)>TEMP_DIFF))			// ֻ��˫�˴���ż��		
									{
										tempd=1;tempu=1;num=0;
										Work_Mode=3;
									}							
								}	
								// �ϵ湦�ʼ��
								if(UP_Power>=SRSTAND_POWER)							// ��������
								{
									if(((UP_Power-SRSTAND_POWER)>SRPOWER_DIFFU))					// ֻ��˫�˴���ż��			
									{								
										poweru=1;num=0;
										Work_Mode=3;
									}
								}
								else																	// ��������
								{
									if(((SRSTAND_POWER-UP_Power)>SRPOWER_DIFFD))					// ֻ��˫�˴���ż��			
									{
										poweru=1;num=0;
										Work_Mode=3;
									}							
								}
								// �µ��¶ȴ��������
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
								// �µ湦�ʼ��
								if(Down_Power>=SRSTAND_POWER)					// ��������							
								{
									if((Down_Power-SRSTAND_POWER)>SRPOWER_DIFFU)
									{
										powerd=1;num=0;
										Work_Mode=3;
									}
								}
								else																// ��������
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
								// �µ��¶ȴ��������
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
								// �µ湦�ʼ��
								if(Down_Power>=DRSTAND_POWER)					// ��������							
								{
									if((Down_Power-DRSTAND_POWER)>DRPOWER_DIFFU)
									{
										powerd=1;num=0;
										Work_Mode=3;
									}
								}
								else																// ��������
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
								Usart3_Print("����쳣");					
							}
							break;
			case 3:	// ����������ʾ���Խ��
							if(UP_Temp==0&&Down_Temp==0)			// 
							{
								Work_Mode=0;										// ����δ����
								strcpy (str1,"δ���Ӵ���");
								strcpy (str2,"��");		
								tempu=0;tempd=0;poweru=0;powerd=0;
								jdvalue=0;
								JDQ_YD1(0);	JDQ_YU2(0);								
							}	
							// ��ʾ��������¶ȴ�����
							if(tempu==1&&tempd==1){str3="���µ洫�����쳣";}
							else if(tempu==1){str3="�ϵ洫�����쳣";}
								else if(tempd==1){str3="�µ洫�����쳣";}
									else {str3="����������";}
							// ��ʾ�����������						
							if(poweru==1&&powerd==1) {str4="���µ湦���쳣";}
							else if(poweru==1)	{str4="�ϵ湦���쳣";}
								else if(powerd==1)	{str4="�µ湦���쳣";}
									else {str4="��������";}
							// �ַ�ƴ��
							strcpy(str1,str3);		
							strcat(str1,str4);			
							// ������Խ��
							if(tempu==0&&tempd==0&&poweru==0&&powerd==0)
							{
								strcpy (str2,"�ϸ�");	
							}
							else
							{
								strcpy (str2,"���ϸ�");	
							}
							// �˳����� �������Ų��Խ��
							if(num>=8)
							{
								Work_Mode=4;
								JDQ_YD1(0);	JDQ_YU2(0);									
								if(tempu==0&&tempd==0&&poweru==0&&powerd==0)
								{
									Usart3_Print("ͨ�����");
								}
								else
								{
									Usart3_Print("����쳣");
								}								
							}						
							break;
			case 4: // ����������ʾ����ֵ
							if(UP_Temp==0&&Down_Temp==0)			// 
							{
								Work_Mode=0;										// ����δ����
								strcpy (str1,"δ���Ӵ���");
								strcpy (str2,"��");		
								tempu=0;tempd=0;poweru=0;powerd=0;
								jdvalue=0;
								JDQ_YD1(0);	JDQ_YU2(0);										
							}								
							break;
			default : break;
		}
			
		// ˢ����ʾ
		if(Msecond_Flage==1&&Work_Mode!=4)
		{
			Msecond_Flage=0;
			
			if(num==0)												// �ϵ湦��
			{
				temp=(uint8_t)UP_Power;
				if(temp<5) temp=0;
				Usart1_Print("%s=%d","nsg.val",temp);						
			}
			if(num==1) 												// �µ湦��
			{
				temp=(uint8_t)Down_Power;
				Usart1_Print("%s=%d","nxg.val",temp);	
			}
			if(num==2) 												// �ϵ��¶�
			{
				temp=(uint8_t)UP_Temp;
				Usart1_Print("%s=%d","nsw.val",temp);
			}
			if(num==3) 												// �µ��¶�
			{
				temp=(uint8_t)Down_Temp;
				Usart1_Print("%s=%d","nxw.val",temp);
			}
			if(num==4) 												// ��ǰ�¶�
			{
				temp=(uint8_t)Now_Temp;
				Usart1_Print("%s=%d","nwd.val",temp);		
			}
			// �ַ���
			if(num==5) Usart1_Print("%s=\"%s\"","tzt.txt",str1);	// ��ʾ����״̬
			if(num==6) Usart1_Print("%s=\"%s\"","tjg.txt",str2);	// ��ʾ����״̬
			if(num==7) Usart1_Print("%s=%d","jd.val",jdvalue); 		// ������

			if(num++>=10) num=0;
		}
  }
}


/******************* (C) COPYRIGHT 2011 Ұ��Ƕ��ʽ���������� *****END OF FILE****/
