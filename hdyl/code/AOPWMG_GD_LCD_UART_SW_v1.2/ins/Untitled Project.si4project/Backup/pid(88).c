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
    float Current_Error;//当前误差
    float Last_Error;//上一次误差
    float Previous_Error;//上上次误差
} Error;
Error sptr;
/*!
 *  @brief      增量式PID
 *  @since      v1.0
 *  *sptr ：误差参数
 *  *pid:  PID参数
 *  NowPlace：实际值
 *  Point：   期望值
 */
// 增量式PID电机控制
int32 PID_Increase( PID *pp, double NextPoint )
{

    int32 iError;	//当前误差
    double	Increase;	//最后得出的实际增量
    double result;
    iError = pp->SetPoint - NextPoint;	// 计算当前误差

    Increase =  pp->Proportion* (iError - sptr->Last_Error)   //比例P
                + pp->Integral * iError      //积分I
                + pp->Derivative * (iError - 2 * sptr->Last_Error + sptr->Previous_Error);  //微分D

    sptr->Previous_Error = sptr->Last_Error;	// 更新前次误差
    sptr->Last_Error = iError;		  	// 更新上次误差

    pp->result = pp->result+Increase;
    if(pp->result <0)
        pp->result = 0;
    if(pp->result>=MAX_OUTPUT)
        pp->result = MAX_OUTPUT;
    return pp->result;	// 返回增量

}

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
    dError = pp->LastError - pp->PrevError; // 当前微分
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
    result =(pp->Proportion * Error // 比例项
             + pp->Integral * pp->SumError // 积分项
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

//主函数
uint32_t pid_proc(double rIn)
{
    rIn_PID = rIn;                // 读取输入变量函数(Read Input )
    rOut = PIDCalc ( &sPID,rIn_PID );   // PID计算函数(Perform PID Interation)
    return (uint32_t)( rOut );              // 输出变量控制函数(Effect Needed Changes)
}

