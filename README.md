# nnftech 2024-11-18 
# STM32 펌웨어 기초 link[https://www.youtube.com/watch?v=e6gILaoTkvs&list=PLvFHFPM09alKygQq-L6_6DwuNqTybIAw0&index=1]
# 가장 최소한의 프로젝트를 구성하기 위한 파일(mcu 프로그래밍) 
# 1. main.c 2. startup_stm32f103c8tx.s 3. LinkerScript파일

  #STM32 펌웨어 구조
  Application Layer (AP)
      ↓
  Hardware Layer (HW)
      ↓
  Board Support Package (BSP)
  
# AP (Application Layer)
실제 구현할 응용 프로그램 로직
다른 MCU로 이식할 때 수정이 필요 없는 부분 / ap.c, ap.h 파일에 구현

HW (Hardware Layer)
MCU의 주변장치 제어
하드웨어 의존적인 코드
hw.c, hw.h 파일에 구현

BSP (Board Support Package)
보드 초기화
기본적인 하드웨어 설정
bsp.c, bsp.h 파일에 구현


// 초기화 함수들 
void bspInit(void)  // 보드 초기화
void hwInit(void)   // 하드웨어 초기화
void apInit(void)   // 응용 프로그램 초기화

// 메인 루프
void apMain(void)   // 실제 응용 프로그램이 동작하는 부분

#프로젝트 구조의 장점
[1. 모듈화된 구조로 유지보수가 용이 2. 다른 MCU로 이식이 쉬움 3. 코드의 재사용성이 높음 4. H/W와 S/W의 명확한 분리]


