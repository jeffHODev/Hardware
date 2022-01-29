#ifndef SENSOR_H
#define SENSOR_H
#include "main.h"
#define FIR_NUM   10
typedef struct 
{
    uint16_t res;
	float ta;
	float tb;

	float res_cal;
	float vol;
	uint16_t br;
	uint16_t vbat;	
	uint16_t temperature;
	unsigned char key_status;
}sensors_stru;
sensors_stru *getSensor(void);
void sensor_proc(void);

#endif

