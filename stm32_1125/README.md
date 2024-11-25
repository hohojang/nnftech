########################2024-11-25############################

STM32 펌웨어 기초 06 Reset Count 

STM32 NUCLEO-L452RE 보드의 Reset 버튼을 이용해 버튼을 누를 시 

터미널에서 count 문자가 출력되도록 하는 프로젝트 

@문제 : 유튜브[https://www.youtube.com/watch?v=Pj3Iel6zrHA&list=PLvFHFPM09alKygQq-L6_6DwuNqTybIAw0&index=7] 강의를 진행 

하시는 분의 경우 사용하는 보드가 달라 내장 reset 버튼을 일반 버튼으로 변경 가능한 기능이 있다. 

나의 경우 STM32L452RE의 회로나 문서를 찾아보고 있지만 아직 찾지 못했음[내장 reset 버튼을 일반 버튼으로 변경 & 사용 가능한지]

현재 진행 및 코드의 경우 [main.c]

CUBE MX 에서 RESET 버튼 핀 설정(PA0) 

#include "string.h" // 문자열 처리용 라이브러리 import 

volatile uint32_t reset_count = 0; // 리셋 카운트 변수 생성 

메인 함수 

 // 시작 메시지 출력 생성 
    char message[] = "Reset Counter 시작!\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);

// GPIO 인터럽트 콜백 함수 생성 
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_0) // 리셋 버튼이 눌렸을 때
    {
        reset_count++; // 카운트 증가 

        // USART를 통해 카운트 출력
        char msg[50];
        sprintf(msg, "Reset Count: %lu\n", reset_count);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    }
}

/// 내일 할 일 ///

내 cpu가 reset 버튼의 일반 버튼으로의 전환이 불가능 할 경우 

1. 보드의 내장 파란 버튼을 GPIO 설정을 통해 push down 시 터미널에서 문자 출력하도록 바꾸기 

2. 다음 프로젝트 진행


