# 📋 Decision Table
1. SSR 제어(SSR_Control)
   
| 조건       | PIR 상태 |  SSR 상태  | 타이머    | 동작      |       설명          |
|------------|----------|-----------|-----------|----------|----------------------|
| PIR감지    | 감지(1)   |OFF      |     -      | SSR ON    |PIR 감지 시 즉시 SSR ON|
| PIR미감지   | 미감지(0) |  ON    | 타이머 시작 | 타이머 시작|PIR 미감지 시 타이머 시작|
| 타이머 경과  | 미감지(0)| ON     | 4초 경과    | SSR OFF   |타이머 경과 후 SSR OFF |

2. 오류 감지 (Error_Detection)

| 조건        | PIR 상태 |  CDS 상태  | SSR 상태       | 타이머     |   결과   |
|------------ |----------|-----------|--------------|-----------|--------------------------------|
| Error 1     | 감지(1)  |밝음 (0)    | OFF (RESET) | 7초 경과    |오류: PIR 감지 + 밝음 + SSR OFF|
| Error 2     | 미감지(0)|어두움 (1)  | ON (SET)      | 7초 경과   |오류: PIR 미감지 + 어두움 + SSR ON|
| 타이머 경과  |   충족   |   충족    |    충족        |ERROR_DELAY|타이머 경과 후 오류 메시지 전송|
| 조건 해제    |  불충족  |  불충족   |   불충족       |   무관     |조건 해제 시 타이머 초기화|

# 변경
## 1. 표현이 모호하여 혼란스러운 코드 직관적으로 정의  
## 2. Doxygen 주석 스타일로 가독성 향상 
```C
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Defines ------------------------------------------------------------------*/
#define LORA_CHANNEL "CH=5"
#define LORA_POWER "PWR=20"
#define CDS_LIGHT_THRESHOLD 1000
#define LIGHT_OFF_DELAY 4000
#define ERROR_DELAY 7000

#define SENSOR_ACTIVE  1  // 센서가 활성 상태 (감지됨)
#define SENSOR_INACTIVE 0 // 센서가 비활성 상태 (미감지)

#define CDS_DARK    1  // 조도 센서: 어두움
#define CDS_BRIGHT  0  // 조도 센서: 밝음

/* Private variables ---------------------------------------------------------*/
GPIO_PinState pir_state;
GPIO_PinState cds_digital_state;
uint32_t cds_analog_value;
uint32_t pir_timer = 0;

/* Sensor Data Structure -----------------------------------------------------*/
typedef struct {
    GPIO_PinState pir_state;
    uint32_t cds_analog_value;
} SensorData;

/* Function Prototypes -------------------------------------------------------*/
void SystemClock_Config(void);
void Read_Sensors(void);
void Control_Light(SensorData *data);
void LoRa_SendData(const char *data);
void Error_Alert(const char *message);
void SSR_Control(uint8_t pir_status, GPIO_PinState ssr_state, uint32_t *light_timer);
void Handle_Error(bool condition, uint32_t *error_timer, const char *message);
void Check_Errors(uint8_t pir_status, uint8_t cds_status, GPIO_PinState ssr_state,
                  uint32_t *error_timer_1, uint32_t *error_timer_2);

/**
 * @brief LoRa 데이터 전송
 */
void LoRa_SendData(const char *data) {
    HAL_UART_Transmit(&huart3, (uint8_t *)data, strlen(data), HAL_MAX_DELAY);
}

/**
 * @brief 에러 알림 (LoRa로 전송)
 * @details
 * 이 함수는 오류가 발생했을 때 LoRa 모듈을 통해 오류 메시지를 전송
 * 메시지에는 CDS 센서 값과 LED 상태가 포함
 * @param message
 *     - 유형: `const char*`
 *     - 설명: 오류 메시지 문자열.
 * @note
 * - `snprintf`를 사용하여 오류 메시지를 포맷
 * - LED 상태와 CDS 센서 값이 함께 포함
 */
void Error_Alert(const char *message) {
    char lora_buffer[100];
    snprintf(lora_buffer, sizeof(lora_buffer),
             "[RTU] ERROR: %s | CDS Analog: %lu | LED: %s",
             message,
             cds_analog_value,
             (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET) ? "ON" : "OFF");
    LoRa_SendData(lora_buffer);
}
/**
 * @brief 센서 데이터 읽기
 * @details
 * 이 함수는 PIR 센서와 CDS 센서의 상태를 읽음
 * PIR 센서는 디지털 입력을, CDS 센서는 아날로그 값을 읽어 저장
 * @note
 * - PIR 센서는 `GPIOC_PIN_6`에서 읽음
 * - CDS 센서의 디지털 값은 `GPIOC_PIN_7`에서 읽음
 * - CDS 센서의 아날로그 값은 ADC를 통해 읽음
 */
void Read_Sensors(void) {
    pir_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);
    cds_digital_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7);

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
        cds_analog_value = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);
}
/**
 * @brief SSR 제어 및 타이머 처리
 * @details
 * 이 함수는 PIR 센서 상태와 SSR 상태를 기반으로 SSR을 ON 또는 OFF로 제어
 * 또한 타이머를 사용하여 일정 시간이 경과한 경우 SSR을 OFF로 전환
 * @param pir_status
 *     - 유형: `uint8_t`
 *     - 설명: PIR 센서 상태 (`SENSOR_ACTIVE` 또는 `SENSOR_INACTIVE`)
 * @param ssr_state
 *     - 유형: `GPIO_PinState`
 *     - 설명: SSR 상태 (`GPIO_PIN_SET` 또는 `GPIO_PIN_RESET`)
 * @param light_timer
 *     - 유형: `uint32_t*`
 *     - 설명: SSR 상태 변경을 위한 타이머 변수.
 */
void SSR_Control(uint8_t pir_status, GPIO_PinState ssr_state, uint32_t *light_timer) {
    if (pir_status && ssr_state == GPIO_PIN_RESET) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
        *light_timer = 0;
    } else if (!pir_status && ssr_state == GPIO_PIN_SET) {
        if (*light_timer == 0) {
            *light_timer = HAL_GetTick();
        } else if ((HAL_GetTick() - *light_timer) >= LIGHT_OFF_DELAY) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
            *light_timer = 0;
        }
    }
}
/**
 * @brief 오류 타이머 및 메시지 전송
 * @details
 * 주어진 조건이 참이면 오류 타이머를 시작하거나, 일정 시간이 지나면 에러 알림을 전송
 * @param condition
 *     - 유형: `bool`
 *     - 설명: 오류 조건.
 * @param error_timer
 *     - 유형: `uint32_t*`
 *     - 설명: 오류 타이머 변수.
 * @param message
 *     - 유형: `const char*`
 *     - 설명: 오류 메시지.
 */
void Handle_Error(bool condition, uint32_t *error_timer, const char *message) {
    if (condition) {
        if (*error_timer == 0) {
            *error_timer = HAL_GetTick();
        } else if ((HAL_GetTick() - *error_timer) >= ERROR_DELAY) {
            Error_Alert(message);
            *error_timer = 0;
        }
    } else {
        *error_timer = 0;
    }
}


/**
 * @brief 오류 감지
 * @details
 * PIR 상태, CDS 상태, SSR 상태를 기반으로 오류를 감지
 * @param pir_status
 * @param cds_status
 * @param ssr_state
 * @param error_timer_1
 * @param error_timer_2
 */
void Check_Errors(uint8_t pir_status, uint8_t cds_status, GPIO_PinState ssr_state,
                  uint32_t *error_timer_1, uint32_t *error_timer_2) {
    Handle_Error(pir_status && (cds_status == CDS_BRIGHT) && ssr_state == GPIO_PIN_RESET,
                 error_timer_1, "[ERROR] PIR Detected, but Light is OFF");
    Handle_Error(!pir_status && (cds_status == CDS_DARK) && ssr_state == GPIO_PIN_SET,
                 error_timer_2, "[ERROR] No PIR, but Light is ON");
}


/**
 * @brief 조명 제어 및 오류 감지 처리
 * @details
 * 이 함수는 PIR 센서와 CDS 센서의 상태를 기반으로 SSR(고체 상태 릴레이)을 제어하고,
 * 센서 값에 따라 오류 조건을 감지하여 메시지를 전송
 * ### 주요 기능:
 * 1. **SSR 제어:**
 *    - PIR 센서 상태(`SENSOR_ACTIVE`, `SENSOR_INACTIVE`)와 SSR 상태를 기반으로 SSR ON/OFF를 제어
 *    - `LIGHT_OFF_DELAY`를 기준으로 SSR OFF 상태를 타이머로 관리
 * 2. **오류 감지:**
 *    - CDS 센서 값(`CDS_DARK`, `CDS_BRIGHT`)과 SSR 상태를 기반으로 오류를 감지
 *    - 오류 발생 시 `Error_Alert`를 통해 LoRa로 오류 메시지를 전송
 * ### 작동 조건:
 * - **SSR ON:**
 *   - PIR 감지(`SENSOR_ACTIVE`) + SSR OFF → SSR ON
 * - **SSR OFF:**
 *   - PIR 미감지(`SENSOR_INACTIVE`) + SSR ON → 타이머(`LIGHT_OFF_DELAY`) 후 SSR OFF
 * - **오류 감지:**
 *   - CDS 밝음(`CDS_BRIGHT`) + PIR 감지(`SENSOR_ACTIVE`) + SSR OFF → 오류 발생
 *   - CDS 어두움(`CDS_DARK`) + PIR 미감지(`SENSOR_INACTIVE`) + SSR ON → 오류 발생
 * @param data
 *     - 유형: `SensorData*`
 *     - 설명: PIR 센서 상태 및 CDS 센서 아날로그 값을 포함한 구조체 포인터.
 * @note
 * - SSR 상태는 GPIO `PA10` 핀을 통해 제어
 * - 오류 메시지는 LoRa를 통해 전송
 */
void Control_Light(SensorData *data) {
    // SSR 제어 및 타이머
    static uint32_t light_timer = 0;   // SSR 제어를 위한 타이머
    static uint32_t error_timer_1 = 0; // 오류 감지 1 타이머
    static uint32_t error_timer_2 = 0; // 오류 감지 2 타이머

    // SSR 상태 읽기
    GPIO_PinState ssr_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);

    // 센서 상태 해석
    uint8_t pir_status = (data->pir_state == GPIO_PIN_SET) ? SENSOR_ACTIVE : SENSOR_INACTIVE;
    uint8_t cds_status = (data->cds_analog_value <= CDS_LIGHT_THRESHOLD) ? CDS_DARK : CDS_BRIGHT;

    /**
     * SSR 제어
     * - PIR 상태와 SSR 상태를 기반으로 SSR을 ON/OFF 제어
     */
    SSR_Control(pir_status, ssr_state, &light_timer);

    /**
     * 오류 감지
     * - PIR, CDS, SSR 상태를 조합하여 오류 조건을 확인
     * - 오류 발생 시 LoRa를 통해 메시지 전송
     */
    Check_Errors(pir_status, cds_status, ssr_state, &error_timer_1, &error_timer_2);
}


/**
 * @brief Main Function
 */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USART3_UART_Init();

    SensorData sensorData;

    while (1) {
        Read_Sensors();
        sensorData.pir_state = pir_state;
        sensorData.cds_analog_value = cds_analog_value;
        Control_Light(&sensorData);
        HAL_Delay(1000);
    }
}


/* System Clock Configuration */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

void Error_Handler(void) {
    __disable_irq();
    while (1) {}
}
```
