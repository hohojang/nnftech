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
static uint32_t ssr_on_time = 0;
uint32_t pir_timer = 0; // PIR 감지 타이머

/* 이전 상태 저장 변수 */
GPIO_PinState prev_pir_state = GPIO_PIN_RESET;
GPIO_PinState prev_cds_digital_state = GPIO_PIN_RESET;
uint32_t prev_cds_analog_value = 0;

/* Function Prototypes -------------------------------------------------------*/
void SystemClock_Config(void);
void Read_Sensors(void);
void Control_Light(void);
void UART_SendString(const char *str);
void LoRa_SendData(const char *data);
void Error_Alert(const char *message);
void Monitor_SSR_Status(void);

/* USER CODE BEGIN 0 */

/**
 * @brief UART 문자열 전송
 */
void UART_SendString(const char *str) {
    HAL_UART_Transmit(&huart3, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}

/**
 * @brief LoRa 데이터 전송
 */
void LoRa_SendData(const char *data) {
    HAL_UART_Transmit(&huart3, (uint8_t *)data, strlen(data), HAL_MAX_DELAY);
    UART_SendString("[INFO] LoRa Data Sent: ");
    UART_SendString(data);
    UART_SendString("\r\n");
}

/**
 * @brief 에러 알림
 */
void Error_Alert(const char *message) {
    UART_SendString("[ERROR] ");
    UART_SendString(message);
    UART_SendString("\r\n");

    LoRa_SendData("[ERROR] ");
    LoRa_SendData(message);
    LoRa_SendData("\r\n");
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
 * @brief 조명 제어 및 에러 감지
 */
void Control_Light(void) {
    static uint32_t light_timer = 0;
    GPIO_PinState light_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);

    // 🌟 1. CDS 값이 어두움 (CDS_LIGHT_THRESHOLD 이하)
    if (cds_analog_value <= CDS_LIGHT_THRESHOLD) {
        if (light_state == GPIO_PIN_RESET) {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET); // 조명 ON
            UART_SendString("[INFO] Light ON (Low Light Condition).\r\n");
        }
        light_timer = 0; // 타이머 초기화
    }
    // 🌟 2. CDS 값이 밝음 (CDS_LIGHT_THRESHOLD 초과)
    else {
        if (pir_state == GPIO_PIN_RESET) {
            if (light_state == GPIO_PIN_SET) {
                if (light_timer == 0) {
                    light_timer = HAL_GetTick(); // 타이머 시작
                }
                else if ((HAL_GetTick() - light_timer) >= LIGHT_OFF_DELAY) {
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); // 조명 OFF
                    UART_SendString("[INFO] Light turned OFF after 4s due to no PIR detection.\r\n");
                    light_timer = 0;
                }
            }
        } else {
            if (light_state == GPIO_PIN_RESET) {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET); // 조명 ON
                UART_SendString("[INFO] Light ON (CDS Bright + PIR Detected).\r\n");
            }
            light_timer = 0; // 타이머 초기화
        }
    }

    // 🌟 3. 에러 조건 확인
    if (cds_analog_value > CDS_LIGHT_THRESHOLD && light_state == GPIO_PIN_SET && pir_state == GPIO_PIN_RESET) {
        Error_Alert("Light ON despite bright condition and no PIR detection.");
    }
    if (cds_analog_value <= CDS_LIGHT_THRESHOLD && light_state == GPIO_PIN_RESET && pir_state == GPIO_PIN_SET) {
        Error_Alert("Light OFF despite dark condition and PIR detection.");
    }

    // 🌟 4. 상태 메시지 출력
    snprintf(uart_buffer, sizeof(uart_buffer),
             "[STATUS] PIR: %s | CDS Analog: %lu | Light: %s\r\n",
             (pir_state == GPIO_PIN_SET) ? "DETECTED" : "NO MOTION",
             cds_analog_value,
             (light_state == GPIO_PIN_SET) ? "ON" : "OFF");
    UART_SendString(uart_buffer);
}

/**
 * @brief SSR 상태 안정성 확인
 */
void Monitor_SSR_Status(void) {
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_SET) {
        ssr_on_time += 1;
    } else {
        ssr_on_time = 0;
    }
}

/* USER CODE END 0 */

/**
 * @brief 메인 함수
 */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_SPI1_Init();
    MX_USART3_UART_Init();

    UART_SendString("[SYSTEM] System Initialized\r\n");

    while (1) {
        Read_Sensors();
        Control_Light();
        Monitor_SSR_Status();
        HAL_Delay(1000); // 1초 주기
    }
}


/**
 * @brief System Clock Configuration
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

/* Error Handler */
void Error_Handler(void) {
    __disable_irq();
    while (1) {}
}
