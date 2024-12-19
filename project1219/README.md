# STM32와 EBYTE E22900T22S를 사용한 SPI 통신 및 LED 제어 

STM32L452RET6 MCU와 **EBYTE E22900T22S LoRa 모듈**을 사용하여 

**SPI 통신**을 통해 데이터를 송수신하고, **LED 상태**로 송수신 상태를 시각적으로 표시

---

## 📜 주요 기능
1. **LoRa 모듈 초기화**:
   - EBYTE E22900T22S 모듈의 RESET 핀을 제어하여 초기화
   - SPI 인터페이스를 통해 LoRa 모듈과 통신을 설정

2. **데이터 송신**:
   - LoRa 모듈의 FIFO 레지스터에 데이터를 기록한 후 송신 모드를 활성화
   - 데이터 송신 중 LED(PA8)가 **ON** 상태로 전환

3. **데이터 수신**:
   - LoRa 모듈의 FIFO 레지스터에서 데이터를 읽어 수신
   - 데이터 수신 완료 시 LED 상태가 **토글**된다

4. **DIO 핀 이벤트 처리**:
   - DIO 핀에서 발생한 인터럽트 이벤트를 감지하고 LED로 상태를 표시

5. **LED 제어**:
   - 송신 중: **LED ON**
   - 수신 완료: **LED 토글**

---

## 🛠️ 하드웨어 구성
### 1. **STM32L452RET6 (MCU)**
   - **SPI 핀 설정**:
     - **PA1**: SPI1_SCK (SPI 클럭 신호)
     - **PA4**: SPI1_NSS (Chip Select)
     - **PA6**: SPI1_MISO (데이터 입력)
     - **PA7**: SPI1_MOSI (데이터 출력)
   - **GPIO 핀 설정**:
     - **PA8**: LED 제어 (송수신 상태 표시)
     - **PB0**: LoRa 모듈 RESET 핀 제어
     - **PB2**: LoRa DIO 핀 입력

### 2. **EBYTE E22900T22S (LoRa 모듈)**
   - **SPI 통신**:
     - SX1276/SX1278 칩 기반으로 동작
     - SPI 인터페이스(NSS, MOSI, MISO, SCK)를 통해 데이터 송수신
   - **GPIO 핀**:
     - **RESET**: 초기화를 위한 핀.
     - **DIO0~DIO5**: 송수신 완료 및 이벤트 발생 감지
   - **전원 공급**:
     - +3.3V로 동작
   - **RF 출력**:
     - 고출력(최대 1W, 22dBm) 지원

### 3. **전원 공급**
   - **12V → 3.3V** 전환 회로(AZ1117H-3.3)로 LoRa 모듈과 MCU에 전원 공급

---

## 💻 소프트웨어 구조
### 1. **`main.c`**
- **역할**:
  - MCU 초기화, E22900T22S와의 통신 제어
  - 송수신 상태를 기반으로 LED 제어
- **핵심 루프**:
  ```c
  while (1)
  {
      // 데이터 송신
      LoRa_SendData(dataToSend, sizeof(dataToSend));
      LED_On();
      HAL_Delay(500);
      LED_Off();
      HAL_Delay(500);

      // DIO 핀 상태 확인
      LoRa_CheckDIO();

      // 데이터 수신
      LoRa_ReceiveData(receivedData, sizeof(receivedData));
      LED_Toggle();
      HAL_Delay(1000);
  }
  ```
### 2. **`LoRa.c`**
- **역할**:
- E22900T22S 모듈 초기화 및 제어(SPI 레지스터 읽기/쓰기).
- 데이터를 송수신하고 상태를 확인
- **핵심 함수**:
- LoRa_Init(): LoRa 모듈 초기화
- LoRa_SendData(): 데이터를 FIFO에 쓰고 송신
- LoRa_ReceiveData(): FIFO에서 데이터를 읽고 수신
- LoRa_CheckDIO(): DIO 핀 이벤트 감지


-------
## 🔧 사용 방법

### 1. 하드웨어 연결
EBYTE E22900T22S 모듈과 STM32 핀을 아래와 같이 연결:

| STM32 핀 | E22900T22S 핀 | 기능              |
|----------|---------------|-------------------|
| PA1      | SCK           | SPI 클럭          |
| PA4      | NSS           | SPI Chip Select   |
| PA6      | MISO          | SPI 데이터 입력   |
| PA7      | MOSI          | SPI 데이터 출력   |
| PB0      | RESET         | 모듈 리셋        |
| PB2      | DIO0          | 송수신 이벤트 감지|

---

### 2. CubeMX 설정
1. **SPI1 설정**:
   - Full-Duplex Master 모드로 설정
   - NSS 핀(Software 관리) 사용
2. **GPIO 설정**:
   - **PA8**: Output (LED 제어용)
   - **PB0**: Output (LoRa RESET 핀)
   - **PB2**: Input (LoRa DIO0 핀)
3. 코드 생성 후 `LoRa.c` 및 `main.c` 파일을 프로젝트에 추가

---

### 3. 펌웨어 업로드
1. STM32CubeIDE 또는 Keil을 사용하여 코드를 빌드
2. ST-LINK를 사용해 STM32에 펌웨어를 업로드

---

### 4. 동작 확인
1. STM32 보드와 LoRa 모듈을 전원에 연결
2. 데이터 송수신 중 LED(PA8)가 켜짐/꺼짐 상태를 표시
3. 디버깅 도구(UART 등)를 추가하면 송수신 데이터를 확인할 수 있다

---

## 📘 주요 파일 설명

### 1. **`main.c`**
- MCU 초기화, LoRa 모듈과의 통신 제어
- LED로 송수신 상태를 시각적으로 표시

### 2. **`LoRa.c`**
- EBYTE E22900T22S 모듈 초기화 및 제어(SPI 레지스터 읽기/쓰기)
- 데이터를 송수신하고 DIO 이벤트를 확인

---
