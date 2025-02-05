//송신부
#include "main.h"
#include "gpio.h"
#include "adc.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

// --- 패킷 구조 ---
#define HEADER 0xAA  // 패킷 헤더
#define DEST_ADDRESS 0x02  // 수신부 주소
#define SRC_ADDRESS 0x01  // 송신부 주소
#define MAX_DATA_SIZE 64  // 최대 데이터 크기

typedef struct {
    uint8_t header;
    uint8_t src_address;
    uint8_t dest_address;
    uint8_t data_length;
    char data[MAX_DATA_SIZE];
    uint8_t checksum;
} LoRaPacket;

// --- 전역 변수 ---
uint8_t pir_detected = 0;
uint8_t led_status = 0;
uint32_t cds_value = 0;
uint32_t last_led_on_time = 0;
volatile uint8_t dma_tx_complete = 1;  // DMA 송신 완료 플래그

#define LED_OFF_DELAY 5000  /**< LED가 OFF되기 전 대기 시간 (5초) */
#define CDS_LOW_THRESHOLD 400  /**< CDS 값이 낮다고 판단할 임계값 */
#define CDS_HIGH_THRESHOLD 2800  /**< CDS 값이 높다고 판단할 임계값 */

// --- 함수 선언 ---
void SystemClock_Config(void);
void ReadSensors(void);
void ControlLED(void);
void SendLoRaPacket(const char *message);
uint8_t CalculateChecksum(const LoRaPacket *packet);

// --- 메인 함수 ---
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_UART4_Init();

    while (1) {
        ReadSensors();  // 센서 값 읽기
        ControlLED();   // LED 상태 제어 및 패킷 전송
        HAL_Delay(100);
    }
}

/**
 * @brief PIR 및 CDS 센서 값을 읽는 함수
 */
void ReadSensors(void) {
    // PIR 센서 감지 상태 읽기
    pir_detected = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);

    // CDS 센서 아날로그 값 읽기
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
        cds_value = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);

    printf("PIR: %d, CDS: %lu\n", pir_detected, cds_value);
}

/**
 * @brief PIR 감지 상태에 따라 LED를 제어하고 LoRa 패킷을 전송하는 함수
 */
void ControlLED(void) {
    if (pir_detected) {
        if (!led_status) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);  // LED 켜기
            led_status = 1;
            last_led_on_time = HAL_GetTick();
            SendLoRaPacket("PIR detected: LED ON");
        }
    } else if (led_status && (HAL_GetTick() - last_led_on_time >= LED_OFF_DELAY)) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);  // LED 끄기
        led_status = 0;
        SendLoRaPacket("LED OFF after timeout");
    }

    // CDS 값에 따른 에러 메시지 전송
    if (cds_value < CDS_LOW_THRESHOLD) {
        SendLoRaPacket("Error: CDS value too low");
    } else if (cds_value > CDS_HIGH_THRESHOLD) {
        SendLoRaPacket("Error: CDS value too high");
    }
}

/**
 * @brief LoRa 패킷을 전송하는 함수
 */
void SendLoRaPacket(const char *message) {
    LoRaPacket packet;
    packet.header = HEADER;
    packet.src_address = SRC_ADDRESS;
    packet.dest_address = DEST_ADDRESS;
    packet.data_length = strlen(message);
    strncpy(packet.data, message, MAX_DATA_SIZE);
    packet.checksum = CalculateChecksum(&packet);

    if (dma_tx_complete) {
        dma_tx_complete = 0;
        HAL_UART_Transmit_DMA(&huart4, (uint8_t *)&packet, sizeof(LoRaPacket));
        printf("패킷 전송: %s\n", message);
    }
}

/**
 * @brief LoRa 패킷의 체크섬을 계산하는 함수
 */
uint8_t CalculateChecksum(const LoRaPacket *packet) {
    uint8_t sum = 0;
    sum += packet->header;
    sum += packet->src_address;
    sum += packet->dest_address;
    sum += packet->data_length;
    for (int i = 0; i < packet->data_length; i++) {
        sum += packet->data[i];
    }
    return sum;
}

/**
 * @brief UART DMA 송신 완료 콜백 함수
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART4) {
        dma_tx_complete = 1;
        printf("패킷 송신 완료\n");
    }
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
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
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
