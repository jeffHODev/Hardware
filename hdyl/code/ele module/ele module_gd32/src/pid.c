#include "pid.h"
#include<string.h>
#include<stdio.h>
#include "sys.h"
PID sPID; // PID Control Structure
double rOut; // PID Response (Output)
double rIn_PID; // PID Feedback (Input)


/*====================================================================================================
PID计算函数
=====================================================================================================*/
double PIDCalc( PID *pp, double NextPoint )
{
    double dError, Error,result,res_tmp;
    static double last_result;
    Error = pp->SetPoint - NextPoint; // 偏差


//	result = pp->Proportion*(Error-pp->LastError)    //增量计算
//	+pp->Integral*Error
//	+pp->Derivative*(Error-2*pp->LastError+pp->PrevError);

//	pp->PrevError=pp->LastError; //存储误差，便于下次计算
//	pp->LastError=Error;

    pp->SumError += Error; // 积分
//	  if(pp->SumError>=MAX_I)
//			pp->SumError = MAX_I;
    dError = pp->LastError - pp->PrevError; // 当前微分
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
    result =(pp->Proportion * Error // 比例项
             + pp->Integral * pp->SumError // 积分项
             + pp->Derivative * dError );


    if(result>last_result)
    {
        res_tmp = result-last_result;
        if((res_tmp>0&&res_tmp>=2000))
        {
            last_result=last_result+150;
            result = last_result;
        }
    }
    else
    {
        res_tmp = last_result-result;
        if((res_tmp>=2000))
        {
            last_result=last_result-300;
            result = last_result;
        }
    }

    if(result <1800)
        result = 1800;
    registerTick(PID_OUT_TICK,5000,1,0);
    if(GetTickResult(PID_OUT_TICK)==1)
    {
        // registerTick(PID_OUT_TICK,0,0,1);
        if(result>=MAX_OUTPUT)
            result = MAX_OUTPUT;
        //else
        //	result = MAX_OUTPUT/3;
    }
    else
    {

            result = result/3;

    }



    pp->result = result;
//	result = 1000;
    return result;        // 微分项
}

/*====================================================================================================
PID结构体变量初始化函数
=====================================================================================================*/
void PIDInit (PID *pp)
{
    memset ( pp,0,sizeof(PID));
}

/*====================================================================================================
读取输入变量函数（在此设定为固定值100）
======================================================================================================*/


/*====================================================================================================
输出变量控制函数
======================================================================================================*/
void actuator(double rDelta)
{
}
void pid_init(float setvalue)
{


    PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion = 20; // Set PID Coefficients  26
    sPID.Integral = 0.00000005;
    sPID.Derivative = 0;
    sPID.SetPoint = setvalue; // Set PID Setpoint
    sPID.LastError = 0;
    sPID.PrevError = 0;
    sPID.SumError = 0;

//	    sPID.Proportion = 5.75; // Set PID Coefficients
//    sPID.Integral = 0.005;
//    sPID.Derivative = 1;
//    sPID.SetPoint = setvalue; // Set PID Setpoint
//    sPID.LastError = 0;
//	  sPID.PrevError = 0;
//	  sPID.SumError = 0;

}

//主函数
uint32_t pid_proc(double rIn)
{
    rIn_PID = rIn;                // 读取输入变量函数(Read Input )
    rOut = PIDCalc ( &sPID,rIn_PID );   // PID计算函数(Perform PID Interation)
    //rOut = 0;
    return (uint32_t)( rOut );              // 输出变量控制函数(Effect Needed Changes)
}

