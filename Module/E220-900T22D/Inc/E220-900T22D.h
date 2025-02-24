#ifndef E220_900T22D_H
#define E220_900T22D_H

#include <stdint.h>
#include "usart.h"
#include "../../Module/Queue/Inc/Queue.h"

#define MAX_PACKET_SIZE 12  // LoRa 패킷 크기 설정

// 📌 함수 선언 추가
void SendLoRaPacket(uint8_t *data, uint8_t length);
void SendLoRaCommand(uint8_t cmd, uint8_t data); // 이 부분 추가
void ReceiveLoRaPacket(void);
void ProcessLoRaTxQueue(CircularQueue *loRaTxQueue);
void ProcessLoRaRxQueue(CircularQueue *loRaRxQueue);

#endif
