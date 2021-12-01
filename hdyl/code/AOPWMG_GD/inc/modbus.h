#ifndef MODBUS_H
#define MODBUS_H
#include "main.h"
#define ORP_ADDR  1
#define PH_ADDR   2


#define MIN_ADDR  ORP_ADDR
#define MAX_ADDR  M4_ADDR



#define M1_ADDR   3
#define M2_ADDR   4
#define M3_ADDR   5
#define M4_ADDR   6



#define UINT_TYPE		0
#define FLOAT_TYPE		1

#define FUNC_WRITE	0x06

#define FUNC_READ	0x03

#define MODBUS_BSIZE	128
typedef struct{
u32 RS485_Baudrate;//通讯波特率
u8 RS485_Parity;//0无校验；1奇校验；2偶校验
u8 RS485_Addr;//从机地址
u16 RS485_Frame_Distance;//数据帧最小间隔（ms),超过此时间则认为是下一帧
u8  RS485_TX_EN;
u8 RS485_RX_BUFF[MODBUS_BSIZE];//接收缓冲区2048字节
u16 RS485_RX_CNT;//接收计数器
u8 RS485_FrameFlag;//帧结束标记
u8 RS485_TX_BUFF[MODBUS_BSIZE];//发送缓冲区
u16 RS485_TX_CNT;//发送计数器
}modbus_stru;

typedef struct{
u8 RS485_Addr;//从机地址
u8 func;
u16 startaddr;
u16 regnum;
u16 datcount;
u16 crc;
u8 datType;
u8 ack;
u8 modbus_data[4];//接收缓冲区2048字节
u8 modbus_txdata[32];//接收缓冲区2048字节

}modbus_pack_stru;


void RS485_Init(void);
void RS485_RxCpltCallback(void);
unsigned char  RS485_Service(void);
modbus_pack_stru *GetModbusDat(void);
void Modbus_Pack(modbus_pack_stru p);
modbus_stru *getModbusPayload(void);
#endif

