/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/
/**
 * @file        adi_evb_i2c.h
 * @brief       I2C APIs for different EVKs
 * @addtogroup  I2C drivers
 * @{
 */

#ifndef __ADI_EVB_I2C_H__
#define __ADI_EVB_I2C_H__

/*============= I N C L U D E S =============*/

#include "board_cfg.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============  D E F I N I T I O N S  =============*/

/**
 * @brief I2C API for EVKs
 *
 * @return  error or success
 */
uint32_t EvbInitI2C(void);

/**
 * @brief Transmits data to an I2C device.
 *
 * This function sends data to a specified I2C device using its device address.
 *
 * @param[in] devAddr The 7-bit I2C device address.
 * @param[in] pData Pointer to the data buffer to be written.
 * @param[in] len The number of bytes to write from the data buffer.
 *
 * @return A 32-bit status code indicating the result of the operation.
 *         Typically, 0 indicates success, while non-zero values indicate errors.
 */
uint32_t EvbTransmitI2C(uint8_t *pData, uint16_t devAddr, uint16_t len);

/**
 * @brief Receives data from an I2C device.
 *
 * This function reads data from a specified I2C device using its device address.
 *
 * @param[in]  devAddr The 7-bit I2C device address.
 * @param[out] pData Pointer to the buffer where the read data will be stored.
 * @param[in]  len The number of bytes to read into the buffer.
 *
 * @return A 32-bit status code indicating the result of the operation.
 *         Typically, 0 indicates success, while non-zero values indicate errors.
 */
uint32_t EvbReceiveI2C(uint8_t *pData, uint16_t devAddr, uint16_t len);

/**
 * @brief Interrupt Handler
 */
void EvbHandlerI2CTx(void);

/**
 * @brief Interrupt Handler
 * @return Pointer to I2C Transmit Completion Flag
 */
volatile uint8_t *EvbGetTxFlag(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADI_EVB_I2C_H__ */

/** @} */