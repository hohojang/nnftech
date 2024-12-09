# STM32 펌웨어 기초 18|19-(FreeRTOS/FreeRTOS 제거)

## 강의 및 결과 영상 URL 
강의 : [https://www.youtube.com/watch?v=BHQUiLKhlmo]

결과 : [https://vimeo.com/1037375172?share=copy#t=0]

## 개요
이 프로젝트는 STM32에서 FreeRTOS를 사용하여 CLI(Command Line Interface)를 구현하는 예제

CLI는 사용자가 UART 인터페이스를 통해 명령어를 입력하고 처리할 수 있도록 함 

이 예제에서는 `USART2`와 `USART3`를 사용하여 데이터 송수신을 처리하며, FreeRTOS를 활용해 여러 작업을 동시에 실행

FreeRTOS의 장단점과 FreeRTOS를 제거하면 바뀌는 내용들에 관하여 다룸 

## FreeRTOS 장단점 
장점 : FreeRTOS는 멀티태스킹, 우선순위 기반 스케줄링, 리소스 관리 등의 강력한 기능을 제공

단점 : 코드 복잡성 증가, 디버깅의 어려움, 리소스 소비 등

요약 : 이 시스템을 사용하는 것이 유리한지 여부는 시스템의 복잡성, 요구되는 실시간 성능, 메모리 제한 등을 고려하여 결정해야한다.

## 사용된 하드웨어
- STM32L452RE(Nucleo-64 보드)
- `USART2`와 `USART3`를 통해 데이터 송수신
- FreeRTOS

## 주요 기능
- **FreeRTOS**: 멀티태스킹을 활용하여 CLI와 LED 토글 작업을 동시에 처리
- **CLI**: 사용자가 UART를 통해 명령어를 입력하면, 해당 명령어를 처리하고 결과를 출력
- **명령어**: 
  - `help`: 사용 가능한 명령어 목록을 표시
  - `md`: 랜덤 메시지를 표시
  - `hello`: 인사 메시지를 표시

## 설정
1. STM32CubeMX를 사용하여 `USART2`와 `USART3`를 설정
2. FreeRTOS를 활성화하고, CLI 및 LED 토글을 위한 두 개의 태스크를 설정
3. `USART2`와 `USART3`는 각각 다른 UART 포트를 사용하여 데이터를 송수신
   
## FreeRTOS 제거 
FreeRTOS가 제거되면, 프로젝트는 멀티태스킹을 사용하지 않고 단일 루프에서 모든 작업을 순차적으로 처리

이에 따라 타이밍 제어나 리소스 공유, 메모리 관리가 더 간단해지지만, 멀티태스킹의 이점(병렬 처리, 태스크 간 통신 등)을 사용 X

대신, 각 작업을 순차적으로 처리하며, 인터럽트 및 타이밍은 HAL 라이브러리 함수들로 대체

## 코드 설명

### 주요 함수
- **osKernelInitialize()**: FreeRTOS 커널 초기화
- **osThreadNew()**: 새로운 태스크 생성
- **osKernelStart()**: 커널 시작
- **osDelay()**: 태스크 지연 (주기적인 동작을 위해 사용)
- **UART_SendString()**: 문자열을 `USART2` 또는 `USART3`를 통해 송신
- **HAL_UART_RxCpltCallback()**: `USART2`로부터 데이터를 수신할 때마다 호출 / 수신된 데이터는 `rx_buffer`에 저장
- **ProcessCLICommand()**: 수신된 명령어를 처리 / 명령어에 따라 적절한 응답을 출력

### 주요 작업
- **CLI_Task**: 사용자 입력을 처리하고 명령어에 맞는 응답을 출력
- **LED_Task**: 프로그램 실행 시 1초에 1회씩 토글모드로 on/off


## FreeRTOS 주요 내용 요약

태스크 생성 및 실행: osThreadNew()로 여러 태스크를 생성하고, 각 태스크에 이름, 스택 크기, 우선순위 등을 지정

태스크 대기: osDelay()를 사용하여 태스크가 지정된 시간 동안 대기하게 하여, 주기적인 작업을 처리

커널 시작: osKernelStart()로 FreeRTOS 커널을 시작하여, 태스크들이 스케줄링되고 실행

태스크 간 상호작용: 큐나 세마포어를 통해 태스크 간 데이터 공유 및 동기화 가능

멀티태스킹 처리: FreeRTOS는 하나의 프로세서에서 여러 태스크를 시간 분할 방식으로 실행하여 멀티태스킹 환경을 구현

태스크 우선순위: 우선순위가 높은 태스크가 먼저 실행되며, osPriority로 우선순위를 관리

이와 같은 방법으로 FreeRTOS는 멀티태스킹 환경을 효율적으로 관리

### 명령어 처리 코드

```c
void ProcessCLICommand(char *command)
{
    // 명령어 처리
    if (strcasecmp(command, "help") == 0)
    {
        UART_SendString(&huart2, "-------- cmd list --------\r\n");
        UART_SendString(&huart2, "HELP - Show available commands\r\n");
        UART_SendString(&huart2, "MD - Display a random message\r\n");
        UART_SendString(&huart2, "HELLO - Display a hello message\r\n");
    }
    else if (strcasecmp(command, "md") == 0)
    {
        UART_SendString(&huart2, "This is a random message!\r\n");
    }
    else if (strcasecmp(command, "hello") == 0)
    {
        UART_SendString(&huart2, "Hello, MY NAME IS JANGHO PARK!\r\n");
    }
    else
    {
        UART_SendString(&huart2, "Unknown command\r\n");
    }
}


