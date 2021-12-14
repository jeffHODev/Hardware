#ifndef __PID_H__
#define __PID_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
	
extern void Cur_PID_Run(void);										// PID���г���	
extern void Start_Cur_PID(uint8_t temp);					// �����������
extern void Cur_PID_Reset(void);									// ��λ����ֵ
extern void Stop_Cur_PID(void);										// ֹͣ�������
	
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif											
											
#endif										




