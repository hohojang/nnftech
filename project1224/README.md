# STM32L452RET6 - PIR 및 CDS 센서 데이터 UART 출력 

# 결과 url : [https://vimeo.com/1041916083?share=copy#t=0]

##  1. 하드웨어 구성

### 센서 및 핀 매핑

| **기능**      | **핀** | **설정**       | **모드**       | **Pull-up/Pull-down** |
|---------------|--------|---------------|---------------|----------------------|
| **PIR 센서**  | **PC6** | **GPIO_Input** | Input mode    | Pull-down           |
| **CDS AO**    | **PB0** | **ADC1_IN15**  | Analog mode   | No Pull             |
| **CDS DO**    | **PC7** | **GPIO_Input** | Input mode    | No Pull             |
| **USART3 TX** | **PC4** | **USART3_TX**  | Alternate Function | -             |
| **USART3 RX** | **PC5** | **USART3_RX**  | Alternate Function | -             |

---

## 2. CubeMX 설정

### ADC1 설정 (CDS AO)  
- **Instance:** ADC1  
- **Channel:** IN15 (PB0)  
- **Resolution:** 12 bits  
- **Data Alignment:** Right Alignment  
- **Continuous Conversion Mode:** Disable  
- **External Trigger:** Software Trigger  
- **Sampling Time:** 2.5 cycles  

### USART3 설정 (터미널 출력)  
- **USART Instance:** USART3  
- **TX Pin:** PC4  
- **RX Pin:** PC5  
- **Baud Rate:** 115200  
- **Word Length:** 8 bits  
- **Stop Bits:** 1  
- **Parity:** None  
- **Mode:** TX/RX  

### GPIO 설정

**PC6 (PIR 센서)**  
- **Mode:** Input  
- **Pull-up/Pull-down:** Pull-down  

**PC7 (CDS DO 센서)**  
- **Mode:** Input  
- **Pull-up/Pull-down:** No Pull  

**PB0 (CDS AO 센서)**  
- **Mode:** Analog (ADC Input)  
- **Pull-up/Pull-down:** No Pull  

**USART3 (PC4, PC5)**  
- **Alternate Function:** AF7 (USART3)  

---

## 3. 소프트웨어 기능

### 3.1. 센서 데이터 읽기
- **PIR 센서 (PC6):** 모션 감지 (`HIGH`/`LOW`)  
- **CDS 센서 AO (PB0):** ADC 값을 통해 조도 데이터 확인  
- **CDS 센서 DO (PC7):** 임계값 초과 여부 확인 (`HIGH`/`LOW`)  

### 3.2. UART 데이터 출력
- UART를 통해 센서 데이터를 주기적으로 출력

**UART 출력 :**
```plaintext
--- Sensor Data ---
PIR Sensor   : MOTION
CDS AO Value : 1234
CDS DO State : HIGH
--------------------

