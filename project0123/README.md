# 송신부  
채널: 0x02
주소(Address): 0x04
UART 상태: 정상(HAL_OK), 데이터 송신 성공(tx_complete = 1).
전송 데이터: {0x01, 0x02, 0x03, 0x04}

# 수신부 
채널: 0x02
주소(Address): 0x05
UART 상태: HAL_TIMEOUT, 데이터 수신 실패(rx_complete = 0).
수신 데이터 버퍼: 비어 있음

# UART4 설정
|설정 항목 | 송신부 | 수신부 | 상태|
|---------|------|-------|---------|
|Baud Rate |	9600	|9600	| 일치 |
|Parity    |	None  |	None|	일치 |
|Stop Bits |	1	    |  1  |	일치 |
|Word Length|	8 bits|	8 bits|	일치|
|Flow Control|	None|None	| 일치|

# GPIO 설정 
|핀 이름 |	송신부    |	수신부    |	상태               | 
|--------|-----------|-----------|-------------------------|
|M1      |GPIOA_PIN_9|	GPIOA_PIN_9|	일치                  |
|M0	     |GPIOB_PIN_1|	GPIOB_PIN_1|	 일치                 |
|TX(UART)|	UART4_TX(PA0)|UART4_TX (PA0)|	일치|
|RX (UART)	|UART4_RX (PA1)	|UART4_RX (PA1)	|일치|
