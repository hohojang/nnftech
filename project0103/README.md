# **디시전 테이블 기법(Decision Table)**

디시전 테이블은 복잡한 **조건(Condition)**과 **동작(Action)**의 조합을 표 형식으로 표현하여 논리적으로 시스템의 동작을 분석하고 설계하는 기법

주로 조건이 많고 중복되기 쉬운 상황에서 사용되며, 모든 가능한 조건과 결과를 체계적으로 검토 가능 

## 
-------------
## if문 개선 코드 

# **주요 개선점 ** 

PIR 상태 우선 처리: PIR 감지 여부에 따라 코드가 먼저 실행

불필요한 중첩 제거: 중복된 조건과 중첩된 if-else 제거 

타이머 관리 일원화: 타이머가 하나의 논리적 흐름에서만 관리

```c 
/**
 * @brief 조명 제어 및 타이머 처리 (PIR 상태 및 타이머 중심)
 */
void Control_Light(void) {
    static uint32_t light_timer = 0; // 타이머 변수
    GPIO_PinState light_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10); // 현재 조명 상태
    
    // 동작 감지 or 미감지

    // PIR 감지 상태 
    if (pir_state == GPIO_PIN_SET) {
        if (light_state == GPIO_PIN_RESET && cds_analog_value <= CDS_LIGHT_THRESHOLD) {
            // 어두움 && PIR 감지 && 조명 OFF → 조명 ON
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
            UART_SendString("[INFO] Light ON (Dark + PIR Detected).\r\n");
        }
        // PIR 감지 시 타이머 초기화
        light_timer = 0;
    }
    // PIR 미감지 상태
    else if (pir_state == GPIO_PIN_RESET) {
        if (light_state == GPIO_PIN_SET) {
            if (light_timer == 0) {
                light_timer = HAL_GetTick(); // 타이머 시작
            } 
            else if ((HAL_GetTick() - light_timer) >= LIGHT_OFF_DELAY) {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); // 조명 OFF
                UART_SendString("[INFO] Light OFF (No PIR for 4s).\r\n");
                light_timer = 0; // 타이머 초기화
            }
        } 
        else {
            light_timer = 0; // 조명이 꺼져 있을 경우 타이머 초기화
        }

        // 밝은 환경에서 조명이 켜져 있을 경우 오류 감지
        if (cds_analog_value > CDS_LIGHT_THRESHOLD && light_state == GPIO_PIN_SET) {
            Error_Alert("No PIR, but Light is ON despite Bright Environment.");
        }
    }

    // 공통 오류 감지
    if (pir_state == GPIO_PIN_SET && light_state == GPIO_PIN_RESET) {
        Error_Alert("PIR Detected, but Light is OFF.");
    }

    // 상태 전송
    Send_EEAM_Status();
}
```
---------

# **디시전 테이블의 구성 요소**
## 1.조건(Condition)
시스템이 판단해야 할 입력 값이나 상태
## 2.동작(Action)
조건에 따라 수행해야 할 시스템의 동작
## 3.규칙(Rule)
특정 조건의 조합에 따라 수행해야 할 동작을 정의
## 4.결과(Result)
조건과 동작이 결합되어 나온 최종 상태
--------



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
