// Copyright 2021, Ryan Wendland, XboxHDMI by Ryzee119
// SPDX-License-Identifier: MIT

#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include "adv7511.h"
#include "interrupt.h"

extern adv7511 encoder;

void SysTick_Handler(void)
{

    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

void EXTI4_15_IRQHandler(void)
{
    encoder.interrupt = 1;
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    while (1)
        ;
}

void SVC_Handler(void)
{
}

void PendSV_Handler(void)
{
}