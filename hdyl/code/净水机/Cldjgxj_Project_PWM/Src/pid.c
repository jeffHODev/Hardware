#include "pid.h"
#include "system.h"
#include "adc.h"
#include "spi.h"

/*****************************��������*******************************************/
static uint8_t Cur_Pid_Enable=0;							// PIDʹ�ܵ���
static float Set_Current=0;										// ���õ���
static float Cur_Pid_diff[3]= {0,0,0};   			// �������ֵ

static float Cur_Pid_Kp=25.5;																	// ϵ�� 25.5
static float Cur_Pid_Ki=0.56;																	// ϵ�� 0.56
static float Cur_Pid_Kd=0;																		// ϵ�� 0
static float Cur_Pidvalue;																		// Ԥ��ֵ

const float Set_Current_Table[10][4]={												// ���ֵ��Ӧ�ĵ���ֵ(�趨������ֵA)
																			{  0,0.5,1.0,4.0},			// TDS 000-200			
																			{  0,0.6,1.0,4.0},			// TDS 200-300	
																			{  0,0.8,2.0,4.0},			// TDS 300-400	
																			{  0,1.0,2.0,4.0},			// TDS 400-500	
																			{  0,1.0,2.0,4.0},			// TDS 500-600	
																			{  0,1.0,2.0,4.0},			// TDS 600-700	
																			{  0,1.0,2.0,4.0},			// TDS 700-800	
																			{  0,1.0,2.0,4.0},			// TDS 800-900	
																			{  0,1.0,2.0,4.0},			// TDS 900-1000
																			{  0,1.0,2.0,4.0},			// TDS 1000����																			
																		};
const uint8_t Set_Init_Value[10][4]={													// ��ˮ �� �� ��
																			{100, 40, 20, 15},			// TDS 000-200
																			{100, 50, 25, 15},			// TDS 200-300
																			{100, 55, 30, 15},			// TDS 300-400
																			{100, 80, 30, 15},			// TDS 400-500
																			{100,100, 35, 18},			// TDS 500-600
																			{100,240, 55, 33},			// TDS 600-700																			
																			{100,245, 70, 30},			// TDS 700-800																			
																			{100,250, 80, 35},			// TDS 800-900		
																			{100,250, 98, 38},			// TDS 900-1000																			
																			{100,250,100, 40},			// TDS 1000����	
																		};												// ͨ��TDS�趨����ʼֵ

// PH7.0 	Y:F = 
// PH8.0 	Y:F =
// PH9.0 	Y:F =
// PH10.0 Y:F =
void Stop_Cur_PID(void)																				// ֹͣ�������
{
  Cur_Pid_Enable=0;
	Cur_Pid_diff[0]=0;
	Cur_Pid_diff[1]=0;
	Cur_Pid_diff[2]=0;
	Cur_Pidvalue=100;
	TLP0501_Value=Cur_Pidvalue;
	Set_ELE_Value(100);																					// ����ѹ�����ڵ����
}

// ��λ����PID
void Cur_PID_Reset(void)																			// ��λ����ֵ
{
	Cur_Pidvalue=100;
	Cur_Pid_Enable=0;
	Cur_Pid_diff[0]=0;
	Cur_Pid_diff[1]=0;
	Cur_Pid_diff[2]=0;
	Set_ELE_Value(100);																					// ����ѹ�����ڵ����
}

// ���ڵ�����  �������PHֵ
void Start_Cur_PID(uint8_t temp)															// �����������
{
	// ��ʼ��
	Cur_Pid_diff[0]=0;
	Cur_Pid_diff[1]=0;
	Cur_Pid_diff[2]=0;
	Cur_Pid_Enable=1;
	// TL0510Ԥ��ֵ Ԥ�����
	if(TDS_Value<200)
	{
		Cur_Pidvalue=Set_Init_Value[0][temp];
		Set_Current=Set_Current_Table[0][temp];
		Auto_Fanxi_Value=Fanxi_Value_Table[0];										// 500Lˮ��ϴһ��
	}
	else if(TDS_Value<300)
	{
		Cur_Pidvalue=Set_Init_Value[1][temp];
		Set_Current=Set_Current_Table[1][temp];
		Auto_Fanxi_Value=Fanxi_Value_Table[0];										// 500Lˮ��ϴһ��
	}
	else if(TDS_Value<400)
	{
		Cur_Pidvalue=Set_Init_Value[2][temp];
		Set_Current=Set_Current_Table[2][temp];
		Auto_Fanxi_Value=Fanxi_Value_Table[1];										// 400Lˮ��ϴһ��
	}
	else if(TDS_Value<500)
	{
		Cur_Pidvalue=Set_Init_Value[3][temp];
		Set_Current=Set_Current_Table[3][temp];
		Auto_Fanxi_Value=Fanxi_Value_Table[2];										// 300Lˮ��ϴһ��
	}
	else if(TDS_Value<600)
	{
		Cur_Pidvalue=Set_Init_Value[4][temp];
		Set_Current=Set_Current_Table[4][temp];
		Auto_Fanxi_Value=Fanxi_Value_Table[3];										// 200Lˮ��ϴһ��
	}
	else if(TDS_Value<700)
	{
		Cur_Pidvalue=Set_Init_Value[5][temp];
		Set_Current=Set_Current_Table[5][temp];
		Auto_Fanxi_Value=Fanxi_Value_Table[4];										// 100Lˮ��ϴһ��
	}
	else if(TDS_Value<800)
	{
		Cur_Pidvalue=Set_Init_Value[6][temp];
		Set_Current=Set_Current_Table[6][temp];
		Auto_Fanxi_Value=Fanxi_Value_Table[4];										// 100Lˮ��ϴһ��
	}
	else if(TDS_Value<900)
	{
		Cur_Pidvalue=Set_Init_Value[7][temp];
		Set_Current=Set_Current_Table[7][temp];
		Auto_Fanxi_Value=Fanxi_Value_Table[4];										// 100Lˮ��ϴһ��
	}
	else 
	{
		Cur_Pidvalue=Set_Init_Value[8][temp];
		Set_Current=Set_Current_Table[8][temp];
		Auto_Fanxi_Value=Fanxi_Value_Table[4];										// 100Lˮ��ϴһ��
	}
}

void Cur_PID_Run(void)																				// PID���г���
{
	uint8_t temp;
	
	if(Cur_Pid_Enable==1)																				// ����PID
	{
		// ���㵱ǰ���
		Cur_Pid_diff[0]=ELE_Current-Set_Current;									// �����ֵ
		// �޶���Χ  
		Cur_Pid_diff[0] = Cur_Pid_diff[0] <= 0.1  ? Cur_Pid_diff[0] :  0.1;
		Cur_Pid_diff[0] = Cur_Pid_diff[0] >= -0.1 ? Cur_Pid_diff[0] : -0.1;
		// ��������
		Cur_Pidvalue+=Cur_Pid_Kp*(Cur_Pid_diff[0]-Cur_Pid_diff[1])+Cur_Pid_Ki*Cur_Pid_diff[0]+Cur_Pid_Kd*((Cur_Pid_diff[0]-Cur_Pid_diff[1])-(Cur_Pid_diff[1]-Cur_Pid_diff[2]));
		// �����ֵ
		Cur_Pid_diff[2]=Cur_Pid_diff[1];													// �������ֵ
		Cur_Pid_diff[1]=Cur_Pid_diff[0];													// �������ֵ
		// �޶���Χ      
		Cur_Pidvalue = Cur_Pidvalue <= 255 ? Cur_Pidvalue : 255;	// ���ֵΪ255
		Cur_Pidvalue = Cur_Pidvalue >=   1 ? Cur_Pidvalue :   1;	// ��СֵΪ1
		// ���ڵ��ӵ�ѹ��
		temp=(uint8_t)Cur_Pidvalue;																//
		Set_ELE_Value(temp);																			//
		TLP0501_Value=temp;
	}
	else
	{
		Set_ELE_Value(250);																				// ����ѹ�����ڵ����
	}
}




