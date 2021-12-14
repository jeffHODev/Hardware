/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************
 * 文件名  ：usart3.c
 * 描述    ：串口3用于和锁芯进行通信      
 * 实验平台：STM32手持机开发板
 * 硬件连接：------------------------
 *          | PB10  - USART3(Tx)     |
 *          | PB11  - USART3(Rx)     |
 *           ------------------------
 * 库版本  ：ST3.0.0
 * 作者    ：小奕
 * 项目说明：RTU远程数据监控项目
 * 编写时间: 2014年03月25日
**********************************************************************************/
#include "usart3.h"

uint8_t Usart3_Tx_Buffer[USART3_TX_BUFFER];					// 串口3发送缓存
uint8_t Usart3_Rx_Buffer[USART3_RX_BUFFER];					// 串口3接收缓存A
uint8_t Usart3_NewFax[USART3_RX_BUFFER];						// 串口3获取一帧新数据缓存
uint8_t In_Usart3_RxBuf=0;													// 串口3接收指针入						 
uint8_t Out_Usart3_RxBuf=0;													// 串口3接收指针出 
uint8_t Usart3_Receive_Flage=0;											// 串口3发现新报文	
uint8_t Usart3_RX_Overtime=0;												// 串口3超时计数器  
uint8_t Usart3_RX_State=0;													// 串口3串口工作标志  =0 未工作  =1 正在工作
uint8_t Usart3_TX_State=0;													// 串口3串口工作标志  =0 未工作  =1 正在工作

//static DMA_InitTypeDef DMA_InitStructureTx;				// 
static DMA_InitTypeDef DMA_InitStructureTx;					// 
static DMA_InitTypeDef DMA_InitStructureRx;
/***********************************************************************************
 * 函数名：USART3_Config
 * 描述  ：USART3 GPIO 配置,工作模式配置。9600 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
**********************************************************************************/
void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* USART3 GPIO config */
	/* Configure USART3 Tx (PB.10) as alternate function input  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PB.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* config USART3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure); 				// set
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);			// 接收中断
	USART_ITConfig(USART3,USART_IT_TC,ENABLE);				// 发送完成中断
	USART_ITConfig(USART3, USART_IT_IDLE,ENABLE);			// 总线空闲中断
	// DMA
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE); 
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE); 
  USART_Cmd(USART3, ENABLE);												// enable	
	
	/* DMA1 Channel2 (triggered by USART3 Tx event) Config */
	DMA_DeInit(DMA1_Channel2);   
	DMA_InitStructureTx.DMA_PeripheralBaseAddr = 0x40004804;  
	DMA_InitStructureTx.DMA_MemoryBaseAddr = (uint32_t)Usart3_Tx_Buffer; 
	DMA_InitStructureTx.DMA_DIR = DMA_DIR_PeripheralDST; 
	DMA_InitStructureTx.DMA_BufferSize = USART3_TX_BUFFER;  
	DMA_InitStructureTx.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructureTx.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStructureTx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
	DMA_InitStructureTx.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	DMA_InitStructureTx.DMA_Mode = DMA_Mode_Normal; 
	DMA_InitStructureTx.DMA_Priority = DMA_Priority_High; 
	DMA_InitStructureTx.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel2, &DMA_InitStructureTx);  
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE); 
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TE, ENABLE); 
	/* Enable USART3 DMA TX request */  
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE); 
	DMA_Cmd(DMA1_Channel2, DISABLE); 
	/* DMA1 Channel3 (triggered by USART3 Rx event) Config */ 
	DMA_DeInit(DMA1_Channel3);   
	DMA_InitStructureRx.DMA_PeripheralBaseAddr = 0x40004804;  
	DMA_InitStructureRx.DMA_MemoryBaseAddr = (uint32_t)Usart3_Rx_Buffer; 
	DMA_InitStructureRx.DMA_DIR = DMA_DIR_PeripheralSRC; 
	DMA_InitStructureRx.DMA_BufferSize = USART3_RX_BUFFER;  
	DMA_InitStructureRx.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructureRx.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStructureRx.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
	DMA_InitStructureRx.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	DMA_InitStructureRx.DMA_Mode = DMA_Mode_Normal; 
	DMA_InitStructureRx.DMA_Priority = DMA_Priority_Medium; 
	DMA_InitStructureRx.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel3, &DMA_InitStructureRx);  
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE); 
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TE, ENABLE); 
	/* Enable USART3 DMA RX request */  
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE); 
	DMA_Cmd(DMA1_Channel3, ENABLE); 
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
uint8_t Usart3_Print(char *Data,...)
{
	const char *s;					// 字符
  int d;  								// 数字 
  char buf[16];						// 存放数字
	char buffer[0xff];			// 存放转换后的数据 最大255个数据字符
	uint8_t ubuffer[0xff];	// 存放转换后的数据 最大255个数据字符
	uint16_t num=0,j=0;					// 存放转换数据的个数
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
	for(j=0;j<num;j++)
	{
		ubuffer[j]=buffer[j];
	}
	// 将转换好的数据发送出去
	state=USART3_SendBuffer(ubuffer, num);
	
	return state;
}

/***********************************************************************************
 * 函数名：USART3_SendBuffer(char *buffer, uint16_t n)
 * 描述  ：获得串口3发送一个数据
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用
**********************************************************************************/
uint8_t USART3_SendBuffer(uint8_t *buffer, uint16_t n)
{
	if(Usart3_TX_State==1) return 1;		// 设备正忙
	DMA_Cmd(DMA1_Channel2, DISABLE);
	memcpy(Usart3_Tx_Buffer, buffer, n);
	DMA1_Channel2->CNDTR=n;
	DMA1_Channel2->CMAR=(uint32_t)Usart3_Tx_Buffer;
	DMA_Cmd(DMA1_Channel2, ENABLE);
	return 0;
}

/***********************************************************************************
 * 函数名：uint8_t USART3_SendDatas(uint16_t n)
 * 描述  ：获得串口3发送一个数据
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用
**********************************************************************************/
uint8_t USART3_SendDatas(uint16_t n)
{
	if(Usart3_TX_State==1) return 1;		// 设备正忙
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA1_Channel2->CNDTR=n;
	DMA1_Channel2->CMAR=(uint32_t)Usart3_Tx_Buffer;
	DMA_Cmd(DMA1_Channel2, ENABLE);
	return 0;
}
/***********************************************************************************
 * 函数名：Usart1_ISR
 * 描述  ：USART1 中断服务程序
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
**********************************************************************************/
void USART3_IRQHandler(void)
{
	// 总线空闲中断被使能
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_IDLE);
		DMA_Cmd(DMA1_Channel3, DISABLE);
		USART_ReceiveData(USART3);		
		In_Usart3_RxBuf = USART3_RX_BUFFER - DMA_GetCurrDataCounter(DMA1_Channel3);		
		Usart3_Receive_Flage=1;
		memcpy(Usart3_NewFax, Usart3_Rx_Buffer, In_Usart3_RxBuf);
		DMA_DeInit(DMA1_Channel3);  
		DMA_Init(DMA1_Channel3, &DMA_InitStructureRx);  
		DMA_Cmd(DMA1_Channel3, ENABLE);			
	}
	else if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		USART_ClearFlag(USART3,USART_FLAG_TC);
		USART_ITConfig(USART3,USART_IT_TC,DISABLE);	
		DMA_Cmd(DMA1_Channel2,DISABLE); 
		Usart3_TX_State = 0;  	
	}	
}


/***********************************************************************************
 * 函数名：EM4305_Read_Data(uint8_t page,uint8_t data[4])
 * 描述  ：从卡中读取一页数据
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用
**********************************************************************************/
uint8_t EM4305_Read_Data(uint8_t page)
{
	uint8_t buf[10];
	uint8_t i,err=0,temp;

	if(page>15) page=0;
	// 填写数据
	buf[0]=0xAA;	buf[1]=0x0A;	buf[2]=0x02;	buf[3]=0x85;
	buf[4]=page;
	// 校验
	for(i=1;i<5;i++)
	{
		err^=buf[i];
	}
	buf[5]=err; buf[6]=0xBB;
	// 发送数据
	temp=USART3_SendBuffer(buf,7);
	
	return temp;
}

/***********************************************************************************
 * 函数名：EM4305_Read_Data(uint8_t page,uint8_t data[4])
 * 描述  ：从卡中读取一页数据
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用 AA 0A 06 84 05 12 34 56 78 85 BB
**********************************************************************************/
uint8_t EM4305_Write_Data(uint8_t page,uint8_t data[4])
{
	uint8_t buf[15];
	uint8_t i,err=0,temp;

	if(page>15) page=0;
	// 填写数据
	buf[0]=0xAA;	buf[1]=0x0A;	buf[2]=0x06;	buf[3]=0x84;
	buf[4]=page; 	buf[5]=data[0];	buf[6]=data[1];	buf[7]=data[2];	buf[8]=data[3];
	// 校验
	for(i=1;i<9;i++)
	{
		err^=buf[i];
	}
	buf[9]=err; buf[10]=0xBB;
	// 发送数据
	temp=USART3_SendBuffer(buf,11);
	
	return temp;
}






// Page 0 : 0x46 0x44 0x42 0x58   FDBX
// Page 1 : UID
// Page 2 : PASSWORD
// Page 3 : OPT
// Page 4 :	Config
// Page 5 :	2018				年
// Page 6 :	1021				月日
// Page 7 :	0000				序列号H
// Page 8 :	0001				序列号L
// Page 9 :	Config		  流量1	
// Page 10:	Config			流量2
// Page 11:	Config			流量3
// Page 12:	Config			流量4
// Page 13:	Config			0000 空 FFFF 废
// Page 14:	RP
// Page 15:	RP

/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/

