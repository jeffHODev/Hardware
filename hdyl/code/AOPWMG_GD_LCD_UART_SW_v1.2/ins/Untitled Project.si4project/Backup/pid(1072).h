#ifndef PID_H
#define PID_H
#define PID_TYPE  0
#define PID_O_MAX   100

#define PID_I_MAX   100

#define MAX_OUTPUT  55000
#define MIN_OUTPUT  0
#include<main.h>
typedef struct PID {
double SetPoint; // �趨Ŀ��Desired value
double Proportion; // ��������Proportional Const
double Integral; // ���ֳ���Integral Const
double Derivative; // ΢�ֳ���Derivative Const
double LastError; // Error[-1]
double result;
double  iError;	//��ǰ���
    double Current_Error;//��ǰ���
    double Last_Error;//��һ�����
    double Previous_Error;//���ϴ����
double PrevError; // Error[-2]
double SumError; // Sums of Errors
	double	Increase;	//���ó���ʵ������
} PID;
uint32_t pid_proc(double rIn);
void pid_init(float setvalue);
void PIDInit (PID *pp);
void pid_init_flow(float setvalue);


#endif

