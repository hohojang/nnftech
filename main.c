#include "main.h"
#include "gpio.h"
#include "adc.h"
#include "usart.h"
#include "../../Module/E220-900T22D/Inc/E220-900T22D.h"
#include "../../Module/Queue/Inc/Queue.h"
#include "../../Module/Sensor/Inc/Sensor.h"
#include "../../Module/Protocol/Inc/ComProtocol.h"
#include <stdio.h>

// 📌 LoRa 송수신을 위한 전역 버퍼 (여기서만 정의)
volatile uint8_t rx_buffer[MAX_PACKET_SIZE];

// 📌 LoRa 송수신을 위한 큐
CircularQueue loRaTxQueue, loRaRxQueue;

void SystemClock_Config(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {
        printf("[LoRa RX] UART Interrupt Triggered! Data Received: ");
        for (uint8_t i = 0; i < MAX_PACKET_SIZE; i++) {
            printf("%02X ", rx_buffer[i]);
        }
        printf("\n");

        // 📌 수신된 데이터를 LoRa 수신 큐에 추가 (캐스팅 적용)
        Enqueue(&loRaRxQueue, (uint8_t *)rx_buffer, MAX_PACKET_SIZE);

        // 📌 다음 데이터 수신 준비
        HAL_UART_Receive_IT(&huart1, (uint8_t *)rx_buffer, MAX_PACKET_SIZE);
    }
}


int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();

    InitQueue(&loRaTxQueue);
    InitQueue(&loRaRxQueue);

    // 📌 LoRa 초기 설정
    ReceiveLoRaPacket();

    while (1) {
        ReadSensors();
        ControlLED();
        ProcessLoRaTxQueue(&loRaTxQueue);
        ProcessLoRaRxQueue(&loRaRxQueue);
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
