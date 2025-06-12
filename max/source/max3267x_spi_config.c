/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     max3267x_spi_config.c
 * @brief    SPI configuration
 * @{
 */

/*============= I N C L U D E S =============*/
#include "max3267x_spi_config.h"
#include "dma.h"
#include "nvic_table.h"
#include "spi.h"
#include <stdint.h>
#include <stdio.h>

/*=============  D E F I N I T I O N S  =============*/
/** Initialise Spi */
static void InitSpi(void *pSpi, uint32_t master);
/** Start Spi Dma*/
static void StartSpiDMA(void *pSpi, MAX_DMA_DESC *pRx, MAX_DMA_DESC *pTx, uint32_t targetSelect);

/*=============  C O D E  =============*/
int32_t MaxInitSpi(MAX_SPI_INSTANCE *pSpiInfo)
{
    int32_t status = 0;

    pSpiInfo->pSpi->ctrl0 &= ~(MXC_F_SPI_CTRL0_EN);

    status = MXC_SPI_Init(pSpiInfo->pSpi, pSpiInfo->master, 0, pSpiInfo->numSlaves, 0,
                          pSpiInfo->spiSpeed);
    MXC_SPI_SetDataSize(pSpiInfo->pSpi, 8);
    if (status == 0)
    {
        status = MXC_SPI_SetWidth(pSpiInfo->pSpi, SPI_WIDTH_STANDARD);
    }
    if (status == 0)
    {
        status = MXC_SPI_SetMode(pSpiInfo->pSpi, pSpiInfo->spiMode);
    }
    if (status == 0)
    {
        MXC_SPI_SetTXThreshold(pSpiInfo->pSpi, 4);
        MXC_SPI_SetRXThreshold(pSpiInfo->pSpi, 0);
    }

    InitSpi(pSpiInfo->pSpi, pSpiInfo->master);
    MXC_DMA->inten |= ((1 << pSpiInfo->txDmaDesc.channel) | (1 << pSpiInfo->rxDmaDesc.channel));
    return status;
}

int32_t MaxSlaveInitSpi(MAX_SPI_INSTANCE *pSpiInfo)
{
    int32_t status = 0;

    pSpiInfo->pSpi->ctrl0 &= ~(MXC_F_SPI_CTRL0_EN);

    status = MXC_SPI_Init(pSpiInfo->pSpi, pSpiInfo->master, 0, pSpiInfo->numSlaves, 0,
                          pSpiInfo->spiSpeed);
    // MXC_SPI_SetDataSize(pSpiInfo->pSpi, 8);
    if (status == 0)
    {
        status = MXC_SPI_SetWidth(pSpiInfo->pSpi, SPI_WIDTH_STANDARD);
    }
    if (status == 0)
    {
        status = MXC_SPI_SetMode(pSpiInfo->pSpi, pSpiInfo->spiMode);
    }
    if (status == 0)
    {
        MXC_SPI_SetTXThreshold(pSpiInfo->pSpi, 4);
        MXC_SPI_SetRXThreshold(pSpiInfo->pSpi, 0);
    }

    InitSpi(pSpiInfo->pSpi, pSpiInfo->master);
    MXC_DMA->inten |= ((1 << pSpiInfo->txDmaDesc.channel) | (1 << pSpiInfo->rxDmaDesc.channel));
    return status;
}

int32_t MaxStartSpiTxDMA(MAX_SPI_INSTANCE *pSpiInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 0;

    MAX_DMA_DESC *pRx = &pSpiInfo->rxDmaDesc;
    MAX_DMA_DESC *pTx = &pSpiInfo->txDmaDesc;

    pRx->pMem = (uint8_t *)&pSpiInfo->dummyCmd;
    pRx->ctrl = (uint32_t)pRx->source | (MXC_DMA_WIDTH_BYTE << MXC_F_DMA_CTRL_SRCWD_POS);
    pRx->numBytes = numBytes;

    pTx->pMem = pData;
    pTx->ctrl = MXC_F_DMA_CTRL_SRCINC | (uint32_t)pTx->source |
                (MXC_DMA_WIDTH_BYTE << MXC_F_DMA_CTRL_SRCWD_POS);
    pTx->numBytes = numBytes;

    StartSpiDMA(pSpiInfo->pSpi, pRx, pTx, pSpiInfo->slaveIndex);

    return status;
}

int32_t MaxSlaveStartSpiTxDMA(MAX_SPI_INSTANCE *pSpiInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 0;

    MAX_DMA_DESC *pRx = &pSpiInfo->rxDmaDesc;
    MAX_DMA_DESC *pTx = &pSpiInfo->txDmaDesc;

    pRx->pMem = (uint8_t *)&pSpiInfo->dummyCmd;
    pRx->ctrl = (uint32_t)pRx->source | (MXC_DMA_WIDTH_BYTE << MXC_F_DMA_CTRL_SRCWD_POS);
    pRx->numBytes = numBytes;

    pTx->pMem = pData;
    pTx->ctrl = MXC_F_DMA_CTRL_SRCINC | (uint32_t)pTx->source |
                (MXC_DMA_WIDTH_BYTE << MXC_F_DMA_CTRL_SRCWD_POS);
    pTx->numBytes = numBytes;

    StartSpiDMA(pSpiInfo->pSpi, pRx, pTx, pSpiInfo->slaveIndex);

    return status;
}

int32_t MaxStartSpiRxDMA(MAX_SPI_INSTANCE *pSpiInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 0;

    MAX_DMA_DESC *pRx = &pSpiInfo->rxDmaDesc;
    MAX_DMA_DESC *pTx = &pSpiInfo->txDmaDesc;

    pRx->pMem = pData;
    pRx->ctrl = MXC_F_DMA_CTRL_DSTINC | (uint32_t)pRx->source |
                (MXC_DMA_WIDTH_BYTE << MXC_F_DMA_CTRL_SRCWD_POS);
    pRx->numBytes = numBytes;

    pTx->pMem = (uint8_t *)&pSpiInfo->dummyCmd;
    pTx->ctrl = pTx->source | (MXC_DMA_WIDTH_BYTE << MXC_F_DMA_CTRL_SRCWD_POS);
    pTx->numBytes = numBytes;

    StartSpiDMA(pSpiInfo->pSpi, pRx, pTx, pSpiInfo->slaveIndex);

    return status;
}

int32_t MaxSlaveStartSpiRxDMA(MAX_SPI_INSTANCE *pSpiInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 0;

    MAX_DMA_DESC *pRx = &pSpiInfo->rxDmaDesc;
    MAX_DMA_DESC *pTx = &pSpiInfo->txDmaDesc;

    pRx->pMem = pData;
    pRx->ctrl = MXC_F_DMA_CTRL_DSTINC | (uint32_t)pRx->source |
                (MXC_DMA_WIDTH_BYTE << MXC_F_DMA_CTRL_SRCWD_POS);
    pRx->numBytes = numBytes;

    pTx->pMem = (uint8_t *)&pSpiInfo->dummyCmd;
    pTx->ctrl = pTx->source | (MXC_DMA_WIDTH_BYTE << MXC_F_DMA_CTRL_SRCWD_POS);
    pTx->numBytes = numBytes;

    StartSpiDMA(pSpiInfo->pSpi, pRx, pTx, pSpiInfo->slaveIndex);

    return status;
}

int32_t MaxStartSpiTxRxDMA(MAX_SPI_INSTANCE *pSpiInfo, uint8_t *pTxData, uint8_t *pRxData,
                           uint32_t numBytes)
{
    int32_t status = 0;

    MAX_DMA_DESC *pRx = &pSpiInfo->rxDmaDesc;
    MAX_DMA_DESC *pTx = &pSpiInfo->txDmaDesc;

    pRx->pMem = pRxData;
    pRx->ctrl = MXC_F_DMA_CTRL_DSTINC | (uint32_t)pRx->source |
                (MXC_DMA_WIDTH_BYTE << MXC_F_DMA_CTRL_SRCWD_POS);
    pRx->numBytes = numBytes;

    pTx->pMem = pTxData;
    pTx->ctrl = MXC_F_DMA_CTRL_SRCINC | (uint32_t)pTx->source |
                (MXC_DMA_WIDTH_BYTE << MXC_F_DMA_CTRL_SRCWD_POS);
    pTx->numBytes = numBytes;

    StartSpiDMA(pSpiInfo->pSpi, pRx, pTx, pSpiInfo->slaveIndex);

    return status;
}

int32_t MaxSetSpiFrequency(MAX_SPI_INSTANCE *pSpiInfo, uint32_t spiFrequency)
{
    int32_t status = 0;

    pSpiInfo->pSpi->ctrl0 &= ~(MXC_F_SPI_CTRL0_EN);
    status = MXC_SPI_SetFrequency(pSpiInfo->pSpi, spiFrequency);
    pSpiInfo->pSpi->ctrl0 |= MXC_F_SPI_CTRL0_EN;

    return status;
}

void MaxClearSpiInterrupt(MAX_SPI_INSTANCE *pSpiInfo)
{
    mxc_spi_regs_t *spiInstance = (mxc_spi_regs_t *)pSpiInfo->pSpi;
    /* 0x5C is the mask for status register */
    MXC_DMA->ch[pSpiInfo->txDmaDesc.channel].status |= 0x5C;
    /* 0x5C is the mask for status register */
    MXC_DMA->ch[pSpiInfo->rxDmaDesc.channel].status |= 0x5C;
    // clear pending dma request
    spiInstance->dma &= ~(MXC_F_SPI_DMA_DMA_TX_EN | MXC_F_SPI_DMA_DMA_RX_EN);
}

void StartSpiDMA(void *pSpi, MAX_DMA_DESC *pRx, MAX_DMA_DESC *pTx, uint32_t targetSelect)
{
    mxc_spi_regs_t *spiInstance = (mxc_spi_regs_t *)pSpi;

    spiInstance->ctrl0 &= ~MXC_F_SPI_CTRL0_START;
    spiInstance->ctrl0 &= ~MXC_F_SPI_CTRL0_SS_ACTIVE;
    spiInstance->ctrl0 |= (unsigned int)(1 << targetSelect) << MXC_F_SPI_CTRL0_SS_ACTIVE_POS;

    // If the SPIn port is set to operate in 4-wire mode,
    // SPIn_CTRL1.rx_num_chars field is ignored and the SPIn_CTRL1.tx_num_chars
    // field is used for both the number of characters to receive and transmit.
    spiInstance->ctrl1 = (pTx->numBytes & MXC_F_SPI_CTRL1_TX_NUM_CHAR)
                         << MXC_F_SPI_CTRL1_TX_NUM_CHAR_POS;
    // flush tx and rx fifo
    spiInstance->dma |= (MXC_F_SPI_DMA_TX_FLUSH | MXC_F_SPI_DMA_RX_FLUSH);
    // Any pending DMA requests are cleared
    spiInstance->dma &= ~(MXC_F_SPI_DMA_DMA_TX_EN | MXC_F_SPI_DMA_DMA_RX_EN);

    MaxEnableDMAChannel(pRx);
    MaxEnableDMAChannel(pTx);

    spiInstance->dma |= (MXC_F_SPI_DMA_DMA_TX_EN | MXC_F_SPI_DMA_DMA_RX_EN);
    // start the spi Transaction
    spiInstance->ctrl0 |= MXC_F_SPI_CTRL0_START;
}

void InitSpi(void *pSpi, uint32_t master)
{

    mxc_spi_regs_t *spiInstance = (mxc_spi_regs_t *)pSpi;
    if (master == 1)
    {
        spiInstance->inten |= MXC_F_SPI_INTEN_TX_OV | MXC_F_SPI_INTEN_RX_OV;
    }
    else
    {
        spiInstance->inten |= MXC_F_SPI_INTEN_SSD;
    }
    spiInstance->ctrl0 &= ~MXC_F_SPI_CTRL0_EN;
    spiInstance->ctrl0 &= ~MXC_F_SPI_CTRL0_START;

    // flush tx and rx fifo
    spiInstance->dma |= (MXC_F_SPI_DMA_TX_FLUSH | MXC_F_SPI_DMA_RX_FLUSH);
    // Any pending DMA requests are cleared
    spiInstance->dma &= ~(MXC_F_SPI_DMA_DMA_TX_EN | MXC_F_SPI_DMA_DMA_RX_EN);
    spiInstance->dma |= (MXC_F_SPI_DMA_TX_FIFO_EN | MXC_F_SPI_DMA_RX_FIFO_EN);

    spiInstance->ctrl0 &= ~MXC_F_SPI_CTRL0_MST_MODE;
    spiInstance->ctrl0 |= ((uint32_t)(master << MXC_F_SPI_CTRL0_MST_MODE_POS));

    spiInstance->ctrl0 |= MXC_F_SPI_CTRL0_EN;
    spiInstance->intfl = MXC_F_SPI_INTFL_MST_DONE;
}

/**
 * @}
 */
