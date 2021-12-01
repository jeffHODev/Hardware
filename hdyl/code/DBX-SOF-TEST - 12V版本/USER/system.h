#ifndef __SYSTEM_H_
#define __SYSTEM_H_
#include "stm32f10x.h"

#define Ntc_Num 126													// 温度传感器测温范围
#define AVER_SAMPLE_NUM 100									// 平均采样
#define INPUT_FILTER_TIME 10								// 按键滤波时间50ms, 单位10ms 	只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
#define INPUT_LONG_TIME   100								// 持续1000ms，认为长按事件 
#define TEMP_DIFF 5													// 温度偏差

#define DRPOWER_DIFFD 11										// 功率偏差下限
#define DRPOWER_DIFFU 11											// 功率偏差上限
#define DRSTAND_POWER 110										// 标准功率

#define SRPOWER_DIFFD 11										// 功率偏差下限
#define SRPOWER_DIFFU 11											// 功率偏差上限
#define SRSTAND_POWER 120										// 标准功率

#define VDD_VALUE 5.0												// 电源电压
#define UP_VALUE  2.5												// 上电无负载值
#define DOW_VALUE 2.5												// 下垫无负载值

extern __IO uint8_t Work_Mode;							// 工作模式
extern __IO uint8_t Chuang_Mode;						// 床垫类别 单人 双人
extern __IO float UP_Temp;									// 上垫温度
extern __IO float Down_Temp;								// 下垫温度
extern __IO float Now_Temp;									// 当前温度
extern __IO float IN_Value;									// 输入电压
extern __IO float UP_Power;									// 上垫功率
extern __IO float Down_Power;								// 下垫功率

uint8_t SW_Posmode_Read(void);
void Postion_GPIO_Config(void);
void Get_Now_Temp(void);
void Get_Now_Value(void);

#endif

/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/
