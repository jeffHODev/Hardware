/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************
 * 文件名  ：usart1.c
 * 描述    ：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据
 *           打印到PC上的超级终端或串口调试助手。         
 * 实验平台：STM32手持机开发板
 * 硬件连接：------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * 库版本  ：ST3.0.0
 * 作者    ：小奕
 * 项目说明：RTU远程数据监控项目
 * 编写时间: 2014年03月25日
**********************************************************************************/
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"

uint8_t Usart1_Tx_Buffer[USART1_TX_BUFFER];			// 串口1发送缓存
uint8_t Usart1_Rx_Buffer[USART1_RX_BUFFER];			// 串口1接收缓存A
uint8_t Usart1_NewFax[USART1_RX_BUFFER];				// 获取一帧新数据
uint8_t In_Usart1_RxBuf=0;											// 串口1接收指针入						 
uint8_t Out_Usart1_RxBuf=0;											// 串口1接收指针出 
uint8_t Usart1_Receive_Flage=0;									// 发现新报文	
uint8_t Usart1_RX_Overtime=0;										// 串口1超时计数器  
uint8_t Usart1_TX_State=0;											// 串口1串口工作标志  =0 未工作  =1 正在工作	
uint8_t Usart1_RX_State=0;											// 串口1串口工作标志  =0 未工作  =1 正在工作	

/***********************************************************************************
 * 函数名：USART1_Config
 * 描述  ：USART1 GPIO 配置,工作模式配置。115200 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
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
	// 设置中断
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);			// 接收中断
	USART_ITConfig(USART1,USART_IT_TC,DISABLE);				// 发送完成中断
	USART_ITConfig(USART1, USART_IT_IDLE,ENABLE);			// 总线空闲中断
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
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 										// 数据源
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
 * 函数名：static char *itoa(int value, char *string, int radix)
 * 描述  ：将整形数据转换成字符串 被USART1_printf()调用
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
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
 * 函数名：USART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART1_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART1_printf( USART1, "\r\n %s \r\n", j );
 */

/***********************************************************************************
 * 函数名：void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 调用  ：外部调用
 *         典型应用USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART1_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART1_printf( USART1, "\r\n %s \r\n", j );
 void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
************************************************************************************/
uint8_t Usart1_Print(char *Data,...)
{
	const char *s;					// 字符
  int d;  								// 数字 
  char buf[16];						// 存放数字
	char buffer[0xff];			// 存放转换后的数据 最大255个数据字符
	uint16_t num=0;					// 存放转换数据的个数
	uint8_t state=0;

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  // '\'
		{									  
			switch ( *++Data )
			{
				case 'r':	// 回车符
									buffer[num++]=0x0d;
									Data ++;
									break;
				case 'n':	// 换行符
									buffer[num++]=0x0a;
									Data ++;
									break;			
				default:	// 其他数据
									Data ++;
									buffer[num++]=*Data;
									break;
			}// switch(*++Data)			 
		}
		else if ( *Data == '%')		// '%'
		{									
			switch ( *++Data )
			{				
				case 's':	// 输出字符串
									s = va_arg(ap, const char *);
									for ( ; *s; s++) 
									{
										buffer[num++]=*s;
									}
									Data++;
									break;

        case 'd': // 十进制
									d = va_arg(ap, int);
									itoa(d, buf, 10);
									for (s = buf; *s; s++) 
									{
										buffer[num++]=*s;
									}
									Data++;
									break;
				default:	// 其他
									Data++;
									buffer[num++]=*Data;
									break;
			}		 
		} /* end of else if */
		else buffer[num++]=*Data++;
	}
	// 增加结束符号
	buffer[num++]=0xFF;buffer[num++]=0xFF;buffer[num++]=0xFF;// HMI结束符号
	// 将转换好的数据发送出去
	state=USART1_SendBuffer(buffer, num);
	
	return state;
}

/***********************************************************************************
 * 函数名：USART1_SendBuffer(char *buffer, uint16_t n)
 * 描述  ：获得串口1发送一个数据
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用 发送字符串
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
 * 函数名：uint8_t  USART1_SendDatas(uint16_t n)
 * 描述  ：获得串口1发送一个数据
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用
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
 * 函数名：Usart1_ISR
 * 描述  ：USART1 中断服务程序
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
**********************************************************************************/
void Usart1_ISR(void)
{
	// 总线空闲中断被使能
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		// 关DMA DMA1_Channel5
		DMA_Cmd(DMA1_Channel5, DISABLE);
		// 清除中断标志
		DMA_ClearFlag(DMA1_FLAG_TC5); 
		// 读取数据清中断		
		USART_ReceiveData(USART1);			
		// 计算接收到数据个数 		
		In_Usart1_RxBuf = USART1_RX_BUFFER - DMA_GetCurrDataCounter(DMA1_Channel5);		
		// 接收到数据标志
		Usart1_Receive_Flage=1;																				
		// 将接收到的数据拷贝
		memcpy(Usart1_NewFax, Usart1_Rx_Buffer, In_Usart1_RxBuf);
		// 重新指向DMA内存数据
//		DMA1_Channel5->CMAR = (uint32_t)Usart1_Rx_Buffer;
		// 重新定义接收数据个数
		DMA1_Channel5->CNDTR=USART1_RX_BUFFER	;
		// 启动DMA DMA1_Channel5
		DMA_Cmd(DMA1_Channel5, ENABLE);
	}
	USART_ClearITPendingBit(USART1, USART_IT_IDLE | USART_IT_PE | USART_IT_FE | USART_IT_NE);	
}


/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/

