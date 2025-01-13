//E22-900T22S 모듈 초기화 and 기본값으로 복원 code 
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RESPONSE_BUFFER_SIZE 64 // 응답 버퍼 크기
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t responseBuffer[RESPONSE_BUFFER_SIZE]; // 응답 데이터를 저장하는 버퍼
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Debug_Print(char *message);
void E22_RestoreDefaults(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// 디버깅 메시지 출력 함수
void Debug_Print(char *message) {
    HAL_UART_Transmit(&huart3, (uint8_t *)message, strlen(message), HAL_MAX_DELAY);
}

// E22 기본값 복원 함수
void E22_RestoreDefaults() {
    // UART Baud Rate 기본값: 9600 bps
    uint8_t baudCommand[] = {0xC1, 0x02, 0x05, 0x03};
    HAL_UART_Transmit(&huart3, baudCommand, sizeof(baudCommand), HAL_MAX_DELAY);
    HAL_Delay(50); // 명령 안정화 대기

    // 채널 기본값: 0x17
    uint8_t channelCommand[] = {0xC1, 0x05, 0x06, 0x17};
    HAL_UART_Transmit(&huart3, channelCommand, sizeof(channelCommand), HAL_MAX_DELAY);
    HAL_Delay(50);

    // 주소 기본값: 0x0000
    uint8_t addressCommand[] = {0xC1, 0x00, 0x06, 0x00, 0x00};
    HAL_UART_Transmit(&huart3, addressCommand, sizeof(addressCommand), HAL_MAX_DELAY);
    HAL_Delay(50);

    // 패리티 및 모드 기본값: 8N1
    uint8_t parityCommand[] = {0xC1, 0x03, 0x08, 0x00};
    HAL_UART_Transmit(&huart3, parityCommand, sizeof(parityCommand), HAL_MAX_DELAY);
    HAL_Delay(50);

    Debug_Print("E22 module restored to defaults.\r\n");
}

// E22 초기화 함수
void E22_Init() {
    // M0 = 0, M1 = 0 (전송/수신 모드)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // M0 = 0
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // M1 = 0
    HAL_Delay(10); // 안정화 대기
}

// AUX 핀 상태 확인 함수
uint8_t E22_CheckAUX() {
    return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2); // AUX 핀 상태 반환
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART3_UART_Init();

    // 설정 모드로 전환 (M0 = 1, M1 = 1)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // M0 = 1
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // M1 = 1
    HAL_Delay(100); // 설정 모드 안정화 대기

    // E22 기본값 복원
    E22_RestoreDefaults();

    // 설정 모드 종료 (M0 = 0, M1 = 0)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // M0 = 0
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // M1 = 0
    Debug_Print("Configuration complete. Switching to normal mode.\r\n");

    // 정상 모드 진입
    E22_Init();

    while (1) {
        HAL_Delay(1000);
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
        Error_Handler();
    }

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    __disable_irq();
    while (1) {
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) { }
#endif /* USE_FULL_ASSERT */
