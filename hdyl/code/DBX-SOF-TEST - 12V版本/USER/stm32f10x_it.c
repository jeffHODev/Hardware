/**
  ******************************************************************************
  * @file    Project/Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    04/06/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "delay.h" 
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
	SysTick_ISR();
}


void USART1_IRQHandler(void)
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


void USART2_IRQHandler(void)
{
	// 总线空闲中断被使能
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		// 关DMA DMA1_Channel6
		DMA_Cmd(DMA1_Channel6, DISABLE);
		// 清除中断标志
		DMA_ClearFlag(DMA1_FLAG_TC6); 
		// 读取数据清中断		
		USART_ReceiveData(USART2);			
		// 计算接收到数据个数 		
		In_Usart2_RxBuf = USART2_RX_BUFFER - DMA_GetCurrDataCounter(DMA1_Channel6);		
		// 接收到数据标志
		Usart2_Receive_Flage=1;																				
		// 将接收到的数据拷贝
		memcpy(Usart2_NewFax, Usart2_Rx_Buffer, In_Usart2_RxBuf);
		// 重新指向DMA内存数据
//		DMA1_Channel6->CMAR = (uint32_t)Usart2_Rx_Buffer;
		// 重新定义接收数据个数
		DMA1_Channel6->CNDTR=USART2_RX_BUFFER	;
		// 启动DMA DMA1_Channel6
		DMA_Cmd(DMA1_Channel6, ENABLE);
	}
	USART_ClearITPendingBit(USART2, USART_IT_IDLE | USART_IT_PE | USART_IT_FE | USART_IT_NE);	
}

void USART3_IRQHandler(void)
{
	// 总线空闲中断被使能
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		// 关DMA DMA1_Channel3
		DMA_Cmd(DMA1_Channel3, DISABLE);
		// 清除中断标志
		DMA_ClearFlag(DMA1_FLAG_TC3); 
		// 读取数据清中断		
		USART_ReceiveData(USART3);			
		// 计算接收到数据个数 		
		In_Usart3_RxBuf = USART3_RX_BUFFER - DMA_GetCurrDataCounter(DMA1_Channel3);		
		// 接收到数据标志
		Usart3_Receive_Flage=1;																				
		// 将接收到的数据拷贝
		memcpy(Usart3_NewFax, Usart3_Rx_Buffer, In_Usart3_RxBuf);
		// 重新指向DMA内存数据
//		DMA1_Channel7->CMAR = (uint32_t)Usart2_Rx_Buffer;
		// 重新定义接收数据个数
		DMA1_Channel3->CNDTR=USART3_RX_BUFFER	;
		// 启动DMA DMA1_Channel3
		DMA_Cmd(DMA1_Channel3, ENABLE);
	}
	USART_ClearITPendingBit(USART3, USART_IT_IDLE | USART_IT_PE | USART_IT_FE | USART_IT_NE);		
}

/*******************************************************************************
* Function Name  : DMAChannel4_IRQHandler
* Description    : This function handles DMA Stream 1interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
// DMAChannel2_IRQHandler usart3_tx
void DMA1_Channel2_IRQHandler(void)
{      
	// 清除中断标志
	DMA_ClearFlag(DMA1_FLAG_TC2); 
	// 关闭DMA中断
	DMA_Cmd(DMA1_Channel2,DISABLE); 
	Usart3_TX_State= 0;                
}

/*******************************************************************************
* Function Name  : DMAChannel5_IRQHandler
* Description    : This function handles DMA Stream 1interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
// DMAChannel3_IRQHandler usart3_Rx
void DMA1_Channel3_IRQHandler(void)
{      
	// 清除中断标志
	DMA_ClearFlag(DMA1_FLAG_TC3); 
	// 关闭DMA中断
	DMA_Cmd(DMA1_Channel3,DISABLE);            
}

/*******************************************************************************
* Function Name  : DMAChannel4_IRQHandler
* Description    : This function handles DMA Stream 1interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
// DMAChannel4_IRQHandler usart1_tx
void DMA1_Channel4_IRQHandler(void)
{      
	// 清除中断标志
	DMA_ClearFlag(DMA1_FLAG_TC4); 
	// 关闭DMA中断
	DMA_Cmd(DMA1_Channel4,DISABLE); 
	Usart1_TX_State= 0;        
	TRXD_SET(0);	
}

/*******************************************************************************
* Function Name  : DMAChannel5_IRQHandler
* Description    : This function handles DMA Stream 1interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
// DMAChannel5_IRQHandler usart1_tx
void DMA1_Channel5_IRQHandler(void)
{      
	// 清除中断标志
	DMA_ClearFlag(DMA1_FLAG_TC5); 
	// 关闭DMA中断
	DMA_Cmd(DMA1_Channel5,DISABLE);            
}

/*******************************************************************************
* Function Name  : DMAChannel4_IRQHandler
* Description    : This function handles DMA Stream 1interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
// DMAChannel6_IRQHandler usart2_Rx
void DMA1_Channel6_IRQHandler(void)
{      
	// 清除中断标志
	DMA_ClearFlag(DMA1_FLAG_TC6); 
	// 关闭DMA中断
	DMA_Cmd(DMA1_Channel6,DISABLE); 
}

/*******************************************************************************
* Function Name  : DMAChannel5_IRQHandler
* Description    : This function handles DMA Stream 1interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
// DMAChannel7_IRQHandler usart2_tx
void DMA1_Channel7_IRQHandler(void)
{      
	// 清除中断标志
	DMA_ClearFlag(DMA1_FLAG_TC7); 
	// 关闭DMA中断
	DMA_Cmd(DMA1_Channel7,DISABLE);  
	Usart2_TX_State= 0;     		
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
