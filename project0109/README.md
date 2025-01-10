## 주요 변경 사항 
### SSR 제어 - > LED 제어 
### 송신부 / 수신부 코드 생성 
### 송신 LoRa -> 수신 LoRa (데이터 송수신) 
### (문제)수신부 반응 x 



## 송신부 - > PC 
### 디시전 테이블 

|         조건               |PIR 감지 | LED 상태 |   동작                               | 
|----------------------------|---------|---------|---------------------------------------|
|PIR 감지 , LED ON           |   1     |   1(ON) |    LED ON                            |
|PIR 감지 , LED OFF          |    1     |    0    |7초 타이머 시작, 7초 후 에러 메시지 전송|   
|PIR 미감지 , LED OFF        |    0    |   0(OFF)|    LED OFF|
|PIR미감지,LED ON(타이머 시작)|    0    |   1(ON) |4초 타이머 후 , LED OFF| 




## 송신부 역할 
### PIR 센서와 CDS 센서를 기반으로 주변 환경을 모니터링
### LED 상태를 제어
### 이상 상태(에러 조건)를 감지하여 LoRa 모듈을 통해 데이터를 전송
송신부 CODE 
```C
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h> // Include stdio.h for printf function

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
    if (led_status == 1 && (HAL_GetTick() - last_led_on_time >= LED_OFF_DELAY + 1000) && !error_sent_led_stuck) {
        SendLoRaMessage("Error: LED stuck ON");
        printf("Error message sent: LED stuck ON\n");
        error_sent_led_stuck = 1;

    } else if (cds_value < CDS_THRESHOLD_LOW && !error_sent_cds_low) {
        SendLoRaMessage("Error: CDS value too low");
        printf("Error message sent: CDS value too low\n");
        error_sent_cds_low = 1;

    } else if (cds_value > CDS_THRESHOLD_HIGH && !error_sent_cds_high) {
        SendLoRaMessage("Error: CDS value too high");
        printf("Error message sent: CDS value too high\n");
        error_sent_cds_high = 1;

    } else {
        error_sent_led_stuck = error_sent_cds_low = error_sent_cds_high = 0;
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
```
## 수신부 -> 노트북
### 디시전 테이블 
|         조건                          |수신 데이터                        |   
|---------------------------------------|----------------------------------|
|    LED 상태 제어                       | "LED ON"                         |   
|                                       | "LED OFF"                        |    
|에러 수신: PIR + LED                   |"Error: PIR detected but LED OFF"  |       
|에러 수신: CDS 낮음                    |"Error: CDS value too low"         |
|에러 수신: CDS 높음                    |"Error: CDS value too high"        |
|에러 수신: LED 안꺼짐                  |"Error: LED stuck ON"              |

## 수신부 역할 
### 송신부로부터 전송된 메시지 수신 
### 수신된 데이터를 기반으로 현재 상태 분석 및 처리 
### 수신된 메시지에 따라 로그를 출력하거나 특정 동작 수행 

수신부 CODE
```C
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h> // Include stdio.h for printf function

// --- Constants ---
#define RX_BUFFER_SIZE 128 /**< 수신 메시지 버퍼 크기 */

// --- Global Variables ---
uint8_t rx_buffer[RX_BUFFER_SIZE]; /**< 수신된 메시지를 저장하는 버퍼 */
uint8_t message_ready = 0;         /**< 메시지 수신 완료 플래그 */

// --- Function Prototypes ---
/**
 * @brief 수신된 메시지를 처리합니다.
 *
 * @param message 수신된 메시지 문자열
 */
void ProcessMessage(const char *message);

/**
 * @brief 시스템 클럭을 설정합니다.
 */
void SystemClock_Config(void);

/**
 * @brief 메인 함수: 시스템 초기화 및 메시지 처리 루프 실행
 */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART3_UART_Init();

    HAL_UART_Receive_IT(&huart3, rx_buffer, RX_BUFFER_SIZE);

    while (1) {
        if (message_ready) {
            ProcessMessage((const char *)rx_buffer);
            message_ready = 0;
            HAL_UART_Receive_IT(&huart3, rx_buffer, RX_BUFFER_SIZE);
        }

        HAL_Delay(100); // CPU 사용량 감소를 위한 대기 시간
    }
}

/**
 * @brief 수신된 메시지를 처리합니다.
 *
 * @param message 수신된 메시지 문자열
 * @details 메시지를 확인하고 동작에 따라 콘솔에 출력합니다.
 */
void ProcessMessage(const char *message) {
    printf("Received message: %s\n", message);

    if (strstr(message, "LED ON") != NULL) {
        printf("Action: LED has been turned ON.\n");
    } else if (strstr(message, "LED OFF") != NULL) {
        printf("Action: LED has been turned OFF.\n");
    } else if (strstr(message, "Error:") != NULL) {
        printf("Action: Error detected - %s\n", message);
    } else {
        printf("Action: Unknown message received.\n");
    }
}

/**
 * @brief UART 수신 완료 콜백 함수
 *
 * @param huart UART 핸들러
 * @details 메시지가 수신되면 플래그를 설정합니다.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
        message_ready = 1;
    }
}
```
