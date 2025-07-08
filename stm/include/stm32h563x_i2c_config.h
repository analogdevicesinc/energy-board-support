/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file  stm32h563x_i2c_config.h
 * @brief I2C sample configuration for STMH563xx
 * @addtogroup    I2C drivers
 * @{
 */

#ifndef __STM32H563XX_I2C_CONFIG_H__
#define __STM32H563XX_I2C_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/*============= I N C L U D E S =============*/
#include "stm32h5xx.h"
#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_i2c.h"
#include <stdint.h>

/*=============  D E F I N I T I O N S  =============*/
/** STM I2C instance info */
typedef struct
{
    /** pointer to I2C Registers Struct */
    I2C_TypeDef *pI2c;
} STM_I2C_INSTANCE;

/**
 * @brief       Initialise I2C
 * @param[in]   pI2cInfo  Pointer to I2C instance struct
 * @return      status
 */
uint32_t StmI2CInit(STM_I2C_INSTANCE *pI2cInfo);

/**
 * @brief Transmits data to an I2C device.
 *
 *
 * @param pI2cInfo Pointer to the I2C instance information structure.
 * @param devAddr The 7-bit I2C device address.
 * @param pData Pointer to the data buffer containing the data to be written.
 * @param len The number of bytes to write from the data buffer.
 * @return uint32_t Returns a status code indicating the result of the operation.
 *                  Typically, 0 indicates success, while non-zero values indicate errors.
 */
uint32_t StmI2CTransmit(STM_I2C_INSTANCE *pI2cInfo, uint16_t devAddr, uint8_t *pData, uint16_t len);

/**
 * @brief Receives data from an I2C device.
 *
 *
 * @param pI2cInfo Pointer to the I2C instance information structure.
 * @param devAddr The 7-bit I2C device address.
 * @param pData Pointer to the data buffer where the read data will be stored.
 * @param len The number of bytes to read into the data buffer.
 * @return uint32_t Returns a status code indicating the result of the operation.
 *                  Typically, 0 indicates success, while non-zero values indicate errors.
 */
uint32_t StmI2CReceive(STM_I2C_INSTANCE *pI2cInfo, uint16_t devAddr, uint8_t *pData, uint16_t len);
#endif /* __STM32H563XX_I2C_CONFIG_H__ */

/** @} */
