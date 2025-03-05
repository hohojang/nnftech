#ifndef E220_900T22D_H
#define E220_900T22D_H

#include "usart.h"
#include "../../Module/Queue/Inc/Queue.h"

#define MAX_PACKET_SIZE 4  // 고정 모드 패킷 사이즈

#define CMD_LED_ON  '1'
#define CMD_LED_OFF '0'
#define CMD_CDS_ERR 'E'

void InitLoRaModule(void);
void SendLoRaData(uint8_t cmd);
void ReceiveLoRaPacket(void);
void ProcessLoRaRxQueue(CircularQueue *queue);

#endif
