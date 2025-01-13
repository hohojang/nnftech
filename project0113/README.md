### 진행 상태 [LoRa 보드 초기화 완료 / 통신 x]
[https://vimeo.com/1046327107?share=copy]

PIR 센서: 움직임 감지

CDS 센서: 주변 밝기 측정

LED 제어: 센서 상태에 따라 LED 켜기/끄기

LoRa 통신: 센서 데이터와 LED 상태, 에러 메시지를 송신부 → 수신부로 전달 x 

[SSR을 LED로 변경하여 수정 완료 / PIR 모션 인식에 따라 LED ON/OFF 미감지 시 4초 후 LED OFF & 너무 높은 CDS 조도 값이 감지될 시 수신부에 에러 메시지 전송 , LED가 일정 시간 이후에도 OFF가 되지 않을 시 에러 메시지 전송]

### 송신부 기능 
### PIR 감지:
움직임이 감지되면 LED를 켜고 상태를 LoRa로 전송

움직임이 멈추고 일정 시간이 지나면 LED를 꺼서 전송

### CDS 측정:
밝기가 너무 낮거나 높을 경우 에러 메시지를 전송

### LoRa 메시지 전송:
"LED ON", "LED OFF", 또는 에러 메시지 전송

### 수신부 기능 
### LoRa 메시지 수신:
송신부에서 전송된 메시지를 UART로 수신

메시지 처리: [테스트용]

"LED ON": LED를 켬

"LED OFF": LED를 끔

"Error: ": 에러 메시지를 출력

송신부
LoRa 핀 배치

M0 → PB10 - > output 

M1 → PB11 - > output 

AUX → PA15  - > input
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
void LoRa_Init(void);
void LoRa_EnterConfigMode(void);
void LoRa_WaitForAUX(void);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART3_UART_Init();
    MX_ADC1_Init();
    MX_TIM2_Init();

    LoRa_Init(); // LoRa 초기화

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
 * @brief LoRa 모듈 초기화
 */
void LoRa_Init(void) {
    // M0 = 0, M1 = 0 (Normal Mode)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
    HAL_Delay(10);
}

/**
 * @brief LoRa 모듈 설정 모드로 전환
 */
void LoRa_EnterConfigMode(void) {
    // M0 = 1, M1 = 1 (Configuration Mode)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
    HAL_Delay(100);
}

/**
 * @brief LoRa AUX 핀 대기
 */
void LoRa_WaitForAUX(void) {
    while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET) {
        HAL_Delay(1);
    }
}

/**
 * @brief LoRa 메시지 전송
 */
void SendLoRaMessage(const char *message) {
    HAL_UART_Transmit(&huart3, (uint8_t *)message, strlen(message), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);
    printf("Message sent: %s\n", message);
}

/**
 * @brief 타이머 인터럽트 콜백 함수
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        // 타이머 주기 작업
    }
}

```
수신부

LoRa 핀 배치

M0 → PB10 - > output 

M1 → PB11 - > output 

AUX → PA15  - > input

```C
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

// --- Constants ---
#define RX_BUFFER_SIZE 128 /**< 수신 메시지 버퍼 크기 */

// --- Global Variables ---
uint8_t rx_buffer[RX_BUFFER_SIZE]; /**< 수신된 메시지를 저장하는 버퍼 */
uint8_t message_ready = 0;         /**< 메시지 수신 완료 플래그 */

// --- Function Prototypes ---
void ProcessMessage(const char *message); /**< 수신된 메시지를 처리하는 함수 */
void SystemClock_Config(void);           /**< 시스템 클럭을 설정하는 함수 */

/**
 * @brief 메인 함수
 * @details 시스템 초기화 및 메시지 처리 루프 실행
 */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART3_UART_Init();

    // UART 인터럽트를 통한 수신 시작
    HAL_UART_Receive_IT(&huart3, rx_buffer, RX_BUFFER_SIZE);

    while (1) {
        if (message_ready) {
            ProcessMessage((const char *)rx_buffer); // 수신된 메시지 처리
            message_ready = 0; // 플래그 초기화
            HAL_UART_Receive_IT(&huart3, rx_buffer, RX_BUFFER_SIZE); // 다시 수신 대기
        }

        HAL_Delay(100); // CPU 사용량 감소를 위한 대기 시간
    }
}

/**
 * @brief 수신된 메시지를 처리
 * @param message 수신된 메시지 문자열
 * @details 메시지를 분석하고 동작에 따라 디버깅 정보를 출력
 */
void ProcessMessage(const char *message) {
    printf("Received message: %s\n", message); // 수신된 메시지 출력

    if (strcmp(message, "LED ON") == 0) {
        printf("Action: LED has been turned ON.\n");
    } else if (strcmp(message, "LED OFF") == 0) {
        printf("Action: LED has been turned OFF.\n");
    } else if (strstr(message, "Error:") != NULL) {
        printf("Action: Error detected - %s\n", message); // 에러 메시지 처리
    } else {
        printf("Action: Unknown message received.\n");
    }
}

/**
 * @brief UART 수신 완료 콜백 함수
 * @param huart UART 핸들러
 * @details 메시지 수신 완료 시 플래그를 설정
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) { // USART3 사용
        message_ready = 1; // 메시지 수신 완료 플래그 설정
    }
}
```
