```c
/**
 * @file main.c
 * @brief LoRa 송신부 프로그램
 *
 * PIR 및 CDS 센서를 기반으로 LED 상태를 제어하고 LoRa 모듈을 통해 상태 정보를 전송하는 프로그램.
 */

#include "main.h"
#include "gpio.h"
#include "adc.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

/// LoRa 패킷 구조 정의
typedef struct {
    uint8_t header;             ///< 패킷 헤더
    uint8_t src_address;        ///< 송신부 주소
    uint8_t dest_address;       ///< 수신부 주소
    uint8_t data_length;        ///< 데이터 길이
    uint8_t sequence_number;    ///< 패킷 순서 번호
    char data[64];              ///< 메시지 데이터
    uint8_t crc;                ///< CRC 체크섬
} LoRaPacket;

// --- 매크로 정의 ---
#define HEADER 0xAA  ///< 패킷 헤더 값
#define DEST_ADDRESS 0x02  ///< 수신부 주소
#define SRC_ADDRESS 0x01   ///< 송신부 주소
#define MAX_DATA_SIZE 64   ///< 최대 데이터 크기
#define LED_OFF_DELAY 5000 ///< LED가 꺼지는 대기 시간 (5초)
#define CDS_LOW_THRESHOLD 400   ///< CDS 센서 낮은 임계값
#define CDS_HIGH_THRESHOLD 2800 ///< CDS 센서 높은 임계값

// --- 전역 변수 ---
uint8_t pir_detected = 0;        ///< PIR 센서 감지 상태
uint8_t led_status = 0;          ///< LED 상태 (0: OFF, 1: ON)
uint32_t cds_value = 0;          ///< CDS 센서 값
uint32_t last_led_on_time = 0;   ///< 마지막으로 LED가 켜진 시간
volatile uint8_t dma_tx_complete = 1; ///< DMA 전송 완료 플래그
static uint8_t sequence_number = 0;   ///< 패킷 순서 번호
static char last_message[MAX_DATA_SIZE] = {0}; ///< 중복 메시지 전송 방지용

// --- 함수 선언 ---
void SystemClock_Config(void);
void ReadSensors(void);
void ControlLED(void);
void SendLoRaPacket_WOR(const char *message);
uint8_t CalculateCRC8(const uint8_t *data, uint8_t length);

/**
 * @brief 메인 함수.
 */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_UART4_Init();

    led_status = 0;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);  ///< LED OFF 초기화

    while (1) {
        ReadSensors();  ///< 센서 값 읽기
        ControlLED();   ///< LED 상태 제어 및 패킷 전송
        HAL_Delay(100);
    }
}

/**
 * @brief PIR 및 CDS 센서 값을 읽는 함수.
 */
void ReadSensors(void) {
    pir_detected = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);  ///< PIR 센서 감지 상태 읽기

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
        cds_value = HAL_ADC_GetValue(&hadc1);  ///< CDS 센서 값 읽기
    }
    HAL_ADC_Stop(&hadc1);

    printf("PIR: %d, CDS: %lu\n", pir_detected, cds_value);
}

/**
 * @brief PIR 상태에 따라 LED를 제어하고 LoRa 패킷을 전송하는 함수.
 */
void ControlLED(void) {
    if (pir_detected) {
        if (!led_status) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);  ///< LED ON
            led_status = 1;
            last_led_on_time = HAL_GetTick();
            SendLoRaPacket_WOR("PIR detected: LED ON");
        }
    } else if (led_status && (HAL_GetTick() - last_led_on_time >= LED_OFF_DELAY)) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);  ///< LED OFF
        led_status = 0;
        SendLoRaPacket_WOR("LED OFF after timeout");
    }

    if (cds_value < CDS_LOW_THRESHOLD) {
        SendLoRaPacket_WOR("Error: CDS value too low");
    } else if (cds_value > CDS_HIGH_THRESHOLD) {
        SendLoRaPacket_WOR("Error: CDS value too high");
    }
}

/**
 * @brief LoRa 패킷을 WOR(반복 전송) 방식으로 전송하는 함수.
 * @param message 송신할 메시지
 */
void SendLoRaPacket_WOR(const char *message) {
    if (strcmp(message, last_message) == 0) {
        return;  ///< 중복 메시지 방지
    }
    strncpy(last_message, message, MAX_DATA_SIZE);

    LoRaPacket packet = {
        .header = HEADER,
        .src_address = SRC_ADDRESS,
        .dest_address = DEST_ADDRESS,
        .data_length = strlen(message),
        .sequence_number = sequence_number++,
    };
    strncpy(packet.data, message, MAX_DATA_SIZE);
    packet.crc = CalculateCRC8((uint8_t *)&packet, sizeof(LoRaPacket) - sizeof(packet.crc));

    printf("LoRa 송신: %s\n", message);

    if (dma_tx_complete) {
        dma_tx_complete = 0;
        HAL_UART_Transmit_DMA(&huart4, (uint8_t *)&packet, sizeof(LoRaPacket));
    }
}

/**
 * @brief CRC-8 체크섬 계산 함수.
 * @param data 입력 데이터
 * @param length 데이터 길이
 * @return CRC 값
 */
uint8_t CalculateCRC8(const uint8_t *data, uint8_t length) {
    uint8_t crc = 0x00;
    for (uint8_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);
        }
    }
    return crc;
}

/**
 * @brief DMA 송신 완료 콜백 함수.
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART4) {
        dma_tx_complete = 1;
    }
}

/**
 * @brief 시스템 클럭을 설정하는 함수.
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

/**
 * @brief 오류 발생 시 무한 루프에 빠지는 함수.
 */
void Error_Handler(void) {
    __disable_irq();
    while (1) {
    }
}
```


```c
/**
 * @file main.c
 * @brief LoRa 수신부 프로그램
 *
 * LoRa 모듈을 통해 수신된 패킷 데이터를 읽고, CRC 검증 및 패킷 순서를 확인하여 디버그 출력하는 프로그램.
 */

#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>
#include <dma.h>


/// LoRa 패킷 구조 정의
typedef struct {
    uint8_t header;             ///< 패킷 헤더
    uint8_t src_address;        ///< 송신부 주소
    uint8_t dest_address;       ///< 수신부 주소
    uint8_t data_length;        ///< 데이터 길이
    uint8_t sequence_number;    ///< 패킷 순서 번호
    char data[64];              ///< 메시지 데이터
    uint8_t crc;                ///< CRC 체크섬
} LoRaPacket;

// --- 매크로 정의 ---
#define HEADER 0xAA  ///< 패킷 헤더 값
#define SRC_ADDRESS 0x01  ///< 송신부 주소
#define DEST_ADDRESS 0x02 ///< 수신부 주소
#define MAX_DATA_SIZE 64  ///< 최대 데이터 크기

// --- 전역 변수 ---
volatile uint8_t rx_buffer[sizeof(LoRaPacket)]; ///< DMA 수신 버퍼
volatile uint8_t message_ready = 0;             ///< 메시지 수신 완료 플래그
static uint8_t last_sequence_number = 0;        ///< 패킷 순서 확인 변수

// --- 함수 선언 ---
void SystemClock_Config(void);
void ProcessReceivedPacket(void);
uint8_t ValidateCRC8(const LoRaPacket *packet);
uint8_t CalculateCRC8(const uint8_t *data, uint8_t length);

/**
 * @brief 메인 함수.
 */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_UART4_Init();

    // DMA를 사용하여 UART4 수신 시작
    HAL_UART_Receive_DMA(&huart4, (uint8_t *)rx_buffer, sizeof(LoRaPacket));
    printf("LoRa 수신부 시작\n");

    while (1) {
        if (message_ready) {
            message_ready = 0;  ///< 플래그 초기화
            ProcessReceivedPacket();  ///< 수신된 패킷 처리
        }
    }
}

/**
 * @brief UART DMA 수신 완료 인터럽트 콜백 함수.
 * @param huart UART 핸들러
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART4) {
        message_ready = 1;
        HAL_UART_Receive_DMA(&huart4, (uint8_t *)rx_buffer, sizeof(LoRaPacket));  ///< DMA 수신 재시작
    }
}

/**
 * @brief 수신된 LoRa 패킷을 처리하는 함수.
 */
void ProcessReceivedPacket(void) {
    LoRaPacket *packet = (LoRaPacket *)rx_buffer;

    // 패킷 헤더 검증
    if (packet->header != HEADER) {
        printf("잘못된 패킷 헤더: %02X\n", packet->header);
        return;
    }

    // CRC 검증
    if (!ValidateCRC8(packet)) {
        printf("CRC 오류 발생: 패킷 무시됨\n");
        return;
    }

    // 패킷 순서 확인 및 경고 출력
    if (packet->sequence_number != last_sequence_number + 1) {
        printf("패킷 손실 또는 순서 오류: 예상 %d, 수신 %d\n", last_sequence_number + 1, packet->sequence_number);
    }
    last_sequence_number = packet->sequence_number;  ///< 최신 순서 번호 업데이트

    // 수신된 데이터 출력
    printf("수신된 메시지: %s\n", packet->data);
}

/**
 * @brief LoRa 패킷의 CRC를 검증하는 함수.
 * @param packet 수신된 LoRa 패킷
 * @return 검증 결과 (1: 성공, 0: 실패)
 */
uint8_t ValidateCRC8(const LoRaPacket *packet) {
    uint8_t calculated_crc = CalculateCRC8((uint8_t *)packet, sizeof(LoRaPacket) - sizeof(packet->crc));
    return (calculated_crc == packet->crc);
}

/**
 * @brief CRC-8 체크섬 계산 함수.
 * @param data 입력 데이터
 * @param length 데이터 길이
 * @return CRC 값
 */
uint8_t CalculateCRC8(const uint8_t *data, uint8_t length) {
    uint8_t crc = 0x00;
    for (uint8_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);
        }
    }
    return crc;
}

/**
 * @brief 시스템 클럭을 설정하는 함수.
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

/**
 * @brief 오류 발생 시 무한 루프에 빠지는 함수.
 */
void Error_Handler(void) {
    __disable_irq();
    while (1) {
    }
}
```
