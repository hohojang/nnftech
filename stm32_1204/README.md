# STM32 Flash Memory Erase, Write, and Read Example

이 프로젝트는 STM32L4 시리즈 MCU를 사용하여 플래시 메모리의 지우기, 쓰기, 읽기 작업을 수행하는 예제입니다. 또한, USART2 및 USART3을 통해 데이터를 전송하며, 플래시 메모리 작업의 결과를 터미널로 출력합니다.

## 주요 기능
- 플래시 메모리의 지정된 주소를 지우고, 64비트 데이터를 기록합니다.
- 기록된 데이터를 읽어와 터미널에 출력합니다.
- USART2와 USART3을 통해 데이터를 주기적으로 전송합니다.

##주요 함수
1. flash_erase_write_read()
기능: 플래시 메모리 지우기, 쓰기, 읽기를 수행합니다.
동작:
플래시 메모리 지운 후 64비트 데이터를 기록하고, 읽은 데이터를 UART를 통해 출력합니다.
2. UART_SendString(UART_HandleTypeDef *huart, const char *str)
기능: UART를 통해 문자열을 전송합니다.
동작: HAL_UART_Transmit()을 사용해 문자열을 전송합니다.
3. GetPage(uint32_t Address)
기능: 플래시 메모리의 주소에 해당하는 페이지 번호를 계산합니다.
동작: 주어진 주소에 대한 플래시 페이지 번호를 계산하여 반환합니다.
4. SystemClock_Config()
기능: 시스템 클럭을 설정합니다.
동작: 외부 고속 클럭(HSE)을 활성화하고 PLL을 설정하여 시스템 클럭을 구성합니다.
5. MX_USART2_UART_Init() & MX_USART3_UART_Init()
기능: USART2와 USART3을 초기화합니다.
동작: USART 설정을 정의하고, HAL_UART_Init()으로 초기화합니다.
6. MX_GPIO_Init()
기능: GPIO 포트를 초기화합니다.
동작: PA2/PA3와 PC4/PC5를 USART2와 USART3의 TX/RX 핀으로 설정합니다.
7. Error_Handler()
기능: 오류 발생 시 시스템을 멈추고 에러를 처리합니다.
동작: 인터럽트를 비활성화하고 무한 루프에 들어갑니다.
8. HAL_FLASH_Unlock() & HAL_FLASH_Lock()
기능: 플래시 메모리의 잠금을 해제하고 복구합니다.
동작: 플래시 작업을 위해 잠금을 해제하고, 작업 후 다시 잠급니다.
9. HAL_FLASH_Ex_Erase()
기능: 플래시 메모리에서 지정된 페이지를 지웁니다.
동작: FLASH_EraseInitTypeDef로 페이지 설정 후 HAL_FLASH_Ex_Erase()로 플래시 지웁니다.
10. HAL_FLASH_Program()
기능: 플래시 메모리에 데이터를 기록합니다.
동작: 지정된 주소에 64비트 데이터를 기록합니다.

## 하드웨어 요구사항
- STM32L4 시리즈 MCU (예: STM32L452RE Nucleo 보드)
- USART2, USART3 핀 연결
  - USART2: PA2 (TX), PA3 (RX)
  - USART3: PC4 (TX), PC5 (RX)

## 소프트웨어 요구사항
- STM32CubeIDE 또는 STM32CubeMX
- STM32 HAL 라이브러리

## 기능 설명
1. **플래시 메모리 지우기, 쓰기 및 읽기**:
   - `FLASH_ADDRESS`에 지정된 주소에서 플래시 메모리를 지운 후, 64비트 데이터를 기록합니다.
   - 기록된 데이터를 읽고 결과를 USART2를 통해 출력합니다.

2. **USART 데이터 전송**:
   - `UART_SendString` 함수를 사용하여 주기적으로 "Data sent via USART2"와 "Data sent via USART3" 메시지를 각각 USART2와 USART3을 통해 전송합니다.

3. **주기적인 동작**:
   - 2초마다 플래시 작업을 수행하고, 데이터를 전송하는 주기를 설정합니다.

## 파일 설명
- `main.c`: 주요 프로그램 코드, 플래시 작업 및 UART 통신 설정
- `stm32l4xx_hal_flash.h`: STM32L4 시리즈의 플래시 작업을 위한 헤더 파일
- `main.h`: 프로젝트의 헤더 파일

## 빌드 및 실행
1. STM32CubeIDE를 열고 프로젝트를 로드합니다.
2. 빌드 후 보드에 코드를 다운로드합니다.
3. 터미널 프로그램을 사용하여 USART2와 USART3의 출력 데이터를 확인합니다.

## 문제 해결
- 플래시 지우기 또는 쓰기 오류가 발생하는 경우, 플래시 영역의 보호 설정을 확인하세요.
- UART 데이터가 출력되지 않는 경우, USART 핀 및 설정을 확인하고, BaudRate가 맞는지 확인하세요.
