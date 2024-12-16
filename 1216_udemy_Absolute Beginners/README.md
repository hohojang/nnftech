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
섹션 23: Optimization

컴파일러 최적화 옵션을 사용하여 성능을 최적화하는 방법을 설명합니다.
섹션 24: volatile

volatile를 사용하여 하드웨어와 인터럽트 관련 변수를 처리하는 방법을 배웁니다.
섹션 25: Structures and Bit fields

구조체와 비트 필드를 사용하여 효율적인 메모리 관리 방법을 배웁니다.
섹션 26: union

공용체를 사용하여 여러 데이터를 하나의 메모리 공간에 저장하고, 메모리 절약 방법을 익힙니다.
