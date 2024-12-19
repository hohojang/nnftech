#include "LoRa.h"
#include <stdio.h>

/* LED 핀 정의 */
#define LED_PIN GPIO_PIN_8      // LED 핀: PA8
#define LED_GPIO_PORT GPIOA     // LED 포트: GPIOA

/* LoRa 초기화 함수 */
void LoRa_Init(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);  // LoRa RESET 핀 Low
    HAL_Delay(10);  // 10ms 대기
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);    // LoRa RESET 핀 High
    HAL_Delay(100);  // 초기화 완료 대기
}

/* LoRa 레지스터에 데이터 쓰기 */
void LoRa_WriteRegister(uint8_t reg, uint8_t value)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);  // NSS Low
    uint8_t txData[2] = {reg | 0x80, value};  // Write 명령
    HAL_SPI_Transmit(&hspi1, txData, 2, HAL_MAX_DELAY);  // SPI 데이터 전송
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);  // NSS High
}

/* LoRa 레지스터에서 데이터 읽기 */
uint8_t LoRa_ReadRegister(uint8_t reg)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);  // NSS Low
    uint8_t txData = reg & 0x7F;  // Read 명령
    uint8_t rxData = 0;
    HAL_SPI_Transmit(&hspi1, &txData, 1, HAL_MAX_DELAY);  // SPI 명령 전송
    HAL_SPI_Receive(&hspi1, &rxData, 1, HAL_MAX_DELAY);   // SPI 데이터 수신
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);  // NSS High
    return rxData;
}

/* LoRa 데이터 송신 */
void LoRa_SendData(uint8_t *data, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        LoRa_WriteRegister(0x00 + i, data[i]);  // FIFO에 데이터 쓰기
    }
    LoRa_WriteRegister(0x01, 0x83);  // 송신 모드 설정
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET);  // 송신 중 LED ON
    HAL_Delay(500);
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET);    // 송신 후 LED OFF
}

/* LoRa 데이터 수신 */
void LoRa_ReceiveData(uint8_t *buffer, uint8_t length)
{
    LoRa_WriteRegister(0x01, 0x85);  // 수신 모드 설정
    HAL_Delay(10);
    for (uint8_t i = 0; i < length; i++)
    {
        buffer[i] = LoRa_ReadRegister(0x00 + i);  // FIFO에서 데이터 읽기
    }
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);  // 수신 완료 시 LED 토글
}

/* LoRa DIO 핀 이벤트 처리 */
void LoRa_CheckDIO(void)
{
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_SET)  // DIO 핀 상태 확인
    {
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);  // DIO 이벤트 시 LED 토글
    }
}
