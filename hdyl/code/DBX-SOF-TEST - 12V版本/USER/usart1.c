/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************
 * �ļ���  ��usart1.c
 * ����    ����printf�����ض���USART1�������Ϳ�����printf��������Ƭ��������
 *           ��ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�         
 * ʵ��ƽ̨��STM32�ֳֻ�������
 * Ӳ�����ӣ�------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * ��汾  ��ST3.0.0
 * ����    ��С��
 * ��Ŀ˵����RTUԶ�����ݼ����Ŀ
 * ��дʱ��: 2014��03��25��
**********************************************************************************/
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"

uint8_t Usart1_Tx_Buffer[USART1_TX_BUFFER];			// ����1���ͻ���
uint8_t Usart1_Rx_Buffer[USART1_RX_BUFFER];			// ����1���ջ���A
uint8_t Usart1_NewFax[USART1_RX_BUFFER];				// ��ȡһ֡������
uint8_t In_Usart1_RxBuf=0;											// ����1����ָ����						 
uint8_t Out_Usart1_RxBuf=0;											// ����1����ָ��� 
uint8_t Usart1_Receive_Flage=0;									// �����±���	
uint8_t Usart1_RX_Overtime=0;										// ����1��ʱ������  
uint8_t Usart1_TX_State=0;											// ����1���ڹ�����־  =0 δ����  =1 ���ڹ���	
uint8_t Usart1_RX_State=0;											// ����1���ڹ�����־  =0 δ����  =1 ���ڹ���	

/***********************************************************************************
 * ��������USART1_Config
 * ����  ��USART1 GPIO ����,����ģʽ���á�115200 8-N-1
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
**********************************************************************************/
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/* USART1 GPIO config */
 	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// TRXD_SET PA8
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	// �����ж�
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);			// �����ж�
	USART_ITConfig(USART1,USART_IT_TC,DISABLE);				// ��������ж�
	USART_ITConfig(USART1, USART_IT_IDLE,ENABLE);			// ���߿����ж�
 	USART_Cmd(USART1, ENABLE);
//	USART_ClearFlag(USART1, USART_FLAG_TC);
	/* DMA clock enable */ 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//DMA1 
	/* DMA1 Channel4 (triggered by USART1 Tx event) Config */ 
	DMA_DeInit(DMA1_Channel4);   
	DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;  
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Usart1_Tx_Buffer; 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
	DMA_InitStructure.DMA_BufferSize = USART1_TX_BUFFER;  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	DMA_InitStructure.DMA_Mode =  DMA_Mode_Normal; 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);  
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE); 
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TE, ENABLE); 
	/* Enable USART1 DMA TX request */  
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); 
	DMA_Cmd(DMA1_Channel4, DISABLE);  
	/* DMA1 Channel5 (triggered by USART1 Rx event) Config */ 
	DMA_DeInit(DMA1_Channel5);   
	DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;  
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Usart1_Rx_Buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 										// ����Դ
	DMA_InitStructure.DMA_BufferSize = USART1_RX_BUFFER;  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);  
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE); 
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TE, ENABLE);   
	/* Enable USART1 DMA RX request */  
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); 
	DMA_Cmd(DMA1_Channel5, ENABLE); 
	TRXD_SET(0);		
}

/***********************************************************************************
 * ��������static char *itoa(int value, char *string, int radix)
 * ����  ������������ת�����ַ��� ��USART1_printf()����
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
************************************************************************************/
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

/*
 * ��������USART1_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���1����USART1
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART1_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART1_printf( USART1, "\r\n %s \r\n", j );
 */

/***********************************************************************************
 * ��������void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���1����USART1
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���ⲿ����
 *         ����Ӧ��USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART1_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART1_printf( USART1, "\r\n %s \r\n", j );
 void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
************************************************************************************/
uint8_t Usart1_Print(char *Data,...)
{
	const char *s;					// �ַ�
  int d;  								// ���� 
  char buf[16];						// �������
	char buffer[0xff];			// ���ת��������� ���255�������ַ�
	uint16_t num=0;					// ���ת�����ݵĸ���
	uint8_t state=0;

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( *Data == 0x5c )  // '\'
		{									  
			switch ( *++Data )
			{
				case 'r':	// �س���
									buffer[num++]=0x0d;
									Data ++;
									break;
				case 'n':	// ���з�
									buffer[num++]=0x0a;
									Data ++;
									break;			
				default:	// ��������
									Data ++;
									buffer[num++]=*Data;
									break;
			}// switch(*++Data)			 
		}
		else if ( *Data == '%')		// '%'
		{									
			switch ( *++Data )
			{				
				case 's':	// ����ַ���
									s = va_arg(ap, const char *);
									for ( ; *s; s++) 
									{
										buffer[num++]=*s;
									}
									Data++;
									break;

        case 'd': // ʮ����
									d = va_arg(ap, int);
									itoa(d, buf, 10);
									for (s = buf; *s; s++) 
									{
										buffer[num++]=*s;
									}
									Data++;
									break;
				default:	// ����
									Data++;
									buffer[num++]=*Data;
									break;
			}		 
		} /* end of else if */
		else buffer[num++]=*Data++;
	}
	// ���ӽ�������
	buffer[num++]=0xFF;buffer[num++]=0xFF;buffer[num++]=0xFF;// HMI��������
	// ��ת���õ����ݷ��ͳ�ȥ
	state=USART1_SendBuffer(buffer, num);
	
	return state;
}

/***********************************************************************************
 * ��������USART1_SendBuffer(char *buffer, uint16_t n)
 * ����  ����ô���1����һ������
 * ����  ����
 * ���  : ��ȡ�����ݷ���1 δ��ȡ�����򷵻�0
 * ����  ���ⲿ���� �����ַ���
**********************************************************************************/
uint8_t  USART1_SendBuffer(char *buffer, uint16_t n)
{
	if(Usart1_TX_State==1) return 1;
	TRXD_SET(1);
	DMA_Cmd(DMA1_Channel4, DISABLE);
	memcpy(Usart1_Tx_Buffer, buffer, n);
	DMA1_Channel4->CNDTR=n;
	DMA1_Channel4->CMAR=(uint32_t)Usart1_Tx_Buffer;
	DMA_Cmd(DMA1_Channel4, ENABLE);
	Usart1_TX_State=1;
	return 0;
}

/***********************************************************************************
 * ��������uint8_t  USART1_SendDatas(uint16_t n)
 * ����  ����ô���1����һ������
 * ����  ����
 * ���  : ��ȡ�����ݷ���1 δ��ȡ�����򷵻�0
 * ����  ���ⲿ����
**********************************************************************************/
uint8_t  USART1_SendDatas(uint16_t n)
{
	if(Usart1_TX_State==1) return 1;
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA1_Channel4->CNDTR=n;
	DMA1_Channel4->CMAR=(uint32_t)Usart1_Tx_Buffer;
	DMA_Cmd(DMA1_Channel4, ENABLE);
	Usart1_TX_State=1;
	return 0;
}
/***********************************************************************************
 * ��������Usart1_ISR
 * ����  ��USART1 �жϷ������
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
**********************************************************************************/
void Usart1_ISR(void)
{
	// ���߿����жϱ�ʹ��
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		// ��DMA DMA1_Channel5
		DMA_Cmd(DMA1_Channel5, DISABLE);
		// ����жϱ�־
		DMA_ClearFlag(DMA1_FLAG_TC5); 
		// ��ȡ�������ж�		
		USART_ReceiveData(USART1);			
		// ������յ����ݸ��� 		
		In_Usart1_RxBuf = USART1_RX_BUFFER - DMA_GetCurrDataCounter(DMA1_Channel5);		
		// ���յ����ݱ�־
		Usart1_Receive_Flage=1;																				
		// �����յ������ݿ���
		memcpy(Usart1_NewFax, Usart1_Rx_Buffer, In_Usart1_RxBuf);
		// ����ָ��DMA�ڴ�����
//		DMA1_Channel5->CMAR = (uint32_t)Usart1_Rx_Buffer;
		// ���¶���������ݸ���
		DMA1_Channel5->CNDTR=USART1_RX_BUFFER	;
		// ����DMA DMA1_Channel5
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
	USART_ClearITPendingBit(USART1, USART_IT_IDLE | USART_IT_PE | USART_IT_FE | USART_IT_NE);	
}


/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/

