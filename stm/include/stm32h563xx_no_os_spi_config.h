/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file  stm32h563xx_no_os_spi_config.h
 * @brief SPI sample configuration for STMH563xx
 * @addtogroup    SPI drivers
 * @{
 */

#ifndef __STM32H563XX_NO_OS_SPI_CONFIG_H__
#define __STM32H563XX_NO_OS_SPI_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/*============= I N C L U D E S =============*/
#include "no_os_spi.h"
#include <stdint.h>

/*=============  D E F I N I T I O N S  =============*/

/**
 * @brief SPI Master Tx Rx API
 * @param  pSpiInfo  - pointer to the spi info
 * @param  pTxData  - pointer to Tx data
 * @param  pRxData  - pointer to Rx data
 * @param  numBytes  - num of bytes to send
 * @param  pCallback  - SPi Rx Callback function pointer
 * @return  error or success
 */
int32_t NoOsStmStartSpiTxRxDMA(struct no_os_spi_desc *pSpiInfo, uint8_t *pTxData, uint8_t *pRxData,
                               uint32_t numBytes, void (*pCallback)());

/**
 * @brief       set spi frequency
 * @param[in]   pSpiInfo  -  Pointer to spi instance struct
 * @param[in]   spiFrequency -  spi frequency in Hz
 * The API sets the nearest possible prescaler based on spiFRequency value
 * @return      status
 */
int32_t StmSetSpiFrequency(struct no_os_spi_desc *pSpiInfo, uint32_t spiFrequency);

/**
 * @brief       No OS ADC SPi Rx Callback
 */
void NoOsAdcSpiRxCpltCallback();

/**
 * @brief       No OS FRAM SPi Rx Callback
 */
void NoOsFramSpiRxCpltCallback();

#ifdef __cplusplus
}
#endif

#endif /* __STM32H563XX_NO_OS_SPI_CONFIG_H__ */

/**
 * @}
 */
