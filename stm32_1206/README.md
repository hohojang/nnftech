# STM32 펌웨어 기초 16 | 17 -(CLI #1 , #2) 

https://vimeo.com/1036664890?share=copy

터미널에서 명령어를 입력하여 STM32와 상호작용 / USART2를 사용하여 명령어를 수신하고 처리

기능
CLI 명령어:

help: 사용 가능한 명령어 목록을 출력

hello: "world"를 출력

reset: 버튼을 눌러 카운트를 리셋

하드웨어 설정

USART2:

TX: PA2

RX: PA3
B1 버튼: 하드웨어 인터럽트를 사용하여 버튼 누름을 감지

UART 인터럽트 설정:

UART 수신이 완료되면 HAL_UART_RxCpltCallback 함수가 호출

입력된 문자는 input_buffer에 저장되고, 엔터 키가 눌리면 ProcessCLICommand 함수에서 명령어가 처리

