#ifndef LORA_H
#define LORA_H

#include "spi.h"
#include "gpio.h"

// LoRa 초기화
void LoRa_Init(void);

// 레지스터 읽기/쓰기
void LoRa_WriteRegister(uint8_t reg, uint8_t value);
uint8_t LoRa_ReadRegister(uint8_t reg);

// 데이터 송수신
void LoRa_SendData(uint8_t *data, uint8_t length);
void LoRa_ReceiveData(uint8_t *buffer, uint8_t length);

// DIO 핀 확인
void LoRa_CheckDIO(void);

#endif // LORA_H
