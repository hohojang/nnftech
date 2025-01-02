# 2025 - 01 - 02 
# STM32 기반 PIR & CDS 센서 정보로 방수등 ON/OFF 및 오류 상황 LoRa 통신 시스템 

##  **프로젝트 개요**
**STM32L452RET6** 마이크로컨트롤러를 사용하여 **PIR 센서**와 **CDS 조도 센서**로부터 데이터를 수집

이를 기반으로 **SSR 릴레이를 제어하여 방수등을 켜고 끄는 시스템**

또한, 시스템 상태 및 오류 정보를 **LoRa 통신 모듈**을 통해 관제 시스템으로 전송

---

## **시스템 동작 다이어그램**

```plaintext
[ PIR 센서 ] -----┐
                 │
[ CDS 센서 ] ----┤-----> [ STM32 MCU ] -----> [ LoRa 모듈 ] -----> [ 관제 시스템 ]
                 │               │
[ SSR 릴레이 ] --┘               │
                                ↓
                         [ 방수등 ON/OFF ]
```
PIR 센서: 사람의 움직임을 감지

CDS 센서: 주변의 조도를 측정

STM32 MCU: 센서 데이터를 기반으로 SSR 릴레이를 제어하고, LoRa를 통해 상태 및 오류 정보를 전송

SSR 릴레이: MCU 신호에 따라 방수등 ON / OFF

LoRa 모듈: 시스템 상태 및 오류 정보를 관제 시스템으로 전송

## **주요 동작 함수 코드**
# 1. 센서 데이터 읽기
함수: Read_Sensors()
설명: PIR 센서와 CDS 센서 데이터를 읽고 저장
```코드:
void Read_Sensors(void) {
    pir_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);
    cds_digital_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7);
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
        cds_analog_value = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);
}
```
# 2. 방수등 제어
함수: Control_Light()
설명: 센서 데이터를 기반으로 방수등을 켜고 끄며 오류를 감지
로직:
CDS 값이 기준 이하 + PIR 감지 → 조명 ON
CDS 값이 기준 초과 + PIR 미감지 → 4초 후 조명 OFF
오류 감지 시 LoRa를 통해 전송
```코드:
void Control_Light(void) {
    if (cds_analog_value <= CDS_LIGHT_THRESHOLD && pir_state == GPIO_PIN_SET) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
    } else if (cds_analog_value > CDS_LIGHT_THRESHOLD && pir_state == GPIO_PIN_RESET) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
    }
}
```
# 3. LoRa 데이터 전송
함수: LoRa_SendData()
설명: LoRa를 통해 시스템 상태 및 오류 메시지를 관제 시스템으로 전송
```코드:
void LoRa_SendData(const char *data) {
    HAL_UART_Transmit(&huart3, (uint8_t *)data, strlen(data), HAL_MAX_DELAY);
    UART_SendString("[INFO] LoRa Data Sent: ");
    UART_SendString(data);
}
```
# 4. 상태 테이블 전송
함수: Send_EEAM_Status()
설명: EEA-M 패킷을 생성하여 시스템 상태 정보를 LoRa로 전송
```코드:
void Send_EEAM_Status(void) {
    snprintf(uart_buffer, sizeof(uart_buffer),
             "[EEA-M] PIR: %s | CDS Analog: %lu | LED: %s",
             (pir_state == GPIO_PIN_SET) ? "DETECTED" : "NO MOTION",
             cds_analog_value,
             (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET) ? "ON" : "OFF");
    LoRa_SendData(uart_buffer);
}
```
# 5. 오류 처리
함수: Error_Alert()
설명: 오류 발생 시 UART 및 LoRa를 통해 오류 메시지를 전송
```코드:
void Error_Alert(const char *message) {
    snprintf(uart_buffer, sizeof(uart_buffer),
             "[ERROR] %s | CDS: %lu | LED: %s",
             message,
             cds_analog_value,
             (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET) ? "ON" : "OFF");
    LoRa_SendData(uart_buffer);
}
```
# 6. 메인 루프
```
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USART3_UART_Init();

    UART_SendString("[SYSTEM] System Initialized\r\n");

    while (1) {
        Read_Sensors();
        Control_Light();
        Monitor_SSR_Status();
        Send_EEAM_Status();
        HAL_Delay(2000); // 2초마다 실행
    }
}
```
