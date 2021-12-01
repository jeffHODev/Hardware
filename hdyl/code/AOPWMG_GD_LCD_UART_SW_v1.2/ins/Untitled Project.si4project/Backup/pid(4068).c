#include "pid.h"
#include<string.h>
#include<stdio.h>
#include "sensor.h"
#include<math.h>

PID sPID; // PID Control Structure
double rOut; // PID Response (Output)
double rIn_PID; // PID Feedback (Input)

float tmp;

//Error_stru *sptr;
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
    double dError,result,res_tmp;
    float Error;
    static double last_result,integ;
    static unsigned char index;
    Error = pp->SetPoint - NextPoint; // 偏差
    if(NextPoint>(pp->SetPoint+PID_O_MAX))    //如果上一次输出变量出现正向的饱和
    {
        tmp = fabs(Error);
        if(tmp>PID_I_MAX)
        {
            ;//index = 0;//偏差大积分不起作用
        }
        else
        {
            //index = 1;
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
            ;//index = 0;//偏差大积分不起作用
        }
        else
        {
            // index = 1;
            if(Error>0)
            {
                pp->SumError += Error;	//正饱和只积分负偏差

            }
        }
    }
    else
    {
        tmp = fabs(Error);
        if(tmp>PID_I_MAX)     //积分分离的PID优化，可参考以往的文章
        {
            ;//index = 0;//偏差大积分不起作用
        }
        else
        {
            ;//index = 1;
            pp->SumError += Error;	//正饱和只积分负偏差

        }
    }

    dError = pp->LastError - pp->PrevError; // 当前微分
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
    result =(pp->Proportion * Error // 比例项
             +pp->Integral * pp->SumError // 积分项
             + pp->Derivative * dError );



    if(result <=5000)
    {
        result = 5000;
    }


    if(result>=MAX_OUTPUT)
    {
        result = MAX_OUTPUT;
        // integ =  0;
    }
    pp->result = result;
    return result;        // 微分项
}
#endif
#if PID_TYPE == 2
double PIDCalc( PID *pp, double NextPoint )
{
    double dError;//,res_tmp;
    float Error;
    static double last_result;//,integ;
    // static unsigned char index;
    Error = pp->SetPoint - NextPoint; // 偏差
    if(pp->LastError==0)
        pp->LastError = Error;
    pp->NextPoint = NextPoint;
    if(NextPoint>(pp->SetPoint+PID_O_MAX))    //如果上一次输出变量出现正向的饱和
    {
        tmp = fabs(Error);
        if(tmp>PID_I_MAX)
        {
            ;//index = 0;//偏差大积分不起作用
        }
        else
        {
            //index = 1;
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
            ;//index = 0;//偏差大积分不起作用
        }
        else
        {
            // index = 1;
            if(Error>0)
            {
                pp->SumError += Error;	//正饱和只积分负偏差

            }
        }
    }
    else
    {
        tmp = fabs(Error);
        if(tmp>PID_I_MAX)     //积分分离的PID优化，可参考以往的文章
        {
            ;//index = 0;//偏差大积分不起作用
        }
        else
        {
            // index = 1;
            pp->SumError += Error;	//正饱和只积分负偏差

        }
    }
    if( Error<0)
    {
        if(pp->LastError>Error)
        {
            pp->dir_cnt_pos++;//实际值增加
            pp->dir_cnt_neg=0;

        }

        else
        {
            pp->dir_cnt_neg++;//实际值减小
            pp->dir_cnt_pos=0;

        }
        if(pp->dir_cnt_neg>255)
            pp->dir_cnt_neg=255;
        if(pp->dir_cnt_pos>255)
            pp->dir_cnt_pos=255;


    }
    else
    {
        pp->dir_cnt_pos=0;
        pp->dir_cnt_neg=0;

    }

    dError = pp->LastError - pp->PrevError; //  当前微分
    pp->PrevError = pp->LastError;
    pp->iError = Error;
    pp->LastError = Error;
      /*      pp->result =(pp->Proportion * Error // 比例项
                         +pp->Integral * pp->SumError // 积分项
                         + pp->Derivative * dError );*/	
    if(pp->dir_cnt_neg>=3)//由于滞后，降低到一定误差范围，控制量需要增加
    {
        if( Error<-100&&Error>=-300)
        {
            pp->result =  pp->result+300;
        }
        else
        {

            pp->result =(pp->Proportion * Error // 比例项
                         +pp->Integral * pp->SumError // 积分项
                         + pp->Derivative * dError );

        }
    }
    else
    {
        if(Error>=300)
        {
            pp->result = MAX_OUTPUT;

        }
        else
        {

            pp->result =(pp->Proportion * Error // 比例项
                         +pp->Integral * pp->SumError // 积分项
                         + pp->Derivative * dError );

        }
    }



    if( pp->result<=19000)
    {
        if(Error<-100)
            pp->result = 17000;
				else
            pp->result = 17000;					
       // if( pp->result<17000)
            pp->result = 17000;
        // integ =  0;
    }

    if( pp->result>=MAX_OUTPUT)
    {
        pp->result = MAX_OUTPUT;
        // integ =  0;
    }
    //result = 27000;

    //pp->result = result;
    return  pp->result;        // 微分项
}

#endif
#if PID_TYPE == 0
/*====================================================================================================
PID计算函数
=====================================================================================================*/
double PIDCalc( PID *pp, double NextPoint )
{
    double dError, Error,result,res_tmp;
    static double last_result,integ;

    Error = pp->SetPoint - NextPoint; // 偏差
    pp->SumError += Error; // 积分
    dError = pp->LastError - pp->PrevError; // 当前微分
    pp->PrevError = pp->LastError;
    pp->LastError = Error;
    result =(pp->Proportion * Error // 比例项
             +integ * pp->SumError // 积分项
             + pp->Derivative * dError );

    if(result>last_result)
    {
        res_tmp = result-last_result;
        if((res_tmp>0&&res_tmp>=2000))//2000
        {
            last_result=last_result+10;
            result = last_result;
        }
    }
    else
    {
        res_tmp = last_result-result;
        if((res_tmp>=2000))//2000
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


    //PIDInit ( &sPID ); // Initialize Structure
    pid_init( setvalue);
    /* sPID.Proportion =260; // Set PID Coefficients  260
     sPID.Integral = 0.0003;
     sPID.Derivative = 0.0001;
     sPID.SetPoint = setvalue; // Set PID Setpoint
     sPID.LastError = 0;
     sPID.PrevError = 0;
     sPID.SumError = 0;
     sPID.result = 3000;*/
}

#if PID_TYPE == 1
void pid_init(float setvalue)
{


    PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion =380; // Set PID Coefficients  260
    sPID.Integral = 0.95;//0.95;//
    sPID.Derivative = 0.8;
    sPID.result = 0;
    sPID.SetPoint = setvalue; // Set PID Setpoint
//   sPID.LastError = 0;
//	sPID.PrevError = 0;
//	sPID.SumError = 0;
}
#endif
#if PID_TYPE == 2
void pid_init(float setvalue)
{


    // PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion =65; // Set PID Coefficients  185 260
    sPID.Integral = 0.003;//0.95;//
    sPID.Derivative = 0.0004;
    // sPID.result = 0;
    sPID.SetPoint = setvalue; // Set PID Setpoint
//   sPID.LastError = 0;
//	sPID.PrevError = 0;
//	sPID.SumError = 0;
}
void pid_init2(float setvalue)
{


    // PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion =65; // Set PID Coefficients  260
    sPID.Integral = 0.003;//0.95;//
    sPID.Derivative = 0.0004;
    //  sPID.result = 0;
    sPID.SetPoint = setvalue; // Set PID Setpoint
    sPID.LastError = 0;
    sPID.PrevError = 0;
    sPID.SumError = 0;
}

#endif
#if PID_TYPE == 0
void pid_init(float setvalue)
{

    PIDInit ( &sPID ); // Initialize Structure
    sPID.Proportion =260; // Set PID Coefficients  260
    sPID.Integral = 0.0003;//
    sPID.Derivative = 0.0001;
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

    rIn_PID = rIn;				 // 读取输入变量函数(Read Input )
    rOut = PIDCalc ( &sPID,rIn_PID );   // PID计算函数(Perform PID Interation)
    return (uint32_t)( rOut );			   // 输出变量控制函数(Effect Needed Changes)


}

