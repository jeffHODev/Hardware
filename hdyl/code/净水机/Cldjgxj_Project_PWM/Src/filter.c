/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "system.h"
#include "filter.h"

// Page 0 : 0x46 0x44 0x42 0x58   FDBX
// Page 1 : UID
// Page 2 : PASSWORD
// Page 3 : OPT
// Page 4 :	Config
// Page 5 :	2018				年
// Page 6 :	1021				月日
// Page 7 :	0000				序列号H
// Page 8 :	0001				序列号L
// Page 9 :	Config		  流量1	
// Page 10:	Config			流量2
// Page 11:	Config			流量3
// Page 12:	Config			流量4
// Page 13:	Config			0000 空 FFFF 废
// Page 14:	RP
// Page 15:	RP

uint16_t PP_Filter_Life=0;							// 6个月 24*30*6
uint16_t C1_Filter_Life=0;							// 12个月
uint16_t UDF_Filter_Life=0;							// 24个月
uint16_t C2_Filter_Life=0;							// 12个月

static uint8_t PP_RFID_Setp=1;					// 读取ID步骤 需要从1开始启动
static uint8_t C1_RFID_Setp=1;					// 读取ID步骤
static uint8_t UDF_RFID_Setp=1;					// 读取ID步骤
static uint8_t C2_RFID_Setp=1;					// 读取ID步骤

static uint8_t PP_Read_Fail_Num=0;			// 滤芯读取失败次数
static uint8_t C1_Read_Fail_Num=0;			// 滤芯读取失败次数
static uint8_t UDF_Read_Fail_Num=0;			// 滤芯读取失败次数
static uint8_t C2_Read_Fail_Num=0;			// 滤芯读取失败次数

static uint8_t PP_RF_ID_Code[4];				// 滤芯ID地址
static uint8_t C1_RF_ID_Code[4];				// 滤芯ID地址
static uint8_t UDF_RF_ID_Code[4];				// 滤芯ID地址
static uint8_t C2_RF_ID_Code[4];				// 滤芯ID地址

/***********滤芯写卡标志****************/
uint8_t PP_Write_Flage=0;								// PP棉滤芯写卡
uint8_t C1_Write_Flage=0;								// C1滤芯写卡
uint8_t UDF_Write_Flage=0;							// UDF芯写卡
uint8_t C2_Write_Flage=0;								// C2芯写卡

uint8_t Filter_Polling_Order=0;					// 轮芯轮询 0 PP滤芯  1 C1滤芯  2 UDF滤芯 3 C2滤芯

uint8_t Probe_PP=0;											// PP棉滤芯 剩余
uint8_t Probe_C1=0;											// C1滤芯
uint8_t Probe_UDF=0;										// UD滤芯
uint8_t Probe_C2=0;											// C2滤芯
uint8_t LED_PP=0;												// PP棉滤芯
uint8_t LED_C1=0;												// C1滤芯
uint8_t LED_UDF=0;											// UD滤芯
uint8_t LED_C2=0;												// C2滤芯

__IO uint8_t Read_Filter_Flage=0;				// 读取滤芯标志
/***********************************************************************************
 * 函数名：EM4305_Read_Data(uint8_t page,uint8_t data[4])
 * 描述  ：从卡中读取一页数据
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用
**********************************************************************************/
uint8_t EM4305_Read_Data(uint8_t page)
{
	uint8_t buf[10];
	uint8_t i,err=0,temp;
	
	if(page>15) page=0;
	// 填写数据
	buf[0]=0xAA;	buf[1]=0x0A;	buf[2]=0x02;	buf[3]=0x85;
	buf[4]=page;
	// 校验
	for(i=1;i<5;i++)
	{
		err^=buf[i];
	}
	buf[5]=err; buf[6]=0xBB;
	// 发送数据
	temp=Uart1_SendData(&huart1,buf,7);
	if(temp!=1)
	{
		HAL_UART_DeInit(&huart1);
		MX_USART1_UART_Init();
	}
	
	return temp;
}
/***********************************************************************************
 * 函数名：EM4305_Read_Data(uint8_t page,uint8_t data[4])
 * 描述  ：从卡中读取一页数据
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用 AA 0A 06 84 05 12 34 56 78 85 BB
**********************************************************************************/
uint8_t EM4305_Write_Data(uint8_t page,uint8_t data[4])
{
	uint8_t buf[15];
	uint8_t i,err=0,temp;

	if(page>15) page=0;
	// 填写数据
	buf[0]=0xAA;	buf[1]=0x0A;	buf[2]=0x06;	buf[3]=0x84;
	buf[4]=page; 	buf[5]=data[0];	buf[6]=data[1];	buf[7]=data[2];	buf[8]=data[3];
	// 校验
	for(i=1;i<9;i++)
	{
		err^=buf[i];
	}
	buf[9]=err; buf[10]=0xBB;
	// 发送数据
	temp=Uart1_SendData(&huart1,buf,11);
	if(temp!=1)
	{
		HAL_UART_DeInit(&huart1);
		MX_USART1_UART_Init();
	}

	return temp;
}

/***********************************************************************************
 * 函数名：void RF_Read_Info(uint8_t step)
 * 描述  ：设置模式
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
void RFID_Read_Info(uint8_t *step)
{
	uint8_t data[4]={0,0,0,0};							// 缓存
	static uint8_t PP_Balanced_Bit=0;				// 均衡写卡 0-1
	static uint8_t C1_Balanced_Bit=0;				// 均衡写卡 0-1
	static uint8_t UDF_Balanced_Bit=0;			// 均衡写卡 0-1
	static uint8_t C2_Balanced_Bit=0;				// 均衡写卡 0-1
	// 读取数据
	switch(*step)
	{
		case 1:	// 读取卡片ID
						EM4305_Read_Data(1);
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
		case 10:// 判断是否需要写卡																				// 重新读取ID
						switch(Filter_Polling_Order)															// 循环读写
						{
							case 0:	// PP
											if(PP_Filter_Life>0)														// 如果滤芯寿命正常
											{
												data[0]=(PP_Filter_Life-1)/256;								// 数据区
												data[1]=(PP_Filter_Life-1)%256;								// 数据区
												data[2]=data[0]^0xFF;													// 校验区
												data[3]=data[1]^0xFF;													// 校验区
												EM4305_Write_Data(8+PP_Balanced_Bit,data);		// 循环读写8 9两个扇区			
												if(PP_Balanced_Bit==1) PP_Balanced_Bit=0;			// 循环读写位
												else PP_Balanced_Bit=1;												// 循环读写位
											}		
											PP_Write_Flage=0;																// 已经完成写卡
											break;
							case 1:	// C1
											if(C1_Filter_Life>0)														// 如果滤芯寿命正常
											{
												data[0]=(C1_Filter_Life-1)/256;								// 数据区
												data[1]=(C1_Filter_Life-1)%256;								// 数据区
												data[2]=data[0]^0xFF;													// 校验区
												data[3]=data[1]^0xFF;													// 校验区
												EM4305_Write_Data(8+C1_Balanced_Bit,data);		// 循环读写8 9两个扇区			
												if(C1_Balanced_Bit==1) C1_Balanced_Bit=0;			// 循环读写位
												else C1_Balanced_Bit=1;												// 循环读写位
											}		
											C1_Write_Flage=0;																// 已经完成写卡
											break;
							case 2:	// UDF
											if(UDF_Filter_Life>0)														// 如果滤芯寿命正常
											{
												data[0]=(UDF_Filter_Life-1)/256;							// 数据区
												data[1]=(UDF_Filter_Life-1)%256;							// 数据区
												data[2]=data[0]^0xFF;													// 校验区
												data[3]=data[1]^0xFF;													// 校验区
												EM4305_Write_Data(8+UDF_Balanced_Bit,data);		// 循环读写8 9两个扇区			
												if(UDF_Balanced_Bit==1) UDF_Balanced_Bit=0;		// 循环读写位
												else UDF_Balanced_Bit=1;											// 循环读写位
											}			
											UDF_Write_Flage=0;															// 已经完成写卡
											break;
							case 3:	// C2
											if(C2_Filter_Life>0)														// 如果滤芯寿命正常
											{
												data[0]=(C2_Filter_Life-1)/256;								// 数据区
												data[1]=(C2_Filter_Life-1)%256;								// 数据区
												data[2]=data[0]^0xFF;													// 校验区
												data[3]=data[1]^0xFF;													// 校验区
												EM4305_Write_Data(8+C2_Balanced_Bit,data);		// 循环读写8 9两个扇区			
												if(C2_Balanced_Bit==1) C2_Balanced_Bit=0;			// 循环读写位
												else C2_Balanced_Bit=1;												// 循环读写位
											}	
											C2_Write_Flage=0;																// 已经完成写卡
											break;
							default : break;							
						}				
						
						break;
		default : *step=1;
							break;
	}
}


/***********************************************************************************
 * 函数名：EM4305_Get_NewFAX(uint8_t *step)
 * 描述  ：用户设置模式 
 * 输入  ：无
 * 输出  ：无 0 未读取到  0xCC写卡成功   0xEE滤芯读取异常 0xAA滤芯正常读取寿命
************************************************************************************/
uint8_t EM4305_Get_NewFAX(uint8_t *step)					// 
{
	uint8_t err=0,num=0,i=0;
	uint8_t ecccode[4]={0x55,0xAA,0x55,0xAA}; 			// 异或码
	static uint8_t id_buf[4]={0x00,0x00,0x00,0x00}; // ID码缓存
	static double ratio=0.0,testvalue,setvalue;												//
	static uint16_t read_life=0;										// 读取滤芯寿命
	
	if(Usart1_NewFax[0]!=0xAA) 											// 校验
	{
		return 0;
	}
	if(Usart1_NewFax[1]!=0x0A) 											// 校验
	{
		return 0;
	}
	num=Usart1_NewFax[2]+4;													// 计算结束符位置
	if(Usart1_NewFax[num]!=0xBB)										// 判断结束符
	{
		return 0;
	}
	// 校验
	for(i=1;i<num;i++)															// 校验
	{
		err^=Usart1_NewFax[i];
	}
	if(err!=0) 
	{
		return 0;
	}
	// 判断是否读写成功 该位为0则异常
	if(Usart1_NewFax[3]!=0)  												// 读取失败
	{
		return 0;																			//
	}
	// 读取数据
	switch(*step)
	{
		case 1:	// 读取卡片ID
						switch(Filter_Polling_Order)									// 寻新滤芯
						{
							case 0:// PP滤芯
										for(i=0;i<4;i++)										  // 读ID
										{
											if(Usart1_NewFax[4+i]!=PP_RF_ID_Code[i])  break;
										}													
										break;
							case 1:// C1滤芯
										for(i=0;i<4;i++)										  // 读ID
										{
											if(Usart1_NewFax[4+i]!=C1_RF_ID_Code[i])  break;
										}									
										break;
							case 2:// UDF滤芯
										for(i=0;i<4;i++)										  // 读ID
										{
											if(Usart1_NewFax[4+i]!=UDF_RF_ID_Code[i])  break;
										}									
										break;
							case 3:// C2滤芯
										for(i=0;i<4;i++)										  // 读ID
										{
											if(Usart1_NewFax[4+i]!=C2_RF_ID_Code[i])  break;
										}									
										break;
							default : break;							
						}
						// 判断滤芯ID
						if(i<4) 														 				 	// 通过对比确认是新滤芯
						{						
							for(i=0;i<4;i++)														// 保存ID 
							{
								id_buf[i]=Usart1_NewFax[4+i];							// 获取ID
							}	
							*step=3;		// 判断初始化标志是否正确							
						}
						else					// 不是新滤芯
						{			
							 *step=8;		// 旧滤芯读取寿命
						}						
						break;
		case 2:	// 判断卡片ID是否用过
						// 如果滤芯注册过跳转到STEP=8
						// 如果滤芯已经
						break;
		case 3:	// 读取初始化标志 UID^0x55AA55AA
						for(i=0;i<4;i++)												// 读取滤芯初始化标志
						{
							if(Usart1_NewFax[4+i]!=(id_buf[i]^ecccode[i]))  break;
						}
						// 判断滤芯ID
						if(i<4) 																// 新滤芯异常非官方认证
						{
							*step=1;															// 继续读取滤芯ID
							 return 0xEE;													// 读取结束
						}
						else																		// 滤芯通过验证
						{
							*step=6;															// 新滤芯通过检验
						}
						break;
		case 4:	// 判断初始化状态
						*step=5;
						break;
		case 5: // 读取滤芯厂商DBX
						*step=6;
						break;
		case 6:	// 读取滤芯规格 活性炭滤芯CC
						switch(Filter_Polling_Order)						// 新滤芯保存滤芯ID
						{
							case 0:// PP滤芯
										if(Usart1_NewFax[4]==0x50&&Usart1_NewFax[5]==0x50)				// 该滤芯为活性炭滤芯	
										{
											*step=8;	Probe_PP=1;																		// 读取滤芯寿命 探测到PP
											for(i=0;i<4;i++)																				// 保存该ID
											{
												PP_RF_ID_Code[i]=id_buf[i];														// 注册ID	
											}
										}	
										else	{	*step=1;	return 0xEE;	}													// 滤芯注册失败
										break;
							case 1:// C1滤芯
										if(Usart1_NewFax[4]==0x43&&Usart1_NewFax[5]==0x31)				// 该滤芯为活性炭滤芯	
										{
											*step=8;	Probe_C1=1;																		// 读取滤芯寿命and探测到C1																																				
											for(i=0;i<4;i++)																				// 保存该ID
											{
												C1_RF_ID_Code[i]=id_buf[i];														// 注册ID	
											}																						
										}			
										else	{	*step=1;	return 0xEE;	}													// 滤芯注册失败
										break;
							case 2:// UDF滤芯
										if(Usart1_NewFax[4]==0x52&&Usart1_NewFax[5]==0x4F)				// 该滤芯为活性炭滤芯	
										{
											*step=8;	Probe_UDF=1;																	// 读取滤芯寿命		探测到UDF							
											for(i=0;i<4;i++)																				// 保存该ID
											{
												UDF_RF_ID_Code[i]=id_buf[i];													// 注册ID	
											}								
										}		
										else	{	*step=1;	return 0xEE;	}													// 滤芯注册失败
										break;
							case 3:// C2滤芯
										if(Usart1_NewFax[4]==0x43&&Usart1_NewFax[5]==0x32)				// 该滤芯为活性炭滤芯	
										{
											*step=8;	Probe_C2=1;																		// 读取滤芯寿命		
											for(i=0;i<4;i++)																				// 保存该ID
											{
												C2_RF_ID_Code[i]=id_buf[i];														// 注册ID	
											}									
										}		
										else	{	*step=1;	return 0xEE;	}													// 滤芯注册失败
										break;
							default : break;							
						}			
						break;
		case 7:	// 读取使用产品类型
						*step=8;																													// 
						break;
		case 8: // 读取滤芯寿命A 4567
						if((Usart1_NewFax[4]^Usart1_NewFax[6])==0xFF&&(Usart1_NewFax[5]^Usart1_NewFax[7])==0xFF)
						{
							read_life=Usart1_NewFax[4]*256+Usart1_NewFax[5];								//
						}
						*step=9;																													// 读取滤芯寿命寄存器B
						break;
		case 9:	// 读取滤芯寿命B 4567
						if((Usart1_NewFax[4]^Usart1_NewFax[6])==0xFF&&(Usart1_NewFax[5]^Usart1_NewFax[7])==0xFF)
						{
							if(read_life>=Usart1_NewFax[4]*256+Usart1_NewFax[5])						// 以低数为准
							{
								read_life=Usart1_NewFax[4]*256+Usart1_NewFax[5];							// 取低数
							} 	
						}
						// 滤芯每小时写卡一次
						if(Hour_Flage!=0)
						{
							Hour_Flage=0;
							PP_Write_Flage=1;
							C1_Write_Flage=1;
							UDF_Write_Flage=1;
							C2_Write_Flage=1;
						}						
						// 判断是否需要写卡
						switch(Filter_Polling_Order)																			// 轮询滤芯
						{
							case 0:	// PP
											Probe_PP=1;
											PP_Filter_Life=read_life;																//
											testvalue=PP_Filter_Life;
											setvalue=PP_FILTER_TIME;
											ratio=testvalue/setvalue;																// 
											if(ratio>1.0/setvalue)LED_PP=(uint8_t)(ratio*6+1);			// 只要滤芯有寿命则显示有一格
											else LED_PP=0;																					// 耗尽									
											if(PP_Write_Flage!=RESET)	{	*step=10;	}									// 写卡标志
											else	{	*step=1;	return 0xAA;}													// 重新读取ID
											break;
							case 1:	// C1
											Probe_C1=1;
											C1_Filter_Life=read_life;																//
											testvalue=C1_Filter_Life;
											setvalue=C1_FILTER_TIME;
											ratio=testvalue/setvalue;																// 
											if(ratio>1.0/setvalue)LED_C1=(uint8_t)(ratio*6+1);			// 只要滤芯有寿命则显示有一格
											else LED_C1=0;																					// 耗尽									
											if(C1_Write_Flage!=RESET)	{	*step=10;	}									// 写卡标志
											else	{	*step=1;	return 0xAA;}													// 重写读取ID
											break;
							case 2:	// UDF
											Probe_UDF=1;
											UDF_Filter_Life=read_life;															//
											testvalue=UDF_Filter_Life;
											setvalue=UDF_FILTER_TIME;
											ratio=testvalue/setvalue;																// 
											if(ratio>1.0/setvalue)LED_UDF=(uint8_t)(ratio*6+1);			// 只要滤芯有寿命则显示有一格
											else LED_UDF=0;																					// 耗尽										
											if(UDF_Write_Flage!=RESET)	{	*step=10;	}								// 写卡标志
											else	{	*step=1;	return 0xAA;}													// 重写读取ID									
											break;
							case 3:	// C2
											Probe_C2=1;
											C2_Filter_Life=read_life;																//
											testvalue=C2_Filter_Life;
											setvalue=C2_FILTER_TIME;
											ratio=testvalue/setvalue;																// 
											if(ratio>1.0/setvalue)LED_C2=(uint8_t)(ratio*6+1);			// 只要滤芯有寿命则显示有一格
											else LED_C2=0;																					// 耗尽										
											if(C2_Write_Flage!=RESET)	{	*step=10;	}									// 写卡标志
											else	{	*step=1;	return 0xAA;}													// 重写读取ID											
											break;
							default : break;					
						}		
						break;
		case 10:	// 写滤芯
						*step=1;																													// 复位	
						return 0xCC;
//						break;
		default : *step=1;
							break;
	}
	// 读取异常
	return 0x01;																																// 非认证滤芯退出
}


/***********************************************************************************
 * 函数名：void System_Read_Filter(void)
 * 描述  ：从卡中读取一页数据
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用 AA 0A 06 84 05 12 34 56 78 85 BB
**********************************************************************************/
void System_Read_Filter(void)																//
{
	static uint8_t last_filter,now_filter;
	static uint8_t time;
	
	// 以下程序防止模块损坏无法读取其他滤芯状态
	now_filter=Filter_Polling_Order;													// 更新当前滤芯号
	if(last_filter!=now_filter)																// 当前滤芯和上次滤芯不一致
	{
		last_filter=now_filter;																	// 更新上次滤芯号
		time=0;																									// 重新计数
	}
	else																											// 当前滤芯和上次滤芯号一致
	{
		if(time++>MAX_RETRY_FILTER_NUM)													// 连续50次卡在当前滤芯
		{
			switch(Filter_Polling_Order)													// 滤芯探测
			{
				case 0:	// 获取PP滤芯参数
								Probe_PP=0;LED_PP=0;												// PP探测失败
								PP_Filter_Life=0;														// PP滤芯寿命清零
								Filter_Polling_Order=1;											// 读取下一个滤芯
								break;
				case 1:	// 获取C1滤芯参数
								Probe_C1=0;LED_C1=0;												// C1探测失败
								C1_Filter_Life=0;														// C1滤芯寿命清零					
								Filter_Polling_Order=2;
								break;
				case 2:	// 获取UDF
								Probe_UDF=0;LED_UDF=0;											// UDF探测失败
								UDF_Filter_Life=0;													// UDF滤芯寿命清零					
								Filter_Polling_Order=3;
								break;
				case 3:	// 获取C2
								Probe_C2=0;LED_C2=0;												// C2探测失败
								C2_Filter_Life=0;														// C2滤芯寿命清零							
								Filter_Polling_Order=0;
								break;
				default : Filter_Polling_Order=0; 
								break;		
			}
			time=0;																								// 重新计数
		}		
	}
	// 滤芯发送读取命令
	switch(Filter_Polling_Order)															//
	{
		case 0:	// 获取PP滤芯参数
						RF_A_SET(1);RF_B_SET(1);												//
		        HAL_Delay(100);
						RFID_Read_Info(&PP_RFID_Setp);									//					
						break;
		case 1:	// 等待C1滤芯数据		
						RF_A_SET(0);RF_B_SET(1);	 
		        HAL_Delay(100);											//
						RFID_Read_Info(&C1_RFID_Setp);									//
						break;
		case 2:	// 等待UDF滤芯数据	
						RF_A_SET(1);RF_B_SET(0); 
		        HAL_Delay(100);												//
						RFID_Read_Info(&UDF_RFID_Setp);									//
						break;
		case 3:	// 获取C2滤芯参数
						RF_A_SET(0);RF_B_SET(0);		
         		HAL_Delay(100);										// 
						RFID_Read_Info(&C2_RFID_Setp);									//
						break;	
		default : Filter_Polling_Order=0;
							break;
	}
}

/***********************************************************************************
 * 函数名：void System_Read_Fax(void)
 * 描述  ：从卡中读取一页数据
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用 AA 0A 06 84 05 12 34 56 78 85 BB
**********************************************************************************/
void System_Read_Fax(void)
{
	uint8_t station=0xFF;
	
	// 获取到1帧数据
	if(Usart1_Ready_Flage!=RESET)										// 读取到数据
	{
		Usart1_Ready_Flage=0;													// 清除标志
	}
	else																						// 串口未收到有效数据
	{
		return;																				// 退出该程序
	}
	// 解析
	switch(Filter_Polling_Order)
	{
		case 0x00:// 当前为PP滤芯
							station=EM4305_Get_NewFAX(&PP_RFID_Setp);			// 探测							
							if(station==0)											// 未正确获取ID信息(ID卡异常)
							{
								if(PP_Read_Fail_Num++>MAX_READ_FILTER_NUM)
								{
									Probe_PP=0;LED_PP=0;						// PP探测失败
									PP_Filter_Life=0;								// PP滤芯寿命清零					
									Filter_Polling_Order=1;					// 读取下一个
								}
							}
							if(station==0xEE)										// 滤芯状态异常
							{
								Probe_PP=0;LED_PP=0;							// PP探测失败
								PP_Filter_Life=0;									// PP滤芯寿命清零
								Filter_Polling_Order=1;						// 读取下一个滤芯
							}							
							if(station==0xCC)										// 滤芯写卡正常
							{
								Filter_Polling_Order=1;						// 读取下一个滤芯
								PP_Read_Fail_Num=0;								//
							}																
							if(station==0xAA)										// 正确获取滤芯寿命
							{
								Filter_Polling_Order=1;						// 读取下一个
								PP_Read_Fail_Num=0;								//
							}
							break;
		case 0x01:// 当前为C1滤芯
							station=EM4305_Get_NewFAX(&C1_RFID_Setp);			//
							if(station==0)											// 未正确获取ID信息(ID卡异常)
							{
								if(C1_Read_Fail_Num++>MAX_READ_FILTER_NUM)
								{
									Probe_C1=0;LED_C1=0;						// C1探测失败
									C1_Filter_Life=0;								// C1滤芯寿命清零									
									Filter_Polling_Order=2;					// 读取下一个
								}
							}
							if(station==0xEE)										// 滤芯状态异常
							{
								Probe_C1=0;LED_C1=0;							// C1探测失败
								C1_Filter_Life=0;									// C1滤芯寿命清零
								Filter_Polling_Order=2;						// 读取下一个滤芯
							}							
							if(station==0xCC)										// 滤芯写卡正常
							{
								Filter_Polling_Order=2;						// 读取下一个滤芯
								C1_Read_Fail_Num=0;								//
							}																
							if(station==0xAA)										// 正确获取滤芯寿命
							{
								Filter_Polling_Order=2;						// 读取下一个
								C1_Read_Fail_Num=0;								//
							}			
							break;
		case 0x02:// 当前为UDF滤芯
							station=EM4305_Get_NewFAX(&UDF_RFID_Setp);		//
							if(station==0)											// 未正确获取ID信息(ID卡异常)
							{
								if(UDF_Read_Fail_Num++>MAX_READ_FILTER_NUM)
								{
									Probe_UDF=0;LED_UDF=0;					// UDF探测失败
									UDF_Filter_Life=0;							// UDF滤芯寿命清零									
									Filter_Polling_Order=3;					// 读取下一个
								}
							}
							if(station==0xEE)										// 滤芯状态异常
							{
								Probe_UDF=0;LED_UDF=0;						// UDF探测失败
								UDF_Filter_Life=0;								// UDF滤芯寿命清零
								Filter_Polling_Order=3;						// 读取下一个滤芯
							}							
							if(station==0xCC)										// 滤芯写卡正常
							{
								Filter_Polling_Order=3;						// 读取下一个滤芯
								UDF_Read_Fail_Num=0;							//
							}																
							if(station==0xAA)										// 正确获取滤芯寿命
							{
								Filter_Polling_Order=3;						// 读取下一个
								UDF_Read_Fail_Num=0;							//
							}				
							break;
		case 0x03:// 当前为C2滤芯
							station=EM4305_Get_NewFAX(&C2_RFID_Setp);			// 探测滤芯	
							if(station==0)											// 未正确获取ID信息(ID卡异常)
							{
								if(C2_Read_Fail_Num++>MAX_READ_FILTER_NUM)
								{
									Probe_C2=0;LED_C2=0;						// C2探测失败
									C2_Filter_Life=0;								// C2滤芯寿命清零									
									Filter_Polling_Order=0;					// 读取下一个
								}
							}
							if(station==0xEE)										// 滤芯状态异常
							{
								Probe_C2=0;LED_C2=0;							// C2探测失败
								C2_Filter_Life=0;									// C2滤芯寿命清零
								Filter_Polling_Order=0;						// 读取下一个滤芯
							}							
							if(station==0xCC)										// 滤芯写卡正常
							{
								Filter_Polling_Order=0;						// 读取下一个滤芯
								C2_Read_Fail_Num=0;								//
							}																
							if(station==0xAA)										// 正确获取滤芯寿命
							{
								Filter_Polling_Order=0;						// 读取下一个
								C2_Read_Fail_Num=0;								//
							}			
							break;
		default : // 异常
							Filter_Polling_Order=0x00;
							break;
	}
}



