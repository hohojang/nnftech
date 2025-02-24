#include "../../Module/Protocol/Inc/ComProtocol.h"
#include <stdio.h>

/**
 * @brief LoRa 패킷 생성
 */
void CreateLoRaPacket(LoRaPacket *packet, uint8_t cmd, uint8_t data) {
    packet->stx = STX;
    packet->cmd = cmd;
    packet->length = 1;
    packet->data = data;
    packet->checksum = STX ^ cmd ^ data;
    packet->etx = ETX;
}

/**
 * @brief LoRa 패킷 해석
 */
void ParseLoRaPacket(uint8_t *receivedData) {
    if (receivedData[0] == STX && receivedData[5] == ETX) {
        printf("[LoRa RX] Command: %02X, Data: %02X\n", receivedData[1], receivedData[3]);
    } else {
        printf("[LoRa RX] Invalid Packet\n");
    }
}
