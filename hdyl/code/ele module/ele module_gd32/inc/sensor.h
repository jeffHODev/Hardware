#ifndef SENSOR_H
#define SENSOR_H
#include "main.h"
#include "modbus.h"

#define ELE_TIMEOUT		30000
#define ON 0
#define OFF 1
#define FORWARD  0
#define BACKWARD 1
#define WASH_TIME  5*60000
#define WASH_TIME_SETTING        1*60000   //wash time setting
#define MAX_WASH_TIME  3
#define WATER_H	0
#define WATER_L	1
#define WATER_M	2
#define WATER_F	3
#define FIR_NUM      20


#define MAX_FLOW  10
#define MIN_FLOW  1

#define MAX_PH    3
#define MIN_PH    2

#define MAX_ORP   1150
#define MIN_ORP   1100
#define TDS_TEMP_FACTOR 0.99f								// TDS温度补偿系数 20摄氏度标准值
#define TDS_NUM      225									// TDS 数量

#define MAX_CURR_VALUE  13000
#define MIN_CURR_VALUE  600
#if FLOW_SIZE == 4
#define DES_CURR_VALUE  10000   //目标电流
#else
#define DES_CURR_VALUE  9000   //目标电流
#endif
#define MAX_LONG_TICK	1000
#define MAX_SHORT_TICK	1000
#define MAX_TICK	18000

#define TX_PERIOD   1000
#define WASH_TIME   18000

typedef struct
{
    unsigned int ele_status;
	unsigned char addr;
	unsigned int wash;
	unsigned int reset;
	float ele_curr;
		float ele_curr_tmp;
	unsigned char online;
	unsigned char inverEle;
	unsigned char wash_time;
	float setCurrent;
}sensor_stru;
void GetWaterLevel(void);
void sensor_process(void);
sensor_stru *GetSensor(void);
void params_Init(void);

void adcInit(void);
uint32_t *get_adc_buf(void);
void test2(void);
#endif
