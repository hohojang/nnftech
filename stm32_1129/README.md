# 2024-11-29 STM32

#STM32 펌웨어 기초 10-UART#4[USB to Serial]#
USB to Serial은 USB 인터페이스를 통해 연결된 장치와 직렬 포트를 통해 데이터를 전송할 수 있도록 하는 기능

이 프로그램에서는 **USB CDC (Communications Device Class)** 인터페이스를 사용하여, **USB 포트**와 **UART** (직렬 포트) 간에 데이터를 변환하는 역할

동작 흐름:

1. **USB 장치**에 연결된 컴퓨터나 다른 장치가 USB 인터페이스를 통해 데이터를 전송
2. 이 데이터는 USB CDC 프로토콜을 사용하여 STM32 마이크로컨트롤러로 전송
3. STM32는 USB CDC 인터페이스를 통해 받은 데이터를 **UART** (직렬 포트)로 전송
4. 반대로, UART로 받은 데이터는 USB CDC를 통해 USB 포트로 전송

### 기능 동작 흐름

1. **UART 데이터 수신**:
    - UART로부터 데이터를 수신하고, 이를 USB를 통해 전송 (`HAL_UART_Receive()` -> `CDC_Transmit_FS()`)
2. **USB 데이터 수신**:
    - USB CDC 인터페이스를 통해 데이터를 수신하면, 수신된 데이터를 UART로 전송하고, 동시에 CDC 버퍼에 데이터를 저장 (`CDC_Receive_FS()`)
3. **USB와 UART 간의 데이터 전송**:
    - UART에서 수신된 데이터를 USB로 전송하고, USB에서 수신된 데이터를 UART로 전송하는 형태로 통신이 이루어진다.
