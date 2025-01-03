/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Private variables ---------------------------------------------------------*/
GPIO_PinState pir_state;         // PIR 센서 상태
GPIO_PinState cds_digital_state; // CDS 디지털 상태
uint32_t cds_analog_value;       // CDS 아날로그 값
char uart_buffer[200];           // UART 디버깅 버퍼

/* LoRa 설정 */
#define LORA_CHANNEL "CH=5"
#define LORA_POWER "PWR=20"
#define CDS_LIGHT_THRESHOLD 1200  // CDS 조도 임계값
#define LIGHT_OFF_DELAY 4000       // PIR 미감지 후 4초 뒤 조명 OFF

/* SSR 안정성 모니터링 */
//SSR의 과열이나 오작동을 감지
static uint32_t ssr_on_time = 0;
uint32_t pir_timer = 0; // PIR 감지 타이머

/* 이전 상태 저장 변수 */
//상태 변화 감지 , 정확한 오류 처리를 위함
GPIO_PinState prev_pir_state = GPIO_PIN_RESET;
GPIO_PinState prev_cds_digital_state = GPIO_PIN_RESET;
uint32_t prev_cds_analog_value = 0;

/* Function Prototypes -------------------------------------------------------*/
//함수 생성
void SystemClock_Config(void);
void Read_Sensors(void);
void Control_Light(void);
void UART_SendString(const char *str);
void LoRa_SendData(const char *data);
void Error_Alert(const char *message);
void Monitor_SSR_Status(void);
void Send_EEAM_Status(void);

/* USER CODE BEGIN 0 */

/**
 * @brief 문자열 데이터 UART 전송
 */
void UART_SendString(const char *str) {
    HAL_UART_Transmit(&huart3, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}

/**
 * @brief LoRa 데이터 전송
 */
//LoRa 모듈과 UART3를 통해 실시간으로 데이터 전송
void LoRa_SendData(const char *data) {
    HAL_UART_Transmit(&huart3, (uint8_t *)data, strlen(data), HAL_MAX_DELAY);
    UART_SendString("[INFO] LoRa Data Sent:\r\n");
    UART_SendString(data);
    UART_SendString("\r\n");
}

/**
 * @brief SSR 상태 안정성 확인
 */
//과열 방지
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
//실시간 오류 감지 및 전송
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
//센서 값 조정 및 정상 작동 확인을 위해 UART3으로 정보 전송
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
 * @brief 조명 제어 및 타이머 처리 (PIR 상태 및 타이머 중심, 최적화)
 */
void Control_Light(void) {
    static uint32_t light_timer = 0; // 타이머 변수
    GPIO_PinState light_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10); // 현재 조명 상태

    if (pir_state == GPIO_PIN_SET) {
        // 🌟 PIR 감지 상태
        if (light_state == GPIO_PIN_RESET && cds_analog_value <= CDS_LIGHT_THRESHOLD) {
            // 어두움 && PIR 감지 && 조명 OFF → 조명 ON
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
            UART_SendString("[INFO] Light ON (Dark + PIR Detected).\r\n");
        }
        light_timer = 0; // 타이머 초기화
    } else {
        // 🌟 PIR 미감지 상태
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

        // 🌟 오류 감지
        if (cds_analog_value > CDS_LIGHT_THRESHOLD && light_state == GPIO_PIN_SET) {
            Error_Alert("No PIR, but Light is ON despite Bright Environment.");
        }
        if (cds_analog_value <= CDS_LIGHT_THRESHOLD && light_state == GPIO_PIN_RESET) {
            Error_Alert("PIR Detected, but Light is OFF.");
        }
    }

    // 🌟 상태 전송
    Send_EEAM_Status();
}


int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USART3_UART_Init();

    UART_SendString("[SYSTEM] System Initialized\r\n");

    while (1) {
        Read_Sensors();
        Control_Light();
        Monitor_SSR_Status();
        HAL_Delay(5000); //5초마다 USART3 센서 정보 출력
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
