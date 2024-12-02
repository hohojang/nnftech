#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h" // USB CDC 인터페이스 헤더 파일 포함
// USB 장치 초기화 함수 선언 (외부 함수)
extern void MX_USB_DEVICE_Init(void);

// UART 핸들러 선언 (외부 변수)
extern UART_HandleTypeDef huart2; // USART2 핸들러
extern UART_HandleTypeDef huart3; // USART3 핸들러

uint8_t rxData; // 수신 데이터를 저장할 변수
uint8_t msg[] = "stm32_1202 usb to serial!\r\n"; // 초기 메시지 전송용 데이터

// 시스템 클럭 설정 함수 (시스템 전체의 클럭을 설정)
void SystemClock_Config(void);

// GPIO 초기화 함수 (LED 및 버튼 핀 설정)
static void MX_GPIO_Init(void);

// USART2 초기화 함수
static void MX_USART2_UART_Init(void);

// USART3 초기화 함수
static void MX_USART3_UART_Init(void);

// UART 수신 인터럽트 콜백 함수 (데이터 수신 시 실행)
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

// USB 수신 데이터 처리 함수
void USB_Data_Receive(uint8_t *data, uint16_t length);

// UART 인터럽트 콜백 함수 정의
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // USART2에서 데이터가 수신된 경우
    if (huart->Instance == USART2)
    {
        // USART3로 수신 데이터를 전송
        HAL_UART_Transmit(&huart3, &rxData, 1, HAL_MAX_DELAY);
        // USB로 수신 데이터를 전송
        USB_Data_Receive(&rxData, 1);
        // USART2의 수신 인터럽트를 재설정하여 다음 데이터를 대기
        HAL_UART_Receive_IT(&huart2, &rxData, 1);
    }
    // USART3에서 데이터가 수신된 경우
    else if (huart->Instance == USART3)
    {
        // USART2로 수신 데이터를 전송
        HAL_UART_Transmit(&huart2, &rxData, 1, HAL_MAX_DELAY);
        // USB로 수신 데이터를 전송
        USB_Data_Receive(&rxData, 1);
        // USART3의 수신 인터럽트를 재설정하여 다음 데이터를 대기
        HAL_UART_Receive_IT(&huart3, &rxData, 1);
    }
}

// USB 데이터 수신 처리 함수
void USB_Data_Receive(uint8_t *data, uint16_t length)
{
    // USB로 수신된 데이터를 UART로 전송
    CDC_Transmit_FS(data, length); // USB CDC를 통해 데이터 전송
}

// 메인 함수: 프로그램의 진입점
int main(void)
{
    HAL_Init(); // HAL 라이브러리 초기화 (타이머, 인터럽트 설정 등)

    SystemClock_Config(); // 시스템 클럭 설정 (CPU 및 주변 장치 클럭)

    MX_GPIO_Init(); // GPIO 핀 초기화 (LED 및 버튼)
    MX_USART2_UART_Init(); // USART2 초기화 (115200bps, 8N1)
    MX_USART3_UART_Init(); // USART3 초기화 (115200bps, 8N1)
    MX_USB_DEVICE_Init(); // USB 장치 초기화 (CDC 클래스 사용)

    // USART2로 초기 메시지 전송
    HAL_UART_Transmit(&huart2, msg, sizeof(msg), HAL_MAX_DELAY);

    // UART 수신 인터럽트 활성화 (USART2 및 USART3)
    HAL_UART_Receive_IT(&huart2, &rxData, 1);
    HAL_UART_Receive_IT(&huart3, &rxData, 1);

    // 메인 루프
    while (1)
    {
        // 별다른 작업 없음 (UART 통신은 인터럽트로 처리됨)
    }
}

// 시스템 클럭 설정 함수
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0}; // 오실레이터 설정 구조체
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0}; // 클럭 설정 구조체

    // 전압 스케일 설정
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler(); // 에러 발생 시 처리
    }

    HAL_PWR_EnableBkUpAccess(); // 백업 영역 접근 활성화
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW); // LSE 구동 설정

    // 오실레이터 초기화
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON; // LSE 활성화
    RCC_OscInitStruct.MSIState = RCC_MSI_ON; // MSI 활성화
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6; // MSI 클럭 범위 설정
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON; // PLL 활성화
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI; // PLL 소스 선택
    RCC_OscInitStruct.PLL.PLLM = 1; // PLL 분배
    RCC_OscInitStruct.PLL.PLLN = 16; // PLL 곱셈
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7; // PLL P 분주비
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2; // PLL Q 분주비
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2; // PLL R 분주비
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    // 클럭 구성
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // PLLCLK를 시스템 클럭으로 설정
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1; // AHB 클럭 분배
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1; // APB1 클럭 분배
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1; // APB2 클럭 분배
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_RCCEx_EnableMSIPLLMode(); // MSI PLL 모드 활성화
}

// USART2 초기화 함수
static void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2; // USART2 설정
    huart2.Init.BaudRate = 115200; // 보드레이트 설정
    huart2.Init.WordLength = UART_WORDLENGTH_8B; // 데이터 길이 8비트
    huart2.Init.StopBits = UART_STOPBITS_1; // 정지 비트 1비트
    huart2.Init.Parity = UART_PARITY_NONE; // 패리티 비트 없음
    huart2.Init.Mode = UART_MODE_TX_RX; // 송수신 모드
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE; // 하드웨어 흐름 제어 없음
    huart2.Init.OverSampling = UART_OVERSAMPLING_16; // 오버샘플링 16배
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE; // 1비트 샘플링 비활성화
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; // 고급 기능 비활성화
    if (HAL_UART_Init(&huart2) != HAL_OK) // UART 초기화
    {
        Error_Handler();
    }
}

// USART3 초기화 함수 (위와 동일한 설정)
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
    huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        Error_Handler();
    }
}

// GPIO 초기화 함수
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0}; // GPIO 설정 구조체

    /* GPIO 포트 클럭 활성화 */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* LED 핀 초기화 */
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = LD4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // 출력 모드
    GPIO_InitStruct.Pull = GPIO_NOPULL; // 풀업/풀다운 없음
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 속도 설정
    HAL_GPIO_Init(LD4_GPIO_Port, &GPIO_InitStruct);

    /* 버튼 핀 초기화 */
    GPIO_InitStruct.Pin = B1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // 하강 엣지 인터럽트
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);
}

// 에러 처리 함수
void Error_Handler(void)
{
    while (1)
    {
        // 무한 루프 (LED 깜빡임 또는 디버깅 추가 가능)
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
