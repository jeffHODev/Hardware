/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************
 * �ļ���  ��system.c
 * ��    ����MODBUS����
 * ʵ��ƽ̨��STM32ͨѶת��
 * Ӳ�����ӣ���
 * ��汾  ��ST3.0.0
 * ����    ��С��
 * ��Ŀ˵����MODBUS
**********************************************************************************/
#include "stm32f10x.h"
#include "driveboard.h"
#include "usart3.h"
#include "system.h"
#include "bsp_led.h"
#include "delay.h"
#include "iic.h"
#include "adc.h"
#include "pid.h"

/*****************************��������*******************************************/
__IO uint8_t Cswater_Temp=0;					// ��ˮ�¶�
__IO float   Hmwater_Temp=0;					// �������¶�
__IO uint8_t Yswater_Temp=0;					// ԭˮ�¶�
__IO uint16_t Now_Elecr_Current=0;		// ��ǰ���� ma 0-4000ma
__IO uint16_t Yswater_Tds=0;					// ԭˮTDS
__IO uint16_t Filter_Life=0;					// ��о���� 0L
__IO uint16_t Filter_Count=0;					// ��о��ˮ�� 1000ml			
__IO uint8_t  Standby_Mode=0;					// 0 ����ģʽ  1����ģʽ
__IO uint8_t 	Work_Step=0;						// �������輰ģʽ
__IO uint8_t  High_Temp_Flage=0;			// ���±���
__IO uint8_t  Show_TDS_Value=0;				// �ϵ���ʾTDSֵ  0 ����   			1 ��ʾTDSֵ
__IO uint8_t  Test_Filter_Flage=0;		// ������о       0 ������о    1 ������о
__IO uint8_t  Tds_Test_Flage=0;				// TDS����־λ  0 ����� 1 ���

__IO uint8_t Set_Temp_Num=0;					// ���ó�ˮ�¶�   0 ����   1 ����  2 ����  3 ����
__IO uint8_t Set_PH_Num=0;						// ���ó�ˮPH     0 ��ˮ   1 ����  2 �м�  3 ǿ��
__IO uint8_t Set_Temp_Table[4]={25,45,65,80};	
__IO uint8_t Set_PH_Table[4]={7,8,9,10};	
__IO uint16_t Set_ORP_Table[4]={000,150,250,350};	

uint8_t RF_ID_Code[4]={0,1,2,3};			// ID����
uint8_t New_Filter_Flage=0;					  // ����о��־

static SENSERMODE_TypeDef Senser_Mode;// ������ģʽ
static uint8_t Write_RFID_Bit=0;			// ����д�� 0-1
static uint16_t Clear_Count=0;				// �ۼ���ϴ��ˮ��	
static uint16_t Total_Flow=0;					// �ۼƳ�ˮ�� ����ͳ��
static uint8_t Erro_Flage=0;					// �����־ 0λ TDS�쳣   1λ�¶��쳣   2λԭˮ��  3��ˮ��  
static uint8_t RF_Read_Num=0;					// ��ȡ��־ RFID �����ȡ10�ξ�û����Ӧ�򱨾�
static uint8_t Filter_Ready_Flage=0;	// ��о��λ��־
static float Now_Current_Speed=0;			// ��ǰˮ���ٶ�

static void Set_Speak_Value(uint8_t flage)
{
	Speak_Flage=flage;
	TIM_Cmd(TIM2,ENABLE);
}
/***********************************************************************************
 * ��������uint8_t Senser_Value_Read(void)
 * ����  ����ȡ��������������
 * ����  ����
 * ���  ����
************************************************************************************/
uint8_t Senser_Value_Read(void)
{
  // �����˲�ʱ���ʱ
  static uint32_t Senser_Time_SXG=0;	      // ������-ˮ�����
  static uint32_t Senser_Time_YSXU=0;	    	// ������-ԭˮ��ˮλ
  static uint32_t Senser_Time_SSXU=0;	      // ������-��ˮ��ˮλ
  static uint32_t Senser_Time_YSXD=0;	    	// ������-ԭˮ��ˮλ
  static uint32_t Senser_Time_SSXD=0;	      // ������-��ˮ��ˮλ
	uint8_t keycode;
	
	// ˮ��ǰ�״̬  ����״̬Ϊ���� = 0 ����
  if(SENSER_SXG==0)																					// ˮ��ǹر�
  {		
    if(Senser_Mode.SENSER_SXG_MODE==SENSER_SXG_HOLD)				// ����̧��
    {
			Senser_Mode.SENSER_SXG_MODE=SENSER_SXG_HOLD;					// ����̧��
    }
    else if(Senser_Mode.SENSER_SXG_MODE==SENSER_SXG_DOWN)   // ǰһ���ǰ�������
    {
      if(Senser_Time_SXG++>SENSER_FILTER_TIME)							// ��⵽������
      {
        Senser_Mode.SENSER_SXG_MODE=SENSER_SXG_HOLD;				// ����̧��
      }		
    }
    else 																										// ǰһ��û�а�������
    {
      if(Senser_Time_SXG++>SENSER_FILTER_TIME)							// ����ͨ���˲����
      {
        Senser_Mode.SENSER_SXG_MODE=SENSER_SXG_UP;					// �ǰ�̧��
      }
    }	
  }
  else
  {
    Senser_Mode.SENSER_SXG_MODE=SENSER_SXG_DOWN;						// �ǰ����
    Senser_Time_SXG=0;
  }			

	// ԭˮ��ˮλ״̬		����״̬Ϊ��ˮλ  = 0 ����
  if(SENSER_YSX==0)																				 	// ԭˮ��
  {		
    if(Senser_Mode.SENSER_YSX_MODE==SENSER_YSX_DOWN)				// ������λ
    {
			Senser_Mode.SENSER_YSX_MODE=SENSER_YSX_DOWN;					// ������λ
    }
    else 																										// ǰһ��û�а�������
    {
      if(Senser_Time_YSXD++>SENSER_FILTER_TIME)							// ����ͨ���˲����
      {
        Senser_Mode.SENSER_YSX_MODE=SENSER_YSX_DOWN;				// ԭˮ��ȱˮ����ԭˮ���ˮλ�쳣
      }
    }	
		Senser_Time_YSXU=0;
  }
  else
  {
    if(Senser_Mode.SENSER_YSX_MODE==SENSER_YSX_UP)					// ������λ
    {
			Senser_Mode.SENSER_YSX_MODE=SENSER_YSX_UP;						// ������λ
    }
    else 																										// ǰһ��û�а�������
    {
      if(Senser_Time_YSXU++>SENSER_FILTER_TIME)							// ����ͨ���˲����
      {
        Senser_Mode.SENSER_YSX_MODE=SENSER_YSX_UP;					// ԭˮ��ȱˮ����ԭˮ���ˮλ�쳣
      }
    }	
		Senser_Time_YSXD=0;
  }		
	// ��ˮ��ˮλ״̬   ����״̬Ϊ��ˮλ  = 0 ����  
  if(SENSER_SSX==0)																				 	// ��ˮ��
  {		
    if(Senser_Mode.SENSER_SSX_MODE==SENSER_SSX_DOWN)				// ����̧��
    {
			Senser_Mode.SENSER_SSX_MODE=SENSER_SSX_DOWN;					// ����̧��
    }
    else 																										// ǰһ��û�а�������
    {
      if(Senser_Time_SSXD++>SENSER_FILTER_TIME)							// ����ͨ���˲����
      {
        Senser_Mode.SENSER_SSX_MODE=SENSER_SSX_DOWN;				// ��ˮ���ˮ������ˮ��λ���쳣
      }
    }	
		Senser_Time_SSXU=0;
  }
  else
  {
    if(Senser_Mode.SENSER_SSX_MODE==SENSER_SSX_UP)					// ����̧��
    {
			Senser_Mode.SENSER_SSX_MODE=SENSER_SSX_UP;						// ����̧��
    }
    else 																										// ǰһ��û�а�������
    {
      if(Senser_Time_SSXU++>SENSER_FILTER_TIME)							// ����ͨ���˲����
      {
        Senser_Mode.SENSER_SSX_MODE=SENSER_SSX_UP;					// ��ˮ���ˮ������ˮ��λ���쳣
      }
    }	
		Senser_Time_SSXD=0;
  }			

	return keycode;
}


/***********************************************************************************
 * ��������void RF_Read_Info(uint8_t step)
 * ����  ������ģʽ
 * ����  ����
 * ���  ����
************************************************************************************/
void RF_Read_Info(uint8_t *step)
{
	uint8_t data[4]={0,0,0,0};
	uint8_t i=0;
	// ��ȡ����
	switch(*step)
	{
		case 1:	// ��ȡ��ƬID
						EM4305_Read_Data(1);
						if(RF_Read_Num++>=9)
						{
							RF_Read_Num=9;
							LED_LX=0;Filter_Life=0;
							Filter_Ready_Flage=0;
						}
						break;
		case 2:	// �жϿ�ƬID�Ƿ��ù�
						// �����оע�����ת��STEP=8
						// �����о�Ѿ�
						*step=3;
						break;
		case 3:	// ��ȡ��ʼ����־ UID^0x55AA55AA
						EM4305_Read_Data(3);
						break;
		case 4:	// �жϳ�ʼ��״̬
						*step=5;
						break;
		case 5: // ��ȡ��о����DBX
						EM4305_Read_Data(5); 
						break;
		case 6:	// ��ȡ��о���
						EM4305_Read_Data(6); 
						break;
		case 7:	// ��ȡ���ò�Ʒ����
						EM4305_Read_Data(7); 
						break;
		case 8: // ��ȡ��о����A	
						EM4305_Read_Data(8); 
						break;
		case 9:	// ��ȡ��о����B
						EM4305_Read_Data(9); 
						break;
		case 10:	// �ж��Ƿ���Ҫд��
						if(Filter_Count>=1000)
						{
							Filter_Count=0;
							if(Filter_Life>0)
							{
								data[0]=(Filter_Life-1)/256;
								data[1]=(Filter_Life-1)%256;
								data[2]=data[0]^0xFF;
								data[3]=data[1]^0xFF;
								EM4305_Write_Data(8+Write_RFID_Bit,data);			// ѭ����д8 9��������			
								if(Write_RFID_Bit==1) Write_RFID_Bit=0;				// 
								else Write_RFID_Bit=1;												// 
							}
							// ������о����������
							data[1]=(uint16_t)Filter_Life/10;
							data[0]=(uint16_t)Filter_Life%10;								
							for(i=0;i<2;i++)																// ���浱ǰ��о���豸	
							{
								EEP_Write_Date(FILTER_LIFT_ADDR+i,data[i]);
							}			
						}
						else *step=1;																			// ���¶�ȡID
						break;
		default : *step=1;
							break;
	}
}

void EM4305_Get_NewFAX(uint8_t *step)
{
	uint8_t err=0,num=0,i=0;
	uint8_t data[2];
	uint8_t ecccode[4]={0x55,0xAA,0x55,0xAA}; 			// �����
	static uint8_t id_buf[4]={0x00,0x00,0x00,0x00}; // ID�뻺��
	static uint8_t first_read=0,read_num=0;					// �����д
	static uint16_t read_life=0;										// 
	static uint8_t new_filter=0;										// ����о��־
	static uint8_t write_filter_flage=0;						// ������о������־
	static uint8_t test_flage=2;										// ������о����
	
	if(Usart3_NewFax[0]!=0xAA) 
	{
		return;
	}
	if(Usart3_NewFax[1]!=0x0A) 
	{
		return;
	}
	num=Usart3_NewFax[2]+4;
	if(Usart3_NewFax[num]!=0xBB)
	{
		return;
	}
	// У��
	for(i=1;i<num;i++)
	{
		err^=Usart3_NewFax[i];
	}
	if(err!=0) 
	{
		return;
	}
	// �ж��Ƿ��д�ɹ� ��λΪ0���쳣
	if(Usart3_NewFax[3]!=0)  						// ��ȡʧ��
	{
		return;
	}
	RF_Read_Num=0;											// �������������־
  Filter_Ready_Flage=1;								// ��⵽��о
	// ��ȡ����
	switch(*step)
	{
		case 1:	// ��ȡ��ƬID
						for(i=0;i<4;i++)										  // ��ID
						{
							if(Usart3_NewFax[4+i]!=RF_ID_Code[i])  break;
						}
						// �ж���оID
						if(i<4) 														  // ͨ���Ա�ȷ��������о
						{
							new_filter=1;												// ����о
							if(read_num++>=3)									  // ������ȡ3���쳣�ż�¼
							{
								*step=3;												  // ��֤ʶ�����Ƿ���ȷ
							}
						}
						else																	// ��������о
						{
							 read_num=0;												// ��λ������
							 new_filter=0;											// ����о
							 if(test_flage==2)									// ������о�Ƿ�Ϊ������о
							 {
								 *step=3;													// ������ȡ��֤��
							 }
							 else if(test_flage==1) 						// ����оΪ������о
							 {
								 *step=1;													// ѭ����ȡ��оID
								 Filter_Life=10;									// ������о����
							   New_Filter_Flage=0;							// ����о���
							   LED_LX=2;												// ������о							 
							 }
							 else 															// ��ȡ��о����
							 {	
								 *step=8;													// ��ȡ����
							 }
						}
						// ����ID
						for(i=0;i<4;i++)											// ����ID 
						{
							id_buf[i]=Usart3_NewFax[4+i];				// ��ȡID
						}
						break;
		case 2:	// �жϿ�ƬID�Ƿ��ù�
						// �����оע�����ת��STEP=8
						// �����о�Ѿ�
						break;
		case 3:	// ��ȡ��ʼ����־ UID^0x55AA55AA
						for(i=0;i<4;i++)
						{
							if(Usart3_NewFax[4+i]!=(id_buf[i]^ecccode[i]))  break;
						}
						// �ж���оID
						if(i<4) 																// ����о�쳣
						{
							*step=1;															// ������ȡ��оID
						}
						else																		// ��оͨ����֤
						{
							*step=4;															// ����оͨ������
							if(new_filter==1)											// ����о
							{
								// ������оID
								for(i=0;i<4;i++)										// �����ID
								{
									RF_ID_Code[i]=id_buf[i];
									// ���浱ǰ��о���豸	
									EEP_Write_Date( RFID_CODE_ADDR+i,id_buf[i]);
								}							
								// ��������о��־ 0Xaa ��Ҫ��ϴ
								EEP_Write_Date( CLERA_FLAGE_ADDR,0xAA);
								// ���浱ǰ��о���豸	
								Set_Speak_Value(3);							 		// ��ʾ��
								new_filter=0;												// �Ѿ�ʶ��Ϊ����о
								first_read=0;												// �״ζ�д���ھ����д
								New_Filter_Flage=1;							 		// ����о���
								Clear_Count=0;									 		// �����ϴˮ������
								Filter_Count=0;									 		// �����оˮ������
								read_num=0;						  				 		// �������о��ȡ��ֹ�������										
							}
						}
						break;
		case 4:	// �жϳ�ʼ��״̬
						break;
		case 5: // ��ȡ��о����DBX
						*step=6;					
						break;
		case 6:	// ��ȡ��о��� ����̿��оCC
						if(Usart3_NewFax[4]==0x43&&Usart3_NewFax[5]==0x43)				// ����оΪ����̿��о	
						{
							 *step=8;	
							 test_flage=0;																					// �ǲ�����о
						}
						else if(Usart3_NewFax[4]==0x54&&Usart3_NewFax[5]==0x54)		// ����оΪ������о
						{
							test_flage=1; Filter_Life=10;														// ������о����
							New_Filter_Flage=0;																			// ����о���
							LED_LX=2;																								// ������о
							*step=1;																								// ���¶�ȡ��оID
						}
						break;
		case 7:	// ��ȡʹ�ò�Ʒ����
						*step=8;	
						break;
		case 8: // ��ȡ��о����A 4567
						if((Usart3_NewFax[4]^Usart3_NewFax[6])==0xFF&&(Usart3_NewFax[5]^Usart3_NewFax[7])==0xFF)
						{
							read_life=Usart3_NewFax[4]*256+Usart3_NewFax[5];
							first_read=1;
						}
						else		// У�����
						{
							first_read=0;
						}
						*step=9;																									// ��ȡ��о�����Ĵ���B
						break;
		case 9:	// ��ȡ��о����B 4567
						if((Usart3_NewFax[4]^Usart3_NewFax[6])==0xFF&&(Usart3_NewFax[5]^Usart3_NewFax[7])==0xFF)
						{
							if(first_read==1)
							{
								if(read_life>=Usart3_NewFax[4]*256+Usart3_NewFax[5])	// �Ե���Ϊ׼
								{
									read_life=Usart3_NewFax[4]*256+Usart3_NewFax[5];
								} 	
							}
						}
						// У����ϸ�����о״̬
						Filter_Life=read_life;	
						// ������о����
						if(write_filter_flage==0)
						{
							write_filter_flage=1;
							data[1]=(uint16_t)Filter_Life/10;
							data[0]=(uint16_t)Filter_Life%10;								
							for(i=0;i<2;i++)						// ���浱ǰ��о���豸	
							{
								EEP_Write_Date(FILTER_LIFT_ADDR+i,data[i]);
							}							
						}
						// �ж���о״̬
						if(Filter_Life>335) LED_LX=5;						// ��о����
						else if(Filter_Life>166) LED_LX=4;			// ��о����
						else if(Filter_Life>50) LED_LX=3;				// ��оһ��
						else if(Filter_Life>5) LED_LX=2;				// ��о���
						else LED_LX=1;													// ��о�����˸
						*step=10;																// ���¶�ȡID
						break;
		case 10:	// д��о
						*step=1;
						break;
		default : *step=1;
							break;
	}
}


/***********************************************************************************
 * ��������uint8_t Work_In_Set(void)
 * ����  ������ģʽ
 * ����  ����
 * ���  ����
************************************************************************************/
uint8_t Work_In_Set(void)
{
	static KEYMODE_TypeDef keymode;	      // ��ǰ����ģʽ		
	static uint8_t set_temp=0;						// �����¶�ʹ��
	static uint16_t waite_time=0;					// ���ȵȴ���ʱ
	uint8_t data[2],i;

	// Erro_Flage=0;							// �����־ 0λ TDS�쳣   1λ�¶��쳣   2λԭˮ��  3��ˮ��  
	// ��⴫����״̬
	if(Yswater_Temp>MAX_YSTEMP)		// ԭˮ����
	{
		LED_GW=1;
		Erro_Flage|=0x02;						// ���󱨾�
	}
	else
	{
		LED_GW=0;
		Erro_Flage&=0xFD;						// ȡ�����󱨾�
	}
	// 
	if(Senser_Mode.SENSER_YSX_MODE==SENSER_YSX_UP)
	{
		LED_QS=1;										// ԭˮ��ȱˮ����ָʾ�Ƶ���
		Erro_Flage|=0x04;						// ���󱨾�
	}
	else
	{
		LED_QS=0;										// ԭˮ��ȱˮ����ָʾϨ��
		Erro_Flage&=0xFB;						// ȡ�����󱨾�
	}
	if(Senser_Mode.SENSER_SSX_MODE==SENSER_SSX_UP)
	{
		LED_MS=1;										// ԭˮ��ȱˮ����ָʾ�Ƶ���
		Erro_Flage|=0x08;						// ���󱨾�
	}
	else
	{
		LED_MS=0;										// ԭˮ��ȱˮ����ָʾϨ��
		Erro_Flage|=0xF7;						// ���󱨾�
	}	
	if(New_Filter_Flage==1)				// ����о��Ҫ��ϴ
	{
		LED_QX=1;
		Set_Temp_Num=0;
		set_temp=0;
	}
	else
	{
		LED_QX=0;
	}
	// ����
	if(Standby_Time>=STANDBY_NUM)	 								// ��������ģʽ
	{
		set_temp=0;																	// ��ֹ�����¶�
		Set_Temp_Num=0;															// ��λ�¶��趨ֵ
		Tds_Test_Flage=0;														// ֹͣ����TDS
	}
	// ���ڵ��ǿ��
	if(keymode.KEY_PH_MODE!=Key_Now_Mode.KEY_PH_MODE)
	{
		keymode.KEY_PH_MODE=Key_Now_Mode.KEY_PH_MODE;
		if(Standby_Mode==0)													// �Ǵ���ģʽ��
		{
			if(Key_Now_Mode.KEY_PH_MODE==KEY_PH_DOWN)
			{
				if(Work_Step==0&&New_Filter_Flage==0&&LED_LX!=1)		// δ�������ܵ��� 
				{
					if(Set_PH_Num<3) Set_PH_Num++;
					else Set_PH_Num=0;			
				}
				else
				{
					Work_Step=11;																			// ֹͣ����
				}
				// �����쳣���ܵ��
				if(LED_LX<=1||LED_TDS==1||LED_GW==1)								// ˮ���쳣���ܵ��
				{
					Set_PH_Num=0;Set_Temp_Num=0;
				}
				if(Set_PH_Num==0)
				{
					Display_Rama[1]=0x12;
					Display_Rama[0]=0x12;
					Display_Ramb[0]=0x12;
					Display_Ramb[1]=0x12;
					Display_Ramb[2]=0x12;	
				}
				else
				{
					Display_Rama[1]=Set_PH_Table[Set_PH_Num]/10;				// ��ʾ�¶�
					Display_Rama[0]=Set_PH_Table[Set_PH_Num]%10;				// ��ʾ�¶�
					Display_Ramb[0]=Set_ORP_Table[Set_PH_Num]/100;
					Display_Ramb[1]=(Set_ORP_Table[Set_PH_Num]%100)/10;
					Display_Ramb[2]=Set_ORP_Table[Set_PH_Num]%10;			
				}
				Set_Speak_Value(2);										// ��������			
			}
		}
		else Standby_Mode=0;
				
		// ����Ƿ��ϵ��ʼ����ʾTDSֵ
		if(Key_Now_Mode.KEY_PH_MODE==KEY_PH_HOLD) 
		{
			if(Init_Falge==0) 											// δ��ʼ��
			{
				Show_TDS_Value=1;											// ��ʾTDS
				Set_Speak_Value(3);										// ������
				Display_Rama[1]=0x00;									// ����汾��
				Display_Rama[0]=0x02;									// ����汾��
				Set_PH_Num=0;													// ��ʾǿ��
			}
			else if(Show_TDS_Value==1)							// ������ʾTDS
			{
				Show_TDS_Value=0;											// �ر���ʾTDS
				Set_Speak_Value(3);										// ������
			}
		}		
	}
	// �����¶ȼ�����
	// ���ڼ����¶�
	if(keymode.KEY_TEM_MODE!=Key_Now_Mode.KEY_TEM_MODE)
	{
		keymode.KEY_TEM_MODE=Key_Now_Mode.KEY_TEM_MODE;
		// ��������
		if(Key_Now_Mode.KEY_TEM_MODE==KEY_TEM_HOLD)
		{
			if(set_temp==1)												// �¶��Ѿ�����
			{
				set_temp=0;													// ����
				Set_Temp_Num=0;											// ��λ�¶���ʾ
				Standby_Time=STANDBY_NUM;						// ��������ģʽ
			}
			else if(LED_LX!=1)										// �¶�����
			{
				set_temp=1;													// ����
				Set_Temp_Num=1;											// ��λ�¶���ʾ
			}
			Set_Speak_Value(2);													// ����
		}
		// �̰������¶�
		if(Standby_Mode==0)														// �Ǵ���ģʽ��
		{
			if(Key_Now_Mode.KEY_TEM_MODE==KEY_TEM_DOWN)
			{
				if(Work_Step==0&&set_temp==1&&LED_LX!=1)	// δ�������ܵ���
				{			
					if(Set_Temp_Num<3)	Set_Temp_Num++;			// �¶�����
					else Set_Temp_Num=0;										// 
				}
				else
				{
					Work_Step=11;														// ֹͣ����
				}
				Set_Speak_Value(2);
			}
		}
		else Standby_Mode=0;
		// �����쳣
		if(LED_LX<=1||LED_TDS==1||LED_GW==1)
		{
			Set_PH_Num=0;Set_Temp_Num=0;
		}
	}
	// ��ʼ����������
	if(keymode.KEY_START_MODE!=Key_Now_Mode.KEY_START_MODE)
	{
		keymode.KEY_START_MODE=Key_Now_Mode.KEY_START_MODE;
		if(Key_Now_Mode.KEY_START_MODE==KEY_START_DOWN)
		{
			if(Standby_Mode==0)												// �Ǵ���ģʽ��		
			{
				if(Work_Step==0) 												// ����ģʽ�²��ܲ���
				{	
					Work_Step=1;	Total_Flow=0;						// ��ʼ��ˮ
					if(LED_TDS==1)												// TDS����ֹͣ���
					{
						Display_Rama[1]=0x12;								// TDS���߲���ʾ�����
						Display_Rama[0]=0x12;
						Display_Ramb[0]=0x12;
						Display_Ramb[1]=0x12;
						Display_Ramb[2]=0x12;							
					}
				}
				else {	Work_Step=11;	}									// ֹͣ
			}
			else Standby_Mode=0;											// �������ģʽ
			Set_Speak_Value(2);			
			// �����쳣
			if(LED_LX<=1||LED_TDS==1||LED_GW==1)
			{
				Set_PH_Num=0;Set_Temp_Num=0;
			}
		}
	}
	// ����ģʽ
	switch(Work_Step)															// ����
	{
		case 0:	// ����ģʽ
						JDQ_Heat(0);												// ֹͣ������
						ELE_ConP(0);												// ���ӵ�����
						ELE_ConN(0);												// ���ӵ�����		
						VALUE_Out(0);												// �رյ�ŷ�
						Cur_PID_Reset();										// ��λ���
						Motor_PID_Reset();									// ��λ���
						break;
		case 1: // ���ԭˮ���Ƿ���ˮ
						// �����ˮ��ˮ�Ƿ�����
						// �����о�Ƿ�װ��
						// ԭˮˮ�µ���5���϶Ȳ�����
						if((Senser_Mode.SENSER_YSX_MODE==SENSER_YSX_UP)||(Senser_Mode.SENSER_SSX_MODE==SENSER_SSX_UP)||(Filter_Ready_Flage==0)||(Yswater_Temp<5))
						{
							Work_Step=11;											// �쳣��ת��ֹͣ
							Set_Speak_Value(3);								// ������
						}
						else
						{
							Work_Step=2;											// ��ת�Ƿ���ת������о
						}
						break;
		case 2:	// ����Ƿ�ջ�����о
						// �����������о�������ϴģʽ
						if(New_Filter_Flage==1&&LED_LX!=1)																 			// �ջ�����о  ��о����
						{
							Work_Step=7;											// ���뷴ϴ
						}
						else
						{
							if(Set_Temp_Num==0)								// ����ˮ
							{
								Work_Step=5;										// ������
							}
							else															// ����ˮ
							{
								Work_Step=3;										// ��ת������
							}						
						}
						break;
		case 3:	// ����������ǰ���ˮ·���Ƿ���ˮ(ͨ��TDS�ж�)
						if(Yswater_Tds<MAX_TDS_VALUE&&Yswater_Temp<MAX_YSTEMP&&LED_LX!=1)				// ˮ������ ˮ������ ��о����
						{						
							Work_Step=4;											// ��Ҫ����		
							// 2019-11-28 Ϊ�˱��������� ��Ϊ�ȳ�ˮ����� ȥ��Ԥ�ȹ���
							// JDQ_Heat(1);											// ����������		
							// 2019-11-28 Ϊ�˱��������� ��Ϊ�ȳ�ˮ����� ȥ��Ԥ�ȹ���s
						}
						else																// ��������
						{
							Work_Step=5;											// ˮ�ÿ�ʼ��ˮ(�����쳣)
						}						
						break;						
		case 4:	// Ԥ�ȷ������¶ȸ���Ԥ���¶ȲŹ���
						if(Set_Temp_Num==1&&Hmwater_Temp>20)
						{
							Work_Step=5;											// ˮ�ÿ�ʼ��ˮ
						}
						if(Set_Temp_Num==2&&Hmwater_Temp>25)
						{
							Work_Step=5;											// ˮ�ÿ�ʼ��ˮ
						}
						if(Set_Temp_Num==3&&Hmwater_Temp>30)
						{
							Work_Step=5;											// ˮ�ÿ�ʼ��ˮ
						}	
						// ��ʱʱ������
						if(waite_time++>200)								// Ԥ��ʱ�䲻����2S
						{
							Work_Step=5;											// ˮ�ÿ�ʼ��ˮ
						}
						// 2019-11-28 Ϊ�˱��������� ��Ϊ�ȳ�ˮ����� ȥ��Ԥ�ȹ��� ����
						Work_Step=5;												// ˮ�ÿ�ʼ��ˮ
						// 2019-11-28 Ϊ�˱��������� ��Ϊ�ȳ�ˮ����� ȥ��Ԥ�ȹ���
						break;	
		case 5: // �ж��Ƿ���Ҫ���
						waite_time=0;												// ��λ��ʱ��
						if(Set_PH_Num==0)										// ������  
						{
							Work_Step=8;											// ����ˮ��
						}
						else																// ��Ҫ�������
						{	
							Work_Step=6;											// ��ת����ʼ���
						}
						break;
		case 6:	// ��ʼ���
						if(Yswater_Tds<MAX_TDS_VALUE&&Yswater_Temp<MAX_YSTEMP&&LED_LX!=1)					// ˮ������  ˮ������ ��о����
						{
							ELE_ConP(0);											// ���ӵ�����
							ELE_ConN(1);											// ���ӵ�����	
							delay_ms(10);											// ��ʱ��
							VALUE_Out(1);											// �򿪵�ŷ�
							Start_Cur_PID(Set_PH_Num);				// ��ʼ���
						}
						Work_Step=8;												// ������һ����
						break;												
		case 7:	// �ж��Ƿ���������ϴ
						if(Yswater_Tds<MAX_TDS_VALUE&&Yswater_Temp<MAX_YSTEMP&&LED_LX!=1)					// ˮ������  ˮ������ ��о����
						{
							Set_PH_Num=2;											// �м�
							ELE_ConP(1);											// ���ӵ�����		
							ELE_ConN(0);											// ���ӵ�����		
							delay_ms(10);											// ��ʱ��
							VALUE_Out(1);											// �򿪵�ŷ�
							Start_Cur_PID(Set_PH_Num);				// �м��ϴ							
						}
						Work_Step=8;												// ������һ����			
						break;										
		case 8:	// ����ˮ�ÿ�ʼ��ˮ
						Start_Motor_PID(Set_Temp_Num);			// ����������ٿ���ˮ��		
						// 2019-11-28 Ϊ�˱��������� ��Ϊ�ȳ�ˮ����� ȥ��Ԥ�ȹ���
						// ��ʱʱ������
						if(waite_time++>200)								// Ԥ��ʱ�䲻����2S
						{
							Work_Step=9;											// ˮ�ÿ�ʼ��ˮ
						}		
						// 2019-11-28 Ϊ�˱��������� ��Ϊ�ȳ�ˮ����� ȥ��Ԥ�ȹ���
						// Work_Step=9;
						// 2019-11-28 Ϊ�˱��������� ��Ϊ�ȳ�ˮ����� ȥ��Ԥ�ȹ���
						break;
		case 9: // ��ԭˮˮ�¸� ���߷������¶ȹ�����λ�������ֵ
						if((Yswater_Temp>=MAX_YSTEMP||Hmwater_Temp>=90)&&(Set_Temp_Num!=0))				// ��λ�¶���ֵ
						{
							Reset_Motor_Value();							// ��λ������ٿ���ˮ��		
						}		
						// 2019-11-28 Ϊ�˱��������� ��Ϊ�ȳ�ˮ����� ȥ��Ԥ�ȹ���
						// �ж��Ƿ���Ҫ����������
						waite_time=0;												// ��λ��ʱ��
						if(Set_Temp_Num!=0&&Yswater_Tds<MAX_TDS_VALUE&&Yswater_Temp<MAX_YSTEMP&&LED_LX!=1)				// ��Ҫ����������  ˮ������ ˮ������ ��о����
						{
							JDQ_Heat(1);											// ����������	
						}
						// 2019-11-28 Ϊ�˱��������� ��Ϊ�ȳ�ˮ����� ȥ��Ԥ�ȹ���
						Work_Step=10;												// ��ˮ
						break;
		case 10:// ��ˮ�еȴ�ֹͣ����
						// �����ˮ��ˮ�Ƿ�����
						if((Senser_Mode.SENSER_YSX_MODE==SENSER_YSX_UP)||(Senser_Mode.SENSER_SSX_MODE==SENSER_SSX_UP))
						{
							Work_Step=11;
							Set_Speak_Value(3);																	// ����
						}	
						// ͳ��ˮ���ٶ� ��������ֵ
						if(Curent_Flage==1)																		// ������ʱ����־
						{
							Curent_Flage=0;																			// ����
							// 2019-11-28 �޸�ˮ��ͳ��  ����WATER_SPEED������ͷ�ļ����޸�Ϊ25.0
							Now_Current_Speed=WATER_SPEED*(PUM2L_OUTPUT/4000.0);// �����ˮ�� 1750ml - 2000mlÿ���Ӳ���  1750/60=29   2000/60=33
							Filter_Count+=Now_Current_Speed;										// ��оʣ��������ʱ
							Total_Flow+=Now_Current_Speed;											// ��������ˮ��
							// �ж�����Ƿ�ϴ��Ҫ����ˮ
							if(New_Filter_Flage==1)															// ����о��ϴ��
							{
								Clear_Count+=Now_Current_Speed;										// ��ϴ��ˮ����
								if(Clear_Count>=MAX_FANXI_VALUE)									// ���ϴˮ��
								{
									Work_Step=11;																		// 
									New_Filter_Flage=0;															//	
									Clear_Count=0;																	//			
									// ��������о��־ 0x00������ϴ
									EEP_Write_Date( CLERA_FLAGE_ADDR,0x00);					//				
								}
							}		
							// ��󵥴γ�ˮ������2L
							if(Total_Flow>MAX_TOTAL_FLOW)												// ���γ�ˮ���ֵ	
							{
								Work_Step=11;Total_Flow=0;												// ֹͣ
							}		
							// ��ʼ���TDS
							if(Total_Flow>TEST_TDS_VALUE)												// ���TDS��ֵ
							{
								Tds_Test_Flage=1;																	// ��ʼ���TDS
							}
							// ��о�쳣״̬
							if(LED_LX<=1)																				// ��оΪ��
							{
								JDQ_Heat(0);																			// ֹͣ������
								ELE_ConP(0);																			// ���ӵ�����
								ELE_ConN(0);																			// ���ӵ�����		
								Set_PH_Num=0;																			// �ر�
								Set_Temp_Num=0;																		// �ر�
							}
						}
						break;
		case 11:	// �ֶ�ֹͣ ֹͣ��⼰ˮ��		 				
						JDQ_Heat(0);							// ֹͣ������
						Motor_PID_Reset();				// ��λ���
						Cur_PID_Reset();					// ��λ������
						Tds_Test_Flage=0;					// ֹͣ���TDS
						delay_ms(10);							// ��ʱ�ر�
						ELE_ConP(0);							// ���ӵ�����
						ELE_ConN(0);							// ���ӵ�����	
						VALUE_Out(0);							// �رյ�ŷ�
						Work_Step=12;							// ��ת		
						break;
		case 12:	// ֹͣ�����ָ���ʾ
						if(Set_PH_Num==0)					// ����RAM
						{
							Display_Rama[1]=0x12;
							Display_Rama[0]=0x12;
							Display_Ramb[0]=0x12;
							Display_Ramb[1]=0x12;
							Display_Ramb[2]=0x12;	
						}
						else
						{

							Display_Ramb[0]=Set_ORP_Table[Set_PH_Num]/100;
							Display_Ramb[1]=(Set_ORP_Table[Set_PH_Num]%100)/10;
							Display_Ramb[2]=Set_ORP_Table[Set_PH_Num]%10;			
						}
						// ���浥����ˮ�����洢��
						if(Filter_Count<1000)
						{
							data[1]=(uint16_t)Filter_Count/10;
							data[0]=(uint16_t)Filter_Count%10;	
							// ���浱ǰ��о���豸		
							for(i=0;i<2;i++)
							{
								// ���浱ǰ��о���豸	
								EEP_Write_Date(RFID_CURRENT_SPEED+i,data[i]);
							}												
						}	
						// ˮ���쳣 
						if(LED_LX<=1||LED_TDS==1||LED_GW==1)			// ˮ���쳣���ܵ��
						{
							Set_PH_Num=0;Set_Temp_Num=0;						// ��λ
						}
						Work_Step=0;								// ��ת	
						break;		
		default : // �����쳣ֵ
						Cur_PID_Reset();
						Motor_PID_Reset();
						Work_Step=11;
						break;
	}	

	return 0;
}


/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/
