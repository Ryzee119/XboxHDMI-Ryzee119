// Copyright 2021, Ryan Wendland, XboxHDMI by Ryzee119
// SPDX-License-Identifier: MIT

#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f0xx_hal.h"
#include "main.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void EXTI4_15_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif
