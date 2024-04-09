/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
	
/* USER CODE END Includes */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
	
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
	void Error_Handler(void);
/* USER CODE BEGIN EFP */
	void update_LED(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
	#define ALL_OFF 0x40810000u
	#define GREEN   0x00000001u
	#define BLUE    0x00000080u
	#define RED     0x00004000u

	#define TRUE 1
	#define FALSE 0
/* USER CODE END Private defines */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
	extern int COUNTER;
	extern int EVERY_OTHER;
	extern int STOP;
/* USER CODE END ET */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
