################stm32펌웨어 기초 10/11 UART#4(USB to Serial) UART#5(USB통신 개선)###############

이 프로젝트는 STM32 마이크로컨트롤러를 사용하여 USB to Serial 기능과 USB 통신 개선을 구현 

이 프로젝트는 USB CDC (Communication Device Class)와 UART (Universal Asynchronous Receiver Transmitter)를 사용하여 USB 장치와 UART 간의 데이터 송수신을 처리

주요 함수 정리

- `main()`: 프로그램의 진입점으로, 초기화 및 메인 루프를 처리
  
- `HAL_UART_RxCpltCallback()`: UART 수신 인터럽트가 발생할 때 호출 / 수신된 데이터를 USB 및 다른 UART로 전송
  
- `USB_Data_Receive()`: USB를 통해 수신된 데이터를 처리
  
- `MX_USB_DEVICE_Init()`: USB 장치를 초기화
  
- `MX_USART2_UART_Init()` 및 `MX_USART3_UART_Init()`: 각각의 UART를 초기화
  
- `MX_GPIO_Init()`: GPIO 핀을 초기화

프로젝트 구성

1. 기능
   
USB to Serial 통신:

USB CDC 인터페이스를 통해 USB 호스트와 데이터를 주고받음

UART2와 UART3 간의 데이터를 실시간으로 전송

USB 데이터 수신 처리:

USB로 수신된 데이터를 UART로 전송하고, 그 반대로도 데이터를 처리

2. 주요 사용 기술
   
STM32 HAL 라이브러리: STM32 개발을 위한 하드웨어 추상화 레벨 라이브러리

USB CDC Class: USB 장치와의 통신을 위한 표준 클래스

USART: UART 인터페이스를 통한 직렬 통신

3. 하드웨어 요구사항
   
STM32 NUCLEO-L452RE 보드

USB 포트를 통한 USB CDC 통신

두 개의 UART 포트 (USART2 및 USART3)

4. 소프트웨어 구성
    
STM32CubeMX를 이용해 프로젝트를 생성하고, STM32CubeIDE에서 개발을 진행

USB CDC 인터페이스를 통해 USB 장치와의 통신을 처리하며, 두 개의 UART 포트를 사용하여 USB와 직렬 통신 간 데이터 송수신을 수행

5. 작동 방식

데이터 송수신:

USART2와 USART3는 각각 다른 UART 포트로 설정

데이터가 USART2나 USART3로 수신되면, 이를 USB CDC 인터페이스를 통해 USB로 전송하고, 반대로 USB로 수신된 데이터는 USART2와 USART3로 전달

인터럽트 방식:

HAL_UART_RxCpltCallback 함수는 데이터 수신 시마다 호출되어 데이터를 처리

6. 사용 방법
    
프로그램 실행:

STM32 개발 보드와 PC를 연결한 후, 프로그램을 실행

USB 포트를 통해 STM32 보드가 연결

시리얼 통신:

USB CDC를 통해 PC와 STM32 보드 간 시리얼 통신

터미널 프로그램을 사용하여 UART 포트를 설정하고 통신을 시작


