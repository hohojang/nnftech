#include "../../Module/E220-900T22D/Inc/E220-900T22D.h"
#include "../../Module/Protocol/Inc/ComProtocol.h"
#include "usart.h"
#include "../../Module/Queue/Inc/Queue.h"
#include <string.h>
#include <stdio.h>

// 📌 rx_buffer는 main.c에서 정의
extern volatile uint8_t rx_buffer[MAX_PACKET_SIZE];

/**
 * @brief LoRa 패킷 송신 (Normal Mode에서 UART 전송)
 */
void SendLoRaPacket(uint8_t *data, uint8_t length) {
    HAL_StatusTypeDef result = HAL_UART_Transmit(&huart1, data, length, HAL_MAX_DELAY);

    if (result == HAL_OK) {
        printf("[LoRa TX] Sent Packet: ");
        for (uint8_t i = 0; i < length; i++) {
            printf("%02X ", data[i]);
        }
        printf("\n");
    } else {
        printf("[LoRa TX] ERROR: Transmission Failed!\n");
    }

    HAL_Delay(50); // 안정적인 전송을 위한 지연
}

/**
 * @brief LoRa 패킷 수신 (UART 인터럽트)
 */
void ReceiveLoRaPacket(void) {
    HAL_UART_Receive_IT(&huart1, (uint8_t *)rx_buffer, MAX_PACKET_SIZE);
}

/**
 * @brief LoRa 패킷 생성 및 전송 (Command 기반)
 */
void SendLoRaCommand(uint8_t cmd, uint8_t data) {
    LoRaPacket packet;
    CreateLoRaPacket(&packet, cmd, data);
    SendLoRaPacket((uint8_t *)&packet, sizeof(LoRaPacket));
}

/**
 * @brief LoRa 송신 큐 처리 (큐에서 데이터를 꺼내 LoRa 전송)
 */
void ProcessLoRaTxQueue(CircularQueue *loRaTxQueue) {
    uint8_t packet[MAX_PACKET_SIZE];

    while (Dequeue(loRaTxQueue, packet)) {
        SendLoRaPacket(packet, MAX_PACKET_SIZE);
        printf("[LoRa TX Queue] Sent: ");
        for (uint8_t i = 0; i < MAX_PACKET_SIZE; i++) {
            printf("%02X ", packet[i]);
        }
        printf("\n");
    }
}

/**
 * @brief LoRa 수신 큐 처리 (큐에서 데이터를 꺼내고 해석)
 */
void ProcessLoRaRxQueue(CircularQueue *loRaRxQueue) {
    uint8_t receivedData[MAX_PACKET_SIZE];

    if (Dequeue(loRaRxQueue, receivedData)) {
        printf("[LoRa RX Queue] Processing Packet: ");
        for (uint8_t i = 0; i < MAX_PACKET_SIZE; i++) {
            printf("%02X ", receivedData[i]);
        }
        printf("\n");

        // LoRa 패킷 해석
        ParseLoRaPacket(receivedData);
    }
}

void TestLoRaTransmission(void) {
    uint8_t testPacket[MAX_PACKET_SIZE] = {0xFE, 0x01, 0x02, 0x00, 0x03, 0x01, 0x01, 0xF5, 0xEF};

    printf("[LoRa TX] Testing Packet Transmission...\n");

    HAL_StatusTypeDef result = HAL_UART_Transmit(&huart1, testPacket, MAX_PACKET_SIZE, 1000);

    if (result == HAL_OK) {
        printf("[LoRa TX] Packet Sent Successfully!\n");
    } else {
        printf("[LoRa TX ERROR] Transmission Failed!\n");
    }
}

