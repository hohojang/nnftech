/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include <string.h>

UART_HandleTypeDef huart2; // UART 핸들러 선언

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void); // 시스템 클록 설정 함수 프로토타입
static void MX_GPIO_Init(void); // GPIO 초기화 함수 프로토타입
static void MX_USART2_UART_Init(void); // USART2 초기화 함수 프로토타입

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init(); // HAL 초기화

  SystemClock_Config(); // 시스템 클록 설정

  MX_GPIO_Init(); // GPIO 초기화
  MX_USB_DEVICE_Init(); // USB 장치 초기화

  while (1)
  {
    char *msg = "지정한 단어입니다.\r\n"; // 전송할 문자열 정의

    CDC_Transmit_FS((uint8_t*)msg, strlen(msg)); // 문자열 전송

    HAL_Delay(2000); // 2초 대기
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0}; // RCC 오실레이터 초기화 구조체
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0}; // RCC 클록 초기화 구조체

  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler(); // 전압 조정 실패 시 에러 핸들러 호출
  }

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
    Error_Handler(); // 오실레이터 설정 실패 시 에러 핸들러 호출
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2; // 클록 타입 설정
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler(); // 클록 설정 실패 시 에러 핸들러 호출
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2; // USART2 인스턴스 설정
  huart2.Init.BaudRate = 115200; // 보드레이트 설정
  huart2.Init.WordLength = UART_WORDLENGTH_8B; // 데이터 비트 길이 설정
  huart2.Init.StopBits = UART_STOPBITS_1; // 정지 비트 설정
  huart2.Init.Parity = UART_PARITY_NONE; // 패리티 설정
  huart2.Init.Mode = UART_MODE_TX_RX; // 송수신 모드 설정
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE; // 하드웨어 흐름 제어 설정
  huart2.Init.OverSampling = UART_OVERSAMPLING_16; // 오버샘플링 설정
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE; // 1비트 샘플링 비활성화
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; // 고급 기능 초기화 비활성화
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler(); // UART 초기화 실패 시 에러 핸들러 호출
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0}; // GPIO 초기화 구조체

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE(); // GPIOC 클록 활성화
  __HAL_RCC_GPIOH_CLK_ENABLE(); // GPIOH 클록 활성화
  __HAL_RCC_GPIOA_CLK_ENABLE(); // GPIOA 클록 활성화
  __HAL_RCC_GPIOB_CLK_ENABLE(); // GPIOB 클록 활성화

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET); // LD4 핀 초기화

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin; // B1 핀 설정
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // 인터럽트 모드 설정
  GPIO_InitStruct.Pull = GPIO_NOPULL; // 풀업/풀다운 비활성화
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct); // B1 핀 초기화

  /*Configure GPIO pin : LD4_Pin */
  GPIO_InitStruct.Pin = LD4_Pin; // LD4 핀 설정
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // 푸시풀 출력 모드 설정
  GPIO_InitStruct.Pull = GPIO_NOPULL; // 풀업/풀다운 비활성화
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 속도 설정
  HAL_GPIO_Init(LD4_GPIO_Port, &GPIO_InitStruct); // LD4 핀 초기화
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq(); // 인터럽트 비활성화
  while (1)
  {
    // 에러 발생 시 무한 루프
  }
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
  // assert 실패 시 사용자 정의 구현 추가 가능
}
#endif /* USE_FULL_ASSERT */
