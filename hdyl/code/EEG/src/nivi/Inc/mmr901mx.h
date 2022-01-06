#ifndef _MMR901XA_H
#define	_MMR901XA_H

#include "main.h"

/*****************************宏定义区********************************************/
/* 宏定义 */
#define Dummy_Byte 0xff
//读写格式
/*-----command of the MMR901XA registers------*/
#define MEASURE_TEMP			0x30		// 测量温度
#define READ_EEPROM				0x40		// 读取EEPROM
#define MEASURE_PRESS			0x50		// 读取压力
#define LOAD_EEPROM				0x60		// 将EEPROM数据读取到REGISTER
#define SET_DELAY					0x70		// 转换延时
#define MEASURE_TEMPC			0x80		// 测量温度连续转换
#define MEASURE_PRESSC		0x90		// 读取压力连续转换
#define STOP_CONVERSION		0xA0 		// 停止转换
#define READ_ADC					0xC0		// 读取转换结果
#define Temp							0x00
#define Press							0x01

/*-----addrres of the MMR901XA EEPROM	------*/
#define EEPROM_ADDR0			0x00		// 保留
#define EEPROM_ADDR1			0x01		// 保留
#define EEPROM_ADDR2			0x02		// C1(13-6)
#define EEPROM_ADDR3			0x03		// C1(5-0)    C2(5-4)
#define EEPROM_ADDR4			0x04		// C2(3-0)		C3(12-9)
#define EEPROM_ADDR5			0x05		// C3(8-1)		
#define EEPROM_ADDR6			0x06		// C3(0) 			C4(8-2)
#define EEPROM_ADDR7			0x07		// C4(1-0)		C5(5-0)
#define EEPROM_ADDR8			0x08		// 保留
#define EEPROM_ADDR9			0x09		// Checksum	(7-0)

/* 硬件连接设置	*/
#define SPI_NSS(a)	do{if (a)\
										HAL_GPIO_WritePin(MMR_CS_GPIO_Port, MMR_CS_Pin, GPIO_PIN_SET);\
										else\
										HAL_GPIO_WritePin(MMR_CS_GPIO_Port, MMR_CS_Pin, GPIO_PIN_RESET);}while(0)

#define MMR_DRDY  (HAL_GPIO_ReadPin(MMR_RD_GPIO_Port, MMR_RD_Pin) == GPIO_PIN_RESET ? 0 : 1)
/*	全局变量声明	*/
										
										
/*	程序声明	*/
uint8_t MMR901MX_Init(void);
int32_t MMR_Read_ADC(void);
void MMR_process(void);
int32_t MMR_get_pressure(void);
#endif /* __NRF_24L01_H */
