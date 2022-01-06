#include "ADS129x.h"
#include "main.h"
#include "sys.h"

//#include "sd_card.h"
#include "myQueue.h"



/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2021/07/01
//版本：V1.0
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


extern QueueInfo *UART_Queue;//队列缓冲区指针


/////////////////////////////////////////////////////////////////////////////////////////////
//芯片结构体
_ADS129x_info ADS129x_info;
//系统工作状态
u8 work_state;		//工作状态
//芯片用
u32 cannle[8];	//存储8个通道的数据g
int32_t	p_Temp[8];	//数据发送缓存
u8 data_to_send[50];//串口发送缓存
volatile u8 ads129x_Cache[27];		//129x	数据缓冲区
volatile u8 ads129x_read_flag=0;	//DRDY读取完成标志
volatile u8 lead_off_flag; //导联脱落标志
unsigned char ads_num=1;//ads1299片选，1：芯片1；2：芯片2



#if ADS129X_DEVICE == ADS1299
u8 ADS129x_REG_BUF[ADS129X_REGNUM]=   //ads1299寄存器数组
{
    0x3e,//ID		ads1299   xx11 1110
    0x95,//CONFIG1	CONFIG1	1001 0110 关CLK时钟输出 0X91 8KSPS  0X92 4K 0X93 2K 0X94 1K 0X95 500SPS   0X96 250SPS
    0xD0,//CONFIG2	1101 0000 内部方波信号 	方波幅值(VREFP–VREFN)/2400=±3.75mV 增益24 实际测到的电压±90mV  权值 4500000uV/8838606 = 0.509uV
    0xEC,//CONFIG3	1110  1100 开内部基准电压4.5V  开右腿驱动放大器, ,内部1/2vref基准，关闭右腿测量和右腿导联脱落检测
    0x00,//LOFF			//关闭导联脱落检测相关
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
#if 1		//ADS1299
#if USE_SQUARE_TESTDATA==1 	//使用方波测试
    0x65,//CH1SET		第一个数 增益 			6-24 5-12 4-8 3-6 2-4 1-2 0-1
    0x65,//CH2SET	 	第二个数 输入模式		0-普通输入 1-输入短路 5-内部测试
    0x65,//CH3SET		0x61 输入短路 增益24
    0x65,//CH4SET		0x60 普通输入 增益24	 0x65 方波测试 增益24
    0x65,//CH5SET	//1299-4  4-8通道默认0x61
    0x65,//CH6SET
    0x65,//CH7SET
    0x65,//CH8SET
#elif 	USE_NOISE_TESTDATA==1	//内部噪声测试
    0x61,//CH1SET		第一个数 增益 			6-24 5-12 4-8 3-6 2-4 1-2 0-1
    0x61,//CH2SET	 	第二个数 输入模式		0-普通输入 1-输入短路 5-内部测试
    0x61,//CH3SET		0x61 输入短路 增益24
    0x61,//CH4SET		0x60 普通输入 增益24	 0x65 方波测试 增益24
    0x61,//CH5SET	//1299-4  4-8通道默认0x61
    0x61,//CH6SET
    0x61,//CH7SET
    0x61,//CH8SET
#else
    0x60,//CH1SET		第一个数 增益 			6-24 5-12 4-8 3-6 2-4 1-2 0-1
    0x60,//CH2SET	 	第二个数 输入模式		0-普通输入 1-输入短路 5-内部测试
    0x60,//CH3SET		0x61 输入短路 增益24
    0x60,//CH4SET		0x60 普通输入 增益24 0x65 方波测试 增益24
    0x60,//CH5SET	//1299-4  4-8通道默认0x61
    0x60,//CH6SET
    0x60,//CH7SET
    0x60,//CH8SET
#endif
#else	//ADS1299-4
#if USE_SQUARE_TESTDATA==1 	//使用方波测试
    0x65,//CH1SET		第一个数 增益 			6-24 5-12 4-8 3-6 2-4 1-2 0-1
    0x65,//CH2SET	 	第二个数 输入模式		0-普通输入 1-输入短路 5-内部测试
    0x65,//CH3SET		0x61 输入短路 增益24
    0x65,//CH4SET		0x60 普通输入 增益24	 0x65 方波测试 增益24
#elif 	USE_NOISE_TESTDATA==1	//内部噪声测试
    0x61,//CH1SET		第一个数 增益 			6-24 5-12 4-8 3-6 2-4 1-2 0-1
    0x61,//CH2SET	 	第二个数 输入模式		0-普通输入 1-输入短路 5-内部测试
    0x61,//CH3SET		0x61 输入短路 增益24
    0x61,//CH4SET		0x60 普通输入 增益24	 0x65 方波测试 增益24
#else
    0x60,//CH1SET		第一个数 增益 			6-24 5-12 4-8 3-6 2-4 1-2 0-1
    0x60,//CH2SET	 	第二个数 输入模式		0-普通输入 1-输入短路 5-内部测试
    0x60,//CH3SET		0x61 输入短路 增益24
    0x60,//CH4SET		0x60 普通输入 增益24 0x65 方波测试 增益24
#endif
    0x61,//CH5SET	//1299-4  4-8通道默认0x61
    0x61,//CH6SET
    0x61,//CH7SET
    0x61,//CH8SET
#endif
/////////////////////////////////////////////////////////////////////////////////
    0x00,//RLD_SENSP	//FF	使用对应通道右腿驱动 //00	禁用右腿驱动
    0x00,//RLD_SENSN	//FF	使用对应通道右腿驱动	//00	禁用右腿驱动
    0x00,//LOFF_SENSP		//关导联脱落检测
    0x00,//LOFF_SENSN		//关导联脱落检测
    0x00,//LOFF_FLIP		//导联脱落导出电流方向
    0x00,//LOFF_STATP		//导联脱落正信号状态（只读）
    0x00,//LOFF_STATN		//导联脱落负信号状态（只读）
    0x00,//GPIO		0000 0000	输出低电平
    0x00,//MISC1	该寄存器提供将SRB1引脚路由到所有通道反向输入的控制
    0x00,//MISC2	此寄存器保留以备将来使用。
    0x00,//CONFIG4	0000 0000	//连续转换 禁用导联脱落比较器
};
#endif






void ads1299NoSet(unsigned char num)
{
    ads_num = num;
}


//ADS129x的IO口初始化
void ADS129x_IO_Init(void)
{
//		GPIO_InitTypeDef 	GPIO_InitStructure;
//		EXTI_InitTypeDef 	EXTI_InitStructure;
//		NVIC_InitTypeDef	NVIC_InitStructure;
//
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
//
//	#if ADS129X_SPI == ADS129x_USE_SPI1
//		SPI1_Init();//初始化SPI引脚
//	#elif	ADS129X_SPI == ADS129x_USE_SPI2
//		SPI2_Init();//初始化SPI引脚
//	#endif
//
//		//CS
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
////		//RESRT
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
////		GPIO_Init(GPIOA, &GPIO_InitStructure);
////		//START
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
////		GPIO_Init(GPIOA, &GPIO_InitStructure);
////		//CLKSEL
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
////		GPIO_Init(GPIOA, &GPIO_InitStructure);

//		//DRDY	//待机时高电平，采集时低电平有效
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//		//DRDY中断初始化
//	  EXTI_ClearITPendingBit(EXTI_Line0);//清除中断标志
//  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);//选择管脚
//  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;						 //选择中断线路
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	 //设置为中断请求，非事件请求
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;						 //外部中断使能
//  	EXTI_Init(&EXTI_InitStructure);
//
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//选择中断通道
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//子优先级
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//使能外部中断通道
//  	NVIC_Init(&NVIC_InitStructure);
//
//		EXTI->IMR &= ~(ADS129X_DRDY_LINE);//屏蔽外部中断
//
//		ADS129X_CS=1;
}


void EXTI10_15_IRQHandler(void)
{


    if(  exti_interrupt_flag_get(EXTI_15)==1)//数据接收中断
    {
        if(ads_num == 2)
        {
            ads1299NoSet(1);


            //__disable_irq();
            //////////////////////////////////////////////////////////////////
            //读n个字节的数据,存到缓存中  //STAT CH1 CH2 ...
            ADS129x_Read_Data();//读取数据
            ///////////////////////////////////////////////////////////////////
            if(work_state == SEND_UART || work_state == SEND_BULE)//蓝牙/串口发送
            {
                //搬运数据至队列缓冲区
                if(queue_data_push(UART_Queue,ads129x_Cache+3,ADS129x_info.Ads129x_Data_Move,ADS129x_info.Ads129x_Write_Num))
                {

                }
            }
            //			   else if(work_state == SEND_WIFI )
            //			   {
            //					   if(queue_data_push(WIFI_Info->WIFI_Queue ,ads129x_Cache+3 ,ADS129x_info.Ads129x_Data_Move,ADS129x_info.Ads129x_Write_Num))
            //					   {

            //					   }
            //			   }
            //// 			   else if(work_state == WRITE_TF )
            //// 			   {
            ////
            //// 			   }
            //			   else
            //			   {

            //			   }
            /////////////////////////////////////////////////////////////////////////////////////////////////

        }


        exti_interrupt_flag_clear(EXTI_15); //清除LINE上的中断标志位
        //__enable_irq();
    }
}

void EXTI1_IRQHandler(void)
{


    if(  exti_interrupt_flag_get(EXTI_1)==1)//数据接收中断
    {
       // if(ads_num == 2)
        {
            ads1299NoSet(1);
            ADS129x_Read_Data();//读取数据
            ///////////////////////////////////////////////////////////////////
            if(work_state == SEND_UART || work_state == SEND_BULE)//蓝牙/串口发送
            {
                //搬运数据至队列缓冲区
                if(queue_data_push(UART_Queue,ads129x_Cache+3,ADS129x_info.Ads129x_Data_Move,ADS129x_info.Ads129x_Write_Num))
                {

                }
            }

        }
        exti_interrupt_flag_clear(EXTI_1); //清除LINE上的中断标志位
        //__enable_irq();
    }
}

//读取x字节数据 //STAT CH1 CH2 ...
__INLINE void ADS129x_Read_Data()//72M时钟下函数耗时大约10us  8M时钟下 函数耗时大约 100us
{
    u8 i;

    chip_sel(ads_num,0);

    for(i=0; i<ADS129X_DATANUM; i++)
    {
        ads129x_Cache[i]=ADS129x_SPI_ReadWriteByte(0X00);

    }

    chip_sel(ads_num,1);

    //ADS129X_CS=1;
#if ADS129X_USE_TRIANGLE_TESTDATA ==1 //通道1使用自增测试数据
    ads129x_Cache[3]=0;//通道1
    ads129x_Cache[4]=0;
    ads129x_Cache[5]=ads129x_test_flag++;
#endif
    ads129x_read_flag=1;//DRDY中断
}


///////////////////////////////////////////////////////////////////////////////////////////
//驱动程序
//通过SPI与ADS129x通信
__INLINE u8 ADS129x_SPI_ReadWriteByte(u8 com)
{
    return spi_TransmitReceive(com);

    /*#if ADS129X_SPI == ADS129x_USE_SPI1
    	return SPI1_ReadWriteByte(com);
    #elif	ADS129X_SPI == ADS129x_USE_SPI2
    	return SPI2_ReadWriteByte(com);
    #endif*/
}
//写命令
void ADS129x_Send_CMD(u8 data)
{
    //ADS129X_CS=0;
    chip_sel(ads_num,0);
    delay_us(10);
    ADS129x_SPI_ReadWriteByte(data);
    delay_us(10);
    // ADS129X_CS=1;
    chip_sel(ads_num,1);
}
//读写寄存器
//输入参数 ：读写命令/要读写寄存器个数/数据指针
void ADS129x_WR_REGS(u8 reg,u8 len,u8 *data)
{
    u8 i;
    // ADS129X_CS=0;
    chip_sel(ads_num,0);
    delay_us(10);
    ADS129x_SPI_ReadWriteByte(reg);
    delay_us(10);
    ADS129x_SPI_ReadWriteByte(len-1);
    if(reg&ADS129X_WREG) //写
    {
        for(i=0; i<len; i++)
        {
            delay_us(10);
            ADS129x_SPI_ReadWriteByte(*data);
            data++;
        }
    }
    else if(reg&ADS129X_RREG)//读
    {
        for(i=0; i<len; i++)
        {
            delay_us(10);
            *data = ADS129x_SPI_ReadWriteByte(0);
            data++;
        }
    }
    delay_us(10);
    chip_sel(ads_num,1);
    //ADS129X_CS=1;
}


//将 ADS129x_REG_BUF 数组中的数据写入寄存器
u8 ADS129x_WRITE_REGBUFF(void)
{
    u8 i,res=0;
    u8 ADS129x_REG_Cache[ADS129X_REGNUM];	//寄存器数据缓存


    ADS129x_WR_REGS(ADS129X_WREG|0,ADS129X_REGNUM,ADS129x_REG_BUF);//写寄存器
    delay_ms(1);
    ADS129x_WR_REGS(ADS129X_RREG|0,ADS129X_REGNUM,ADS129x_REG_Cache);//回读
    delay_ms(1);
////////////////////////////////////////////////////////////////
    ADS129X_DEBUG("要写的寄存器数据:\r\n");
    for(i=0; i<ADS129X_REGNUM; i++	)	//要写的寄存器数据
        ADS129X_DEBUG("%d %x\r\n",i,ADS129x_REG_BUF[i]);
    ADS129X_DEBUG("回读的寄存器数据:\r\n");
//////////////////////////////////////////////////////////
    for(i=0; i<ADS129X_REGNUM; i++	)	//检查REGNUM个寄存器
    {
        ADS129X_DEBUG("%d %x\r\n",i,ADS129x_REG_Cache[i]); //回读到的数据
        if(ADS129x_REG_BUF[i] != ADS129x_REG_Cache[i])
        {
#if ADS129X_DEVICE == ADS1298
            if(i!=0 && i!=18 && i!=19 && i != 20)	//ID 导联脱落和GPIO相关,不做判断
                res=1;
            else
                continue;
#elif ADS129X_DEVICE == ADS1299
            if(i!=0 && i!=18 && i!=19 && i != 20 	//ID 导联脱落和GPIO相关,不做判断
                    && i!=9 && i!=10 && i!=11 && i != 12 	) //1299-4 9 10 11 12 都是0，前面都设置对了的话，这个也不做判断了
                res=1;
            else
                continue;
#elif ADS129X_DEVICE == ADS1292
            if(i!= 0 && i!=8 && i != 11)	//0 8 和11是ID 导联脱落和GPIO相关
                res=1;
            else
                continue;
#endif
        }
    }
    if(res == 0)
        ADS129X_DEBUG("寄存器初始化成功\r\n");
    else
        ADS129X_DEBUG("寄存器初始化失败\r\n");
    return res;
}


u8 ADS129x_REG_Init(void)
{
    u8 i;
    u8 ADS129x_REG_Cache[ADS129X_REGNUM];
///////////////////////////////////////////////////////////////////////////
    ADS129x_Send_CMD(ADS129X_SDATAC);//发送停止连续读取数据命令
    delay_ms(100);
    ADS129x_Send_CMD(ADS129X_WAKEUP);//唤醒模式
    delay_ms(100);
    ADS129x_Send_CMD(ADS129X_RESET);//复位
    delay_ms(1000);
    ADS129x_Send_CMD(ADS129X_SDATAC);//发送停止连续读取数据命令
    delay_ms(100);
//		ADS129x_WR_REGS(ADS129X_RREG|0,ADS129X_REGNUM,ADS129x_REG_Cache);//读默认寄存器
//		ADS129X_DEBUG("读默认寄存器数据:\r\n");
//		for(i=0;i<ADS129X_REGNUM;i++	)	//读寄存器设置
//		{
//				ADS129x_DEBUG("%d %x\r\n",i,REG_Cache[i]);
//		}
    return ADS129x_WRITE_REGBUFF();//写寄存器
}



//初始化
//输入cmd： 0蓝牙或串口发送 / 1 wifi发送/ 2 TF卡存储

void ADS129x_ReInit(u8 cmd)
{
    u8 res;
    u8 RL_temp=0;

    //初始化1299
//		Main_printf("Init ads129x ");
//		//ADS129x_IO_Init();	//初始化ads129x
/////////////////////////////////////////////////////////////////////////////
//		RL_temp=(!KE_1);
//		Main_printf("右腿设置 %x:\r\n",RL_temp);

    ADS129x_REG_BUF[13] = RL_temp; //设置右腿驱动
    ADS129x_REG_BUF[14] = RL_temp;
///////////////////////////////////////////////////////////////////////////
    while(ADS129x_REG_Init())//初始化寄存器
    {
        // Main_printf(" err");
        delay_ms(500);
        //LED1=!LED1;//DS0闪烁
    }
    //Main_printf(" ok\r\n");
    ADS129x_Send_CMD(ADS129X_RDATAC);
    ADS129x_Send_CMD(ADS129X_START);	//发送开始数据转换（等效于拉高START引脚）
    delay_ms(500);
///////////////////////////////////////////////////////////////////////////
    ADS129x_info.Ads129x_Use_Cannle = ADS129X_USE_CANNLE*ADS129X_CASCADE_NUM; //通道*级联
    ADS129x_info.Ads129x_Data_Move = 	ADS129x_info.Ads129x_Use_Cannle*3; //一次采样搬运N字节有效数据 3*通道数

//		if(cmd==0)//串口/蓝牙
//		{
//				ADS129x_info.Ads129x_Write_Num = UART_QUEUE_LENGTH/ADS129x_info.Ads129x_Data_Move ; //搬运N次做一个包
//		}
//		else if(cmd==1)//WiFi
//		{
//				ADS129x_info.Ads129x_Write_Num = WIFI_QUEUE_LENGTH/ADS129x_info.Ads129x_Data_Move ; //搬运N次做一个包
//		}
//		else if(cmd==2)//TF卡
//		{
//				//ADS129x_info.Ads129x_Write_Num = SD_QUEUE_LENGTH/ADS129x_info.Ads129x_Data_Move ; //搬运N次做一个包
//		}
    //  Main_printf("发送 %d 通道数据\r\n",ADS129x_info.Ads129x_Use_Cannle);
    //  Main_printf("一次采样搬运 %d 字节\r\n",ADS129x_info.Ads129x_Data_Move);
    //  Main_printf("搬运 %d 次做一个包\r\n",ADS129x_info.Ads129x_Write_Num);
}




/*功能：把采到的3个字节转成有符号32位数 */
int32_t get_volt(u32 num)
{
    int32_t change;
    change = num;
    change <<= 8;
    change >>= 8;
    return change;
}

