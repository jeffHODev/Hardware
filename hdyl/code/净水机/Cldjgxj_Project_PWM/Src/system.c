/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "system.h"
#include "filter.h"
#include "bsp_led.h"
#include "feeprom.h"
#include "pid.h"
#include "tim.h"

KEYMODE_TypeDef Key_Now_Mode;				// ��������
KEYMODE_TypeDef Key_Last_Mode;			// ��������
// ϵͳ����
uint8_t Power_Mode=3;								// ��Դģʽ 0 �ػ�  1 ������ʾ����  2 ��������  3
uint16_t MSecond=0;					 				// ϵͳ���к���
uint8_t  Second=0;					 				// ϵͳ������ 
uint8_t  Minute=0;									// ϵͳ���з���
uint16_t Hour=0;										// ϵͳ����Сʱ 
uint8_t  MS_Flage=0;								// 10ms��־λ
uint8_t Hour_Flage=0;								// 1Сʱ��־
uint8_t  Second_flage=0;						// ���־
uint8_t  Speak_Mode=0;							// ����������ģʽ 0 ����  1 ��һ��  2 ������
uint8_t JS_VA_Time=0;								// ��ˮ�������ۼ�ʱ��
// ���в���
uint8_t ElE_Work_Mode=0;						// ��⹤��ģʽ
uint8_t ELE_Strength=1;							// ���ǿ�� 0 ��ˮ 1 ������  2 ��  3 ǿ
uint8_t SET_Ele_Value=100;					// ���õ���ѹ
uint8_t TLP0501_Value=0;						// TLP0501����ֵ
float Flow_Now_Value=0;							// ��ǰˮ����ֵ
float Flow_Now_Fre=0;								// ��ǰƵ��ֵ
float Last_Float_Flow=0;						// �ϴ���ˮ��
double Fanxi_Float_Flow=0;					// �ۼƷ�ϴ����
double Toatl_Float_Flow=0;					// �ۼ���ˮ����

__IO uint8_t LLJ_Rest_Time=0;				// ��λ��������ʱʱ��
__IO uint8_t LLJ_Start_Flage=0;			// ������������־λ
__IO uint8_t LLJ_End_Flage=0;				// ������������־λ
__IO uint16_t LLJ_Now_Value=0;			// �����Ƶ�ǰֵ

uint8_t Fanxi_Flage=0;							// ��ϴ��־
uint8_t Auto_Fanxi_Flage=0;					// �Զ���ϴ��־
uint8_t Fanxi_Mode=0;								// ���ڷ���ϴģʽ
uint16_t Fanxi_Delay_Time=0;				// ��ϴ��ʱʱ��  ���û�ʹ�ú�30���Ӻ�ϴ
uint16_t Fanxi_Continue_Time=0;			// ��ϴ����ʱ��
uint16_t Chongxi_Continue_Time=0;		// ��ϴ����ʱ��
uint8_t Fanxi_Step=0;								// ��ϴ����
uint8_t Queshi_Flage=0;							// ȱˮ��־
uint16_t Queshui_Delay_Time=0;			// ȱˮ��ʱʱ��
uint16_t Fanxi_Num=0;								// ��ϴ��ɴ���
uint16_t Fanxi_Value_Table[5]={500,400,300,200,100};
uint16_t Auto_Fanxi_Value=200;			// �Զ���ϴ����ֵ
/***********************************************************************************
 * ��������delay_us
 * ����  ��us��ʱ
 * ����  ����
 * ���  ����
************************************************************************************/
void delay_us(uint32_t nCount)
{
	uint32_t i,j;
	for(i=0;i<nCount;i++)
	{
  	for(j=0;j<10;j++)
		{
			__ASM("NOP");
		}
	}
}
/***********************************************************************************
 * ��������delay_ms
 * ����  ��ms��ʱ
 * ����  ����
 * ���  ����
************************************************************************************/
void delay_ms(uint32_t nCount)
{
	uint32_t i;
	for(i=0;i<nCount;i++)
	{
		delay_us(1000);
	}  	
}
/***********************************************************************************
 * ��������void Button_Value_Read(void)
 * ����  �����빤��ģʽ
 * ����  ����
 * ���  ����
************************************************************************************/
void Button_Value_Read(void)
{
  static uint16_t key_ele_time,key_res_time,key_val_open_time,key_val_close_time;
	
	//KEY_ELE
  if( KEY_ELE_Read==GPIO_PIN_RESET)										// ��ֵ�ж�													
  {		
    if(Key_Now_Mode.KEY_ELE_MODE==KEY_ELE_HOLD)				//		
    {
      Key_Now_Mode.KEY_ELE_MODE=KEY_ELE_HOLD;					
    }
    else if(Key_Now_Mode.KEY_ELE_MODE==KEY_ELE_DOWN)	// ǰһ���ǰ�������
    {
      if( key_ele_time++>BUTTON_LONG_TIME)						// ��⵽������
      {
        Key_Now_Mode.KEY_ELE_MODE=KEY_ELE_HOLD;				// 
      }		
    }
    else 																							// ǰһ��û�а�������
    {
      if( key_ele_time++>BUTTON_FILTER_TIME)					// ����ͨ���˲����
      {
        Key_Now_Mode.KEY_ELE_MODE=KEY_ELE_DOWN;				// 
				Speak_Flage=2;																//
      }
    }	
  }
  else
  {
    Key_Now_Mode.KEY_ELE_MODE=KEY_ELE_UP;							// ����̧��
    key_ele_time=0;                                   // 
  }	
	//KEY_RES
  if( KEY_RES_Read==GPIO_PIN_RESET)										// ��ֵ�ж�													
  {		
    if(Key_Now_Mode.KEY_RES_MODE==KEY_RES_HOLD)				//		
    {
      Key_Now_Mode.KEY_RES_MODE=KEY_RES_HOLD;					
    }
    else if(Key_Now_Mode.KEY_RES_MODE==KEY_RES_DOWN)	// ǰһ���ǰ�������
    {
      if( key_res_time++>BUTTON_LONG_TIME)						// ��⵽������
      {
        Key_Now_Mode.KEY_RES_MODE=KEY_RES_HOLD;				// 				
      }		
    }
    else 																							// ǰһ��û�а�������
    {
      if( key_res_time++>BUTTON_FILTER_TIME)					// ����ͨ���˲����
      {
        Key_Now_Mode.KEY_RES_MODE=KEY_RES_DOWN;				// 
				Speak_Flage=2;
      }
    }	
  }
  else
  {
    Key_Now_Mode.KEY_RES_MODE=KEY_RES_UP;							// ����̧��
    key_res_time=0;                                  	// 
  }	
	//KEY_VAL
  if( TEST_VAL_Read==GPIO_PIN_RESET)									// �����ź�											
  {		
		key_val_close_time=0;															// �ط���ʱ����
    if(Key_Now_Mode.KEY_VAL_MODE==KEY_VAL_OPEN)				// ����״̬	
    {
      Key_Now_Mode.KEY_VAL_MODE=KEY_VAL_OPEN;					// ����
			JS_VA_SET(1);																		// ���ý�ˮ����	
    }
    else if( key_val_open_time++>HIGH_PRESS_OPEN_TIME)// ��⵽������
		{
       Key_Now_Mode.KEY_VAL_MODE=KEY_VAL_OPEN;				// 
			 JS_VA_SET(1);																	// ���ý�ˮ����	
    }
		else {Key_Now_Mode.KEY_VAL_MODE=KEY_VAL_NULL;}		// ���ź�δȷ��
  }
  else																								// �����ź�
  {
		key_val_open_time=0;															// ������ʱ����
    if(Key_Now_Mode.KEY_VAL_MODE==KEY_VAL_CLOSE)			// 	
    {
			 Key_Now_Mode.KEY_VAL_MODE=KEY_VAL_CLOSE;				// �ط�״̬
			 if(ElE_Work_Mode==0&&Fanxi_Step==0)						// �ͻ�û��ʹ��ˮ�豸Ҳδ�ڷ�ϴ
			 {
					JS_VA_SET(0);																// ���ý�ˮ���ر�			 
			 }		
    }
    else if(key_val_close_time++>HIGH_PRESS_CLOSE_TIME)// ��⵽�ط��ź�
		{
       Key_Now_Mode.KEY_VAL_MODE=KEY_VAL_CLOSE;				// �ط�
			 if(ElE_Work_Mode==0&&Fanxi_Step==0)						// �ͻ�û��ʹ��ˮ�豸Ҳδ�ڷ�ϴ
			 {
					JS_VA_SET(0);																// ���ý�ˮ���ر�			 
			 }
    }
		else {Key_Now_Mode.KEY_VAL_MODE=KEY_VAL_NULL;}		// ���ź�δȷ��
  }	
}

/***********************************************************************************
 * ��������void User_Work_Set(void)
 * ����  ���û�����ģʽ
 * ����  ����
 * ���  ����
************************************************************************************/
void User_Work_Set(void)
{
	// ���ڵ��ģʽ
	if(Key_Now_Mode.KEY_ELE_MODE!=Key_Last_Mode.KEY_ELE_MODE)
	{
		Key_Last_Mode.KEY_ELE_MODE=Key_Now_Mode.KEY_ELE_MODE;
		if(Key_Now_Mode.KEY_ELE_MODE==KEY_ELE_DOWN&&LLJ_Start_Flage==0&&Fanxi_Step==0)		// ��ˮ���ܵ�����λ
		{
			if(ELE_Strength++>2)
			{
				ELE_Strength=0;
			}	
			HAL_FLASH_Unlock();																							// Flash ����
			EE_WriteVariable(VirtAddVarTab[1],ELE_Strength); 								// ������ǿ��
			HAL_FLASH_Lock();																								// Flash ����
		}
		Standby_Time_Delay=STANDBY_TIME;
	}
	// ����ϴ
	if(Key_Now_Mode.KEY_RES_MODE!=Key_Last_Mode.KEY_RES_MODE)
	{
		Key_Last_Mode.KEY_RES_MODE=Key_Now_Mode.KEY_RES_MODE;
		if(Key_Now_Mode.KEY_RES_MODE==KEY_RES_DOWN)
		{
			if(Fanxi_Mode==1)																								// �ر���ͨ����ˮ��			
			{
				LED_ZS=0;	LED_DJ=0; LED_CX=0;										
				Fanxi_Mode=0;SS_VA_SET(0);ST_VA_SET(0);															
			}
		}		
		if(Key_Now_Mode.KEY_RES_MODE==KEY_RES_HOLD&&LLJ_Start_Flage==0&&Queshi_Flage==0)		// ��ˮ���ܷ���ϴ
		{
			if(Fanxi_Mode==1)	{	Fanxi_Mode=0;}
			else {	Fanxi_Flage=1;	}	
			Speak_Flage=6;																									// ��ʼ����ϴ
		}
		Standby_Time_Delay=STANDBY_TIME;
	}
} //end

/***********************************************************************************
 * ��������void Run_Work_Mode(void)
 * ����  ���û�����ģʽ
 * ����  ����
 * ���  ����
************************************************************************************/
void Run_Work_Mode(void)
{	
	uint16_t value=0;
	
	switch(ElE_Work_Mode)
	{
		case 0:	// ����ģʽ
						if(LLJ_Start_Flage==1&&Fanxi_Mode==0)	// ˮ���ж������Ҳ��ڷ�ϴ������
						{
							ElE_Work_Mode=1;			// ������ˮģʽ
							JS_VA_SET(1);					// ���ý�ˮ����	
							LED_ZS=1;							// ��ˮָʾ�Ƶ���
						}
						break;
		case 1:	// �ж���о״̬
						if(LED_PP>0&&LED_C1>0&&LED_UDF>0&&LED_C2>0)		// ��о����
						{		
							if(ELE_Strength!=0)		// �������
							{
								ELE_CONA_SET(1);		// �����ȡ��ˮ �̵�����������
								ELE_CONB_SET(0);		// �����ȡ��ˮ
								ST_VA_SET(0);				// �ر���ͨ��
								SS_VA_SET(1);				// ����ˮ��											
								LED_DJ=1;						// ���ָʾ�Ƶ���		
								delay_ms(50);				// ��ʱ
							}		
							Speak_Flage=3;				// ��ʼ���
						}
						else	{Speak_Flage=4;}	// δ���
						ElE_Work_Mode=2;				// ������ˮģʽ						
						break;
		case 2:	// ���ģʽ		
						HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); 		// ��PWM
						if(ELE_Strength!=0)			// �Ǿ�ˮģʽ
						{
							Start_Cur_PID(ELE_Strength);// ��������ѹ	
						}						
						ElE_Work_Mode=3;				// ������ˮģʽ							
						break;
		case 3:	// ����еȴ��û��ر�
						if(LLJ_Start_Flage==0)	// ˮ�ֲ�תֹͣ��ˮ
						{
							Stop_Cur_PID();				// ������ѹ����		
							HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
							ElE_Work_Mode=4;			// ֹͣ��ˮ
							delay_ms(50);					// ��ʱ
						}
						break;
		case 4:	// ֹͣ��ˮ
						value=(uint16_t)Toatl_Float_Flow;
						HAL_FLASH_Unlock();												// Flash ����
						EE_WriteVariable(VirtAddVarTab[3],value);	// �ۼ�ˮ������
						HAL_FLASH_Lock();													// Flash ����
						ELE_CONA_SET(0);				// ֹͣ���
						ELE_CONB_SET(0);				// �����ȡ��ˮ			
						ST_VA_SET(0);						// �ر���ͨ��
						SS_VA_SET(0);						// �ر���ˮ��				
						LED_ZS=0;								// ��ˮָʾ�Ƶ���
						LED_DJ=0;								// ���ָʾ�Ƶ���
						ElE_Work_Mode=0;				// ������ˮģʽ
						break;
		default : // 
						break;
	}
} //end




// ADC��ȡϵͳ���в���
void ADC_Read_System_Value(void)
{
	uint8_t i;
	static uint8_t init=0;
	static float ele_value,ele_current,sys_value;
	static double ele_current_zero;
	double ele_value_sum=0,ele_current_sum=0,sys_value_sum=0;	// ����ѹ ������ �豸��ѹ
	double res;
	
	for(i=0;i<ADC_FITER;i++)
	{
		ele_current_sum+=ADC_Value[i][0];
		ele_value_sum+=ADC_Value[i][1];
		sys_value_sum+=ADC_Value[i][2];
	}
	ele_current_sum/=ADC_FITER;
	ele_value_sum/=ADC_FITER;
	sys_value_sum/=ADC_FITER;
	
	// ADC���ݳ�ʼ��	����100������ 1000ms
	if(init<VALUE_FILTER_NUM)
	{
		init++;
		ele_current+=ele_current_sum;
		ele_value+=ele_value_sum;
		sys_value+=sys_value_sum;
		if(init==VALUE_FILTER_NUM) 
		{
			ele_current/=VALUE_FILTER_NUM;
			ele_value/=VALUE_FILTER_NUM;
			sys_value/=VALUE_FILTER_NUM;
			ele_current_zero=ele_current;
			ele_value=(ele_value*(3.3/4095.0))/(1.2/11.2);
			ele_current=0;
			sys_value=(sys_value*(3.3/4095.0))/(1.2/11.2);
		}
		return;
	}
	// �������ѹ
	res=(ele_value_sum*(3.3/4095.0))/(1.2/11.2);
	ele_value=ele_value-ele_value/VALUE_FILTER_NUM+res/VALUE_FILTER_NUM;
	ELE_Value=ele_value;
	// �궨������
	// ���������
	if(ElE_Work_Mode==0&&Fanxi_Step==0&&Standby_Time_Delay==0)	// �ڴ���ģʽ�±궨����
	{
		ele_current_zero=ele_current_zero-ele_current_zero/VALUE_FILTER_NUM+ele_current_sum/VALUE_FILTER_NUM;// ���±궨����
	}
	if(ele_current_sum<ele_current_zero) ele_current_sum=ele_current_zero-ele_current_sum;
	else ele_current_sum=0;
	res=ele_current_sum*(3.3/4095.0);
	res/=0.185;
	ele_current=ele_current-ele_current/VALUE_FILTER_NUM+res/VALUE_FILTER_NUM;
	ELE_Current=ele_current;
	// ����ϵͳ��ѹ
	res=(sys_value_sum*(3.3/4095.0))/(1.2/11.2);
	sys_value=sys_value-sys_value/VALUE_FILTER_NUM+res/VALUE_FILTER_NUM;
	SYS_Value=sys_value;
}

/***********************************************************************************
 * ��������void Test_Water_Flow(void)
 * ����  ������ˮ��
 * ����  ����
 * ���  ����
************************************************************************************/
void Test_Water_Flow(void)		
{
	static uint8_t last_flage;
	double hz;
	
	// ͳ���ۼ���ˮ��
	if(last_flage!=LLJ_Start_Flage)						// ����һ�������ƴ�״̬
	{
		last_flage=LLJ_Start_Flage;							// ����״̬
		if(LLJ_Start_Flage!=0)									// ����������
		{
			Last_Float_Flow=0;										// ����ϴ�����
			Fanxi_Delay_Time=FANXI_DELAY_TIME;		// ���û�ʹ�ú�һ��ʱ���ſ�ʼ��ϴ
		}
	}
	// ����������Ƿ���
	if(LLJ_Start_Flage==0) 										// �����ƹر�
	{
		Flow_Now_Value=0;												// ˮ����
		Flow_Now_Fre=0;													// Ƶ��Ϊ0
		return;
	}
	if(LLJ_Now_Value==0) return ;							// ����
	// �������ٶ�Ӧ��Ƶ��
	hz=1.0/((double)LLJ_Now_Value/100000.0);	// �������ǰƵ��
	// ���㵱ǰ����
	Flow_Now_Fre=hz;													// ��ǰˮ��Ƶ��
	Flow_Now_Value=hz/36.0;										// ��ǰˮ����
	// ��ǰ��ˮ��
	Last_Float_Flow+=Flow_Now_Value/60.0;			// ��ǰ��ˮ��
	// �ۼ���ˮ��
	Toatl_Float_Flow+=Flow_Now_Value/60.0;		// ��ǰ��ˮ��
	// �ۼƷ�ϴˮ��
	Fanxi_Float_Flow+=Flow_Now_Value/60.0;		// �ۼƷ�ϴˮ��
}

/***********************************************************************************
 * ��������void Back_Wash_Oper(void)
 * ����  ������ϴ
 * ����  ����
 * ���  : ��ȡ�����ݷ���1 δ��ȡ�����򷵻�0
 * ����  ���ⲿ���� AA 0A 06 84 05 12 34 56 78 85 BB
**********************************************************************************/
void Back_Wash_Oper(void)
{
	uint8_t strength=3;
	
	// ��ֵ����TDS����
	if(Fanxi_Float_Flow>Auto_Fanxi_Value&&Queshi_Flage==0)	// ��ȱˮ״̬�²��ܷ�ϴ
	{
		Fanxi_Float_Flow=0;
		Auto_Fanxi_Flage=1;
	}
	// ���TDS
	switch(Fanxi_Step)
	{
		case 0: // �ȴ���ϴģʽ
						if(Auto_Fanxi_Flage==1&&Fanxi_Delay_Time==0)	// ��ϴ��־��λ
						{
							Fanxi_Continue_Time=FANXI_CON_TIME;		// ��ϴ����3����
							Fanxi_Mode=1;													// ��ϴ��־
							Auto_Fanxi_Flage=0;										// 
							Fanxi_Flage=0;												// ��λ�ñ�־
							Fanxi_Step=1;													// ������ϴģʽ
							ST_VA_SET(1);													// ����ͨ��
							JS_VA_SET(1);													// ���ý�ˮ����	
						}	
						if(Fanxi_Flage==1)											// �ֶ���ϴ
						{
							Fanxi_Continue_Time=FANXI_CON_TIME;		// ��ϴ����3����
							Fanxi_Mode=1;													// ��ϴ��־
							Auto_Fanxi_Flage=0;										// 
							Fanxi_Flage=0;												// ��λ�ñ�־
							Fanxi_Step=1;													// ������ϴģʽ		
							ST_VA_SET(1);													// ����ͨ��
							JS_VA_SET(1);													// ���ý�ˮ����	
						}
						break;
		case 1:	// �򿪵�ŷ�
						if(Probe_PP&&Probe_C1&&Probe_UDF&&Probe_C2)		// ��о����
						{			
							ELE_CONA_SET(0);											// ��ϴ������
							ELE_CONB_SET(1);											// ��ϴ������	�̵�����������
							delay_ms(50);													// ��ʱ
							LED_DJ=1;															// ���ָʾ�Ƶ���		
						}
						LED_CX=1;																// ��ϴ					
						SS_VA_SET(1);														// ����ˮ��
						Fanxi_Step=2;														// ������ˮģʽ
						break;
		case 2:	// ��ʼ���
						HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); 		// ��PWM
						if(Probe_PP&&Probe_C1&&Probe_UDF&&Probe_C2)		// ��о����
						{
							Start_Cur_PID(strength);							// ��������ѹ							
						}
						Fanxi_Step=3;														// ������ˮģʽ	
						break;
		case 3:	// ��ϴ�еȴ��û��ر�
						if(LLJ_Start_Flage==1)									// ˮ����תֹͣ��ϴ
						{
							Stop_Cur_PID();												// ������ѹ����		
							HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
							delay_ms(50);													// ��ʱ
							Chongxi_Continue_Time=CHONGXI_CON_TIME;	// ��ϴ����ʱ��2����
							Fanxi_Step=4;													// ֹͣ��ˮ
						}
						if(Fanxi_Continue_Time==0)							// ��ϴ����
						{
							Stop_Cur_PID();												// ������ѹ����		
							HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
							delay_ms(50);													// ��ʱ
							Chongxi_Continue_Time=CHONGXI_CON_TIME;	// ��ϴ����ʱ��2����
							Fanxi_Step=4;													// ֹͣ��ˮ
						}
						if(Fanxi_Mode==0)												// �û��ֶ��رշ�ϴ
						{
							Stop_Cur_PID();												// ������ѹ����	
							HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);							
							delay_ms(50);													// ��ʱ
							Chongxi_Continue_Time=CHONGXI_CON_TIME;	// ��ϴ����ʱ��2����
							Fanxi_Step=4;													// ֹͣ��ˮ						
						}
						break;
		case 4:	// ��ϴ��ˮ
						ELE_CONA_SET(0);												// ֹͣ���
						ELE_CONB_SET(0);												// ֹͣ���
						LED_DJ=0;																// ���ָʾ��Ϩ��
						if(LLJ_Start_Flage==1)									// ˮ����תֹͣ��ϴ
						{					
							ST_VA_SET(0);													// �ر���ͨ��
							Fanxi_Step=5;													// ֹͣ��ˮ
						}
						if(Chongxi_Continue_Time==0)						// ��ϴ����
						{					
							ST_VA_SET(0);													// �ر���ͨ��
							delay_ms(50);													// ��ʱ
							Fanxi_Step=5;													// ֹͣ��ˮ
						}	
						if(Fanxi_Mode==0)												// �û��ֶ��رշ�ϴ
						{
							ST_VA_SET(0);													// �ر���ͨ��
							delay_ms(50);													// ��ʱ
							Fanxi_Step=5;													// ֹͣ��ˮ
						}
						break;
		case 5:	// ֹͣ��ϴ
						Fanxi_Num++;Fanxi_Float_Flow=0;					// ��ϴ��������
						HAL_FLASH_Unlock();															// Flash ����
						EE_WriteVariable(VirtAddVarTab[2],Fanxi_Num);		// ��ȡ�ۼƷ�ϴ����
						HAL_FLASH_Lock();																// Flash ����
						ELE_CONA_SET(0);												// ֹͣ���
						ELE_CONB_SET(0);												// �����ȡ��ˮ									
						SS_VA_SET(0);														// �ر���ˮ��		
						ST_VA_SET(0);														// �ر���ͨ��
						Fanxi_Mode=0;														// ֹͣ��ϴ	
						LED_ZS=0;																// ��ˮָʾ�Ƶ���
						LED_DJ=0;																// ���ָʾ�Ƶ���
						LED_CX=0;																// ��ϴ
						Fanxi_Step=0;														// 
						Fanxi_Mode=0;														//
						break;
		default : // 
						break;
	}	
}


void Read_System_Info(void)
{
	uint16_t state=0,flow=0;
	
	EE_ReadVariable(VirtAddVarTab[0], &state);					// ��ȡ�豸����
	if(state==0xa5a5)
	{
		EE_ReadVariable(VirtAddVarTab[1], &state);				// ��ȡ�豸����
		if(state>3) state=1;ELE_Strength=state;						// Ĭ������
		EE_ReadVariable(VirtAddVarTab[2], &flow);					// ��ȡ�ۼƷ�ϴ����
		Fanxi_Num=flow;																		// ��ϴ����
		EE_ReadVariable(VirtAddVarTab[3], &flow);					// ��ȡ�ۼ�ˮ��
		Toatl_Float_Flow=flow;														// �ۼ�ˮ��
	}
	else
	{
		ELE_Strength=1;Fanxi_Num=0;												//
		EE_WriteVariable(VirtAddVarTab[0],0xa5a5);				// ������ǿ��
		EE_WriteVariable(VirtAddVarTab[1],ELE_Strength); 	// ������ǿ��
		EE_WriteVariable(VirtAddVarTab[2],Fanxi_Num);			// ��ȡ�ۼƷ�ϴ����
		EE_WriteVariable(VirtAddVarTab[3],0);							// ����ۼ�ˮ��
	}
}




// end
