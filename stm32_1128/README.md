############################STM32 펌웨어 기초 09-UART#3[DMA]#########################


## Vimeo Video

[](https://img.youtube.com/vi/1034121444/0.jpg)](https://vimeo.com/1034121444)

##프로젝트 개요

DMA(Direct Memory Access)를 이용한 UART 송수신

송신과 수신은 DMA(Direct Memory Access)를 활용하여 비차단적으로 처리 

송신 데이터는 문자열로 출력되며, 수신된 데이터는 64바이트 버퍼에 저장되고, 수신 완료 후 송신

##주요 기능

UART 송신: 문자열을 DMA로 UART 송신.

UART 수신: UART로부터 데이터를 비차단적으로 수신하고, 수신 완료 후 해당 데이터를 다시 송신.

DMA 사용: UART 송수신을 DMA로 처리하여 CPU 부하를 줄이고 효율적인 데이터 전송.



##사용된 주요 함수

        HAL_Init(): HAL 라이브러리 초기화.
        SystemClock_Config(): 시스템 클럭 설정 (HSI 오실레이터 및 PLL 사용).
        MX_GPIO_Init(): GPIO 초기화 (B1 핀 입력, LD4 핀 출력).
        MX_DMA_Init(): DMA 초기화 (USART2 송수신 DMA 설정).
        MX_USART2_UART_Init(): USART2 UART 초기화 (9600 보드레이트, 8비트 데이터, 1스톱 비트).
        HAL_UART_RxCpltCallback(): UART 수신 완료 콜백 함수 (수신된 데이터를 다시 송신).
        Error_Handler(): 오류 발생 시 호출, 무한 루프에 빠짐.
        


초기화:

시스템 및 주변 장치 초기화

(HAL_Init(), SystemClock_Config(), MX_GPIO_Init(), MX_DMA_Init(), MX_USART2_UART_Init()).
송신:

HAL_UART_Transmit_DMA()를 통해 문자열을 UART로 송신.
수신:

HAL_UART_Receive_DMA()를 통해 UART로부터 데이터를 비차단적으로 수신 시작.
수신 처리:

수신 완료 후 HAL_UART_RxCpltCallback() 콜백 함수가 호출되며, 수신된 데이터를 다시 송신.




DMA를 사용함에 있어서 좋은점:

UART 송수신을 DMA로 처리하여 CPU 부하를 줄이고 데이터 처리 성능 향상.


콜백 함수:
수신 완료 후 자동으로 처리하도록 콜백 함수를 사용하여 실시간 데이터 처리.


동작 흐름

초기화: 시스템이 초기화되고, 송신 데이터(문자열)가 UART로 송신

수신 대기: UART에서 DMA를 통해 수신이 시작

수신 처리: 수신이 완료되면 HAL_UART_RxCpltCallback() 함수가 호출되어 데이터를 다시 송신

