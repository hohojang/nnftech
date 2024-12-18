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
#include<stdio.h>
#include <stdint.h>

// 간단한 지연 함수, 시간을 소비하여 1초 간격을 만들어줍니다.
void delay(void)
{
   for(uint32_t i = 0; i < 300000; i++);  // 시간 지연 루프
}

int main(void)
{
   // GPIOD 레지스터 주소 설정
   uint32_t volatile *const pGPIODModeReg   = (uint32_t*)(0x48000C00 + 0x00); // GPIOD_MODER
   uint32_t volatile *const pInputDataReg   = (uint32_t*)(0x48000C00 + 0x10); // GPIOD_IDR (입력 데이터 레지스터)
   uint32_t volatile *const pOutputDataReg  = (uint32_t*)(0x48000C00 + 0x14); // GPIOD_ODR (출력 데이터 레지스터)
   uint32_t volatile *const pClockCtrlReg   = (uint32_t*)(0x40021000 + 0x4C); // RCC_AHB2ENR (GPIO 클록 활성화 레지스터)
   uint32_t volatile *const pPullupDownReg  = (uint32_t*)(0x48000C00 + 0x0C); // GPIOD_PUPDR (풀업/풀다운 레지스터)

   // 1. GPIOD 주변 장치의 클록 활성화
   *pClockCtrlReg |= (1<<3);  // GPIOD의 클록을 활성화 (비트 3을 1로 설정)

   // 2. PD0, PD1, PD2, PD3을 출력 모드로 설정 (행 설정)
   *pGPIODModeReg &= ~(0xFF);  // 기존 설정을 클리어
   *pGPIODModeReg |= 0x55;     // PD0~PD3을 출력 모드로 설정 (2비트씩 설정)

   // 3. PD8, PD9, PD10, PD11을 입력 모드로 설정 (열 설정)
   *pGPIODModeReg &= ~(0xFF << 16);  // PD8~PD11의 비트를 클리어

   // 4. PD8, PD9, PD10, PD11에 풀업 저항 활성화
   *pPullupDownReg &= ~(0xFF << 16);  // 기존 설정을 클리어
   *pPullupDownReg |=  (0x55 << 16);  // 풀업 설정 (2비트씩 설정)

   while(1)
   {
      // 1행을 HIGH로 설정
      *pOutputDataReg |= 0x0f;  // PD0~PD3을 HIGH로 설정

      // 1행의 첫 번째 핀 (PD0)을 LOW로 설정
      *pOutputDataReg &= ~(1 << 0);  // PD0을 LOW로 설정

      // 열을 스캔
      // 열 1(PD8) 상태 확인
      if(!(*pInputDataReg & (1 << 8))){  // PD8이 LOW라면 (키가 눌렸다면)
         delay();  // 키가 눌린 것을 확인한 후 지연
         printf("1\n");  // 키 1이 눌림
      }
      // 열 2(PD9) 상태 확인
      if(!(*pInputDataReg & (1 << 9))){  // PD9가 LOW라면
         printf("2\n");
      }
      // 열 3(PD10) 상태 확인
      if(!(*pInputDataReg & (1 << 10))){  // PD10이 LOW라면
         printf("3\n");
      }
      // 열 4(PD11) 상태 확인
      if(!(*pInputDataReg & (1 << 11))){  // PD11이 LOW라면
         printf("A\n");
      }

      // 1행을 다시 HIGH로 설정
      *pOutputDataReg |= 0x0f;  // PD0~PD3을 다시 HIGH로 설정
      // 2행을 LOW로 설정 (PD1)
      *pOutputDataReg &= ~(1 << 1);  // PD1을 LOW로 설정

      // 열을 스캔
      // 열 1(PD8) 상태 확인
      if(!(*pInputDataReg & (1 << 8))){  // PD8이 LOW라면
         delay();  // 키가 눌린 것을 확인한 후 지연
         printf("4\n");
      }
      // 열 2(PD9) 상태 확인
      if(!(*pInputDataReg & (1 << 9))){  // PD9가 LOW라면
         printf("5\n");
      }
      // 열 3(PD10) 상태 확인
      if(!(*pInputDataReg & (1 << 10))){  // PD10이 LOW라면
         printf("6\n");
      }
      // 열 4(PD11) 상태 확인
      if(!(*pInputDataReg & (1 << 11))){  // PD11이 LOW라면
         printf("B\n");
      }

      // 1행을 다시 HIGH로 설정
      *pOutputDataReg |= 0x0f;  // PD0~PD3을 다시 HIGH로 설정
      // 3행을 LOW로 설정 (PD2)
      *pOutputDataReg &= ~(1 << 2);  // PD2을 LOW로 설정

      // 열을 스캔
      // 열 1(PD8) 상태 확인
      if(!(*pInputDataReg & (1 << 8))){  // PD8이 LOW라면
         delay();  // 키가 눌린 것을 확인한 후 지연
         printf("7\n");
      }
      // 열 2(PD9) 상태 확인
      if(!(*pInputDataReg & (1 << 9))){  // PD9가 LOW라면
         printf("8\n");
      }
      // 열 3(PD10) 상태 확인
      if(!(*pInputDataReg & (1 << 10))){  // PD10이 LOW라면
         printf("9\n");
      }
      // 열 4(PD11) 상태 확인
      if(!(*pInputDataReg & (1 << 11))){  // PD11이 LOW라면
         printf("C\n");
      }

      // 1행을 다시 HIGH로 설정
      *pOutputDataReg |= 0x0f;  // PD0~PD3을 다시 HIGH로 설정
      // 4행을 LOW로 설정 (PD3)
      *pOutputDataReg &= ~(1 << 3);  // PD3을 LOW로 설정

      // 열을 스캔
      // 열 1(PD8) 상태 확인
      if(!(*pInputDataReg & (1 << 8))){  // PD8이 LOW라면
         delay();  // 키가 눌린 것을 확인한 후 지연
         printf("*\n");
      }
      // 열 2(PD9) 상태 확인
      if(!(*pInputDataReg & (1 << 9))){  // PD9가 LOW라면
         // 추가 코드 (키 2 출력 등)
      }
   }
}