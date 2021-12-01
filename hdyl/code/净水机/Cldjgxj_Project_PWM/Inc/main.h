/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TEST_VA_Pin GPIO_PIN_13
#define TEST_VA_GPIO_Port GPIOC
#define ADC_PW_Pin GPIO_PIN_0
#define ADC_PW_GPIO_Port GPIOA
#define TDS_P_Pin GPIO_PIN_2
#define TDS_P_GPIO_Port GPIOA
#define ADC_ELE_Pin GPIO_PIN_3
#define ADC_ELE_GPIO_Port GPIOA
#define ELE_CONA_Pin GPIO_PIN_4
#define ELE_CONA_GPIO_Port GPIOA
#define ELE_CONB_Pin GPIO_PIN_5
#define ELE_CONB_GPIO_Port GPIOA
#define TDS_N_Pin GPIO_PIN_6
#define TDS_N_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_0
#define LED_GPIO_Port GPIOB
#define JS_VALUE_Pin GPIO_PIN_2
#define JS_VALUE_GPIO_Port GPIOB
#define SPI2_NSS_Pin GPIO_PIN_12
#define SPI2_NSS_GPIO_Port GPIOB
#define KEY_ELE_Pin GPIO_PIN_14
#define KEY_ELE_GPIO_Port GPIOB
#define RF_A_Pin GPIO_PIN_11
#define RF_A_GPIO_Port GPIOA
#define RF_B_Pin GPIO_PIN_12
#define RF_B_GPIO_Port GPIOA
#define KEY_RES_Pin GPIO_PIN_15
#define KEY_RES_GPIO_Port GPIOA
#define TM_SCK_Pin GPIO_PIN_3
#define TM_SCK_GPIO_Port GPIOB
#define TM_DIN_Pin GPIO_PIN_4
#define TM_DIN_GPIO_Port GPIOB
#define ST_VA_Pin GPIO_PIN_7
#define ST_VA_GPIO_Port GPIOB
#define SS_VA_Pin GPIO_PIN_8
#define SS_VA_GPIO_Port GPIOB
#define LLJ_SENSER_Pin GPIO_PIN_9
#define LLJ_SENSER_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
