/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file  adi_evb_crc.h
 * @brief CRC API header file
 * @addtogroup    CRC drivers
 * @{
 */

#ifndef __ADI_EVB_CRC_H__
#define __ADI_EVB_CRC_H__

#ifdef __cplusplus
extern "C" {
#endif

/*============= I N C L U D E S =============*/

#include <stdint.h>

/**
 * @brief Calculates CRC for the given data
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  pData  - pointer to the data for which CRC needs to be calculated
 * @param[in]  numBytes  - number of bytes to be considered for CRC calculation
 *
 * @return  CRC value
 */
int32_t EvbCrcCalculate(void *hEvb, uint8_t *pData, uint32_t numBytes);

/**
 * @brief Gets the CRC value
 * @param[in] hEvb - pointer to Evb handle
 * @param[out]  pData - pointer to destination data buffer
 *
 * @return 0 on success, error code otherwise
 */
int32_t EvbCrcGetValue(void *hEvb, uint32_t *pData);

/**
 * @brief       Clear crc interrupt
 */
void EvbCrcClearInterrupt(void);

/**
 * @brief       Reset Crc module
 */
void EvbCrcReset(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADI_EVB_CRC_H__ */

/**
 * @}
 */
