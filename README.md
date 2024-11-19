############################### nnftech 2024-11-18 ###############################

# STM32 펌웨어 기초 study link ↓
------------------------------------------------------------------------------------------------------------------
[https://www.youtube.com/watch?v=e6gILaoTkvs&list=PLvFHFPM09alKygQq-L6_6DwuNqTybIAw0&index=1]
------------------------------------------------------------------------------------------------------------------
/////stm32 L452RE 보드////// 

------------------------------------------------------------------------------------------------------------------
# 가장 최소한의 프로젝트를 구성하기 위한 파일(mcu 프로그래밍) 
# 1. main.c 2. startup_stm32f103c8tx.s 3. LinkerScript파일
------------------------------------------------------------------------------------------------------------------
  #STM32 펌웨어 구조
  Application Layer (AP)
      ↓
  Hardware Layer (HW)
      ↓
  Board Support Package (BSP)
------------------------------------------------------------------------------------------------------------------
  
# AP (Application Layer)

실제 구현할 응용 프로그램 로직

다른 MCU로 이식할 때 수정이 필요 없는 부분 / ap.c, ap.h 파일에 구현

------------------------------------------------------------------------------------------------------------------

HW (Hardware Layer)
MCU의 주변장치 제어
하드웨어 의존적인 코드
hw.c, hw.h 파일에 구현

------------------------------------------------------------------------------------------------------------------

BSP (Board Support Package)
보드 초기화
기본적인 하드웨어 설정
bsp.c, bsp.h 파일에 구현

-------------------------------------------------------------------------------------------------------------------

// 초기화 함수들 
void bspInit(void)  // 보드 초기화
void hwInit(void)   // 하드웨어 초기화
void apInit(void)   // 응용 프로그램 초기화

// 메인 루프
void apMain(void)   // 실제 응용 프로그램이 동작하는 부분

--------------------------------------------------------------------------------------------------------------------

#프로젝트 구조의 장점
[1. 모듈화된 구조로 유지보수가 용이 2. 다른 MCU로 이식이 쉬움 3. 코드의 재사용성이 높음 4. H/W와 S/W의 명확한 분리]



############################### nnftech 2024-11-19 ###############################

# STM32 펌웨어 기초 02 - LED #1 / #2 

1. 프로젝트 생성
2. 핀 설정 및 소스코드 생성 [ioc 파일 ]
2-1. System Core > GPIO > PA5 핀의 User Label을 나중에 작업시 알아보기 쉽게 led로 변경
2-2. System Core > GPIO > PC13 핀의 값을 변경 ↓ ↓
GPIO mode: 'External Interrupt Mode with Rising edge trigger detection'
GPIO Pull-up/Pull-down: No pull-up and no pull-down
User Label: BTN
2-3. System Core > GPIO > NVIC에서 Exit line[15:10] interrupts의 Enabled 선택
     System Core > NVIC > NVIC에서 Exit line[15:10] interrupts의 Enabled 선택
     System Core > NVIC > NVIC에서 Exit line[15:10] interrupts의 Enabled 선택
3. 'Ctrl + S' 또는 'Code Generation' 버튼을 눌러서 코드를 생성
4. stm32f1xx_hal_gpio.c 파일에 HAL_GPIO_EXTI_Callback 함수는 __weak로 정의되어 있음
   이 함수를 main.c 에 재 선언해서 구현해서 interrupt를 처리
5. main.c 함수에 아래와 같이 HAL_GPIO_EXTI_Callback 함수를 구함
5-1. void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == BTN_Pin)
	{
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);  // LED 모드 변경
	}
}
6. 디버깅  디버깅 할때 초록색 벌레표시를 꼭 눌른 다음 오른쪽 초록색 화살표를 눌러야함 
