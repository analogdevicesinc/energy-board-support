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
#include "adi_crc.h"
#include <stdint.h>

/**
 * @brief       Initialise uart
 * @param[in]   phCrc -  Pointer to uart instance struct
 * @return      status
 */
ADI_CRC_RESULT EvbInitCrc(void **phCrc);

/**
 * @brief       Set crc config
 * @param[in]   pData -  Pointer to crc data struct
 * @return      status
 */
int32_t EvbCrcSetConfig(ADI_CRC_DATA *pData);

/**
 * @brief       Calculate Crc
 * @param[in]   pCrcData -  Pointer to crc data struct
 * @param[in]   pData -  Pointer to data for which crc must be calculated
 * @param[in]   offset -  offset value
 * @param[in]   numBytes -  num of data bytes
 */
void EvbCalculateCrc(ADI_CRC_DATA *pCrcData, uint8_t *pData, uint16_t offset, uint32_t numBytes);

/**
 * @brief       Set crc config
 * @param[in]   hCrc -  Crc handle
 * @param[in]   pData -  Pointer to crc value
 * @return      status
 */
ADI_CRC_RESULT EvbGetCrc(ADI_CRC_HANDLE hCrc, uint32_t *pData);

/**
 * @brief       Clear crc interrupt
 */
void EvbCrcClearInterrupt(void);

/**
 * @brief       Reset Crc module
 */
void EvbResetHw(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADI_EVB_CRC_H__ */

/**
 * @}
 */
