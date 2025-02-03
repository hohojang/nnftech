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

# 송신부 
PIR 센서 감지 시 LED ON 및 LoRa 메시지 전송

PIR 감지 종료 후 5초 후 LED OFF 및 상태 메시지 전송

CDS 센서 값이 너무 크거나 작은 경우 에러 메시지 전송

# 수신부 
DMA 기반으로 UART4를 통해 LoRa 메시지 수신

수신된 메시지를 분석하여 에러 및 상태 출력

설정 확인
1. LoRa 모듈 설정 (송신부/수신부 동일하게 설정)
```c
// 송신부와 수신부의 LoRa 모듈 설정
SendATCommand("AT+ADDR=0x04");    // 주소 설정
SendATCommand("AT+CH=0x02");      // 채널 설정
SendATCommand("AT+BAUD=9600");    // UART 속도 설정
SendATCommand("AT+PARAMETER=4,7,1,1");  // 전송 파라미터 설정
```
