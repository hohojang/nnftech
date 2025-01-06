# 기존 동작 코드와 개선 코드의 차이점 

-------

## 함수 분리 모듈화:

개선코드 1: SSR_Control, Error_Detection으로 SSR 제어와 오류 감지가 독립된 함수로 분리되어 유지보수와 재사용성이 뛰어남

기존코드 2: 모든 로직이 하나의 함수 내에 포함되어 코드가 복잡함

--------

## 오류 타이머 관리:

개선코드 1: error_timer_1, error_timer_2로 각각의 오류 조건에 대한 타이머를 독립적으로 관리

기존코드 2: 하나의 타이머로 모든 오류 상태를 처리하려 해 명확성이 떨어짐

----------
## 가독성 및 유지보수성:

개선코드 1: 각 기능이 독립적으로 분리되어 명확하고 이해하기 쉬움

기존코드 2: 조건문이 길고 중첩되어 가독성이 낮

## 기타 수정 사항:

개선코드 1:
1.1 CDS 센서 값은 오류 조건에서만 사용
1.2 SSR ON/OFF 판단은 PIR 감지 상태만으로 제어
1.3 불필요한 센서 의존성 제거로 제어 로직이 단순화됨

기존코드 2:
2.1 CDS 센서 값이 SSR 제어 및 오류 감지 로직에 모두 사용됨
2.2 로직 복잡성 증가 및 오작동 가능성 상승
2.3 PIR과 CDS가 상호 의존적이라 로직이 불명확함


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
    if (pir_status && ssr_state == GPIO_PIN_RESET) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
        UART_SendString("[INFO] Light ON (PIR Detected).\r\n");
        *light_timer = 0;
    }
    else if (!pir_status && ssr_state == GPIO_PIN_SET) {
        if (*light_timer == 0) *light_timer = HAL_GetTick();
        else if ((HAL_GetTick() - *light_timer) >= LIGHT_OFF_DELAY) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
            UART_SendString("[INFO] Light OFF (No PIR Detected for 4s).\r\n");
            *light_timer = 0;
        }
    }
}
/**
 * @brief 오류 감지 및 7초 후 메시지 전송
 * @param condition 오류 조건 (1 = 조건 만족)
 * @param timer 오류 타이머 변수
 * @param message 오류 메시지
 */
typedef struct {
    uint32_t error_timer_1;
    uint32_t error_timer_2;
} ErrorTimer;

ErrorTimer error_timer = {0, 0};

/**
 * @brief 오류 감지 및 메시지 전송
 */
void Custom_Error_Handler(bool condition, uint32_t *timer, const char *message) {
    if (condition) {
        if (*timer == 0) *timer = HAL_GetTick();
        else if ((HAL_GetTick() - *timer) >= 7000) {
            Error_Alert(message);
            LoRa_SendData(message);
            *timer = 0;
        }
    } else {
        *timer = 0;
    }
}
/**
 * @brief 조명 제어 및 타이머 처리
 */
void Control_Light(SensorData *data) {
    static uint32_t light_timer = 0;
    static uint32_t error_timer_1 = 0;
    static uint32_t error_timer_2 = 0;

    GPIO_PinState ssr_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
    uint8_t pir_status = (data->pir_state == GPIO_PIN_SET) ? 1 : 0;
    uint8_t cds_dark = (data->cds_analog_value <= CDS_LIGHT_THRESHOLD) ? 1 : 0;

    /** SSR 제어 **/
    SSR_Control(pir_status, ssr_state, &light_timer);

    /** 오류 감지 **/
    Custom_Error_Handler(pir_status && !cds_dark && ssr_state == GPIO_PIN_RESET, &error_timer_1,
                   "[ERROR] PIR Detected, but Light is OFF (Bright Environment).");
    Custom_Error_Handler(!pir_status && cds_dark && ssr_state == GPIO_PIN_SET, &error_timer_2,
                   "[ERROR] No PIR, but Light is ON (Dark Environment).");

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





+-------------------------+      PIR Detected      +-------------------------+
|      SSR OFF            | --------------------> |      SSR ON             |
| (No PIR, Light OFF)     |                       | (PIR Detected, Light ON)|
+-------------------------+                       +-------------------------+
           ^                                           |
           | No PIR + Timer (4s elapsed)               |
           |                                           v
+-------------------------+      Timer Reset     +-------------------------+
|    Timer Started        | <------------------ |   PIR Not Detected      |
| (No PIR, Light ON)      |                       | Timer Running          |
+-------------------------+                       +-------------------------+

