/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file    max3267x_flc_config.c
 * @brief   This file contains the routines for initializing, configuring and
 * starting FLC for writing and reading data from flash memory.
 * @{
 */

#include "max3267x_flc_config.h"
#include "flc.h"
#include "icc.h"
#include <stdint.h>

int32_t MaxInitFlc()
{
    int status = -1;
    status = MXC_FLC_Init();
    return status;
}

int32_t MaxFlcWrite(uint8_t *pData, uint32_t addr, uint32_t numBytes)
{
    int32_t status = 0;
    MXC_ICC_Disable();
    status = MXC_FLC_PageErase(addr);
    if (status == 0)
    {
        status = MXC_FLC_Write(addr, numBytes, (void *)pData);
    }
    if (status == 0)
    {
        MXC_ICC_Enable();
    }
    return status;
}

void MaxFlcRead(uint32_t addr, uint32_t numBytes, uint8_t *pData)
{
    MXC_FLC_Read((int)addr, pData, (int)numBytes);
}

int32_t MaxFlcErase(uint32_t addr)
{
    int32_t status = 0;
    status = MXC_FLC_PageErase(addr);
    return status;
}

/**
 * @}
 */
