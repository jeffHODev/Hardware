#ifndef __NIBP_IF_H__
#define __NIBP_IF_H__

#include "main.h"

void nibp_if_init(void);
void nibp_if_start(void);
void nibp_if_process(void);
int8_t nibp_if_cmd(uint8_t cmd);
int8_t nibp_if_stop(void);
uint16_t nibp_if_getpressure(void);
void nibp_tst_start(uint16_t dst);
void nibp_tst_stop(void);
void nibp_tst_valve_set(uint8_t stat);
void nibp_tst_motor_set(uint16_t data);










#endif







