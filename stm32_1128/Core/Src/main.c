#include "main.h"

// UART 핸들러 선언
UART_HandleTypeDef huart2;
// DMA 핸들러 선언
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart2_rx;  // USART2 수신용 DMA 핸들러

// 송신 및 수신 데이터 버퍼
uint8_t tx_data[] = "안녕 안녕~~!\r\n";
uint8_t rx_data[64];  // 수신 데이터 버퍼

// 함수 선언
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);  // 수신 완료 콜백 함수

int main(void)
{
    HAL_Init();  // HAL 라이브러리 초기화
    SystemClock_Config();  // 시스템 클럭 설정

    MX_GPIO_Init();  // GPIO 초기화
    MX_DMA_Init();   // DMA 초기화
    MX_USART2_UART_Init();  // UART2 초기화

    // UART DMA로 문자열 송신
    HAL_UART_Transmit_DMA(&huart2, tx_data, sizeof(tx_data) - 1);  // NULL 문자 제외

    // DMA로 UART 수신 시작
    HAL_UART_Receive_DMA(&huart2, rx_data, sizeof(rx_data));  // 수신 시작

    while (1)
    {
        // 필요에 따라 추가적인 로직을 작성할 수 있음
        // 예: 버튼 눌림 확인, 상태 변경 등
    }
}

// 시스템 클럭 설정 함수
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
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
        Error_Handler();
    }

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

// USART2 UART 초기화 함수
static void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;  // 보드레이트 설정
    huart2.Init.WordLength = UART_WORDLENGTH_8B;  // 데이터 길이 설정
    huart2.Init.StopBits = UART_STOPBITS_1;  // 스톱 비트 설정
    huart2.Init.Parity = UART_PARITY_NONE;  // 패리티 비트 설정
    huart2.Init.Mode = UART_MODE_TX_RX;  // 송수신 모드 설정
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;  // 하드웨어 흐름 제어 없음
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;  // 오버샘플링 설정
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;  // 1비트 샘플링 비활성화
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;  // 고급 기능 비활성화
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();  // UART 초기화 실패 시 에러 핸들러 호출
    }
}

// DMA 초기화 함수
static void MX_DMA_Init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();  // DMA1 클록 활성화

    // USART2 TX DMA 설정
    HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);  // DMA1 채널7 IRQ 설정 및 활성화
    HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

    // USART2 RX DMA 설정
    hdma_usart2_rx.Instance = DMA1_Channel6;  // DMA1 채널6 (USART2 RX)
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;  // Peripheral -> Memory
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;  // Peripheral 주소 자동 증가 비활성화
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;  // Memory 주소 자동 증가 활성화
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  // 데이터 정렬 설정
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;  // 메모리 데이터 정렬 설정
    hdma_usart2_rx.Init.Mode = DMA_CIRCULAR;  // 순환 모드
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;  // 우선순위 설정
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
        Error_Handler();  // DMA 초기화 실패 시 에러 핸들러 호출
    }

    // DMA와 UART 연결
    __HAL_LINKDMA(&huart2, hdmarx, hdma_usart2_rx);
}

// GPIO 초기화 함수
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // GPIO 포트 클록 활성화
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // LD4 핀 초기화 (출력 핀)
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);

    // B1 핀 초기화 (입력 핀, 외부 인터럽트 사용)
    GPIO_InitStruct.Pin = B1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

    // LD4 핀 초기화 (출력 핀)
    GPIO_InitStruct.Pin = LD4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LD4_GPIO_Port, &GPIO_InitStruct);
}

// UART 수신 완료 콜백 함수
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        // 수신된 데이터를 처리하는 코드
        // 예: 수신된 데이터를 다른 함수에 전달하거나 출력
        HAL_UART_Transmit(&huart2, rx_data, sizeof(rx_data), HAL_MAX_DELAY);  // 수신된 데이터 다시 송신
    }
}

// 오류 발생 시 호출되는 함수
void Error_Handler(void)
{
    __disable_irq();  // 인터럽트 비활성화
    while (1)
    {
        // 오류 발생 시 무한루프
    }
}

#ifdef USE_FULL_ASSERT
// assert_failed 함수
void assert_failed(uint8_t *file, uint32_t line)
{
    // Assertion 실패 시 처리 코드
}
#endif
