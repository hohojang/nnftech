//송신부
#include "main.h"
#include "gpio.h"
#include "adc.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

// --- 전역 변수 ---
uint8_t pir_detected = 0;      /**< PIR 센서 감지 상태 */
uint8_t led_status = 0;        /**< LED 상태 (1: 켜짐, 0: 꺼짐) */
uint32_t cds_value = 0;        /**< CDS 센서 값 */
uint32_t last_led_on_time = 0; /**< 마지막으로 LED가 켜진 시간 */
#define LED_OFF_DELAY 5000     /**< LED가 OFF되기 전 대기 시간 (5초) */

// --- LoRa 설정 상수 ---
#define LORA_ADDRESS 0x01       /**< LoRa 모듈의 주소 설정 */
#define LORA_CHANNEL 0x04       /**< LoRa 모듈의 통신 채널 설정 */
#define LORA_BAUDRATE 9600      /**< LoRa 모듈의 UART 통신 속도 설정 */

// --- 함수 선언 ---
void SystemClock_Config(void);
void ReadSensors(void);
void ControlLED(void);
void SendLoRaMessage(const char *message);
void ConfigureLoRa(void);
void SendATCommand(const char *command, char *response, uint16_t response_size);

/**
 * @brief 메인 함수
 * @details 시스템 초기화 후 무한 루프에서 센서를 읽고 LoRa 메시지를 전송하는 역할
 */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_UART4_Init();

    // LoRa 초기화
    ConfigureLoRa();

    while (1) {
        ReadSensors();  // 센서 값 읽기
        ControlLED();   // LED 상태 제어 및 LoRa 메시지 전송
        HAL_Delay(100);
    }
}

/**
 * @brief LoRa 모듈 초기 설정 함수
 * @details LoRa 모듈을 설정 모드로 진입 후 주소, 채널, 통신 속도를 설정하고 저장
 */
void ConfigureLoRa(void) {
    char command[32];
    char response[64] = {0};

    // --- M0 = 1, M1 = 1 (Configuration Mode) ---
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);  // M0 = 1
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);  // M1 = 1
    HAL_Delay(100);

    // --- LoRa 설정 ---
    snprintf(command, sizeof(command), "AT+ADDR=%02X", LORA_ADDRESS);
    SendATCommand(command, response, sizeof(response));

    snprintf(command, sizeof(command), "AT+CH=%02X", LORA_CHANNEL);
    SendATCommand(command, response, sizeof(response));

    snprintf(command, sizeof(command), "AT+BAUD=%d", LORA_BAUDRATE);
    SendATCommand(command, response, sizeof(response));

    SendATCommand("AT+SAVE", response, sizeof(response));

    // --- M0 = 0, M1 = 0 (Normal Mode) ---
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);  // M0 = 0
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);  // M1 = 0
    HAL_Delay(100);  // 안정화 대기 시간
}

/**
 * @brief AT 명령어를 LoRa 모듈로 전송하고 응답을 받는 함수
 *
 * @param command AT 명령어 문자열
 * @param response LoRa 모듈로부터의 응답을 저장할 버퍼
 * @param response_size 응답 버퍼 크기
 */
void SendATCommand(const char *command, char *response, uint16_t response_size) {
    HAL_UART_Transmit(&huart4, (uint8_t *)command, strlen(command), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart4, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);
    HAL_UART_Receive(&huart4, (uint8_t *)response, response_size, 2000);
    printf("AT 명령어 응답: %s\n", response);
}

/**
 * @brief PIR 및 CDS 센서 값을 읽는 함수
 * @details PIR 센서의 디지털 입력 값을 읽고 CDS 센서의 아날로그 값을 변환하여 저장합니다.
 */
void ReadSensors(void) {
    pir_detected = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);  // PIR 센서 상태 읽기

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
        cds_value = HAL_ADC_GetValue(&hadc1);  // CDS 센서 값 읽기
    }
    HAL_ADC_Stop(&hadc1);
}

/**
 * @brief PIR 감지 상태에 따라 LED를 제어하고 상태 메시지를 전송하는 함수
 * @details PIR이 감지되면 LED를 켜고 일정 시간 후 자동으로 끕니다.
 */
void ControlLED(void) {
    if (pir_detected) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
        led_status = 1;
        last_led_on_time = HAL_GetTick();
        SendLoRaMessage("PIR detected: LED ON");
    } else if (led_status == 1 && (HAL_GetTick() - last_led_on_time >= LED_OFF_DELAY)) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
        led_status = 0;
        SendLoRaMessage("LED OFF after timeout");
    }

    // CDS 센서 값에 따른 에러 메시지 전송
    if (cds_value < 200) {
        SendLoRaMessage("Error: CDS value too low");
    } else if (cds_value > 3500) {
        SendLoRaMessage("Error: CDS value too high");
    }
}

/**
 * @brief LoRa 메시지를 송신하는 함수
 *
 * @param message 송신할 메시지 문자열
 */
void SendLoRaMessage(const char *message) {
    HAL_UART_Transmit(&huart4, (uint8_t *)message, strlen(message), HAL_MAX_DELAY);
    printf("LoRa 송신 메시지: %s\n", message);
}

/**
 * @brief 시스템 클럭을 설정하는 함수
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
        Error_Handler();
    }

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
}

void Error_Handler(void) {
    __disable_irq();
    while (1) {
    }
}
