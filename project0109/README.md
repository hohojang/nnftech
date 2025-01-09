## 송신부 - > PC 
### 디시전 테이블 
| PIR 감지       | LED 상태 |  CDS 값        | 에러      |       송신 메시지                                                   |
|------------|-------------|----------------|-----------|--------------------------------------------------------------------|
|     1      |     1       |   정상          |   X       | "LED ON" (LED가 켜졌음을 알림)                                     |
|     0      |     0       |   정상          |   X       |"LED OFF" (LED가 꺼졌음을 알림)                                      |
|     1      |     1       |   정상          |   O       |"Error: LED stuck ON" (LED가 꺼지지 않음)                            |
|      0     |     0       |cds_value < 500  |   O       |"Error: CDS value too low" (조도가 너무 낮음)                        |
|      0     |     0       |cds_value > 3500 |   O       |"Error: CDS value too high" (조도가 너무 밝음)                       |
|      1     |     1       |cds_value < 500  |   O       |"Error: LED ON and CDS value too low" (LED 켜짐 + 조도가 너무 낮음)   |
|     1      |     1       |cds_value > 3500 |   O       |"Error: LED ON and CDS value too high" (LED 켜짐 + 조도가 너무 밝음)  |
|     0      |     0       |    정상         |    X      |"All OFF" (PIR 감지 없음 + LED 꺼짐 + 정상 범위)                      |






## 수신부 - > 노트북 
| 메시지       | LED 상태  |  에러    | 동작       |  
|------------|-----------|-----------|-----------|
| "LED ON"   | 1          |X      |     -      | LED가 켜졌음을 확인 후 로그 출력    |
|"LED OFF"    | 0         | X    | 타이머 시작 | LED가 꺼졌음을 확인 후 로그 출력|
| "Error: LED stuck ON"| 미감지(0)| O    | 4초 경과    | LED가 꺼지지 않음을 에러로 처리  |
|"Error: CDS value too low"| - | O      |CDS 값이 너무 낮음을 에러로 처리 |
|"Error: CDS value too high"|-| O   |CDS 값이 너무 높음을 에러로 처리 |
|Unknown 메시지| - | - | 알 수 없는 메시지 로그 출력|
