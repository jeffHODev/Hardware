#include "spo.h"
#include "bsp.h"
#include "config.h"
#define  cishu   16      //滤波的次数  8
#define  Snum    200     //存储的次数  200
#define  rogncuo 25      //容错的次数 
unsigned char d_up=0;
unsigned char d_dn=0;

/***********************************************************
函数名称:data_proc
函数功能:数据处理
入口参数:dat
出口参数:无
************************************************************/
void  data_proc(unsigned int temp)
{
    unsigned int x;
    x=temp;
    qw=bw=sw=gw=0;
    if(x>=1000)do
        {
            qw++;
            x=x-1000;
        }
        while(x>999);
    if(x>=100)do
        {
            bw++;
            x=x-100;
        }
        while(x>99);
    if(x>=10)do
        {
            sw++;
            x=x-10;
        }
        while(x>9);
    if(x>=1)do
        {
            gw++;
            x=x-1;
        }
        while(x>0);
}

//对200个已知样本进行测试
//ADC_tab 红光的
void  cal_RLED(void)
{
    unsigned char  i;
    F_RLED_max=F_RLED_min=UART_R_buf[0];  //获得初始值,舍弃第一个采样值
    for(i=1; i<Snum; i++)
    {
        if(F_RLED_max<UART_R_buf[i])
        {
            F_RLED_max=UART_R_buf[i];
            //peak=i;
        }
        else if(F_RLED_min>UART_R_buf[i])
        {
            F_RLED_min=UART_R_buf[i];
            //valley=i;
        }
    }
    F_RLED=F_RLED_min+(F_RLED_max-F_RLED_min)/2; //平均频率
    F_RLED_err=F_RLED_max-F_RLED_min;	          //误差
}
//对200个已知样本进行测试
//ADC_tab 红外光的
void  cal_IRLED(void)
{
    unsigned char  i;
    F_IRLED_max=F_IRLED_min=UART_IR_buf[0];  //获得初始值,舍弃第一个采样值
    for(i=1; i<Snum; i++)
    {
        if(F_IRLED_max<UART_IR_buf[i])
        {
            F_IRLED_max=UART_IR_buf[i];
            //peak=i;
        }
        else if(F_IRLED_min>UART_IR_buf[i])
        {
            F_IRLED_min=UART_IR_buf[i];
            //valley=i;
        }
    }
    F_IRLED=F_IRLED_min+(F_IRLED_max-F_IRLED_min)/2; //平均频率
    F_IRLED_err=F_IRLED_max-F_IRLED_min;	          //误差
}
//对160个已知样本进行测试
//ADC_tab	  LED
//通过ADC采样数据计算频率
void  cal_F_IRLED(void)
{
    unsigned char    i;
    unsigned int     samp;
    unsigned char    count=0;              //计数
    samp=F_IRLED_min+(F_IRLED_max-F_IRLED_min)*7/10;	   //70%数据
//峰-谷=551-483，适用于1.5HZ以下脉搏，太高了测不准。
    d_up=(F_IRLED_max-F_IRLED_min)/7; //按标准60HZ计算
    d_dn=(F_IRLED_max-F_IRLED_min)/7;
//在这个范围的数据
    for(i=0; i<Snum ; i++)
    {
        if((UART_IR_buf[i]<=(samp+d_up))&&((UART_IR_buf[i]+d_dn)>=samp))  //容错范围正负2
        {
            store_buf[count]=i;   //0-49	  记录当前对应的值
            count++;
        }
        if(count>99)
            break;                //大于99，则跳出
    }
//频率容错机制
    for(i=0; i<count; i++)
    {
        cycle=store_buf[i]-store_buf[0];
        if(cycle>rogncuo)
            break;              //调出循环
    }
    time1=i;             //第一次的容错值
    for(i=time1; i<count; i++)
    {
        cycle=store_buf[i]-store_buf[time1];
        if(cycle>rogncuo)
            break;              //第二次的容错值
    }
    time2=i;
    for(i=time2; i<count; i++)
    {
        cycle=store_buf[i]-store_buf[time2];
        if(cycle>rogncuo)
            break;              //第二次的容错值
    }
    time3=i;
    changdu=store_buf[time2]-store_buf[0];
    changdu1=store_buf[time3]-store_buf[time1];
    changdu=(changdu+changdu1)>>1;             //计算脉搏周期
    Freq=7200/(unsigned int)changdu;          //计算1min钟的脉搏数
}
//=========================================
//血氧处理
//=========================================
void  deal_spo2(void)
{
    //红光
    cal_RLED();       //计算采集数据中的最大最小值
    //红外光
    cal_IRLED();      //计算采集数据中的最大最小值
    cal_F_IRLED();    //计算频率
    R=((float)F_RLED_err/(float)F_RLED)/((float)F_IRLED_err/(float)F_IRLED);
    //SPO2=110-15*R;    if(R<=30)
    if(R<=5)
        //SPO2=110-R*3.6;
        SPO2=101-R*2.0;
    else
        SPO2=0;

    if(SPO2>99)
        SPO2=99;          //血样最大值为99%

}
//参数初始化
void  canshu_init(void)
{
    unsigned char j=0;
    for(j=0; j<cishu; j++)
        filter_buf[j]=70;
}
//参数初始化
void  canshu_init2(void)
{
    unsigned char j=0;
    for(j=0; j<cishu; j++)
        filter_SPO2[j]=99;
}
void  canshu_init3(void)
{
    unsigned char j=0;
    for(j=0; j<cishu; j++)
        filter_buf[j]=0;
}

//滑动平均滤波
//滑动去极值求平均
void  Slide_filter(void)
{
    unsigned char i,j;
    sum=0;
    for(j=0; j<(cishu-1); j++)			 //采样值更行
    {
        i=j+1;
        filter_buf[j]=filter_buf[i];
    }
    filter_buf[cishu-1]=Freq;	          //当前值放在最后
    //以上完成滑动滤波作用
    MB_max=MB_min=filter_buf[0];
    for(i=1; i<cishu; i++)
    {
        if(MB_max<filter_buf[i])
        {
            MB_max=filter_buf[i];
        }
        else if(MB_min>filter_buf[i])
        {
            MB_min=filter_buf[i];
        }
    }
    for(j=0; j<cishu; j++)              //求和
    {
        sum+=filter_buf[j];
    }
    sum=sum-MB_max;
    sum=sum-MB_min;
    //以上完成去极值作用
    XYMB[1]=sum/14;
    if(XYMB[1]>160)                          //对脉搏进行限制
        XYMB[1]=160;
}
//滑动平均滤波
void  Slide_filter_spo2(void)
{
    unsigned char i,j;
    sum=0;
    for(j=0; j<(cishu-1); j++)			 //采样值更行
    {
        i=j+1;
        filter_SPO2[j]=filter_SPO2[i];
    }
    filter_SPO2[cishu-1]=SPO2;      //当前值放在最后
    for(j=0; j<cishu; j++)          //求和
    {
        sum+=filter_SPO2[j];
    }
    XYMB[0]=sum>>4;
}

//数据采集处理
void  samp_deal(void)
{
    //如果是在采集数据，并且一次采集没有完成,也没有发送数据
    //采集数据
    static uint32_t systick_10ms;
    if((HAL_GetTick()-systick_10ms)>=10)
    {
        systick_10ms = HAL_GetTick();
        if(step==0)
        {
            light_red();		 //开红光
            //Pulse_count=ADC10_samp(0x02); //通道2，脉搏电压采集
            Pulse_count=getAdcBuf()->adc_value[3]; //通道ADC12_10，脉搏电压采集
            Pulse_count=Pulse_count/4;
            UART_R_buf[RS232_cnt]=Pulse_count;
        }
        else
        {
            light_Ired();		 //开红外光
            //Pulse_count=ADC10_samp(0x02); //通道2，脉搏电压采集
            Pulse_count=getAdcBuf()->adc_value[3]; //通道ADC12_10，脉搏电压采集
            Pulse_count=Pulse_count/4;
            UART_IR_buf[RS232_cnt]=Pulse_count;
        }
        if(diuqi>0)
        {
            diuqi--;
            RS232_cnt=0;
        }
        else
            RS232_cnt++;		  //完成一次采集,如果采集间隔超过10ms，认为超时。
        if(RS232_cnt>=Snum)
        {
            onetimes_flag=1;
            RS232_cnt=0;
        }

    }

}
void  no_finger_check(void)
{
    if(F_IRLED_max<F_IRLED_min+50)
    {
        finger_flg=0;
        canshu_init3();
        nofinger_cnt=1;
    }
    else
    {
        finger_flg=1;
        if(nofinger_cnt==1)
        {
            canshu_init();            //参数初始化
            canshu_init2();           //参数初始化
            nofinger_cnt=0;
        }
    }
}

void spo_proc()
{
    samp_deal();   //分时任务
    //keypro();	   //
    if(onetimes_flag==1)
    {
        step++;
        if(step==1)
        {
            light_Ired();		//开红外光
        }
        if(step>=2)
        {
            light_stop();		  //停止
            deal_spo2();			  //血氧处理
            Slide_filter();		  //滑动滤波
            Slide_filter_spo2();   //滤除血氧
            no_finger_check();     //无手指检测
            data_proc(XYMB[0]);		  //血氧饱和度
            SPO2_buf[1]=sw;
            SPO2_buf[0]=gw;
            data_proc(XYMB[1]);		  //脉搏
            MB_buf[2]=bw;
            MB_buf[1]=sw;
            MB_buf[0]=gw;
            if(nofinger_cnt == 1)
            	{
            	XYMB[0] = 0;
				XYMB[1] = 0;

			}
            //Send_char1(0x68);	  //串口发送字头
            //  Send_char1(XY);		//串口发送血氧
            //  Send_char1(MB);		//串口发送脉搏
            // Send_char1(0x16); 	  //串口发送字尾 20:14:04:04:24:59   未连接，闪。连接，灯常亮。

            light_red();			  //开红光
            step=0;
        }
        diuqi=80;
        RS232_cnt=0;
        onetimes_flag=0;		  //一次完成
    }

}
uint8_t *getSpo(void)
{
    return XYMB;
}


