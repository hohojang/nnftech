/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LoRa.h" // LoRa 모듈 제어를 위한 헤더파일
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_PIN GPIO_PIN_8      // LED 핀 (PA8)
#define LED_GPIO_PORT GPIOA     // LED가 연결된 포트 (GPIOA)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t dataToSend[] = "Hello LoRa!";  // 송신할 데이터
uint8_t receivedData[16] = {0};        // 수신 데이터를 저장할 버퍼
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LED_On(void);       // LED를 켜는 함수
void LED_Off(void);      // LED를 끄는 함수
void LED_Toggle(void);   // LED 상태를 토글하는 함수
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  // 초기 변수 또는 설정 초기화 시 필요한 작업을 여기서 수행
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init(); // HAL 라이브러리 초기화

  /* USER CODE BEGIN Init */
  // 추가 초기화 코드 삽입
  /* USER CODE END Init */

  SystemClock_Config(); // 시스템 클럭 설정

  MX_GPIO_Init(); // GPIO 초기화
  MX_SPI1_Init(); // SPI1 초기화

  /* USER CODE BEGIN 2 */
  LoRa_Init(); // LoRa 모듈 초기화
  LED_Off();   // LED 초기 상태: OFF
  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
    /* USER CODE BEGIN WHILE */
    // 데이터 송신
    LoRa_SendData(dataToSend, sizeof(dataToSend)); // LoRa 모듈로 데이터 전송
    LED_On();  // 데이터 송신 중 LED ON
    HAL_Delay(500); // 500ms 대기
    LED_Off(); // 데이터 송신 후 LED OFF
    HAL_Delay(500); // 500ms 대기

    // DIO 핀 상태 확인
    LoRa_CheckDIO(); // LoRa 모듈의 DIO 핀 이벤트 확인

    // 데이터 수신
    LoRa_ReceiveData(receivedData, sizeof(receivedData)); // 수신된 데이터를 읽음
    LED_Toggle(); // 데이터 수신 완료 시 LED 상태를 토글
    HAL_Delay(1000); // 1초 대기
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // 메인 루프 종료 후 실행될 수 있는 추가 작업
    /* USER CODE END 3 */
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void LED_On(void)
{
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET); // LED ON (Active Low)
}

void LED_Off(void)
{
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET); // LED OFF (Active Low)
}

void LED_Toggle(void)
{
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN); // LED 상태 토글
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  // 디버깅을 위한 기본 Error Handler
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
  // assert 실패 시 출력될 디버깅 코드
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
