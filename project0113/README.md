송신부
```C
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h> 

// --- Constants ---
#define CDS_THRESHOLD_LOW 500   /**< CDS 센서 낮은 밝기 임계값 */
#define CDS_THRESHOLD_HIGH 3500 /**< CDS 센서 높은 밝기 임계값 */
#define LED_OFF_DELAY 4000      /**< LED를 끄기 전 대기 시간 (밀리초) */

// --- Global Variables ---
uint8_t pir_detected = 0; /**< PIR 센서 감지 상태 */
uint8_t led_status = 0;   /**< 현재 LED 상태 (1: 켜짐, 0: 꺼짐) */
uint8_t last_led_status = 0; /**< 이전 LED 상태 (1: 켜짐, 0: 꺼짐) */
uint8_t error_sent_led_stuck = 0; /**< LED 고장 에러 전송 플래그 */
uint8_t error_sent_cds_low = 0;   /**< CDS 낮은 값 에러 전송 플래그 */
uint8_t error_sent_cds_high = 0;  /**< CDS 높은 값 에러 전송 플래그 */
uint32_t cds_value = 0; /**< CDS 센서 측정값 */
uint32_t last_led_on_time = 0; /**< LED가 마지막으로 켜진 시간 기록 */

// --- Function Prototypes ---

void LED_Control(uint8_t state);
void CheckErrorAndSend(void);
void SendLoRaMessage(const char *message);
void SystemClock_Config(void);
void ProcessSensorsAndAct(void);


int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART3_UART_Init();
    MX_ADC1_Init();
    MX_TIM2_Init();

    HAL_TIM_Base_Start_IT(&htim2);

    while (1) {
        ProcessSensorsAndAct();
        HAL_Delay(100); // CPU 사용량 감소를 위한 대기 시간
    }
}
/**
 * @brief PIR, CDS 센서 및 LED 상태를 처리
 *
 * @details
 * - PIR 센서를 읽어 LED 상태를 제어
 * - CDS 센서 값을 읽어 저장
 * - 에러 조건을 확인하고 메시지를 전송
 */
void ProcessSensorsAndAct(void) {
    pir_detected = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);
    if (pir_detected) {
        LED_Control(1);
        last_led_on_time = HAL_GetTick();

    } else if (led_status == 1 && (HAL_GetTick() - last_led_on_time >= LED_OFF_DELAY)) {
        LED_Control(0);
    }

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
        cds_value = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);

    CheckErrorAndSend();
}
/**
 * @brief LED 상태를 제어
 *
 * @param state 켜짐(1) 또는 꺼짐(0) 상태
 * @details 상태 변경 시 LoRa로 메시지를 전송하고 GPIO 핀을 제어
 */
void LED_Control(uint8_t state) {
    if (state != led_status) {
        led_status = state;
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
        SendLoRaMessage(state ? "LED ON" : "LED OFF");
    }
}
/**
 * @brief 에러 조건을 확인하고 메시지를 전송
 *
 * @details
 * - LED가 일정 시간 이상 켜져 있으면 "LED stuck ON" 에러를 전송
 * - CDS 값이 너무 낮거나 너무 높으면 에러 메시지를 전송
 * - 에러가 해제되면 플래그를 초기화
 */
void CheckErrorAndSend(void) {
    uint32_t current_time = HAL_GetTick();

    if (led_status == 1 && (current_time - last_led_on_time >= LED_OFF_DELAY + 1000)) {
        if (!error_sent_led_stuck) {
            SendLoRaMessage("Error: LED stuck ON");
            error_sent_led_stuck = 1;
        }
    } else if (cds_value < CDS_THRESHOLD_LOW) {
        if (!error_sent_cds_low) {
            SendLoRaMessage("Error: CDS value too low");
            error_sent_cds_low = 1;
        }
    } else if (cds_value > CDS_THRESHOLD_HIGH) {
        if (!error_sent_cds_high) {
            SendLoRaMessage("Error: CDS value too high");
            error_sent_cds_high = 1;
        }
    } else {
        error_sent_led_stuck = 0;
        error_sent_cds_low = 0;
        error_sent_cds_high = 0;
    }
}
/**
 * @brief LoRa를 통해 메시지를 전송
 *
 * @param message 전송할 메시지 문자열
 * @details HAL UART API를 이용하여 문자열 메시지를 송신
 */
void SendLoRaMessage(const char *message) {
    HAL_UART_Transmit(&huart3, (uint8_t *)message, strlen(message), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);
    printf("Message sent: %s\n", message);
}

/**
 * @brief 타이머 인터럽트 콜백 함수
 *
 * @param htim 타이머 핸들러
 * @details 타이머 주기 작업이 필요하면 구현
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {

    }
  }
 }
}
```
