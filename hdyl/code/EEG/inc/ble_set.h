#ifndef BLE_SET_H
#define BLE_SET_H
#include "main.h"

#define Success 0
#define Failure 1

#define CONFIG_MODE    0
#define TRANSMIMT_MODE 1
#define CONNECT_STA    2 
#define DIS_CONNECT  3
#define DATA_STA 4
#define WAKUP 5
#define SLEEP 6
unsigned int sendCommand(char *Command, char *Response, uint32_t Timeout, unsigned char Retry);
void BleModeSetting(unsigned char mode);
unsigned char BleConSeting(unsigned char mode);
#endif

