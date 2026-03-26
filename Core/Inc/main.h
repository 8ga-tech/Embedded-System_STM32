/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MotorA_Pin GPIO_PIN_0
#define MotorA_GPIO_Port GPIOA
#define Motora_Pin GPIO_PIN_1
#define Motora_GPIO_Port GPIOA
#define MotorB_Pin GPIO_PIN_2
#define MotorB_GPIO_Port GPIOA
#define Motorb_Pin GPIO_PIN_3
#define Motorb_GPIO_Port GPIOA
#define duoji_Pin GPIO_PIN_0
#define duoji_GPIO_Port GPIOB
#define chaosheng_echo_Pin GPIO_PIN_1
#define chaosheng_echo_GPIO_Port GPIOB
#define chaosheng_trig_Pin GPIO_PIN_2
#define chaosheng_trig_GPIO_Port GPIOB
#define yuyin_Pin GPIO_PIN_10
#define yuyin_GPIO_Port GPIOB
#define yuyin_in_Pin GPIO_PIN_11
#define yuyin_in_GPIO_Port GPIOB
#define shoushi1_Pin GPIO_PIN_5
#define shoushi1_GPIO_Port GPIOB
#define OLED_and_shoushi_Pin GPIO_PIN_6
#define OLED_and_shoushi_GPIO_Port GPIOB
#define OLED_and_shoushiB7_Pin GPIO_PIN_7
#define OLED_and_shoushiB7_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */


typedef enum {
    MODE_AUTO_PWM,      // 自动渐变模式
    MODE_PID_CLOSED     // PID闭环模式
} ControlMode;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
