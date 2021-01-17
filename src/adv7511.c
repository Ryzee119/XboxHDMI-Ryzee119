// Copyright 2021, Ryan Wendland, XboxHDMI by Ryzee119
// SPDX-License-Identifier: MIT

#include <string.h>
#include <stdio.h>
#include "stm32f0xx_hal.h"
#include "adv7511.h"

void adv7511_struct_init(adv7511 *encoder)
{
    encoder->hot_plug_detect = 0;
    encoder->interrupt = 0;
    encoder->monitor_sense = 0;
}

uint8_t adv7511_read_register(I2C_HandleTypeDef *i2c, uint8_t address)
{
    HAL_StatusTypeDef status;
    uint8_t register_value = 0;

    status = HAL_I2C_Mem_Read(i2c,
                              ADV7511_MAIN_I2C_ADDR,
                              address,
                              0x01,
                              &register_value,
                              0x01,
                              100);

    if (status != HAL_OK)
    {
        printf("Error with HAL_I2C_Mem_Read %u\r\n", status);
    }

    return register_value;
}

uint8_t adv7511_write_register(I2C_HandleTypeDef *i2c, uint8_t address, uint8_t value)
{
    HAL_StatusTypeDef status;
    status = HAL_I2C_Mem_Write(i2c,
                               ADV7511_MAIN_I2C_ADDR,
                               address,
                               0x01, //Memory Address Size
                               &value,
                               0x01, //Bytes to send
                               100); //Timeout (ms)
    if (status != HAL_OK)
    {
        printf("Error with HAL_I2C_Mem_Write %u\r\n", status);
        return 1;
    }

    return 0;
}

uint8_t adv7511_write_cec(I2C_HandleTypeDef *i2c, uint8_t address, uint8_t value)
{
    HAL_StatusTypeDef status;
    status = HAL_I2C_Mem_Write(i2c,
                               ADV7511_CEC_I2C_ADDR_DEFAULT,
                               address,
                               0x01, //Memory Address Size
                               &value,
                               0x01, //Bytes to send
                               100); //Timeout (ms)
    if (status != HAL_OK)
    {
        printf("Error with HAL_I2C_Mem_Write %u\r\n", status);
        return 1;
    }

    return 0;
}

uint8_t adv7511_update_register(I2C_HandleTypeDef *i2c, uint8_t address,
                                uint8_t mask, uint8_t new_value)
{
    uint8_t working_value, retval;
    uint8_t register_value = adv7511_read_register(i2c, address);

    working_value = register_value & ~mask;
    new_value &= mask;
    new_value |= working_value;

    retval = adv7511_write_register(i2c, address, new_value);

    printf("Register 0x%02x : 0x%02x -> 0x%02x\r\n",
           address, register_value, adv7511_read_register(i2c, address));

    return retval;
}

uint8_t adv7511_power_up(I2C_HandleTypeDef *i2c, adv7511 *encoder)
{
    uint8_t status;
    /* ADI recommended values for proper operation. */
    static const uint8_t add = 0, val = 1, msk = 2;

    static const uint8_t adv7511_startup[11][3] = {
        //Address, Value, Bit mask//
        {0x41, 0x00, 0b01000000},
        {0x98, 0x03, 0b11111111}, //Must be set to 0x03 for proper operation
        {0x9a, 0xe0, 0b11100000}, //Must be set to 0xe0 for proper operation
        {0x9c, 0x30, 0b11111111}, //Must be set to 0x30 for proper operation
        {0x9d, 0x01, 0b00000001}, //Must be set to 0b01 for proper operation
        {0xa2, 0xa4, 0b11111111}, //Must be set to 0xA4 for proper operation
        {0xa3, 0xa4, 0b11111111}, //Must be set to 0xA4 for proper operation
        {0xe0, 0xd0, 0b11111111}, //Must be set to 0xD0 for proper operation
        {0xf9, 0x00, 0b11111111}, //This should be set to a non-conflicting I2C address (0x00)
        {0x55, 0x01, 0b00000011}, //Set AVI info-frame to TV
        {0x96, 0xc0, 0b11000000}, //Enable HPD and Monitor Sense interrupts
    };

    if (encoder->hot_plug_detect)
    {
        for (uint8_t i = 0; i < sizeof(adv7511_startup) / 3; i++)
        {
            status = adv7511_update_register(i2c,
                                             adv7511_startup[i][add],
                                             adv7511_startup[i][msk],
                                             adv7511_startup[i][val]);

            if (status != 0)
            {
                printf("Error with HAL_I2C_Master_Transmit %u\r\n", status);
                return 1;
            }
        }
        HAL_Delay(5);
    }

    return 0;
}

uint8_t apply_csc(I2C_HandleTypeDef *i2c, uint8_t *coefficients)
{
    uint8_t error = 0;
    for (uint8_t i = 0x18; i <= 0x2F; i++)
    {
        error |= adv7511_update_register(i2c, i, 0xFF, coefficients[i - 0x18]);
    }

    return error;
}
