// Copyright 2021, Ryan Wendland, XboxHDMI by Ryzee119
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include "main.h"
#include "stm32f0xx_hal.h"
#include "adv7511.h"

I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;
adv7511 encoder;

void SystemClock_Config(void);
static void init_gpio(void);
static void init_i2c1(void);
static void init_uart2(void);

//Stdout print on UART for printf
int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, 100);
    return len;
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    init_gpio();
    init_i2c1();
    init_uart2();

    HAL_Delay(200);
    printf("\r\nADV7511 Chip Revision %u\r\n", adv7511_read_register(&hi2c1, 0x00));

    uint8_t error = 0;

    //Initialise the encoder object
    adv7511_struct_init(&encoder);

    //Force Hot Plug Detect High
    encoder.hot_plug_detect = 1;
    error |= adv7511_write_register(&hi2c1, 0xD6, 0b11000000);

    //Power up the encoder and set fixed registers
    error |= adv7511_power_up(&hi2c1, &encoder);
    HAL_Delay(200);

    //Set video input mode to YCbCr 444, 12bit databus DDR.
    error |= adv7511_update_register(&hi2c1, 0x15, 0b00001111, 0b000000101); //ID=5

    //Set video style to style 1 (Y[3:0] Cb[7:0] first edge, Cr[7:0] Y[7:4] second edge)
    error |= adv7511_update_register(&hi2c1, 0x16, 0b00001100, 0b00001000); //style 1 01 = style 2    10 = style 1  11 = style 3

    //Set DDR Input Edge  first half of pixel data clocking edge, Bit 1 |= 0 for falling edge, 1 for rising edge CHECK
    error |= adv7511_update_register(&hi2c1, 0x16, 0b00000010, 0b00000010); //Rising

    //Bit order reverse for input signals. 1 |= LSB .... MSB Reverse Bus Order
    //Just how my PCB is layed out.
    error |= adv7511_update_register(&hi2c1, 0x48, 0b01000000, 0b01000000);

    //DDR Alignment . 1 |= DDR input is D[35:18] (left aligned), 0 = right aligned
    error |= adv7511_update_register(&hi2c1, 0x48, 0b00100000, 0b00000000);

    //Clock Delay adjust.
    error |= adv7511_update_register(&hi2c1, 0xD0, 0b10000000, 0b10000000);
    error |= adv7511_update_register(&hi2c1, 0xD0, 0b01110000, 3 << 4); //0 to 6, 3 = no delay
    error |= adv7511_update_register(&hi2c1, 0xBA, 0b11100000, 3 << 5);

    //Must be 11 for ID=5 (No sync pulse)
    error |= adv7511_update_register(&hi2c1, 0xD0, 0b00001100, 0b00001100);

    //Enable DE generation. This is derived from HSYNC,VSYNC for video active framing
    error |= adv7511_update_register(&hi2c1, 0x17, 0b00000001, 0b00000001);

    //Set Output Format to 4:4:4
    error |= adv7511_update_register(&hi2c1, 0x16, 0b10000000, 0b00000000);

    //Start AVI Infoframe Update
    error |= adv7511_update_register(&hi2c1, 0x4A, 0b01000000, 0b01000000);

    //Infoframe output format to YCbCr 4:4:4 in infoframe* 10=YCbCr4:4:4, 00=RGB
    error |= adv7511_update_register(&hi2c1, 0x55, 0b01100000, 0b01000000);

    //Infoframe output aspect ratio default to 4:3
    error |= adv7511_update_register(&hi2c1, 0x56, 0b00110000, 0b00010000);
    error |= adv7511_update_register(&hi2c1, 0x56, 0b00001111, 0b00001000);

    //END AVI Infoframe Update
    error |= adv7511_update_register(&hi2c1, 0x4A, 0b01000000, 0b00000000);

    //Output Color Space Selection 0 = RGB 1 = YCbCr
    error |= adv7511_update_register(&hi2c1, 0x16, 0b00000001, 0b00000001);

    //Set Output to HDMI Mode (Instead of DVI Mode)
    error |= adv7511_update_register(&hi2c1, 0xAF, 0b00000010, 0b00000010);

    //Enable General Control Packet CHECK
    error |= adv7511_update_register(&hi2c1, 0x40, 0b10000000, 0b10000000);

    //Disable CSC
    //error |= adv7511_update_register(&hi2c1, 0x18, 0xFF, 0x00);

    //SETUP AUDIO
    //Set 48kHz Audio clock CHECK (N Value)
    error |= adv7511_update_register(&hi2c1, 0x01, 0xFF, 0x00);
    error |= adv7511_update_register(&hi2c1, 0x02, 0xFF, 0x18);
    error |= adv7511_update_register(&hi2c1, 0x03, 0xFF, 0x00);

    //Set SPDIF audio source
    error |= adv7511_update_register(&hi2c1, 0x0A, 0b01110000, 0b00010000);

    //SPDIF enable
    error |= adv7511_update_register(&hi2c1, 0x0B, 0b10000000, 0b10000000);

    if (error)
    {
        printf("Encountered error when setting up ADV7511\r\n");
    }

    apply_csc(&hi2c1, (uint8_t *)identityMatrix);
    uint8_t vic = 0x80;
    uint8_t pll_lock = 0;

    while (1)
    {
        if (error)
        {
            printf("Encountered error when setting up ADV7511\r\n");
            error = 0;
        }

        //Check PLL status
        pll_lock = (adv7511_read_register(&hi2c1, 0x9E) >> 4) & 0x01;
        if (pll_lock == 0)
        {
            printf("PLL Lock: %u\r\n", pll_lock);
            HAL_GPIO_WritePin(GPIOA, STATUS_LED, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, STATUS_LED, GPIO_PIN_SET);
        }

        if ((adv7511_read_register(&hi2c1, 0x3e) >> 2) != (vic & 0x0F))
        {
            printf("VIC Changed!\r\n");
            //Set MSB to 1. This indicates a recent change.
            vic = ADV7511_VIC_CHANGED | adv7511_read_register(&hi2c1, 0x3e) >> 2;
            printf("Detected VIC#: 0x%02x\r\n", vic & ADV7511_VIC_CHANGED_CLEAR);
        }

        if (encoder.interrupt)
        {
            uint8_t interrupt_register = adv7511_read_register(&hi2c1, 0x96);
            printf("Interrupt occurred!\r\n");
            printf("interrupt_register: 0x%02x\r\n", interrupt_register);

            if (interrupt_register & ADV7511_INT0_HPD)
            {
                printf("HPD interrupt\r\n");
                encoder.hot_plug_detect = (adv7511_read_register(&hi2c1, 0x42) >> 6) & 0x01;
            }

            if (interrupt_register & ADV7511_INT0_MONITOR_SENSE)
            {
                printf("Monitor Sense Interrupt\r\n");
                encoder.monitor_sense = (adv7511_read_register(&hi2c1, 0x42) >> 5) & 0x01;
            }

            (encoder.hot_plug_detect) ? printf("HDMI cable detected\r\n") : printf("HDMI cable not detected\r\n");
            (encoder.monitor_sense) ? printf("Monitor is ready\r\n") : printf("Monitor is not ready\r\n");

            if (encoder.hot_plug_detect && encoder.monitor_sense)
            {
                adv7511_power_up(&hi2c1, &encoder);
            }

            encoder.interrupt = 0;
            //Re-enable interrupts
            adv7511_update_register(&hi2c1, 0x96, 0b11000000, 0xC0);
        }

        if (vic & ADV7511_VIC_CHANGED)
        {
            uint16_t hs_delay = 0, vs_delay = 0, active_w = 0, active_h = 0;
            uint8_t ddr_edge = 1;
            vic &= ADV7511_VIC_CHANGED_CLEAR;

            //Set pixel rep mode to auto
            //error |= adv7511_update_register(&hi2c1, 0x3B, 0b01100000, 0b00000000);

            if (vic == ADV7511_VIC_VGA_640x480_4_3)
            {
                printf("Set timing for VGA 640x480 4:3\r\n");
                //Infoframe output aspect ratio default to 4:3
                error |= adv7511_update_register(&hi2c1, 0x56, 0b00110000, 0b00010000);
                ddr_edge = 1;
                hs_delay = 119; //121
                vs_delay = 36;
                active_w = 720;
                active_h = 480;
            }

            else if (vic == ADV7511_VIC_480p_4_3 || vic == ADV7511_VIC_480p_16_9 || vic == ADV7511_VIC_UNAVAILABLE)
            {
                if (vic == ADV7511_VIC_480p_16_9)
                {
                    //Infoframe output aspect ratio default to 16:9
                    error |= adv7511_update_register(&hi2c1, 0x56, 0b00110000, 0b00100000);
                    printf("Set timing for 480p 16:9\r\n");
                }
                else
                {
                    //Infoframe output aspect ratio default to 4:3
                    error |= adv7511_update_register(&hi2c1, 0x56, 0b00110000, 0b00010000);
                    printf("Set timing for 480p 4:3\r\n");
                }
                ddr_edge = 1;
                hs_delay = 118;
                vs_delay = 36;
                active_w = 720;
                active_h = 480;
            }

            else if (vic == ADV7511_VIC_720p_60_16_9)
            {
                printf("Set timing for 720p 16:9\r\n");
                //Infoframe output aspect ratio default to 16:9
                error |= adv7511_update_register(&hi2c1, 0x56, 0b00110000, 0b00100000);
                ddr_edge = 1;
                hs_delay = 299; //259?
                vs_delay = 25;
                active_w = 1280;
                active_h = 720;
            }

            else if (vic == ADV7511_VIC_1080i_60_16_9)
            {
                printf("Set timing for 1080i 16:9\r\n");
                //Infoframe output aspect ratio default to 16:9
                error |= adv7511_update_register(&hi2c1, 0x56, 0b00110000, 0b00100000);
                //Set interlace offset
                error |= adv7511_update_register(&hi2c1, 0x37, 0b11100000, 0 << 5);
                //Offset for Sync Adjustment Vsync Placement
                error |= adv7511_update_register(&hi2c1, 0xDC, 0b11100000, 0 << 5);
                ddr_edge = 1;
                hs_delay = 233; //232
                vs_delay = 22;
                active_w = 1920;
                active_h = 540;
            }

            error |= adv7511_update_register(&hi2c1, 0x16, 0b00000010, ddr_edge << 1);
            error |= adv7511_update_register(&hi2c1, 0x36, 0b00111111, (uint8_t)vs_delay);
            error |= adv7511_update_register(&hi2c1, 0x35, 0b11111111, (uint8_t)(hs_delay >> 2));
            error |= adv7511_update_register(&hi2c1, 0x36, 0b11000000, (uint8_t)(hs_delay << 6));
            error |= adv7511_update_register(&hi2c1, 0x37, 0b00011111, (uint8_t)(active_w >> 7));
            error |= adv7511_update_register(&hi2c1, 0x38, 0b11111110, (uint8_t)(active_w << 1));
            error |= adv7511_update_register(&hi2c1, 0x39, 0b11111111, (uint8_t)(active_h >> 4));
            error |= adv7511_update_register(&hi2c1, 0x3A, 0b11110000, (uint8_t)(active_h << 4));
            printf("Actual Pixel Repetition : 0x%02x\r\n", (adv7511_read_register(&hi2c1, 0x3D) & 0xC0) >> 6);
            printf("Actual VIC Sent : 0x%02x\r\n", adv7511_read_register(&hi2c1, 0x3D) & 0x1F);
        }
    }
}

void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    //Initializes the CPU, AHB and APB busses clocks
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    //Initializes the CPU, AHB and APB busses clocks
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    //Configure the Systick interrupt time
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    //Configure the Systick
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    //SysTick_IRQn interrupt configuration
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void init_i2c1(void)
{

    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x2000090E;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    //Configure Analogue filter
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    //Configure Digital filter
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}

static void init_uart2(void)
{

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_HalfDuplex_Init(&huart2) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}

static void init_gpio(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    //GPIO Ports Clock Enable
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    //Configure GPIO pins : PA5 PA6 PA7 (Unused)
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //Configure GPIO pin : PB1 (Unused)
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    //Configure GPIO pins : PA10 (Status LED)
    GPIO_InitStruct.Pin = STATUS_LED;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //Configure GPIO pin : ADV7511_INT_Pin
    GPIO_InitStruct.Pin = ADV7511_INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ADV7511_INT_GPIO_Port, &GPIO_InitStruct);

    //EXTI interrupt init
    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void _Error_Handler(char *file, int line)
{
    while (1)
    {
    }
}