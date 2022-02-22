#ifndef __ECG_H__
#define __ECG_H__

#include "main2.h"
#define PARAMS_SIZE  75
typedef struct{
  uint8_t data[PARAMS_SIZE];
}MISCDATA;

void ecg_init(void);
void ecg_enable(uint8_t enable);
uint8_t getRegister(uint8_t addr);
uint8_t WriteRegister(uint8_t addr, uint8_t data);












#endif





