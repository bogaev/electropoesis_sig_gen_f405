/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define ADC1_IN0_IND1_POS_Pin GPIO_PIN_0
#define ADC1_IN0_IND1_POS_GPIO_Port GPIOA
#define ADC1_IN1_IND1_NEG_Pin GPIO_PIN_1
#define ADC1_IN1_IND1_NEG_GPIO_Port GPIOA
#define ADC1_IN2_IND2_POS_Pin GPIO_PIN_2
#define ADC1_IN2_IND2_POS_GPIO_Port GPIOA
#define ADC1_IN3_IND2_NEG_Pin GPIO_PIN_3
#define ADC1_IN3_IND2_NEG_GPIO_Port GPIOA
#define ADC1_IN6_IND4_POS_Pin GPIO_PIN_6
#define ADC1_IN6_IND4_POS_GPIO_Port GPIOA
#define ADC1_IN7_IND4_NEG_Pin GPIO_PIN_7
#define ADC1_IN7_IND4_NEG_GPIO_Port GPIOA
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define TEST_MODE_Pin GPIO_PIN_12
#define TEST_MODE_GPIO_Port GPIOB
#define LED2_GREEN_Pin GPIO_PIN_13
#define LED2_GREEN_GPIO_Port GPIOB
#define RELAY_220V_Pin GPIO_PIN_6
#define RELAY_220V_GPIO_Port GPIOC
#define IND7_POS_DMA_Pin GPIO_PIN_8
#define IND7_POS_DMA_GPIO_Port GPIOC
#define IND7_NEG_DMA_Pin GPIO_PIN_9
#define IND7_NEG_DMA_GPIO_Port GPIOC
#define IND1_POS_DMA_Pin GPIO_PIN_8
#define IND1_POS_DMA_GPIO_Port GPIOA
#define IND1_NEG_DMA_Pin GPIO_PIN_9
#define IND1_NEG_DMA_GPIO_Port GPIOA
#define LED1_GREEN_Pin GPIO_PIN_10
#define LED1_GREEN_GPIO_Port GPIOA
#define IND2_POS_DMA_Pin GPIO_PIN_15
#define IND2_POS_DMA_GPIO_Port GPIOA
#define LED3_RED_Pin GPIO_PIN_10
#define LED3_RED_GPIO_Port GPIOC
#define LED4_RED_Pin GPIO_PIN_11
#define LED4_RED_GPIO_Port GPIOC
#define IND2_NEG_DMA_Pin GPIO_PIN_3
#define IND2_NEG_DMA_GPIO_Port GPIOB
#define IND4_POS_DMA_Pin GPIO_PIN_6
#define IND4_POS_DMA_GPIO_Port GPIOB
#define IND4_NEG_DMA_Pin GPIO_PIN_7
#define IND4_NEG_DMA_GPIO_Port GPIOB
#define RESERVE_1_Pin GPIO_PIN_8
#define RESERVE_1_GPIO_Port GPIOB
#define RESERVE_2_Pin GPIO_PIN_9
#define RESERVE_2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define LED_ON(PORT, PIN) HAL_GPIO_WritePin(PORT, PIN, GPIO_PIN_SET)
#define LED_OFF(PORT, PIN) HAL_GPIO_WritePin(PORT, PIN, GPIO_PIN_RESET)

#define THIS_IS_MAIN_MCU
//#define SELF_MESSAGE_ON

#ifndef THIS_IS_MAIN_MCU
#define RELAY_220V_Pin GPIO_PIN_2
#define RELAY_220V_GPIO_Port GPIOC
#endif
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
