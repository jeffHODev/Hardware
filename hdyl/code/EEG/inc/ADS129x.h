#ifndef __ADS129x_H
#define __ADS129x_H	 

//#include "sys.h" 
#include "myQueue.h"


///////////////////////////////////////////////////////////////////////////////
//芯片及功能选择
#define	ADS1298		1 
#define	ADS1299		2
#define	ADS1292		3
#define		SEND_BULE			 1 //蓝牙发送
#define		SEND_WIFI			 2 //WIFI发送
#define		SEND_UART			 3 //主串口发送
#define		WRITE_TF	 		 4 //写SD卡
#define		SEND_USB			 5 //USB发送
#define	ADS129x_USE_SPI1		1
#define	ADS129x_USE_SPI2		2
///////////////////////////////////////////////////////////////////////////////
//芯片及功能选择
#define ADS129X_DEVICE			ADS1299	 	//芯片选择
#define ADS129X_UART_DEBUG	0 	//寄存器串口调试	 //1 使用寄存器串口调试 0不适用
#define ADS129X_CASCADE_NUM	1		//设备级联数量
#define ADS129X_USE_CANNLE	8 	//芯片使用N个通道
//测试数据选项（优先级 锯齿波>方波>噪声）
#define ADS129X_USE_TRIANGLE_TESTDATA 	0 		//通道1使用锯齿波自增测试数据 	//1 使用测试数据 0不使用 虚拟数据，不使用ADS129X
#define ADS129X_USE_SQUARE_TESTDATA 		0 		//使用内部方波数据 				//1 使用测试数据 0不使用 方波数据，通过ADS129X采集
#define	ADS129X_USE_NOISE_TESTDATA			1			//测试内部短接噪声
////////////////////////////////////////////////////////////////////////////////
//引脚及通讯接口
#define ADS129X_DRDY_LINE			EXTI_Line0//DRDY中断通道
#define ADS129X_SPI 					ADS129x_USE_SPI1	//SPI选择
#define ADS129X_DRDY					PBin(0)
#define ADS129X_CS						PAout(4)		
//	#define ADS129X_RESET 	PAout(0)
//	#define ADS129X_START		PAout(2)	
//	#define ADS129X_CLKSEL	PAout(3)	
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
typedef struct
{
		u8 Ads129x_Cascade_Num;//芯片级联数量
		u8 Ads129x_Use_Cannle;//一个芯片使用N个通道
	
		u16 Ads129x_Data_Move; //每次中断搬运N个数据 ADS129X_Data_Move =  Ads129x_USE_Cannle*3
		u16 Ads129x_Write_Num; //搬运N次 做一个包   ADS129x_Write_Num = ADS129x_SAMPLING_POINT / Ads129x_USE_Cannle

		QueueInfo *ADS129x_Queue;//数据缓冲区指针
}_ADS129x_info;
extern _ADS129x_info ADS129x_info;//芯片结构
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//芯片寄存器数量及中断读取的字节数
#if ADS129X_DEVICE == ADS1298 
	#define ADS129X_REGNUM 26			//寄存器数量
	#define ADS129X_DATANUM 27		//一次DRDY中断的数据读取数量
#elif ADS129X_DEVICE == ADS1299
	#define ADS129X_REGNUM 24			//寄存器数量
	#define ADS129X_DATANUM 27		//数据读取数量
#elif ADS129X_DEVICE == ADS1292
	#define ADS129X_REGNUM 12			//寄存器数量
	#define ADS129X_DATANUM 9			//数据读取数量
#endif
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//声明外部变量
extern u8 work_state;//系统工作状态
//芯片相关变量
extern	u8 ADS129x_REG_BUF[ADS129X_REGNUM];//寄存器数组
extern	u8 data_to_send[50];//串口发送缓存
extern	u32 cannle[8];	//存储8个通道的数据g
extern	int32_t	p_Temp[8];	//数据缓存
extern volatile u8 ads129x_Cache[27];		//129x数据缓冲区
extern volatile u8 ads129x_read_flag;	//DRDY 读取完成1次标志
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//串口DEBUG	
#if ADS129X_UART_DEBUG == 1 
		#define ADS129X_DEBUG(...)    Main_printf(__VA_ARGS__)
#else
		#define ADS129X_DEBUG(...)		
#endif
////////////////////////////////////////////////////////////////////////////////	

/////////////////////////////////////////////////////////////////////////////////////////////
//命令定义
//注意： 开机默认处于 RDATAC 模式时，要读写寄存器必须先发送SDATAC命令
//如果器件处于 RDATAC 模式，则必须先发出 SDATAC命令，然后才能将任何其他命令发送到器件中。
//系统命令
	#define ADS129X_WAKEUP	0X02	//从待机模式唤醒
	#define ADS129X_STANDBY	0X04	//进入待机模式
	#define ADS129X_RESET	0X06	//复位
	#define ADS129X_START	0X08	//启动转换
	#define ADS129X_STOP	0X0A	//停止转换
//数据读取命令
	#define ADS129X_RDATAC	0X10	//启用连续的数据读取模式,默认使用此模式
	#define ADS129X_SDATAC	0X11	//停止连续的数据读取模式
	#define ADS129X_RDATA	0X12	//通过命令读取数据;支持多个读回。
//寄存器读取命令
	//r rrrr=要读、写的寄存器首地址	 //	n nnnn=要读写的寄存器数量
	#define ADS129X_RREG	0X20	//读取  001r rrrr(首字节) 000n nnnn(2字节) 
	#define ADS129X_WREG	0X40	//写入  010r rrrr(首字节) 000n nnnn(2字节)
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
int32_t get_volt(u32 num);//把采到的3个字节转成有符号32位数

void ADS129x_IO_Init(void); //初始化ADS1292引脚
void ADS129x_ReInit(u8 cmd);//重复初始化

__INLINE u8 ADS129x_SPI_ReadWriteByte(u8 com);		//SPI通信
__INLINE void ADS129x_Read_Data(void);//72M时钟下函数耗时大约10us  8M时钟下 函数耗时大约 100us

void ADS129x_Send_CMD(u8 data);//发送命令
void ADS129x_WR_REGS(u8 reg,u8 len,u8 *data);//读写寄存器
u8 ADS129x_WRITE_REGBUFF(void);//将寄存器数组写入寄存器
u8 ADS129x_REG_Init(void);//寄存器初始化

#endif


/* //默认寄存器值 1298
0 92
1 6
2 0
3 40
4 0
5 0
6 0
7 0
8 0
9 0
10 0
11 0
12 0
13 0
14 0
15 0
16 0
17 0
18 0
19 0
20 f
21 0
22 0
23 0
24 0
25 0
*/

/* //默认寄存器值 1299
0 3e
1 96
2 c0
3 60
4 0
5 61
6 61
7 61
8 61
9 61
10 61
11 61
12 61
13 0
14 0
15 0
16 0
17 0
18 0
19 0
20 f
21 0
22 0
23 0
*/

