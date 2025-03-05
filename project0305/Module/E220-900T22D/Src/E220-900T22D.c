#include "../../Module/E220-900T22D/Inc/E220-900T22D.h"
#include "gpio.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;
extern uint8_t rx_buffer[MAX_PACKET_SIZE];

void InitLoRaModule(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);  // M0 = 0
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);    // M1 = 1 (Fixed Mode)
    HAL_Delay(100);

}

void SendLoRaData(uint8_t cmd) {
    uint8_t packet[4] = { 0x02, 0x03, 0x04, cmd };
    HAL_UART_Transmit(&huart1, packet, sizeof(packet), HAL_MAX_DELAY);
    printf("[LoRa TX] Sent CMD: %c\n", cmd);
}

void ProcessLoRaRxQueue(CircularQueue *queue) {
    uint8_t data[MAX_PACKET_SIZE];
    while (Dequeue(queue, data)) {
        printf("[LoRa RX] CMD: %c (Full: %02X %02X %02X %02X)\n",
               data[3], data[0], data[1], data[2], data[3]);
        switch (data[3]) {
            case CMD_LED_ON:
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
                break;
            case CMD_LED_OFF:
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
                break;
            case CMD_CDS_ERR:
                printf("[LoRa RX] CDS Error Detected!\n");
                break;
        }
    }
}
