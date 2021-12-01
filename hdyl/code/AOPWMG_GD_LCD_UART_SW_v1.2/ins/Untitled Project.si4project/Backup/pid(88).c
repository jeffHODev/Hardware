#include "pid.h"
#include<string.h>
#include<stdio.h>
PID sPID; // PID Control Structure
double rOut; // PID Response (Output)
double rIn_PID; // PID Feedback (Input)

typedef struct PID
{
    float P,I,D,limit;
} PID;
typedef struct Error
{
    float Current_Error;//��ǰ���
    float Last_Error;//��һ�����
    float Previous_Error;//���ϴ����
} Error;
Error sptr;
/*!
 *  @brief      ����ʽPID
 *  @since      v1.0
 *  *sptr ��������
 *  *pid:  PID����
 *  NowPlace��ʵ��ֵ
 *  Point��   ����ֵ
 */
// ����ʽPID�������
int32 PID_Increase( PID *pp, double NextPoint )
{

    int32 iError;	//��ǰ���
    double	Increase;	//���ó���ʵ������
    double result;
    iError = pp->SetPoint - NextPoint;	// ���㵱ǰ���

    Increase =  pp->Proportion* (iError - sptr->Last_Error)   //����P
                + pp->Integral * iError      //����I
                + pp->Derivative * (iError - 2 * sptr->Last_Error + sptr->Previous_Error);  //΢��D

    sptr->Previous_Error = sptr->Last_Error;	// ����ǰ�����
    sptr->Last_Error = iError;		  	// �����ϴ����

    pp->result = pp->result+Increase;
    if(pp->result <0)
        pp->result = 0;
    if(pp->result>=MAX_OUTPUT)
        pp->result = MAX_OUTPUT;
    return pp->result;	// ��������

}

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
        result = 0;
    if(result>=MAX_OUTPUT)
        result = MAX_OUTPUT;
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
    sPID.Proportion =260; // Set PID Coefficients  260
    sPID.Integral = 0.0003;
    sPID.Derivative = 0.00001;
    sPID.SetPoint = setvalue; // Set PID Setpoint
    sPID.LastError = 0;
    sPID.PrevError = 0;
    sPID.SumError = 0;
    sPID.result = 3000;
}
void pid_init_flow(float setvalue)
{


    PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion =260; // Set PID Coefficients  260
    sPID.Integral = 0.0003;
    sPID.Derivative = 0.00001;
    sPID.result = 3000;
    sPID.SetPoint = setvalue; // Set PID Setpoint
//   sPID.LastError = 0;
//	sPID.PrevError = 0;
//	sPID.SumError = 0;
}

//������
uint32_t pid_proc(double rIn)
{
    rIn_PID = rIn;                // ��ȡ�����������(Read Input )
    rOut = PIDCalc ( &sPID,rIn_PID );   // PID���㺯��(Perform PID Interation)
    return (uint32_t)( rOut );              // ����������ƺ���(Effect Needed Changes)
}

