# 하드웨어 구성
보드 및 주요 연결

MCU: STM32L452RET6 (LQFP64 패키지)

LoRa 모듈: EBYTE E22 (UART 통신)

PIR 센서, CDS 센서, 내장 LED

UART4 사용: 송신부와 수신부 간의 LoRa 모듈 연결

GPIO 핀 배치:

PIR 센서: PC6 (Input, Pull-Up)

CDS 센서: PB0 (ADC1_IN15, Analog Mode)

내장 LED: PA8 (Output)

LoRa M0: PB10 (Output, Normal Mode 설정 시 LOW)

LoRa M1: PB11 (Output, Normal Mode 설정 시 LOW)

