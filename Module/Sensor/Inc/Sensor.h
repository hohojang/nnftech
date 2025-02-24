#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

// PIR 센서 유지 시간 (ms)
#define PIR_HOLD_TIME       5000  // PIR 감지 후 LED 유지 시간 (5초)

// CDS 센서 임계값 (조도 값에 따라 LED 자동 제어)
#define CDS_LOW_THRESHOLD   400   // 어두운 환경 (LED ON)
#define CDS_HIGH_THRESHOLD  2800  // 밝은 환경 (LED OFF)

// 외부에서 참조할 센서 변수
extern uint8_t isPirDetected;
extern uint32_t cdsLightLevel;
extern uint8_t isLedOn;
extern uint32_t lastLedOnTime;

// 센서 데이터 읽기 및 제어 함수
void ReadSensors(void);
void ControlLED(void);

#endif  // SENSOR_H
