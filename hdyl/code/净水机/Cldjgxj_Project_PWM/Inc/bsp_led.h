#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
	
extern uint8_t LED_ZS;							// ��ˮ
extern uint8_t LED_DJ;							// ���
extern uint8_t LED_CX;							// ��ϴ
extern uint8_t LED_QS;							// ȱˮ	
extern uint8_t Refresh_LED;					// ǿ��ˢ��LCD
extern __IO uint8_t Standby_Time_Delay;			// ������ʱʱ��
	
	
void bsp_led_init(void);						// LED��ʼ��
void Refresh_LED_Display(void);			// LEDˢ����ʾ

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif											
											
#endif


