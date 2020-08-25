#ifndef MODBUS_H
#define MODBUS_H
#include "main.h"

#define MODBUS_BSIZE	1024
typedef struct{
u32 RS485_Baudrate;//ͨѶ������
u8 RS485_Parity;//0��У�飻1��У�飻2żУ��
u8 RS485_Addr;//�ӻ���ַ
u16 RS485_Frame_Distance;//����֡��С�����ms),������ʱ������Ϊ����һ֡
u8  RS485_TX_EN;
u8 RS485_RX_BUFF[MODBUS_BSIZE];//���ջ�����2048�ֽ�
u16 RS485_RX_CNT;//���ռ�����
u8 RS485_FrameFlag;//֡�������
u8 RS485_TX_BUFF[MODBUS_BSIZE];//���ͻ�����
u16 RS485_TX_CNT;//���ͼ�����
}modbus_stru;

void RS485_Init(void);
void RS485_Service(void);

#endif
