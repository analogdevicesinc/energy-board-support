/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     stm_crc.c
 * @brief    This file contains the CRC routines for STM platform.
 * @{
 */

/*=============  I N C L U D E S   =============*/

#include "adi_evb.h"
#include "app_cfg.h"
#ifdef STM32H735xx
#include "stm32h7xx_hal.h"
#include "stm32h7xx_it.h"
#else
#include "stm32h5xx_hal.h"
#include "stm32h5xx_it.h"
#endif
#include "stm32h5xx_hal_crc.h"

/*=============  D E F I N I T I O N S  =============*/

/*=============  D A T A  =============*/

/** Evb crc info */
typedef struct
{
    /** callback */
    ADI_EVB_CALLBACK pfCallback;
    /** CRC value */
    uint32_t crc;
} EVB_CRC_INFO;

/** creating crc instance*/
static CRC_HandleTypeDef hCRC;

static EVB_CRC_INFO evbCrcInfo;
/*=============  C O D E  =============*/
int32_t EvbInitCrc(void **phCrc, ADI_EVB_CRC_CONFIG *pConfig)
{
    int32_t status = 0;
    hCRC.Instance = CRC;
    hCRC.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
    hCRC.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
    hCRC.Init.GeneratingPolynomial = 0x1021;
    hCRC.Init.CRCLength = CRC_POLYLENGTH_16B;
    hCRC.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
    hCRC.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
    hCRC.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
    hCRC.Init.GeneratingPolynomial = 0x1021;
    hCRC.Init.CRCLength = CRC_POLYLENGTH_16B;
    evbCrcInfo.pfCallback = pConfig->pfCallback;
    /* Just a keeping a API provision to support multiple SPIs later*/
    *phCrc = &evbCrcInfo;
    if (HAL_CRC_Init(&hCRC) != HAL_OK)
    {
        status = -1;
    }
    return status;
}

int32_t EvbCrcCalculate(void *hEvb, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 0;
    (void)hEvb; /* Dummy use of argument */
    evbCrcInfo.crc = HAL_CRC_Calculate(&hCRC, (uint32_t *)pData, numBytes);
    if (evbCrcInfo.pfCallback != NULL)
    {
        evbCrcInfo.pfCallback();
    }
    return status;
}

int32_t EvbCrcGetValue(void *hEvb, uint32_t *pData)
{
    int32_t status = 0;
    (void)hEvb; /* Dummy use of argument */
    *pData = evbCrcInfo.crc;
    return status;
}

/**
 * @}
 */
