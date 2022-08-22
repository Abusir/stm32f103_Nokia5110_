/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
HAL_StatusTypeDef user_delay_us(uint32_t us);
HAL_StatusTypeDef user_delay_ms(uint32_t ms);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define board_led_Pin GPIO_PIN_13
#define board_led_GPIO_Port GPIOC
#define HW_504_GND_Pin GPIO_PIN_0
#define HW_504_GND_GPIO_Port GPIOA
#define HW_503_VCC_Pin GPIO_PIN_1
#define HW_503_VCC_GPIO_Port GPIOA
#define VRx_Pin GPIO_PIN_6
#define VRx_GPIO_Port GPIOA
#define VRy_Pin GPIO_PIN_7
#define VRy_GPIO_Port GPIOA
#define sw_Pin GPIO_PIN_0
#define sw_GPIO_Port GPIOB
#define sw_EXTI_IRQn EXTI0_IRQn
#define Nokia_RST_Pin GPIO_PIN_12
#define Nokia_RST_GPIO_Port GPIOB
#define Nokia_CE_Pin GPIO_PIN_13
#define Nokia_CE_GPIO_Port GPIOB
#define Nokia_DC_Pin GPIO_PIN_14
#define Nokia_DC_GPIO_Port GPIOB
#define Nokia_Din_Pin GPIO_PIN_15
#define Nokia_Din_GPIO_Port GPIOB
#define Nokia_CLK_Pin GPIO_PIN_8
#define Nokia_CLK_GPIO_Port GPIOA
#define Nokia_VCC_Pin GPIO_PIN_9
#define Nokia_VCC_GPIO_Port GPIOA
#define Nokia_BL_Pin GPIO_PIN_10
#define Nokia_BL_GPIO_Port GPIOA
#define Nokia_GND_Pin GPIO_PIN_11
#define Nokia_GND_GPIO_Port GPIOA
#define CH340_TX_Pin GPIO_PIN_6
#define CH340_TX_GPIO_Port GPIOB
#define CH340_RX_Pin GPIO_PIN_7
#define CH340_RX_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
