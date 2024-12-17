# Microcontroller Embedded C Programming: Absolute Beginners 24-12-17

### 강의를 수강하며 공부한 내용 기록 / 섹션27 ~ x [강의 167 ~ 강의 188]

### 학습한 강의 내용 / 용어 정리  
- 아래 내용들의 실습 내용은 첨부된 코드를 확인해주세요

- 섹션별 코드 설명 및 사용된 주요 동작

# 요약
- **섹션 27**: 비트 필드를 사용하여 주변 장치 레지스터를 제어.
- **섹션 28**: 4x4 매트릭스 키패드의 인터페이스와 키 감지 로직 구현.
- **섹션 29**: 배열의 개념과 함수 전달 및 교환 프로그램 작성.
- **섹션 30**: 문자열의 입력, 출력 및 구조체를 사용한 데이터 관리.

# 섹션 27: 주변 레지스터와 비트 필드 

### 주요 내용
### typedef 구조체를 활용하여 주변 장치 레지스터를 구성하는 방법 학습.
### GPIO와 RCC 레지스터를 비트 필드 구조체로 구성하여 사용.
### 코드 동작 설명
### RCC_AHB2ENR 레지스터를 사용해 GPIO 클록 활성화.
### 구조체 기반으로 특정 비트 위치를 제어하는 방법을 구현.
```c
#include "main.h"

typedef struct {
    uint32_t gpioa_en : 1;
    uint32_t gpiob_en : 1;
    uint32_t gpioc_en : 1;
    uint32_t gpiod_en : 1;
    // 다른 비트 필드도 추가 가능
} RCC_AHB1ENR_t;

int main(void)
{
    RCC_AHB1ENR_t volatile *const pClkCtrlReg = (RCC_AHB1ENR_t*) 0x4002104C;
    pClkCtrlReg->gpiod_en = 1;  // GPIOD 클록 활성화
    for (;;);
}
```
