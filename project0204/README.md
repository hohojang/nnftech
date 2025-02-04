# 진행상황
회로도에 맞게 GPIO 세팅 

M0 = 0 (PA9)

M1 = 1 (PB1)

PC6 = PIR 

PA8 = LED 

PB0 = ADC -> CDS 조도센서 

UART4_TX = PA0

UART4_RX = PA1 

AT Command로 LoRa configration 시도하였으나 동작 x 

USB To TTL 모듈과 LoRa 모듈을 1대1로 연결 후 RF_Setting 시도 -> 포트는 열리지만 모듈 인식 x  

# 송신부 
```c
#include <dma.h>
#include "main.h"
#include "gpio.h"
#include "adc.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "stm32l4xx.h"  // Core Debug 및 ITM 관련 설정

// --- 전역 변수 ---
uint8_t pir_detected = 0;
uint8_t led_status = 0;
uint32_t cds_value = 0;
uint32_t last_led_on_time = 0;
char lora_message[128];  // 전송 메시지 버퍼
HAL_StatusTypeDef uart_status;  // UART 송신 상태 추적 변수
volatile uint8_t dma_tx_complete = 1;  // DMA 송신 완료 플래그

#define LED_OFF_DELAY 5000  /**< LED가 OFF되기 전 대기 시간 (5초) */

// --- 설정 상수 ---
#define DESTINATION_ADDRESS 0x02    /**< 수신부 주소 */
#define LORA_ADDRESS 0x01           /**< 송신부 주소 */
#define LORA_CHANNEL 0x04           /**< 설정할 채널 */
#define LORA_BAUDRATE 9600          /**< UART 통신 속도 */

// --- 함수 선언 ---
void SystemClock_Config(void);
void ReadSensors(void);
void ControlLED(void);
void SendLoRaMessage_DMA(const char *message);
void ConfigureLoRa(void);
void SendATCommand(const char *command, char *response, uint16_t response_size);

// --- SWV 디버깅용 printf 설정 ---
struct __FILE { int handle; };
FILE __stdout;

// fputc 함수 재정의 - SWV로 printf 출력
int fputc(int ch, FILE *f) {
    if ((CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk) &&
        (ITM->TCR & ITM_TCR_ITMENA_Msk) &&
        (ITM->TER & (1UL << 0))) {
        while (!(ITM->PORT[0].u32));
        ITM->PORT[0].u8 = ch;
    }
    return ch;
}

/**
 * @brief 메인 함수
 */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_UART4_Init();

    // LoRa 초기화
    ConfigureLoRa();

    while (1) {
        ReadSensors();  // 센서 값 읽기
        ControlLED();   // LED 상태 제어 및 LoRa 메시지 전송
        HAL_Delay(100);
    }
}

/**
 * @brief LoRa 모듈 초기 설정 함수
 */
void ConfigureLoRa(void) {
    char command[32];
    char response[64] = {0};

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);  // M0 = 0 (PA9)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);  // M1 = 1 (PB1)
    HAL_Delay(1000);  // 안정화 시간

    // --- LoRa 설정 ---
    snprintf(command, sizeof(command), "AT+ADDR=%02X", LORA_ADDRESS);
    SendATCommand(command, response, sizeof(response));

    snprintf(command, sizeof(command), "AT+CH=%02X", LORA_CHANNEL);
    SendATCommand(command, response, sizeof(response));

    snprintf(command, sizeof(command), "AT+BAUD=%d", LORA_BAUDRATE);
    SendATCommand(command, response, sizeof(response));

    SendATCommand("AT+SAVE", response, sizeof(response));

    // --- M0 = 0, M1 = 0 (Normal Transmission Mode) ---
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);  // M0 = 0 (PA9)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);  // M1 = 0 (PB1)
    HAL_Delay(1000);  // 안정화 시간
}

/**
 * @brief AT 명령어를 LoRa 모듈로 전송하고 응답을 받는 함수
 */
void SendATCommand(const char *command, char *response, uint16_t response_size) {
    HAL_UART_Transmit(&huart4, (uint8_t *)command, strlen(command), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart4, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);

    if (response != NULL) {
        HAL_UART_Receive(&huart4, (uint8_t *)response, response_size, 2000);
        printf("AT 명령어 응답: %s\n", response);
    }
}

/**
 * @brief PIR 및 CDS 센서 값을 읽는 함수
 */
void ReadSensors(void) {
    pir_detected = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
        cds_value = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);
}

/**
 * @brief PIR 감지 상태에 따라 LED를 제어하고 상태 메시지를 전송하는 함수
 */
void ControlLED(void) {
    if (pir_detected) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
        led_status = 1;
        last_led_on_time = HAL_GetTick();
        SendLoRaMessage_DMA("PIR detected: LED ON");
    } else if (led_status == 1 && (HAL_GetTick() - last_led_on_time >= LED_OFF_DELAY)) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
        led_status = 0;
        SendLoRaMessage_DMA("LED OFF after timeout");
    }

    if (cds_value < 400) {
        SendLoRaMessage_DMA("Error: CDS value too low");
    } else if (cds_value > 2800) {
        SendLoRaMessage_DMA("Error: CDS value too high");
    }
}

/**
 * @brief DMA를 통한 LoRa 메시지 송신 함수
 */
void SendLoRaMessage_DMA(const char *message) {
    snprintf(lora_message, sizeof(lora_message), "%s\r\n", message);

    if (dma_tx_complete) {
        dma_tx_complete = 0;  // 송신 중 플래그 설정
        HAL_UART_Transmit_DMA(&huart4, (uint8_t *)lora_message, strlen(lora_message));
    } else {
        printf("이전 DMA 송신이 아직 완료되지 않음\n");
    }
}

/**
 * @brief UART DMA 송신 완료 콜백 함수
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART4) {
        dma_tx_complete = 1;  // 송신 완료 플래그 설정
        printf("LoRa 메시지 송신 완료\n");
    }
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
```

# 수신부
```c
//수신부
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>
#include <dma.h>

// --- 전역 변수 ---
volatile uint8_t rx_buffer[64];  // DMA 수신 버퍼
volatile uint8_t message_ready = 0;  // 수신 완료 플래그
volatile uint32_t last_check_time = 0;  // 마지막 메시지 체크 시간
volatile char at_response[64];  // AT 명령어 응답 버퍼 추가

// --- LoRa 설정 상수 ---
#define LORA_ADDRESS 0x02
#define LORA_CHANNEL 0x04
#define LORA_BAUDRATE 9600

// --- 함수 프로토타입 ---
void SystemClock_Config(void);
void ConfigureLoRa(void);
void SendATCommand(const char *command, volatile char *response, uint16_t response_size);
void ProcessReceivedMessage(void);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_UART4_Init();  // UART4 초기화

    // LoRa 설정
    ConfigureLoRa();

    // DMA를 통한 UART 수신 시작
    HAL_UART_Receive_DMA(&huart4, (uint8_t *)rx_buffer, sizeof(rx_buffer));

    printf("LoRa 수신부 시작\n");

    while (1) {
        // 1초마다 수신 버퍼 체크
        if (HAL_GetTick() - last_check_time >= 1000) {
            last_check_time = HAL_GetTick();  // 마지막 체크 시간 업데이트

            if (message_ready) {
                message_ready = 0;  // 플래그 리셋
                ProcessReceivedMessage();  // 수신된 메시지 처리
            }
        }
    }
}

void ConfigureLoRa(void) {
    char command[32];
    char response[64] = {0};

    // --- M0 = 0, M1 = 1 (Configuration Mode) ---
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);  // M0 = 0 (PA9)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);  // M1 = 1 (PB1)
    HAL_Delay(1000);  // 안정화 시간

    // AT+ADDR 설정
    snprintf(command, sizeof(command), "AT+ADDR=%02X", LORA_ADDRESS);
    SendATCommand(command, response, sizeof(response));
    printf("주소 설정 응답: %s\n", response);

    // AT+CH 설정
    snprintf(command, sizeof(command), "AT+CH=%02X", LORA_CHANNEL);
    SendATCommand(command, response, sizeof(response));
    printf("채널 설정 응답: %s\n", response);

    // AT+BAUD 설정
    snprintf(command, sizeof(command), "AT+BAUD=%d", LORA_BAUDRATE);
    SendATCommand(command, response, sizeof(response));
    printf("통신 속도 설정 응답: %s\n", response);

    // 설정 저장
    SendATCommand("AT+SAVE", response, sizeof(response));
    printf("설정 저장 응답: %s\n", response);

    // --- M0 = 0, M1 = 0 (Normal Transmission Mode) ---
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);  // M0 = 0 (PA9)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);  // M1 = 0 (PB1)
    HAL_Delay(1000);  // 안정화 시간
}


void SendATCommand(const char *command, volatile char *response, uint16_t response_size) {
    HAL_UART_Transmit(&huart4, (uint8_t *)command, strlen(command), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart4, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);

    if (response != NULL) {
        memset((char *)response, 0, response_size);  // 응답 버퍼 초기화
        if (HAL_UART_Receive(&huart4, (uint8_t *)response, response_size, 5000) == HAL_OK) {
            printf("AT 명령어 %s에 대한 응답: %s\n", command, response);  // 응답 출력
        } else {
            printf("AT 명령어 %s 수신 실패\n", command);
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART4) {
        // 수신 완료 플래그 설정
        message_ready = 1;

        // DMA 수신 버퍼에 새 데이터가 쌓일 수 있도록 재시작
        if (HAL_UART_Receive_DMA(&huart4, (uint8_t *)rx_buffer, sizeof(rx_buffer)) != HAL_OK) {
            printf("DMA 수신 재시작 실패\n");
        }
    }
}


void ProcessReceivedMessage(void) {
    size_t received_length = strlen((char *)rx_buffer);
    if (received_length == 0 || strstr((char *)rx_buffer, "\r\n") == NULL) {
        printf("불완전한 메시지 수신 - 무시됨\n");
        return;
    }

    printf("수신된 LoRa 메시지: %s\n", rx_buffer);

    if (strstr((char *)rx_buffer, "PIR detected") != NULL) {
        printf("PIR 감지 메시지 처리 완료\n");
    } else if (strstr((char *)rx_buffer, "CDS value too low") != NULL) {
        printf("CDS 값이 너무 낮습니다.\n");
    } else if (strstr((char *)rx_buffer, "CDS value too high") != NULL) {
        printf("CDS 값이 너무 높습니다.\n");
    } else if (strstr((char *)rx_buffer, "LED OFF after timeout") != NULL) {
        printf("LED가 꺼졌습니다.\n");
    } else {
        printf("알 수 없는 메시지입니다.\n");
    }

    // 메시지 처리 후 버퍼 초기화
    memset((uint8_t *)rx_buffer, 0x00, sizeof(rx_buffer));
}

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
```

