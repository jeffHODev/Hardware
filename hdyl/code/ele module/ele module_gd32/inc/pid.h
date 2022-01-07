#ifndef PID_H
#define PID_H

#define MAX_OUTPUT  33000  //最大pwm输出量
#define MIN_OUTPUT  0
#define MAX_I 30000
#include<main.h>
typedef struct PID {
double SetPoint; // 设定目标Desired value
double Proportion; // 比例常数Proportional Const
double Integral; // 积分常数Integral Const
double Derivative; // 微分常数Derivative Const
double LastError; // Error[-1]
double result;
double PrevError; // Error[-2]
double SumError; // Sums of Errors
} PID;
uint32_t pid_proc(double rIn);
void pid_init(float setvalue);
void PIDInit (PID *pp);


#endif

