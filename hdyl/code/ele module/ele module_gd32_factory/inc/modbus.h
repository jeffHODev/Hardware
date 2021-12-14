#ifndef MODBUS_H
#define MODBUS_H
#include "main.h"
#define MIN_ADDR  1
#define MAX_ADDR  2

#define ORP_ADDR  1
#define PH_ADDR   2

#define UINT_TYPE		0
#define FLOAT_TYPE		1


#define FUNC_READ	0x03
#define FUNC_WRITE	0x06

#define MODBUS_BSIZE	32
typedef struct{
uint32_t RS485_Baudrate;//通讯波特率
uint8_t RS485_Parity;//0无校验；1奇校验；2偶校验
uint8_t RS485_Addr;//从机地址
uint16_t RS485_Frame_Distance;//数据帧最小间隔（ms),超过此时间则认为是下一帧
uint8_t  RS485_TX_EN;
uint8_t RS485_RX_BUFF[MODBUS_BSIZE];//接收缓冲区2048字节
uint8_t RS485_RX_BUFF_tmp[MODBUS_BSIZE];
uint16_t RS485_RX_CNT;//接收计数器
uint8_t RS485_FrameFlag;//帧结束标记
uint8_t RS485_TX_BUFF[MODBUS_BSIZE];//发送缓冲区
uint16_t RS485_TX_CNT;//发送计数器
	uint8_t modbus_data[4];//接收缓冲区2048字节
	uint8_t modbus_txdata[32];//接收缓冲区2048字节

}modbus_stru;

typedef struct{
uint8_t RS485_Addr;//从机地址
uint8_t func;
uint16_t startaddr;
uint16_t regnum;

unsigned char payload_len;
uint16_t datcount;
uint16_t crc;
uint8_t datType;
uint8_t ack;
uint8_t modbus_data[4];//接收缓冲区2048字节
uint8_t modbus_txdata[32];//接收缓冲区2048字节

}modbus_pack_stru;

void RS485_RxCpltCallback(void);
void RS485_Init(void);
unsigned char  RS485_Service(void);
modbus_pack_stru *GetModbusDat(void);
modbus_pack_stru *GetModbusPack(void);
modbus_stru *GetModbusPayLoad(void);
void Modbus_Pack(modbus_pack_stru *p);
unsigned char Modbus_06_Solve(void);
void buffer_reset(void);
#endif

