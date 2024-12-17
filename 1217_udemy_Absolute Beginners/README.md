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

----
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
# 섹션 28: 4x4 매트릭스 키패드 인터페이스

### 주요 내용
## 1. 키패드 구조
### 행(GPIO 출력), 열(GPIO 입력)으로 구성.
### 풀업 저항 사용 시 미누름 상태에서 HIGH, 눌림 상태에서 LOW가 읽힘.
## 2. 로직 구현
### 행을 하나씩 LOW로 설정하고, 열의 상태를 검사하여 키 입력 확인.
### 코드 동작 설명
### GPIOD 레지스터를 사용하여 키패드의 행(출력)과 열(입력) 핀 설정.
### 각 행의 출력을 LOW로 만들고, 열의 상태를 검사하여 키 입력 확인.
### 입력에 풀업 저항 활성화.
```c
#include<stdio.h>
#include<stdint.h>

void delay(void) { for (uint32_t i = 0; i < 300000; i++); }

int main(void)
{
    uint32_t volatile *const pGPIODModeReg   = (uint32_t*)(0x48000C00);
    uint32_t volatile *const pInputDataReg   = (uint32_t*)(0x48000C10);
    uint32_t volatile *const pOutputDataReg  = (uint32_t*)(0x48000C14);
    uint32_t volatile *const pClockCtrlReg   = (uint32_t*)(0x4002104C);
    uint32_t volatile *const pPullupDownReg  = (uint32_t*)(0x48000C0C);

    *pClockCtrlReg |= (1 << 3);   // GPIOD 클록 활성화
    *pGPIODModeReg &= ~(0xFF);    // PD0-PD3 출력 모드 설정
    *pGPIODModeReg |= 0x55;       // PD8-PD11 입력 모드
    *pPullupDownReg |= (0x55 << 16);

    while (1)
    {
        *pOutputDataReg |= 0x0F;  // 모든 행 HIGH 설정
        *pOutputDataReg &= ~(1 << 0);  // 첫 번째 행 LOW

        if (!(*pInputDataReg & (1 << 8))) { printf("1\n"); delay(); }
        if (!(*pInputDataReg & (1 << 9))) { printf("2\n"); }
    }
}
```
# 섹션 29: 배열 

# 주요 내용
### 배열의 기본 개념 및 초기화 방법 학습.
### 배열을 함수에 전달하고 출력하는 방법.
### 두 배열의 값을 교환하는 프로그램 구현.
# 코드 동작 설명
### 1. 배열 크기 및 주소 확인
배열의 크기와 시작 주소를 출력.
### 2. 배열 초기화 및 출력
모든 요소를 0xFF로 초기화하고 특정 인덱스 값을 변경.
### 3. 두 배열 교환
입력받은 두 배열의 내용을 교환하는 함수 구현.
```c
#include<stdio.h>
#include<stdint.h>

// 배열을 출력하는 함수 선언
void display_array(int32_t *array, uint32_t size);

// 두 배열을 교환하는 함수 선언
void swap_arrays(int32_t *array1, int32_t *array2, uint32_t size);

int main(void)
{
    // 배열 초기화
    int32_t array1[] = {1, 2, 3};
    int32_t array2[] = {4, 5, 6};

    // 배열 교환 전 출력
    printf("Before swap:\n");
    display_array(array1, 3);  // array1 출력
    display_array(array2, 3);  // array2 출력

    // 배열 교환
    swap_arrays(array1, array2, 3);

    // 배열 교환 후 출력
    printf("After swap:\n");
    display_array(array1, 3);  // 교환된 array1 출력
    display_array(array2, 3);  // 교환된 array2 출력
}

// 배열의 각 요소를 출력하는 함수 정의
void display_array(int32_t *array, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {
        printf("%d ", array[i]);  // 배열의 각 원소 출력
    }
    printf("\n");
}

// 두 배열의 값을 교환하는 함수 정의
void swap_arrays(int32_t *array1, int32_t *array2, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {
        int32_t temp = array1[i];  // array1의 현재 원소를 temp에 저장
        array1[i] = array2[i];     // array2의 원소를 array1에 복사
        array2[i] = temp;          // temp에 저장된 array1의 원소를 array2에 복사
    }
}

```
# 섹션 30: 문자열 

# 주요 내용
### 문자열의 기본 개념 학습 (널 문자 '\0').
### 문자열과 문자 배열의 차이점 이해.
### 공백이 포함된 문자열 입력 방법(scanf와 %[^\n] 사용).
# 코드 동작 설명
## 1. 문자열 입력
### scanf를 사용해 문자열을 입력받고 출력.
## 2. 공백이 포함된 문자열
### %[^\n] 포맷을 사용하여 문자열을 공백 포함 입력받음.
## 3. 구조체 활용
### 학생 정보를 저장하고 출력하는 프로그램 작성.
