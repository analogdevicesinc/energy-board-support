/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file  stm32h563xx_spi_config.h
 * @brief SPI sample configuration for STMH563xx
 * @addtogroup    SPI drivers
 * @{
 */

#ifndef __STM32H563XX_SPI_CONFIG_H__
#define __STM32H563XX_SPI_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/*============= I N C L U D E S =============*/
#include "stm32h5xx.h"
#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_spi.h"
#include <stdint.h>

/*=============  D E F I N I T I O N S  =============*/
/** STM spi instance info */
typedef struct
{
    /** Dummy command buffer is required during transmit and receive spi dma */
    uint32_t dummyCmd;
    /** master or slave SPI config*/
    uint32_t master;
    /** spi speed: (SysClk / prescaler) Hz */
    uint32_t prescaler;
    /** clk polarity */
    uint32_t clkPol;
    /** clk phase*/
    uint32_t clkPha;
    /** pointer to spi registers struct */
    SPI_TypeDef *pSpi;
    /** Handle of SPI */
    SPI_HandleTypeDef *phSpi;
    /** suspenState is used to suspend from the blocking state */
    volatile uint8_t suspendState;
    /** slave select pin */
    uint16_t ssPin;
    /** port for slave select */
    GPIO_TypeDef *ssPort;

} STM_SPI_INSTANCE;

/**
 * @brief       Initialise spi
 * @param[in]   pSpiInfo  Pointer to spi instance struct
 * @return      status
 */
int32_t StmInitSpi(STM_SPI_INSTANCE *pSpiInfo);

/**
 * @brief       start spi tx
 * @param[in]   pSpiInfo  -  Pointer to spi instance struct
 * @param[in]   pData -  Pointer to data
 * @param[in]   numBytes - no of bytes
 * @return      status
 */
int32_t StmStartSpiTxDMA(STM_SPI_INSTANCE *pSpiInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief       start spi rx
 * @param[in]   pSpiInfo  -  Pointer to spi instance struct
 * @param[in]   pData -  Pointer to data
 * @param[in]   numBytes - no of bytes
 * @return      status
 */
int32_t StmStartSpiRxDMA(STM_SPI_INSTANCE *pSpiInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief SPI Master Tx Rx API
 * @param[in]  pSpiInfo  - pointer to the spi info
 * @param[in]  pTxData  - pointer to Tx data
 * @param[in]  pRxData  - pointer to Rx data
 * @param[in]  numBytes  - num of bytes to send
 * @return  error or success
 */
int32_t StmStartSpiTxRxDMA(STM_SPI_INSTANCE *pSpiInfo, uint8_t *pTxData, uint8_t *pRxData,
                           uint32_t numBytes);
/**
 * @brief       set spi frequency
 * @param[in]   pSpiInfo  -  Pointer to spi instance struct
 * @param[in]   spiFrequency -  spi frequency in Hz
 * The API sets the nearest possible prescaler based on spiFRequency value
 * @return      status
 */
int32_t StmSetSpiFrequency(STM_SPI_INSTANCE *pSpiInfo, uint32_t spiFrequency);

/**
 * @brief       Slave select pin configuration
 * @param[in]   pSpiInfo  Pointer to spi instance struct
 * @return      status
 */
int32_t StmSlaveInitSpi(STM_SPI_INSTANCE *pSpiInfo);

/**
 * @brief       No OS ADC SPi Rx Callback
 */
void NoOsSpiRxCpltCallback(void);

/**
 * @brief Set GPIO pin for Chip Select
 *
 */
void StmSetChipSelect(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32H563XX_SPI_CONFIG_H__ */

/**
 * @}
 */
