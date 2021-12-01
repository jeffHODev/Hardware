#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
	
extern uint8_t LED_ZS;							// 制水
extern uint8_t LED_DJ;							// 电解
extern uint8_t LED_CX;							// 冲洗
extern uint8_t LED_QS;							// 缺水	
extern uint8_t Refresh_LED;					// 强制刷新LCD
extern __IO uint8_t Standby_Time_Delay;			// 待机延时时间
	
	
void bsp_led_init(void);						// LED初始化
void Refresh_LED_Display(void);			// LED刷新显示

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif											
											
#endif


