/*!
    \file    gd32f4xx_libopt.h
    \brief   library optional for gd32f4xx

    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/


#include "gd32f4xx_it.h"
#include "protocol.h"

uint32_t sys_tick;
uint8_t UART3_DMA_Finish=1;
//extern FlagStatus g_transfer_complete;

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{

    sys_tick++;
	  reapte_task();

}

/*!
    \brief      this function handles DMA1_Channel7_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA1_Channel7_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA1, DMA_CH7, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA1, DMA_CH7, DMA_INT_FLAG_FTF);
        ;// g_transfer_complete = SET;
    }
}
void DMA0_Channel4_IRQHandler(void)
{
    if(dma_interrupt_flag_get(DMA0, DMA_CH4, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH4, DMA_INT_FLAG_FTF);
        UART3_DMA_Finish=1;
			//	nvic_irq_enable(EXTI1_IRQn, 2U, 0U);
			//	nvic_irq_disable(EXTI10_15_IRQn);

    }
}
/*!
    \brief      this function handles DMA0_Channel4_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA1_Channel2_IRQHandler(void)
{
   
	    if(dma_interrupt_flag_get(DMA1, DMA_CH2, DMA_INT_FLAG_FTF))
    {
		dma_interrupt_flag_clear(DMA1, DMA_CH2, DMA_INT_FLAG_FTF);

        ;//g_transfer_complete = SET;
    }
}
void DMA0_Channel2_IRQHandler(void)
{
    dma_interrupt_flag_clear(DMA0, DMA_CH2, DMA_INT_FLAG_FTF);
    if(dma_interrupt_flag_get(DMA0, DMA_CH2, DMA_INT_FLAG_FTF))
    {

        ;//g_transfer_complete = SET;
    }
}
unsigned char tmp2[128],i;
extern  uint32_t powerSleepTick;
void UART3_IRQHandler(void)
{
    dma_single_data_parameter_struct dma_single_data_parameter;
    if(RESET != usart_interrupt_flag_get(UART3, USART_INT_FLAG_IDLE)) //空闲中断
    {
        usart_interrupt_flag_clear(UART3,USART_INT_FLAG_IDLE);	/* 清除空闲中断标志位 */
        powerSleepTick = HAL_GetTick();
        tmp2[i++] = usart_data_receive(UART3);
			 packet_proc();
        uart3_rx_config();

    }

    if(RESET != usart_interrupt_flag_get(UART3, USART_INT_FLAG_TC)) //空闲中断
    {
        usart_interrupt_flag_clear(UART3,USART_INT_FLAG_TC);	/* 清除空闲中断标志位 */

        HAL_UART_TxCpltCallback();
        // RS485_RxCpltCallback();						/* 清除接收完成标志位 */
    }

}
void DMA0_Channel6_IRQHandler(void)
{
   
	    if(dma_interrupt_flag_get(DMA0, DMA_CH6, DMA_INT_FLAG_FTF))
    {
		dma_interrupt_flag_clear(DMA0, DMA_CH6, DMA_INT_FLAG_FTF);

        ;//g_transfer_complete = SET;
    }
}
void USART1_IRQHandler(void)
{

    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_TC)) //空闲中断
    {
        usart_interrupt_flag_clear(USART1,USART_INT_FLAG_TC);	/* 清除空闲中断标志位 */

        //HAL_UART_TxCpltCallback();
        // RS485_RxCpltCallback();						/* 清除接收完成标志位 */
    }


}

void TIMER2_IRQHandler(void)
{

}
void EXTI5_9_IRQHandler(void)
{
    if(  exti_interrupt_flag_get(EXTI_6)==1)//数据接收中断
    {
        key_proc();
        exti_interrupt_flag_clear(EXTI_6); //清除LINE上的中断标志位
    }
}

