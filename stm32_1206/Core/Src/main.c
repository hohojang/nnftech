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
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define RX_BUFFER_SIZE 1 // UART 수신 버퍼 크기
#define MAX_INPUT_LENGTH 100 // 명령어 최대 길이

/* USER CODE END PD */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
volatile uint32_t reset_count = 0; // 리셋 카운트 변수
uint8_t rx_buffer[RX_BUFFER_SIZE]; // UART 수신 버퍼
char input_buffer[MAX_INPUT_LENGTH]; // CLI 입력 버퍼
int buffer_index = 0; // 입력 버퍼 인덱스

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void UART_SendString(UART_HandleTypeDef *huart, const char *str);
void ProcessCLICommand(char *command);
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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  UART_SendString(&huart2, "=== STM32 CLI Example ===\r\n");

  HAL_UART_Receive_IT(&huart2, rx_buffer, RX_BUFFER_SIZE); // UART 비동기 수신 시작

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 64;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

// UART 인터럽트 콜백 함수
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        // 수신된 문자가 \r 또는 \n인 경우 명령어 처리
        if (rx_buffer[0] == '\r' || rx_buffer[0] == '\n') // 엔터키가 들어오면 명령어 종료
        {
            input_buffer[buffer_index] = '\0'; // 명령어 종료
            ProcessCLICommand(input_buffer); // 명령어 처리
            buffer_index = 0; // 입력 버퍼 초기화
        }
        // 백스페이스 처리
        else if (rx_buffer[0] == '\b')
        {
            if (buffer_index > 0)
            {
                buffer_index--; // 버퍼에서 문자 삭제
                UART_SendString(&huart2, "\b \b"); // 백스페이스 문자 출력 처리
            }
        }
        // 정상 문자 처리
        else if (buffer_index < MAX_INPUT_LENGTH - 1)
        {
            // 수신된 문자를 버퍼에 저장
            input_buffer[buffer_index++] = rx_buffer[0];
        }

        // UART 수신 대기
        HAL_UART_Receive_IT(&huart2, rx_buffer, RX_BUFFER_SIZE);
    }
}

// CLI 명령어 처리 함수
void ProcessCLICommand(char *command)
{
    if (strcmp(command, "help") == 0)
    {
        UART_SendString(&huart2, "Available commands:\r\n");
        UART_SendString(&huart2, "help    - Show this help message\r\n");
        UART_SendString(&huart2, "hello   - Print 'world'\r\n");
        UART_SendString(&huart2, "reset   - Reset the button press\r\n");
    }
    else if (strcmp(command, "hello") == 0)
    {
        UART_SendString(&huart2, "world\r\n");
    }
    else if (strcmp(command, "reset") == 0)
    {
        reset_count = 0;
        UART_SendString(&huart2, "Button press reset.\r\n");
    }
    // "Unknown command" 메시지 출력은 제거됨
}

// UART로 문자열 전송 함수
void UART_SendString(UART_HandleTypeDef *huart, const char *str)
{
    while (*str)
    {
        HAL_UART_Transmit(huart, (uint8_t *)str++, 1, HAL_MAX_DELAY);
    }
}

/* USER CODE END 4 */

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1)
  {
    /* USER CODE END Error_Handler_Debug */
  }
}
