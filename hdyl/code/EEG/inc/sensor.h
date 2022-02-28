#ifndef SENSOR_H
#define SENSOR_H
#include "main2.h"
#define FIR_NUM   10
typedef struct 
{
    uint16_t res;
	float ta;
	float tb;
  unsigned char chip_num;
	float res_cal;
	float vol;
	uint16_t br;
	uint16_t tst;
	uint16_t vbat;	
	uint16_t temperature;
	unsigned char key_status;
	unsigned char update;
}sensors_stru;
sensors_stru *getSensor(void);
void sensor_proc(void);

#endif

