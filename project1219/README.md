# STM32와 EBYTE E22900T22S를 사용한 SPI 통신 및 LED 제어 프로젝트

이 프로젝트는 STM32L452RET6 MCU와 **EBYTE E22900T22S LoRa 모듈**을 사용하여 **SPI 통신**을 통해 데이터를 송수신하고, **LED 상태**로 송수신 상태를 시각적으로 표시합니다.

---

## 📜 주요 기능
1. **LoRa 모듈 초기화**:
   - EBYTE E22900T22S 모듈의 RESET 핀을 제어하여 초기화합니다.
   - SPI 인터페이스를 통해 LoRa 모듈과 통신을 설정합니다.

2. **데이터 송신**:
   - LoRa 모듈의 FIFO 레지스터에 데이터를 기록한 후 송신 모드를 활성화합니다.
   - 데이터 송신 중 LED(PA8)가 **ON** 상태로 전환됩니다.

3. **데이터 수신**:
   - LoRa 모듈의 FIFO 레지스터에서 데이터를 읽어 수신합니다.
   - 데이터 수신 완료 시 LED 상태가 **토글**됩니다.

4. **DIO 핀 이벤트 처리**:
   - DIO 핀에서 발생한 인터럽트 이벤트를 감지하고 LED로 상태를 표시합니다.

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
     - SX1276/SX1278 칩 기반으로 동작.
     - SPI 인터페이스(NSS, MOSI, MISO, SCK)를 통해 데이터 송수신.
   - **GPIO 핀**:
     - **RESET**: 초기화를 위한 핀.
     - **DIO0~DIO5**: 송수신 완료 및 이벤트 발생 감지.
   - **전원 공급**:
     - +3.3V로 동작.
   - **RF 출력**:
     - 고출력(최대 1W, 22dBm) 지원.

### 3. **전원 공급**
   - **12V → 3.3V** 전환 회로(AZ1117H-3.3)로 LoRa 모듈과 MCU에 전원 공급.

---

## 💻 소프트웨어 구조
### 1. **`main.c`**
- **역할**:
  - MCU 초기화, E22900T22S와의 통신 제어.
  - 송수신 상태를 기반으로 LED 제어.
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
