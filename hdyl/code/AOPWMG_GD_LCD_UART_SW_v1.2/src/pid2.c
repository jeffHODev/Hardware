#include <reg52.h>

typedef unsigned char      uChar8;     

typedef unsigned int       uInt16;

typedef unsigned long int   uInt32;

 

sbit ConOut = P1^1;     //加热丝接到P1.1口

 

typedef struct  PID_Value

{

    uInt32 liEkVal[3];          //差值保存，给定和反馈的差值

    uChar8 uEkFlag[3];          //符号，1则对应的为负数，0为对应的为正数   

    uChar8 uKP_Coe;             //比例系数

    uChar8 uKI_Coe;             //积分常数

    uChar8 uKD_Coe;             //微分常数

    uInt16 iPriVal;             //上一时刻值

    uInt16 iSetVal;             //设定值

    uInt16 iCurVal;             //实际值

}PID_ValueStr;

 

PID_ValueStr PID;               //定义一个结构体，这个结构体用来存算法中要用到的各种数据

bit g_bPIDRunFlag = 0;          //PID运行标志位，PID算法不是一直在运算。而是每隔一定时间，算一次。

/* ********************************************************

/* 函数名称：PID_Operation()                                 

/* 函数功能：PID运算                   

/* 入口参数：无（隐形输入，系数、设定值等）                     

/* 出口参数：无（隐形输出，U(k)）

/* 函数说明：U(k)+KP*[E(k)-E(k-1)]+KI*E(k)+KD*[E(k)-2E(k-1)+E(k-2)]                                     

******************************************************** */

void PID_Operation(void)

{

    uInt32 Temp[3] = {0};   //中间临时变量

    uInt32 PostSum = 0;     //正数和

    uInt32 NegSum = 0;      //负数和

    if(PID.iSetVal > PID.iCurVal)                //设定值大于实际值否？

    {

        if(PID.iSetVal - PID.iCurVal > 10)      //偏差大于10否？

            PID.iPriVal = 100;                  //偏差大于10为上限幅值输出(全速加热)

        else                                    //否则慢慢来

        {

            Temp[0] = PID.iSetVal - PID.iCurVal;    //偏差<=10,计算E(k)

            PID.uEkFlag[1] = 0;                     //E(k)为正数,因为设定值大于实际值

            /* 数值进行移位，注意顺序，否则会覆盖掉前面的数值 */

            PID.liEkVal[2] = PID.liEkVal[1];

            PID.liEkVal[1] = PID.liEkVal[0];

            PID.liEkVal[0] = Temp[0];

            /* =================================================================== */

            if(PID.liEkVal[0] > PID.liEkVal[1])              //E(k)>E(k-1)否？

            {

                Temp[0] = PID.liEkVal[0] - PID.liEkVal[1];  //E(k)>E(k-1)

                PID.uEkFlag[0] = 0;                         //E(k)-E(k-1)为正数

            }                                      

            else

            {

                Temp[0] = PID.liEkVal[1] - PID.liEkVal[0];  //E(k)<E(k-1)

                PID.uEkFlag[0] = 1;                         //E(k)-E(k-1)为负数

            }                       

            /* =================================================================== */

            Temp[2] = PID.liEkVal[1] * 2;                   //2E(k-1)

            if((PID.liEkVal[0] + PID.liEkVal[2]) > Temp[2]) //E(k-2)+E(k)>2E(k-1)否？

            {

                Temp[2] = (PID.liEkVal[0] + PID.liEkVal[2]) - Temp[2];

                PID.uEkFlag[2]=0;                           //E(k-2)+E(k)-2E(k-1)为正数

            }                                              

            else                                            //E(k-2)+E(k)<2E(k-1)

            {

                Temp[2] = Temp[2] - (PID.liEkVal[0] + PID.liEkVal[2]);

                PID.uEkFlag[2] = 1;                         //E(k-2)+E(k)-2E(k-1)为负数

            }                                  

            /* =================================================================== */

            Temp[0] = (uInt32)PID.uKP_Coe * Temp[0];        //KP*[E(k)-E(k-1)]

            Temp[1] = (uInt32)PID.uKI_Coe * PID.liEkVal[0]; //KI*E(k)

            Temp[2] = (uInt32)PID.uKD_Coe * Temp[2];        //KD*[E(k-2)+E(k)-2E(k-1)]

            /* 以下部分代码是讲所有的正数项叠加，负数项叠加 */

            /* ========= 计算KP*[E(k)-E(k-1)]的值 ========= */

            if(PID.uEkFlag[0] == 0)

                PostSum += Temp[0];                         //正数和

            else                                            

                NegSum += Temp[0];                          //负数和

            /* ========= 计算KI*E(k)的值 ========= */

            if(PID.uEkFlag[1] == 0)    

                PostSum += Temp[1];                         //正数和

            else

                ;   /* 空操作。就是因为PID.iSetVal > PID.iCurVal（即E(K)>0）才进入if的，

                    那么就没可能为负，所以打个转回去就是了 */

            /* ========= 计算KD*[E(k-2)+E(k)-2E(k-1)]的值 ========= */

            if(PID.uEkFlag[2]==0)

                PostSum += Temp[2];             //正数和

            else

                NegSum += Temp[2];              //负数和

            /* ========= 计算U(k) ========= */                        

            PostSum += (uInt32)PID.iPriVal;        

            if(PostSum > NegSum)                 //是否控制量为正数

            {

                Temp[0] = PostSum - NegSum;

                if(Temp[0] < 100 )               //小于上限幅值则为计算值输出

                    PID.iPriVal = (uInt16)Temp[0];

                else PID.iPriVal = 100;         //否则为上限幅值输出

            }

            else                                //控制量输出为负数，则输出0(下限幅值输出)

                PID.iPriVal = 0;

        }

    }

    else PID.iPriVal = 0;                       //同上，嘿嘿

}

/* ********************************************************

/* 函数名称：PID_Output()                                    

/* 函数功能：PID输出控制                 

/* 入口参数：无（隐形输入，U(k)）                        

/* 出口参数：无（控制端）                                     

******************************************************** */

void PID_Output(void)

{

    static uInt16 iTemp;

    static uChar8 uCounter;

    iTemp = PID.iPriVal;

    if(iTemp == 0)

        ConOut = 1;     //不加热

    else ConOut = 0;    //加热

    if(g_bPIDRunFlag)   //定时中断为100ms(0.1S)，加热周期10S(100份*0.1S)

    {

        g_bPIDRunFlag = 0;

        if(iTemp) iTemp--;      //只有iTemp>0，才有必要减“1”

        uCounter++;

        if(100 == uCounter)

        {

            PID_Operation();    //每过0.1*100S调用一次PID运算。

            uCounter = 0;  

        }

    }

}

/* ********************************************************

/* 函数名称：PID_Output()                                    

/* 函数功能：PID输出控制                 

/* 入口参数：无（隐形输入，U(k)）                        

/* 出口参数：无（控制端）                                     

******************************************************** */

void Timer0Init(void)

{

    TMOD |= 0x01;   // 设置定时器0工作在模式1下

    TH0 = 0xDC;

    TL0 = 0x00;     // 赋初始值

    TR0 = 1;        // 开定时器0

    EA = 1;         // 开总中断

    ET0 = 1;        // 开定时器中断

}

 

void main(void)

{

    Timer0Init();

    while(1)

    {

        PID_Output();

    }

}

 

void Timer0_ISR(void) interrupt 1

{

    static uInt16 uiCounter = 0;

    TH0 = 0xDC;

    TL0 = 0x00;

    uiCounter++;

    if(100 == uiCounter)

    {

        g_bPIDRunFlag = 1;

    }

}
————————————————
版权声明：本文为CSDN博主「小小角色熊」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/xiaobaixiongxiong/article/details/83792580

