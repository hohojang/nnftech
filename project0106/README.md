# 기존 동작 코드와 개선 코드의 차이점 


# 기능 설명
## 1. SSR 제어 (SSR_Control)
PIR 센서 감지 시 SSR을 ON (조명 ON)

PIR 센서 미감지 상태에서 4초 후 SSR을 OFF (조명 OFF)

## 2. 오류 감지 (Error_Detection)

특정 오류 조건을 감지하고 7초 후 오류 메시지 전송

오류 조건:

Error 1: PIR 감지 + 밝음 + SSR OFF

Error 2: PIR 미감지 + 어두움 + SSR ON

## 3. 상태 전송 (Send_EEAM_Status)

현재 상태를 UART 및 LoRa를 통해 전송



# 📋 Decision Table
1. SSR 제어(SSR_Control)
   
| 조건       | PIR 상태 |  SSR 상태  | 타이머    | 결과       |
|------------|----------|-----------|-----------|-----------|
| Rule 1     | 감지(1)  |OFF(RESET) |     -     | SSR ON    |
| Rule 2     | 미감지(0)|  밝음(>)  | 0(초기화)  | 타이머 시작|
| Rule 3     | 미감지(0)| 어두움(≤) | 4초 경과   | SSR OFF   |

2. 오류 감지 (Error_Detection)

| 조건        | PIR 상태 |  CDS 상태  | SSR 상태       | 타이머     |   결과   |
|------------ |----------|-----------|--------------|-----------|--------------------------------|
| Error 1     | 감지(1)  |밝음 (0)    |  OFF (RESET) | 7초 경과    |오류: PIR 감지 + 밝음 + SSR OFF|
| Error 2     | 미감지(0)|어두움 (1)  | ON (SET)      | 7초 경과   |오류: PIR 미감지 + 어두움 + SSR ON|
