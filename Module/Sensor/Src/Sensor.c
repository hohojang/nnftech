#include "../../Module/Sensor/Inc/Sensor.h"
#include "../../Module/E220-900T22D/Inc/E220-900T22D.h"
#include "../../Module/Protocol/Inc/ComProtocol.h"
#include "gpio.h"
#include "adc.h"
#include "usart.h"
#include <stdio.h>

// 센서 상태 변수
uint8_t isPirDetected = 0;
uint32_t cdsLightLevel = 0;
uint8_t isLedOn = 0;
uint32_t lastLedOnTime = 0;

/**
 * @brief 센서 데이터 읽기 (PIR 및 CDS 센서 값)
 */
void ReadSensors(void) {
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
        cdsLightLevel = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);

    isPirDetected = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);

    if (isPirDetected) {
        printf("[PIR Sensor] Motion Detected! Sending LoRa...\n");
        SendLoRaCommand(CMD_PIR_DET, 1);
    }

    printf("[Sensor] CDS Level: %lu, PIR: %d\n", cdsLightLevel, isPirDetected);
}

/**
 * @brief LED 제어 (조도 센서 및 PIR 기반)
 */
void ControlLED(void) {
    uint32_t currentTime = HAL_GetTick();

    if (isPirDetected) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
        lastLedOnTime = currentTime;
        isLedOn = 1;
    } else if (isLedOn && (currentTime - lastLedOnTime >= PIR_HOLD_TIME)) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
        isLedOn = 0;
    }
}
