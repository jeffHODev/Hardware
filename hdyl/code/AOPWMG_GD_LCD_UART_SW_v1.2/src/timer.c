#include "timer.h"
#include "filter.h"
#if CPU == ST
TIM_HandleTypeDef TIM3_Handler;         //定时器5句柄
#endif
//定时器5通道1输入捕获配置
//arr：自动重装值(TIM2,TIM5是32位的!!)
//psc：时钟预分频数
void TIM3_CH3_Cap_Init(uint32_t arr,unsigned int psc)
{
#if CPU == ST
    TIM_IC_InitTypeDef TIM3_CH33Config;

    TIM3_Handler.Instance=TIM3;                          //通用定时器5
    TIM3_Handler.Init.Prescaler=psc;                     //分频系数
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=arr;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频银子
    HAL_TIM_IC_Init(&TIM3_Handler);//初始化输入捕获时基参数

    TIM3_CH33Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
    TIM3_CH33Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
    TIM3_CH33Config.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM3_CH33Config.ICFilter=0;                          //配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM3_Handler,&TIM3_CH33Config,TIM_CHANNEL_3);//配置TIM5通道1

    HAL_TIM_IC_Start_IT(&TIM3_Handler,TIM_CHANNEL_3);   //开启TIM3的捕获通道3，并且开启捕获中断
    __HAL_TIM_ENABLE_IT(&TIM3_Handler,TIM_IT_UPDATE);   //使能更新中断
#else

#endif
}



//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
unsigned char  TIM3CH3_CAPTURE_STA=0;	//输入捕获状态
uint32_t	TIM3CH3_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)


//定时器5中断服务函数
/*void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);//定时器共用处理函数
}*/


#if CPU == ST
//定时器更新中断（计数溢出）中断处理回调函数， 该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//更新中断（溢出）发生时执行
{

    if((TIM3CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
    {
        if(TIM3CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
        {
            if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
            {
                TIM3CH3_CAPTURE_STA|=0X80;		//标记成功捕获了一次
                TIM3CH3_CAPTURE_VAL=0XFFFFFFFF;
            }
            else TIM3CH3_CAPTURE_STA++;
        }
    }
}
#else


#endif

uint32_t timecount,test,timecount1,edge_cnt;
float fq=-1,fq_t;
unsigned char state,fq_flag;
uint32_t result;
//定时器输入捕获中断处理回调函数，该函数在HAL_TIM_IRQHandler中会被调用
void TIM3CaptureChannel3Callback()//捕获中断发生时执行
{
    
    /*	if((TIM3CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
    	{
    		if(TIM3CH3_CAPTURE_STA&0X40)		//捕获到一个下降沿
    			{
    				TIM3CH3_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
                    TIM3CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_3);//获取当前的捕获值.
                    TIM_RESET_CAPTUREPOLARITY(&TIM3_Handler,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
                    TIM_SET_CAPTUREPOLARITY(&TIM3_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//配置TIM5通道1上升沿捕获
    			}else  								//还未开始,第一次捕获上升沿
    			{
    				TIM3CH3_CAPTURE_STA=0;			//清空
    				TIM3CH3_CAPTURE_VAL=0;
    				TIM3CH3_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
    				__HAL_TIM_DISABLE(&TIM3_Handler);        //关闭定时器5
    				__HAL_TIM_SET_COUNTER(&TIM3_Handler,0);
    				TIM_RESET_CAPTUREPOLARITY(&TIM3_Handler,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
    				TIM_SET_CAPTUREPOLARITY(&TIM3_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);//定时器5通道1设置为下降沿捕获
    				__HAL_TIM_ENABLE(&TIM3_Handler);//使能定时器5
    			}
    	}*/

#if CPU == ST

    {
        TIM_RESET_CAPTUREPOLARITY(&TIM3_Handler,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
        //清除中断标志位
        if(state==0)    //捕获第一个上升沿
        {
            state=1;
            timecount=HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_3); 	   //记录第一次上升沿的CNT值
        }
        else if(state==1)//捕获第二个上升沿
        {
            fq_flag = 1;
            state=0;
            timecount1=HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_3);   //记录第二次上升沿的CNT值
            if(timecount<timecount1)
            {
                test=timecount1-timecount; 		  //两次上升沿的差值
            }
            else if(timecount>timecount1)
            {
                test=(0xffff-timecount)+timecount1;  //两次上升沿的差值
            }
            else
                test=0;
            uint32_t result;
            result = filter2(test);
            if(result!=NULL)
                fq=1000000/result;					  //脉冲的频率
            else
                fq = -1;
        }
    }



#else
    if(SET == timer_interrupt_flag_get(TIMER2,TIMER_INT_CH2))
    {
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER2,TIMER_INT_CH2);

        if(0 == state)
        {
            /* read channel 0 capture value */
            timecount = timer_channel_capture_value_register_read(TIMER2,TIMER_CH_2)+1;
			edge_cnt++;
            state = 1;
        }
        else if(1 == state)
        {
            fq_flag = 1;
            state=0;
            /* read channel 0 capture value */
            timecount1 = timer_channel_capture_value_register_read(TIMER2,TIMER_CH_2)+1;

            if(timecount1 > timecount)
            {
                test = (timecount1 - timecount);
            }
            else
            {
                test = ((0xFFFF - timecount) + timecount1);
            }

           // result = filter2(test);
            if(test!=NULL)
						{
						  fq=1000000/test;					 //脉冲的频率
							fq_t = fq;
						}
              
            else
                fq = -1;


            state = 0;
        }
    }

#endif


}
float *GetCapture()
{

    // if(fq_flag==0)
    // return -1;
    return &fq;

}
uint32_t *GetCapture_cnt()
{

    // if(fq_flag==0)
    // return -1;
    return &edge_cnt;

}
#if CPU == ST
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
#else
void HAL_TIM_IC_CaptureCallback()
#endif
{
#if CPU == ST
    if(htim->Instance == TIM3)
    {
        if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_3)
        {
            TIM3CaptureChannel3Callback();
        }
    }
#else
    //if(SET == timer_interrupt_flag_get(TIMER2,TIMER_INT_CH2)){
    TIM3CaptureChannel3Callback();
    //}

#endif
}

