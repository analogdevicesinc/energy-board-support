/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        max3267x_dma_config.h
 * @brief      User defined Dma APIs for max3267x platform
 * @addtogroup SPI drivers
 * @{
 */

#ifndef __MAX3267X_DMA_CONFIG__
#define __MAX3267X_DMA_CONFIG__

#ifdef __cplusplus
extern "C" {
#endif

/*============= I N C L U D E S =============*/
#include "dma.h"
#include "spi.h"
#include "uart.h"
#include <stdint.h>

/*=============  D E F I N I T I O N S  =============*/
/** DMA channel info */
typedef struct
{
    /** ctrl */
    uint32_t ctrl;
    /** channel */
    int32_t channel;
    /** type of dma request */
    uint32_t source;
    /** pointer to data to transfer */
    uint8_t *pMem;
    /** num of bytes */
    uint32_t numBytes;

} MAX_DMA_DESC;

/*======= P U B L I C   P R O T O T Y P E S ========*/
/**
 * @brief Gets the IRQn for a DMA channel
 * @param channel - DMA channel
 * @return IRQn of the selected DMA channel
 */
IRQn_Type MaxGetDMAIRQn(int channel);

/**
 * @brief Enables dma channel
 * @param pDesc - pointer to dma channel info struct
 */
void MaxEnableDMAChannel(MAX_DMA_DESC *pDesc);

/**
 * @brief Gets request type of spi
 * @param pSpi - pointer to spi registers
 * @return request type
 */
mxc_dma_reqsel_t MaxGetSpiDMAReqSelRx(mxc_spi_regs_t *pSpi);

/**
 * @brief Gets request type for uart
 * @param pSpi - pointer to spi registers
 * @return request type
 */
mxc_dma_reqsel_t MaxGetUartDMAReqSelRx(mxc_uart_regs_t *pSpi);

/**
 * @brief Gets request type for spi dma
 * @param pSpi - pointer to spi registers
 * @return request type
 */
mxc_dma_reqsel_t MaxGetSpiDMAReqSelTx(mxc_spi_regs_t *pSpi);

/**
 * @brief Gets request type for uart dma
 * @param pSpi - pointer to spi registers
 * @return request type
 */
mxc_dma_reqsel_t MaxGetUartDMAReqSelTx(mxc_uart_regs_t *pSpi);

#ifdef __cplusplus
}
#endif

#endif /* __MAX3267X_DMA_CONFIG__ */

/**
 * @}
 */
