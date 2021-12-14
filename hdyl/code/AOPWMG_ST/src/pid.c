/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************
 * 文件名  ：system.c
 * 描    述：MODBUS开发
 * 实验平台：STM32通讯转换
 * 硬件连接：无
 * 库版本  ：ST3.0.0
 * 作者    ：小奕
 * 项目说明：MODBUS
**********************************************************************************/
#include "pid.h"
//#include "driveboard.h"
/*****************************变量声明*******************************************/
static uint8_t Cur_Pid_Enable=0;							// PID使能电流
static uint8_t Motor_Pid_Enable=0;						// PID使能温度
static uint16_t Set_Current=0;								// 设置电流
static uint16_t Set_Tempvalue=0;							// 设置温度

static float Cur_Pid_Kp=0;										// 系数 0.05
static float Cur_Pid_Ki=0.16;									// 系数 0.36
static float Cur_Pid_Kd=0;										// 系数 0

static float Tem_Pid_Kp=35;										// 系数 20 
static float Tem_Pid_Ki=0.33;									// 系数 0.33 
static float Tem_Pid_Kd=0;										// 系数 0

static float Cur_Pid_diff[3]= {0,0,0};   			// 电流误差值
static float Motor_Pid_diff[3]= {0,0,0};   		// 温度误差值
static float motor_pid=0;

static int16_t Cur_Pidvalue=0;								// 电流PID参数
static int16_t Motor_Pidvalue2L=0;						// 温度PID参数

// 首次进入PID初始值否则水升温过慢 常温 45摄氏度 65摄氏度 80摄氏度   
static uint16_t Set_HOTPwm_Value[]={3500,2500,2100,1800};			// 加热模式下对应的电机PWM值（设置电机初始值） (3500 3000 2500  1800) 
const uint16_t Set_HMtemp_Value[] ={   0,  65,  80,  90};			// 设置不同温度对应厚膜温度(设置温度  45 65 80 ℃)
const uint16_t Set_PHpwm_Value[]  ={4000,3500,3250, 500};			// 设置电解PWM初始值 (设置电解PWM初值)
const uint16_t Set_Current_Table[]={   0, 500,1000,3000};			// PH值对应的电流值(设定电解电流值mA)

// PH7.0 	Y:F = 
// PH8.0 	Y:F =
// PH9.0 	Y:F =
// PH10.0 Y:F =
void Stop_Cur_PID(void)										// 停止电流输出
{
  Cur_Pid_Enable=0;
	Cur_Pid_diff[0]=0;
	Cur_Pid_diff[1]=0;
	Cur_Pid_diff[2]=0;
}

// 调节电解电流  调节输出PH值
void Start_Cur_PID(uint8_t temp)					// 调节输出电流
{
	Set_Current=Set_Current_Table[temp];
	Cur_Pidvalue=Set_PHpwm_Value[temp];
	Cur_Pid_diff[0]=0;
	Cur_Pid_diff[1]=0;
	Cur_Pid_diff[2]=0;
	Cur_Pid_Enable=1;
}

// 调节出水温度 
void Start_Motor_PID(uint8_t temp)				// 调节输出水温
{
//	// 根据不同水温进行调节
//	switch(temp)														// 根据不同水温进行调节
//	{
//		case 0:	// 常温水
//						Motor_Pid_Enable=2;						// 固定水速
//						if(Set_PH_Num==0) Set_HOTPwm_Value[0]=3500;		// PH=--
//						if(Set_PH_Num==1) Set_HOTPwm_Value[0]=2500;		// PH=8.0
//						if(Set_PH_Num==2) Set_HOTPwm_Value[0]=2000;		// PH=9.0
//						if(Set_PH_Num==3) Set_HOTPwm_Value[0]=1800;		// PH=10.0
//						break;
//		case 1:	//适口
//						Motor_Pid_Enable=1;						// PWM调节水速
////						Set_HOTPwm_Value[1]=3000;			// 45
//						break;
//		case 2:	// 中温
//						Motor_Pid_Enable=1;						// PWM调节水速
////						Set_HOTPwm_Value[2]=2500;			// 65
//						break;
//		case 3:	// 高温
//						Motor_Pid_Enable=1;						// PWM调节水速
////						Set_HOTPwm_Value[3]=1400;			// 80
//						break;
//		default :	temp=0;
//							break;
//	}
//	Set_Tempvalue=Set_HMtemp_Value[temp];		// 获取设置温度
//	Motor_Pidvalue2L=Set_HOTPwm_Value[temp];// 获取PWM初值
//	Motor_Pid_diff[0]=0;										//
//	Motor_Pid_diff[1]=0;										//
//	Motor_Pid_diff[2]=0;										//
}


// 复位电流PID
void Cur_PID_Reset(void)
{
	Cur_Pidvalue=0;
	Cur_Pid_Enable=0;
	Cur_Pid_diff[0]=0;
	Cur_Pid_diff[1]=0;
	Cur_Pid_diff[2]=0;
	//Cur_OUTPUT=4000;
}

// 复位电机PID
void Motor_PID_Reset(void)								// 复位电机
{
	Motor_Pid_Enable=0;
	Motor_Pidvalue2L=0;
	Motor_Pid_diff[0]=0;
	Motor_Pid_diff[1]=0;
	Motor_Pid_diff[2]=0;
	//PUM2L_OUTPUT=0;
	//PUM1L_OUTPUT=0;
}

// 当原水水温高于38度时调整出水温度
void Reset_Motor_Value(void)						
{
	Motor_Pidvalue2L=3000;
}

void Cur_PID_Run(void)																				// PID运行程序
{
	if(Cur_Pid_Enable==1)
	{
		// 计算当前误差
	//	Cur_Pid_diff[0]=Now_Elecr_Current-Set_Current;						// 计算差值
		// 限定范围  
		Cur_Pid_diff[0] = Cur_Pid_diff[0] <= 100  ? Cur_Pid_diff[0] :  100;
		Cur_Pid_diff[0] = Cur_Pid_diff[0] >= -100 ? Cur_Pid_diff[0] : -100;
		// 增量计算
		Cur_Pidvalue+=Cur_Pid_Kp*(Cur_Pid_diff[0]-Cur_Pid_diff[1])+Cur_Pid_Ki*Cur_Pid_diff[0]+Cur_Pid_Kd*((Cur_Pid_diff[0]-Cur_Pid_diff[1])-(Cur_Pid_diff[1]-Cur_Pid_diff[2]));
		// 缓存差值
		Cur_Pid_diff[2]=Cur_Pid_diff[1];													// 保存误差值
		Cur_Pid_diff[1]=Cur_Pid_diff[0];													// 保存误差值
		// 限定范围      
		Cur_Pidvalue = Cur_Pidvalue <= 4000 ? Cur_Pidvalue : 4000;
		Cur_Pidvalue = Cur_Pidvalue >=    0 ? Cur_Pidvalue :    0;
		//Cur_OUTPUT=Cur_Pidvalue;
	}
	else
	{
		//Cur_OUTPUT=4000;
	}
}


void Motor_PID_Run(void)																			  // 每100ms执行一次
{
	if(Motor_Pid_Enable>0)																				// 使能PID
	{
		// 计算当前误差
		switch(Motor_Pid_Enable)
		{
			case 1: 	// 电机调速
							//	Motor_Pid_diff[0]=Hmwater_Temp-Set_Tempvalue;		// 计算差值 
								break;
			case 2:		// 电机不调速
								Motor_Pid_diff[0]=0;														// 不计算差值
								Motor_Pid_diff[1]=0;														// 不计算差值
								Motor_Pid_diff[2]=0;														// 不计算差值
								break;
			default : // 异常
								break;
		}
		// 限定范围  
		Motor_Pid_diff[0] = Motor_Pid_diff[0] <= 200  ? Motor_Pid_diff[0] :  200;
		Motor_Pid_diff[0] = Motor_Pid_diff[0] >= -10 ? Motor_Pid_diff[0]  :  -10;
		motor_pid=Motor_Pid_diff[0];
		motor_pid=motor_pid;
		// 增量计算
		Motor_Pidvalue2L+=Tem_Pid_Kp*(Motor_Pid_diff[0]-Motor_Pid_diff[1])+Tem_Pid_Ki*Motor_Pid_diff[0]+Tem_Pid_Kd*((Motor_Pid_diff[0]-Motor_Pid_diff[1])-(Motor_Pid_diff[1]-Motor_Pid_diff[2]));
		// 缓存差值
		Motor_Pid_diff[2]=Motor_Pid_diff[1];											// 保存误差值
		Motor_Pid_diff[1]=Motor_Pid_diff[0];											// 保存误差值
		// 限定范围      
	//	Motor_Pidvalue2L = Motor_Pidvalue2L <= 4000 ? Motor_Pidvalue2L : 4000;
//		Motor_Pidvalue2L = Motor_Pidvalue2L >= PUM_MINVALUE2L ? Motor_Pidvalue2L : PUM_MINVALUE2L;	
		// 驱动器赋值
		//PUM2L_OUTPUT = Motor_Pidvalue2L;													// 调速
	}
	else
	{
	//	PUM2L_OUTPUT=0;																						// 关闭输出
		//PUM1L_OUTPUT=0;																						// 关闭输出
	}
}


/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/
