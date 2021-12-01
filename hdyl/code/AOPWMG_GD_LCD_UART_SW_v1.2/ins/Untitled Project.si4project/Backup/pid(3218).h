#ifndef PID_H
#define PID_H

#define MAX_OUTPUT  45000
#define MIN_OUTPUT  0
#include<main.h>
typedef struct PID {
int32_t SetPoint; // �趨Ŀ��Desired value
double Proportion; // ��������Proportional Const
double Integral; // ���ֳ���Integral Const
double Derivative; // ΢�ֳ���Derivative Const
int32_t LastError; // Error[-1]
int32_t result;
int32_t  iError;	//��ǰ���
    int32_t Current_Error;//��ǰ���
    int32_t Last_Error;//��һ�����
    int32_t Previous_Error;//���ϴ����
int32_t PrevError; // Error[-2]
int32_t SumError; // Sums of Errors
	int32_t	Increase;	//���ó���ʵ������
} PID;
uint32_t pid_proc(double rIn);
void pid_init(float setvalue);
void PIDInit (PID *pp);
void pid_init_flow(float setvalue);


#endif

