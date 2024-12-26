# 🛠️ **PIR 및 CDS 센서를 이용한 TY-302 조명 제어 + LoRa 통신 **

---

## 📚 **1. 프로젝트 개요**

**STM32L452RET6** MCU를 사용하여 **PIR 센서**, **CDS 센서**, **SSR 릴레이**, 및 **LoRa 통신 모듈**을 통해 조명을 제어하고 상태를 모니터링하는 시스템 
조명 상태와 센서 데이터를 실시간으로 UART 및 LoRa 통신을 통해 확인할 수 있다

---

## 🚀 **2. 주요 기능**

### 📊 **송신기 (Transmitter)**
- **PIR 센서 (PC6)**: 움직임 감지  
- **CDS 센서 (PC7, PB0)**: 빛 감지 (디지털, 아날로그)  
- **SSR 릴레이 (PB2)**: 220V 방수등 ON/OFF 제어  
- **LoRa 송신 (USART3)**: 센서 데이터를 주기적으로 송신  
- **UART 디버깅 (USART3)**: 상태 메시지 출력  

### 📡 **수신기 (Receiver)**
- **LoRa 수신 (USART3)**: 송신기에서 전송된 데이터 수신  
- **UART 디버깅 (USART3)**: 오류 상태 모니터링  
- **에러 검출**: 비정상적인 조명 상태 감지 및 알림  

---

## 💡 **3. 동작 방식**

1. **PIR 센서 감지**  
   - 움직임이 감지되면 SSR 릴레이를 통해 조명 ON 
2. **CDS 센서 확인**  
   - CDS 센서로 주변 밝기를 측정
3. **SSR 릴레이 제어**  
   - PIR 센서와 CDS 센서 데이터를 바탕으로 조명을 ON/OFF
4. **LoRa 데이터 전송**  
   - 센서 상태와 조명 상태를 LoRa를 통해 송신
5. **SSR 안정성 모니터링**  
   - SSR이 장시간 켜져 있으면 경고 메시지를 출력
6. **수신기 오류 감지**  
   - 수신기는 데이터를 분석해 비정상적인 상태를 UART를 통해 알림 

---

## 📝 **4. 하드웨어 연결**

### 🔌 **핀 매핑**

| **기능** | **MCU 핀** | **설명** |
|----------|-----------|---------|
| PIR 센서 | PC6       | 디지털 입력 |
| CDS 센서 | PC7 (디지털), PB0 (아날로그) | 빛 감지 |
| SSR 릴레이 | PB2       | 조명 제어 |
| LoRa M0 | PB11      | 모드 선택 |
| LoRa M1 | PB12      | 모드 선택 |
| LoRa TX | PB10      | 데이터 송신 |
| LoRa RX | PB9       | 데이터 수신 |

### ⚡ **전원 연결**

- **SSR 릴레이 (P1)**: AC220V 전원 연결  
- **MCU 전원 (P5)**: 12V DC 입력  

---

## 🛠️ **5. 개발 환경**

- **IDE:** STM32CubeIDE  
- **MCU:** STM32L452RET6  
- **Debugger:** ST-Link V2  
- **LoRa 모듈:** E22-900T22S  
- **UART 디버깅:** USB-to-TTL 어댑터  
- **전원 공급:** 12V DC 어댑터  

---

## 📝 **6. 코드 설명**

### 🔹 **메인 루프 (main.c)**

- **센서 읽기:** `Read_Sensors()`  
- **조명 제어:** `Control_Light()`  
- **LoRa 전송:** `LoRa_SendData()`  
- **에러 검출:** `Check_Combined_Error()`  
- **SSR 모니터링:** `Monitor_SSR_Status()`  
- **디버깅 출력:** `Debug_SensorStatus()`  

### 🔹 **LoRa 설정**

```c
LoRa_SendData("AT+CH=5");   // 채널 설정
LoRa_SendData("AT+PWR=20"); // 출력 전력 설정
LoRa_SendData("AT+SAVE");   // 설정 저장

