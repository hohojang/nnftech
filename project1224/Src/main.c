/* USER CODE BEGIN Header */
/**
  *******************************************************************************
  * @file           : main.c
  * @brief          : PIR 및 CDS 센서 데이터를 USART3로 출력
  *******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>
#include <stdio.h>

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
char uart_buffer[100];
uint32_t cds_value = 0;          // CDS 센서 AO 값 (아날로그)
GPIO_PinState pir_state;         // PIR 센서 상태 (디지털)
GPIO_PinState cds_digital_state; // CDS 센서 DO 상태 (디지털)
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_ADC1_Init(void);
void Read_Sensors(void);
void UART_SendString(char *str);

/* USER CODE BEGIN 0 */
/**
 * @brief 센서 데이터 읽기 및 UART 출력 (2열 포맷)
 */
void Read_Sensors(void) {
    // PIR 센서 상태 읽기 (PC6)
    pir_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);

    // CDS 센서 디지털 상태 읽기 (PC7)
    cds_digital_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7);

    // CDS 센서 아날로그 값 읽기 (PB0 - ADC1_IN15)
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
        cds_value = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);

    // UART로 데이터 전송 (2열 포맷)
    snprintf(uart_buffer, sizeof(uart_buffer),
             "\r\n--- Sensor Data ---\r\n"
             "PIR Sensor   : %s\r\n"
             "CDS AO Value : %lu\r\n"
             "CDS DO State : %s\r\n"
             "--------------------\r\n",
             (pir_state == GPIO_PIN_SET) ? "MOTION" : "NO MOTION",
             cds_value,
             (cds_digital_state == GPIO_PIN_SET) ? "HIGH" : "LOW");

    UART_SendString(uart_buffer);
}

/**
 * @brief USART3로 문자열 전송
 */
void UART_SendString(char *str) {
    HAL_UART_Transmit(&huart3, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
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
    MX_ADC1_Init();

    while (1) {
        Read_Sensors(); // 센서 데이터 읽기 및 UART 전송
        HAL_Delay(4000); // 5초 대기
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

/**
  * @brief ADC1 Initialization Function
  * @retval None
  */
static void MX_ADC1_Init(void) {
    ADC_ChannelConfTypeDef sConfig = {0};
    hadc1.Instance = ADC1;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.NbrOfConversion = 1;
    HAL_ADC_Init(&hadc1);

    sConfig.Channel = ADC_CHANNEL_15; // PB0
    sConfig.Rank = ADC_REGULAR_RANK_1;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

/**
  * @brief USART3 Initialization Function
  * @retval None
  */
static void MX_USART3_UART_Init(void) {
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(&huart3);
}

/**
  * @brief GPIO Initialization Function
  * @retval None
  */
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // PIR 센서 (PC6)
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // CDS 센서 DO (PC7)
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
  * @brief  Error Handler
  * @retval None
  */
void Error_Handler(void) {
    __disable_irq();
    while (1) {}
}
