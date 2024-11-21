#############2024-11-21######################
STM32 펌웨어 기초 03-USB CDS 

이 프로젝트는 STM32 마이크로컨트롤러를 사용하여 USB CDC(Communications Device Class) 기능을 구현해보기

USB를 통해 PC와 통신하며, 주기적으로 문자열을 전송(아직 미완성입니다/STM32 CUbe IDE에 시리얼모니터 설정 문제)

순서. 
사용 보드에 맞춰서 cubemx에서 설정

usb 메뉴에 Device(FS) 체크 / PA12 , PA11 에 USB 핀이 설정된다.

cubemx에서 Middleware > USB_DEVICE 메뉴에서 class For FS IP를 communication Device Class(Virtual Port Com)을 선택

PA1핀도 Output으로 설정 후 GENERATE CODE

주요 기능
USB CDC 초기화: USB 장치 초기화

문자열 전송: 내가 지정한 단어 문자열을 2초 간격으로 전송.

시스템 클록 설정: 마이크로컨트롤러의 클록을 설정하여 안정적인 동작 보장.

함수 공부 

HAL_Init(): HAL 라이브러리를 초기화
SystemClock_Config(): 시스템 클록을 설정
MX_GPIO_Init(): GPIO를 초기화
MX_USB_DEVICE_Init(): USB 장치를 초기화
CDC_Transmit_FS(): USB를 통해 문자열을 전송
