#include "main.h"
#include "stm32l4xx_hal_flash.h"  // STM32L4에 필요한 헤더 포함

#define FLASH_ADDRESS 0x08020000  // 플래시 주소
#define FLASH_PAGE_SIZE 0x800      // STM32L4에서 페이지 크기

UART_HandleTypeDef huart2;  // USART2 핸들러
UART_HandleTypeDef huart3;  // USART3 핸들러

void flash_erase_write_read(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_GPIO_Init(void);
void SystemClock_Config(void);
void Error_Handler(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();

    // 주기적으로 플래시 메모리 작업을 수행
    while (1)
    {
        flash_erase_write_read();
        HAL_Delay(2000);  // 2초마다 플래시 작업을 반복
    }
}

// 플래시 메모리 지우고 쓰고 읽는 함수
void flash_erase_write_read(void)
{
    uint32_t data_to_write = 0x12345678;  // 쓸 데이터
    uint32_t read_data = 0;

    // 플래시를 지우기 전에 Unlock 해야 함
    HAL_FLASH_Unlock();

    // 플래시 메모리 지우기
    FLASH_EraseInitTypeDef eraseInit;
    uint32_t PageError;

    // STM32L4에서의 플래시 지우기 설정
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;  // 페이지 단위로 지우기
    eraseInit.Page = FLASH_SECTOR_4;  // 지울 페이지 설정
    eraseInit.NbPages = 1;  // 지울 페이지 수 설정 (1 페이지)

    // 플래시 지우기 실행
    if (HAL_FLASHEx_Erase(&eraseInit, &PageError) != HAL_OK)
    {
        printf("Flash Erase Failed!\r\n");
    }
    else
    {
        printf("Flash Erase Success!\r\n");
    }

    // 플래시 메모리에 데이터 쓰기
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_ADDRESS, data_to_write) != HAL_OK)
    {
        printf("Flash Write Failed!\r\n");
    }
    else
    {
        printf("Flash Write Success!\r\n");
    }

    // 플래시에서 데이터 읽기
    read_data = *(volatile uint32_t*)FLASH_ADDRESS;
    printf("Read Data: 0x%08X\r\n", read_data);

    // 플래시 잠금
    HAL_FLASH_Lock();
}

// 시스템 클럭 설정
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

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
}

// USART2 초기화
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

// USART3 초기화
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

// GPIO 초기화
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = B1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LD4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LD4_GPIO_Port, &GPIO_InitStruct);
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    // 사용자 정의 구현 추가 가능
}
#endif /* USE_FULL_ASSERT */
