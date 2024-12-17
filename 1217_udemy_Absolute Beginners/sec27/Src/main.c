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

#include "main.h"

/*
    uint32_t *pClkCtrlReg   = (uint32_t*)0x4002104C;  // RCC_AHB2ENR 주소
    uint32_t *pPortCModeReg = (uint32_t*)0x48000800;  // GPIOC_MODER 주소
    uint32_t *pPortCOutReg  = (uint32_t*)0x48000814;  // GPIOC_ODR 주소 */


int main(void)
{
	RCC_AHB1ENR_t volatile*const pClkCtrlReg = (RCC_AHB1ENR_t*) 0x4002104C;
	// 1. GPIOC 주변 장치 클록 활성화(SET the 3rd bit position)
	pClkCtrlReg->gpiod_en = 1;

	for(;;);
}
