//stm32_1203과 stm32_1204의 내용 동일

#include "main.h"
#include "stm32l4xx_hal_flash.h"  // STM32L4에 필요한 헤더 포함
#include <stdio.h>
#include <string.h>  // strlen을 사용하기 위한 헤더 파일 추가

// 플래시 주소 및 상수 정의
#define FLASH_ADDRESS 0x08020000  // 플래시 작업을 수행할 시작 주소

#ifndef FLASH_PAGE_SIZE
#define FLASH_PAGE_SIZE 0x800      // 플래시 페이지 크기 (2KB)
#endif

#ifndef FLASH_BANK_SIZE
#define FLASH_BANK_SIZE 0x100000   // 플래시 은행 크기 (1MB)
#endif

// UART 핸들러 선언
UART_HandleTypeDef huart2;  // USART2 핸들러
UART_HandleTypeDef huart3;  // USART3 핸들러

// 함수 프로토타입
void flash_erase_write_read(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_GPIO_Init(void);
void SystemClock_Config(void);
void Error_Handler(void);
static uint32_t GetPage(uint32_t Address);
void UART_SendString(UART_HandleTypeDef *huart, const char *str);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();

    // 주기적으로 데이터를 전송
    while (1)
    {
        flash_erase_write_read();
        UART_SendString(&huart2, "Data sent via USART2.\r\n");
        UART_SendString(&huart3, "Data sent via USART3.\r\n");
        HAL_Delay(2000);  // 2초 대기
    }
}

// 플래시 메모리 지우기, 쓰기, 읽기 함수
void flash_erase_write_read(void)
{
    uint64_t data_to_write = 0x123456789ABCDEF0;  // 64비트 데이터
    uint64_t read_data = 0;

    HAL_FLASH_Unlock();

    // 플래시 메모리 지우기 설정
    FLASH_EraseInitTypeDef eraseInit;
    uint32_t PageError;

    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;  // 페이지 단위로 지우기
    eraseInit.Banks = FLASH_BANK_1;  // STM32L4에서는 Bank 지정 필요
    eraseInit.Page = GetPage(FLASH_ADDRESS);  // 시작 페이지
    eraseInit.NbPages = 1;  // 지울 페이지 수 (1페이지)

    if (HAL_FLASHEx_Erase(&eraseInit, &PageError) != HAL_OK)
    {
        UART_SendString(&huart2, "Flash Erase Failed!\r\n");
    }
    else
    {
        UART_SendString(&huart2, "Flash Erase Success!\r\n");
    }

    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_ADDRESS, data_to_write) != HAL_OK)
    {
        UART_SendString(&huart2, "Flash Write Failed!\r\n");
    }
    else
    {
        UART_SendString(&huart2, "Flash Write Success!\r\n");
    }

    read_data = *(volatile uint64_t*)FLASH_ADDRESS;
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "Read Data: 0x%016llX\r\n", (unsigned long long)read_data);
    UART_SendString(&huart2, buffer);

    HAL_FLASH_Lock();
}

// UART 문자열 전송 함수
// UART 문자열 전송 함수
void UART_SendString(UART_HandleTypeDef *huart, const char *str)
{
    if (HAL_UART_Transmit(huart, (uint8_t*)str, strlen(str), HAL_MAX_DELAY) != HAL_OK)
    {
        // 전송 실패 시 에러 메시지 출력
        Error_Handler();  // 또는 다른 에러 처리 방법
    }
}

// 주소에 해당하는 플래시 페이지를 계산하는 함수
static uint32_t GetPage(uint32_t Address)
{
    return (Address - FLASH_BASE) / FLASH_PAGE_SIZE;
}

// 시스템 클럭 설정
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 20;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
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

    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        Error_Handler();
    }
}

// GPIO 초기화
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // USART2 핀 초기화
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;  // PA2, PA3 (TX, RX)
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;  // USART2 대체 기능
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // USART3 핀 초기화
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;  // PC10, PC11 (TX, RX)
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;  // USART3 대체 기능
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

// 에러 처리 함수
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}
