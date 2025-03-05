#include "../../Module/Sensor/Inc/Sensor.h"
#include "../../Module/E220-900T22D/Inc/E220-900T22D.h"
#include "adc.h"
#include "gpio.h"
#include <stdio.h>

static uint8_t pirDetected = 0;
static uint32_t cdsLevel = 0;
static uint32_t ledOnTime = 0;

// 센서 초기화
void InitSensor(void) {
    pirDetected = 0;
    ledOnTime = 0;
}

// 센서 데이터 읽기 및 LoRa 전송
void ReadSensors(void) {
    pirDetected = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
        cdsLevel = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);

    if (pirDetected) {
        SendLoRaData(CMD_LED_ON);
        ledOnTime = HAL_GetTick();
    } else {
        if (ledOnTime != 0 && HAL_GetTick() - ledOnTime >= 5000) {
            SendLoRaData(CMD_LED_OFF);
            ledOnTime = 0;
        }
    }

    if (cdsLevel < 400 || cdsLevel > 2800) {
        SendLoRaData(CMD_CDS_ERR);
    }
}

// LED 제어 (송신부용)
void ControlLED(void) {
    if (pirDetected || (ledOnTime != 0 && HAL_GetTick() - ledOnTime < 5000)) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
    }
}
