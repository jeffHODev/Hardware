#ifndef SYS_H
#define SYS_H
#include "main.h"
#define TICK_SIZE  12
#define FLOW_TICK_NO  	0
#define TDS_TICK_NO  	1
#define ORP_TICK_NO  	2
#define PH_TICK_NO  	3
#define WATER_TICK_NO   4
#define SW_TICK_NO  	5
#define StART_TICK_NO  	6
#define SENSOR_TICK_NO  7
#define WIFI_TICK       8
#define WASH_TICK       9

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



#endif

