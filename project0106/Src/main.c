/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // bool 타입 지원

/* Private variables ---------------------------------------------------------*/
GPIO_PinState pir_state;         // PIR 센서 상태
GPIO_PinState cds_digital_state; // CDS 디지털 상태
uint32_t cds_analog_value;       // CDS 아날로그 값
char uart_buffer[200];           // UART 디버깅 버퍼

/* LoRa 설정 */
#define LORA_CHANNEL "CH=5"
#define LORA_POWER "PWR=20"
#define CDS_LIGHT_THRESHOLD 1000  // CDS 조도 임계값 (1000으로 설정)
#define LIGHT_OFF_DELAY 4000       // PIR 미감지 후 4초 뒤 조명 OFF

/* SSR 안정성 모니터링 */
static uint32_t ssr_on_time = 0;
uint32_t pir_timer = 0; // PIR 감지 타이머

/* 이전 상태 저장 변수 */
GPIO_PinState prev_pir_state = GPIO_PIN_RESET;
GPIO_PinState prev_cds_digital_state = GPIO_PIN_RESET;
uint32_t prev_cds_analog_value = 0;

/* 센서 데이터 구조체 ---------------------------------------------------------*/
typedef struct {
    GPIO_PinState pir_state;  // PIR 센서 상태
    uint32_t cds_analog_value; // CDS 아날로그 센서 값
} SensorData;

/* Function Prototypes -------------------------------------------------------*/
void SystemClock_Config(void);
void Read_Sensors(void);
void Control_Light(SensorData *data);
void UART_SendString(const char *str);
void LoRa_SendData(const char *data);
void Error_Alert(const char *message);
void Monitor_SSR_Status(void);
void Send_EEAM_Status(void);

/**
 * @brief 문자열 데이터 UART 전송
 */
void UART_SendString(const char *str) {
    HAL_UART_Transmit(&huart3, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}

/**
 * @brief LoRa 데이터 전송
 */
void LoRa_SendData(const char *data) {
    HAL_UART_Transmit(&huart3, (uint8_t *)data, strlen(data), HAL_MAX_DELAY);
    UART_SendString("[INFO] LoRa Data Sent:\r\n");
    UART_SendString(data);
    UART_SendString("\r\n");
}

/**
 * @brief SSR 상태 안정성 확인
 */
void Monitor_SSR_Status(void) {
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET) {
        ssr_on_time += 1;
    } else {
        ssr_on_time = 0;
    }
}

/**
 * @brief 에러 알림
 */
void Error_Alert(const char *message) {
    UART_SendString("\r\n[ERROR] ---------- System Alert ----------\r\n");
    snprintf(uart_buffer, sizeof(uart_buffer),
             "[ERROR] %s\r\n"
             "[ERROR] CDS Analog: %lu\r\n"
             "[ERROR] LED State: %s\r\n"
             "[ERROR] -----------------------------------\r\n",
             message,
             cds_analog_value,
             (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET) ? "ON" : "OFF");
    UART_SendString(uart_buffer);

    LoRa_SendData("[ERROR] ");
    LoRa_SendData(message);
}

/**
 * @brief 센서 데이터 읽기
 */
void Read_Sensors(void) {
    pir_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6); // PIR 센서 읽기
    cds_digital_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7); // CDS 디지털 센서 읽기

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
        cds_analog_value = HAL_ADC_GetValue(&hadc1); // CDS 아날로그 값 읽기
    }
    HAL_ADC_Stop(&hadc1);
}

/**
 * @brief 상태 테이블 전송
 */
void Send_EEAM_Status(void) {
    UART_SendString("\r\n[EEA-M] ---------- LoRa Packet ----------\r\n");
    snprintf(uart_buffer, sizeof(uart_buffer),
             "[EEA-M] PIR: %s\r\n"
             "[EEA-M] CDS Analog: %lu\r\n"
             "[EEA-M] LED: %s\r\n"
             "[EEA-M] ----------------------------------\r\n",
             (pir_state == GPIO_PIN_SET) ? "DETECTED" : "NO MOTION",
             cds_analog_value,
             (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET) ? "ON" : "OFF");
    UART_SendString(uart_buffer);

    LoRa_SendData("[EEA-M] Status Packet Sent");
}

/**
 * @brief SSR 제어 및 타이머 처리
 * @param pir_status PIR 상태 (1 = 감지, 0 = 미감지)
 * @param ssr_state SSR 상태 (ON/OFF)
 * @param light_timer 타이머 변수
 */
void SSR_Control(uint8_t pir_status, GPIO_PinState ssr_state, uint32_t *light_timer) {
    // Rule 1: PIR 감지 && SSR OFF → SSR ON
    if (pir_status && ssr_state == GPIO_PIN_RESET) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
        UART_SendString("[INFO] Light ON (PIR Detected).\r\n");
        *light_timer = 0; // 타이머 초기화
        return;
    }

    // Rule 2: PIR 미감지 && SSR ON → 타이머로 SSR OFF
    if (!pir_status && ssr_state == GPIO_PIN_SET) {
        if (*light_timer == 0) {
            *light_timer = HAL_GetTick(); // 타이머 시작
        } else if ((HAL_GetTick() - *light_timer) >= LIGHT_OFF_DELAY) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
            UART_SendString("[INFO] Light OFF (No PIR Detected for 4s).\r\n");
            *light_timer = 0; // 타이머 초기화
        }
    }
}
/**
 * @brief 오류 감지 및 메시지 전송
 * @param pir_status PIR 상태 (1 = 감지, 0 = 미감지)
 * @param cds_dark CDS 상태 (1 = 어두움, 0 = 밝음)
 * @param ssr_state SSR 상태 (ON/OFF)
 * @param error_timer 오류 타이머 변수
 * @param message 오류 메시지
 */
void Handle_Error(bool condition, uint32_t *error_timer, const char *message) {
    if (condition) {
        if (*error_timer == 0) {
            *error_timer = HAL_GetTick(); // 타이머 시작
        } else if ((HAL_GetTick() - *error_timer) >= 7000) {
            Error_Alert(message);
            LoRa_SendData(message);
            *error_timer = 0; // 타이머 초기화
        }
    } else {
        *error_timer = 0; // 오류 조건 해제 시 타이머 초기화
    }
}
/**
 * @brief 오류 감지 (단일 조건 기반)
 */
void Check_Errors(uint8_t pir_status, uint8_t cds_dark, GPIO_PinState ssr_state,
                  uint32_t *error_timer_1, uint32_t *error_timer_2) {
    // Error 1: PIR 감지 && CDS 밝음 && SSR OFF
    Handle_Error(pir_status && !cds_dark && ssr_state == GPIO_PIN_RESET,
                 error_timer_1,
                 "[ERROR] PIR Detected, but Light is OFF (Bright Environment)");

    // Error 2: PIR 미감지 && CDS 어두움 && SSR ON
    Handle_Error(!pir_status && cds_dark && ssr_state == GPIO_PIN_SET,
                 error_timer_2,
                 "[ERROR] No PIR, but Light is ON (Dark Environment)");
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

    // SSR 제어
    SSR_Control(pir_status, ssr_state, &light_timer);

    // 오류 감지
    Check_Errors(pir_status, cds_dark, ssr_state, &error_timer_1, &error_timer_2);
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
        Monitor_SSR_Status();
        HAL_Delay(5000);
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
