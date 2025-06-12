/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file  max3267x_spi_config.h
 * @brief User defined SPI APIs for max3267x platform
 * @defgroup    SPI drivers
 * @{
 */

#ifndef __MAX3267X_SPI_CONFIG_H__
#define __MAX3267X_SPI_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/*============= I N C L U D E S =============*/
#include "max3267x_dma_config.h"
#include "spi.h"
#include <stdint.h>

/*=============  D E F I N I T I O N S  =============*/
/**
 * @brief Maxim SPI instance info
 */
typedef struct
{
    /** Dummy command buffer is required during transmit and receive spi dma */
    uint32_t dummyCmd;
    /** rx dma channel */
    MAX_DMA_DESC rxDmaDesc;
    /** tx dma channel */
    MAX_DMA_DESC txDmaDesc;
    /** pointer to spi registers struct */
    mxc_spi_regs_t *pSpi;
    /** spi speed */
    uint32_t spiSpeed;
    /** no of slaves */
    uint8_t numSlaves;
    /** master or slave mode */
    uint8_t master;
    /** spi mode */
    uint8_t spiMode;
    /** slave index */
    uint8_t slaveIndex;
    /** flag for rx complete*/
    volatile uint8_t isRxComplete;
    /** flag for tx complete*/
    volatile uint8_t isTxComplete;
    /** flag for tx error*/
    volatile uint8_t txError;
    /** flag for rx error*/
    volatile uint8_t rxError;
    /** suspenState is used to suspend from the blocking state */
    volatile uint8_t suspendState;

} MAX_SPI_INSTANCE;

/*======= P U B L I C   P R O T O T Y P E S ========*/
/**
 * @brief Initialses Master SPI
 * @param[in]  pSpiInfo  - pointer to the spi info struct
 * @return  error or success
 */
int32_t MaxInitSpi(MAX_SPI_INSTANCE *pSpiInfo);

/**
 * @brief SPI Master Tx API
 * @param[in]  pSpiInfo  - pointer to the spi info
 * @param[in]  pData  - pointer to Tx data
 * @param[in]  numBytes  - num of bytes to send
 * @return  error or success
 */
int32_t MaxStartSpiTxDMA(MAX_SPI_INSTANCE *pSpiInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief SPI Master Rx API
 * @param[in]  pSpiInfo  - pointer to the spi info
 * @param[in]  pData  - pointer to Tx data
 * @param[in]  numBytes  - num of bytes to send
 * @return  error or success
 */
int32_t MaxStartSpiRxDMA(MAX_SPI_INSTANCE *pSpiInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief SPI Set frequency
 * @param[in]  pSpiInfo  - pointer to the spi info
 * @param[in]  spiFrequency  - spi frequency in Hz
 * @return  error or success
 */
int32_t MaxSetSpiFrequency(MAX_SPI_INSTANCE *pSpiInfo, uint32_t spiFrequency);

/**
 * @brief SPI clear interrupt
 * @param[in]  pSpiInfo  - pointer to the spi info
 */
void MaxClearSpiInterrupt(MAX_SPI_INSTANCE *pSpiInfo);

/**
 * @brief SPI Slave Init
 * @param[in]  pSpiInfo  - pointer to the spi info
 * @return  error or success
 */
int32_t MaxSlaveInitSpi(MAX_SPI_INSTANCE *pSpiInfo);

/**
 * @brief SPI Slave Tx API
 * @param[in]  pSpiInfo  - pointer to the spi info
 * @param[in]  pData  - pointer to Tx data
 * @param[in]  numBytes  - num of bytes to send
 * @return  error or success
 */
int32_t MaxSlaveStartSpiTxDMA(MAX_SPI_INSTANCE *pSpiInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief SPI Slave Rx API
 * @param[in]  pSpiInfo  - pointer to the spi info
 * @param[in]  pData  - pointer to Rx data
 * @param[in]  numBytes  - num of bytes to send
 * @return  error or success
 */
int32_t MaxSlaveStartSpiRxDMA(MAX_SPI_INSTANCE *pSpiInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief SPI Master Tx Rx API
 * @param[in]  pSpiInfo  - pointer to the spi info
 * @param[in]  pTxData  - pointer to Tx data
 * @param[in]  pRxData  - pointer to Rx data
 * @param[in]  numBytes  - num of bytes to send
 * @return  error or success
 */
int32_t MaxStartSpiTxRxDMA(MAX_SPI_INSTANCE *pSpiInfo, uint8_t *pTxData, uint8_t *pRxData,
                           uint32_t numBytes);

#ifdef __cplusplus
}
#endif

#endif /* __MAX3267X_SPI_CONFIG_H__ */

/**
 * @}
 */
