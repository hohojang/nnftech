# STM32L452RET6 MCU를 사용하여 CDS 광센서와 PIR 모션 센서를 통합하고, UART를 통해 센서 데이터를 전송
# 센서 데이터를 UART 통신을 통해 PC로 출력하며, 주기적으로 타이머(TIM2) 인터럽트를 통해 센서 값을 업데이트

# 하드웨어 구성
### MCU: STM32L452RET6
### CDS 센서: 아날로그 입력 (ADC1)
### PIR 센서: 디지털 입력 (GPIOC, Pin 6)
### UART 통신: USART1 (Baudrate: 9600)

# ADC 데이터 읽기 (CDS 센서)
### ADC1을 사용해 CDS 센서로부터 조도 데이터를 읽음
### GPIO 데이터 읽기 (PIR 센서)
### GPIO 핀(PC6)을 통해 PIR 센서 상태를 읽음
# UART 출력
### 센서 데이터와 상태를 UART를 통해 PC로 출력
### TIM2 주기적 인터럽트
### TIM2를 사용해 1초마다 센서 데이터를 주기적으로 읽고 UART로 출력

# ADC 설정 (CDS 센서)
### ADC1 채널을 사용하여 CDS 센서 값을 읽음
```c
uint32_t Read_ADC(void) {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(&hadc1);
}
```

# GPIO 설정 (PIR 센서)
### GPIOC_PIN_6에서 PIR 센서의 상태를 읽음
```c
pir_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);
```

# TIM2 인터럽트 콜백
### 주기적으로 CDS 및 PIR 센서 데이터를 읽고 UART로 출력
```c
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    cds_value = Read_ADC();
    pir_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);
    printf("PIR State: %s, CDS Value: %lu\n",
           pir_state ? "DETECTED" : "NOT DETECTED", cds_value);
}
```
# UART 전송
### UART를 통해 센서 값을 출력
```c
int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}
```
# 하드웨어 문제 상황 (USB to TTL 오류)
### 현재 USB to TTL 어댑터가 작동하지 않아 UART를 통해 센서 값을 확인할 수 없는 상황입니다.
