#ifndef __ADC_H
#define	__ADC_H

#include "stm32f10x.h"

#define CHANNEL_NUM  9										// 
#define SAMPLE_NUM   20										// 

void ADC1_Mode_Config(void);
uint16_t ReadADCAverageValue(uint8_t channel);
#endif /* __ADC_H */

