/**
 * @file main.c
 * @brief LoRa 송신부 코드 (Fixed Mode 지원)
 * @details
 * - PIR 센서 감지 시 LED ON/OFF 및 LoRa 패킷 전송
 * - CDS 센서 값을 측정하여 LoRa 패킷 전송
 * - LoRa 모듈을 Fixed Mode (M0=1, M1=0)로 설정하여 특정 주소로 전송
 */

#include "main.h"
#include "gpio.h"
#include "adc.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

/** @brief LoRa 패킷 정의 */
#define HEADER       0x00   ///< 패킷 헤더
#define DEST_ADDR    0x02   ///< 목적지 주소
#define CHANNEL      0x04   ///< LoRa 채널
#define LED_ON_CMD   0xA1   ///< LED ON 명령
#define LED_OFF_CMD  0xA2   ///< LED OFF 명령
#define CDS_LOW_ERR  0xE1   ///< CDS 값이 너무 낮을 때 오류 코드
#define CDS_HIGH_ERR 0xE2  ///< CDS 값이 너무 높을 때 오류 코드
#define ON           0x01   ///< ON 상태 값
#define OFF          0x00   ///< OFF 상태 값

#define CDS_LOW_THRESHOLD  400   ///< CDS 센서 낮은 값 임계치
#define CDS_HIGH_THRESHOLD 2800  ///< CDS 센서 높은 값 임계치
#define LED_ON_TIME        5000  ///< LED ON 유지 시간

/** @brief LoRa 모듈 M0, M1 핀 설정 */
#define LORA_M0_PORT GPIOB
#define LORA_M0_PIN GPIO_PIN_1
#define LORA_M1_PORT GPIOA
#define LORA_M1_PIN GPIO_PIN_9

/** @brief 전송 디버깅을 위한 패킷 버퍼 */
static volatile uint8_t debug_tx_buffer[5] __attribute__((used)) = {0};

/** @brief PIR 센서 감지 상태 */
uint8_t pir_detected = 0;
/** @brief CDS 센서 값 */
uint32_t cds_value = 0;
/** @brief LED 상태 */
uint8_t led_status = 0;
/** @brief 마지막 LED ON 시간 기록 */
uint32_t last_led_on_time = 0;

void SystemClock_Config(void);
void ReadSensors(void);
void ControlLED(void);
void SendLoRaPacket(uint8_t cmd, uint8_t data);
void SetLoRaMode(void);

/**
 * @brief 메인 실행 루프
 */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();

    SetLoRaMode();
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

    while (1) {
        ReadSensors();
        ControlLED();

        printf("TX Buffer: %02X %02X %02X %02X %02X\n",
               debug_tx_buffer[0], debug_tx_buffer[1], debug_tx_buffer[2],
               debug_tx_buffer[3], debug_tx_buffer[4]);

        HAL_Delay(500);
    }
}

/**
 * @brief LoRa 모듈을 Fixed Mode로 설정 (M0=1, M1=0)
 */
void SetLoRaMode(void) {
    HAL_GPIO_WritePin(LORA_M0_PORT, LORA_M0_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LORA_M1_PORT, LORA_M1_PIN, GPIO_PIN_RESET);
}

/**
 * @brief PIR 및 CDS 센서 데이터 읽기
 */
void ReadSensors(void) {
    pir_detected = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
        cds_value = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);
}

/**
 * @brief LED 상태 제어 및 LoRa 패킷 전송
 */
void ControlLED(void) {
    if (pir_detected) {
        if (!led_status) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
            led_status = 1;
            last_led_on_time = HAL_GetTick();
            SendLoRaPacket(LED_ON_CMD, ON);
        }
    }
    else if (led_status && (HAL_GetTick() - last_led_on_time >= LED_ON_TIME)) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
        led_status = 0;
        SendLoRaPacket(LED_OFF_CMD, OFF);
    }
    if (cds_value < CDS_LOW_THRESHOLD) {
        SendLoRaPacket(CDS_LOW_ERR, OFF);
    }
    else if (cds_value > CDS_HIGH_THRESHOLD) {
        SendLoRaPacket(CDS_HIGH_ERR, OFF);
    }
}

/**
 * @brief LoRa 패킷을 전송
 * @param cmd 전송할 명령 코드
 * @param data 전송할 데이터 값
 */
void SendLoRaPacket(uint8_t cmd, uint8_t data) {
    uint8_t packet[5] = {HEADER, DEST_ADDR, CHANNEL, cmd, data}; // ✅ 패킷 길이 조정
    memcpy((uint8_t *)debug_tx_buffer, packet, sizeof(packet));

    printf("TX Packet: %02X %02X %02X %02X %02X\n",
           debug_tx_buffer[0], debug_tx_buffer[1], debug_tx_buffer[2],
           debug_tx_buffer[3], debug_tx_buffer[4]);

    HAL_UART_Transmit(&huart1, packet, sizeof(packet), HAL_MAX_DELAY);
}

/**
 * @brief 시스템 클럭 설정
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
        Error_Handler();
    }

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 64;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief 오류 발생 시 무한 루프
 */
void Error_Handler(void) {
    __disable_irq();
    while (1) {
    }
}
