#ifndef COM_PROTOCOL_H
#define COM_PROTOCOL_H

#include <stdint.h>

#define STX         0xFE  ///< 패킷 시작
#define ETX         0xEF  ///< 패킷 종료

#define PACKET_SIZE 9  ///< 총 9바이트 패킷

// 패킷 명령어 정의
#define CMD_TRANS_CTRL    0x01  ///< 전등 제어
#define CMD_STATE_CHANGE  0x02  ///< 전등 상태 변경
#define CMD_PIR_DET       0x03  ///< PIR 감지
#define CMD_LED_ERROR     0x04  ///< LED 동작 불량
#define CMD_PIR_HOLD_TIME 0xA1  ///< PIR 유지 시간 설정

typedef struct {
    uint8_t stx;
    uint8_t cmd;
    uint8_t length;
    uint8_t data;
    uint8_t checksum;
    uint8_t etx;
} LoRaPacket;

void CreateLoRaPacket(LoRaPacket *packet, uint8_t cmd, uint8_t data);
void ParseLoRaPacket(uint8_t *receivedData);

#endif
