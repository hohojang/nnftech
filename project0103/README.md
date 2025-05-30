# **디시전 테이블 기법(Decision Table)**

디시전 테이블은 복잡한 **조건(Condition)**과 **동작(Action)**의 조합을 표 형식으로 표현하여 논리적으로 시스템의 동작을 분석하고 설계하는 기법

주로 조건이 많고 중복되기 쉬운 상황에서 사용되며, 모든 가능한 조건과 결과를 체계적으로 검토 가능 
## 
-------------
## if문 개선 코드 

## 주요 개선점

1. PIR 상태 우선 처리: PIR 감지 여부에 따라 코드가 먼저 실행

2. 불필요한 중첩 제거: 중복된 조건과 중첩된 if-else 제거 

3. 타이머 관리 일원화: 타이머가 하나의 논리적 흐름에서만 관리

```c 
/**
 * @brief 조명 제어 및 타이머 처리 (PIR 상태 및 타이머 중심, 최적화)
 */
void Control_Light(void) {
    static uint32_t light_timer = 0; // 타이머 변수
    GPIO_PinState light_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10); // 현재 조명 상태

    if (pir_state == GPIO_PIN_SET) {
        //PIR 감지 상태
        if (light_state == GPIO_PIN_RESET && cds_analog_value <= CDS_LIGHT_THRESHOLD) {
            // 어두움 && PIR 감지 && 조명 OFF → 조명 ON
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
            UART_SendString("[INFO] Light ON (Dark + PIR Detected).\r\n");
        }
        light_timer = 0; // 타이머 초기화
    } else {
        //PIR 미감지 상태
        if (light_state == GPIO_PIN_SET) {
            // 타이머 시작 또는 4초 경과 시 조명 OFF
            if (light_timer == 0) {
                light_timer = HAL_GetTick(); // 타이머 시작
            } else if ((HAL_GetTick() - light_timer) >= LIGHT_OFF_DELAY) {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); // 조명 OFF
                UART_SendString("[INFO] Light OFF (No PIR for 4s).\r\n");
                light_timer = 0; // 타이머 초기화
            }
        } else {
            // 조명이 꺼져 있을 때 타이머 리셋
            light_timer = 0;
        }

        // 오류 감지
        if (cds_analog_value > CDS_LIGHT_THRESHOLD && light_state == GPIO_PIN_SET) {
            Error_Alert("No PIR, but Light is ON despite Bright Environment.");
        }
        if (cds_analog_value <= CDS_LIGHT_THRESHOLD && light_state == GPIO_PIN_RESET) {
            Error_Alert("PIR Detected, but Light is OFF.");
        }
    }

    // 상태 전송
    Send_EEAM_Status();
}
```
---------

# **디시전 테이블에 기반하여 Control_Light 함수 수정** 


# 📋 Decision Table
| 조건       | PIR 상태 | CDS 값     | 동작       | 설명                 |
|------------|----------|-----------|-----------|---------------------- |
| Rule 1     | 감지(O)  | 어두움(≤) | 조명 ON   | PIR 감지 + 어두움       |
| Rule 2     | 감지(O)  | 밝음(>)   | 오류 알림 | PIR 감지 + 밝음         |
| Rule 3     | 미감지(X)| 어두움(≤) | 타이머 ON | PIR 미감지 + 어두움      |
| Rule 4     | 미감지(X)| 어두움(≤) | 조명 OFF  | 4초 후 조명 OFF         |
| Rule 5     | 미감지(X)| 밝음(>)   | 오류 알림 | PIR 미감지 + 밝음       |
| Rule 6     | 미감지(X)| -         | 타이머 OFF| 조명 꺼짐, 타이머 리셋  |

# 가상 RTU (Virtual RTU)란?
**RTU (Remote Terminal Unit)**는 원격 장치 제어 및 모니터링을 수행하는 장치로,

주로 산업 자동화, 전력 시스템, 스마트 그리드, 원격 측정 등에서 사용

**가상 RTU (Virtual RTU)**는 하드웨어 RTU를 소프트웨어 환경에서 **에뮬레이션(Emulation)**하거나 **시뮬레이션(Simulation)**한 시스템

실제 RTU 장비 없이 네트워크 프로토콜과 통신 명령어를 테스트하고, 데이터를 수집하거나 제어 명령을 전달하는 환경을 제공

# 진행해야 할 사항 
1. 900메가 E22900T22S LoRa 제어 커맨더 셋 

2. E22900T22S이 장착되어있는 보드를 방수등과 연결 

3. 가상RTU를 통해 데이터를 받고 처리 
