/******************** (C) COPYRIGHT 2014 盛元电子工作室 **************************
* 文件名  ：ATRF.c
* 描述    ：主函数       
* 实验平台：STM32
* 库版本  ：ST3.0.0
* 作者    ：小奕
* 项目说明：ECG无线收发
**********************************************************************************/
/* 包含系统头文件 */
#include "mmr901mx.h"

extern SPI_HandleTypeDef hspi3;

/* 全局变量声明  */
uint16_t MMR_Calib_C1=0;			// 校正值 14bit
uint16_t MMR_Calib_C2=0;			// 校正值  6bit
uint16_t MMR_Calib_C3=0;			// 校正值	13bit
uint16_t MMR_Calib_C4=0;			// 校正值  9bit
uint16_t MMR_Calib_C5=0;			// 校正值  6bit
uint16_t Press_Zero=0;				// 压力参数相对零点
int32_t  Press_Deltat=0;			// 压力温度修正参数
int32_t  Temp_Value=0;				// 当前温度

static void delay_us(int n)
{
  while(n--)
  {
    __asm("nop");
  }
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
uint8_t SPI_SendByte(uint8_t byte)
{
  HAL_SPI_TransmitReceive(&hspi3, &byte, &byte, 1, 100);
  return byte;
}

/***********************************************************************************
* 函数名：uint8_t MMR_Read_EEPROM(uint8_t addr)
* 描述  ：读取EEPROM寄存器
* 输入  ：addr 将操作寄存器的地址 
* 输出  ：无
************************************************************************************/
uint8_t MMR_Read_EEPROM(uint8_t addr)
{
  uint8_t data,data1,data2;
  
  SPI_NSS(0);													// 使能C		
  delay_us(2);
  SPI_SendByte(READ_EEPROM+addr);			// 发送要写的寄存器地址
  data1=SPI_SendByte(Dummy_Byte);			// 读取数据
  data2=SPI_SendByte(Dummy_Byte);			// 读取数据
  data=(data1<<1)+(data2>>7);					// 合成数据
  SPI_NSS(1);													// 使能C	
  delay_us(2);
  return data;
}

/***********************************************************************************
* 函数名：uint8_t MMR_Read_EEPROM(uint8_t addr)
* 描述  ：读取EEPROM寄存器
* 输入  ：addr 将操作寄存器的地址 
* 输出  ：无
************************************************************************************/
uint8_t MMR_Load_EEPROM(uint8_t addr)
{
  uint8_t data,data1,data2;
  
  SPI_NSS(0);													// 使能C		
  delay_us(2);
  SPI_SendByte(READ_EEPROM+addr);			// 发送要写的寄存器地址
  data1=SPI_SendByte(Dummy_Byte);			// 读取数据
  data2=SPI_SendByte(Dummy_Byte);			// 读取数据
  data=(data1<<1)+(data2>>7);					// 合成数据
  SPI_NSS(1);													// 使能C	
  delay_us(2);
  return data;	
}

/***********************************************************************************
* 函数名：void MMR_Send_CMD(uint8_t cmd)
* 描述  ：发送命令
* 输入  ：无
* 输出  ：无
************************************************************************************/
void MMR_Send_CMD(uint8_t cmd)
{
  SPI_NSS(0);													// 使能C		
  delay_us(2);
  SPI_SendByte(cmd);									// 发送命令
  SPI_NSS(1);													// 使能C	
  delay_us(2);
}

/***********************************************************************************
* 函数名：uint16_t MMR_Read_ADC(void)
* 描述  ：发送命令
* 输入  ：无
* 输出  ：读取温度  压力
************************************************************************************/
int32_t MMR_Read_ADC(void)						
{
  uint16_t data,temp;									// 压力缓存
  int32_t spct_a,spct_b,spct,spc,spt25x,px100;
  double px100d;
  
  SPI_NSS(0);													// 使能C		
  delay_us(2);												// 短延时
  data=SPI_SendByte(READ_ADC);				// 发送命令
  temp=data<<14;											// 读取后两位 高2位
  data=SPI_SendByte(Dummy_Byte);			// 发送命令
  temp|=data<<6;											// 读取中间8位
  data=SPI_SendByte(Dummy_Byte);			// 发送命令
  temp|=data>>2;											// 读取最后6位
  SPI_NSS(1);													// 使能C	
  
  // 计算过程	
  spct_a = (MMR_Calib_C2 + 32) * (-36) * Press_Deltat;
  spct_b = spct_a / 160;
  spct = (1<<16) + spct_b;
  spc = 13312 * (MMR_Calib_C1 + 24576);
  spt25x = (spc / spct)*25;
  px100d = (((double)spt25x) * ( temp - Press_Zero )) /(1<<16);
  px100 = (int32_t)px100d;	
  
  return px100;
}

static int32_t pressure = 0;

int32_t MMR_get_pressure(void)
{
  return pressure;
}

void MMR_process(void)
{
  static u8 state = 0;
  if(state == 0)
  {
    MMR_Send_CMD(MEASURE_PRESSC);
    state = 1;
  }
  else if(!MMR_DRDY)
  {
    pressure = MMR_Read_ADC();
    state = 0;
  }
}
uint8_t eeprom[12];
/***********************************************************************************
* 函数名：uint8_t MMR901MX_Init(void)
* 描述  ：检测应答信号
* 输入  ：无
* 输出  ：无
************************************************************************************/
uint8_t MMR901MX_Init(void)
{	
  uint8_t i;
  uint16_t erro=0;
  uint16_t data,temp;														// 压力缓存
  int32_t trefc,stc,tx10,ta,d2c,d2ref;
  double tad,txd,et1,tref;
  
  
  //**************解决测量血压异常问题，2021.8.19
  SPI_NSS(0);													// 使能C		
  delay_us(2);												// 短延时
  data=SPI_SendByte(READ_ADC);				// 发送命令
  temp=data<<14;											// 读取后两位
  data=SPI_SendByte(Dummy_Byte);			// 发送命令
  temp|=(data<<6);										// 读取中间8位
  data=SPI_SendByte(Dummy_Byte);			// 发送命令
  temp|=data>>2;											// 读取最后6位
  SPI_NSS(1);	
  //**********************************2021.8.19
  // 停止测量温度
  MMR_Send_CMD(STOP_CONVERSION);				// 停止测量	
  HAL_Delay(1);
  // 将EEPROM数据传送到MMR901XA的内部寄存器
  MMR_Send_CMD(LOAD_EEPROM);						// 
  // 读取EEPROM
  for(i=0;i<10;i++)											// 读取EEPROM
  {
    eeprom[i]= MMR_Read_EEPROM(i);			//
    erro+=eeprom[i];										//
  }
  erro=(erro/256)+(erro%256);						// 计算校验值
  if(erro!=0xFF) 		return 1;						// 传感器异常		
  // 校正参数校验
  // C1
  MMR_Calib_C1=((eeprom[2]<<6)&0x3FC0)|((eeprom[3]>>2)&0x003F);							// c1<13:6>  |   c1<5:0>   = 14bit
  // C2
  MMR_Calib_C2=((eeprom[3]<<4)&0x30)|((eeprom[4]>>4)&0x0F);									// c2<5:4>	 |	 c2<3:0>   = 6bit
  // C3
  MMR_Calib_C3=((eeprom[4]<<9)&0x1E00)|((eeprom[5]<<1)&0x01FE)|((eeprom[6]>>7)&0x0001);  // c3<12:9> |  c3<8:1>  |  c3<0>     = 13bit
  // C4
  MMR_Calib_C4=((eeprom[6]<<2)&0x01FC)|((eeprom[7]>>6)&0x0003);							// c4<8:2>   |	 c4<1:0>   = 9bit
  // C5
  MMR_Calib_C5=eeprom[7]&0x3F;																							// c5<5:0>   = 6bit
  // 开始测量温度
  MMR_Send_CMD(MEASURE_TEMP);	
  // 读取温度转换值
  while(MMR_DRDY);
  // 读取温度
  SPI_NSS(0);													// 使能C		
  delay_us(2);												// 短延时
  data=SPI_SendByte(READ_ADC);				// 发送命令
  temp=data<<14;											// 读取后两位
  data=SPI_SendByte(Dummy_Byte);			// 发送命令
  temp|=(data<<6);										// 读取中间8位
  data=SPI_SendByte(Dummy_Byte);			// 发送命令
  temp|=data>>2;											// 读取最后6位
  SPI_NSS(1);													// 使能C	
  // 停止测量温度
  MMR_Send_CMD(STOP_CONVERSION);			// 停止测量
  // 测试数据
  //	MMR_Calib_C1=5796;
  //	MMR_Calib_C2=36;
  //	MMR_Calib_C3=3950;
  //	MMR_Calib_C4=312;
  //	MMR_Calib_C5=17;
  //	temp=32800;
  // 开始计算温度
  // (1) calculate the reference temperature
  trefc = (1<<14) * 50 * MMR_Calib_C5 + 26214400;
  // (2) calculate the ADC value in the temperature sensor at the reference temperature
  //	d2ref = TC_D2TREF_TYP_X10 + ( (((sint32)func_calib_data.c3) - M_POW2_12) * TC_D2TREF_RANGE_X10) / M_POW2_13;
  //	d2ref = (322150) + ( (((sint32)func_calib_data.c3) - M_POW2_12) * (196600) ) / M_POW2_13;
  //  d2ref = 322150+ 196600 * (((int32_t)MMR_Calib_C3 - 4096) / 8192.0);
  d2ref = 223850 + ((int32_t)MMR_Calib_C3 * 24575) /(1<<10);
  // (3) calculate the slope of the temperature conversion equation
  stc = 30720 + 40 * MMR_Calib_C4;
  // (4) calculation the difference the value of (2) from the ADC value in the temperature sensor
  d2c = 10 * temp - d2ref;
  // (5) convert the ADC value into the temperature
  tad = ((double)stc) * d2c + trefc;
  txd = (tad/ (1<<20)) / 10.0;
  tref = (trefc/ (1<<20)) / 10.0;
  et1 = (-8.056181e-4) * (txd - (50.0-tref) ) * (txd - tref);
  tad = ((txd - et1) * 10.0 * (1<<20));
  ta = (int32_t)tad;
  // (6) convert into the uints of 0.1 degrees C
  tx10 = ta / (1<<20);								// 温度
  Temp_Value=tx10;										// 
  // (7) calculate the difference from the reference temperature[x10 deg.]
  Press_Deltat = (ta-trefc)/(1<<20);	// 温度校准压力值		
  // 开始测量压力值
  MMR_Send_CMD(MEASURE_PRESS);				// 开启压力转换 
  temp=0;
  // 读取压力转换值读取温度转换值
  while(MMR_DRDY);
  SPI_NSS(0);													// 使能C		
  delay_us(2);												// 短延时
  data=SPI_SendByte(READ_ADC);				// 发送命令
  temp=data<<14;											// 读取后两位
  data=SPI_SendByte(Dummy_Byte);			// 发送命令
  temp|=data<<6;											// 读取中间8位
  data=SPI_SendByte(Dummy_Byte);			// 发送命令
  temp|=data>>2;											// 读取最后6位
  Press_Zero=temp;										// 获取数据(压力零点)
  SPI_NSS(1);													// 使能C	
  MMR_Send_CMD(STOP_CONVERSION);			// 停止测量	
  HAL_Delay(10);												// 短延时
  return 0x00;
}

/******************* (C) COPYRIGHT 2011 (C) COPYRIGHT 2012 For XiaoyI ************************/



