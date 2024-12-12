## Microcontroller Embedded C Programming: Absolute Beginners 24-12-12

### 강의를 수강하며 공부한 내용 기록 / 섹션13 ~ 섹션16 [강의 78 ~ 강의 97]
### [섹션14 - 섹션15 개념 정리 강의입니다. 코드 x]

### 학습한 강의 내용 / 용어 정리 
### 아래 내용들의 실습 내용은 첨부된 코드를 확인해주세요
# 섹션 13: **포인터 (Pointers)**

## 79. **포인터의 읽기 및 쓰기 작업 (Read and Write Operation on Pointers)**  
- **포인터 (Pointer)**: 변수의 메모리 주소를 저장하는 변수로, 변수에 저장된 값을 직접 접근하거나 수정가능
- **읽기 작업 (Read Operation)**: 포인터가 가리키는 메모리 위치에서 값을 읽는 작업
- **쓰기 작업 (Write Operation)**: 포인터를 통해 가리키는 메모리 위치에 값을 저장하는 작업

## 80. **포인터 연습 구현 (Pointer Exercise Implementation)**  
- 포인터를 사용하여 메모리 주소를 처리하는 연습을 통해 포인터의 사용법과 중요성을 이해하고 실습

## 81. **포인터 데이터 타입의 중요성 (Significance of Pointer Data Types)**  
- 포인터는 데이터의 타입에 맞춰 메모리 주소를 저장
- **정수형 포인터 (int*)**, **문자형 포인터 (char*)** 등의 포인터 타입을 사용해야 하며, 잘못된 포인터 타입을 사용하면 데이터에 잘못 접근할 수 있다

## 82. **포인터 데이터 타입의 중요성 (Significance of Pointer Data Types)**  
- 포인터를 선언할 때 정확한 데이터 타입을 지정해야함
- 타입에 맞는 메모리 크기와 연산을 고려하여 올바른 타입의 포인터를 사용

---

# 섹션 14: **<stdint.h>의 중요성 (Importance of <stdint.h>)**

## 83. **<stdint.h>의 중요성 (Importance of <stdint.h>)**  
- **<stdint.h>**: C 언어에서 정수형 데이터의 크기를 명확히 정의하는 헤더 파일 
- 예를 들어, `int32_t`, `int64_t` 등의 타입을 사용하면 플랫폼에 관계없이 정확한 크기의 정수형 변수를 사용할 수 있다
- 이를 통해 이식성이 높은 코드 작성이 가능하며, 하드웨어와 소프트웨어 간의 데이터 처리에서 오류를 줄일 수 있다

---

# 섹션 15: **연산자 (Operators)**

## 84. **C 언어의 연산자 (Operators in 'C')**  
- C 언어에서 **연산자 (Operator)**는 변수와 상수에 대해 다양한 작업을 수행
- **산술 연산자 (Arithmetic Operators)**: `+`, `-`, `*`, `/`, `%` 등  
- **비교 연산자 (Relational Operators)**: `==`, `!=`, `>`, `<`, `>=`, `<=` 등  
- **논리 연산자 (Logical Operators)**: `&&`, `||`, `!` 등

## 85. **C 언어의 단항 연산자 (Unary Operator in 'C')**  
- **단항 연산자 (Unary Operator)**는 하나의 피연산자에 대해 작업을 수행하는 연산자
- 예를 들어, `++` (증가), `--` (감소), `!` (부정) 등이 있다

## 86. **포인터와 단항 연산자 (Unary Operators with Pointers)**  
- 포인터와 단항 연산자를 함께 사용하여 **포인터 연산**을 할 수 있다
- 예를 들어, `++p` 또는 `p++`를 사용하여 포인터가 가리키는 메모리 주소를 증가시킬 수 있다

## 87. **C 언어의 관계 연산자 (Relational Operators in 'C')**  
- **관계 연산자 (Relational Operators)**는 두 값 간의 **비교**를 수행
- 주요 관계 연산자로는 `==`, `!=`, `>`, `<`, `>=`, `<=` 등이 있다

## 88. **C 언어의 논리 연산자 (Logical Operators in 'C')**  
- **논리 연산자 (Logical Operators)**는 **조건식을 결합**하거나 **평가**하는 데 사용 
- 주요 논리 연산자로는 `&&` (AND), `||` (OR), `!` (NOT) 등이 있다

---

# 섹션 16: **의사결정 (Decision Making)**

## 89. **'if' 문 (if statement)**  
- **`if` 문**은 조건식이 참일 경우 해당 블록을 실행하는 조건문
- 조건식이 참이면 코드 블록을 실행하고, 그렇지 않으면 실행하지 않는다

## 90. **if 문 연습 (if statement exercise)**  
- `if` 문을 사용하는 실습을 통해 조건식을 평가하고 조건에 따른 프로그램 흐름 제어를 연습

## 91. **if 문 연습 해결책 (if statement exercise solution)**  
- 주어진 조건을 만족하는 코드 작성 방법을 `if` 문을 사용하여 해결한 예시

## 92. **'if' 와 'else' (if and 'else')**  
- `else`는 `if`와 함께 사용되어, 조건식이 **거짓**일 때 실행되는 코드 블록을 정의  
- `if`와 `else`는 조건이 참일 때와 거짓일 때 각각 다른 코드를 실행하는 데 사용

## 93. **'if' 와 'else' 연습 구현 1부 (if and 'else' exercise implementation part 1)**  
- `if`와 `else` 문을 사용하여 조건을 처리하는 연습을 진행

## 94. **'if' 와 'else' 연습 구현 2부 (if and 'else' exercise implementation part 2)**  
- 더 복잡한 조건문을 처리하며 `if`와 `else` 문을 활용하는 방법을 심화 실습

## 95. **'if' 와 'else' 연습 구현 3부 (if and 'else' exercise implementation part 3)**  
- `if`와 `else`의 결합을 통한 조건문 실습을 마무리하고, 다양한 조건을 다루는 방법을 연습

## 96. **'if-else-if' 사다리 구조 (if-else-if ladder statements)**  
- 여러 조건을 비교할 때 **`if-else-if` 구조**를 사용하여 여러 조건을 처리
- 각 조건을 순차적으로 검사하고, 조건이 참일 경우 해당 코드 블록을 실행

## 97. **'if-else-if' 사다리 구조 연습 (if-else-if ladder exercise)**  
- `if-else-if` 구조를 활용하여 **다양한 조건**을 처리하는 방법을 실습
- 여러 조건을 **효율적으로 평가**하고 처리하는 방법을 연습

