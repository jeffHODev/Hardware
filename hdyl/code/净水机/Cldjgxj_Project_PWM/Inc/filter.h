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
/***************************ϵͳ��������*****************************/

/***************************ϵͳ��������*****************************/	
extern uint8_t PP_Write_Flage;								// PP����од��
extern uint8_t C1_Write_Flage;								// C1��од��
extern uint8_t UDF_Write_Flage;								// UDFод��
extern uint8_t C2_Write_Flage;								// C2од��
	
extern uint16_t PP_Filter_Life;								// 6���� 24*30*6
extern uint16_t C1_Filter_Life;								// 12����
extern uint16_t UDF_Filter_Life;							// 24����
extern uint16_t C2_Filter_Life;								// 12����

extern uint8_t Probe_PP;											// PP����о ʣ��
extern uint8_t Probe_C1;											// C1��о
extern uint8_t Probe_UDF;											// UD��о
extern uint8_t Probe_C2;											// C2��о
extern uint8_t LED_PP;												// PP����о
extern uint8_t LED_C1;												// C1��о
extern uint8_t LED_UDF;												// UD��о
extern uint8_t LED_C2;												// C2��о

extern __IO uint8_t Read_Filter_Flage;				// ��ȡ��о��־

void System_Read_Filter(void);								//
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif											
											
#endif





