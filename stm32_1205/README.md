# STM32 펌웨어 기초 14 | 15 -(USB DFU #1 , #2) Flash와 USART 통신을 통해 DFU 부트로더 진입 

STM32L4 시리즈 MCU를 사용하여 플래시 메모리의 지우기, 쓰기, 읽기 작업을 수행하고, USART2 및 USART3을 통해 데이터를 전송하며,

특정 조건에서 USB DFU 부트로더로 진입하는 기능을 구현

# 문제해결
STM32L4 MCU에서 플래시 메모리 작업을 수행할 때 메모리 주소와 페이지 크기 설정이 중요합니다.
UART를 통한 데이터 송수신 및 USB DFU 부트로더로의 전환을 구현하기 위한 적절한 초기화가 필요합니다.
주요 기능
플래시 메모리 작업:

지정된 주소에서 플래시 메모리를 지운 후, 64비트 데이터를 기록.
기록된 데이터를 읽고 결과를 USART2를 통해 출력.
USART2 및 USART3을 통한 데이터 송수신:

UART_SendString 함수를 사용하여 주기적으로 데이터를 전송.
USB DFU 부트로더로 진입:

'd' 문자를 수신하면 USB DFU 부트로더로 진입.
주요 함수
1. flash_erase_write_read()
기능: 플래시 메모리 지우기, 쓰기 및 읽기를 수행.
동작: 플래시 메모리를 지운 후 64비트 데이터를 기록하고, 읽은 데이터를 UART를 통해 출력.

2. **UART_SendString(UART_HandleTypeDef huart, const char str)
기능: UART를 통해 문자열을 전송.
동작: HAL_UART_Transmit()을 사용하여 문자열을 전송.

3. GetPage(uint32_t Address)
기능: 플래시 메모리의 주소에 해당하는 페이지 번호를 계산.
동작: 주어진 주소에 대한 플래시 페이지 번호를 계산하여 반환.

4. JumpToDFU()
기능: USB DFU 부트로더로 진입.
동작: STM32L4의 부트로더 주소로 점프하여 USB DFU 모드로 전환.

5. SystemClock_Config()
기능: 시스템 클럭을 설정.
동작: 외부 고속 클럭(HSE)을 활성화하고 PLL을 설정하여 시스템 클럭을 구성.

6. MX_USART2_UART_Init() & MX_USART3_UART_Init()
기능: USART2 및 USART3 초기화.
동작: USART 설정을 정의하고, HAL_UART_Init()으로 초기화.

7. MX_GPIO_Init()
기능: GPIO 포트를 초기화.
동작: PA2/PA3와 PC10/PC11을 각각 USART2와 USART3의 TX/RX 핀으로 설정.

8. Error_Handler()
기능: 오류 발생 시 시스템을 멈추고 에러를 처리.
동작: 인터럽트를 비활성화하고 무한 루프에 들어감.

하드웨어 요구사항
STM32L4 시리즈 MCU (예: STM32L452RE Nucleo 보드)
USART2, USART3 핀 연결
USART2: PA2 (TX), PA3 (RX)
USART3: PC10 (TX), PC11 (RX)
USB DFU 기능을 사용하기 위해 USB 연결이 가능한 환경
소프트웨어 요구사항
STM32CubeIDE 또는 STM32CubeMX
STM32 HAL 라이브러리
USB DFU 드라이버 (USB DFU 모드 진입을 위한 장치 연결 필요)
기능 설명
플래시 메모리 지우기, 쓰기 및 읽기

지정된 플래시 주소에서 플래시 메모리를 지우고 64비트 데이터를 기록.
기록된 데이터를 읽어 결과를 USART2를 통해 출력.
USART 데이터 전송

UART_SendString 함수를 사용하여 주기적으로 "Data sent via USART2"와 "Data sent via USART3" 메시지를 각각 USART2와 USART3을 통해 전송.
DFU 부트로더 진입

UART2에서 'd' 문자가 수신되면 JumpToDFU() 함수를 호출하여 USB DFU 부트로더로 진입.
주기적인 동작

2초마다 플래시 작업을 수행하고 데이터를 전송하는 주기를 설정.
파일 설명
main.c: 주요 프로그램 코드, 플래시 작업, UART 통신 및 DFU 부트로더 기능 구현.
stm32l4xx_hal_flash.h: STM32L4 시리즈의 플래시 작업을 위한 헤더 파일.
usb_device.h: USB DFU 기능을 위한 헤더 파일.
main.h: 프로젝트의 헤더 파일.
빌드 및 실행
STM32CubeIDE를 열고 프로젝트를 로드.
빌드 후 보드에 코드를 다운로드.
터미널 프로그램을 사용하여 USART2와 USART3의 출력 데이터를 확인.
'd' 문자를 UART2로 전송하면 USB DFU 부트로더로 진입.

