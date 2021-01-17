/**
  ******************************************************************************
  * @file    stm32f0xx_hal_conf.h
  * @brief   HAL configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

#ifndef __STM32F0xx_HAL_CONF_H
#define __STM32F0xx_HAL_CONF_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

#define HAL_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#define HAL_SMBUS_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
/*#define HAL_ADC_MODULE_ENABLED*/
/*#define HAL_CRYP_MODULE_ENABLED*/
/*#define HAL_CAN_MODULE_ENABLED*/
/*#define HAL_CEC_MODULE_ENABLED*/
/*#define HAL_COMP_MODULE_ENABLED*/
/*#define HAL_CRC_MODULE_ENABLED*/
/*#define HAL_CRYP_MODULE_ENABLED*/
/*#define HAL_TSC_MODULE_ENABLED*/
/*#define HAL_DAC_MODULE_ENABLED*/
/*#define HAL_I2S_MODULE_ENABLED*/
/*#define HAL_IWDG_MODULE_ENABLED*/
/*#define HAL_LCD_MODULE_ENABLED*/
/*#define HAL_LPTIM_MODULE_ENABLED*/
/*#define HAL_RNG_MODULE_ENABLED*/
/*#define HAL_RTC_MODULE_ENABLED*/
/*#define HAL_SPI_MODULE_ENABLED*/
/*#define HAL_TIM_MODULE_ENABLED*/
/*#define HAL_USART_MODULE_ENABLED*/
/*#define HAL_IRDA_MODULE_ENABLED*/
/*#define HAL_SMARTCARD_MODULE_ENABLED*/
/*#define HAL_WWDG_MODULE_ENABLED*/
/*#define HAL_PCD_MODULE_ENABLED*/

/* 
 * Value of the External oscillator in Hz
*/
#if !defined(HSE_VALUE)
#define HSE_VALUE ((uint32_t)8000000)
#endif

/* 
 * Time out for HSE start up, in ms
*/
#if !defined(HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT ((uint32_t)100)
#endif

/* 
 * Value of the Internal oscillator in Hz
*/
#if !defined(HSI_VALUE)
#define HSI_VALUE ((uint32_t)8000000)
#endif

/* 
 * Time out for HSI start up
*/
#if !defined(HSI_STARTUP_TIMEOUT)
#define HSI_STARTUP_TIMEOUT ((uint32_t)5000)
#endif

/* 
 * Value of the Internal High Speed oscillator for ADC in Hz.
 * The real value may vary depending on the variations
 * in voltage and temperature.
*/
#if !defined(HSI14_VALUE)
#define HSI14_VALUE ((uint32_t)14000000)
#endif

/* 
 * Value of the Internal High Speed oscillator for USB in Hz.
 * The real value may vary depending on the variations
 * in voltage and temperature.
*/
#if !defined(HSI48_VALUE)
#define HSI48_VALUE ((uint32_t)48000000)
#endif

/* 
 * Value of the Internal Low Speed oscillator in Hz
 * The real value may vary depending on the variations
 * in voltage and temperature.
*/
#if !defined(LSI_VALUE)
#define LSI_VALUE ((uint32_t)40000)
#endif

/* 
 * Value of the External Low Speed oscillator in Hz
*/
#if !defined(LSE_VALUE)
#define LSE_VALUE ((uint32_t)32768)
#endif

/*
 * Time out for LSE start up, in ms
*/
#if !defined(LSE_STARTUP_TIMEOUT)
#define LSE_STARTUP_TIMEOUT ((uint32_t)5000)
#endif

/*
 * Value of VDD in mv
*/
#define VDD_VALUE ((uint32_t)3300)

/*
 * tick interrupt priority (lowest by default)
*/
#define TICK_INT_PRIORITY ((uint32_t)0)

#define USE_RTOS 0
#define PREFETCH_ENABLE 1
#define INSTRUCTION_CACHE_ENABLE 0
#define DATA_CACHE_ENABLE 0
#define USE_SPI_CRC 0U

#ifdef HAL_RCC_MODULE_ENABLED
#include "stm32f0xx_hal_rcc.h"
#endif /* HAL_RCC_MODULE_ENABLED */

#ifdef HAL_GPIO_MODULE_ENABLED
#include "stm32f0xx_hal_gpio.h"
#endif /* HAL_GPIO_MODULE_ENABLED */

#ifdef HAL_DMA_MODULE_ENABLED
#include "stm32f0xx_hal_dma.h"
#endif /* HAL_DMA_MODULE_ENABLED */

#ifdef HAL_CORTEX_MODULE_ENABLED
#include "stm32f0xx_hal_cortex.h"
#endif /* HAL_CORTEX_MODULE_ENABLED */

#ifdef HAL_ADC_MODULE_ENABLED
#include "stm32f0xx_hal_adc.h"
#endif /* HAL_ADC_MODULE_ENABLED */

#ifdef HAL_CAN_MODULE_ENABLED
#include "stm32f0xx_hal_can.h"
#endif /* HAL_CAN_MODULE_ENABLED */

#ifdef HAL_CEC_MODULE_ENABLED
#include "stm32f0xx_hal_cec.h"
#endif /* HAL_CEC_MODULE_ENABLED */

#ifdef HAL_COMP_MODULE_ENABLED
#include "stm32f0xx_hal_comp.h"
#endif /* HAL_COMP_MODULE_ENABLED */

#ifdef HAL_CRC_MODULE_ENABLED
#include "stm32f0xx_hal_crc.h"
#endif /* HAL_CRC_MODULE_ENABLED */

#ifdef HAL_DAC_MODULE_ENABLED
#include "stm32f0xx_hal_dac.h"
#endif /* HAL_DAC_MODULE_ENABLED */

#ifdef HAL_FLASH_MODULE_ENABLED
#include "stm32f0xx_hal_flash.h"
#endif /* HAL_FLASH_MODULE_ENABLED */

#ifdef HAL_I2C_MODULE_ENABLED
#include "stm32f0xx_hal_i2c.h"
#endif /* HAL_I2C_MODULE_ENABLED */

#ifdef HAL_I2S_MODULE_ENABLED
#include "stm32f0xx_hal_i2s.h"
#endif /* HAL_I2S_MODULE_ENABLED */

#ifdef HAL_IRDA_MODULE_ENABLED
#include "stm32f0xx_hal_irda.h"
#endif /* HAL_IRDA_MODULE_ENABLED */

#ifdef HAL_IWDG_MODULE_ENABLED
#include "stm32f0xx_hal_iwdg.h"
#endif /* HAL_IWDG_MODULE_ENABLED */

#ifdef HAL_PCD_MODULE_ENABLED
#include "stm32f0xx_hal_pcd.h"
#endif /* HAL_PCD_MODULE_ENABLED */

#ifdef HAL_PWR_MODULE_ENABLED
#include "stm32f0xx_hal_pwr.h"
#endif /* HAL_PWR_MODULE_ENABLED */

#ifdef HAL_RTC_MODULE_ENABLED
#include "stm32f0xx_hal_rtc.h"
#endif /* HAL_RTC_MODULE_ENABLED */

#ifdef HAL_SMARTCARD_MODULE_ENABLED
#include "stm32f0xx_hal_smartcard.h"
#endif /* HAL_SMARTCARD_MODULE_ENABLED */

#ifdef HAL_SMBUS_MODULE_ENABLED
#include "stm32f0xx_hal_smbus.h"
#endif /* HAL_SMBUS_MODULE_ENABLED */

#ifdef HAL_SPI_MODULE_ENABLED
#include "stm32f0xx_hal_spi.h"
#endif /* HAL_SPI_MODULE_ENABLED */

#ifdef HAL_TIM_MODULE_ENABLED
#include "stm32f0xx_hal_tim.h"
#endif /* HAL_TIM_MODULE_ENABLED */

#ifdef HAL_TSC_MODULE_ENABLED
#include "stm32f0xx_hal_tsc.h"
#endif /* HAL_TSC_MODULE_ENABLED */

#ifdef HAL_UART_MODULE_ENABLED
#include "stm32f0xx_hal_uart.h"
#endif /* HAL_UART_MODULE_ENABLED */

#ifdef HAL_USART_MODULE_ENABLED
#include "stm32f0xx_hal_usart.h"
#endif /* HAL_USART_MODULE_ENABLED */

#ifdef HAL_WWDG_MODULE_ENABLED
#include "stm32f0xx_hal_wwdg.h"
#endif /* HAL_WWDG_MODULE_ENABLED */

#define assert_param(expr) ((void)0U)

#ifdef __cplusplus
}
#endif

#endif