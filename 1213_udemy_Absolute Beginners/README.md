## Microcontroller Embedded C Programming: Absolute Beginners 24-12-13

### 강의를 수강하며 공부한 내용 기록 / 섹션16_ ~ 섹션20 [강의 98 ~ 강의 136]

### 학습한 강의 내용 / 용어 정리 
### 아래 내용들의 실습 내용은 첨부된 코드를 확인해주세요

## 섹션 16: Decision Making

### 98. if-else-if Ladder Exercise Solution
- **if-else-if 구문**: 여러 조건을 차례로 검사하며, 해당 조건에 맞는 코드 블록을 실행하는 제어 구조.  
- **사용법**: `if (조건1) { ... } else if (조건2) { ... } else { ... }`

### 99. Conditional Operator
- **조건부 연산자 (`? :`)**: 간단한 조건문을 작성하는 연산자. `if-else` 구문을 간소화할 수 있음.  
  - 형식: `조건 ? 참일때 값 : 거짓일때 값`

### 100. Switch Case Statement in 'C'
- **switch-case**: 여러 조건을 처리할 때 `if-else` 보다 효율적인 방법으로 사용됨.  
  - `switch` 문은 주어진 값을 비교하여 일치하는 `case`로 분기함.
  - **예시**: 
    ```c
    switch (x) {
        case 1: ...; break;
        case 2: ...; break;
        default: ...;
    }
    ```

### 101. Switch Case Exercise
- `switch` 구문을 사용하여 주어진 조건을 만족하는 처리를 함.

### 102. Switch Case Exercise Solution
- `switch` 구문을 활용한 문제 해결법을 제공하여 실제 코드 구현을 돕는 코드 예시.

### 103. Switch Case Exercise Solution Continued
- 앞서 제시된 문제에 대한 추가 해결 방법과 코드 예시를 제공.

---

## 섹션 17: Bitwise Operators

### 104. Bitwise Operators in 'C'
- **비트 연산자**: 비트 단위로 연산을 수행하는 연산자.  
  - 연산자: `&` (AND), `|` (OR), `^` (XOR), `~` (NOT)  
  - 예시: `x & y`는 `x`와 `y`의 비트별 AND 연산을 수행.

### 105. Bitwise AND and Bitwise OR
- **비트별 AND (`&`)**: 두 값의 각 비트를 비교하여 모두 `1`일 때만 `1`을 반환함.  
- **비트별 OR (`|`)**: 두 값의 각 비트를 비교하여 하나라도 `1`이면 `1`을 반환함.

### 106. Applicability of Bitwise Operators: Testing of Bits
- **비트 테스트**: 비트 연산자를 사용하여 특정 비트의 상태를 확인할 수 있음.  
  - **예시**: `x & (1 << n)`은 `x`의 `n`번째 비트가 `1`인지 확인하는 방법.

### 107. Finding a Number Even or Odd Using Testing of Bits
- **짝수/홀수 판단**: 비트 연산을 사용하여 숫자가 짝수인지 홀수인지를 판단.  
  - **홀수**: 마지막 비트가 `1`임.  
  - **짝수**: 마지막 비트가 `0`임.

### 108. Applicability of Bitwise Operators: Setting of Bits
- **비트 설정**: 비트 연산자를 사용하여 특정 비트를 `1`로 설정.  
  - 예시: `x |= (1 << n)`은 `x`의 `n`번째 비트를 `1`로 설정.

### 109. Applicability of Bitwise Operators: Clearing of Bits
- **비트 클리어**: 비트 연산자를 사용하여 특정 비트를 `0`으로 설정.  
  - 예시: `x &= ~(1 << n)`은 `x`의 `n`번째 비트를 `0`으로 설정.

### 110. Applicability of Bitwise Operators: XOR
- **비트 XOR (`^`)**: 두 값의 비트를 비교하여 같으면 `0`, 다르면 `1`을 반환.  
  - **용도**: 값의 상태를 반전시킬 때 유용함.

---

## 섹션 18: Embedded C Coding Exercise for LED

### 111. Coding to Turn on an LED
- **LED 켜기**: I/O 포트를 설정하여 LED를 켬. 일반적으로 `GPIO` 레지스터를 설정하여 LED를 제어함.

### 112. Controlling IO Pin Using Software
- **소프트웨어로 I/O 핀 제어**: 소프트웨어를 사용하여 특정 I/O 핀을 `HIGH` 또는 `LOW`로 설정하여 하드웨어를 제어하는 방법.

### 113. Processor Addressable Memory Regions
- **프로세서 주소 지정 가능 메모리 영역**: 프로세서가 접근할 수 있는 메모리 주소 범위. 예를 들어, Flash, SRAM 등이 있음.

### 114. STM32 Memory Map
- **STM32 메모리 맵**: STM32 마이크로컨트롤러의 메모리 배치를 나타내며, 각 메모리 영역의 주소를 정의함.

### 115. Memory Mapped Peripheral Registers and IO Access
- **메모리 맵 레지스터 및 I/O 액세스**: 메모리 맵을 통해 주변 장치의 레지스터에 접근하고 제어하는 방법.

### 116. Procedure to Turn ON the LED
- **LED 켜는 절차**: LED를 켜는 방법에 대해 설명하며, 포트 및 레지스터 설정을 다룸.

### 117. Enabling Peripheral Clock
- **주변 장치 클럭 활성화**: 특정 하드웨어 주변 장치가 작동하려면 해당 장치의 클럭을 활성화해야 함.

### 118. Calculating Peripheral Register Addresses
- **주변 장치 레지스터 주소 계산**: 주변 장치 레지스터의 주소를 계산하여 프로그램에서 액세스함.

### 119. LED ON Exercise Coding
- **LED ON 연습 코드**: LED를 켜는 코드를 실습하며, 하드웨어 제어에 대해 학습함.

---

## 섹션 19: Bitwise Shift Operators

### 120. Bitwise Right Shift Operator
- **우측 시프트 연산자 (`>>`)**: 비트를 오른쪽으로 이동시키는 연산자로, 숫자를 `2`로 나누는 효과를 제공.

### 121. Bitwise Left Shift Operator
- **좌측 시프트 연산자 (`<<`)**: 비트를 왼쪽으로 이동시키는 연산자로, 숫자를 `2`로 곱하는 효과를 제공.

### 122. Applicability of Bitwise Shift Operators
- **비트 시프트 연산자 활용**: 숫자 값을 빠르게 곱하거나 나누는 데 유용하며, 비트 연산자와 함께 사용됨.

### 123. Modifying LED On Exercise Using Bitwise Shift Operators
- **LED ON 연습에서 비트 시프트 연산자 사용**: 비트 시프트 연산자를 이용해 LED 제어 방법을 학습.

### 124. Bit Extraction
- **비트 추출**: 특정 비트를 추출하여 해당 값을 사용하거나 검사하는 방법을 학습.

---

## 섹션 20: Looping

### 125. Looping in 'C'
- **반복문**: 코드의 특정 블록을 반복해서 실행하는 구문. `for`, `while`, `do-while` 등을 사용하여 구현할 수 있음.

### 126. While Loop
- **while 반복문**: 조건이 참인 동안 반복 실행하는 구문.
  - 형식: `while (조건) { ... }`

### 127. While Loop Exercise
- `while` 반복문을 이용한 실습 문제.

### 128. While Loop and Semicolon
- `while` 반복문과 세미콜론 사용에 대해 설명함.

### 129. While Loop Exercise
- `while` 반복문을 이용한 또 다른 실습 문제를 제공.

### 130. 'Do While' Loop
- **do-while 반복문**: 반복 조건을 마지막에 검사하며, 적어도 한 번은 실행되는 반복문.
  - 형식: `do { ... } while (조건);`

### 131. 'For' Loop
- **for 반복문**: 반복 횟수가 미리 정해져 있는 경우에 유용한 반복문.
  - 형식: `for (초기화; 조건; 증감) { ... }`

### 132. 'For' Loop Exercise
- `for` 반복문을 이용한 실습 문제.

### 133. 'For' Loop Number Pyramid Exercise
- `for` 반복문을 사용하여 숫자 피라미드를 출력하는 문제.

### 134. 'For' Loop Exercise Solution
- `for` 반복문을 이용한 문제의 해결 방법을 제공.

### 135. Modifying LED On with Software Delay
- **소프트웨어 지연**: 코드 내에서 지연을 추가하여 LED 상태를 조정하는 방법을 학습.

### 136. LED Toggle Using Software Delay
- **LED 토글**: LED의 상태를 주기적으로 변경하는 방법으로, 소프트웨어 지연을 사용함.
