############################### nnftech 2024-11-19 ###############################

# STM32 펌웨어 기초 02 - LED #1 

   ## 동영상
   [241119 동영상 보기](https://vimeo.com/1031400673)

///////////////↓ 순 서////////////////// 

프로젝트 생성

핀 설정 및 소스코드 생성 [ioc 파일 ]

System Core > GPIO > PA5 핀의 User Label을 나중에 작업시 알아보기 쉽게 led로 변경

System Core > GPIO > PC13 핀의 값을 변경 ↓ ↓

GPIO mode: 'External Interrupt Mode with Rising edge trigger detection'

GPIO Pull-up/Pull-down: No pull-up and no pull-down

User Label: BTN

System Core > GPIO > NVIC에서 Exit line[15:10] interrupts의 Enabled 선택

System Core > NVIC > NVIC에서 Exit line[15:10] interrupts의 Enabled 선택

System Core > NVIC > NVIC에서 Exit line[15:10] interrupts의 Enabled 선택

'Ctrl + S' 또는 'Code Generation' 버튼을 눌러서 코드를 생성

stm32f1xx_hal_gpio.c 파일에 HAL_GPIO_EXTI_Callback 함수는 __weak로 정의되어 있음

이 함수를 main.c 에 재 선언해서 구현해서 interrupt를 처리

main.c 함수에 아래와 같이 HAL_GPIO_EXTI_Callback 함수를 구함

---------------------------------------------------------------------------------
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == BTN_Pin)
	{
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);  // LED 모드 변경
	}
}

-----------------------------------------------------------------------------------


디버깅 할때 초록색 벌레표시를 꼭 눌른 다음 오른쪽 초록색 화살표를 눌러야함 
