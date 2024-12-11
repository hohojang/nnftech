## Microcontroller Embedded C Programming: Absolute Beginners 24-12-11

### 강의를 수강하며 공부한 내용 기록 / 섹션7 ~ 섹션13 [강의 51 ~ 강의 78]

학습한 강의 내용 정리 / 용어 정리 

## 섹션 7: Functions

### 51. Typecasting in 'C'

### Typecasting: 데이터의 타입을 명시적으로 변환하는 과정.

### 예시:

### float x = 3.14;

### int y = (int)x;  // x를 int 타입으로 변환

### 52. Typecasting in 'C' (contd)

### 타입 변환 시 데이터 손실 가능성 설명.

### 정수와 실수 간 변환에서 소수점 이하 값 손실.

## 섹션 8: Microcontroller and Hello World

### 53. Embedded - 'Hello World'

### 임베디드 환경에서 printf를 사용한 간단한 "Hello World" 프로그램 실행.

### 54. Testing printf over ARM Cortex M4 ITM+SWO line

### ITM (Instrumentation Trace Macrocell): 디버깅을 위해 Cortex-M 프로세서가 제공하는 기능.

### SWO (Single Wire Output): 단일 와이어로 디버깅 데이터 출력.

### 55. Issues with IDE

### STM32CubeIDE 설정 문제 해결 방법 소개.

### 56. Testing printf over OpenOCD semihosting

### Semihosting: 호스트 시스템에서 I/O 작업을 처리하는 방식.

### OpenOCD를 사용한 semihosting 설정.

### 57. Run Sizeof on Embedded Target

### sizeof 연산자를 사용해 임베디드 시스템에서 데이터 크기 확인.

### 58. Sizeof Testing

### 다양한 데이터 타입(int, float, double)의 크기를 테스트.

### 59. Compiler Settings on STM32CubeIDE

### STM32CubeIDE에서 컴파일러 최적화와 빌드 설정.

## 섹션 9: Build Process

### 60. Embedded Project Build Process

### 빌드 단계:

### Preprocessing: 매크로와 헤더 파일 처리.

### Compilation: C 코드를 어셈블리로 변환.

### Assembly: 어셈블리 코드를 머신 코드로 변환.

### Linking: 개별 오브젝트 파일을 결합하여 실행 파일 생성.

## 섹션 10: Analyzing Embedded C Code

### 61. What is a Microcontroller?

### Microcontroller: CPU, 메모리, I/O를 포함한 소형 컴퓨터 시스템.

### 62. Code Memory of the Microcontroller

### 플래시 메모리와 RAM의 용도 설명.

### 63. Code and Data of the Program Using Memory Browser

### STM32CubeIDE의 메모리 브라우저를 사용해 프로그램 데이터 분석.

### 64. Analyzing ELF File Using GNU Tools

### ELF (Executable and Linkable Format): 실행 파일 포맷.

### readelf, objdump를 사용해 ELF 파일 분석.

### 65. Disassembly

### 어셈블리 수준에서 프로그램의 동작 분석.

### 66. IDE Option for Instruction-Level Debugging

### 명령어 단위 디버깅 방법과 IDE 옵션.

## 섹션 11: Data Types to Manipulate Floating Point Data

### 67. Manipulating Decimal Numbers in 'C'

### 소수점 데이터(float, double)의 처리 방법.

### 68. Single vs Double Precision

### Single Precision: 32비트, 더 적은 메모리 사용.

### Double Precision: 64비트, 더 높은 정밀도 제공.

### 69. Working with Float and Double Variables

### 실수 데이터 타입의 연산 및 출력.

## 섹션 12: Taking Input from the User Using scanf()

### 70. Scanf Introduction

### scanf 함수의 기본 사용법.

### 71-74. Scanf Exercises

### 다양한 데이터 타입 입력 및 처리 연습.

### 예시:

### int x;
### printf("Enter an integer: ");
### scanf("%d", &x);

### 75. Scanf and Float with Scientific Notation

### Scientific Notation:

### e 또는 E 표기법 사용.

### 예: 3.14e+2 (314.0)

## 섹션 13: Pointers

### 76. Pointers in 'C'

### 포인터의 기본 개념과 사용법.

### 포인터: 변수의 메모리 주소를 저장.

### 77. Pointer Variables and Pointer Data Types

### 포인터 변수 선언 및 초기화 방법.

### 78. Pointer Variables and Initialization

### 초기화되지 않은 포인터를 사용하는 경우의 문제점 설명.

## 용어 정리

### Typecasting: 데이터 타입 변환.

### ITM/SWO: ARM Cortex-M 디버깅 도구.

### Semihosting: 호스트 시스템에서 I/O를 처리하는 방식.

### ELF: 실행 파일 포맷.

### Single/Double Precision: 실수 데이터의 정밀도 수준.

### Pointer: 메모리 주소를 가리키는 변수.

