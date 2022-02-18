#ifndef __USART_H
#define __USART_H
#include "sys.h" 
#include "myQueue.h"
#include "main.h"



//UART1 TX PA9 RX PA10
//UART2 TX PA2 RX PA3



//////////////////////////////////////////////////////////////////////////
#define UART_QUEUE_LENGTH	27 //��������������������Ч���ݳ���24*38=912 
#define UART_QUEUE_SIZE	5  //���д�С

extern QueueInfo *UART_Queue;//���л�����ָ��
//////////////////////////////////////////////////////////////////////////
#define UART1_DEBUG  1
#define USART_REC_LEN  			255  	////���ڽ��ջ�������С
//////////////////////////////////////////////////////////////////////////
#if UART1_DEBUG == 1
#define Main_printf(...) printf(__VA_ARGS__)
#else
#define Main_printf(...) 
#endif
//////////////////////////////////////////////////////////////////////////


/*extern uint8_t USART_RX_BUF[USART_REC_LEN] ;   //���ջ���                         
extern u16 USART_RX_STA;         		//����״̬���	

extern uint8_t USART2_RX_BUF[USART_REC_LEN] ;   //���ջ���   
extern u16 USART2_RX_LEN;

extern uint8_t USART3_RX_BUF[USART_REC_LEN] ;   //���ջ���   
extern u16 USART3_RX_LEN;




void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void uart4_init(u32 bound);
//void u2_printf(char* fmt,...);  
//void u3_printf(char* fmt,...);  
//void u4_printf(char* fmt,...);  


void USART2_Clear(void);
void USART3_Clear(void);*/


//void USARTx_Send(USART_TypeDef* USARTx, uint8_t *Data, u16 Len);  
int16_t Str_search(uint8_t* buf,uint8_t len,uint8_t* result);//�ж�ָ���ַ����Ƿ���������
int16_t char_search(uint8_t* buf,uint8_t len,uint8_t result);

#endif


