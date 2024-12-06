# STM32 펌웨어 기초 16 | 17 -(CLI #1 , #2) 

# 유튜브 강의 : https://www.youtube.com/watch?v=ihtNouEHwgA 

# 결과 동영상 https://vimeo.com/1036664890?share=copy

터미널에서 명령어를 입력하여 STM32와 상호작용 / USART2를 사용하여 명령어를 수신하고 처리

# 문제 상황에 대한 해결 

터미널에 문자가 출력이 안되는 상황 -> CUBE MX에서 사용하는 UART를 글로벌 인터럽트로 변경 후 해결

특정 명령어를 입력 시 출력 내용이 앞에 한 글자만 출력되는 상황 ->  인터럽트 콜백 함수에서 해당 명령어가 들어오고 엔터 키를 눌러야 동작하도록 변경 후 해결 완료 

# 기능 및 주요 함수 

CLI 명령어:

help: 사용 가능한 명령어 목록을 출력

hello: "world"를 출력

reset: 버튼을 눌러 카운트를 리셋

HAL_UART_RxCpltCallback()
설명: UART 수신 완료 인터럽트가 발생했을 때 호출되는 콜백 함수
주요 동작:
수신된 데이터가 엔터 키(\r 또는 \n)인 경우, 명령어 종료 후 ProcessCLICommand() 함수를 호출하여 명령어를 처리
백스페이스 문자가 수신된 경우, 입력 버퍼에서 이전 문자를 삭제하고 백스페이스 처리 문자를 UART로 전송
그 외의 경우, 수신된 문자를 input_buffer에 저장하고 계속해서 수신 대기

ProcessCLICommand()
설명: 입력된 CLI 명령어를 처리하는 함수입니다.
주요 동작:
help 명령어가 입력되면 사용 가능한 명령어 목록을 출력
hello 명령어가 입력되면 "world"를 출력
reset 명령어가 입력되면 reset_count를 0으로 초기화하고 버튼 리셋 메시지를 출력

UART_SendString()
설명: UART를 통해 문자열을 전송하는 함수
주요 동작:
주어진 문자열의 각 문자를 순차적으로 UART로 전송
문자열의 끝에 도달할 때까지 한 문자씩 전송

# 하드웨어 설정

USART2:

TX: PA2

RX: PA3
B1 버튼: 하드웨어 인터럽트를 사용하여 버튼 누름을 감지

# UART 인터럽트 설정:

UART 수신이 완료되면 HAL_UART_RxCpltCallback 함수가 호출

입력된 문자는 input_buffer에 저장되고, 엔터 키가 눌리면 ProcessCLICommand 함수에서 명령어가 처리

