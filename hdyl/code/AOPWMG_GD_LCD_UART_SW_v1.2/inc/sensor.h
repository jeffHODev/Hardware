#ifndef SENSOR_H
#define SENSOR_H
#include "main.h"
#include "modbus.h"

/*#define TDS_LEVEL1   500
#define TDS_LEVEL2   650  // 650-150
#define TDS_LEVEL3   950
#define TDS_LEVEL4   1000
#define TDS_LEVEL5   1100
#define TDS_LEVEL6   1200*/
//2.5-----500
//2.5-3--- 500
//3-3.5----650
//3.5-4---  950
/*#define TDS_LEVEL1   500-20
#define TDS_LEVEL2   650-120-20  // 650-150
#define TDS_LEVEL3   700-20//650-30   //850
#define TDS_LEVEL4   1100+20//-100  //1000
#define TDS_LEVEL5   1100-100 //1000
#define TDS_LEVEL6   1200-50
*/

#define TDS_LEVEL1   500-20
#define TDS_LEVEL2   650-120-20  // 650-150
#define TDS_LEVEL3   700-20-10//650-30   //850
#define TDS_LEVEL4   1000+20-10//-100  //1000
//#define TDS_LEVEL5   1100-100 //1000
//#define TDS_LEVEL6   1200-50

/*#define TDS_LEVEL1   500-20+100
#define TDS_LEVEL2   650-120+100  // 650-150
#define TDS_LEVEL3   700+20+120//650-30   //850
#define TDS_LEVEL4   1100+20+30+50//-100  //1000
#define TDS_LEVEL5   1100-100 //1000
#define TDS_LEVEL6   1200-50*/


#define FLOW_PERIOD		1
#define FLOW_FACTOR	60000/FLOW_PERIOD
#define PUMP_VALUE    65535


#define FLOW_VALUE 2
#define FLOW_RATIO  575//575//650.0//575.0  // 575 499  537


#define FLOW_THRES   0.5
#define WATER_H	0   // high water level
#define WATER_L	1  // low level
#define WATER_M	2 // middle level
#define WATER_F	3 //sensor fault
#define SALT_TIMEOUT   0xffffffff

#define WATER_LEVEL 0   //water level voltage level
#define FIR_NUM          20  //filter 1
#define FIR_NUM_FLOW     5  //filter 1
#define FIR_NUM_MAX      100 // filter 2

#define MAX_FLOW  7 //
#define MIN_FLOW  0

#define MAX_PH    5
#define MIN_PH    0

#define MAX_ORP   1550   
#define MIN_ORP   -100

#define MAX_TDS_VALUE  1700 //进水tds阈值300----  1300
#define MIN_TDS_VALUE  300

#define MAX_TDS1_VALUE  150//150  //原水tds阈值 150
#define MIN_TDS1_VALUE  0


//#define MAX_TDS_VALUE  10
//#define MIN_TDS_VALUE  0


#define TDS_TEMP_FACTOR 0.99f								// TDS温度补偿系数 20摄氏度标准值




#define MAX_LONG_TICK	    1000

#define MAX_SHORT_TICK	    5000
#define MAX_TICK	        90000
#define WATER_LSHORT_TICK	1000
#define WATER_L_DELAY_TICK	1*60*1000


#define WASH_TIME_SETTING        3*60000   //wash time setting
#define INIT_WASH_TIME_SETTING   15000    // init wash time setting 

//#define TX_PERIOD   1000
#define WASH_TIME   30000

#define ADC_SIZE    2
#define MODULE_SIZE  4


#define NORMAL_INDEX  				0
#define TDS1_INDEX  				1
#define TDS2_INDEX  				2
#define FLOW_INDEX  				3
#define ORP_INDEX  					4
#define HSW_INDEX  					5
#define WATER_LEVEL_INDEX  			6
#define ELE_INDEX  					7
#define WASH_INDEX  				8
#define PH_INDEX  					9
#define FACTORY_INDEX  				10
#define NOWATER_INDEX  				11
#define SYSTEM_INDEX  				12

#define SHUNT_INDEX  				13
#define WORK_STATUS_SIZE  			15
#define MAX_WASH_TIME  				3

#define MAX_TX_TIMES   				3

typedef struct
{
    unsigned char swH;
    unsigned char water_level;
    float tds1;
    float tds2;
    float tds3;

    double  flow;
	float  last_sum_ele_time;
	float last_water_quantity;

	float  sum_ele_time;
	float water_quantity;
    double temperature;
    double ph;
    double orp;
    double vcc;
    unsigned int warn;
    double ele_curr;
    float ele_Mcurr[MODULE_SIZE];
    float ele_MOnLine[MODULE_SIZE];
    unsigned char ele_offLine_T[MODULE_SIZE];
    float ele_EleStatus[MODULE_SIZE];
    float orp_ph_adc;
    uint32_t delay_timeout;
    uint32_t salt_water_timeout;
    unsigned char wash_time;
    unsigned char sum_module;
    unsigned char err_flag;
    unsigned char water_status;
    unsigned char status[WORK_STATUS_SIZE];// 20:正常  1：tds1 2:tds2 3：流量 4：orp
    //  5:高压开关6：水位开关 7:电解中8:清洗中

} sensor_stru;
void GetWaterLevel(void);
void sensor_process(void);
sensor_stru *GetSensor(void);
modbus_pack_stru *GetModbusPack(void);
void adcInit(void);
void adcResume(void);
uint16_t *get_adc_buf(void);
void GetModbusSens(unsigned char addr,unsigned char func,unsigned int reg,unsigned int regCount,unsigned char *buf,unsigned char datcount);
void setState(unsigned char setvealue, unsigned char addr);
void GetFlow(void);

#endif
