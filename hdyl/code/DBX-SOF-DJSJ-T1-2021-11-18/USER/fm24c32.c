/******************** (C) COPYRIGHT 2014 盛元电子工作室 **************************
 * 文件名  ：FM24C256.c
 * 描述    ：IIC 应用函数         
 * 实验平台：STM32手持机开发板
 * 硬件连接: 
 * 库版本  ：ST3.0.0
 * 作者    ：小奕
 * 项目说明：流量计项目
**********************************************************************************/
#include "iic.h"
#include "delay.h" 

/* 本函数声明的变量	*/
// EEPROM容量为32768 BYTES
/***********************************EEPROM存储规划*************************************/ 
/***********************************0-99字节存放设备基本信息**************************/ 
/*		地址  0-9  	  :	设备序列号		  	  [5 BYTE]				共2个字											*/
/*		地址  10-19  	:	初始化标志		  	  [5 BYTE]				共2个字											*/
/*		地址  20-29  	:	设备当前RFID		  	[5 BYTE]				共2个字											*/
/*		地址  30-39  	:	设备故障代码记录		[5 BYTE]				共2个字											*/
/*		地址  40-49  	:	设备故障代码记录		[5 BYTE]				共2个字											*/
/*		地址  50-59  	:	设备故障代码记录		[5 BYTE]				共2个字											*/
/***********************************100-199ADC数据内容段*******************************/ 

/********************************************************************************
 * 函数名：EEP_Write_Date( uint16_t address, uint8_t date )	
 * 描述  ：指定地址写数据
 * 输入  ：无																	
 * 输出  ：无																	
 * 调用  ：外部调用			
*********************************************************************************/
uint8_t EEP_Write_Date( uint16_t address, uint8_t date )			// 指定地址写数据
{
	IIC_WP(0);
	delay_us(10);
	if(!I2C_Start()){I2C_Stop(); return 0;}	  			// 起始条件
	I2C_SendByte(Write_EE);													// 设置器件硬件地址
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	  		// 等待应答信号
	I2C_SendByte(address/256);						 					// 设置高起始地址 
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	  		// 等待应答信号
	I2C_SendByte(address%256); 											// 设置低起始地址    
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	  		// 等待应答信号
	I2C_SendByte(date);															// 写入数据
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 			// 等待应答信号
	I2C_Stop(); 																		// 发送停止信号
	IIC_WP(1);
	delay_ms(20);
	return 1;
}

/*************************************************************************************
 *	函数名称:	void EEP_Write_Date( uint16_t address, uint8_t date )					// 指定地址写数据
 *	输入变量:	
 *	输出变量:	功能描述:	指定地址写数据
 *	调		用：外部调用		
*************************************************************************************/
uint8_t EEP_Write_Dates( uint16_t address,uint16_t num, uint8_t *pdate )		// 指定地址写数据
{
	IIC_WP(0);
	delay_us(10);
	if(!I2C_Start()){I2C_Stop(); return 0;}	  			// 起始条件
	I2C_SendByte(Write_EE);													// 设置器件硬件地址
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	  		// 等待应答信号
	I2C_SendByte(address/256);						 					// 设置高起始地址 
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 			// 等待应答信号
	I2C_SendByte(address%256); 											// 设置低起始地址    
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 			// 等待应答信号
	for(;num>0;num--)																// 循环写入多个数据
	{
		I2C_SendByte(*pdate++);												// 写入数据
		if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 		// 等待应答信号
	}	
	I2C_Stop(); 																		// 发送停止信号
	IIC_WP(1);	
	delay_ms(20);																		// 延时
	
	return 1;																				// 返回操作成功标识
}

/*************************************************************************************
 *	函数名称:	void EEP_Write_Date( uint16_t address, uint8_t date )		// 指定地址写数据
 *	输入变量:	
 *	输出变量:	功能描述:	指定地址写数据
 *	调		用：外部调用		
*************************************************************************************/
uint8_t EEP_Write_NULL( uint16_t address,uint16_t num )		// 指定地址写数据
{
	IIC_WP(0);
	delay_us(10);
	if(!I2C_Start()){I2C_Stop(); return 0;}	  			// 起始条件
	I2C_SendByte(Write_EE);													// 设置器件硬件地址
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	  		// 等待应答信号
	I2C_SendByte(address/256);						 					// 设置高起始地址 
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 			// 等待应答信号
	I2C_SendByte(address%256); 											// 设置低起始地址    
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 			// 等待应答信号
	for(;num>0;num--)																// 循环写入多个数据
	{
		I2C_SendByte(0X00);														// 写入数据
		if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 		// 等待应答信号
	}	
	I2C_Stop(); 																		// 发送停止信号
	delay_ms(20);
	IIC_WP(1);	
	return 1;																				// 返回操作成功标识
}
/*************************************************************************************
 *	函数名称:	uint8_t EEP_Read_Date( uint8_t address )   指定地址读取一个字节
 *	输入变量:	uint8_t ms
 *	输出变量:
 *	功能描述:	突发式读取EEPROM
*************************************************************************************/
uint8_t EEP_Read_Date( uint16_t address )						//突发式读取EEPROM
{
	uint8_t dat;
	
	if(!I2C_Start()){I2C_Stop(); return 0;}	  				// 起始条件
	I2C_SendByte(Write_EE);														// 设置器件硬件地址
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// 等待应答信号
	I2C_SendByte(address/256);						 						// 设置高起始地址 
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// 等待应答信号
	I2C_SendByte(address%256); 												// 设置低起始地址       
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// 等待应答信号
	if(!I2C_Start()){I2C_Stop(); return 0;}	  				// 起始条件
	I2C_SendByte( Read_EE );													// 发送设备硬件地址和读信号
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// 等待应答信号
	dat = I2C_ReceiveByte();													// 读取字节
	I2C_NoAck();																			// 最后一个数据发送NOACK	
	I2C_Stop(); 																			// 发送停止信号
	return( dat );																		// 返回数据
}

/*************************************************************************************
 *	函数名称:	uint8_t EEP_Read_Dates( uint8_t address )	指定地址读取多个字节
 *	输入变量:	uint8_t address 读取数据的地址   uint8_t n读取数据的个数 
 *	输出变量:	uint8_t *temp 读取到的位置
 *	功能描述:	突发式读取EEPROM
*************************************************************************************/
uint8_t EEP_Read_Dates( uint16_t address,uint16_t num,uint8_t *temp )//突发式读取EEPROM
{
	if(!I2C_Start()){I2C_Stop(); return 0;}	  				// 起始条件
	I2C_SendByte(Write_EE);														// 设置器件硬件地址
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// 等待应答信号
	I2C_SendByte(address/256);						 						// 设置高起始地址 
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// 等待应答信号
	I2C_SendByte(address%256); 												// 设置低起始地址       
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// 等待应答信号
	if(!I2C_Start()){I2C_Stop(); return 0;}	  				// 起始条件
	I2C_SendByte( Read_EE );													// 发送设备硬件地址和读信号
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// 等待应答信号
	for(;num>0;num--)																	// 读取N个字节
	{
		*temp++ = I2C_ReceiveByte();										// 读取一个字节
		if(num==1)  I2C_NoAck();												// 最后一个数据发送NOACK	
		else 	  I2C_Ack();															// 读取下一个数据发送ACK			
	}
	I2C_Stop(); 																			// 发送停止信号
	return 1;
}

/******************** (C) COPYRIGHT 2014 盛元电子工作室 **************************/
