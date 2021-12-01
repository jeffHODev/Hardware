#include "pid.h"
#include<string.h>
#include<stdio.h>
PID sPID; // PID Control Structure
double rOut; // PID Response (Output)
double rIn_PID; // PID Feedback (Input)


/*====================================================================================================
PID���㺯��
=====================================================================================================*/
double PIDCalc( PID *pp, double NextPoint )
{
    double dError, Error,result,res_tmp;
	static double last_result;
	
  Error = pp->SetPoint - NextPoint; // ƫ��
//	result = pp->Proportion*(Error-pp->LastError)    //��������
//	+pp->Integral*Error
//	+pp->Derivative*(Error-2*pp->LastError+pp->PrevError);

//	pp->PrevError=pp->LastError; //�洢�������´μ���
//	pp->LastError=Error;	
	
    pp->SumError += Error; // ����
    dError = pp->LastError - pp->PrevError; // ��ǰ΢��
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
	result =(pp->Proportion * Error // ������
    + pp->Integral * pp->SumError // ������
    + pp->Derivative * dError );

   
	/* if(result>last_result)
	 {
		 res_tmp = result-last_result;
		 if((res_tmp>0&&res_tmp>=10000))
		{
				last_result=last_result+500;
			result = last_result;		
			}	 
	 }
	 else
	 {
	   res_tmp = last_result-result;
		 if((res_tmp>=10000))
		{
				last_result=last_result-500;
			  result = last_result;		
			}
	 }*/
	
   //result = result + 10000;
  
	if(result <=0)
		result = 0;
	
	if(result>=MAX_OUTPUT)
		result = MAX_OUTPUT;
	//result = 65530;
//	result = 8000;
	pp->result = result;
    return result;        // ΢����
}

/*====================================================================================================
PID�ṹ�������ʼ������
=====================================================================================================*/
void PIDInit (PID *pp)
{
memset ( pp,0,sizeof(PID));
}

/*====================================================================================================
��ȡ��������������ڴ��趨Ϊ�̶�ֵ100��
======================================================================================================*/


/*====================================================================================================
����������ƺ���
======================================================================================================*/
void actuator(double rDelta)  
{
}
void pid_init(float setvalue)
{


    PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion =260; // Set PID Coefficients  130
    sPID.Integral = 0.0003;
    sPID.Derivative = 0.00001;
    sPID.SetPoint = setvalue; // Set PID Setpoint
    sPID.LastError = 0;
	sPID.PrevError = 0;
	sPID.SumError = 0;
}

//������
uint32_t pid_proc(double rIn)
{
        rIn_PID = rIn;                // ��ȡ�����������(Read Input )
        rOut = PIDCalc ( &sPID,rIn_PID );   // PID���㺯��(Perform PID Interation)
        return (uint32_t)( rOut );              // ����������ƺ���(Effect Needed Changes)
}

