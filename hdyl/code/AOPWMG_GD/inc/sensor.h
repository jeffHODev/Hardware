#ifndef SENSOR_H
#define SENSOR_H
#include "main.h"
#include "modbus.h"
#define WATER_H	0
#define WATER_L	1
#define WATER_M	2
#define WATER_F	3

#define WATER_LEVEL 0
#define FIR_NUM          20
#define FIR_NUM_MAX      50

#define MAX_FLOW  10
#define MIN_FLOW  0.1
#define MAX_PH    7
#define MIN_PH    0
#define MAX_ORP   1250
#define MIN_ORP   50
#define MAX_TDS_VALUE  2000
#define MIN_TDS_VALUE  30


#define TDS_TEMP_FACTOR 0.99f								// TDS温度补偿系数 20摄氏度标准值
#define TDS_NUM      112									// TDS 数量



#define MAX_LONG_TICK	1000
#define MAX_SHORT_TICK	5000
#define MAX_TICK	30000
#define WATER_LSHORT_TICK	1000


#define TX_PERIOD   1000
#define WASH_TIME   180000

#define ADC_SIZE    2
#define MODULE_SIZE  4


typedef struct
{
    unsigned char swH;
	unsigned char water_level;
	float tds1;
	float tds2;

	float flow;
	float temperature;
	float ph;
	float orp;
	float vcc;
	unsigned int warn;
	float ele_curr;
	float ele_Mcurr[MODULE_SIZE];
	float ele_MOnLine[MODULE_SIZE];
	float ele_EleStatus[MODULE_SIZE];

	float orp_ph_adc;
	uint32_t timeout;
	unsigned char err_flag;
	unsigned char water_status;
	unsigned char status[8];// 0:正常  1：tds1 2:tds2 3：流量 4：orp 
	                     //  5:高压开关6：水位开关 7:电解中
	
}sensor_stru;
void GetWaterLevel(void);
void sensor_process(void);
sensor_stru *GetSensor(void);
modbus_pack_stru *GetModbusPack(void);
void adcInit(void);
void adcResume(void);
uint16_t *get_adc_buf(void);
void GetModbusSens(unsigned char addr,unsigned char func,unsigned int reg,unsigned int regCount,unsigned char *buf,unsigned char datcount);

#endif
