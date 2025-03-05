#include "main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"
#include "../../Module/E220-900T22D/Inc/E220-900T22D.h"
#include "../../Module/Queue/Inc/Queue.h"
#include "../../Module/Sensor/Inc/Sensor.h"
#include <stdio.h>
#include <string.h>

//#define ROLA_TX
#define ROLA_RX

uint8_t rx_buffer[MAX_PACKET_SIZE];
uint8_t tempBuffer[MAX_PACKET_SIZE];
CircularQueue loRaTxQueue, loRaRxQueue;

void SystemClock_Config(void);

// === 보완 추가: 디버그 및 에러 확인용 변수들 ===
volatile uint8_t uartRxCompleteFlag = 0;  // Live Expression 확인용 (수신 성공 카운트)
volatile uint8_t uartErrorCount = 0;       // Live Expression 확인용 (수신 에러 카운트)
volatile uint8_t debugM0 = 0;               // Live Expression 확인용 (M0 핀 상태)
volatile uint8_t debugM1 = 0;               // Live Expression 확인용 (M1 핀 상태)

// printf 재정의 (디버그 로그 출력용 - 송신부용)
int fputc(int ch, FILE *f) {
    HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

// UART 수신 완료 콜백
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
#ifdef ROLA_RX
    if (huart == &huart1) {
        memcpy(tempBuffer, rx_buffer, MAX_PACKET_SIZE);
        Enqueue(&loRaRxQueue, tempBuffer);
        uartRxCompleteFlag++;  // 매 수신마다 값 증가 (Live Expression 감지용)

        // 기존 코드 유지 (디버그 로그는 필요에 따라 제거 가능)
        printf("[LoRa RX] Data: %02X %02X %02X %02X\n",
               tempBuffer[0], tempBuffer[1], tempBuffer[2], tempBuffer[3]);

        // 다음 수신 대기 설정 (기존 유지)
        HAL_UART_Receive_IT(&huart1, rx_buffer, MAX_PACKET_SIZE);
    }
#endif
}

// === 보완 추가: UART 에러 콜백 추가 (수신부 안정성 강화) ===
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
#ifdef ROLA_RX
    if (huart == &huart1) {
        uartErrorCount++;  // 에러 감지 (Live Expression 확인용)
        HAL_UART_Receive_IT(&huart1, rx_buffer, MAX_PACKET_SIZE);  // 에러 후에도 다시 수신 대기
    }
#endif
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();

    InitQueue(&loRaTxQueue);
    InitQueue(&loRaRxQueue);
    InitSensor();
    InitLoRaModule();

    // === 보완 추가: M0/M1 실제 핀 상태 저장 (Live Expression 확인용) ===
    debugM0 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
    debugM1 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);

#ifdef ROLA_RX
    printf("LoRa Receiver Mode Start\n");
    HAL_UART_Receive_IT(&huart1, rx_buffer, MAX_PACKET_SIZE);  // 최초 수신 대기
#endif

    while (1) {
#ifdef ROLA_TX
        ReadSensors();
        ControlLED();
#endif

#ifdef ROLA_RX
        ProcessLoRaRxQueue(&loRaRxQueue);
#endif

        HAL_Delay(1000);
    }
}

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

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
