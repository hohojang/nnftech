# STM32 펌웨어 기초 16 | 17 -(CLI #1 , #2) 

# 유튜브 강의 : https://www.youtube.com/watch?v=ihtNouEHwgA 

# 결과 동영상 https://vimeo.com/1036664890?share=copy

터미널에서 명령어를 입력하여 STM32와 상호작용 / USART2를 사용하여 명령어를 수신하고 처리

# 문제 상황에 대한 해결 

터미널에 문자가 출력이 안되는 상황 -> CUBE MX에서 사용하는 UART를 글로벌 인터럽트로 변경 후 해결

특정 명령어를 입력 시 출력 내용이 앞에 한 글자만 출력되는 상황 ->  인터럽트 콜백 함수에서 해당 명령어가 들어오고 엔터 키를 눌러야 동작하도록 변경 후 해결 완료 

# 기능

CLI 명령어:

help: 사용 가능한 명령어 목록을 출력

hello: "world"를 출력

reset: 버튼을 눌러 카운트를 리셋

# 하드웨어 설정

USART2:

TX: PA2

RX: PA3
B1 버튼: 하드웨어 인터럽트를 사용하여 버튼 누름을 감지

# UART 인터럽트 설정:

UART 수신이 완료되면 HAL_UART_RxCpltCallback 함수가 호출

입력된 문자는 input_buffer에 저장되고, 엔터 키가 눌리면 ProcessCLICommand 함수에서 명령어가 처리

