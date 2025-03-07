/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/*
사용하지 않는 변수를 컴파일러가 생성하지 않는 이유
volatile 키워드를 사용하여 컴파일러가 변수에 대한 최적화를 수행하지 않도록 강제할 수 있음
*/

/*
#include <stdint.h>

int main(void)
{
	uint8_t data1;
	uint8_t data2;

	data1 = 50;

	data2 = data1;

	data2 = data1;
	for(;;);
}
*/
/*
//volatile를 사용하여 프로그램 버그 수정
#include<stdint.h>

int main(void)
{
    volatile uint32_t *pClkCtrlReg      = (volatile uint32_t*)0x4002104C;  // RCC_AHB2ENR 주소
    volatile uint32_t *pPortCModeReg    = (volatile uint32_t*)0x48000800;  // GPIOC_MODER 주소
    volatile uint32_t *pPortCOutReg     = (volatile uint32_t*)0x48000814;  // GPIOC_ODR 주소
    volatile uint32_t *pPortAModeReg    = (volatile uint32_t*)0x48000000;  // GPIOA_MODER 주소
    volatile uint32_t *pPortAInReg      = (volatile uint32_t*)0x48000010;  // GPIOA_IDR 주소
    volatile uint32_t *pPortAPullUpDown = (volatile uint32_t*)0x4800000C;  // GPIOA_PUPDR 주소

    // 1. GPIO 클럭 활성화
    *pClkCtrlReg |= (1 << 3);  // GPIOCEN 비트 설정
    *pClkCtrlReg |= (1 << 0);  // GPIOAEN 비트 설정

    // 2. GPIOC 핀 13을 출력 모드로 설정
    *pPortCModeReg &= ~(3 << 26);  // 26, 27 비트를 클리어
    *pPortCModeReg |= (1 << 26);   // 출력 모드로 설정

    // 3. GPIOA 핀 0을 입력 모드로 설정
    *pPortAModeReg &= ~(3 << 0);   // GPIOA 핀 0을 입력 모드로 설정

    // 4. GPIOA 핀 0 풀업 설정
    *pPortAPullUpDown &= ~(3 << 0); // 클리어
    *pPortAPullUpDown |= (1 << 0);  // 풀업 설정

    // 5. 무한 루프
    while (1)
    {
        uint8_t pinStatus = (uint8_t)(*pPortAInReg & 0x1); // GPIOA 핀 0 상태 읽기
        if (pinStatus)
        {
            *pPortCOutReg |= (1 << 13);  // GPIOC 핀 13 HIGH (LED ON)
        }
        else
        {
            *pPortCOutReg &= ~(1 << 13); // GPIOC 핀 13 LOW (LED OFF)
        }
    }
}
*/
#include<stdint.h>
#include<stdio.h>

uint8_t g_button_pressed = 0;
uint32_t g_button_pressed_count = 0;

void button_init(void);

// EXTI(외부 인터럽트) 펜딩 레지스터 주소
uint32_t volatile *pEXTTIPendReg      = (uint32_t*) (0x40010400 + 0x14); // EXTI_PR1 (펜딩 레지스터)
// RCC(리셋 및 클럭 제어) AHB2 버스 클럭 활성화 레지스터
uint32_t volatile *pClkCtrlReg        = (uint32_t*) (0x40021000 + 0x4C); // RCC_AHB2ENR (GPIO 클럭 제어 레지스터)
// RCC APB2 버스 클럭 활성화 레지스터
uint32_t volatile *pClkCtrlRegApb2    = (uint32_t*) (0x40021000 + 0x60); // RCC_APB2ENR (APB2 클럭 제어 레지스터)
// GPIOC 모드 설정 레지스터 주소
uint32_t volatile *pGPIOCModeReg      = (uint32_t*) (0x48000800 + 0x00); // GPIOC_MODER (GPIOC 모드 설정)
// GPIOC 풀업/풀다운 설정 레지스터 주소
uint32_t volatile *pGPIOCPullReg      = (uint32_t*) (0x48000800 + 0x0C); // GPIOC_PUPDR (풀업/풀다운 설정)
// EXTI 인터럽트 마스크 레지스터 주소
uint32_t volatile *pEXTIMaskReg       = (uint32_t*) (0x40010400 + 0x00); // EXTI_IMR1 (인터럽트 마스크 레지스터)
// EXTI 상승 엣지 트리거 설정 레지스터 주소
uint32_t volatile *pEXTTIEdgeCtrlReg  = (uint32_t*) (0x40010400 + 0x08); // EXTI_RTSR1 (Rising Trigger 설정 레지스터)
// NVIC 인터럽트 활성화 레지스터 주소
uint32_t volatile *pNVICIRQEnReg      = (uint32_t*) 0xE000E100;          // NVIC_ISER (인터럽트 활성화 레지스터)

int main(void)
{
	button_init();

	while(1)
	{
		*pEXTIMaskReg &= ~(1 << 13); // GPIOC 핀 13 인터럽트 비활성화
		if(g_button_pressed){
			for(uint32_t i=0; i<500000/2; i++); // 디바운싱을 위한 딜레이
			g_button_pressed_count++;
			printf("Button is pressed : %lu\n", g_button_pressed_count);
			g_button_pressed = 0;
		}
		*pEXTIMaskReg |= (1 << 13); // GPIOC 핀 13 인터럽트 활성화
	}
}

void button_init(void)
{
	// GPIOC 및 SYSCFG 클럭 활성화
	*pClkCtrlReg |= (1 << 2);      // GPIOC 클럭 활성화
	*pClkCtrlRegApb2 |= (1 << 0);  // SYSCFG 클럭 활성화

	// GPIOC 핀 13을 입력 모드로 설정
	*pGPIOCModeReg &= ~(3 << 26);  // GPIOC 핀 13 모드 클리어 (입력 모드)

	// GPIOC 핀 13 풀업 설정
	*pGPIOCPullReg &= ~(3 << 26);  // GPIOC 핀 13 풀업/풀다운 클리어
	*pGPIOCPullReg |= (1 << 26);   // GPIOC 핀 13 풀업 설정

	// EXTI 설정: GPIOC 핀 13에 인터럽트 연결
	*pEXTTIEdgeCtrlReg |= (1 << 13); // 상승 엣지에서 트리거
	*pEXTIMaskReg |= (1 << 13);      // GPIOC 핀 13 인터럽트 활성화

	// NVIC 설정: EXTI15_10 인터럽트 활성화
	*pNVICIRQEnReg |= (1 << 6);      // EXTI15_10 인터럽트 활성화
}

void EXTI15_10_IRQHandler(void)
{
	g_button_pressed = 1;
	*pEXTTIPendReg |= (1 << 13); // GPIOC 핀 13 인터럽트 플래그 클리어
}
