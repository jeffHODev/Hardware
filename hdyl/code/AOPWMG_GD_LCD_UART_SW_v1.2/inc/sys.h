#ifndef SYS_H
#define SYS_H
#include "main.h"
#define TICK_SIZE  25
#define FLOW_TICK_NO  	0
#define TDS_TICK_NO  	1
#define ORP_TICK_NO  	2
#define PH_TICK_NO  	3
#define WATER_TICK_NO   4
#define SW_TICK_NO  	5
#define StART_TICK_NO  	6
#define SENSOR_TICK_NO  7
#define LCD_TICK       8
#define WASH_TICK       9
#define CALIB_TICK       10
#define ADC_TICK         	  11
#define STOP_ELE_TICK         12
#define MODULE_RESET_TICK     13
#define TDS1_TICK_NO  	14
#define FLOW_TICK_NO2  	15
#define MODBUS_TIMEOUT_TICK_NO 16
#define TDS_SETTING_TICK_NO 17
#define PID_SETTING_TICK_NO 18
#define BEEP_TICK  	20
#define SW_TICK  	21
#define PUMP_TICK_NO  	22

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
void delay_us(uint32_t nCount);
unsigned char GetTickNum(unsigned char TickNo);
unsigned char setTickResult(unsigned char TickNo,unsigned char oper);
#endif

