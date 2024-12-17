/*
 * main.h
 *
 *  Created on: Dec 17, 2024
 *      Author: dwegw
 */

#ifndef MAIN_H_
#define MAIN_H_

#include<stdint.h>

// RCC_AHB1ENR 레지스터를 비트 필드를 사용하여 정의한 구조체
// 이 구조체는 GPIO, DMA, ETH 등 주변 장치의 클록을 활성화하는 데 사용됩니다.
typedef struct
{
    uint32_t gpioa_en       :1;  // GPIOA 클록 활성화
    uint32_t gpiob_en       :1;  // GPIOB 클록 활성화
    uint32_t gpioc_en       :1;  // GPIOC 클록 활성화
    uint32_t gpiod_en       :1;  // GPIOD 클록 활성화
    uint32_t gpioe_en       :1;  // GPIOE 클록 활성화
    uint32_t gpiof_en       :1;  // GPIOF 클록 활성화
    uint32_t gpiog_en       :1;  // GPIOG 클록 활성화
    uint32_t gpioh_en       :1;  // GPIOH 클록 활성화
    uint32_t gpioi_en       :1;  // GPIOI 클록 활성화

    uint32_t reserved1      :3;  // 예약된 비트 (사용되지 않음)
    uint32_t crc_en         :1;  // CRC 클록 활성화
    uint32_t reserved2      :3;  // 예약된 비트 (사용되지 않음)
    uint32_t reserved3      :2;  // 예약된 비트 (사용되지 않음)
    uint32_t bkpsram_en     :1;  // 백업 SRAM 클록 활성화
    uint32_t reserved4      :1;  // 예약된 비트 (사용되지 않음)
    uint32_t ccmdat_en      :1;  // Command Data 활성화
    uint32_t dma1_en        :1;  // DMA1 클록 활성화
    uint32_t dma2_en        :1;  // DMA2 클록 활성화
    uint32_t reserved5      :2;  // 예약된 비트 (사용되지 않음)
    uint32_t ethmac_en      :1;  // Ethernet MAC 클록 활성화
    uint32_t ethmactx_en    :1;  // Ethernet TX 활성화
    uint32_t ethmacrx_en    :1;  // Ethernet RX 활성화
    uint32_t ethmacptp_en   :1;  // Ethernet PTP 활성화
    uint32_t otghs_en       :1;  // OTG HS 클록 활성화
    uint32_t otghsulpi_en   :1;  // OTG HS ULPI 클록 활성화
} RCC_AHB1ENR_t;

// GPIO 출력 데이터 레지스터를 비트 필드를 사용하여 정의한 구조체
// 각 핀의 출력 상태를 설정하거나 읽을 때 사용됩니다.
typedef struct
{
    uint32_t pin_0          :1;  // GPIO 핀 0 출력 상태
    uint32_t pin_1          :1;  // GPIO 핀 1 출력 상태
    uint32_t pin_2          :1;  // GPIO 핀 2 출력 상태
    uint32_t pin_3          :1;  // GPIO 핀 3 출력 상태
    uint32_t pin_4          :1;  // GPIO 핀 4 출력 상태
    uint32_t pin_5          :1;  // GPIO 핀 5 출력 상태
    uint32_t pin_6          :1;  // GPIO 핀 6 출력 상태
    uint32_t pin_7          :1;  // GPIO 핀 7 출력 상태
    uint32_t pin_8          :1;  // GPIO 핀 8 출력 상태
    uint32_t pin_9          :1;  // GPIO 핀 9 출력 상태
    uint32_t pin_10         :1;  // GPIO 핀 10 출력 상태
    uint32_t pin_11         :1;  // GPIO 핀 11 출력 상태
    uint32_t pin_12         :1;  // GPIO 핀 12 출력 상태
    uint32_t pin_13         :1;  // GPIO 핀 13 출력 상태
    uint32_t pin_14         :1;  // GPIO 핀 14 출력 상태
    uint32_t pin_15         :1;  // GPIO 핀 15 출력 상태
    uint32_t reserved       :16; // 예약된 비트 (사용되지 않음)
} GPIOx_ODR_t;

// GPIO 모드 레지스터를 비트 필드를 사용하여 정의한 구조체
// 각 핀의 모드를 설정(입력, 출력 등)할 때 사용
typedef struct
{
    uint32_t pin_0          :2;  // GPIO 핀 0 모드
    uint32_t pin_1          :2;  // GPIO 핀 1 모드
    uint32_t pin_2          :2;  // GPIO 핀 2 모드
    uint32_t pin_3          :2;  // GPIO 핀 3 모드
    uint32_t pin_4          :2;  // GPIO 핀 4 모드
    uint32_t pin_5          :2;  // GPIO 핀 5 모드
    uint32_t pin_6          :2;  // GPIO 핀 6 모드
    uint32_t pin_7          :2;  // GPIO 핀 7 모드
    uint32_t pin_8          :2;  // GPIO 핀 8 모드
    uint32_t pin_9          :2;  // GPIO 핀 9 모드
    uint32_t pin_10         :2;  // GPIO 핀 10 모드
    uint32_t pin_11         :2;  // GPIO 핀 11 모드
    uint32_t pin_12         :2;  // GPIO 핀 12 모드
    uint32_t pin_13         :2;  // GPIO 핀 13 모드
    uint32_t pin_14         :2;  // GPIO 핀 14 모드
    uint32_t pin_15         :2;  // GPIO 핀 15 모드
} GPIOx_MODE_t;

#endif /* MAIN_H_ */
