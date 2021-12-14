/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************
 * �ļ���  ��system.c
 * ��    ����MODBUS����
 * ʵ��ƽ̨��STM32ͨѶת��
 * Ӳ�����ӣ���
 * ��汾  ��ST3.0.0
 * ����    ��С��
 * ��Ŀ˵����MODBUS
**********************************************************************************/
#include "pid.h"
//#include "driveboard.h"
/*****************************��������*******************************************/
static uint8_t Cur_Pid_Enable=0;							// PIDʹ�ܵ���
static uint8_t Motor_Pid_Enable=0;						// PIDʹ���¶�
static uint16_t Set_Current=0;								// ���õ���
static uint16_t Set_Tempvalue=0;							// �����¶�

static float Cur_Pid_Kp=0;										// ϵ�� 0.05
static float Cur_Pid_Ki=0.16;									// ϵ�� 0.36
static float Cur_Pid_Kd=0;										// ϵ�� 0

static float Tem_Pid_Kp=35;										// ϵ�� 20 
static float Tem_Pid_Ki=0.33;									// ϵ�� 0.33 
static float Tem_Pid_Kd=0;										// ϵ�� 0

static float Cur_Pid_diff[3]= {0,0,0};   			// �������ֵ
static float Motor_Pid_diff[3]= {0,0,0};   		// �¶����ֵ
static float motor_pid=0;

static int16_t Cur_Pidvalue=0;								// ����PID����
static int16_t Motor_Pidvalue2L=0;						// �¶�PID����

// �״ν���PID��ʼֵ����ˮ���¹��� ���� 45���϶� 65���϶� 80���϶�   
static uint16_t Set_HOTPwm_Value[]={3500,2500,2100,1800};			// ����ģʽ�¶�Ӧ�ĵ��PWMֵ�����õ����ʼֵ�� (3500 3000 2500  1800) 
const uint16_t Set_HMtemp_Value[] ={   0,  65,  80,  90};			// ���ò�ͬ�¶ȶ�Ӧ��Ĥ�¶�(�����¶�  45 65 80 ��)
const uint16_t Set_PHpwm_Value[]  ={4000,3500,3250, 500};			// ���õ��PWM��ʼֵ (���õ��PWM��ֵ)
const uint16_t Set_Current_Table[]={   0, 500,1000,3000};			// PHֵ��Ӧ�ĵ���ֵ(�趨������ֵmA)

// PH7.0 	Y:F = 
// PH8.0 	Y:F =
// PH9.0 	Y:F =
// PH10.0 Y:F =
void Stop_Cur_PID(void)										// ֹͣ�������
{
  Cur_Pid_Enable=0;
	Cur_Pid_diff[0]=0;
	Cur_Pid_diff[1]=0;
	Cur_Pid_diff[2]=0;
}

// ���ڵ�����  �������PHֵ
void Start_Cur_PID(uint8_t temp)					// �����������
{
	Set_Current=Set_Current_Table[temp];
	Cur_Pidvalue=Set_PHpwm_Value[temp];
	Cur_Pid_diff[0]=0;
	Cur_Pid_diff[1]=0;
	Cur_Pid_diff[2]=0;
	Cur_Pid_Enable=1;
}

// ���ڳ�ˮ�¶� 
void Start_Motor_PID(uint8_t temp)				// �������ˮ��
{
//	// ���ݲ�ͬˮ�½��е���
//	switch(temp)														// ���ݲ�ͬˮ�½��е���
//	{
//		case 0:	// ����ˮ
//						Motor_Pid_Enable=2;						// �̶�ˮ��
//						if(Set_PH_Num==0) Set_HOTPwm_Value[0]=3500;		// PH=--
//						if(Set_PH_Num==1) Set_HOTPwm_Value[0]=2500;		// PH=8.0
//						if(Set_PH_Num==2) Set_HOTPwm_Value[0]=2000;		// PH=9.0
//						if(Set_PH_Num==3) Set_HOTPwm_Value[0]=1800;		// PH=10.0
//						break;
//		case 1:	//�ʿ�
//						Motor_Pid_Enable=1;						// PWM����ˮ��
////						Set_HOTPwm_Value[1]=3000;			// 45
//						break;
//		case 2:	// ����
//						Motor_Pid_Enable=1;						// PWM����ˮ��
////						Set_HOTPwm_Value[2]=2500;			// 65
//						break;
//		case 3:	// ����
//						Motor_Pid_Enable=1;						// PWM����ˮ��
////						Set_HOTPwm_Value[3]=1400;			// 80
//						break;
//		default :	temp=0;
//							break;
//	}
//	Set_Tempvalue=Set_HMtemp_Value[temp];		// ��ȡ�����¶�
//	Motor_Pidvalue2L=Set_HOTPwm_Value[temp];// ��ȡPWM��ֵ
//	Motor_Pid_diff[0]=0;										//
//	Motor_Pid_diff[1]=0;										//
//	Motor_Pid_diff[2]=0;										//
}


// ��λ����PID
void Cur_PID_Reset(void)
{
	Cur_Pidvalue=0;
	Cur_Pid_Enable=0;
	Cur_Pid_diff[0]=0;
	Cur_Pid_diff[1]=0;
	Cur_Pid_diff[2]=0;
	//Cur_OUTPUT=4000;
}

// ��λ���PID
void Motor_PID_Reset(void)								// ��λ���
{
	Motor_Pid_Enable=0;
	Motor_Pidvalue2L=0;
	Motor_Pid_diff[0]=0;
	Motor_Pid_diff[1]=0;
	Motor_Pid_diff[2]=0;
	//PUM2L_OUTPUT=0;
	//PUM1L_OUTPUT=0;
}

// ��ԭˮˮ�¸���38��ʱ������ˮ�¶�
void Reset_Motor_Value(void)						
{
	Motor_Pidvalue2L=3000;
}

void Cur_PID_Run(void)																				// PID���г���
{
	if(Cur_Pid_Enable==1)
	{
		// ���㵱ǰ���
	//	Cur_Pid_diff[0]=Now_Elecr_Current-Set_Current;						// �����ֵ
		// �޶���Χ  
		Cur_Pid_diff[0] = Cur_Pid_diff[0] <= 100  ? Cur_Pid_diff[0] :  100;
		Cur_Pid_diff[0] = Cur_Pid_diff[0] >= -100 ? Cur_Pid_diff[0] : -100;
		// ��������
		Cur_Pidvalue+=Cur_Pid_Kp*(Cur_Pid_diff[0]-Cur_Pid_diff[1])+Cur_Pid_Ki*Cur_Pid_diff[0]+Cur_Pid_Kd*((Cur_Pid_diff[0]-Cur_Pid_diff[1])-(Cur_Pid_diff[1]-Cur_Pid_diff[2]));
		// �����ֵ
		Cur_Pid_diff[2]=Cur_Pid_diff[1];													// �������ֵ
		Cur_Pid_diff[1]=Cur_Pid_diff[0];													// �������ֵ
		// �޶���Χ      
		Cur_Pidvalue = Cur_Pidvalue <= 4000 ? Cur_Pidvalue : 4000;
		Cur_Pidvalue = Cur_Pidvalue >=    0 ? Cur_Pidvalue :    0;
		//Cur_OUTPUT=Cur_Pidvalue;
	}
	else
	{
		//Cur_OUTPUT=4000;
	}
}


void Motor_PID_Run(void)																			  // ÿ100msִ��һ��
{
	if(Motor_Pid_Enable>0)																				// ʹ��PID
	{
		// ���㵱ǰ���
		switch(Motor_Pid_Enable)
		{
			case 1: 	// �������
							//	Motor_Pid_diff[0]=Hmwater_Temp-Set_Tempvalue;		// �����ֵ 
								break;
			case 2:		// ���������
								Motor_Pid_diff[0]=0;														// �������ֵ
								Motor_Pid_diff[1]=0;														// �������ֵ
								Motor_Pid_diff[2]=0;														// �������ֵ
								break;
			default : // �쳣
								break;
		}
		// �޶���Χ  
		Motor_Pid_diff[0] = Motor_Pid_diff[0] <= 200  ? Motor_Pid_diff[0] :  200;
		Motor_Pid_diff[0] = Motor_Pid_diff[0] >= -10 ? Motor_Pid_diff[0]  :  -10;
		motor_pid=Motor_Pid_diff[0];
		motor_pid=motor_pid;
		// ��������
		Motor_Pidvalue2L+=Tem_Pid_Kp*(Motor_Pid_diff[0]-Motor_Pid_diff[1])+Tem_Pid_Ki*Motor_Pid_diff[0]+Tem_Pid_Kd*((Motor_Pid_diff[0]-Motor_Pid_diff[1])-(Motor_Pid_diff[1]-Motor_Pid_diff[2]));
		// �����ֵ
		Motor_Pid_diff[2]=Motor_Pid_diff[1];											// �������ֵ
		Motor_Pid_diff[1]=Motor_Pid_diff[0];											// �������ֵ
		// �޶���Χ      
	//	Motor_Pidvalue2L = Motor_Pidvalue2L <= 4000 ? Motor_Pidvalue2L : 4000;
//		Motor_Pidvalue2L = Motor_Pidvalue2L >= PUM_MINVALUE2L ? Motor_Pidvalue2L : PUM_MINVALUE2L;	
		// ��������ֵ
		//PUM2L_OUTPUT = Motor_Pidvalue2L;													// ����
	}
	else
	{
	//	PUM2L_OUTPUT=0;																						// �ر����
		//PUM1L_OUTPUT=0;																						// �ر����
	}
}


/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/
