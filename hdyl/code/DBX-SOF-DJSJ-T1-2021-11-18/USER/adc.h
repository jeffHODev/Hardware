#ifndef __ADC_H
#define	__ADC_H

#include "stm32f10x.h"

#define CHANNEL_NUM  7										// 
#define SAMPLE_NUM   5										// 
#define FIR_NUM      25
#define TDS_FIR_NUM  10
#define OPA_POW      3.0/13.0							// DOW 3.1K UP 10K
#define TDS_NUM      225									// TDS ÊýÁ¿
#define PRE_VALUE		 3.3/4095							// 

extern float IC_Temp;
extern float IC_Vdd;
extern float POW_Value;								


															
void ADC1_Mode_Config(void);
void ADC2_Mode_Config(void);
float ReadADCAverageValue(uint8_t channel);
#endif /* __ADC_H */

