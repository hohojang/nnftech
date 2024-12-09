# STM32 펌웨어 기초 18 | 19 -(FreeRTOS, FreeRTOS 제거)

## 개요
이 프로젝트는 STM32에서 FreeRTOS를 사용하여 CLI(Command Line Interface)를 구현하는 예제입니다. CLI는 사용자가 UART 인터페이스를 통해 명령어를 입력하고 처리할 수 있도록 합니다. 이 예제에서는 `USART2`와 `USART3`를 사용하여 데이터 송수신을 처리하며, FreeRTOS를 활용해 여러 작업을 동시에 실행합니다. 또한, 명령어 처리 후 CLI 프롬프트가 반복적으로 출력됩니다.

## 사용된 하드웨어
- STM32L452RE(Nucleo-64 보드)
- `USART2`와 `USART3`를 통해 데이터 송수신
- FreeRTOS

## 주요 기능
- **FreeRTOS**: 멀티태스킹을 활용하여 CLI와 LED 토글 작업을 동시에 처리합니다.
- **CLI**: 사용자가 UART를 통해 명령어를 입력하면, 해당 명령어를 처리하고 결과를 출력합니다.
- **명령어**: 
  - `help`: 사용 가능한 명령어 목록을 표시합니다.
  - `md`: 랜덤 메시지를 표시합니다.
  - `hello`: 인사 메시지를 표시합니다.

## 설정
1. STM32CubeMX를 사용하여 `USART2`와 `USART3`를 설정합니다. 
2. FreeRTOS를 활성화하고, CLI 및 LED 토글을 위한 두 개의 태스크를 설정합니다.
3. `USART2`와 `USART3`는 각각 다른 UART 포트를 사용하여 데이터를 송수신합니다.

## 코드 설명

### 주요 함수
- **UART_SendString**: 문자열을 `USART2` 또는 `USART3`를 통해 송신합니다.
- **HAL_UART_RxCpltCallback**: `USART2`로부터 데이터를 수신할 때마다 호출됩니다. 수신된 데이터는 `rx_buffer`에 저장됩니다.
- **ProcessCLICommand**: 수신된 명령어를 처리합니다. 명령어에 따라 적절한 응답을 출력합니다.

### 주요 작업
- **CLI_Task**: 사용자 입력을 처리하고 명령어에 맞는 응답을 출력합니다. 또한, 매번 명령어 처리 후 `cli#` 프롬프트를 출력합니다.

### 코드 예시

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
        UART_SendString(&huart2, "Hello, welcome to the STM32 CLI!\r\n");
    }
    else
    {
        UART_SendString(&huart2, "Unknown command\r\n");
    }
}

