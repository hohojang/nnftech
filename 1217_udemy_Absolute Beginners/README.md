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
## 섹션 27: 주변 레지스터와 비트 필드

### 주요 내용
- `typedef` 구조체를 활용하여 주변 장치 레지스터를 구성하는 방법 학습.
- GPIO와 RCC 레지스터를 비트 필드 구조체로 구성하여 사용.

### 코드 동작 설명
- `RCC_AHB1ENR` 레지스터를 사용해 GPIO 클록 활성화.
- 구조체 기반으로 특정 비트 위치를 제어하는 방법을 구현.

```c
#include "main.h"

// 비트 필드를 사용하여 주변 장치 레지스터를 구성
typedef struct {
    uint32_t gpioa_en : 1;
    uint32_t gpiob_en : 1;
    uint32_t gpioc_en : 1;
    uint32_t gpiod_en : 1;
    // 다른 비트 필드도 추가 가능
} RCC_AHB1ENR_t;

int main(void)
{
    // 구조체 포인터를 통해 RCC_AHB1ENR 레지스터 주소에 접근
    RCC_AHB1ENR_t volatile *const pClkCtrlReg = (RCC_AHB1ENR_t*) 0x4002104C;
    
    // GPIOD 클록 활성화
    pClkCtrlReg->gpiod_en = 1;  

    // 무한 루프
    for (;;);
}

```
## 섹션 28: 4x4 매트릭스 키패드 인터페이스

### 주요 내용
1. **키패드 구조**
   - 행(GPIO 출력), 열(GPIO 입력)으로 구성.
   - 풀업 저항 사용 시 미누름 상태에서 HIGH, 눌림 상태에서 LOW가 읽힘.
   
2. **로직 구현**
   - 행을 하나씩 LOW로 설정하고, 열의 상태를 검사하여 키 입력 확인.

### 코드 동작 설명
- `GPIOD` 레지스터를 사용하여 키패드의 행(출력)과 열(입력) 핀 설정.
- 각 행의 출력을 LOW로 만들고, 열의 상태를 검사하여 키 입력 확인.
- 입력에 풀업 저항 활성화.

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

    // GPIOD 클록 활성화
    *pClockCtrlReg |= (1 << 3);   
    *pGPIODModeReg &= ~(0xFF);    // PD0-PD3 출력 모드 설정
    *pGPIODModeReg |= 0x55;       // PD8-PD11 입력 모드
    *pPullupDownReg |= (0x55 << 16);

    while (1)
    {
        *pOutputDataReg |= 0x0F;  // 모든 행 HIGH 설정
        *pOutputDataReg &= ~(1 << 0);  // 첫 번째 행 LOW

        // 키 입력 확인
        if (!(*pInputDataReg & (1 << 8))) { printf("1\n"); delay(); }
        if (!(*pInputDataReg & (1 << 9))) { printf("2\n"); }
    }
}
```
## 섹션 29: 배열

### 주요 내용
- 배열의 기본 개념 및 초기화 방법 학습.
- 배열을 함수에 전달하고 출력하는 방법.
- 두 배열의 값을 교환하는 프로그램 구현.

### 코드 동작 설명
1. **배열 크기 및 주소 확인**
   - 배열의 크기와 시작 주소를 출력.
   
2. **배열 초기화 및 출력**
   - 모든 요소를 `0xFF`로 초기화하고 특정 인덱스 값을 변경.
   
3. **두 배열 교환**
   - 입력받은 두 배열의 내용을 교환하는 함수 구현.

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

    return 0;
}

// 배열을 출력하는 함수 정의
void display_array(int32_t *array, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

// 두 배열을 교환하는 함수 정의
void swap_arrays(int32_t *array1, int32_t *array2, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++) {
        int32_t temp = array1[i];
        array1[i] = array2[i];
        array2[i] = temp;
    }
}

```

## 섹션 30: 문자열 입력, 출력 및 구조체 사용한 데이터 관리

### 주요 내용
- **문자열의 기본 개념 학습**: 문자열은 널 문자('\0')로 끝나는 문자들의 배열로 정의됩니다.
- **문자열과 문자 배열의 차이점 이해**: 문자열은 문자 배열로 구현되지만, 문자 배열은 반드시 널 문자로 종료되어야 문자열로 간주됩니다.
- **공백이 포함된 문자열 입력 방법**: `scanf`와 `%[^\n]` 형식을 사용하여 공백이 포함된 문자열을 입력받을 수 있습니다.

### 코드 동작 설명
1. **구조체를 사용한 데이터 관리**
   - `STUDENT_INFO_t` 구조체를 정의하여 학생의 정보를 저장합니다.
   - `students` 배열에 여러 학생 정보를 저장하고 출력합니다.
   
2. **문자열 처리**
   - `name`, `branch`, `dob` 등의 문자열을 구조체 멤버로 선언하여 관리합니다.
   - 문자열을 입력받을 때 공백을 포함한 문자열을 입력받을 수 있도록 `%[^\n]` 형식을 사용합니다.

3. **구조체를 사용한 출력**
   - 각 학생의 정보를 구조체 멤버를 통해 출력합니다.

```c
#include<stdio.h>
#include<stdint.h>
#include<string.h>

// 학생 정보를 저장하기 위한 구조체 정의
typedef struct {
    int rollNumber;
    char name[100];
    char branch[50];
    char dob[15];
    int semester;
} STUDENT_INFO_t;

// 학생 정보 초기화
STUDENT_INFO_t students[2] = {
    {9876, "Ashok Kumar", "Mechanical", "12/11/1999", 7},
    {1234, "Ram Kumar", "Computer", "01/01/2000", 6}
};

// 학생들의 모든 정보를 출력하는 함수 정의
void display_all_records(STUDENT_INFO_t *records, int size);

int main()
{
    // 모든 학생 정보를 출력하는 함수 호출
    display_all_records(students, 2);
    return 0;
}

// 학생들의 모든 정보를 출력하는 함수 정의
void display_all_records(STUDENT_INFO_t *records, int size)
{
    for (int i = 0; i < size; i++) {
        printf("Roll Number: %d\n", records[i].rollNumber);
        printf("Name: %s\n", records[i].name);
        printf("Branch: %s\n", records[i].branch);
        printf("DOB: %s\n", records[i].dob);
        printf("Semester: %d\n\n", records[i].semester);
    }
}

```
