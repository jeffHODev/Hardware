#ifndef PID_H
#define PID_H

#define MAX_OUTPUT  45000
#define MIN_OUTPUT  0
#include<main.h>
typedef struct PID {
int32_t SetPoint; // 设定目标Desired value
double Proportion; // 比例常数Proportional Const
double Integral; // 积分常数Integral Const
double Derivative; // 微分常数Derivative Const
int32_t LastError; // Error[-1]
int32_t result;
int32_t  iError;	//当前误差
    int32_t Current_Error;//当前误差
    int32_t Last_Error;//上一次误差
    int32_t Previous_Error;//上上次误差
int32_t PrevError; // Error[-2]
int32_t SumError; // Sums of Errors
	int32_t	Increase;	//最后得出的实际增量
} PID;
uint32_t pid_proc(double rIn);
void pid_init(float setvalue);
void PIDInit (PID *pp);
void pid_init_flow(float setvalue);


#endif

