# 기존 동작 코드와 개선 코드의 차이점 

-------

## 함수 분리:

개선코드 1: SSR_Control, Error_Detection으로 SSR 제어와 오류 감지가 독립된 함수로 분리되어 유지보수와 재사용성이 뛰어남.

기존코드 2: 모든 로직이 하나의 함수 내에 포함되어 코드가 복잡함.

--------

## 오류 타이머 관리:

개선코드 1: error_timer_1, error_timer_2로 각각의 오류 조건에 대한 타이머를 독립적으로 관리.

기존코드 2: 하나의 타이머로 모든 오류 상태를 처리하려 해 명확성이 떨어짐.

----------
## 가독성 및 유지보수성:

개선코드 1: 각 기능이 독립적으로 분리되어 명확하고 이해하기 쉬움.

기존코드 2: 조건문이 길고 중첩되어 가독성이 낮음.

## 기타 수정 사항:

CDS 조도센서를 오류 조건에만 사용.

----------

1. 개선 코드
```C
/**
 * @brief SSR 제어 및 타이머 처리
 * @param pir_status PIR 상태 (1 = 감지, 0 = 미감지)
 * @param ssr_state SSR 상태 (ON/OFF)
 * @param light_timer 타이머 변수
 */
void SSR_Control(uint8_t pir_status, GPIO_PinState ssr_state, uint32_t *light_timer) {
    if (pir_status) {
        // Rule 1: PIR 감지 → SSR ON
        if (ssr_state == GPIO_PIN_RESET) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
            UART_SendString("[INFO] Light ON (PIR Detected).\r\n");
        }
        *light_timer = 0; // 타이머 초기화
    }
    else if (ssr_state == GPIO_PIN_SET) {
        // Rule 2: PIR 미감지 → SSR OFF (4초 딜레이)
        if (*light_timer == 0) {
            *light_timer = HAL_GetTick();
        }
        else if ((HAL_GetTick() - *light_timer) >= LIGHT_OFF_DELAY) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
            UART_SendString("[INFO] Light OFF (No PIR Detected for 4s).\r\n");
            *light_timer = 0;
        }
    }
}
/**
 * @brief 오류 감지 및 7초 후 메시지 전송
 * @param pir_status PIR 상태 (1 = 감지, 0 = 미감지)
 * @param cds_dark CDS 상태 (1 = 어두움, 0 = 밝음)
 * @param ssr_state SSR 상태 (ON/OFF)
 */
void Error_Detection(uint8_t pir_status, uint8_t cds_dark, GPIO_PinState ssr_state) {
	//오류 조건 독립성을 위한 에러 타이머 분리
    static uint32_t error_timer_1 = 0; // Error 1 타이머
    static uint32_t error_timer_2 = 0; // Error 2 타이머

    // **Error 1:** PIR 감지 + 밝음 + SSR OFF → 오류
    if (pir_status && !cds_dark && ssr_state == GPIO_PIN_RESET) {
        if (error_timer_1 == 0) {
            error_timer_1 = HAL_GetTick(); // Error 1 타이머 시작
        }
        else if ((HAL_GetTick() - error_timer_1) >= 7000) { // 7초 경과 후 오류 메시지
            Error_Alert("PIR Detected, but Light is OFF (Bright Environment).");
            LoRa_SendData("[ERROR] PIR Detected, but Light is OFF (Bright Environment).");
            error_timer_1 = 0; // Error 1 타이머 초기화
        }
    }
    else {
        error_timer_1 = 0; // 오류 상태 해제 시 타이머 초기화
    }
    // **Error 2:** PIR 미감지 + 어두움 + SSR ON → 오류
    if (!pir_status && cds_dark && ssr_state == GPIO_PIN_SET) {
        if (error_timer_2 == 0) {
            error_timer_2 = HAL_GetTick(); // Error 2 타이머 시작
        }
        else if ((HAL_GetTick() - error_timer_2) >= 7000) { // 7초 경과 후 오류 메시지
            Error_Alert("No PIR, but Light is ON (Dark Environment).");
            LoRa_SendData("[ERROR] No PIR, but Light is ON (Dark Environment).");
            error_timer_2 = 0; // Error 2 타이머 초기화
        }
    }
    else {
        error_timer_2 = 0; // 오류 상태 해제 시 타이머 초기화
    }
}
/**
 * @brief 조명 제어 및 타이머 처리 (오류 메시지 딜레이 포함)
 */
void Control_Light(void) {
    static uint32_t light_timer = 0; // SSR 타이머 변수
    GPIO_PinState ssr_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10); // SSR 상태 확인

    // 상태 변수
    uint8_t pir_status = (pir_state == GPIO_PIN_SET) ? 1 : 0; // PIR 상태 (1 = 감지, 0 = 미감지)
    uint8_t cds_dark = (cds_analog_value <= CDS_LIGHT_THRESHOLD) ? 1 : 0; // CDS 상태 (1 = 어두움, 0 = 밝음)

    /** SSR 제어 **/
    SSR_Control(pir_status, ssr_state, &light_timer);

    /** 오류 감지 (7초 후 메시지 전송) **/
    Error_Detection(pir_status, cds_dark, ssr_state);

    /** 상태 전송 **/
    Send_EEAM_Status();
}
```
2. 기존 코드
```C
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
