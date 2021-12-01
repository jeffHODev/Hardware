#ifndef _TPL0501_H
#define	_TPL0501_H

#include "stm32f0xx.h"

/*****************************宏定义区********************************************/

/* 硬件连接设置	*/
/* TPL0501_CS1---PC10	*/
#define TPL0501_CS(a) 	  do{if (a)\
													GPIO_SetBits(GPIOA,GPIO_Pin_5);\
													else\
													GPIO_ResetBits(GPIOA,GPIO_Pin_5);}while(0)

/* TPL0501_SCLK---PC10	*/
#define TPL0501_SCLK(a) 	  do{if (a)\
													GPIO_SetBits(GPIOA,GPIO_Pin_7);\
													else\
													GPIO_ResetBits(GPIOA,GPIO_Pin_7);}while(0)																				
										
/* TPL0501_SCLK---PC10	*/
#define TPL0501_DINW(a) 	  do{if (a)\
													GPIO_SetBits(GPIOA,GPIO_Pin_6);\
													else\
													GPIO_ResetBits(GPIOA,GPIO_Pin_6);}while(0)		

/* ADS1256_READY	--	PC1	*/
#define TPL0501_DINR			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)													
													
uint8_t TPL0501_SendByte(uint8_t byte);

#endif /* __NRF_24L01_H */
