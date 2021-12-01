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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app.h"
#include "bsp_beep.h"
#include "sys.h"
#include "sensor.h"
#include "app_lcd.h"
#include "devCtrl.h"
#include "bsp_beep.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern uint32_t adc1_val_buf[5];
extern  uint8_t buf_cmd[LCD_BUF_SIZE];
//extern static unsigned char init_lcd;
/* USER CODE END PTD */
uint8_t rxbuffer[10];
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#if CPU == ST
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

SD_HandleTypeDef hsd;

SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart6_rx;

SRAM_HandleTypeDef hsram1;
#else
void rcu_config(void);
void gpio_config(void);
void nvic_config(void);
void dma_config(void);
void adc_config(void);
#endif
/* USER CODE BEGIN PV */
#if CPU == ST
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_ADC1_Init(void);
static void MX_FSMC_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_SPI3_Init(void);

/* USER CODE BEGIN PFP */
#endif
/* USER CODE END PFP */
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
static void MX_UART4_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);
//static void MX_DMA_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM1_Init(void);
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern uint8_t Speak_Flage;

unsigned char flag;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
#if CPU == ST
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */
#endif
    /* USER CODE END SysInit */
    rcu_config();
    gpio_config();
    systick_config();
    MX_USART6_UART_Init();
    MX_GPIO_Init();
    adc_config();
    /*configure DMA0 interrupt*/
    /* NVIC configuration */
    nvic_config();
    /* DMA configuration */
    dma_config();
    //gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT,GPIO_PUPD_NONE, GPIO_PIN_9);
    //gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ,GPIO_PIN_9);
    gpio_bit_reset(GPIOD, GPIO_PIN_9);
    /* ADC configuration */

    /* Initialize all configured peripherals */
    //
    //X_ADC1_Init();
    //_FSMC_Init();
    //_SDIO_SD_Init();
    //MX_SPI3_Init();
    MX_TIM3_Init();
    MX_UART4_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();
    MX_TIM4_Init();
    MX_TIM1_Init();
    /* USER CODE BEGIN 2 */
    beep_pro(3,1);
    lcd_init();
    Flow_Init();
    power_off();
    while(module_stop_ele())
        ;
    Speak_Flage=0;
    //fwdgt_config(0x1fff,FWDGT_PSC_DIV64);
    //fwdgt_enable();
    while (1)
    {
        /* USER CODE END WHILE */
        /* USER CODE BEGIN 3 */
       // fwdgt_counter_reload();
      //  app();
app_test();

//			  if(flag==6)
//			  flag=3;
//				else
//					flag=6;
//					Speak_Flage=flag;
//			delay_ms(5000);
//			EleSwCtrl(WATER_SW,OFF);//废水出水阀开
//			EleSwCtrl(HCILO_SW,OFF);//废水出水阀开
//			EleSwCtrl(WASTE_SW,OFF);//废水出水阀开
//			EleSwCtrl(WASH_SW,OFF);//废水出水阀开
//
//			EleSwCtrl(WATER_SW,ON);//废水出水阀开
//			EleSwCtrl(HCILO_SW,ON);//废水出水阀开
//			EleSwCtrl(WASH_SW,ON);//废水出水阀开
//			EleSwCtrl(WASH_SW,OFF);//废水出水阀开
//			EleSwCtrl(WASTE_SW,ON);//废水出水阀开



        //usart_data_transmit(USART2, 0x5a);
    }
    /* USER CODE END 3 */
}
void rcu_config(void)
{
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOE);

    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA1);
    /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV6);
}
/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* config the GPIO as analog mode, for ADC */
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,GPIO_PIN_6);

    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,GPIO_PIN_4);

    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,GPIO_PIN_4);

    /*configure PB4 (TIMER2 CH0) as alternate function*/
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_0);

    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_0);


    /*Configure PB3 PB10 PB11(TIMER1 CH1 CH2 CH3) as alternate function*/
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_11);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_11);

}

/**
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
  */
void nvic_config(void)
{


    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(ADC_IRQn, 1, 1);
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(TIMER2_IRQn, 1, 1);
    nvic_irq_enable(DMA1_Channel7_IRQn, 0, 0);
    nvic_irq_enable(DMA1_Channel2_IRQn, 0, 1);
}

/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
unsigned char txbuffer[10];
extern   modbus_stru modbus_usr;
void dma_config(void)
{


    dma_single_data_parameter_struct dma_init_struct;
    /* ADC_DMA_channel configuration */
//    dma_single_data_parameter_struct dma_single_data_parameter;
    /* enable DMA0 clock */
    rcu_periph_clock_enable(RCU_DMA0);
    /* enable DMA0 clock */
    rcu_periph_clock_enable(RCU_DMA1);

    /* ADC_DMA_channel configuration */


    /* ADC DMA_channel configuration */
    dma_deinit(DMA0, ADC0_DMA);
    dma_deinit(DMA0, DMA_CH4);
    dma_deinit(DMA0, DMA_CH1);

    dma_deinit(DMA1, DMA_CH5);
    nvic_irq_enable(DMA1_Channel0_IRQn, 0, 0);
    nvic_irq_enable(DMA1_Channel4_IRQn, 0, 0);
    nvic_irq_enable(DMA1_Channel5_IRQn, 0, 0);
    /* ADC_DMA_channel configuration */


//    /* ADC DMA_channel configuration */
//    dma_deinit(DMA1, ADC0_DMA);

//    /* ADC DMA_channel configuration */
//    dma_deinit(DMA1, ADC0_DMA);

//    /* initialize DMA single data mode */
//    dma_single_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
//    dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
//    dma_single_data_parameter.memory0_addr = (uint32_t)(get_adc_buf());
//    dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
//    dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
//    dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;
//    dma_single_data_parameter.number = ADC_SIZE;
//    dma_single_data_parameter.priority = DMA_PRIORITY_HIGH;
//    dma_single_data_mode_init(DMA1, ADC0_DMA, &dma_single_data_parameter);
//    dma_channel_subperipheral_select(DMA1, ADC0_DMA, DMA_SUBPERI0);

//    /* enable DMA circulation mode */
//    dma_circulation_enable(DMA1, ADC0_DMA);

//    /* enable DMA channel */
//    dma_channel_enable(DMA1, ADC0_DMA);

//

//    /* deinitialize DMA1 channel7(USART0 tx) */
//    dma_single_data_para_struct_init(&dma_init_struct);
//    dma_deinit(DMA1, DMA_CH7);
//    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
//    dma_init_struct.memory0_addr = (uint32_t)txbuffer;
//    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
//    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
//    dma_init_struct.number = 1;
//    dma_init_struct.periph_addr = ((uint32_t)&USART_DATA(USART5));
//    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
//    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
//    dma_single_data_mode_init(DMA1, DMA_CH7, &dma_init_struct);

//    /* configure DMA mode */
//    dma_circulation_disable(DMA1, DMA_CH7);
//    dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI5);
//    /* enable DMA1 channel7 transfer complete interrupt */
//    dma_interrupt_enable(DMA1, DMA_CH7, DMA_CHXCTL_FTFIE);
//    /* enable DMA1 channel7 */
//    dma_channel_enable(DMA1, DMA_CH7);



    /* deinitialize DMA1 channel2 (USART0 rx) */
    dma_deinit(DMA1, DMA_CH2);
    dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.memory0_addr = (uint32_t)(modbus_usr.RS485_RX_BUFF);
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.number = LCD_BUF_SIZE;
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART5);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH2, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH2);
    dma_channel_subperipheral_select(DMA1, DMA_CH2, DMA_SUBPERI5);
    /* enable DMA1 channel2 transfer complete interrupt */
    dma_interrupt_enable(DMA1, DMA_CH2, DMA_CHXCTL_FTFIE);
    /* enable DMA1 channel2 */
    dma_channel_enable(DMA1, DMA_CH2);





    dma_deinit(DMA0, DMA_CH1);
    dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.memory0_addr = (uint32_t)(buf_cmd);
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.number = MODBUS_BSIZE;
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART2);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH1, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH1);
    dma_channel_subperipheral_select(DMA0, DMA_CH1, DMA_SUBPERI4);
    /* enable DMA1 channel2 transfer complete interrupt */
    dma_interrupt_enable(DMA0, DMA_CH1, DMA_CHXCTL_FTFIE);
    /* enable DMA1 channel2 */
    dma_channel_enable(DMA0, DMA_CH1);




}
/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
//    /* ADC mode config */
//    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
//    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
//    /* ADC continous function enable */
//    /* ADC data alignment config */
//    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);

//    /* ADC channel length config */


//    adc_channel_length_config(ADC0,ADC_INSERTED_CHANNEL,3);

//    /* ADC temperature sensor channel config */
//    adc_inserted_channel_config(ADC0,2,ADC_CHANNEL_16,ADC_SAMPLETIME_144);
//    /* ADC internal reference voltage channel config */
//    adc_inserted_channel_config(ADC0,0,ADC_CHANNEL_17,ADC_SAMPLETIME_144);
//    /* ADC 1/4 voltate of external battery config */
//    adc_inserted_channel_config(ADC0,1,ADC_CHANNEL_18,ADC_SAMPLETIME_144);

//    /* ADC external trigger disable */
//    adc_external_trigger_config(ADC0,ADC_INSERTED_CHANNEL,EXTERNAL_TRIGGER_DISABLE);
//    /* ADC data alignment config */
//    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
//    /* ADC SCAN function enable */
//    adc_special_function_config(ADC0,ADC_SCAN_MODE,ENABLE);
//    /* ADC Vbat channel enable */
//    adc_channel_16_to_18(ADC_VBAT_CHANNEL_SWITCH,ENABLE);
//    /* ADC temperature and Vref enable */
//    adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH,ENABLE);
//    /* ADC trigger config */
//    adc_external_trigger_source_config(ADC0, ADC_INSERTED_CHANNEL, ADC_EXTTRIG_REGULAR_T0_CH0);
//    /* ADC external trigger enable */
//    adc_external_trigger_config(ADC0, ADC_INSERTED_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
//    /* clear the ADC flag */
//    // adc_interrupt_flag_clear(ADC0, ADC_INT_FLAG_EOC);
//    //adc_interrupt_flag_clear(ADC0, ADC_INT_FLAG_EOIC);
//    /* enable ADC interrupt */
//    // adc_interrupt_enable(ADC0, ADC_INT_EOIC);

//    /* ADC channel length config */
//    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 2);
//    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
//    /* ADC regular channel config */
//    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_6, ADC_SAMPLETIME_15);
//    adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_14, ADC_SAMPLETIME_15);
//    //adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_6, ADC_SAMPLETIME_15);
//    // adc_regular_channel_config(ADC0, 3, ADC_CHANNEL_7, ADC_SAMPLETIME_15);
//    /* ADC trigger config */
//    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_T0_CH0);
//    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
//    /* ADC discontinuous mode */
//    // adc_discontinuous_mode_config(ADC0, ADC_REGULAR_CHANNEL, 1);
//    /* ADC DMA function enable */
//    adc_dma_request_after_last_enable(ADC0);
//    adc_dma_mode_enable(ADC0);

//    /* enable ADC interface */
//    adc_enable(ADC0);
//    /* wait for ADC stability */
//    delay_ms(1);
//    /* ADC calibration and reset calibration */
//    adc_calibration_enable(ADC0);
//    /* enable ADC software trigger */
//    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
//    /* enable ADC software trigger */
//    adc_software_trigger_enable(ADC0, ADC_INSERTED_CHANNEL);

    /* reset ADC */
//    adc_deinit();
//    /* ADC mode config */
//    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
//
//    adc_channel_length_config(ADC0,ADC_INSERTED_CHANNEL,3);

//    /* ADC temperature sensor channel config */
//    adc_inserted_channel_config(ADC0,0,ADC_CHANNEL_16,ADC_SAMPLETIME_144);
//    /* ADC internal reference voltage channel config */
//    adc_inserted_channel_config(ADC0,1,ADC_CHANNEL_17,ADC_SAMPLETIME_144);
//    /* ADC 1/4 voltate of external battery config */
//    adc_inserted_channel_config(ADC0,2,ADC_CHANNEL_18,ADC_SAMPLETIME_144);

//    /* ADC external trigger disable */
//    adc_external_trigger_config(ADC0,ADC_INSERTED_CHANNEL,EXTERNAL_TRIGGER_DISABLE);
//    /* ADC data alignment config */
//    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
//    /* ADC SCAN function enable */
//    adc_special_function_config(ADC0,ADC_SCAN_MODE,ENABLE);
//    /* ADC Vbat channel enable */
//    adc_channel_16_to_18(ADC_VBAT_CHANNEL_SWITCH,ENABLE);
//    /* ADC temperature and Vref enable */
//    adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH,ENABLE);
//    /* ADC trigger config */
//    adc_external_trigger_source_config(ADC0, ADC_INSERTED_CHANNEL, ADC_EXTTRIG_REGULAR_T0_CH0);
//    /* ADC external trigger enable */
//    adc_external_trigger_config(ADC0, ADC_INSERTED_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
//
//
//
//
//
//
//
//
//
//
////
//    /* ADC contineous function disable */
//    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
//    /* ADC scan mode disable */
//    //adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
//    /* ADC data alignment config */
//    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
//    /* ADC channel length config */
//    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1U);
//
//    /* ADC trigger config */
//    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_T0_CH0);
//    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

//    /* enable ADC interface */
//    adc_enable(ADC0);
//    delay_ms(1U);
//    /* ADC calibration and reset calibration */
//    adc_calibration_enable(ADC0);
//		adc_software_trigger_enable(ADC0, ADC_INSERTED_CHANNEL);
    /* reset ADC */
    adc_deinit();
    /* ADC mode config */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
    /* ADC contineous function disable */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
    /* ADC scan mode disable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1U);

    /* ADC trigger config */
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_T0_CH0);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

    /* enable ADC interface */
    adc_enable(ADC0);
    // delay_1ms(1U);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
#if CPU == ST
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 120;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 5;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
#else

#endif
}
#if CPU == ST
/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

    /* USER CODE BEGIN ADC1_Init 0 */
#if CPU == ST
    /* USER CODE END ADC1_Init 0 */

    ADC_ChannelConfTypeDef sConfig = {0};

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */
    /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
    */
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = ENABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 3;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
    */
    sConfig.Channel = ADC_CHANNEL_VREFINT;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
    */
    sConfig.Channel = ADC_CHANNEL_6;
    sConfig.Rank = 2;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
    */
    sConfig.Channel = ADC_CHANNEL_14;
    sConfig.Rank = 3;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */
#else
    dma_single_data_parameter_struct dma_single_data_parameter;
    rcu_periph_clock_enable(RCU_ADC0);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA1);
    /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV6);

    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,GPIO_PIN_6);
    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,GPIO_PIN_4);
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(ADC_IRQn, 1, 1);
    /* ADC DMA_channel configuration */
    dma_deinit(DMA1, DMA_CH0);

    /* initialize DMA single data mode */
    dma_single_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
    dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_single_data_parameter.memory0_addr = (uint32_t)(get_adc_buf());
    dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
    dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;
    dma_single_data_parameter.number = ADC_SIZE;
    dma_single_data_parameter.priority = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH0, &dma_single_data_parameter);
    dma_channel_subperipheral_select(DMA1, DMA_CH0, DMA_SUBPERI0);

    /* enable DMA circulation mode */
    dma_circulation_enable(DMA1, DMA_CH0);

    /* enable DMA channel */
    dma_channel_enable(DMA1, DMA_CH0);

    /* ADC mode config */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
    /* ADC continous function enable */
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);

    /* ADC channel length config */


    adc_channel_length_config(ADC0,ADC_INSERTED_CHANNEL,2);

    /* ADC temperature sensor channel config */
    //adc_inserted_channel_config(ADC0,0,ADC_CHANNEL_16,ADC_SAMPLETIME_144);
    /* ADC internal reference voltage channel config */
    adc_inserted_channel_config(ADC0,0,ADC_CHANNEL_17,ADC_SAMPLETIME_144);
    /* ADC 1/4 voltate of external battery config */
    adc_inserted_channel_config(ADC0,1,ADC_CHANNEL_18,ADC_SAMPLETIME_144);

    /* ADC external trigger disable */
    adc_external_trigger_config(ADC0,ADC_INSERTED_CHANNEL,EXTERNAL_TRIGGER_DISABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
    /* ADC SCAN function enable */
    adc_special_function_config(ADC0,ADC_SCAN_MODE,ENABLE);
    /* ADC Vbat channel enable */
    adc_channel_16_to_18(ADC_VBAT_CHANNEL_SWITCH,ENABLE);
    /* ADC temperature and Vref enable */
    adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH,ENABLE);



    /* ADC trigger config */
    adc_external_trigger_source_config(ADC0, ADC_INSERTED_CHANNEL, ADC_EXTTRIG_REGULAR_T0_CH0);
    /* ADC external trigger enable */
    adc_external_trigger_config(ADC0, ADC_INSERTED_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
    /* clear the ADC flag */
    // adc_interrupt_flag_clear(ADC0, ADC_INT_FLAG_EOC);
    //adc_interrupt_flag_clear(ADC0, ADC_INT_FLAG_EOIC);
    /* enable ADC interrupt */
    // adc_interrupt_enable(ADC0, ADC_INT_EOIC);

    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 2);
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
    /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_6, ADC_SAMPLETIME_15);
    adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_14, ADC_SAMPLETIME_15);
    //adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_6, ADC_SAMPLETIME_15);
    // adc_regular_channel_config(ADC0, 3, ADC_CHANNEL_7, ADC_SAMPLETIME_15);
    /* ADC trigger config */
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_T0_CH0);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, EXTERNAL_TRIGGER_DISABLE);
    /* ADC discontinuous mode */
    // adc_discontinuous_mode_config(ADC0, ADC_REGULAR_CHANNEL, 1);
    /* ADC DMA function enable */
    adc_dma_request_after_last_enable(ADC0);
    adc_dma_mode_enable(ADC0);

    /* enable ADC interface */
    adc_enable(ADC0);
    /* wait for ADC stability */
    delay_ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
    /* enable ADC software trigger */
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
    /* enable ADC software trigger */
    adc_software_trigger_enable(ADC0, ADC_INSERTED_CHANNEL);

    dma_deinit(DMA1, DMA_CH0);







#endif
    /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDIO_SD_Init(void)
{

    /* USER CODE BEGIN SDIO_Init 0 */
#if CPU == ST
    /* USER CODE END SDIO_Init 0 */

    /* USER CODE BEGIN SDIO_Init 1 */

    /* USER CODE END SDIO_Init 1 */
    hsd.Instance = SDIO;
    hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
    hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    hsd.Init.ClockDiv = 0;
    if (HAL_SD_Init(&hsd) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SDIO_Init 2 */
#else

#endif
    /* USER CODE END SDIO_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

    /* USER CODE BEGIN SPI3_Init 0 */
    spi_parameter_struct spi_init_struct;
#if CPU == ST
    /* USER CODE END SPI3_Init 0 */

    /* USER CODE BEGIN SPI3_Init 1 */

    /* USER CODE END SPI3_Init 1 */
    /* SPI3 parameter configuration*/
    hspi3.Instance = SPI3;
    hspi3.Init.Mode = SPI_MODE_MASTER;
    hspi3.Init.Direction = SPI_DIRECTION_2LINES;
    hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi3.Init.NSS = SPI_NSS_SOFT;
    hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi3.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi3) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI3_Init 2 */
#else
    rcu_periph_clock_enable(RCU_SPI3);
    /* SPI3 GPIO config */
    gpio_af_set(GPIOE, GPIO_AF_5, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 );
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);


    /* SPI1 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_32;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI1, &spi_init_struct);

    /* SPI3 parameter config */
    spi_init_struct.device_mode = SPI_MASTER;
    spi_init_struct.nss         = SPI_NSS_HARD;
    spi_init(SPI3, &spi_init_struct);

#endif
    /* USER CODE END SPI3_Init 2 */

}
#endif
/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

    /* USER CODE BEGIN TIM1_Init 0 */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

#if CPU == ST
    /* USER CODE END TIM1_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    /* USER CODE BEGIN TIM1_Init 1 */

    /* USER CODE END TIM1_Init 1 */
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 0;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 65535;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    {
        Error_Handler();
    }
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM1_Init 2 */

    /* USER CODE END TIM1_Init 2 */
    HAL_TIM_MspPostInit(&htim1);
#else

    /* -----------------------------------------------------------------------
     TIMER1 configuration: generate 3 PWM signals with 3 different duty cycles:
     TIMER1CLK = SystemCoreClock / 200 = 1MHz

     TIMER1 channel1 duty cycle = (4000/ 16000)* 100  = 25%
     TIMER1 channel2 duty cycle = (8000/ 16000)* 100  = 50%
     TIMER1 channel3 duty cycle = (12000/ 16000)* 100 = 75%
     ----------------------------------------------------------------------- */


    rcu_periph_clock_enable(RCU_TIMER0);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(TIMER0);

    /* TIMER1 configuration */
    timer_initpara.prescaler         = 199;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 15999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0,&timer_initpara);

    /* CH1,CH2 and CH3 configuration in PWM mode */
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

//    timer_channel_output_config(TIMER0,TIMER_CH_1,&timer_ocintpara);
//    timer_channel_output_config(TIMER0,TIMER_CH_2,&timer_ocintpara);
    timer_channel_output_config(TIMER0,TIMER_CH_3,&timer_ocintpara);

//    /* CH1 configuration in PWM mode1,duty cycle 25% */
//    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,3999);
//    timer_channel_output_mode_config(TIMER0,TIMER_CH_1,TIMER_OC_MODE_PWM0);
//    timer_channel_output_shadow_config(TIMER0,TIMER_CH_1,TIMER_OC_SHADOW_DISABLE);

//    /* CH2 configuration in PWM mode1,duty cycle 50% */
//    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_2,7999);
//    timer_channel_output_mode_config(TIMER1,TIMER_CH_2,TIMER_OC_MODE_PWM0);
//    timer_channel_output_shadow_config(TIMER1,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);

    /* CH3 configuration in PWM mode1,duty cycle 75% */
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_3,0);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_3,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);
    timer_primary_output_config(TIMER0,ENABLE);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER0);
    /* auto-reload preload enable */
    timer_enable(TIMER0);

#endif

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

    /* USER CODE BEGIN TIM3_Init 0 */
#if CPU == ST
    /* USER CODE END TIM3_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};

    /* USER CODE BEGIN TIM3_Init 1 */

    /* USER CODE END TIM3_Init 1 */
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 59;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 0xffff;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV2;
    sConfigIC.ICFilter = 4;
    if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM3_Init 2 */
#else
    /* TIMER2 configuration: input capture mode -------------------
    	the external signal is connected to TIMER2 CH0 pin (PB4)
    	the rising edge is used as active edge
    	the TIMER2 CH0CV is used to compute the frequency value
    	------------------------------------------------------------ */
    timer_ic_parameter_struct timer_icinitpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER2);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    /*configure PB4 (TIMER2 CH0) as alternate function*/

    timer_deinit(TIMER2);

    /* TIMER2 configuration */
    timer_initpara.prescaler         = 199;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 65535;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);

    /* TIMER2  configuration */
    /* TIMER2 CH0 input capture configuration */
    timer_icinitpara.icpolarity  = TIMER_IC_POLARITY_FALLING;
    timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
    timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
    timer_icinitpara.icfilter    = 0x64;
    timer_input_capture_config(TIMER2,TIMER_CH_2,&timer_icinitpara);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);
    /* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(TIMER2,TIMER_INT_CH2);
    /* channel 0 interrupt enable */
    timer_interrupt_enable(TIMER2,TIMER_INT_CH2);

    /* TIMER2 counter enable */
    timer_enable(TIMER2);








#endif
    /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

    /* USER CODE BEGIN TIM4_Init 0 */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
#if CPU == ST
    /* USER CODE END TIM4_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    /* USER CODE BEGIN TIM4_Init 1 */

    /* USER CODE END TIM4_Init 1 */
    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 0;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = 65535;
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM4_Init 2 */

    /* USER CODE END TIM4_Init 2 */
    HAL_TIM_MspPostInit(&htim4);
#else
    rcu_periph_clock_enable(RCU_GPIOB);

    /*Configure PB3 PB10 PB11(TIMER1 CH1 CH2 CH3) as alternate function*/
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_8);

    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6);

    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7);

    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_8);
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_6);
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_7);
    /* -----------------------------------------------------------------------
       TIMER1 configuration: generate 3 PWM signals with 3 different duty cycles:
       TIMER1CLK = SystemCoreClock / 200 = 1MHz

       TIMER1 channel1 duty cycle = (4000/ 16000)* 100	= 25%
       TIMER1 channel2 duty cycle = (8000/ 16000)* 100	= 50%
       TIMER1 channel3 duty cycle = (12000/ 16000)* 100 = 75%
       ----------------------------------------------------------------------- */


    rcu_periph_clock_enable(RCU_TIMER3);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    timer_deinit(TIMER3);

    /* TIMER1 configuration */
    timer_initpara.prescaler 		= 0;
    timer_initpara.alignedmode		= TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection	= TIMER_COUNTER_UP;
    timer_initpara.period			= 65535;
    timer_initpara.clockdivision 	= TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER3,&timer_initpara);

    /* CH1,CH2 and CH3 configuration in PWM mode */
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocnpolarity	= TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocidlestate	= TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER3,TIMER_CH_0,&timer_ocintpara);
    timer_channel_output_config(TIMER3,TIMER_CH_1,&timer_ocintpara);
    timer_channel_output_config(TIMER3,TIMER_CH_2,&timer_ocintpara);

    /* CH1 configuration in PWM mode1,duty cycle 25% */
    timer_channel_output_pulse_value_config(TIMER3,TIMER_CH_0,0);
    timer_channel_output_mode_config(TIMER3,TIMER_CH_0,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER3,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);

    /* CH2 configuration in PWM mode1,duty cycle 50% */
    timer_channel_output_pulse_value_config(TIMER3,TIMER_CH_1,0);
    timer_channel_output_mode_config(TIMER3,TIMER_CH_1,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER3,TIMER_CH_1,TIMER_OC_SHADOW_DISABLE);

    /* CH3 configuration in PWM mode1,duty cycle 75% */
    timer_channel_output_pulse_value_config(TIMER3,TIMER_CH_2,0);
    timer_channel_output_mode_config(TIMER3,TIMER_CH_2,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER3,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER3);
    /* auto-reload preload enable */
    timer_enable(TIMER3);

#endif

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

    /* USER CODE BEGIN UART4_Init 0 */
#if CPU == ST
    /* USER CODE END UART4_Init 0 */

    /* USER CODE BEGIN UART4_Init 1 */

    /* USER CODE END UART4_Init 1 */
    huart4.Instance = UART4;
    huart4.Init.BaudRate = 115200;
    huart4.Init.WordLength = UART_WORDLENGTH_8B;
    huart4.Init.StopBits = UART_STOPBITS_1;
    huart4.Init.Parity = UART_PARITY_NONE;
    huart4.Init.Mode = UART_MODE_TX_RX;
    huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart4.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart4) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN UART4_Init 2 */
#else
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_UART4);

    /* configure the USART4 Tx pin and USART0 Rx pin */
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_0);
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_1);

    /* configure USART4 Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);

    /* configure USART0 Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);

    /* USART configure */
    usart_deinit(UART4);
    usart_baudrate_set(UART4, 115200U);
    usart_receive_config(UART4, USART_RECEIVE_ENABLE);
    usart_transmit_config(UART4, USART_TRANSMIT_ENABLE);
    usart_enable(UART4);

#endif
    /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */
#if CPU == ST
    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */
#else
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
    usart_baudrate_set((USART0), 115200U);
    usart_receive_config((USART0), USART_RECEIVE_ENABLE);
    usart_transmit_config((USART0), USART_TRANSMIT_ENABLE);
    usart_enable((USART0));

#endif
    /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

    /* USER CODE BEGIN USART2_Init 0 */
#if CPU == ST
    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */
#else
    rcu_periph_clock_enable(RCU_USART2);

    /* configure the USART4 Tx pin and USART0 Rx pin */
  //  gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_2);
  //  gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_3);

    /* configure USART4 Tx as alternate function push-pull */
  //  gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);
  //  gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

    /* configure USART0 Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);

    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(USART1, 9600U);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_enable(USART1);
    /* USART2 interrupt Init */
    usart_interrupt_enable(USART1, USART_INT_RBNE);


#endif
    /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

    /* USER CODE BEGIN USART3_Init 0 */
#if CPU == ST
    /* USER CODE END USART3_Init 0 */

    /* USER CODE BEGIN USART3_Init 1 */

    /* USER CODE END USART3_Init 1 */
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART3_Init 2 */
#else
    rcu_periph_clock_enable(RCU_USART2);

    /* configure the USART4 Tx pin and USART0 Rx pin */
    gpio_af_set(GPIOB, GPIO_AF_7, GPIO_PIN_10);
    gpio_af_set(GPIOB, GPIO_AF_7, GPIO_PIN_11);

    /* configure USART4 Tx as alternate function push-pull */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* configure USART0 Rx as alternate function push-pull */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_11);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

    /* USART configure */
    usart_deinit(USART2);
    usart_baudrate_set(USART2, 9600U);
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
    usart_enable(USART2);

    nvic_irq_enable(USART2_IRQn, 0, 0);
    usart_interrupt_enable(USART2, USART_INT_RBNE);
    //   usart_interrupt_enable(USART2, USART_INT_IDLE);
//	usart_dma_receive_config(USART2, USART_DENR_ENABLE);

    //nvic_irq_enable(DMA1_Channel2_IRQn, 0, 1);

#endif
    /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

    /* USER CODE BEGIN USART6_Init 0 */
#if CPU == ST
    /* USER CODE END USART6_Init 0 */

    /* USER CODE BEGIN USART6_Init 1 */

    /* USER CODE END USART6_Init 1 */
    huart6.Instance = USART6;
    huart6.Init.BaudRate = 9600;
    huart6.Init.WordLength = UART_WORDLENGTH_8B;
    huart6.Init.StopBits = UART_STOPBITS_1;
    huart6.Init.Parity = UART_PARITY_NONE;
    huart6.Init.Mode = UART_MODE_TX_RX;
    huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart6.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart6) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART6_Init 2 */
#else
    rcu_periph_clock_enable(RCU_USART5);

    /* configure the USART4 Tx pin and USART0 Rx pin */
    gpio_af_set(GPIOC, GPIO_AF_8, GPIO_PIN_6);
    gpio_af_set(GPIOC, GPIO_AF_8, GPIO_PIN_7);

    /* configure USART4 Tx as alternate function push-pull */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    /* configure USART0 Rx as alternate function push-pull */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);

    /* USART configure */
    usart_deinit(USART5);
    usart_baudrate_set((USART5), 9600U);
    usart_receive_config((USART5), USART_RECEIVE_ENABLE);
    usart_transmit_config((USART5), USART_TRANSMIT_ENABLE);
    usart_enable((USART5));
    nvic_irq_enable(USART5_IRQn, 0, 0);
    usart_interrupt_enable(USART5, USART_INT_IDLE);
    /* USART DMA enable*/
    usart_dma_receive_config(USART5, USART_DENR_ENABLE);
    usart_dma_transmit_config(USART5, USART_DENT_ENABLE);

#endif
    /* USER CODE END USART6_Init 2 */

}
#if CPU == ST
/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{
//    dma_single_data_parameter_struct dma_single_data_parameter;
#if CPU == ST
    /* DMA controller clock enable */
    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Stream1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
    /* DMA2_Stream0_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
    /* DMA2_Stream1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
#else



#endif

}
#endif
/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
#if CPU == ST
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOE, Ele_ConB_Pin|Ele_ConA_Pin|LCD_BL_Pin|LCD_RST_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, CVALUE1_Pin|CVALUE2_Pin|CVALUE3_Pin|CVALUE4_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(TDS_P_GPIO_Port, TDS_P_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, TDS_N_Pin|CVALUE5_Pin|scl_Pin|sda_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, EN1_485_Pin|EN2_485_Pin|WIFI_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_SET);

    /*Configure GPIO pins : Ele_ConB_Pin Ele_ConA_Pin LCD_BL_Pin LCD_RST_Pin */
    GPIO_InitStruct.Pin = Ele_ConB_Pin|Ele_ConA_Pin|LCD_BL_Pin|LCD_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : CVALUE1_Pin CVALUE2_Pin CVALUE3_Pin CVALUE4_Pin */
    GPIO_InitStruct.Pin = CVALUE1_Pin|CVALUE2_Pin|CVALUE3_Pin|CVALUE4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : TEST_VA_Pin */
    GPIO_InitStruct.Pin = TEST_VA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(TEST_VA_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : TDS_P_Pin SPI3_CS_Pin */
    GPIO_InitStruct.Pin = TDS_P_Pin|SPI3_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : SensorA_Pin */
    GPIO_InitStruct.Pin = SensorA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SensorA_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : TDS_N_Pin CVALUE5_Pin scl_Pin sda_Pin */
    GPIO_InitStruct.Pin = TDS_N_Pin|CVALUE5_Pin|scl_Pin|sda_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : LCD_IRQ_Pin SensorB_Pin LCD_READY_Pin */
    GPIO_InitStruct.Pin = LCD_IRQ_Pin|SensorB_Pin|LCD_READY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pin : int_Pin */
    GPIO_InitStruct.Pin = int_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(int_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : EN1_485_Pin EN2_485_Pin WIFI_EN_Pin */
    GPIO_InitStruct.Pin = EN1_485_Pin|EN2_485_Pin|WIFI_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : LCD_BUSY_Pin */
    GPIO_InitStruct.Pin = LCD_BUSY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(LCD_BUSY_GPIO_Port, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
#else
    /* enable GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOH);


#if VER == 0

    /*Configure GPIO pin Output Level */
    gpio_bit_reset(GPIOE, Ele_ConB_Pin|Ele_ConA_Pin|LCD_BL_Pin|LCD_RST_Pin);

    /*Configure GPIO pin Output Level */
    gpio_bit_reset(GPIOC, CVALUE1_Pin|CVALUE2_Pin|CVALUE3_Pin|CVALUE4_Pin);

    /*Configure GPIO pin Output Level */
    gpio_bit_reset(TDS_P_GPIO_Port, TDS_P_Pin);

    /*Configure GPIO pin Output Level */
    gpio_bit_reset(GPIOB, TDS_N_Pin|CVALUE5_Pin|scl_Pin|sda_Pin);

    /*Configure GPIO pin Output Level */
    gpio_bit_reset(GPIOD, EN1_485_Pin|EN2_485_Pin|WIFI_EN_Pin);

    /*Configure GPIO pin Output Level */
    gpio_bit_write(SPI3_CS_GPIO_Port, SPI3_CS_Pin, SET);




    /*Configure GPIO pins : Ele_ConB_Pin Ele_ConA_Pin LCD_BL_Pin LCD_RST_Pin */
    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,Ele_ConB_Pin|Ele_ConA_Pin|
                  LCD_BL_Pin|LCD_RST_Pin);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ,Ele_ConB_Pin|
                            Ele_ConA_Pin|LCD_BL_Pin|LCD_RST_Pin);

    /*Configure GPIO pins : CVALUE1_Pin CVALUE2_Pin CVALUE3_Pin CVALUE4_Pin */
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, CVALUE1_Pin|CVALUE2_Pin|
                  CVALUE3_Pin|CVALUE4_Pin);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, CVALUE1_Pin|
                            CVALUE2_Pin|CVALUE3_Pin|CVALUE4_Pin);

    /*Configure GPIO pin : TEST_VA_Pin */

    gpio_mode_set(TEST_VA_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TEST_VA_Pin);
    gpio_output_options_set(TEST_VA_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TEST_VA_Pin);


    /*Configure GPIO pins : TDS_P_Pin SPI3_CS_Pin */
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TDS_P_Pin|SPI3_CS_Pin);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TDS_P_Pin|SPI3_CS_Pin);


    /*Configure GPIO pin : SensorA_Pin */
    gpio_mode_set(SensorA_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, TDS_P_Pin|SPI3_CS_Pin);
    // gpio_output_options_set(SensorA_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TDS_P_Pin|SPI3_CS_Pin);


    /*Configure GPIO pins : TDS_N_Pin CVALUE5_Pin scl_Pin sda_Pin */
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TDS_N_Pin|scl_Pin|sda_Pin);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TDS_N_Pin|scl_Pin|sda_Pin);
	gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, CVALUE5_Pin);


	gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, CVALUE5_Pin);




    /*Configure GPIO pins : LCD_IRQ_Pin SensorB_Pin LCD_READY_Pin */
    gpio_mode_set(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, LCD_IRQ_Pin|SensorB_Pin|LCD_READY_Pin);
    gpio_mode_set(TEST_VA_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, TEST_VA_Pin);
    gpio_mode_set(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, SensorB_Pin);
    gpio_mode_set(SensorA_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, SensorA_Pin);




    /*Configure GPIO pins : EN1_485_Pin EN2_485_Pin WIFI_EN_Pin */

    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, EN1_485_Pin|EN2_485_Pin|WIFI_EN_Pin);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, EN1_485_Pin|EN2_485_Pin|WIFI_EN_Pin);
    /*Configure GPIO pin : LCD_BUSY_Pin */
    gpio_mode_set(LCD_BUSY_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, LCD_BUSY_Pin);

    /* EXTI interrupt init*/

    /* enable the key clock */
    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4|GPIO_PIN_5);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_4|GPIO_PIN_5);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /* configure button pin as input */
    gpio_mode_set(int_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,int_Pin);
    /* enable and set key EXTI interrupt to the lowest priority */
    nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);

    /* connect key EXTI line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN13);

    /* configure key EXTI line */
    exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_13);

#else






	/*Configure GPIO pin Output Level */
	   gpio_bit_reset(GPIOE, Ele_ConB_Pin|Ele_ConA_Pin|LCD_BL_Pin|LCD_RST_Pin);

	   gpio_bit_reset(GPIOE, CVALUE1_Pin|CVALUE4_Pin|CVALUE5_Pin);
	   gpio_bit_reset(GPIOA, CVALUE3_Pin|CVALUE2_Pin);


	   /*Configure GPIO pin Output Level */
	   gpio_bit_reset(TDS_P_GPIO_Port, TDS_P_Pin);
	
	   /*Configure GPIO pin Output Level */
	   gpio_bit_reset(GPIOB, TDS_N_Pin|CVALUE5_Pin|scl_Pin|sda_Pin);
	
	   /*Configure GPIO pin Output Level */
	   gpio_bit_reset(GPIOD, EN1_485_Pin|EN2_485_Pin|WIFI_EN_Pin);
	
	   /*Configure GPIO pin Output Level */
	   gpio_bit_write(SPI3_CS_GPIO_Port, SPI3_CS_Pin, SET);

	   /*Configure GPIO pins : Ele_ConB_Pin Ele_ConA_Pin LCD_BL_Pin LCD_RST_Pin */
	   gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,Ele_ConB_Pin|Ele_ConA_Pin|
					 LCD_BL_Pin|LCD_RST_Pin);
	   gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ,Ele_ConB_Pin|
							   Ele_ConA_Pin|LCD_BL_Pin|LCD_RST_Pin);


	   /*Configure GPIO pins : CVALUE1_Pin CVALUE2_Pin CVALUE3_Pin CVALUE4_Pin */
	   gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, CVALUE1_Pin
					 |CVALUE4_Pin|CVALUE5_Pin);
	   
	   gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ,CVALUE1_Pin
					 |CVALUE4_Pin|CVALUE5_Pin);
	   /*Configure GPIO pins : CVALUE1_Pin CVALUE2_Pin CVALUE3_Pin CVALUE4_Pin */
	   gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, CVALUE2_Pin| CVALUE3_Pin);

	   gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ,CVALUE2_Pin|
							   CVALUE3_Pin);



	   /*Configure GPIO pin : TEST_VA_Pin */
	
	   gpio_mode_set(TEST_VA_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TEST_VA_Pin);
	   gpio_output_options_set(TEST_VA_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TEST_VA_Pin);
	
	
	   /*Configure GPIO pins : TDS_P_Pin SPI3_CS_Pin */
	   gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TDS_P_Pin|SPI3_CS_Pin);
	   gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TDS_P_Pin|SPI3_CS_Pin);
	
	
	   /*Configure GPIO pin : SensorA_Pin */
	   gpio_mode_set(SensorA_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, TDS_P_Pin|SPI3_CS_Pin);
	   // gpio_output_options_set(SensorA_GPIO_Port, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TDS_P_Pin|SPI3_CS_Pin);
	
	
	   /*Configure GPIO pins : TDS_N_Pin CVALUE5_Pin scl_Pin sda_Pin */
	   gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TDS_N_Pin|CVALUE5_Pin|scl_Pin|sda_Pin);
	   gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, TDS_N_Pin|CVALUE5_Pin|scl_Pin|sda_Pin);
	
	
	
	
	   /*Configure GPIO pins : LCD_IRQ_Pin SensorB_Pin LCD_READY_Pin */
	   gpio_mode_set(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, LCD_IRQ_Pin|SensorB_Pin|LCD_READY_Pin);
	   gpio_mode_set(TEST_VA_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, TEST_VA_Pin);
	   gpio_mode_set(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, SensorB_Pin);
	   gpio_mode_set(SensorA_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, SensorA_Pin);
	
	
	
	
	   /*Configure GPIO pins : EN1_485_Pin EN2_485_Pin WIFI_EN_Pin */
	
	   gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, EN1_485_Pin|EN2_485_Pin|WIFI_EN_Pin);
	   gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, EN1_485_Pin|EN2_485_Pin|WIFI_EN_Pin);
	   /*Configure GPIO pin : LCD_BUSY_Pin */
	   gpio_mode_set(LCD_BUSY_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, LCD_BUSY_Pin);
	
	   /* EXTI interrupt init*/
	
	   /* enable the key clock */
	   gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4|GPIO_PIN_5);
	   gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_4|GPIO_PIN_5);
	   rcu_periph_clock_enable(RCU_SYSCFG);
	
	   /* configure button pin as input */
	   gpio_mode_set(int_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,int_Pin);
	   /* enable and set key EXTI interrupt to the lowest priority */
	   nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);
	
	   /* connect key EXTI line to key GPIO pin */
	   syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN13);
	
	   /* configure key EXTI line */
	   exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	   exti_interrupt_flag_clear(EXTI_13);

#endif


#endif

}
#if CPU == ST
/* FSMC initialization function */
static void MX_FSMC_Init(void)
{

    /* USER CODE BEGIN FSMC_Init 0 */
#if CPU == ST
    /* USER CODE END FSMC_Init 0 */

    FSMC_NORSRAM_TimingTypeDef Timing = {0};

    /* USER CODE BEGIN FSMC_Init 1 */

    /* USER CODE END FSMC_Init 1 */

    /** Perform the SRAM1 memory initialization sequence
    */
    hsram1.Instance = FSMC_NORSRAM_DEVICE;
    hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    /* hsram1.Init */
    hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
    hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
    hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_8;
    hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
    hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
    hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
    hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
    hsram1.Init.PageSize = FSMC_PAGE_SIZE_NONE;
    /* Timing */
    Timing.AddressSetupTime = 15;
    Timing.AddressHoldTime = 15;
    Timing.DataSetupTime = 255;
    Timing.BusTurnAroundDuration = 15;
    Timing.CLKDivision = 16;
    Timing.DataLatency = 17;
    Timing.AccessMode = FSMC_ACCESS_MODE_A;
    /* ExtTiming */

    if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
    {
        Error_Handler( );
    }

    /* USER CODE BEGIN FSMC_Init 2 */
#else

#endif
    /* USER CODE END FSMC_Init 2 */
}
#endif
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
