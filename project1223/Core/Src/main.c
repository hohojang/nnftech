/* USER CODE BEGIN Header */
/**
  *******************************************************************************
  * @file           : main.c
  * @brief          : Main program body for CDS and PIR sensor integration
  *******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  *******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;     // ADC 핸들러
TIM_HandleTypeDef htim2;     // 타이머 핸들러
UART_HandleTypeDef huart1;   // UART 핸들러

/* USER CODE BEGIN PV */
uint32_t cds_value = 0; // CDS 센서 값 (ADC 읽기 결과 저장)
uint8_t pir_state = 0;  // PIR 센서 상태 (0: No Motion, 1: Motion Detected)
/* USER CODE END PV */

/* Function prototypes -------------------------------------------------------*/
void SystemClock_Config(void);     // 시스템 클럭 설정 함수
static void MX_GPIO_Init(void);    // GPIO 초기화 함수
static void MX_ADC1_Init(void);    // ADC 초기화 함수
static void MX_TIM2_Init(void);    // 타이머 초기화 함수
static void MX_USART1_UART_Init(void); // UART 초기화 함수
uint32_t Read_ADC(void);           // ADC 값 읽기 함수

/* Redirect printf to UART */
int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}

/* USER CODE BEGIN 0 */

/**
 * @brief ADC 값 읽기
 * @retval uint32_t : ADC 변환 결과
 * @note ADC를 시작하고 변환 완료 후 값을 반환합니다.
 */
uint32_t Read_ADC(void) {
    HAL_ADC_Start(&hadc1); // ADC 시작
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // 변환 완료 대기
    return HAL_ADC_GetValue(&hadc1); // ADC 값 반환
}

/**
 * @brief TIM2 인터럽트 콜백 함수
 * @param htim : 타이머 핸들러 포인터
 * @note 주기적으로 CDS 및 PIR 센서 데이터를 읽어 UART로 출력합니다.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        cds_value = Read_ADC();  // CDS 센서 값 읽기
        pir_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6); // PIR 센서 상태 읽기

        printf("PIR State: %s, CDS Value: %lu\n",
               pir_state ? "DETECTED" : "NOT DETECTED", cds_value);
    }
}

/* USER CODE END 0 */

/**
  * @brief  메인 프로그램 진입점
  * @retval int
  */
int main(void) {
    /* MCU Configuration--------------------------------------------------------*/

    HAL_Init(); // HAL 라이브러리 초기화
    SystemClock_Config(); // 시스템 클럭 설정

    /* 주변 장치 초기화 */
    MX_GPIO_Init();       // GPIO 초기화
    MX_ADC1_Init();       // ADC 초기화
    MX_TIM2_Init();       // TIM2 초기화
    MX_USART1_UART_Init(); // UART 초기화

    printf("System Initialized\n"); // UART로 초기화 메시지 전송

    /* TIM2 인터럽트 시작 */
    HAL_TIM_Base_Start_IT(&htim2); // 타이머 인터럽트 활성화

    /* 무한 루프 */
    while (1) {
        HAL_Delay(100); // 100ms 대기
    }
}

/**
  * @brief 시스템 클럭 설정
  * @retval None
  * @note MCU의 시스템 클럭을 구성합니다.
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

/**
  * @brief ADC1 초기화 함수
  * @retval None
  * @note CDS 센서 값을 읽기 위한 ADC를 설정합니다.
  */
static void MX_ADC1_Init(void) {
    ADC_ChannelConfTypeDef sConfig = {0};

    hadc1.Instance = ADC1;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    if (HAL_ADC_Init(&hadc1) != HAL_OK) {
        Error_Handler();
    }

    sConfig.Channel = ADC_CHANNEL_15;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
}

/**
  * @brief TIM2 초기화
  * @retval None
  * @note 주기적으로 센서 데이터를 읽기 위한 타이머를 설정합니다.
  */
static void MX_TIM2_Init(void) {
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 7999;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 50000;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/**
  * @brief UART 초기화
  * @retval None
  */
static void MX_USART1_UART_Init(void) {
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    HAL_UART_Init(&huart1);
}

/**
  * @brief GPIO 초기화
  * @retval None
  */
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
  * @brief 오류 핸들러
  */
void Error_Handler(void) {
    printf("Error Occurred!\n");
    while (1);
}
