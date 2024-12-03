STM32L4 FLASH Memory Example
개요
이 프로젝트는 STM32L4 마이크로컨트롤러에서 내장 FLASH 메모리를 지우고, 쓰고, 읽는 기능을 구현한 예제입니다. 이 코드는 USART를 통해 결과를 출력하며, 주기적으로 FLASH 메모리 작업을 수행합니다.
기능
FLASH 메모리 지우기
FLASH 메모리에 데이터 쓰기
FLASH 메모리에서 데이터 읽기
USART를 통한 결과 출력
하드웨어 요구 사항
STM32 NUCLEO-L452RE 보드
USB to UART 변환기 (USART 출력 확인을 위한)
소프트웨어 요구 사항
STM32CubeIDE
STM32 HAL 라이브러리
코드 설명
main.c: FLASH 메모리 작업을 수행하는 주요 코드입니다.
flash_erase_write_read(): FLASH 메모리를 지우고, 데이터를 쓰고, 읽는 함수입니다.
SystemClock_Config(): 시스템 클럭을 설정하는 함수입니다.
MX_USART2_UART_Init() 및 MX_USART3_UART_Init(): USART 초기화 함수입니다.
MX_GPIO_Init(): GPIO 초기화 함수입니다.
Error_Handler(): 오류 발생 시 호출되는 함수입니다.
사용 방법
STM32CubeIDE에서 프로젝트를 열고, 코드를 빌드합니다.
STM32 NUCLEO-L452RE 보드에 코드를 업로드합니다.
USB to UART 변환기를 사용하여 USART2 또는 USART3의 TX 핀을 PC에 연결합니다.
4. 터미널 프로그램(예: PuTTY, Tera Term)을 사용하여 115200 baud rate로 연결합니다.
5. 보드를 리셋하면 FLASH 메모리 작업 결과가 터미널에 출력됩니다.
현재 문제 상황
현재 코드에서 발생하는 문제는 다음과 같습니다:
1. 'FLASH_SECTOR_4' undeclared:
FLASH_SECTOR_4가 정의되지 않았습니다. STM32L4의 데이터 시트를 참조하여 사용 가능한 섹터 번호를 확인하고, 올바른 섹터 번호로 수정해야 합니다.
2. 'FLASH_TYPEPROGRAM_WORD' undeclared:
FLASH_TYPEPROGRAM_WORD가 정의되지 않았습니다. STM32L4의 HAL 라이브러리에서 이 상수가 포함된 헤더 파일이 올바르게 포함되었는지 확인하고, 필요에 따라 FLASH_TYPEPROGRAM_DOUBLEWORD로 변경해야 합니다.
해결 방안
STM32L4 데이터 시트를 참조하여 사용 가능한 FLASH 섹터 번호를 확인하고, 코드에서 적절한 섹터 번호로 수정합니다.
HAL 라이브러리의 버전을 확인하고, 필요한 상수가 정의되어 있는지 확인합니다. 만약 FLASH_TYPEPROGRAM_WORD가 정의되지 않았다면, FLASH_TYPEPROGRAM_DOUBLEWORD를 사용하도록 수정합니다.
참고 자료
STM32L4 Series Datasheet
STM32L4 Reference Manual
이 README.md 파일은 STM32L4 FLASH 메모리 예제 프로젝트에 대한 개요와 사용 방법, 현재 문제 상황 및 해결 방안을 제공합니다.
