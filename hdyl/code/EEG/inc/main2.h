/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32f4xx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
typedef unsigned int u16;
typedef unsigned char u8;
typedef uint32_t u32;
#define ADS_CHANNEL 16

#include "string.h"
/* USER CODE END Includes */
#define BLE_DEBUG  0

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define VER 1
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */
#if CPU == ST
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
#endif
typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
}GPIO_PinState;


/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

#define MMR_RD_Pin GPIO_PIN_2
#define MMR_RD_GPIO_Port GPIOE
#define BDATA_Pin GPIO_PIN_5
#define BDATA_GPIO_Port GPIOE
#define BLINK_Pin GPIO_PIN_6
#define BLINK_GPIO_Port GPIOE
#define BDISC_Pin GPIO_PIN_13
#define BDISC_GPIO_Port GPIOC
#define BWKP_Pin GPIO_PIN_14
#define BWKP_GPIO_Port GPIOC
#define BMOD_Pin GPIO_PIN_15
#define BMOD_GPIO_Port GPIOC
#define VBAT_STA_Pin GPIO_PIN_2
#define VBAT_STA_GPIO_Port GPIOC
#define BRST_Pin GPIO_PIN_0
#define BRST_GPIO_Port GPIOB
#define BLE_EN_Pin GPIO_PIN_2
#define BLE_EN_GPIO_Port GPIOB

#define ADS129x_DRDY_Pin GPIO_PIN_1
#define ADS129x_DRDY_GPIO_Port GPIOB
#define POW_HEA_Pin GPIO_PIN_8
#define POW_HEA_GPIO_Port GPIOE
#define PRESS_EN_Pin GPIO_PIN_9
#define PRESS_EN_GPIO_Port GPIOE
#define POW_EN_5V_Pin GPIO_PIN_10
#define POW_EN_5V_GPIO_Port GPIOE

#define ADS129x_CS2_Pin GPIO_PIN_14
#define ADS129x_CS2_GPIO_Port GPIOE
#define ADS129x_DRDY2_Pin GPIO_PIN_15
#define ADS129x_DRDY2_GPIO_Port GPIOE
#define ADS129x_CS_Pin GPIO_PIN_12
#define ADS129x_CS_GPIO_Port GPIOB


#define LED1_Pin GPIO_PIN_8
#define LED1_GPIO_Port GPIOD
#define LED2_Pin GPIO_PIN_9
#define LED2_GPIO_Port GPIOD
#define OUT1_Pin GPIO_PIN_10
#define OUT1_GPIO_Port GPIOD
#define OUT2_Pin GPIO_PIN_11
#define OUT2_GPIO_Port GPIOD
#define OUT3_Pin GPIO_PIN_12
#define OUT3_GPIO_Port GPIOD
#define OUT4_Pin GPIO_PIN_13
#define OUT4_GPIO_Port GPIOD
#define SPEAK_Pin GPIO_PIN_11
#define SPEAK_GPIO_Port GPIOA
#define MMR_CS_Pin GPIO_PIN_12
#define MMR_CS_GPIO_Port GPIOA
#define SPI2_CS_Pin GPIO_PIN_15
#define SPI2_CS_GPIO_Port GPIOA
#define KEY1_Pin GPIO_PIN_7
#define KEY1_GPIO_Port GPIOA
#define KEY2_Pin GPIO_PIN_6
#define KEY2_GPIO_Port GPIOA
#define MOTOR_Pin GPIO_PIN_8
#define MOTOR_GPIO_Port GPIOB
#define VALUE_Pin GPIO_PIN_9
#define VALUE_GPIO_Port GPIOB
#define POW_EN_3V3_Pin GPIO_PIN_0
#define POW_EN_3V3_GPIO_Port GPIOE




/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
