/*******************************************************************************
* @file    TIM4/TIM4_TimeBase/stm8l15x_it.c
* @author  MCD Application Team
* @version V1.4.0
* @date    09/24/2010
* @brief   Main Interrupt Service Routines.
*          This file provides template for all peripherals interrupt service routine.
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
* <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
*/

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_it.h"
#include "stm8l15x_rtc.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_tim1.h"
#include "uart_hal.h"
#include "Protocal.h"
/** @addtogroup STM8L15x_StdPeriph_Examples
* @{
*/

/** @addtogroup TIM4_TimeBase
* @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
* @brief Dummy interrupt routine
* @par Parameters:
* None
* @retval
* None
*/
INTERRUPT_HANDLER(NonHandledInterrupt, 0)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
#endif

/**
* @brief TRAP interrupt routine
* @par Parameters:
* None
* @retval
* None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief FLASH Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(FLASH_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief DMA1 channel0 and channel1 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief DMA1 channel2 and channel3 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief RTC / CSS_LSE Interrupt routine.
* @param  None
* @retval None
*/
unsigned char wake_up_flag;
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
   wake_up_flag =1;
  //RTC_WakeUpCmd(DISABLE);
  RTC_ClearITPendingBit(RTC_IT_WUT);
}
/**
* @brief External IT PORTE/F and PVD Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief External IT PORTB / PORTG Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTIB_G_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief External IT PORTD /PORTH Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTID_H_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  
}

/**
* @brief External IT PIN0 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI0_IRQHandler, 8)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief External IT PIN1 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI1_IRQHandler, 9)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  //LoraM1Flag = 1;
  // ExitInterFlag = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
  
  
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);
  //EXTI_ClearITPendingBit(EXTI_IT_Pin1);
}

/**
* @brief External IT PIN2 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI2_IRQHandler, 10)
{
  //LoraM0Flag = 1;
  //EXTI_ClearITPendingBit(EXTI_IT_Pin2);
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief External IT PIN3 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI3_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  
  // EXTI_ClearITPendingBit(EXTI_IT_Pin3);
}

/**
* @brief External IT PIN4 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI4_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief External IT PIN5 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI5_IRQHandler, 13)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief External IT PIN6 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI6_IRQHandler, 14)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  
}

/**
* @brief External IT PIN7 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(EXTI7_IRQHandler, 15)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief LCD /AES Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(LCD_AES_IRQHandler, 16)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief CLK switch/CSS/TIM1 break Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler, 17)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief ADC1/Comparator Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler, 18)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
* @param  None
* @retval None
*/
uart_stru uart2;
extern _SaveData Save_Data;
unsigned char j=0;
INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler, 20)
{
  unsigned char res;
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  unsigned char t;
  
  if(USART_GetFlagStatus(USART2, USART_FLAG_IDLE) != RESET)
  {
    t = USART2->SR;
    t = USART2->DR;//IDLE清零需要依次读SR 和DR 寄存器
    USART_ClearITPendingBit(USART2,  USART_IT_IDLE);	
  }
  else
  {
    res = USART_ReceiveData8(USART2);
    if(res == '$')
      j = 0;	
    uart2.rxbuffer[j++]=res;
    if(uart2.rxbuffer[0] == '$' && uart2.rxbuffer[4] == 'M' && uart2.rxbuffer[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
    {
      if(res == '\n')									   
      {
        uart2.received_flag =1;	
        memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
        memcpy(Save_Data.GPS_Buffer, uart2.rxbuffer, j); 	//保存数据
        Save_Data.isGetData = 1;
        j=0;
        memset( uart2.rxbuffer, 0, BUFFERSIZE);      //清空		
      }
      
    }
  }
  
  if(j>=BUFFERSIZE)
    j=0;
  
}


/**
* @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler, 21)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  

TIM3_ClearITPendingBit(TIM3_IT_Update);
  
}
/**
* @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler, 22)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler, 23)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief TIM1 Capture/Compare Interrupt routine.
* @param  None
* @retval None
*/

INTERRUPT_HANDLER(TIM1_CC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  
  
  
  
  
  
  
  
}

/**
* @brief TIM4 Update/Overflow/Trigger Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler, 25)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}
/**
* @brief SPI1 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(SPI1_IRQHandler, 26)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler, 27)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
* @param  None
* @retval None
*/
extern uart_stru uart;
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler, 28)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
  unsigned char t;
  if(USART_GetFlagStatus(USART1, USART_FLAG_IDLE) != RESET)
  {
    
    uart.received_flag =1;
    //uart_str.receive_flag ++;
    t = USART1->SR;
    t = USART1->DR;//IDLE清零需要依次读SR 和DR 寄存器
    USART_ClearITPendingBit(USART1,  USART_IT_IDLE);
    //uart_str.rx_len =  DMA_GetCurrDataCounter(DMA1_Channel2);
    DMA_START_RX();//重新开启串口DMA接收模式
    
  }
  
  
}

/**
* @brief I2C1 / SPI2 Interrupt routine.
* @param  None
* @retval None
*/
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler, 29)
{
  /* In order to detect unexpected events during development,
  it is recommended to set a breakpoint on the following instruction.
  */
}

/**
* @}
*/

/**
* @}
*/
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/