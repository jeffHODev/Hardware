/******************** (C) COPYRIGHT 2014 盛元电子工作室 **************************
 * 文件名  ：DS3231.c
 * 描述    ：IIC 应用函数         
 * 实验平台：STM32手持机开发板
 * 硬件连接: 
 * 库版本  ：ST3.0.0
 * 作者    ：小奕
 * 项目说明：流量计项目
**********************************************************************************/
#include "iic.h"
#include "delay.h" 

__IO KEYMODE_TypeDef Key_Now_Mode;
uint8_t Key_Code_Value=0;
/********************************************************************************
 * 函数名：I2C_GPIO_Config		 												
 * 描述  ：I2C1 I/O配置	   														
 * 输入  ：无																	
 * 输出  ：无																	
 * 调用  ：内部调用																
*********************************************************************************/

void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	IIC_SCL(1); IIC_SDA(0);IIC_WP(1);
  /* GPIOA Periph clock enable */
  /* Configure PA8 and PA9 in output pushpull mode */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	delay_ms(10);
  I2C_Stop();
}
/********************************************************************************
 * 函数名：I2C_Start									
 * 描述  ：I2C1起始条件										
 * 输入  ：无																	
 * 输出  ：无																	
 * 调用  ：内部调用																
*********************************************************************************/
bool I2C_Start(void)
{	
	uint16_t timeout=I2C_TIME_OUT;
	
  IIC_SCL(1);
  IIC_SDA(1);	 
  delay_us(10);
	while(--timeout && SDA_read==0)
	{
		delay_us(10);
	}
	IIC_SDA(0);									// 
	delay_us(10);	
	IIC_SCL(0);									// 
	if(timeout==0) return FALSE;
	else return TRUE;
}
/********************************************************************************
 * 函数名：2C_Stop						
 * 描述  ：I2C1停止条件										
 * 输入  ：无																	
 * 输出  ：无																	
 * 调用  ：内部调用																
*********************************************************************************/
void I2C_Stop(void)
{
	IIC_SDA(0);
	IIC_SCL(0);
	delay_us(5);
  IIC_SCL(1);
	delay_us(5);
  IIC_SDA(1);
}
/********************************************************************************
 * 函数名：I2C_Ack				
 * 描述  ：I2C1 ACK应答							
 * 输入  ：无																	
 * 输出  ：无																	
 * 调用  ：内部调用																
*********************************************************************************/
void I2C_Ack(void)
{	
	IIC_SCL(0);
  IIC_SDA(0);
  delay_us(10);
  IIC_SCL(1);
  delay_us(10);
  IIC_SCL(0);
}
/********************************************************************************
 * 函数名：I2C_NoAck				
 * 描述  ：I2C1 NoAck应答							
 * 输入  ：无																	
 * 输出  ：无																	
 * 调用  ：内部调用																
*********************************************************************************/
void I2C_NoAck(void)
{	
	IIC_SCL(0);
  IIC_SDA(1);
  delay_us(10);
  IIC_SCL(1);
  delay_us(10);
  IIC_SCL(0);
}

/********************************************************************************
 * 函数名：I2C_WaitAck		
 * 描述  ：等待Ack应答					
 * 输入  ：无																	
 * 输出  ：无																	
 * 调用  ：内部调用																
*********************************************************************************/
bool I2C_WaitAck(void) 	 		//返回为:=1有ACK,=0无ACK
{
  uint16_t timeout=I2C_TIME_OUT;

  IIC_SCL(1);
  delay_us(10);
  while(--timeout && SDA_read)						// 等待总线应答 
  {
    delay_us(10);
  }
	IIC_SCL(0);
	delay_us(10);
  if(timeout==0) return FALSE;	
	else return TRUE; 
}

/********************************************************************************
 * 函数名：I2C_SendByte		
 * 描述  ：I2C 发送一个字节					
 * 输入  ：无																	
 * 输出  ：无																	
 * 调用  ：内部调用																
*********************************************************************************/
void I2C_SendByte(uint8_t SendByte) //数据从高位到低位//
{
  uint8_t i=8;
  
	IIC_SCL(0);
	delay_us(5);
  while(i--)
  {
    if(SendByte&0x80)	IIC_SDA(1); 			 
    else IIC_SDA(0); 		  
    SendByte<<=1;
    delay_us(5);
    IIC_SCL(1);
    delay_us(5);
    IIC_SCL(0);
		delay_us(5);
  }
}
/********************************************************************************
 * 函数名：I2C_ReceiveByte	
 * 描述  ：I2C 接收一个字节					
 * 输入  ：无																	
 * 输出  ：无																	
 * 调用  ：内部调用																
*********************************************************************************/
uint8_t I2C_ReceiveByte(void)  			//数据从高位到低位
{ 
  uint8_t i=8;
  uint8_t ReceiveByte=0;

  IIC_SDA(1); 			
  while(i--)
  {     
    IIC_SCL(1);
    delay_us(10);
    ReceiveByte<<=1; 
    if(SDA_read)	ReceiveByte++;
		delay_us(10);
		IIC_SCL(0);
  }
  return ReceiveByte;
}

/*************************************************************************************
 *	函数名称:	void EEP_Write_Date( uint16_t address, uint8_t date )		
 *	输入变量:	
 *	输出变量:	功能描述:	指定地址写数据
 *	调		用：外部调用		
*************************************************************************************/
bool I2C_Write( uint8_t address,uint8_t num,uint8_t *temp )
{
    // 
    if(!I2C_Start()){I2C_Stop(); return FALSE;}	  	// 起始条件
    I2C_SendByte(Write_GT208);										// 设置器件硬件地址
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	  // 等待应答信号
    
    I2C_SendByte(address); 													// 设置低起始地址    
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	 	// 等待应答信号
    // 
    for(;num>0;num--)																// 循环写入多个数据
    {
			I2C_SendByte(*temp++);												// 写入数据
			if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	// 等待应答信号
    }
    // 
    I2C_Stop(); 						// 发送停止信号
    return TRUE;						// 返回操作成功标识
}

/*************************************************************************************
 *	函数名称:	uint8_t EEP_Read_Dates( uint8_t address )	指定地址读取多个字节
 *	输入变量:	uint8_t address 读取数据的地址   uint8_t n读取数据的个数 
 *	输出变量:	uint8_t *temp 读取到的位置
 *	功能描述:	突发式读取EEPROM
*************************************************************************************/
bool I2C_Read( uint8_t address,uint8_t num, uint8_t *temp )
{
    if(!I2C_Start()){I2C_Stop(); return FALSE;}	  	// 起始条件
    I2C_SendByte(Write_GT208);											// 设置器件硬件地址
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	 	// 等待应答信号
    
    I2C_SendByte(address); 													// 设置低起始地址       
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	 	// 等待应答信号
    
    if(!I2C_Start()){I2C_Stop(); return FALSE;}	  	// 起始条件
    I2C_SendByte(Read_GT208);												// 设置器件硬件地址
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	 	// 等待应答信号
    
    for(;num>0;num--)																// 读取N个字节
    {	
        *temp++ = I2C_ReceiveByte();			        	// 读取一个字节
        if(num==1)  I2C_NoAck();										// 最后一个数据发送NOACK	
        else 	  I2C_Ack();				        					// 读取下一个数据发送ACK			
    }
    
    I2C_Stop(); 																		// 发送停止信号
    return TRUE;
}


uint8_t Init_GT208L(void)
{
  uint8_t idcode;
	uint8_t setcmd[]={0x0B,0x55,0x00};
  uint8_t setvalue[]={0x01,0x05,0x20};
	//uint8_t sensitivity[]={0x03,0x03,0x3F,0x03,0x3F,0x3F,0x3F,0x3F};		
	//  uint8_t sensitivity[]={0x0F,0x0a,0x0F,0x0F,0x0f,0x0f,0x0F,0x0f};		// High 0x3f   Low  0x02
		
	 uint8_t sensitivity[]={0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a};		// Low  0x3f     High 0x02	4 5 7
		
  I2C_GPIO_Config();
  // RED ID
  I2C_Read(CHIP_ID_REG,0x01,&idcode);
  // SET
  I2C_Write(TOUCH_CHANNEL_EN,sizeof(setcmd),setcmd);
  I2C_Write(GENERAL1_CON_REG,sizeof(setvalue),setvalue);
	I2C_Write(SENSITIVITY_REG,sizeof(sensitivity),sensitivity);		
	// IDLE time set
	// 2018-07-18
	setcmd[0]=0x08;
	I2C_Write(0x3D,1,&setcmd[0]);
	
	return idcode;

}

uint8_t GT208L_Read_Key(void)
{
  uint8_t keycode=0xff;
  
  if(I2C_Read(TOUCH_OUTPUT_REG,0x01,&keycode)==FALSE) return 0; 

  return keycode;

}

/***********************************************************************************
 * 函数名：Button_Read(void)
 * 描述  ：读取按键数据
 * 输入  ：无
 * 输出  ：无
************************************************************************************/uint8_t Button_Read(void)
{			
  // 按键滤波时间计时
  static uint32_t Key_Time_PH=0;	      // 按键检测时间		PH   设定按键
  static uint32_t Key_Time_TEM=0;	      // 按键检测时间		TM   设定按键
  static uint32_t Key_Time_START=0;	    // 按键检测时间		START   设定按键	
	uint8_t keycode;
  
	keycode=GT208L_Read_Key();										// 硬件
	Key_Code_Value=keycode;												// 
	// PH按键按下
  if(keycode&0x02)															// SET按键按下
  {		
    if(Key_Now_Mode.KEY_PH_MODE==KEY_PH_HOLD)		// 长按键
    {
			Key_Now_Mode.KEY_PH_MODE=KEY_PH_HOLD;			//
    }
    else if(Key_Now_Mode.KEY_PH_MODE==KEY_PH_DOWN)    // 前一次是按键按下
    {
      if(Key_Time_PH++>BUTTON_LONG_TIME)				// 检测到长按键
      {
        Key_Now_Mode.KEY_PH_MODE=KEY_PH_HOLD;		// 
      }		
    }
    else 																				// 前一次没有按键按下
    {
      if(Key_Time_PH++>BUTTON_FILTER_TIME)			// 按键通过滤波检测
      {
        Key_Now_Mode.KEY_PH_MODE=KEY_PH_DOWN;		// 
				Standby_Time=0;		
      }
    }
  }
  else
  {
    Key_Now_Mode.KEY_PH_MODE=KEY_PH_UP;					// 按键抬起
    Key_Time_PH=0;
  }
	// TM按键按下
  if(keycode&0x01)															// SET按键按下
  {		
    if(Key_Now_Mode.KEY_TEM_MODE==KEY_TEM_HOLD)		// 长按键
    {
			Key_Now_Mode.KEY_TEM_MODE=KEY_TEM_HOLD;			//
    }
    else if(Key_Now_Mode.KEY_TEM_MODE==KEY_TEM_DOWN)    // 前一次是按键按下
    {
      if(Key_Time_TEM++>BUTTON_LONG_TIME)				// 检测到长按键
      {
        Key_Now_Mode.KEY_TEM_MODE=KEY_TEM_HOLD;		// 
      }		
    }
    else 																				// 前一次没有按键按下
    {
      if(Key_Time_TEM++>BUTTON_FILTER_TIME)			// 按键通过滤波检测
      {
        Key_Now_Mode.KEY_TEM_MODE=KEY_TEM_DOWN;		// 
				Standby_Time=0;				
      }
    }	
  }
  else
  {
    Key_Now_Mode.KEY_TEM_MODE=KEY_TEM_UP;					// 按键抬起
    Key_Time_TEM=0;
  }
	// START按键按下
  if(keycode&0x08)															// SET按键按下
  {		
    if(Key_Now_Mode.KEY_START_MODE==KEY_START_HOLD)		// 长按键
    {
			Key_Now_Mode.KEY_START_MODE=KEY_START_HOLD;			//
    }
    else if(Key_Now_Mode.KEY_START_MODE==KEY_START_DOWN)    // 前一次是按键按下
    {
      if(Key_Time_START++>BUTTON_LONG_TIME)				// 检测到长按键
      {
        Key_Now_Mode.KEY_START_MODE=KEY_START_HOLD;		// 
      }		
    }
    else 																				// 前一次没有按键按下
    {
      if(Key_Time_START++>BUTTON_FILTER_TIME)			// 按键通过滤波检测
      {
        Key_Now_Mode.KEY_START_MODE=KEY_START_DOWN;		// 
				Standby_Time=0;
      }
    }	
  }
  else
  {
    Key_Now_Mode.KEY_START_MODE=KEY_START_UP;					// 按键抬起
    Key_Time_START=0;
  }	
	
	return keycode;
}
