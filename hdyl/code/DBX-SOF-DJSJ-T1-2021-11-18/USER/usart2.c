/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************
 * �ļ���  ��usart2.c
 * ����    ������HW MG323ģ��       
 * ʵ��ƽ̨��STM32�ֳֻ�������
 * Ӳ�����ӣ�------------------------
 *          | PA2  - USART2(Tx)      |
 *          | PA3  - USART2(Rx)      |
 *           ------------------------
 * ��汾  ��ST3.0.0
 * ����    ��С��
 * ��Ŀ˵����RTUԶ�����ݼ����Ŀ
 * ��дʱ��: 2014��03��25��
**********************************************************************************/
#include "usart2.h"

uint8_t Usart2_Tx_Buffer[USART2_TX_BUFFER];				// ����2���ͻ���
uint8_t Usart2_Rx_Buffer[USART2_RX_BUFFER];				// ����2���ջ���A
uint8_t Usart2_NewFax[USART2_RX_BUFFER];					// ����2��ȡһ֡�����ݻ���
uint8_t In_Usart2_RxBuf=0;												// ����2����ָ����						 
uint8_t Out_Usart2_RxBuf=0;												// ����2����ָ��� 
uint8_t Usart2_Receive_Flage=0;										// ����2�����±���	
uint8_t Usart2_RX_Overtime=0;											// ����2��ʱ������  
uint8_t Usart2_RX_State=0;												// ����2���ڹ�����־  =0 δ����  =1 ���ڹ���
uint8_t Usart2_TX_State=0;												// ����2���ڹ�����־  =0 δ����  =1 ���ڹ���
/***********************************************************************************
 * ��������USART2_Config
 * ����  ��USART2 GPIO ����,����ģʽ���á�9600 8-N-1
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
**********************************************************************************/
void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/* USART2 GPIO config */
 	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* config USART2 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 
	USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);			// �����ж�
	USART_ITConfig(USART2,USART_IT_TC,DISABLE);				// ��������ж�
	USART_ITConfig(USART2, USART_IT_IDLE,ENABLE);			// ���߿����ж�
	USART_Cmd(USART2, ENABLE);															// ʹ��USART2
		
	/* DMA1 Channel5 (triggered by USART2 Tx event) Config */
	DMA_DeInit(DMA1_Channel7);   
	DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40004404;  
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Usart2_Tx_Buffer; 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
	DMA_InitStructure.DMA_BufferSize = USART2_TX_BUFFER;  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);  
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE); 
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TE, ENABLE); 
	/* Enable USART1 DMA TX request */  
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); 
	DMA_Cmd(DMA1_Channel7, DISABLE);  
	/* DMA1 Channel6 (triggered by USART1 Rx event) Config */ 
	DMA_DeInit(DMA1_Channel6);   
	DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40004404;  
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Usart2_Rx_Buffer; 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
	DMA_InitStructure.DMA_BufferSize = USART2_RX_BUFFER;  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);  
	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE); 
	DMA_ITConfig(DMA1_Channel6, DMA_IT_TE, ENABLE); 
	/* Enable USART2 DMA RX request */  
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE); 
	DMA_Cmd(DMA1_Channel6, ENABLE); 	
}

/***********************************************************************************
 * ��������USART2_SendBuffer(char *buffer, uint16_t n)
 * ����  ����ô���2����һ������
 * ����  ����
 * ���  : ��ȡ�����ݷ���1 δ��ȡ�����򷵻�0
 * ����  ���ⲿ����
**********************************************************************************/
uint8_t  USART2_SendBuffer(char *buffer, uint16_t n)
{
	if(Usart2_TX_State==1) return 1;		// �豸��æ
	DMA_Cmd(DMA1_Channel7, DISABLE);
	memcpy(Usart2_Tx_Buffer, buffer, n);
	DMA1_Channel7->CNDTR=n;
	DMA1_Channel7->CMAR=(uint32_t)Usart2_Tx_Buffer;
	DMA_Cmd(DMA1_Channel7, ENABLE);
	return 0;
}

/***********************************************************************************
 * ��������uint8_t  USART2_SendDatas(uint16_t n)
 * ����  ����ô���2����һ������
 * ����  ����
 * ���  : ��ȡ�����ݷ���1 δ��ȡ�����򷵻�0
 * ����  ���ⲿ����
**********************************************************************************/
uint8_t  USART2_SendDatas(uint16_t n)
{
	if(Usart2_TX_State==1) return 1;		// �豸��æ
	DMA_Cmd(DMA1_Channel7, DISABLE);
	DMA1_Channel7->CNDTR=n;
	DMA1_Channel7->CMAR=(uint32_t)Usart2_Tx_Buffer;
	DMA_Cmd(DMA1_Channel7, ENABLE);
	return 0;
}
/***********************************************************************************
 * ��������Usart1_ISR
 * ����  ��USART1 �жϷ������
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
**********************************************************************************/
void Usart2_ISR(void)
{
	// ���߿����жϱ�ʹ��
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		// ��DMA DMA1_Channel7
		DMA_Cmd(DMA1_Channel7, DISABLE);
		// ����жϱ�־
		DMA_ClearFlag(DMA1_FLAG_TC7); 
		// ��ȡ�������ж�		
		USART_ReceiveData(USART2);			
		// ������յ����ݸ��� 		
		In_Usart2_RxBuf = USART2_RX_BUFFER - DMA_GetCurrDataCounter(DMA1_Channel7);		
		// ���յ����ݱ�־
		Usart2_Receive_Flage=1;																				
		// �����յ������ݿ���
		memcpy(Usart2_NewFax, Usart2_Rx_Buffer, In_Usart2_RxBuf);
		// ����ָ��DMA�ڴ�����
//		DMA1_Channel7->CMAR = (uint32_t)Usart2_Rx_Buffer;
		// ���¶���������ݸ���
		DMA1_Channel7->CNDTR=USART2_RX_BUFFER	;
		// ����DMA DMA1_Channel5
		DMA_Cmd(DMA1_Channel7, ENABLE);
	}
	USART_ClearITPendingBit(USART2, USART_IT_IDLE | USART_IT_PE | USART_IT_FE | USART_IT_NE);	
}



/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/

