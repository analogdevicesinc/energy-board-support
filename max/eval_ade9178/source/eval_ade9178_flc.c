/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file    eval_ade9178_flc.c
 * @brief   This file contains the routines for initializing, configuring and
 * starting FLC for writing and reading data from flash memory.
 * @{
 */

#include "max32670.h"
#include "max3267x_flc_config.h"
#include <stdint.h>
#include <string.h>

/** Points to last page in flash, which is guaranteed to be unused by the application */
#define LAST_MEM_ADDR_FLASH MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE

int32_t EvbInitNvm(void *hEvb)
{
    int32_t status = 0;
    if (hEvb != NULL)
    {
        status = MaxInitFlc();
    }
    return status;
}

int32_t EvbNvmWrite(void *hEvb, uint8_t *pSrc, uint32_t numBytes, uint32_t timeOutCount)
{
    int status = 0;
    uint32_t memAddr;
    uint8_t pageNum;
    uint16_t addrOffset;
    (void)timeOutCount; /* Dummy use of argument */
    if (hEvb != NULL)
    {
        // Extract page number and address offset from pSrc
        memcpy(&pageNum, &pSrc[0], sizeof(uint8_t));
        memcpy(&addrOffset, &pSrc[1], sizeof(uint16_t));
        memAddr = LAST_MEM_ADDR_FLASH - ((pageNum + 1) * MXC_FLASH_PAGE_SIZE) + addrOffset;
        if (memAddr < LAST_MEM_ADDR_FLASH)
        {
            status = MaxFlcWrite(&pSrc[3], memAddr, numBytes);
        }
    }
    return status;
}

int32_t EvbNvmRead(void *hEvb, uint8_t *pSrc, uint8_t *pDst, uint32_t numBytes,
                   uint32_t timeOutCount)
{
    int status = 0;
    uint32_t memAddr;
    uint8_t pageNum;
    uint16_t addrOffset;
    (void)timeOutCount; /* Dummy use of argument */
    if (hEvb != NULL)
    {
        // Extract page number and address offset from pSrc
        memcpy(&pageNum, &pSrc[0], sizeof(uint8_t));
        memcpy(&addrOffset, &pSrc[1], sizeof(uint16_t));
        memAddr = LAST_MEM_ADDR_FLASH - ((pageNum + 1) * MXC_FLASH_PAGE_SIZE) + addrOffset;
        if (memAddr < LAST_MEM_ADDR_FLASH)
        {
            MaxFlcRead(memAddr, numBytes, pDst);
        }
    }
    return status;
}

int32_t EvbNvmErase(void *hEvb, uint32_t addr)
{
    int status = 0;
    uint32_t memAddr;
    if (hEvb != NULL)
    {
        // Calculate the memory address based on the given address and page size
        memAddr = LAST_MEM_ADDR_FLASH - ((addr + 1) * MXC_FLASH_PAGE_SIZE);
        if (memAddr < LAST_MEM_ADDR_FLASH)
        {
            status = MaxFlcErase(memAddr);
        }
    }
    return status;
}

/**
 * @}
 */
