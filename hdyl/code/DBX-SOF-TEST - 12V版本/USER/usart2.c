/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************
 * 文件名  ：usart2.c
 * 描述    ：连接HW MG323模块       
 * 实验平台：STM32手持机开发板
 * 硬件连接：------------------------
 *          | PA2  - USART2(Tx)      |
 *          | PA3  - USART2(Rx)      |
 *           ------------------------
 * 库版本  ：ST3.0.0
 * 作者    ：小奕
 * 项目说明：RTU远程数据监控项目
 * 编写时间: 2014年03月25日
**********************************************************************************/
#include "usart2.h"

uint8_t Usart2_Tx_Buffer[USART2_TX_BUFFER];				// 串口2发送缓存
uint8_t Usart2_Rx_Buffer[USART2_RX_BUFFER];				// 串口2接收缓存A
uint8_t Usart2_NewFax[USART2_RX_BUFFER];					// 串口2获取一帧新数据缓存
uint8_t In_Usart2_RxBuf=0;												// 串口2接收指针入						 
uint8_t Out_Usart2_RxBuf=0;												// 串口2接收指针出 
uint8_t Usart2_Receive_Flage=0;										// 串口2发现新报文	
uint8_t Usart2_RX_Overtime=0;											// 串口2超时计数器  
uint8_t Usart2_RX_State=0;												// 串口2串口工作标志  =0 未工作  =1 正在工作
uint8_t Usart2_TX_State=0;												// 串口2串口工作标志  =0 未工作  =1 正在工作
/***********************************************************************************
 * 函数名：USART2_Config
 * 描述  ：USART2 GPIO 配置,工作模式配置。9600 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
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
	USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);			// 接收中断
	USART_ITConfig(USART2,USART_IT_TC,DISABLE);				// 发送完成中断
	USART_ITConfig(USART2, USART_IT_IDLE,ENABLE);			// 总线空闲中断
	USART_Cmd(USART2, ENABLE);															// 使能USART2
		
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
 * 函数名：USART2_SendBuffer(char *buffer, uint16_t n)
 * 描述  ：获得串口2发送一个数据
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用
**********************************************************************************/
uint8_t  USART2_SendBuffer(char *buffer, uint16_t n)
{
	if(Usart2_TX_State==1) return 1;		// 设备正忙
	DMA_Cmd(DMA1_Channel7, DISABLE);
	memcpy(Usart2_Tx_Buffer, buffer, n);
	DMA1_Channel7->CNDTR=n;
	DMA1_Channel7->CMAR=(uint32_t)Usart2_Tx_Buffer;
	DMA_Cmd(DMA1_Channel7, ENABLE);
	return 0;
}

/***********************************************************************************
 * 函数名：uint8_t  USART2_SendDatas(uint16_t n)
 * 描述  ：获得串口2发送一个数据
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用
**********************************************************************************/
uint8_t  USART2_SendDatas(uint16_t n)
{
	if(Usart2_TX_State==1) return 1;		// 设备正忙
	DMA_Cmd(DMA1_Channel7, DISABLE);
	DMA1_Channel7->CNDTR=n;
	DMA1_Channel7->CMAR=(uint32_t)Usart2_Tx_Buffer;
	DMA_Cmd(DMA1_Channel7, ENABLE);
	return 0;
}
/***********************************************************************************
 * 函数名：Usart1_ISR
 * 描述  ：USART1 中断服务程序
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
**********************************************************************************/
void Usart2_ISR(void)
{
	// 总线空闲中断被使能
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		// 关DMA DMA1_Channel7
		DMA_Cmd(DMA1_Channel7, DISABLE);
		// 清除中断标志
		DMA_ClearFlag(DMA1_FLAG_TC7); 
		// 读取数据清中断		
		USART_ReceiveData(USART2);			
		// 计算接收到数据个数 		
		In_Usart2_RxBuf = USART2_RX_BUFFER - DMA_GetCurrDataCounter(DMA1_Channel7);		
		// 接收到数据标志
		Usart2_Receive_Flage=1;																				
		// 将接收到的数据拷贝
		memcpy(Usart2_NewFax, Usart2_Rx_Buffer, In_Usart2_RxBuf);
		// 重新指向DMA内存数据
//		DMA1_Channel7->CMAR = (uint32_t)Usart2_Rx_Buffer;
		// 重新定义接收数据个数
		DMA1_Channel7->CNDTR=USART2_RX_BUFFER	;
		// 启动DMA DMA1_Channel5
		DMA_Cmd(DMA1_Channel7, ENABLE);
	}
	USART_ClearITPendingBit(USART2, USART_IT_IDLE | USART_IT_PE | USART_IT_FE | USART_IT_NE);	
}



/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/

