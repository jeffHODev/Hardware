#ifndef SENSOR_H
#define SENSOR_H
#include "main.h"
#include "modbus.h"
#define WATER_H	0
#define WATER_L	1
#define WATER_M	2
#define WATER_F	3
#define FIR_NUM      25


#define MAX_FLOW  10
#define MIN_FLOW  1

#define MAX_PH    3
#define MIN_PH    2

#define MAX_ORP   1150
#define MIN_ORP   1100
#define TDS_TEMP_FACTOR 0.99f								// TDS温度补偿系数 20摄氏度标准值
#define TDS_NUM      225									// TDS 数量

#define MAX_TDS_VALUE  1200
#define MIN_TDS_VALUE  1100

#define MAX_LONG_TICK	1000
#define MAX_SHORT_TICK	1000
#define MAX_TICK	18000

#define TX_PERIOD   1000
#define WASH_TIME   18000

typedef struct
{
    unsigned char swH;
	unsigned char water_level;
	float tds;
	float flow;
	float temperature;
	float ph;
	float orp;
	unsigned int warn;
	float ele_curr;
	float orp_ph_adc;
	uint32_t timeout;
	unsigned char err_flag;
	unsigned char water_status;
	
}sensor_stru;
void GetWaterLevel(void);
void sensor_process(void);
sensor_stru *GetSensor(void);
modbus_pack_stru *GetModbusPack(void);
void adcInit(void);
void adcResume(void);

#endif
