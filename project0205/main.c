//수신부
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

// --- 패킷 구조 ---
#define HEADER 0xAA  // 패킷 헤더
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
volatile uint8_t rx_buffer[sizeof(LoRaPacket)];  // DMA 수신 버퍼
volatile uint8_t message_ready = 0;  // 수신 완료 플래그

// --- 함수 선언 ---
void SystemClock_Config(void);
void ProcessReceivedPacket(void);
uint8_t ValidateChecksum(const LoRaPacket *packet);

// --- 메인 함수 ---
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_UART4_Init();

    // DMA를 통한 UART 수신 시작 (형 변환 적용)
    HAL_UART_Receive_DMA(&huart4, (uint8_t *)(rx_buffer), sizeof(LoRaPacket));
    printf("LoRa 수신부 시작\n");

    while (1) {
        if (message_ready) {
            message_ready = 0;  // 플래그 리셋
            ProcessReceivedPacket();  // 수신된 패킷 처리
        }
    }
}

/**
 * @brief DMA 수신 완료 콜백 함수
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART4) {
        message_ready = 1;  // 수신 완료 플래그 설정
        HAL_UART_Receive_DMA(&huart4, (uint8_t *)(rx_buffer), sizeof(LoRaPacket));  // 다음 수신 시작
    }
}

/**
 * @brief 수신된 패킷을 처리하는 함수
 */
void ProcessReceivedPacket(void) {
    LoRaPacket *packet = (LoRaPacket *)rx_buffer;

    // 패킷 헤더 및 체크섬 검증
    if (packet->header != HEADER) {
        printf("잘못된 패킷 헤더: %02X\n", packet->header);
        return;
    }
    if (!ValidateChecksum(packet)) {
        printf("체크섬 오류: 패킷 무시됨\n");
        return;
    }

    // 유효한 메시지 출력
    printf("수신된 메시지: %s\n", packet->data);

    // 특정 메시지에 따른 동작 추가 가능
    if (strstr(packet->data, "PIR detected") != NULL) {
        printf("PIR 감지 메시지 처리\n");
    } else if (strstr(packet->data, "Error: CDS value too low") != NULL) {
        printf("CDS 값이 너무 낮음 - 경고 처리\n");
    } else if (strstr(packet->data, "Error: CDS value too high") != NULL) {
        printf("CDS 값이 너무 높음 - 경고 처리\n");
    } else if (strstr(packet->data, "LED OFF after timeout") != NULL) {
        printf("LED가 꺼졌음\n");
    } else {
        printf("알 수 없는 메시지\n");
    }
}

/**
 * @brief 패킷의 체크섬을 검증하는 함수
 */
uint8_t ValidateChecksum(const LoRaPacket *packet) {
    uint8_t calculated_checksum = 0;
    calculated_checksum += packet->header;
    calculated_checksum += packet->src_address;
    calculated_checksum += packet->dest_address;
    calculated_checksum += packet->data_length;
    for (int i = 0; i < packet->data_length; i++) {
        calculated_checksum += packet->data[i];
    }
    return (calculated_checksum == packet->checksum);
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
