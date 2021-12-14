#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "stm32f10x.h"

extern __IO uint8_t LED_QS;							// 原水箱缺水     0 1 2
extern __IO uint8_t LED_MS;							// 酸水箱水满			0 1 2
extern __IO uint8_t LED_TDS;						// 水质TDS指示灯  0 1 2
extern __IO uint8_t LED_LX;							// 滤芯已满指示		0 1 2
extern __IO uint8_t LED_GW;							// 原水水温高     0 1 2
extern __IO uint8_t LED_QX;							// 清洗模式															
extern __IO uint8_t LED_Brightness;	  	// 亮度调节
extern __IO uint8_t Display_Rama[3];		// 显示缓冲区 PH   10.5pH
extern __IO uint8_t Display_Ramb[3];		// 显示缓冲区 ORP  -385mv
extern __IO uint8_t Display_Ramc[2];		// 显示缓冲区 温度 88  ℃

/* 硬件连接设置	*/
/* Configure TM1640_SCLK pins: PA4*/
#define TM1640_SCLK(a)do{if (a)\
											GPIO_ResetBits(GPIOB,GPIO_Pin_15);\
											else\
											GPIO_SetBits(GPIOB,GPIO_Pin_15);}while(0)
/* Configure TM1640_DINA pins: PA3*/
#define TM1640_DIN(a)	do{if (a)\
											GPIO_ResetBits(GPIOB,GPIO_Pin_14);\
											else\
											GPIO_SetBits(GPIOB,GPIO_Pin_14);}while(0)

void bsp_led_init(void);
void Refresh_LED_Display(void);

#endif


