#ifndef __PID_H__
#define __PID_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
	
extern void Cur_PID_Run(void);										// PID运行程序	
extern void Start_Cur_PID(uint8_t temp);					// 调节输出电流
extern void Cur_PID_Reset(void);									// 复位电流值
extern void Stop_Cur_PID(void);										// 停止电流输出
	
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif											
											
#endif										




