#ifndef SYS_H
#define SYS_H
#include "main.h"
//#define WASH_TICK_NO  12
//#define FLOW_TICK_NO  	0
//#define TDS_TICK_NO  	1
//#define ORP_TICK_NO  	2
//#define PH_TICK_NO  	3
//#define WATER_TICK_NO   4
//#define SW_TICK_NO  	5
//#define StART_TICK_NO  	6
//#define SENSOR_TICK_NO  7
//#define WIFI_TICK       
#define WASH_TICK_NO   0
#define CURR_TICK       1
#define LOWCURR_TICK       2
#define ABNORMAL_TICK       3
#define PID_OUT_TICK      4 
#define TICK_SIZE       9
//#define WASH_TIME_SETTING        5*60000   //wash time setting
typedef struct
{
	unsigned char TickNum[TICK_SIZE];
	uint32_t TickTime[TICK_SIZE];
	unsigned char TickStatus[TICK_SIZE];
	uint32_t TickTimeTmp[TICK_SIZE];	
	unsigned char Tickmode[TICK_SIZE];
}tick_stru;

unsigned char GetTickResult(unsigned char TickNo);
void TickPro(void);
unsigned char registerTick(unsigned char TickNo,uint32_t ms,unsigned char status,unsigned char mode);
unsigned char GetTickStatus(unsigned char TickNo);
void delay_ms(uint32_t count);
uint32_t HAL_GetTick(void);
void systick_config(void);

#endif

