#include "pid.h"
#include<string.h>
#include<stdio.h>
#include "sensor.h"
#include<math.h>

PID sPID; // PID Control Structure
double rOut; // PID Response (Output)
double rIn_PID; // PID Feedback (Input)
#define PID_TYPE  0
#define PID_O_MAX   30

#define PID_I_MAX   200
float tmp;
typedef struct
{
    uint32_t Current_Error;//��ǰ���
    uint32_t Last_Error;//��һ�����
    uint32_t Previous_Error;//���ϴ����
} Error_stru;
Error_stru *sptr;
/*!
 *  @brief      ����ʽPID
 *  @since      v1.0
 *  *sptr ��������
 *  *pid:  PID����
 *  NowPlace��ʵ��ֵ
 *  Point��   ����ֵ
 */
// ����ʽPID�������
#if PID_TYPE == 1
double PIDCalc( PID *pp, double NextPoint )
{
    uint32_t dError, Error,result,res_tmp;
    static uint32_t last_result,integ;
    static unsigned char index;
    Error = pp->SetPoint - NextPoint; // ƫ��


//	result = pp->Proportion*(Error-pp->LastError)    //��������
//	+pp->Integral*Error
//	+pp->Derivative*(Error-2*pp->LastError+pp->PrevError);

//	pp->PrevError=pp->LastError; //�洢�������´μ���
//	pp->LastError=Error;

    // pp->SumError += Error; // ����
  

    if(NextPoint>(pp->SetPoint+PID_O_MAX))    //�����һ�����������������ı���
    {
			  tmp = fabs(Error);
        if(tmp>PID_I_MAX)
        {
            index = 0;//ƫ�����ֲ�������
        }
        else
        {
            index = 1;
            if(Error<0)
            {
                pp->SumError += Error;  //������ֻ���ָ�ƫ��
            }
        }
    }
    else if(NextPoint<(pp->SetPoint-PID_O_MAX))  //�����һ������������ָ���ı���
    {
						  tmp = fabs(Error);
        if(tmp>PID_I_MAX)
        {
            index = 0;//ƫ�����ֲ�������
        }
        else
        {
            index = 1;
            if(Error>0)
            {
                pp->SumError += Error;  //������ֻ������ƫ��
            }
        }
    }
    else
    {
			 tmp = fabs(Error);
        if(tmp>PID_I_MAX)     //���ַ����PID�Ż����ɲο�����������
        {
            index = 0;//ƫ�����ֲ�������
        }
        else
        {
            index = 1;
            pp->SumError += Error;
        }
    }
   
    dError = pp->LastError - pp->PrevError; // ��ǰ΢��
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
    result =(pp->Proportion * Error // ������
             +pp->Integral *index* pp->SumError // ������
             + pp->Derivative * dError );

   /* if(result>last_result)
    {
        res_tmp = result-last_result;
        if((res_tmp>0&&res_tmp>=2000))
        {
            last_result=last_result+10;
            result = last_result;
        }
    }
    else
    {
        res_tmp = last_result-result;
        if((res_tmp>=2000))
        {
            last_result=last_result-10;
            result = last_result;
        }
    }*/

    if(result <0)
    {
        result = 0;
        //integ =  0;
    }
  //  else
       // integ =  pp->Integral;

    if(result>=MAX_OUTPUT)
    {
        result = MAX_OUTPUT;
       // integ =  0;
    }
   // else
      //  integ =  pp->Integral;
    pp->result = result;
    return result;        // ΢����
}

#else
/*====================================================================================================
PID���㺯��
=====================================================================================================*/
double PIDCalc( PID *pp, double NextPoint )
{
    double dError, Error,result,res_tmp;
    static double last_result,integ;

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
             +integ * pp->SumError // ������
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

    if(result>last_result)
    {
        res_tmp = result-last_result;
        if((res_tmp>0&&res_tmp>=2000))
        {
            last_result=last_result+10;
            result = last_result;
        }
    }
    else
    {
        res_tmp = last_result-result;
        if((res_tmp>=2000))
        {
            last_result=last_result-10;
            result = last_result;
        }
    }

    if(result <0)
    {
        result = 0;
        integ =  0;
    }
    else
        integ =  pp->Integral;

    if(result>=MAX_OUTPUT)
    {
        result = MAX_OUTPUT;
        integ =  0;
    }
    else
        integ =  pp->Integral;
    pp->result = result;
    return result;        // ΢����
}
#endif
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
void pid_init_flow(float setvalue)
{


    PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion =260; // Set PID Coefficients  260
    sPID.Integral = 0.0003;
    sPID.Derivative = 0.0001;
    sPID.SetPoint = setvalue; // Set PID Setpoint
    sPID.LastError = 0;
    sPID.PrevError = 0;
    sPID.SumError = 0;
    sPID.result = 3000;
}

#if PID_TYPE == 1

void pid_init(float setvalue)
{


    PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion =100; // Set PID Coefficients  260
    sPID.Integral = 0.0003;//
    sPID.Derivative = 0.0001;
    sPID.result = 0;
    sPID.SetPoint = setvalue; // Set PID Setpoint
//   sPID.LastError = 0;
//	sPID.PrevError = 0;
//	sPID.SumError = 0;
}
#else
void pid_init(float setvalue)
{

    PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion =260; // Set PID Coefficients  260
    sPID.Integral = 0.0003;//
    sPID.Derivative = 0.00001;
    sPID.result = 0;
    sPID.SetPoint = setvalue; // Set PID Setpoint
//   sPID.LastError = 0;
//	sPID.PrevError = 0;
//	sPID.SumError = 0;
}
#endif

//������
uint32_t pid_proc(double rIn)
{
    rIn_PID = rIn;                // ��ȡ�����������(Read Input )
    rOut = PIDCalc ( &sPID,rIn_PID );   // PID���㺯��(Perform PID Interation)
    return (uint32_t)( rOut );              // ����������ƺ���(Effect Needed Changes)
}

