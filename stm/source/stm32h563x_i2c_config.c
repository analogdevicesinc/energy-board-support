/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     stm32h563x_i2c_config.c
 * @brief    I2C configuration
 * @{
 */
/*============= I N C L U D E S =============*/
#include "stm32h563x_i2c_config.h"
#include "app_cfg.h"
#include "board_cfg.h"
#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_i2c.h"
#include <stdint.h>

/*=============  D E F I N I T I O N S  =============*/
/** Handler for I2C */
extern I2C_HandleTypeDef hI2C;
/*=============  C O D E  =============*/
uint32_t StmI2CInit(STM_I2C_INSTANCE *pI2cInfo)
{
    uint32_t status = 0;
    HAL_StatusTypeDef halStatus;
    hI2C.Instance = pI2cInfo->pI2c;
    /** The timing value has to be generated from STM32CubeMX after
     * supplying desired I2C Clock Speed.
     */
    // 0x106029FF -> 400 kHz
    hI2C.Init.Timing = 0xC035A6; // 1 MHz - 0xC035A6
    hI2C.Init.OwnAddress1 = 0;
    hI2C.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hI2C.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hI2C.Init.OwnAddress2 = 0;
    hI2C.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hI2C.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hI2C.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    halStatus = HAL_I2C_Init(&hI2C);

    /** Configure Analogue filter
     */
    if (halStatus == HAL_OK)
    {
        HAL_I2CEx_ConfigAnalogFilter(&hI2C, I2C_ANALOGFILTER_ENABLE);
    }

    /** Configure Digital filter
     */
    if (halStatus == HAL_OK)
    {
        HAL_I2CEx_ConfigDigitalFilter(&hI2C, 0);
    }
    if (halStatus == HAL_OK)
    {
        status = 0;
    }
    else
    {
        status = 1;
    }

    return status;
}

uint32_t StmI2CTransmit(STM_I2C_INSTANCE *pI2cInfo, uint16_t devAddr, uint8_t *pData, uint16_t len)
{
    uint32_t status = 0;
    HAL_StatusTypeDef halStatus = HAL_OK;

    if (pI2cInfo != NULL)
    {
        halStatus = HAL_I2C_Master_Transmit_DMA(&hI2C, devAddr, pData, len);
        if (halStatus == HAL_OK)
        {
            status = 0;
        }
        else
        {
            status = 1;
        }
    }
    else
    {
        status = 1;
    }
    return status;
}

uint32_t StmI2CReceive(STM_I2C_INSTANCE *pI2cInfo, uint16_t devAddr, uint8_t *pData, uint16_t len)
{
    uint32_t status = 0;
    HAL_StatusTypeDef halStatus = HAL_OK;

    if (pI2cInfo != NULL)
    {
        halStatus = HAL_I2C_Master_Receive(&hI2C, devAddr, pData, len, HAL_MAX_DELAY);
        if (halStatus == HAL_OK)
        {
            status = 0;
        }
        else
        {
            status = 1;
        }
    }
    else
    {
        status = 1;
    }
    return status;
}
/** @} */
