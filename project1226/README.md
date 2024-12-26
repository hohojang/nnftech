# 🚀 **TY-302 조명 제어 시스템 + LoRa 통신**

## 📋 **프로젝트 설명**
**PIR 센서**와 **CDS 센서**를 사용하여 **TY-302 방수등**을 제어하는 시스템 
조명 상태와 센서 데이터를 **UART**를 통해 출력하고, **LoRa 통신**을 통해 원격으로 상태를 전송(미완성)

---

## ⚙️ **기술 스택**
- **MCU:** STM32L452RET6  
- **LoRa 모듈:** EBYTE E22T900T22S  
- **조명:** TY-302 방수등 (220V)  
- **센서:** PIR 센서, CDS 센서  
- **통신:** UART, LoRa  

---

## 📡 **주요 기능**

### 1️⃣ **센서 데이터 읽기**
- **PIR 센서:** 움직임 감지 여부 확인  
- **CDS 센서:** 조도의 아날로그 및 디지털 값 확인  

### 2️⃣ **조명 제어 로직**
1. **빛이 강하고 PIR 감지가 없는 경우:**  
   - PIR 센서가 감지되지 않으면 **5초 후 자동으로 조명 OFF**  

2. **빛이 약한데 조명이 꺼진 경우:**  
   - UART 및 LoRa로 **에러 메시지 전송**

3. **실시간 상태 출력:**  
   - PIR 상태, CDS 센서 값, 조명 상태 UART로 출력  

### 3️⃣ **SSR 상태 모니터링**
- SSR이 **장시간 ON 상태일 경우** UART를 통해 경고 메시지 출력  

### 4️⃣ **LoRa 데이터 전송**
- 시스템 상태 및 에러 메시지를 **LoRa를 통해 원격 전송**  

---

## 🛠️ **하드웨어 핀 설정**

### 📌 **GPIO 설정**

| **기능**       | **핀 번호** | **설명**           |
|-----------------|------------|--------------------|
| **PIR 센서**   | PC6        | 움직임 감지        |
| **CDS 센서**   | PC7        | 조도 디지털 신호   |
| **CDS 아날로그** | PB0        | 조도 아날로그 값   |
| **SSR 제어**   | PB2        | 조명 ON/OFF 제어   |
| **LoRa M0**    | PB11       | LoRa 모드 핀       |
| **LoRa M1**    | PB12       | LoRa 모드 핀       |
| **LoRa UART_TX** | PD8        | UART TX (LoRa 송신) |
| **LoRa UART_RX** | PD9        | UART RX (LoRa 수신) |
| **GND**        | P5         | 공통 GND 연결      |
| **12V 전원**   | P5         | 시스템 전원 공급   |
| **조명 출력**  | P2         | TY-302 방수등 제어 |

### 📌 **UART 설정**

| **UART 인터페이스** | **TX 핀** | **RX 핀** | **기능**        |
|----------------------|---------|---------|----------------|
| **USART3 (LoRa)**    | PD8     | PD9     | LoRa 통신      |
| **USART1 (디버깅)**  | PA9     | PA10    | UART 디버깅    |

### 📌 **ADC 설정**

| **기능**       | **핀 번호** | **설명**        |
|-----------------|------------|---------------|
| **CDS 센서**   | PB0        | 아날로그 입력  |

### 📌 **전원 설정**

| **기능**      | **핀 번호** | **설명**    |
|---------------|------------|-----------|
| **SSR 전원**  | P1         | 220V 공급  |
| **GND/12V**   | P5         | GND/12V   |
| **조명 출력** | P2         | 방수등 출력 |

---

## 🔄 **시스템 동작 흐름**
1. **센서 상태 확인**  
   - PIR 및 CDS 센서 값을 읽고 UART로 출력  

2. **조명 제어**  
   - PIR 감지 및 CDS 조도 값에 따라 조명을 ON/OFF  

3. **에러 감지**  
   - CDS 값이 낮고 조명이 꺼져있으면 에러 메시지 전송  

4. **SSR 상태 모니터링**  
   - SSR이 장시간 ON일 경우 경고 메시지 전송  

5. **LoRa 전송**  
   - 상태 및 에러 메시지 LoRa로 송신  

---

## 📝 **주요 코드 설명**

### 🔹 **센서 데이터 읽기**
```c
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
### 🔹 **조명 제어**
```c
void Control_Light(void) {
    static uint32_t light_timer = 0;

    if (cds_analog_value > CDS_LIGHT_THRESHOLD) {
        if (pir_state == GPIO_PIN_RESET) {
            if (light_timer == 0) {
                light_timer = HAL_GetTick();
            } else if ((HAL_GetTick() - light_timer) >= LIGHT_OFF_DELAY) {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
                UART_SendString("[INFO] Light turned OFF after 5s due to no PIR detection.\r\n");
                light_timer = 0;
            }
        } else {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
            UART_SendString("[INFO] Light ON (CDS Bright + PIR Detected).\r\n");
            light_timer = 0;
        }
    } else {
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET) {
            UART_SendString("[ERROR] Low light detected, but light is OFF!\r\n");
            LoRa_SendData("[ERROR] Low light detected, but light is OFF!");
        }
    }
}
```
