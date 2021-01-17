// Copyright 2021, Ryan Wendland, XboxHDMI by Ryzee119
// SPDX-License-Identifier: MIT

#ifndef __MAIN_H__
#define __MAIN_H__

#define ADV7511_INT_Pin GPIO_PIN_9
#define ADV7511_INT_GPIO_Port GPIOA
#define ADV7511_INT_EXTI_IRQn EXTI4_15_IRQn
#define STATUS_LED GPIO_PIN_10

#ifdef __cplusplus
extern "C"
{
#endif

void _Error_Handler(char *, int);
#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

#ifdef __cplusplus
}
#endif

#endif