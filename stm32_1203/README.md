################STM32 펌웨어 기초 12/13 - (FLASH #1,#2)###############

STM32L4 마이크로컨트롤러에서 내장 FLASH 메모리를 지우고, 쓰고, 읽는 기능을 구현

https://www.youtube.com/watch?v=jEAyh8Q_JFg [강의 url]

이 코드는 USART를 통해 결과를 출력, 주기적으로 FLASH 메모리 작업을 수행

[기능]

FLASH 메모리 지우기

FLASH 메모리에 데이터 쓰기

FLASH 메모리에서 데이터 읽기

USART를 통한 결과 출력


[하드웨어 요구 사항]

STM32 NUCLEO-L452RE 보드

USB to UART 변환기 (USART 출력 확인을 위한)


[메인 코드 위치 및 주요 함수]

main.c: FLASH 메모리 작업을 수행하는 주요 코드

flash_erase_write_read(): FLASH 메모리를 지우고, 데이터를 쓰고, 읽는 함수

SystemClock_Config(): 시스템 클럭을 설정하는 함수

MX_USART2_UART_Init() 및 MX_USART3_UART_Init(): USART 초기화 함수

MX_GPIO_Init(): GPIO 초기화 함수

Error_Handler(): 오류 발생 시 호출되는 함수

[현재 문제 상황]

현재 코드에서 발생하는 문제

1. 'FLASH_SECTOR_4' undeclared:
FLASH_SECTOR_4가 정의x STM32L4의 데이터 시트를 참조하여 사용 가능한 섹터 번호를 확인하고, 올바른 섹터 번호로 수정해야 한다.


2. 'FLASH_TYPEPROGRAM_WORD' undeclared:
FLASH_TYPEPROGRAM_WORD가 정의x STM32L4의 HAL 라이브러리에서 이 상수가 포함된 헤더 파일이 올바르게 포함되었는지 확인하고, 필요에 따라 FLASH_TYPEPROGRAM_DOUBLEWORD로 변경


내일 할일 - 해결방안

STM32L4 데이터 시트를 참조하여 사용 가능한 FLASH 섹터 번호를 확인하고, 코드에서 적절한 섹터 번호로 수정해야함.

HAL 라이브러리의 버전을 확인하고, 필요한 상수가 정의되어 있는지 확인

만약 FLASH_TYPEPROGRAM_WORD가 정의되지 않았다면, FLASH_TYPEPROGRAM_DOUBLEWORD를 사용하도록 수정

