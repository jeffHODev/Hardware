/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp.h"
#include "app_ble.h"
#include "ADS129x.h"
#include "sys.h"
#include "ecg.h"
#include "protocol.h"
#include "mmr901mx.h"
#include "nibp_if.h"
#include "ecg.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

//extern static unsigned char init_lcd;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void rcu_config(void);
void gpio_config(void);
void nvic_config(void);
void dma_config(void);
void adc_config(void);
void spi_config(void);
void uart0_config(void);
void uart1_config(void);
void uart3_config(void);


extern  uint8_t buffer_rx2[16];
uint32_t test = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    rcu_config(); 
	test = 1;
    systick_config();test = 2;delay_ms(2000);
    gpio_config();test = 3;
	
   
    spi_config();test = 4;
	uart0_config();test = 5;
	uart1_config();test = 6;
	uart3_config();	test = 7;
    dma_config();  test = 8;
	 adc_config();test = 9;
    timer_config(TIMER3, 1);test = 10;
	 
   //ADS129x_ReInit(0);
	test = 11;
	 app_init();
    protocol_init();
    ecg_init();
	  test = 12;
//	MMR901MX_Init();
    nibp_if_init();
    while (1)
    {
     test = 12;
     app();
     test = 13;
        //usart_data_transmit(USART2, 0x5a);
    }
    /* USER CODE END 3 */
}

void rcu_config(void)
{
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA1);
	rcu_periph_clock_enable(RCU_DMA0);
    /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV6);
	  rcu_periph_clock_enable(RCU_SPI1);
}
void gpio_config(void)
{

    /* enable the key clock */
    rcu_periph_clock_enable(RCU_SYSCFG);

    //****************************ble wake up pin********************************************
    gpio_mode_set(ADS129x_DRDY_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN,ADS129x_DRDY_Pin);

    /* enable and set key EXTI interrupt to the lowest priority */
// nvic_irq_enable(EXTI1_IRQn, 2U, 0U);

    /* connect key EXTI line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN1);

    /* configure key EXTI line */
    exti_init(EXTI_1, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_1);

    gpio_mode_set(ADS129x_DRDY2_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN,ADS129x_DRDY2_Pin);

    /* enable and set key EXTI interrupt to the lowest priority */
// nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);

    /* connect key EXTI line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOE, EXTI_SOURCE_PIN15);

    /* configure key EXTI line */
    exti_init(EXTI_15, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_15);



    //*******************************key pin *************************************
    /* configure button pin as input */
    gpio_mode_set(KEY1_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN,KEY1_Pin||KEY2_Pin);

    /* enable and set key EXTI interrupt to the lowest priority */
    nvic_irq_enable(EXTI5_9_IRQn, 2U, 0U);

    /* connect key EXTI line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN7||EXTI_SOURCE_PIN6);

    /* configure key EXTI line */
    exti_init(EXTI_6, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    //exti_init(EXTI_7, EXTI_INTERRUPT, EXTI_TRIG_RISING);
   // exti_interrupt_flag_clear(EXTI_7);
    exti_interrupt_flag_clear(EXTI_6);







    /* config the GPIO as analog mode, for ADC */
    gpio_mode_set(MMR_RD_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE,MMR_RD_Pin);
    gpio_mode_set(BDATA_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE,BDATA_Pin);
    gpio_mode_set(BLINK_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE,BLINK_Pin);
    gpio_mode_set(BLINK_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE,BLINK_Pin);
    gpio_mode_set(BDISC_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,BDISC_Pin);
    gpio_output_options_set(ADS129x_DRDY2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,ADS129x_DRDY2_Pin);
    gpio_mode_set(BWKP_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,BWKP_Pin);
    gpio_output_options_set(BWKP_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,BWKP_Pin);

    gpio_mode_set(BMOD_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,BMOD_Pin);
    gpio_output_options_set(BMOD_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,BMOD_Pin);

    gpio_mode_set(VBAT_STA_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE,VBAT_STA_Pin);


    gpio_mode_set(BRST_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,BRST_Pin);
    gpio_output_options_set(BRST_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,BRST_Pin);

    gpio_mode_set(ADS129x_DRDY_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE,ADS129x_DRDY_Pin);
    gpio_mode_set(POW_HEA_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,POW_HEA_Pin);
    gpio_output_options_set(POW_HEA_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,POW_HEA_Pin);

    gpio_mode_set(PRESS_EN_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,PRESS_EN_Pin);
    gpio_output_options_set(PRESS_EN_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,PRESS_EN_Pin);

    gpio_mode_set(POW_EN_5V_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,POW_EN_5V_Pin);
    gpio_output_options_set(POW_EN_5V_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,POW_EN_5V_Pin);

    gpio_mode_set(ADS129x_CS2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,ADS129x_CS2_Pin);
    gpio_output_options_set(ADS129x_CS2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,ADS129x_CS2_Pin);//ads1299 cs2
    gpio_mode_set(ADS129x_CS_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,ADS129x_CS_Pin);
    gpio_output_options_set(ADS129x_CS_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,ADS129x_CS_Pin);//ads1299 cs1

    gpio_mode_set(LED1_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,LED1_Pin);
    gpio_output_options_set(LED1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,LED1_Pin);

    gpio_mode_set(LED2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,LED2_Pin);
    gpio_output_options_set(LED2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,LED2_Pin);

    gpio_mode_set(BLE_EN_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,BLE_EN_Pin);
    gpio_output_options_set(BLE_EN_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,BLE_EN_Pin);


    gpio_mode_set(OUT1_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,OUT1_Pin);
    gpio_output_options_set(OUT1_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,OUT1_Pin);


    gpio_mode_set(OUT2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,OUT2_Pin);
    gpio_output_options_set(OUT2_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,OUT2_Pin);

    gpio_mode_set(OUT3_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,OUT3_Pin);
    gpio_output_options_set(OUT3_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,OUT3_Pin);


    gpio_mode_set(OUT4_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,OUT4_Pin);
    gpio_output_options_set(OUT4_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,OUT4_Pin);

    gpio_mode_set(MMR_CS_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,MMR_CS_Pin);
    gpio_output_options_set(MMR_CS_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,MMR_CS_Pin);

    gpio_mode_set(SPI2_CS_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,SPI2_CS_Pin);
    gpio_output_options_set(SPI2_CS_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,SPI2_CS_Pin);

    gpio_mode_set(VALUE_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,VALUE_Pin);
    gpio_output_options_set(VALUE_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,VALUE_Pin);

    gpio_mode_set(POW_EN_3V3_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,POW_EN_3V3_Pin);
    gpio_output_options_set(POW_EN_3V3_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,POW_EN_3V3_Pin);


	gpio_bit_write(BLE_EN_GPIO_Port, BLE_EN_Pin, SET);

    gpio_bit_write(BDISC_GPIO_Port, BDISC_Pin, RESET);
    gpio_bit_write(BMOD_GPIO_Port, BMOD_Pin, RESET);
    gpio_bit_write(BRST_GPIO_Port, BRST_Pin, RESET);
    gpio_bit_write(ADS129x_CS2_GPIO_Port, ADS129x_CS2_Pin, SET);
    gpio_bit_write(ADS129x_CS_GPIO_Port, ADS129x_CS_Pin, SET);
    gpio_bit_write(OUT1_GPIO_Port, OUT1_Pin, RESET);
    gpio_bit_write(OUT2_GPIO_Port, OUT2_Pin, RESET);
    gpio_bit_write(OUT3_GPIO_Port, OUT3_Pin, RESET);
    gpio_bit_write(OUT4_GPIO_Port, OUT4_Pin, RESET);
    gpio_bit_write(SPI2_CS_GPIO_Port, SPI2_CS_Pin, SET);
    gpio_bit_write(MMR_CS_GPIO_Port, MMR_CS_Pin, SET);
    gpio_bit_write(VALUE_GPIO_Port, VALUE_Pin, SET);

    gpio_bit_write(POW_EN_3V3_GPIO_Port, POW_EN_3V3_Pin, RESET);
    gpio_bit_write(POW_EN_5V_GPIO_Port, POW_EN_5V_Pin, RESET);
    gpio_bit_write(PRESS_EN_GPIO_Port, PRESS_EN_Pin, RESET);
    gpio_bit_write(POW_HEA_GPIO_Port, POW_HEA_Pin, RESET);
    gpio_bit_write(BLE_EN_GPIO_Port, BLE_EN_Pin, RESET);


    gpio_mode_set(SPEAK_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, SPEAK_Pin);
    gpio_output_options_set(SPEAK_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,SPEAK_Pin);
    gpio_af_set(SPEAK_GPIO_Port, GPIO_AF_1, SPEAK_Pin);

    gpio_mode_set(MOTOR_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, MOTOR_Pin);
    gpio_output_options_set(MOTOR_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,MOTOR_Pin);
    gpio_af_set(MOTOR_GPIO_Port, GPIO_AF_1, MOTOR_Pin);


    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|
                  GPIO_PIN_4|GPIO_PIN_5);
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,GPIO_PIN_4);


    /* SPI1 GPIO config */


	    gpio_af_set(GPIOB, GPIO_AF_5,  GPIO_PIN_15 | GPIO_PIN_13 |GPIO_PIN_14);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE,  GPIO_PIN_15 | GPIO_PIN_13 |GPIO_PIN_14);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15 | GPIO_PIN_13 |GPIO_PIN_14);





}

void timer_config(uint32_t timer_periph, uint32_t time_interval_ms) {
    timer_parameter_struct timer_initpara;

    switch(timer_periph){
    case TIMER0:
    	rcu_periph_clock_enable(RCU_TIMER0);
    	break;
    case TIMER1:
        rcu_periph_clock_enable(RCU_TIMER1);
        break;
    case TIMER2:
        rcu_periph_clock_enable(RCU_TIMER2);
        break;
    case TIMER3:
        rcu_periph_clock_enable(RCU_TIMER3);
        break;
    case TIMER4:
        rcu_periph_clock_enable(RCU_TIMER4);
        break;
    case TIMER5:
        rcu_periph_clock_enable(RCU_TIMER5);
        break;
    case TIMER6:
        rcu_periph_clock_enable(RCU_TIMER6);
        break;
    default:
    	break;
    }

    timer_deinit(timer_periph);
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler         = 11999; //10799;	//108M/10800 = 10K Hz
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = (uint32_t)10*time_interval_ms;//(uint32_t)1000000U/time_interval_us;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(timer_periph, &timer_initpara);
	
	
	

    
   
}


void dma_config()
{
    /* ADC_DMA_channel configuration */
    dma_single_data_parameter_struct dma_single_data_parameter;
    /* enable DMA1 */
    rcu_periph_clock_enable(RCU_DMA0);
    /* ADC DMA_channel configuration */
    dma_deinit(DMA1, DMA_CH0);
	rcu_periph_clock_enable(RCU_DMA1);

    /* initialize DMA single data mode */
  /*  dma_single_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
    dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_single_data_parameter.memory0_addr = (uint32_t)&(getAdcBuf()->adc_value);
    dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
    dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;
    dma_single_data_parameter.number = 6;
    dma_single_data_parameter.priority = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH0, &dma_single_data_parameter);
    dma_channel_subperipheral_select(DMA1, DMA_CH0, DMA_SUBPERI0);
		dma_channel_enable(DMA1, DMA_CH0);

*/



		dma_deinit(DMA1, DMA_CH0);
		
		/* initialize DMA single data mode */
		dma_single_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
		dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
		dma_single_data_parameter.memory0_addr = (uint32_t)(getAdcBuf()->adc_value);
		dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
		dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
		dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;
		dma_single_data_parameter.number = 6;
		dma_single_data_parameter.priority = DMA_PRIORITY_HIGH;
		dma_single_data_mode_init(DMA1, DMA_CH0, &dma_single_data_parameter);
		dma_channel_subperipheral_select(DMA1, DMA_CH0, DMA_SUBPERI0);
		
		/* enable DMA circulation mode */
		dma_circulation_enable(DMA1, DMA_CH0);
		
		/* enable DMA channel */





		
		/* deinitialize DMA channel1 */
		dma_deinit(DMA0,DMA_CH4);
		dma_single_data_parameter.direction = DMA_MEMORY_TO_PERIPH;
		dma_single_data_parameter.memory0_addr = NULL;
		dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
		dma_single_data_parameter.periph_memory_width = DMA_MEMORY_WIDTH_8BIT;
		dma_single_data_parameter.number = 0;
		dma_single_data_parameter.periph_addr = (uint32_t)&USART_DATA(UART3);
		dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
		dma_single_data_parameter.priority = DMA_PRIORITY_ULTRA_HIGH;
		dma_single_data_mode_init(DMA0, DMA_CH4, &dma_single_data_parameter);
		
   /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH4);
    dma_channel_subperipheral_select(DMA0, DMA_CH4, DMA_SUBPERI4);
    dma_interrupt_enable(DMA0, DMA_CH4, DMA_CHXCTL_FTFIE);


	 
	 /* deinitialize DMA1 channel2 (USART0 rx) */
	 dma_deinit(DMA0, DMA_CH2);
	 dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;
	 dma_single_data_parameter.memory0_addr = (uint32_t)buffer_rx2;
	 dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	 dma_single_data_parameter.number = 16;
	 dma_single_data_parameter.periph_addr = (uint32_t)&USART_DATA(UART3);
	 dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	 dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
	 dma_single_data_parameter.priority = DMA_PRIORITY_ULTRA_HIGH;
	 dma_single_data_mode_init(DMA0, DMA_CH2, &dma_single_data_parameter);
	 
	 /* configure DMA mode */
	 dma_circulation_enable(DMA0, DMA_CH2);
	 dma_channel_subperipheral_select(DMA0, DMA_CH2, DMA_SUBPERI4);
	 /* enable DMA1 channel2 transfer complete interrupt */
	 dma_interrupt_enable(DMA0, DMA_CH2, DMA_CHXCTL_FTFIE);
	 /* enable DMA1 channel2 */
	 dma_channel_enable(DMA0, DMA_CH2);

    /* USART DMA enable for transmission and reception */
    //usart_dma_transmit_config(USART0, USART_DENT_ENABLE);


    /* wait DMA Channel transfer complete */
 //   while(RESET == dma_flag_get(DMA1, DMA_CH7, DMA_FLAG_FTF));
    /* deinitialize DMA channel2 (USART0 rx) */
//		   dma_deinit(DMA0, DMA_CH4);
//		   dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;
//		   dma_single_data_parameter.memory0_addr = (uint32_t)rxbuffer;
//		   dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
//		   dma_single_data_parameter.number = 10;
//		   dma_single_data_parameter.periph_addr = (uint32_t)&USART_DATA(USART0);
//		   dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
//		   dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
//		   dma_single_data_parameter.priority = DMA_PRIORITY_ULTRA_HIGH;
//		   dma_single_data_mode_init(DMA1, DMA_CH4, &dma_init_struct);
//		   /* configure DMA mode */
//		   dma_circulation_disable(DMA0, DMA_CH4);
//		   dma_channel_subperipheral_select(DMA0, DMA_CH4, DMA_SUBPERI4);
    /* enable DMA channel2 */
    // dma_channel_enable(DMA0, DMA_CH2);
    //usart_dma_receive_config(USART0, USART_DENR_ENABLE);

}
void uart0_config(void)
{
    rcu_periph_clock_enable(RCU_USART0);

    /* configure the USART4 Tx pin and USART0 Rx pin */
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);

    /* configure USART4 Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    /* configure USART0 Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(((USART0)), 115200);
    usart_receive_config(((USART0)), USART_RECEIVE_ENABLE);
    usart_transmit_config(((USART0)), USART_TRANSMIT_ENABLE);
    usart_enable(((USART0)));
    //nvic_irq_enable(USART1_IRQn, 0, 0);
    // usart_interrupt_enable(USART1, USART_INT_IDLE);
    // usart_interrupt_enable(USART1, USART_INT_TC);
    /* USART DMA enable*/


}

void uart1_config(void)
{
    rcu_periph_clock_enable(RCU_USART1);

    /* configure the USART4 Tx pin and USART0 Rx pin */
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_2);
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_3);

    /* configure USART4 Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    /* configure USART0 Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(((USART1)), 115200);
    usart_receive_config(((USART1)), USART_RECEIVE_ENABLE);
    usart_transmit_config(((USART1)), USART_TRANSMIT_ENABLE);
    usart_enable(((USART1)));
    //nvic_irq_enable(USART1_IRQn, 0, 0);
    // usart_interrupt_enable(USART1, USART_INT_IDLE);
    // usart_interrupt_enable(USART1, USART_INT_TC);
    /* USART DMA enable*/


}

void uart3_config(void)
{
    rcu_periph_clock_enable(RCU_UART3);

    /* configure the USART4 Tx pin and USART0 Rx pin */
    gpio_af_set(GPIOA, GPIO_AF_8, GPIO_PIN_0);
    gpio_af_set(GPIOA, GPIO_AF_8, GPIO_PIN_1);

    /* configure USART4 Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);

    /* configure USART0 Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);

    /* USART configure */
    usart_deinit(UART3);
    usart_baudrate_set(((UART3)), 460800);
    usart_receive_config(((UART3)), USART_RECEIVE_ENABLE);
    usart_transmit_config(((UART3)), USART_TRANSMIT_ENABLE);
    usart_enable(((UART3)));
    nvic_irq_enable(UART3_IRQn, 0, 0);
    usart_interrupt_enable(UART3, USART_INT_IDLE);
    /* USART DMA enable*/
    usart_dma_receive_config(UART3, USART_DENR_ENABLE);
    usart_dma_transmit_config(UART3, USART_DENT_ENABLE);

usart_interrupt_enable(UART3, USART_INT_FLAG_TC);

//    nvic_irq_enable(UART3_IRQn, 0, 0);
//    usart_interrupt_enable(UART3, USART_INT_RBNE);


}

void spi_config(void)
{
    spi_parameter_struct spi_init_struct;

    /* SPI1 parameter config */
   /* spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_32;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init_struct.nss         = SPI_NSS_SOFT;
    spi_init(SPI1, &spi_init_struct);

*/



    /* SPI1 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_32;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI1, &spi_init_struct);

	spi_enable(SPI1);






}
void adc_config(void)
{

	adc_deinit();

	/* ADC mode config */
	   adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
	   /* ADC contineous function disable */
	   adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
	   /* ADC scan mode disable */
	   adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
	   /* ADC data alignment config */
	   adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
	   
	   /* ADC channel length config */
	   //adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 4);
	   /* ADC regular channel config */
	   /* ADC channel length config */
	   adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 6);
	   /* ADC regular channel config */
	   adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_10, ADC_SAMPLETIME_56);//ta
	   adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_11, ADC_SAMPLETIME_56);//tb
	   adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_13, ADC_SAMPLETIME_56);//vbat
	   adc_regular_channel_config(ADC0, 3, ADC_CHANNEL_14, ADC_SAMPLETIME_56);//spo
	   adc_regular_channel_config(ADC0, 4, ADC_CHANNEL_15, ADC_SAMPLETIME_56);//br
	   adc_regular_channel_config(ADC0, 5, ADC_CHANNEL_4, ADC_SAMPLETIME_56);//hea
	   /* ADC trigger config */
	   adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_T0_CH0); 
	   adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
	   
	   /* ADC DMA function enable */
	   adc_dma_request_after_last_enable(ADC0);
	   adc_dma_mode_enable(ADC0);
	   
	   /* enable ADC interface */
	   adc_enable(ADC0);
	   /* wait for ADC stability */
	   //delay_1ms(1);
	   /* ADC calibration and reset calibration */
	   adc_calibration_enable(ADC0);
	   
	   /* enable ADC software trigger */
	  






}
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
