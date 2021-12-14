/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************
 * 文件名  ：system.c
 * 描    述：MODBUS开发
 * 实验平台：STM32通讯转换
 * 硬件连接：无
 * 库版本  ：ST3.0.0
 * 作者    ：小奕
 * 项目说明：MODBUS
**********************************************************************************/
#include "stm32f10x.h"
#include "driveboard.h"
#include "usart3.h"
#include "system.h"
#include "bsp_led.h"
#include "delay.h"
#include "iic.h"
#include "adc.h"
#include "pid.h"

/*****************************变量声明*******************************************/
__IO uint8_t Cswater_Temp=0;					// 出水温度
__IO float   Hmwater_Temp=0;					// 加热体温度
__IO uint8_t Yswater_Temp=0;					// 原水温度
__IO uint16_t Now_Elecr_Current=0;		// 当前电流 ma 0-4000ma
__IO uint16_t Yswater_Tds=0;					// 原水TDS
__IO uint16_t Filter_Life=0;					// 滤芯寿命 0L
__IO uint16_t Filter_Count=0;					// 滤芯用水量 1000ml			
__IO uint8_t  Standby_Mode=0;					// 0 正常模式  1待机模式
__IO uint8_t 	Work_Step=0;						// 工作步骤及模式
__IO uint8_t  High_Temp_Flage=0;			// 高温报警
__IO uint8_t  Show_TDS_Value=0;				// 上电显示TDS值  0 正常   			1 显示TDS值
__IO uint8_t  Test_Filter_Flage=0;		// 测试滤芯       0 正常滤芯    1 测试滤芯
__IO uint8_t  Tds_Test_Flage=0;				// TDS检测标志位  0 不检测 1 检测

__IO uint8_t Set_Temp_Num=0;					// 设置出水温度   0 常温   1 适中  2 中温  3 高温
__IO uint8_t Set_PH_Num=0;						// 设置出水PH     0 净水   1 弱碱  2 中碱  3 强碱
__IO uint8_t Set_Temp_Table[4]={25,45,65,80};	
__IO uint8_t Set_PH_Table[4]={7,8,9,10};	
__IO uint16_t Set_ORP_Table[4]={000,150,250,350};	

uint8_t RF_ID_Code[4]={0,1,2,3};			// ID卡号
uint8_t New_Filter_Flage=0;					  // 新滤芯标志

static SENSERMODE_TypeDef Senser_Mode;// 传感器模式
static uint8_t Write_RFID_Bit=0;			// 均衡写卡 0-1
static uint16_t Clear_Count=0;				// 累计清洗用水量	
static uint16_t Total_Flow=0;					// 累计出水量 单次统计
static uint8_t Erro_Flage=0;					// 错误标志 0位 TDS异常   1位温度异常   2位原水空  3酸水满  
static uint8_t RF_Read_Num=0;					// 读取标志 RFID 如果读取10次均没有相应则报警
static uint8_t Filter_Ready_Flage=0;	// 滤芯就位标志
static float Now_Current_Speed=0;			// 当前水流速度

static void Set_Speak_Value(uint8_t flage)
{
	Speak_Flage=flage;
	TIM_Cmd(TIM2,ENABLE);
}
/***********************************************************************************
 * 函数名：uint8_t Senser_Value_Read(void)
 * 描述  ：获取各个传感器参数
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
uint8_t Senser_Value_Read(void)
{
  // 按键滤波时间计时
  static uint32_t Senser_Time_SXG=0;	      // 传感器-水箱盖子
  static uint32_t Senser_Time_YSXU=0;	    	// 传感器-原水箱水位
  static uint32_t Senser_Time_SSXU=0;	      // 传感器-酸水箱水位
  static uint32_t Senser_Time_YSXD=0;	    	// 传感器-原水箱水位
  static uint32_t Senser_Time_SSXD=0;	      // 传感器-酸水箱水位
	uint8_t keycode;
	
	// 水箱盖板状态  正常状态为盖下 = 0 常闭
  if(SENSER_SXG==0)																					// 水箱盖关闭
  {		
    if(Senser_Mode.SENSER_SXG_MODE==SENSER_SXG_HOLD)				// 持续抬起
    {
			Senser_Mode.SENSER_SXG_MODE=SENSER_SXG_HOLD;					// 持续抬起
    }
    else if(Senser_Mode.SENSER_SXG_MODE==SENSER_SXG_DOWN)   // 前一次是按键按下
    {
      if(Senser_Time_SXG++>SENSER_FILTER_TIME)							// 检测到长按键
      {
        Senser_Mode.SENSER_SXG_MODE=SENSER_SXG_HOLD;				// 持续抬起
      }		
    }
    else 																										// 前一次没有按键按下
    {
      if(Senser_Time_SXG++>SENSER_FILTER_TIME)							// 按键通过滤波检测
      {
        Senser_Mode.SENSER_SXG_MODE=SENSER_SXG_UP;					// 盖板抬起
      }
    }	
  }
  else
  {
    Senser_Mode.SENSER_SXG_MODE=SENSER_SXG_DOWN;						// 盖板放下
    Senser_Time_SXG=0;
  }			

	// 原水箱水位状态		正常状态为高水位  = 0 常闭
  if(SENSER_YSX==0)																				 	// 原水箱
  {		
    if(Senser_Mode.SENSER_YSX_MODE==SENSER_YSX_DOWN)				// 持续就位
    {
			Senser_Mode.SENSER_YSX_MODE=SENSER_YSX_DOWN;					// 持续就位
    }
    else 																										// 前一次没有按键按下
    {
      if(Senser_Time_YSXD++>SENSER_FILTER_TIME)							// 按键通过滤波检测
      {
        Senser_Mode.SENSER_YSX_MODE=SENSER_YSX_DOWN;				// 原水箱缺水或者原水箱分水位异常
      }
    }	
		Senser_Time_YSXU=0;
  }
  else
  {
    if(Senser_Mode.SENSER_YSX_MODE==SENSER_YSX_UP)					// 持续就位
    {
			Senser_Mode.SENSER_YSX_MODE=SENSER_YSX_UP;						// 持续就位
    }
    else 																										// 前一次没有按键按下
    {
      if(Senser_Time_YSXU++>SENSER_FILTER_TIME)							// 按键通过滤波检测
      {
        Senser_Mode.SENSER_YSX_MODE=SENSER_YSX_UP;					// 原水箱缺水或者原水箱分水位异常
      }
    }	
		Senser_Time_YSXD=0;
  }		
	// 酸水箱水位状态   正常状态为低水位  = 0 常闭  
  if(SENSER_SSX==0)																				 	// 酸水箱
  {		
    if(Senser_Mode.SENSER_SSX_MODE==SENSER_SSX_DOWN)				// 持续抬起
    {
			Senser_Mode.SENSER_SSX_MODE=SENSER_SSX_DOWN;					// 持续抬起
    }
    else 																										// 前一次没有按键按下
    {
      if(Senser_Time_SSXD++>SENSER_FILTER_TIME)							// 按键通过滤波检测
      {
        Senser_Mode.SENSER_SSX_MODE=SENSER_SSX_DOWN;				// 酸水箱废水满或酸水箱位置异常
      }
    }	
		Senser_Time_SSXU=0;
  }
  else
  {
    if(Senser_Mode.SENSER_SSX_MODE==SENSER_SSX_UP)					// 持续抬起
    {
			Senser_Mode.SENSER_SSX_MODE=SENSER_SSX_UP;						// 持续抬起
    }
    else 																										// 前一次没有按键按下
    {
      if(Senser_Time_SSXU++>SENSER_FILTER_TIME)							// 按键通过滤波检测
      {
        Senser_Mode.SENSER_SSX_MODE=SENSER_SSX_UP;					// 酸水箱废水满或酸水箱位置异常
      }
    }	
		Senser_Time_SSXD=0;
  }			

	return keycode;
}


/***********************************************************************************
 * 函数名：void RF_Read_Info(uint8_t step)
 * 描述  ：设置模式
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
void RF_Read_Info(uint8_t *step)
{
	uint8_t data[4]={0,0,0,0};
	uint8_t i=0;
	// 读取数据
	switch(*step)
	{
		case 1:	// 读取卡片ID
						EM4305_Read_Data(1);
						if(RF_Read_Num++>=9)
						{
							RF_Read_Num=9;
							LED_LX=0;Filter_Life=0;
							Filter_Ready_Flage=0;
						}
						break;
		case 2:	// 判断卡片ID是否用过
						// 如果滤芯注册过跳转到STEP=8
						// 如果滤芯已经
						*step=3;
						break;
		case 3:	// 读取初始化标志 UID^0x55AA55AA
						EM4305_Read_Data(3);
						break;
		case 4:	// 判断初始化状态
						*step=5;
						break;
		case 5: // 读取滤芯厂商DBX
						EM4305_Read_Data(5); 
						break;
		case 6:	// 读取滤芯规格
						EM4305_Read_Data(6); 
						break;
		case 7:	// 读取适用产品类型
						EM4305_Read_Data(7); 
						break;
		case 8: // 读取滤芯寿命A	
						EM4305_Read_Data(8); 
						break;
		case 9:	// 读取滤芯寿命B
						EM4305_Read_Data(9); 
						break;
		case 10:	// 判断是否需要写卡
						if(Filter_Count>=1000)
						{
							Filter_Count=0;
							if(Filter_Life>0)
							{
								data[0]=(Filter_Life-1)/256;
								data[1]=(Filter_Life-1)%256;
								data[2]=data[0]^0xFF;
								data[3]=data[1]^0xFF;
								EM4305_Write_Data(8+Write_RFID_Bit,data);			// 循环读写8 9两个扇区			
								if(Write_RFID_Bit==1) Write_RFID_Bit=0;				// 
								else Write_RFID_Bit=1;												// 
							}
							// 保存滤芯寿命到主板
							data[1]=(uint16_t)Filter_Life/10;
							data[0]=(uint16_t)Filter_Life%10;								
							for(i=0;i<2;i++)																// 保存当前滤芯到设备	
							{
								EEP_Write_Date(FILTER_LIFT_ADDR+i,data[i]);
							}			
						}
						else *step=1;																			// 重新读取ID
						break;
		default : *step=1;
							break;
	}
}

void EM4305_Get_NewFAX(uint8_t *step)
{
	uint8_t err=0,num=0,i=0;
	uint8_t data[2];
	uint8_t ecccode[4]={0x55,0xAA,0x55,0xAA}; 			// 异或码
	static uint8_t id_buf[4]={0x00,0x00,0x00,0x00}; // ID码缓存
	static uint8_t first_read=0,read_num=0;					// 均衡读写
	static uint16_t read_life=0;										// 
	static uint8_t new_filter=0;										// 新滤芯标志
	static uint8_t write_filter_flage=0;						// 保存滤芯寿命标志
	static uint8_t test_flage=2;										// 测试滤芯接入
	
	if(Usart3_NewFax[0]!=0xAA) 
	{
		return;
	}
	if(Usart3_NewFax[1]!=0x0A) 
	{
		return;
	}
	num=Usart3_NewFax[2]+4;
	if(Usart3_NewFax[num]!=0xBB)
	{
		return;
	}
	// 校验
	for(i=1;i<num;i++)
	{
		err^=Usart3_NewFax[i];
	}
	if(err!=0) 
	{
		return;
	}
	// 判断是否读写成功 该位为0则异常
	if(Usart3_NewFax[3]!=0)  						// 读取失败
	{
		return;
	}
	RF_Read_Num=0;											// 清除读检测次数标志
  Filter_Ready_Flage=1;								// 检测到滤芯
	// 读取数据
	switch(*step)
	{
		case 1:	// 读取卡片ID
						for(i=0;i<4;i++)										  // 读ID
						{
							if(Usart3_NewFax[4+i]!=RF_ID_Code[i])  break;
						}
						// 判断滤芯ID
						if(i<4) 														  // 通过对比确认是新滤芯
						{
							new_filter=1;												// 新滤芯
							if(read_num++>=3)									  // 连续读取3次异常才记录
							{
								*step=3;												  // 验证识别码是否正确
							}
						}
						else																	// 不是新滤芯
						{
							 read_num=0;												// 复位计数器
							 new_filter=0;											// 旧滤芯
							 if(test_flage==2)									// 测试滤芯是否为测试滤芯
							 {
								 *step=3;													// 继续读取验证码
							 }
							 else if(test_flage==1) 						// 该滤芯为测试滤芯
							 {
								 *step=1;													// 循环读取滤芯ID
								 Filter_Life=10;									// 调整滤芯寿命
							   New_Filter_Flage=0;							// 新滤芯标记
							   LED_LX=2;												// 测试滤芯							 
							 }
							 else 															// 读取滤芯寿命
							 {	
								 *step=8;													// 读取寿命
							 }
						}
						// 保存ID
						for(i=0;i<4;i++)											// 保存ID 
						{
							id_buf[i]=Usart3_NewFax[4+i];				// 获取ID
						}
						break;
		case 2:	// 判断卡片ID是否用过
						// 如果滤芯注册过跳转到STEP=8
						// 如果滤芯已经
						break;
		case 3:	// 读取初始化标志 UID^0x55AA55AA
						for(i=0;i<4;i++)
						{
							if(Usart3_NewFax[4+i]!=(id_buf[i]^ecccode[i]))  break;
						}
						// 判断滤芯ID
						if(i<4) 																// 新滤芯异常
						{
							*step=1;															// 继续读取滤芯ID
						}
						else																		// 滤芯通过验证
						{
							*step=4;															// 新滤芯通过检验
							if(new_filter==1)											// 新滤芯
							{
								// 保存滤芯ID
								for(i=0;i<4;i++)										// 保存该ID
								{
									RF_ID_Code[i]=id_buf[i];
									// 保存当前滤芯到设备	
									EEP_Write_Date( RFID_CODE_ADDR+i,id_buf[i]);
								}							
								// 保存新滤芯标志 0Xaa 需要清洗
								EEP_Write_Date( CLERA_FLAGE_ADDR,0xAA);
								// 保存当前滤芯到设备	
								Set_Speak_Value(3);							 		// 提示音
								new_filter=0;												// 已经识别为新滤芯
								first_read=0;												// 首次读写用于均衡读写
								New_Filter_Flage=1;							 		// 新滤芯标记
								Clear_Count=0;									 		// 清除反洗水量计数
								Filter_Count=0;									 		// 清除滤芯水量计数
								read_num=0;						  				 		// 清除新滤芯读取防止错误计数										
							}
						}
						break;
		case 4:	// 判断初始化状态
						break;
		case 5: // 读取滤芯厂商DBX
						*step=6;					
						break;
		case 6:	// 读取滤芯规格 活性炭滤芯CC
						if(Usart3_NewFax[4]==0x43&&Usart3_NewFax[5]==0x43)				// 该滤芯为活性炭滤芯	
						{
							 *step=8;	
							 test_flage=0;																					// 非测试滤芯
						}
						else if(Usart3_NewFax[4]==0x54&&Usart3_NewFax[5]==0x54)		// 该滤芯为测试滤芯
						{
							test_flage=1; Filter_Life=10;														// 调整滤芯寿命
							New_Filter_Flage=0;																			// 新滤芯标记
							LED_LX=2;																								// 测试滤芯
							*step=1;																								// 重新读取滤芯ID
						}
						break;
		case 7:	// 读取使用产品类型
						*step=8;	
						break;
		case 8: // 读取滤芯寿命A 4567
						if((Usart3_NewFax[4]^Usart3_NewFax[6])==0xFF&&(Usart3_NewFax[5]^Usart3_NewFax[7])==0xFF)
						{
							read_life=Usart3_NewFax[4]*256+Usart3_NewFax[5];
							first_read=1;
						}
						else		// 校验错误
						{
							first_read=0;
						}
						*step=9;																									// 读取滤芯寿命寄存器B
						break;
		case 9:	// 读取滤芯寿命B 4567
						if((Usart3_NewFax[4]^Usart3_NewFax[6])==0xFF&&(Usart3_NewFax[5]^Usart3_NewFax[7])==0xFF)
						{
							if(first_read==1)
							{
								if(read_life>=Usart3_NewFax[4]*256+Usart3_NewFax[5])	// 以低数为准
								{
									read_life=Usart3_NewFax[4]*256+Usart3_NewFax[5];
								} 	
							}
						}
						// 校验完毕更新滤芯状态
						Filter_Life=read_life;	
						// 保存滤芯寿命
						if(write_filter_flage==0)
						{
							write_filter_flage=1;
							data[1]=(uint16_t)Filter_Life/10;
							data[0]=(uint16_t)Filter_Life%10;								
							for(i=0;i<2;i++)						// 保存当前滤芯到设备	
							{
								EEP_Write_Date(FILTER_LIFT_ADDR+i,data[i]);
							}							
						}
						// 判断滤芯状态
						if(Filter_Life>335) LED_LX=5;						// 滤芯三格
						else if(Filter_Life>166) LED_LX=4;			// 滤芯二格
						else if(Filter_Life>50) LED_LX=3;				// 滤芯一格
						else if(Filter_Life>5) LED_LX=2;				// 滤芯红灯
						else LED_LX=1;													// 滤芯红灯闪烁
						*step=10;																// 重新读取ID
						break;
		case 10:	// 写滤芯
						*step=1;
						break;
		default : *step=1;
							break;
	}
}


/***********************************************************************************
 * 函数名：uint8_t Work_In_Set(void)
 * 描述  ：设置模式
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
uint8_t Work_In_Set(void)
{
	static KEYMODE_TypeDef keymode;	      // 当前按键模式		
	static uint8_t set_temp=0;						// 设置温度使能
	static uint16_t waite_time=0;					// 加热等待延时
	uint8_t data[2],i;

	// Erro_Flage=0;							// 错误标志 0位 TDS异常   1位温度异常   2位原水空  3酸水满  
	// 检测传感器状态
	if(Yswater_Temp>MAX_YSTEMP)		// 原水高温
	{
		LED_GW=1;
		Erro_Flage|=0x02;						// 错误报警
	}
	else
	{
		LED_GW=0;
		Erro_Flage&=0xFD;						// 取消错误报警
	}
	// 
	if(Senser_Mode.SENSER_YSX_MODE==SENSER_YSX_UP)
	{
		LED_QS=1;										// 原水箱缺水报警指示灯点亮
		Erro_Flage|=0x04;						// 错误报警
	}
	else
	{
		LED_QS=0;										// 原水箱缺水报警指示熄灭
		Erro_Flage&=0xFB;						// 取消错误报警
	}
	if(Senser_Mode.SENSER_SSX_MODE==SENSER_SSX_UP)
	{
		LED_MS=1;										// 原水箱缺水报警指示灯点亮
		Erro_Flage|=0x08;						// 错误报警
	}
	else
	{
		LED_MS=0;										// 原水箱缺水报警指示熄灭
		Erro_Flage|=0xF7;						// 错误报警
	}	
	if(New_Filter_Flage==1)				// 新滤芯需要清洗
	{
		LED_QX=1;
		Set_Temp_Num=0;
		set_temp=0;
	}
	else
	{
		LED_QX=0;
	}
	// 休眠
	if(Standby_Time>=STANDBY_NUM)	 								// 进入休眠模式
	{
		set_temp=0;																	// 禁止设置温度
		Set_Temp_Num=0;															// 复位温度设定值
		Tds_Test_Flage=0;														// 停止测试TDS
	}
	// 调节电解强度
	if(keymode.KEY_PH_MODE!=Key_Now_Mode.KEY_PH_MODE)
	{
		keymode.KEY_PH_MODE=Key_Now_Mode.KEY_PH_MODE;
		if(Standby_Mode==0)													// 非待机模式下
		{
			if(Key_Now_Mode.KEY_PH_MODE==KEY_PH_DOWN)
			{
				if(Work_Step==0&&New_Filter_Flage==0&&LED_LX!=1)		// 未工作才能调节 
				{
					if(Set_PH_Num<3) Set_PH_Num++;
					else Set_PH_Num=0;			
				}
				else
				{
					Work_Step=11;																			// 停止工作
				}
				// 工况异常不能电解
				if(LED_LX<=1||LED_TDS==1||LED_GW==1)								// 水质异常不能电解
				{
					Set_PH_Num=0;Set_Temp_Num=0;
				}
				if(Set_PH_Num==0)
				{
					Display_Rama[1]=0x12;
					Display_Rama[0]=0x12;
					Display_Ramb[0]=0x12;
					Display_Ramb[1]=0x12;
					Display_Ramb[2]=0x12;	
				}
				else
				{
					Display_Rama[1]=Set_PH_Table[Set_PH_Num]/10;				// 显示温度
					Display_Rama[0]=Set_PH_Table[Set_PH_Num]%10;				// 显示温度
					Display_Ramb[0]=Set_ORP_Table[Set_PH_Num]/100;
					Display_Ramb[1]=(Set_ORP_Table[Set_PH_Num]%100)/10;
					Display_Ramb[2]=Set_ORP_Table[Set_PH_Num]%10;			
				}
				Set_Speak_Value(2);										// 蜂鸣器响			
			}
		}
		else Standby_Mode=0;
				
		// 检测是否上电初始化显示TDS值
		if(Key_Now_Mode.KEY_PH_MODE==KEY_PH_HOLD) 
		{
			if(Init_Falge==0) 											// 未初始化
			{
				Show_TDS_Value=1;											// 显示TDS
				Set_Speak_Value(3);										// 报警音
				Display_Rama[1]=0x00;									// 软件版本号
				Display_Rama[0]=0x02;									// 软件版本号
				Set_PH_Num=0;													// 显示强碱
			}
			else if(Show_TDS_Value==1)							// 开启显示TDS
			{
				Show_TDS_Value=0;											// 关闭显示TDS
				Set_Speak_Value(3);										// 报警音
			}
		}		
	}
	// 长按温度键解锁
	// 调节加热温度
	if(keymode.KEY_TEM_MODE!=Key_Now_Mode.KEY_TEM_MODE)
	{
		keymode.KEY_TEM_MODE=Key_Now_Mode.KEY_TEM_MODE;
		// 长按解锁
		if(Key_Now_Mode.KEY_TEM_MODE==KEY_TEM_HOLD)
		{
			if(set_temp==1)												// 温度已经解锁
			{
				set_temp=0;													// 锁定
				Set_Temp_Num=0;											// 复位温度显示
				Standby_Time=STANDBY_NUM;						// 待机锁定模式
			}
			else if(LED_LX!=1)										// 温度上锁
			{
				set_temp=1;													// 解锁
				Set_Temp_Num=1;											// 复位温度显示
			}
			Set_Speak_Value(2);													// 长按
		}
		// 短按调节温度
		if(Standby_Mode==0)														// 非待机模式下
		{
			if(Key_Now_Mode.KEY_TEM_MODE==KEY_TEM_DOWN)
			{
				if(Work_Step==0&&set_temp==1&&LED_LX!=1)	// 未工作才能调节
				{			
					if(Set_Temp_Num<3)	Set_Temp_Num++;			// 温度增加
					else Set_Temp_Num=0;										// 
				}
				else
				{
					Work_Step=11;														// 停止工作
				}
				Set_Speak_Value(2);
			}
		}
		else Standby_Mode=0;
		// 工况异常
		if(LED_LX<=1||LED_TDS==1||LED_GW==1)
		{
			Set_PH_Num=0;Set_Temp_Num=0;
		}
	}
	// 开始按键被按下
	if(keymode.KEY_START_MODE!=Key_Now_Mode.KEY_START_MODE)
	{
		keymode.KEY_START_MODE=Key_Now_Mode.KEY_START_MODE;
		if(Key_Now_Mode.KEY_START_MODE==KEY_START_DOWN)
		{
			if(Standby_Mode==0)												// 非待机模式下		
			{
				if(Work_Step==0) 												// 待机模式下才能操作
				{	
					Work_Step=1;	Total_Flow=0;						// 开始出水
					if(LED_TDS==1)												// TDS过高停止电解
					{
						Display_Rama[1]=0x12;								// TDS过高不显示不电解
						Display_Rama[0]=0x12;
						Display_Ramb[0]=0x12;
						Display_Ramb[1]=0x12;
						Display_Ramb[2]=0x12;							
					}
				}
				else {	Work_Step=11;	}									// 停止
			}
			else Standby_Mode=0;											// 进入待机模式
			Set_Speak_Value(2);			
			// 工况异常
			if(LED_LX<=1||LED_TDS==1||LED_GW==1)
			{
				Set_PH_Num=0;Set_Temp_Num=0;
			}
		}
	}
	// 工作模式
	switch(Work_Step)															// 工作
	{
		case 0:	// 待机模式
						JDQ_Heat(0);												// 停止发热体
						ELE_ConP(0);												// 链接到正极
						ELE_ConN(0);												// 链接到负极		
						VALUE_Out(0);												// 关闭电磁阀
						Cur_PID_Reset();										// 复位电机
						Motor_PID_Reset();									// 复位电机
						break;
		case 1: // 检测原水箱是否有水
						// 检测酸水箱水是否满了
						// 检测滤芯是否安装好
						// 原水水温低于5摄氏度不工作
						if((Senser_Mode.SENSER_YSX_MODE==SENSER_YSX_UP)||(Senser_Mode.SENSER_SSX_MODE==SENSER_SSX_UP)||(Filter_Ready_Flage==0)||(Yswater_Temp<5))
						{
							Work_Step=11;											// 异常跳转至停止
							Set_Speak_Value(3);								// 报警音
						}
						else
						{
							Work_Step=2;											// 跳转是否跳转到新滤芯
						}
						break;
		case 2:	// 检测是否刚换完滤芯
						// 如果更换新滤芯则进入清洗模式
						if(New_Filter_Flage==1&&LED_LX!=1)																 			// 刚换完滤芯  滤芯正常
						{
							Work_Step=7;											// 进入反洗
						}
						else
						{
							if(Set_Temp_Num==0)								// 常温水
							{
								Work_Step=5;										// 不加热
							}
							else															// 高温水
							{
								Work_Step=3;										// 跳转到加热
							}						
						}
						break;
		case 3:	// 启动发热体前检测水路中是否有水(通过TDS判断)
						if(Yswater_Tds<MAX_TDS_VALUE&&Yswater_Temp<MAX_YSTEMP&&LED_LX!=1)				// 水质正常 水温正常 滤芯正常
						{						
							Work_Step=4;											// 需要加热		
							// 2019-11-28 为了保护发热体 改为先出水后加热 去掉预热功能
							// JDQ_Heat(1);											// 启动发热体		
							// 2019-11-28 为了保护发热体 改为先出水后加热 去掉预热功能s
						}
						else																// 跳过加热
						{
							Work_Step=5;											// 水泵开始出水(条件异常)
						}						
						break;						
		case 4:	// 预热发热体温度高于预热温度才工作
						if(Set_Temp_Num==1&&Hmwater_Temp>20)
						{
							Work_Step=5;											// 水泵开始出水
						}
						if(Set_Temp_Num==2&&Hmwater_Temp>25)
						{
							Work_Step=5;											// 水泵开始出水
						}
						if(Set_Temp_Num==3&&Hmwater_Temp>30)
						{
							Work_Step=5;											// 水泵开始出水
						}	
						// 延时时间增加
						if(waite_time++>200)								// 预热时间不超过2S
						{
							Work_Step=5;											// 水泵开始出水
						}
						// 2019-11-28 为了保护发热体 改为先出水后加热 去掉预热功能 增加
						Work_Step=5;												// 水泵开始出水
						// 2019-11-28 为了保护发热体 改为先出水后加热 去掉预热功能
						break;	
		case 5: // 判断是否需要电解
						waite_time=0;												// 复位计时器
						if(Set_PH_Num==0)										// 无需电解  
						{
							Work_Step=8;											// 启动水泵
						}
						else																// 需要开启电解
						{	
							Work_Step=6;											// 跳转至开始电解
						}
						break;
		case 6:	// 开始电解
						if(Yswater_Tds<MAX_TDS_VALUE&&Yswater_Temp<MAX_YSTEMP&&LED_LX!=1)					// 水质正产  水温正产 滤芯正常
						{
							ELE_ConP(0);											// 链接到正极
							ELE_ConN(1);											// 链接到负极	
							delay_ms(10);											// 延时打开
							VALUE_Out(1);											// 打开电磁阀
							Start_Cur_PID(Set_PH_Num);				// 开始电解
						}
						Work_Step=8;												// 进入下一步骤
						break;												
		case 7:	// 判断是否能启动反洗
						if(Yswater_Tds<MAX_TDS_VALUE&&Yswater_Temp<MAX_YSTEMP&&LED_LX!=1)					// 水质正产  水温正产 滤芯正常
						{
							Set_PH_Num=2;											// 中碱
							ELE_ConP(1);											// 链接到正极		
							ELE_ConN(0);											// 链接到负极		
							delay_ms(10);											// 延时打开
							VALUE_Out(1);											// 打开电磁阀
							Start_Cur_PID(Set_PH_Num);				// 中碱档冲洗							
						}
						Work_Step=8;												// 进入下一步骤			
						break;										
		case 8:	// 启动水泵开始出水
						Start_Motor_PID(Set_Temp_Num);			// 启动电机调速控制水温		
						// 2019-11-28 为了保护发热体 改为先出水后加热 去掉预热功能
						// 延时时间增加
						if(waite_time++>200)								// 预热时间不超过2S
						{
							Work_Step=9;											// 水泵开始出水
						}		
						// 2019-11-28 为了保护发热体 改为先出水后加热 去掉预热功能
						// Work_Step=9;
						// 2019-11-28 为了保护发热体 改为先出水后加热 去掉预热功能
						break;
		case 9: // 当原水水温高 或者发热体温度过高则复位电机调速值
						if((Yswater_Temp>=MAX_YSTEMP||Hmwater_Temp>=90)&&(Set_Temp_Num!=0))				// 复位温度阈值
						{
							Reset_Motor_Value();							// 复位电机调速控制水温		
						}		
						// 2019-11-28 为了保护发热体 改为先出水后加热 去掉预热功能
						// 判断是否需要启动发热体
						waite_time=0;												// 复位计时器
						if(Set_Temp_Num!=0&&Yswater_Tds<MAX_TDS_VALUE&&Yswater_Temp<MAX_YSTEMP&&LED_LX!=1)				// 需要启动发热体  水质正常 水温正常 滤芯正常
						{
							JDQ_Heat(1);											// 启动发热体	
						}
						// 2019-11-28 为了保护发热体 改为先出水后加热 去掉预热功能
						Work_Step=10;												// 出水
						break;
		case 10:// 出水中等待停止操作
						// 检测酸水箱水是否满了
						if((Senser_Mode.SENSER_YSX_MODE==SENSER_YSX_UP)||(Senser_Mode.SENSER_SSX_MODE==SENSER_SSX_UP))
						{
							Work_Step=11;
							Set_Speak_Value(3);																	// 报警
						}	
						// 统计水流速度 计算流量值
						if(Curent_Flage==1)																		// 流量定时检测标志
						{
							Curent_Flage=0;																			// 清零
							// 2019-11-28 修改水量统计  定义WATER_SPEED参数在头文件中修改为25.0
							Now_Current_Speed=WATER_SPEED*(PUM2L_OUTPUT/4000.0);// 电机出水量 1750ml - 2000ml每分钟测算  1750/60=29   2000/60=33
							Filter_Count+=Now_Current_Speed;										// 滤芯剩余升数计时
							Total_Flow+=Now_Current_Speed;											// 单次最大出水量
							// 判断如果是反洗需要多少水
							if(New_Filter_Flage==1)															// 新滤芯反洗中
							{
								Clear_Count+=Now_Current_Speed;										// 反洗用水计数
								if(Clear_Count>=MAX_FANXI_VALUE)									// 最大反洗水数
								{
									Work_Step=11;																		// 
									New_Filter_Flage=0;															//	
									Clear_Count=0;																	//			
									// 保存新滤芯标志 0x00结束清洗
									EEP_Write_Date( CLERA_FLAGE_ADDR,0x00);					//				
								}
							}		
							// 最大单次出水不超过2L
							if(Total_Flow>MAX_TOTAL_FLOW)												// 单次出水最大值	
							{
								Work_Step=11;Total_Flow=0;												// 停止
							}		
							// 开始检测TDS
							if(Total_Flow>TEST_TDS_VALUE)												// 检测TDS阈值
							{
								Tds_Test_Flage=1;																	// 开始检测TDS
							}
							// 滤芯异常状态
							if(LED_LX<=1)																				// 滤芯为空
							{
								JDQ_Heat(0);																			// 停止发热体
								ELE_ConP(0);																			// 链接到正极
								ELE_ConN(0);																			// 链接到负极		
								Set_PH_Num=0;																			// 关闭
								Set_Temp_Num=0;																		// 关闭
							}
						}
						break;
		case 11:	// 手动停止 停止电解及水泵		 				
						JDQ_Heat(0);							// 停止发热体
						Motor_PID_Reset();				// 复位电机
						Cur_PID_Reset();					// 复位电解电流
						Tds_Test_Flage=0;					// 停止检测TDS
						delay_ms(10);							// 延时关闭
						ELE_ConP(0);							// 链接到正极
						ELE_ConN(0);							// 链接到负极	
						VALUE_Out(0);							// 关闭电磁阀
						Work_Step=12;							// 跳转		
						break;
		case 12:	// 停止工作恢复显示
						if(Set_PH_Num==0)					// 更新RAM
						{
							Display_Rama[1]=0x12;
							Display_Rama[0]=0x12;
							Display_Ramb[0]=0x12;
							Display_Ramb[1]=0x12;
							Display_Ramb[2]=0x12;	
						}
						else
						{

							Display_Ramb[0]=Set_ORP_Table[Set_PH_Num]/100;
							Display_Ramb[1]=(Set_ORP_Table[Set_PH_Num]%100)/10;
							Display_Ramb[2]=Set_ORP_Table[Set_PH_Num]%10;			
						}
						// 保存单次用水量到存储器
						if(Filter_Count<1000)
						{
							data[1]=(uint16_t)Filter_Count/10;
							data[0]=(uint16_t)Filter_Count%10;	
							// 保存当前滤芯到设备		
							for(i=0;i<2;i++)
							{
								// 保存当前滤芯到设备	
								EEP_Write_Date(RFID_CURRENT_SPEED+i,data[i]);
							}												
						}	
						// 水质异常 
						if(LED_LX<=1||LED_TDS==1||LED_GW==1)			// 水质异常不能电解
						{
							Set_PH_Num=0;Set_Temp_Num=0;						// 复位
						}
						Work_Step=0;								// 跳转	
						break;		
		default : // 故障异常值
						Cur_PID_Reset();
						Motor_PID_Reset();
						Work_Step=11;
						break;
	}	

	return 0;
}


/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/
