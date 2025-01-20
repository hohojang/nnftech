# 현재 상태 
M0, M1 핀이 모두 GND로 연결되어 Normal Mode 상태 

MCU: STM32L452RET6

LoRa 모듈: E22-900T-22S

UART 사용: 송신부와 수신부 모두 USART1을 통해 통신

# 문제점 
UART 테스트에서 출력 없음

UART 송신부에서 "Hello" 메시지를 전송하도록 작성했지만, 출력이 보이지 않음.

UART 수신부 코드로도 데이터가 감지되지 않음.

LoRa를 통한 송수신 동작 미확인

UART 자체 통신이 동작하지 않아 LoRa를 통한 메시지 전달 여부 확인 불가.

# 점검한 사항 

# 송신부 RF_Setting 
Baud Rate: 9600 bps

Parity: 8N1 (No Parity)

Air Rate: 2.4 kbps

Transmit Power: 22 dBm

Address: 4

Channel: 2

Packet Size: 32 Bytes

Transmission Mode: Fixed Mode

```C
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

// --- Constants ---
#define CDS_THRESHOLD_LOW 500   /**< CDS 센서 낮은 밝기 임계값 */
#define CDS_THRESHOLD_HIGH 3500 /**< CDS 센서 높은 밝기 임계값 */
#define LED_OFF_DELAY 4000      /**< LED를 끄기 전 대기 시간 (밀리초) */
#define DEST_ADDR 5             /**< 수신부 주소 */

// --- Global Variables ---
uint8_t pir_detected = 0; /**< PIR 센서 감지 상태 */
uint8_t led_status = 0;   /**< 현재 LED 상태 */
uint8_t error_sent = 0;   /**< 에러 전송 플래그 */
uint32_t cds_value = 0;   /**< CDS 센서 측정값 */
uint32_t last_led_on_time = 0; /**< LED가 마지막으로 켜진 시간 기록 */

// --- Function Prototypes ---
void CreatePacket(uint8_t *packet, const char *payload, uint16_t *packet_length);
void SendLoRaMessage(const char *message);
void LED_Control(uint8_t state);
void ProcessSensorsAndSendMessages(void);
uint8_t CalculateCRC(const uint8_t *data, uint16_t length);
void SystemClock_Config(void);
void LoRa_Init(void);
void LoRa_WaitForAUX(void);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();
    MX_TIM2_Init();

    LoRa_Init(); // LoRa 초기화

    HAL_TIM_Base_Start_IT(&htim2);

    while (1) {
        ProcessSensorsAndSendMessages();
        HAL_Delay(100);
    }
}

/**
 * @brief LoRa 모듈 초기화
 */
void LoRa_Init(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET); // M0 = 0
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); // M1 = 0
    HAL_Delay(10); // 안정화 대기
}

/**
 * @brief 패킷 생성
 */
void CreatePacket(uint8_t *packet, const char *payload, uint16_t *packet_length) {
    uint16_t payload_length = strlen(payload);
    packet[0] = 0x7E;               // START_BYTE
    packet[1] = 0x01;               // 송신부 주소
    packet[2] = DEST_ADDR;          // 목적지 주소
    memcpy(&packet[3], payload, payload_length);

    // CalculateCRC 호출 시 캐스팅 추가
    uint8_t crc = CalculateCRC((const uint8_t *)payload, payload_length);
    packet[3 + payload_length] = crc;
    packet[4 + payload_length] = 0x7F; // END_BYTE
    *packet_length = 5 + payload_length;
}


/**
 * @brief CRC 계산
 */
uint8_t CalculateCRC(const uint8_t *data, uint16_t length) {
    uint8_t crc = 0;
    for (uint16_t i = 0; i < length; i++) {
        crc ^= data[i];
    }
    return crc;
}

/**
 * @brief LoRa 메시지 전송
 */
void SendLoRaMessage(const char *message) {
    uint8_t packet[256];
    uint16_t packet_length = 0;

    CreatePacket(packet, message, &packet_length);

    LoRa_WaitForAUX(); // 모듈 준비 상태 확인
    HAL_UART_Transmit(&huart1, packet, packet_length, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);
    printf("Message sent: %s\n", message);
}

/**
 * @brief LoRa AUX 핀 대기
 */
void LoRa_WaitForAUX(void) {
    while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET) {
        HAL_Delay(1);
    }
}

/**
 * @brief 센서 상태 확인 및 메시지 전송
 */
void ProcessSensorsAndSendMessages(void) {
    pir_detected = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);
    if (pir_detected) {
        LED_Control(1);
        last_led_on_time = HAL_GetTick();
        error_sent = 0; // 에러 플래그 초기화
    } else if (led_status == 1 && (HAL_GetTick() - last_led_on_time >= LED_OFF_DELAY)) {
        LED_Control(0);
    }

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
        cds_value = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);

    if (cds_value < CDS_THRESHOLD_LOW) {
        SendLoRaMessage("Error: CDS Value Too Low");
    } else if (cds_value > CDS_THRESHOLD_HIGH) {
        SendLoRaMessage("Error: CDS Value Too High");
    }
}

/**
 * @brief LED 상태 제어
 */
void LED_Control(uint8_t state) {
    if (state != led_status) {
        led_status = state;
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
        SendLoRaMessage(state ? "LED ON" : "LED OFF");
    }
}
```

# 수신부 RF_Setting 
Baud Rate: 9600 bps

Parity: 8N1 (No Parity)

Air Rate: 2.4 kbps

Receive Power: 22 dBm

Address: 5

Channel: 2

Packet Size: 32 Bytes

Transmission Mode: Fixed Mode

```C
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

#define START_BYTE 0x7E
#define END_BYTE 0x7F
#define MAX_PACKET_SIZE 256

uint8_t rx_buffer[MAX_PACKET_SIZE] = {0};
uint8_t packet_ready = 0;
uint16_t received_length = 0;

void SystemClock_Config(void);
void ProcessReceivedPacket(uint8_t *packet, uint16_t length);
uint8_t ValidatePacket(uint8_t *packet, uint16_t length);
uint8_t CalculateCRC(const uint8_t *data, uint16_t length);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    HAL_UART_Receive_IT(&huart1, rx_buffer, MAX_PACKET_SIZE);

    while (1) {
        if (packet_ready) {
            if (ValidatePacket(rx_buffer, received_length)) {
                ProcessReceivedPacket(rx_buffer, received_length);
            } else {
                printf("Invalid packet received.\n");
            }
            packet_ready = 0;
            memset(rx_buffer, 0, sizeof(rx_buffer));
            HAL_UART_Receive_IT(&huart1, rx_buffer, MAX_PACKET_SIZE);
        }
    }
}

void ProcessReceivedPacket(uint8_t *packet, uint16_t length) {
    const char *payload = (const char *)&packet[3]; // Payload 시작 위치
    if (strcmp(payload, "LED ON") == 0) {
        printf("Received: LED ON\n");
    } else if (strcmp(payload, "LED OFF") == 0) {
        printf("Received: LED OFF\n");
    } else if (strstr(payload, "Error") != NULL) {
        printf("Error received: %s\n", payload);
    } else {
        printf("Unknown command: %s\n", payload);
    }
}

uint8_t ValidatePacket(uint8_t *packet, uint16_t length) {
    if (length < 5) return 0; // 최소 길이 검증
    if (packet[0] != START_BYTE || packet[length - 1] != END_BYTE) return 0; // 시작/종료 바이트 확인
    uint8_t crc = CalculateCRC(&packet[3], length - 5); // CRC 계산
    return (crc == packet[length - 2]); // 수신된 CRC와 비교
}

uint8_t CalculateCRC(const uint8_t *data, uint16_t length) {
    uint8_t crc = 0;
    for (uint16_t i = 0; i < length; i++) {
        crc ^= data[i];
    }
    return crc;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        packet_ready = 1; // 패킷 수신 완료 플래그
        received_length = MAX_PACKET_SIZE - __HAL_DMA_GET_COUNTER(huart->hdmarx); // 실제 수신된 데이터 길이 계산
    }
}
```
