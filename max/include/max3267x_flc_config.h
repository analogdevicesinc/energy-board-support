/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file  max3267x_flc_config.h
 * @brief User defined Uart APIs for max3267x platform
 * @defgroup    SPI drivers
 * @{
 */

#ifndef __MAX3267X_FLC_CONFIG__
#define __MAX3267X_FLC_CONFIG__

#ifdef __cplusplus
extern "C" {
#endif

/*============= I N C L U D E S =============*/

#include <stdint.h>

/*=============  D E F I N I T I O N S  =============*/

/*======= P U B L I C   P R O T O T Y P E S ========*/

/**
 * @brief Initialize the flash controller
 * @note This function should be called before any flash operations.
 * @return  error or success
 */
int32_t MaxInitFlc(void);

/**
 * @brief Write data to flash memory.
 * This function writes data to the flash memory at the specified address.
 * @param[in]  pData  - pointer to the data to be written
 * @param[in] addr - address to write the data
 * @param[in] numBytes - number of bytes to write
 * @return  error or success
 */
int32_t MaxFlcWrite(uint8_t *pData, uint32_t addr, uint32_t numBytes);

/**
 * @brief Read data from flash memory.
 * This function reads data from the flash memory at the specified address.
 * @param[in]  addr  - address to read the data
 * @param[in] numBytes - number of bytes to read
 * @param[out] pData - pointer to the buffer to store the read data
 */
void MaxFlcRead(uint32_t addr, uint32_t numBytes, uint8_t *pData);

/**
 * @brief Erases the page of flash at the specified address.
 * @param[in] addr - address to erase
 * @return  error or success
 */
int32_t MaxFlcErase(uint32_t addr);

#ifdef __cplusplus
}
#endif

#endif /* __MAX3267X_FLC_CONFIG__ */

/**
 * @}
 */
