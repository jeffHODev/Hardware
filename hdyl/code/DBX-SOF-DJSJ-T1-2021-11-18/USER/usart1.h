#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "misc.h"

// PAGE0  USER						0x44 0x12 0x07 0x19    D 2018-07-29
// PAGE1  UID
// PAGE2  password
// PAGE3  USER	
// PAGE4  CONFIG
// PAGE5  USER	
// PAGE6  USER	
// PAGE7  USER	
// PAGE8  USER	
// PAGE9  USER	
// PAGE10 USER	
// PAGE11 USER
// PAGE12 USER
// PAGE13 USER
// PAGE14 USER
// PAGE15 USER
/*****************************�궨����********************************************/
#define	 USART1_TX_BUFFER				0xff												// ����1���ͻ���
#define	 USART1_RX_BUFFER				0xff												// ����1���ջ���A
#define  USART_NUM1             1
#define  USART_NUM2             2
#define  USART_NUM3             3
#define  RF_Write_UserInfo      0x84												// д��������Ϣ
#define  RF_Read_UserInfo				0x85												// ����������Ϣ
#define  RF_Wake_UP							0x86												// ��½
/**************************ȫ�ֱ���������******************************************/
extern uint8_t Usart1_Tx_Buffer[USART1_TX_BUFFER];					// ����1���ͻ���
extern uint8_t Usart1_Rx_Buffer[USART1_RX_BUFFER];					// ����1���ջ���A
extern uint8_t Usart1_NewFax[USART1_RX_BUFFER];							// ��ȡһ֡������
extern uint8_t In_Usart1_RxBuf;															// ����1����ָ����						 
extern uint8_t Out_Usart1_RxBuf;														// ����1����ָ��� 
extern uint8_t Usart1_Receive_Flage;												// �����±���	
extern uint8_t Usart1_RX_Overtime;													// ����1��ʱ������  
extern uint8_t Usart1_TX_State;															// ����1���ڹ�����־  =0 δ����  =1 ���ڹ���	
extern uint8_t Usart1_RX_State;															// ����1���ڹ�����־  =0 δ����  =1 ���ڹ���

typedef enum
{
    RF_ENOERR,                  /*!< no error. */
		RF_CMDERR,									/*����ִ��ʧ��*/
		RF_DATANOERR=0x80,					/*��ʶ�������óɹ�*/
    RF_WRITEERR,                /*д��ʧ��*/
    RF_READERR,                 /*����ʧ��*/
    RF_NOEXISTENT,              /*��������*/
    RF_MISMATCH,                /*����ƥ��*/
    RF_PARERR,                  /*��������*/
    RF_ERR=0x87,               	/*����������� */
    RF_REGERR=0x8F,             /*����ָ�����*/
		RF_IOERR,										/*io����*/
}
rfErro_Code;
/**************************�ⲿ����������******************************************/

void NVIC_Configuration(void);
void USART1_Config(void);																		// ����1���ú���
uint8_t Usart1_Print(char *Data,...);												// ��ӡ
uint8_t USART1_SendBuffer(uint8_t *buffer, uint16_t n);				// ͨ������1��������
uint8_t  USART1_SendDatas(uint16_t n);											// ��������
void Usart1_ISR(void);
rfErro_Code RF_Read_Page(uint8_t page);
rfErro_Code RF_Write_Page(uint8_t page,uint8_t *buffer);
rfErro_Code RF_Land(uint8_t page,uint8_t *buffer);
										//
#endif /* __USART1_H */

/******************** (C) COPYRIGHT 2014 ʢԪ���ӹ����� **************************/
