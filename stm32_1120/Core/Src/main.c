// stm32_1120.c
#include "main.h"

// 함수 프로토타입 선언
// 시스템 클럭을 설정하는 함수
void SystemClock_Config(void);
// GPIO 초기화 함수
static void MX_GPIO_Init(void);

int main(void) {
    // HAL 라이브러리 초기화
    HAL_Init();

    // 시스템 클럭 설정
    SystemClock_Config();

    // GPIO 초기화
    MX_GPIO_Init();

    // 무한 루프
    while (1) {
        // PA5 핀의 상태를 토글
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

        // 2초 대기
        HAL_Delay(2000);
    }
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // HSI 오실레이터 설정
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI; // HSI 오실레이터 타입 설정
    RCC_OscInitStruct.HSIState = RCC_HSI_ON; // HSI 오실레이터 활성화
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT; // 기본 보정 값 설정
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE; // PLL 비활성화

    // 오실레이터 구성
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    // CPU, AHB, APB 클럭 설정
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; // 클럭 타입 설정
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI; // 시스템 클럭 소스 설정
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1; // AHB 클럭 분주 설정
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1; // APB1 클럭 분주 설정

    // 클럭 구성
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // GPIOA 클럭 활성화
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // PA5 핀 설정
    GPIO_InitStruct.Pin = GPIO_PIN_5; // PA5 핀 번호
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // 푸시풀 출력 모드 설정
    GPIO_InitStruct.Pull = GPIO_NOPULL; // 풀업/풀다운 설정 없음
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // 낮은 속도로 설정

    // GPIO 초기화
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
