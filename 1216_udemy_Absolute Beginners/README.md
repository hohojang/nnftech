# Microcontroller Embedded C Programming: Absolute Beginners 24-12-16

### 강의를 수강하며 공부한 내용 기록 / 섹션21 ~ 26 [강의 137 ~ 강의 166]

### 학습한 강의 내용 / 용어 정리  
### 아래 내용들의 실습 내용은 첨부된 코드를 확인해주세요

섹션별 코드 설명 및 사용된 주요 동작

# 섹션 21: Type qualifier 'const'  
#### 강의 137 ~ 139
- **const**는 변수를 수정할 수 없도록 만들어주는 키워드로, 변수의 값이 변경되지 않음을 보장합니다.
- **const**로 선언된 변수는 읽기 전용으로 취급되며, 컴파일러에 의해 최적화에서 제외됩니다.
- **const**를 사용하면 의도한 값이 변경되지 않도록 보호할 수 있습니다.

**예시 코드:**
```c
uint8_t const data = 10;
uint8_t *ptr = (uint8_t*)&data;
*ptr = 50;  // 수정이 불가능하여 컴파일 오류 발생
```

# 섹션 22: Pin-read
#### 강의 141 ~ 142
- **GPIO 핀 읽기**는 임베디드 시스템에서 핀 상태를 확인하고 제어하는 기본적인 작업입니다.
- GPIO를 통해 핀의 상태를 읽어와서 조건문으로 LED를 제어하는 예제를 작성합니다.
**예시 코드:**
```c
// GPIOA 핀 0을 읽어 GPIOC 핀 13의 LED를 제어하는 코드
uint8_t pinStatus = (uint8_t)(*pPortAInReg & 0x1);  // GPIOA 핀 0 상태 읽기
if (pinStatus) {
    *pPortCOutReg |= (1 << 13);  // GPIOC 핀 13 HIGH (LED ON)
} else {
    *pPortCOutReg &= ~(1 << 13); // GPIOC 핀 13 LOW (LED OFF)
}
```
# 섹션 23: Optimization(최적화 결과 이미지 첨부)
#### 강의 143 ~ 145
- **컴파일러 최적화를 이해**하고, 코드 성능을 개선하기 위한 최적화 수준 설정 방법을 배웁니다.
- 최적화 수준에 따른 코드 크기 및 성능 변화 예시를 분석합니다.
### 주요 내용:
### O0: 최적화가 전혀 적용되지 않음.
### O1: 최적화가 적용되어 코드 크기가 줄어들고 성능이 향상

# 섹션 24: 'volatile' type qualifier
#### 강의 146 ~ 151
- **volatile**는 변수의 값을 외부에서 변경할 수 있음을 나타내며, 최적화를 방지합니다.
- 인터럽트 서비스 루틴(ISR)과 같이 하드웨어 레지스터나 외부 장치와 상호작용하는 코드에서 중요합니다.
**예시 코드:**
```c
volatile uint32_t flag = 0;
void ISR_handler(void) {
    flag = 1;  // ISR에서 flag 값을 변경
}
```

# 섹션 25: Structures and Bit fields
#### 강의 152 ~ 159
- **Structure**는 여러 변수들을 하나의 묶음으로 관리할 수 있는 데이터 구조입니다.
- **Bit field**는 구조체 내에서 특정 비트 단위로 데이터를 저장할 수 있도록 도와줍니다.
## 주요 내용:
### 구조체의 크기 및 메모리 정렬 방식 이해
### 비트 필드를 사용하여 메모리 공간을 절약하고, 하드웨어 레지스터를 효율적으로 다루는 방법을 배웁니다.
**예시 코드 (구조체):**
```c
struct carModel {
    uint32_t carNumber;
    uint32_t carPrice;
    uint16_t carMaxSpeed;
    float carWeight;
};
```
**예시 코드 (비트필드):**
```c
struct Packet {
    uint32_t crc       : 2;
    uint32_t status    : 1;
    uint32_t payload   : 12;
    uint32_t bat       : 3;
    uint32_t sensor    : 3;
    uint32_t longAddr  : 8;
    uint32_t shortAddr : 2;
    uint32_t addMode   : 1;
};
```
# 섹션 26: union
#### 강의 160 ~ 166
- **Union**은 여러 데이터 타입이 동일한 메모리 위치를 공유하도록 하는 자료형입니다.
- Union은 주로 메모리 절약을 위해 사용되며, 하나의 변수에 여러 가지 타입을 담을 수 있습니다.
## 주요 내용:
### Union과 Structure의 차이점 이해
### Union을 사용하여 메모리를 효율적으로 관리하는 방법을 학습합니다.
**예시 코드:**
```c
union Packet {
    uint32_t packetValue;
    struct {
        uint32_t crc       : 2;
        uint32_t status    : 1;
        uint32_t payload   : 12;
        uint32_t bat       : 3;
        uint32_t sensor    : 3;
        uint32_t longAddr  : 8;
        uint32_t shortAddr : 2;
        uint32_t addMode   : 1;
    } packetFields;
};
```
```c
union Address {
    uint16_t shortAddr;
    uint32_t longAddr;
};
```
```c
union Address addr;

addr.shortAddr = 0xABCD;
addr.longAddr = 0xEEEECCCC;

printf("short addr = %#X\n", addr.shortAddr);
printf("long addr = %#X\n", addr.longAddr);
```
