#ifndef __fm24c256_H
#define	__fm24c256_H

#include "stm32f10x.h"

/* 宏定义	*/

#define I2C_TIME_OUT    4000					// 
#define Write_EE 				0xa0		 			// EEprom Write
#define Read_EE 				0xa1					// EEprom Read


// 基本参数
#define Option_Flage 0xA5A5						// 初始化标志
#define SERIAL_NUM_ADDR  0						// 序列号
#define OPER_FLAGE_ADDR  5						// 初始化标志地址
#define SLAVE_CODE_ADDR  10						// 设备通讯地址保存地址
#define SLAVE_BAUD_ADDR  15						// 设备通讯波特率	
#define CONTROL_MODE_ADDR  20					// 设备控制模式
#define LED_BRIGHT_ADDR  25						// 设备显示亮度 LED_KEY_Bright
// ADC参数
#define ADC_BASES_ADDR   100					// ADC基础地址
#define ADC_SIZE_VALUE   20						// ADC参数数值大小
#define ADC_FILTER_ADDR  0						// ADC滤波参数
#define ADC_RANGE_ADDR   5						// ADC量程值
#define ADC_ZERO_ADDR    10						// ADC零点校准值
#define ADC_FULL_ADDR    15						// ADC满量程校准值
// DAC参数
#define DAC_BASES_ADDR   300					// DAC基础地址
#define DAC_SIZE_VALUE   10						// DAC参数数值大小
#define DAC_RANGE_ADDR   0						// DAC量程值
#define DAC_FULL_ADDR    5						// DAC满量程校准值
// FLUX参数
#define FLUX_BASES_ADDR  500					// FLUX基础地址
#define FLUX_SIZE_VALUE  100					// FLUX参数数值大小
#define FLUX_INADEE_ADDR       0			// 当前层自增减使能标志地址
#define FLUX_UNIT_ADDR         5			// 单位设定
#define FLUX_SPOT_ADDR				10			// 流量量程小数点
#define FLUX_SHOW_ACC_ADDR		15			// 是否显示流量累积值
#define FLUX_FILTER_ADDR  		20			// ADC滤波参数
#define FLUX_RANGE_MULT_ADDR  25			// 流量计放大倍数											1-1000
#define FLUX_TIME_AUTO_ADDR		30			// 自动运行模式下流量计自增减时间  		0-255	S
#define FLUC_TIME_RUN_ADDR		35			// 用于自增减运行的时间计数				
#define FLUX_VALUE_AUTO_ADDR	40			// 自动模式下自增减运行幅度值         0-255 N
#define FLUX_RANGE_ADDR				45			// 流量计当前量程下满量程值						0-5000
#define FLUX_ALARM_VALUE_ADDR	50			// 最大偏差报警值
#define FLUX_VALUE_SET_ADDR		55			// 流量设定值
#define FLUX_DAC_FULL_ADDR		60			// DAC满量程
#define FLUX_ADC_ZERO_ADDR		65			// ADC0点量程值
#define FLUX_ADC_FULL_ADDR		70			// ADC满量程之
#define FLUX_MAX_FLOW_ADDR		75			// 最大流量设定值
#define FLUX_TIME_DOWN_ADDR		80			// 工作倒计时


/* 硬件连接设置	*/
// IIC数据线
#define IIC_WP(a)	do{if (a)\
										GPIO_SetBits(GPIOB,GPIO_Pin_13);\
										else\
										GPIO_ResetBits(GPIOB,GPIO_Pin_13);}while(0)
// 读取IIC数据线
#define SDA_read    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)
// IIC时钟线
#define IIC_SCL(a)	do{if (a)\
										GPIO_SetBits(GPIOB,GPIO_Pin_14);\
										else\
										GPIO_ResetBits(GPIOB,GPIO_Pin_14);}while(0)
// IIC数据线
#define IIC_SDA(a)	do{if (a)\
										GPIO_SetBits(GPIOB,GPIO_Pin_15);\
										else\
										GPIO_ResetBits(GPIOB,GPIO_Pin_15);}while(0)

//typedef enum {FALSE = 0, TRUE = !FALSE} bool;	
										
/* 函数声明	*/
void I2C_GPIO_Config(void);
uint8_t EEP_Read_Dates( uint16_t address,uint16_t num,uint8_t *temp );				// 突发式读取EEPROM
uint8_t EEP_Read_Date( uint16_t address );																	// 突发式读取EEPROM
uint8_t EEP_Write_Dates( uint16_t address,uint16_t num, uint8_t *pdate );		// 指定地址写数据
uint8_t EEP_Write_Date( uint16_t address, uint8_t date );									// 指定地址写数据
uint8_t EEP_Write_NULL( uint16_t address,uint16_t num );										// 指定地址写数据
#endif

/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/

