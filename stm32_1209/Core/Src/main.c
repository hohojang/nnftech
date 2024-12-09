/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define RX_BUFFER_SIZE 1 // UART 수신 버퍼 크기
#define MAX_INPUT_LENGTH 100 // 명령어 최대 길이

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LED_Task */
osThreadId_t LED_TaskHandle;
const osThreadAttr_t LED_Task_attributes = {
  .name = "LED_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CLI_Task */
osThreadId_t CLI_TaskHandle;
const osThreadAttr_t CLI_Task_attributes = {
  .name = "CLI_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */
char rx_buffer[100]; // UART로 받은 데이터를 저장할 버퍼
int rx_index = 0;    // 버퍼에 저장된 데이터의 인덱스
char input_buffer[MAX_INPUT_LENGTH]; // CLI 입력 버퍼
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
void StartDefaultTask(void *argument);
void LED_Task1(void *argument);
void CLI_Task1(void *argument);

/* USER CODE BEGIN PFP */
void UART_SendString(UART_HandleTypeDef *huart, const char *str);
void ProcessCLICommand(char *command);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
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
  MX_USART3_UART_Init();

  /* Init scheduler */
  osKernelInitialize();

  /* Create the thread(s) */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
  LED_TaskHandle = osThreadNew(LED_Task1, NULL, &LED_Task_attributes);
  CLI_TaskHandle = osThreadNew(CLI_Task1, NULL, &CLI_Task_attributes);

  /* Start scheduler */
  osKernelStart();

  while (1) {
  }
}

/**
  * @brief  System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage */
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

    /** Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

  // UART 수신 인터럽트 활성화
  HAL_UART_Receive_IT(&huart2, (uint8_t *)&rx_buffer[0], RX_BUFFER_SIZE);
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
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
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure GPIO pin : LD4_Pin */
  GPIO_InitStruct.Pin = LD4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD4_GPIO_Port, &GPIO_InitStruct);
}

/**
  * @brief Function implementing the StartDefaultTask thread.
  * @param argument: Not used
  * @retval None
  */
void StartDefaultTask(void *argument)
{
    for(;;)
    {
        osDelay(1);
    }
}

/**
  * @brief Function implementing the LED_Task thread.
  * @param argument: Not used
  * @retval None
  */
void LED_Task1(void *argument)
{
  for (;;)//무한 루프
  {
    HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin); // LED 토글
    osDelay(1000); // 1초 대기
  }
}

/**
  * @brief Function implementing the CLI_Task thread.
  * @param argument: Not used
  * @retval None
  */
void CLI_Task1(void *argument)
{
  // 초기 프롬프트 출력
  UART_SendString(&huart2, "cli# ");

  while (1)
  {
    osDelay(100); // CPU 과부하 방지용 대기
    if (rx_index > 0)
    {
      rx_buffer[rx_index] = '\0'; // 문자열 끝을 나타내는 널 문자
      strcpy(input_buffer, rx_buffer); // 받은 데이터를 input_buffer에 복사
      rx_index = 0; // 인덱스 초기화

      // 디버깅 출력
      UART_SendString(&huart2, "Received command: ");
      UART_SendString(&huart2, input_buffer);
      UART_SendString(&huart2, "\r\n");

      // 명령어 처리
      ProcessCLICommand(input_buffer);

      // 명령어 처리 후 cli# 프롬프트 출력
      UART_SendString(&huart2, "\r\ncli# "); // 명령어 처리 후 새로운 프롬프트 출력
    }
  }
}



/* UART 수신 콜백 함수 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) {
        // 수신된 문자를 rx_buffer에 저장
        rx_buffer[rx_index++] = (char)(huart2.Instance->RDR); // RDR로 변경

        // 줄바꿈 문자 또는 캐리지 리턴 문자 수신 시 처리
        if (rx_buffer[rx_index - 1] == '\n' || rx_buffer[rx_index - 1] == '\r') {
            // 줄바꿈 문자 수신 시, 문자열 끝을 처리하고 명령어를 전달
            rx_buffer[rx_index - 1] = '\0';  // 줄바꿈 문자 제거
            UART_SendString(&huart2, "\r\n"); // 응답을 받았다는 표시로 한 줄 추가
            rx_index = 0; // 인덱스 초기화

            // UART 수신을 계속 대기
            HAL_UART_Receive_IT(&huart2, (uint8_t *)&rx_buffer[0], RX_BUFFER_SIZE);
        }
        else if (rx_index >= sizeof(rx_buffer) - 1) {
            // 버퍼 오버플로우 방지
            rx_index = sizeof(rx_buffer) - 1;
        }

        // UART 수신을 계속 대기
        HAL_UART_Receive_IT(&huart2, (uint8_t *)&rx_buffer[rx_index], RX_BUFFER_SIZE);
    }
}


// CLI 명령어 처리 함수
void ProcessCLICommand(char *command)
{
    // 줄바꿈 문자 제거
    command[strcspn(command, "\r\n")] = 0;

    if (strcasecmp(command, "help") == 0)
    {
        UART_SendString(&huart2, "-------- cmd list --------\r\n");
        UART_SendString(&huart2, "HELP - Show available commands\r\n");
        UART_SendString(&huart2, "MD - Display a random message\r\n");
        UART_SendString(&huart2, "HELLO - Display a hello message\r\n"); // 'hello' 명령어 안내 추가
    }
    else if (strcasecmp(command, "md") == 0)
    {
        UART_SendString(&huart2, "This is a random message!\r\n");
    }
    else if (strcasecmp(command, "hello") == 0) // hello 명령어 처리 추가
    {
        UART_SendString(&huart2, "Hello, MY NAME IS JANGHO PARK!\r\n");
    }
    else
    {
        UART_SendString(&huart2, "Unknown command\r\n");
    }
}

// UART로 문자열 전송 함수
void UART_SendString(UART_HandleTypeDef *huart, const char *str)
{
    while (*str)
    {
        HAL_UART_Transmit(huart, (uint8_t *)str++, 1, HAL_MAX_DELAY);
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
