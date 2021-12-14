#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "stm32f10x.h"

extern __IO uint8_t LED_QS;							// ԭˮ��ȱˮ     0 1 2
extern __IO uint8_t LED_MS;							// ��ˮ��ˮ��			0 1 2
extern __IO uint8_t LED_TDS;						// ˮ��TDSָʾ��  0 1 2
extern __IO uint8_t LED_LX;							// ��о����ָʾ		0 1 2
extern __IO uint8_t LED_GW;							// ԭˮˮ�¸�     0 1 2
extern __IO uint8_t LED_QX;							// ��ϴģʽ															
extern __IO uint8_t LED_Brightness;	  	// ���ȵ���
extern __IO uint8_t Display_Rama[3];		// ��ʾ������ PH   10.5pH
extern __IO uint8_t Display_Ramb[3];		// ��ʾ������ ORP  -385mv
extern __IO uint8_t Display_Ramc[2];		// ��ʾ������ �¶� 88  ��

/* Ӳ����������	*/
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


