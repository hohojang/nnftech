####################### STM32 펌웨어 기초 6-2 RESET COUNT #########################


[결과 프로젝트 영상](https://vimeo.com/1033384079)



24-11-26 


어제 발생한 문제 : 나의 보드 CPU는 RESET 버튼을 일반 버튼으로 바꾸는것이 불가능한가? 


결론 : 불가능 


변경 : RESET 버튼 대신 보드 내장 버튼을 사용 


순서

Cube Mx에서 (PC13) 보드의 파란색 버튼 GPIO PIN 세팅 

USART2 사용 / 보드레이트 115200 

코드 생성 

main.h 

#include <string.h> // 문자열 처리를 위한 헤더 라이브러리 import 

extern volatile uint32_t reset_count; // 전역 변수 선언

main.c

#include "string.h" // 문자열 처리를 위한 헤더 라이브러리 import

UART_HandleTypeDef huart2; // UART 핸들

volatile uint32_t reset_count = 0; // 리셋 카운트 변수

// 초기 메시지 출력

    char initial_message[] = "카운트 시작! 파란 버튼 클릭\n";
    
    HAL_UART_Transmit(&huart2, (uint8_t *)initial_message, strlen(initial_message), HAL_MAX_DELAY);

//메인 루프 부분은 항상 활성 상태를 유지하며 이벤트에 대응할 준비를 한다.
 // 메인 루프
    while (1)
    {
        // 특별한 작업 필요 없음. 인터럽트(이벤트)에 의해 동작.


        /* GPIO 핀 : PC13 설정 */
        
    GPIO_InitStruct.Pin = GPIO_PIN_13; // 보드 내장 버튼 핀
    
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//인터럽트를 활성화하지 않으면 해당 핀에서 발생한 신호가 NVIC로 전달x

핸들러가 호출되지 않기 때문에 

EXTI15_10_IRQn: NVIC가 관리하는 외부 인터럽트 라인 10~15 그룹을 활성화

0,0 -> 최고 우선순위 //

     // NVIC 인터럽트 활성화 
     
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}


// GPIO 인터럽트 콜백 함수
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_13)// 파란색 버튼 (PC13)이 눌리면! 
    {
        reset_count++;// 카운트 증가

        // USART를 통해 카운트 출력
        char msg[50];
        sprintf(msg, "\rCOUNT: %lu\n", reset_count);
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    }
}





     

