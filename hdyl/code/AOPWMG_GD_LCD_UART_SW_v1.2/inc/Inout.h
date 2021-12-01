#ifndef INOUT_H
#define INOUT_H
#include "main.h"
#define UART3_SIZE		32
typedef struct
{
    unsigned char Usart3_NewFax[UART3_SIZE];
    unsigned  char 	key_wash_mode;
    unsigned  char 	key_reset_mode;
    unsigned  char 	key_wifi_mode;
    unsigned char key_cali_mode;
    unsigned char key_cali_type;
    unsigned char key_cali_value;
    unsigned  char receivedFlag;
	unsigned char update;
    float orb_cali_value;
} inout_stru;
typedef struct
{
    unsigned char command;
    unsigned char control_id;
	  unsigned char last_ctrl_id;
    unsigned char page_id;
    unsigned char status;
    unsigned char value;
    unsigned char key;	
    unsigned char touch_flag;
	unsigned char last_page_id;
	  unsigned char str[20];
	unsigned char next_page;
	unsigned char factory_count;
} touch_stru;
uint8_t Send_LED_Data(void);
uint8_t System_Read_Fax(void);
void inout_Init(void);
void inout_callback(void);
inout_stru *GetInOut(void);
void Soft_Reset(void);
touch_stru *getTouch(void);
uint8_t touch_pro(void);
touch_stru *GetTouchInOut(void);


#endif

