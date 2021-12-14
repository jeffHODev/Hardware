#ifndef __FILTER_H__
#define __FILTER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

#define MAX_READ_FILTER_NUM 10
#define MAX_RETRY_FILTER_NUM 50
#define FITER_PP 	0x01
#define FITER_C1 	0x02
#define FITER_UDF 0x03
#define FITER_C2 	0x04
/***************************系统变量声明*****************************/

/***************************系统函数声明*****************************/	
extern uint8_t PP_Write_Flage;								// PP棉滤芯写卡
extern uint8_t C1_Write_Flage;								// C1滤芯写卡
extern uint8_t UDF_Write_Flage;								// UDF芯写卡
extern uint8_t C2_Write_Flage;								// C2芯写卡
	
extern uint16_t PP_Filter_Life;								// 6个月 24*30*6
extern uint16_t C1_Filter_Life;								// 12个月
extern uint16_t UDF_Filter_Life;							// 24个月
extern uint16_t C2_Filter_Life;								// 12个月

extern uint8_t Probe_PP;											// PP棉滤芯 剩余
extern uint8_t Probe_C1;											// C1滤芯
extern uint8_t Probe_UDF;											// UD滤芯
extern uint8_t Probe_C2;											// C2滤芯
extern uint8_t LED_PP;												// PP棉滤芯
extern uint8_t LED_C1;												// C1滤芯
extern uint8_t LED_UDF;												// UD滤芯
extern uint8_t LED_C2;												// C2滤芯

extern __IO uint8_t Read_Filter_Flage;				// 读取滤芯标志

void System_Read_Filter(void);								//
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif											
											
#endif





