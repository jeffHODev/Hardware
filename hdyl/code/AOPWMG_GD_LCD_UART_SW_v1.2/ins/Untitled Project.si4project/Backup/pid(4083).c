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
    uint32_t Current_Error;//当前误差
    uint32_t Last_Error;//上一次误差
    uint32_t Previous_Error;//上上次误差
} Error_stru;
Error_stru *sptr;
/*!
 *  @brief      增量式PID
 *  @since      v1.0
 *  *sptr ：误差参数
 *  *pid:  PID参数
 *  NowPlace：实际值
 *  Point：   期望值
 */
// 增量式PID电机控制
#if PID_TYPE == 1
double PIDCalc( PID *pp, double NextPoint )
{
    uint32_t dError, Error,result,res_tmp;
    static uint32_t last_result,integ;
    static unsigned char index;
    Error = pp->SetPoint - NextPoint; // 偏差


//	result = pp->Proportion*(Error-pp->LastError)    //增量计算
//	+pp->Integral*Error
//	+pp->Derivative*(Error-2*pp->LastError+pp->PrevError);

//	pp->PrevError=pp->LastError; //存储误差，便于下次计算
//	pp->LastError=Error;

    // pp->SumError += Error; // 积分
  

    if(NextPoint>(pp->SetPoint+PID_O_MAX))    //如果上一次输出变量出现正向的饱和
    {
			  tmp = fabs(Error);
        if(tmp>PID_I_MAX)
        {
            index = 0;//偏差大积分不起作用
        }
        else
        {
            index = 1;
            if(Error<0)
            {
                pp->SumError += Error;  //正饱和只积分负偏差
            }
        }
    }
    else if(NextPoint<(pp->SetPoint-PID_O_MAX))  //如果上一次输出变量出现负向的饱和
    {
						  tmp = fabs(Error);
        if(tmp>PID_I_MAX)
        {
            index = 0;//偏差大积分不起作用
        }
        else
        {
            index = 1;
            if(Error>0)
            {
                pp->SumError += Error;  //负饱和只积分正偏差
            }
        }
    }
    else
    {
			 tmp = fabs(Error);
        if(tmp>PID_I_MAX)     //积分分离的PID优化，可参考以往的文章
        {
            index = 0;//偏差大积分不起作用
        }
        else
        {
            index = 1;
            pp->SumError += Error;
        }
    }
   
    dError = pp->LastError - pp->PrevError; // 当前微分
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
    result =(pp->Proportion * Error // 比例项
             +pp->Integral *index* pp->SumError // 积分项
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
    return result;        // 微分项
}

#else
/*====================================================================================================
PID计算函数
=====================================================================================================*/
double PIDCalc( PID *pp, double NextPoint )
{
    double dError, Error,result,res_tmp;
    static double last_result,integ;

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
             +integ * pp->SumError // 积分项
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
    return result;        // 微分项
}
#endif
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

//主函数
uint32_t pid_proc(double rIn)
{
    rIn_PID = rIn;                // 读取输入变量函数(Read Input )
    rOut = PIDCalc ( &sPID,rIn_PID );   // PID计算函数(Perform PID Interation)
    return (uint32_t)( rOut );              // 输出变量控制函数(Effect Needed Changes)
}

