/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     eval_ade9178_spi.c
 * @brief
 * @{
 */

/*============= I N C L U D E S =============*/

#include "adi_evb.h"
#include "app_cfg.h"
#include "dma.h"
#include "icc.h"
#include "max3267x_dma_config.h"
#include "max3267x_spi_config.h"
#include "max3267x_uart_config.h"
#include "mxc_device.h"
#include "nvic_table.h"
#include <stdint.h>
#include <stdio.h>

/*=============  D E F I N I T I O N S  =============*/

/** Evb spi info */
typedef struct
{
    /** maxim spi instance info */
    MAX_SPI_INSTANCE adeSpiInfo;
    /** callback */
    ADI_EVB_CALLBACK pfAdeSpiRxCallback;
    /** callback */
    ADI_EVB_CALLBACK pfAdeSpiTxCallback;

} EVB_SPI_INFO;

/** Spi irq handler */
static void SpiIrqHandler(void);
/** Tx dma irq handler */
static void SpiTxIrqHandler(void);
/** Rx dma irq handler */
static void SpiRxIrqHandler(void);
/** Setup ISRs */
static void SetupSpiIsr(void);
/** Evb spi init */
static int32_t InitAdeSpi(ADI_EVB_SPI_CONFIG *pConfig);

/*=============  D A T A  =============*/
/** Evb spi info */
static EVB_SPI_INFO evbSpiInfo;

/*=============  C O D E  =============*/

int32_t EvbInitSpi(void **phSpi, ADI_EVB_SPI_CONFIG *pConfig)
{
    int32_t status = 0;

    status = InitAdeSpi(pConfig);

    if (status == 0)
    {
        /* Just a keeping a API provision to support multiple SPIs later*/
        *phSpi = &evbSpiInfo.adeSpiInfo;
    }

    SetupSpiIsr();

    return status;
}

int32_t EvbAdeSpiTransmitAsync(void *hEvb, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = -1;
    MAX_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.adeSpiInfo;
    if (hEvb != NULL)
    {
        status = MaxStartSpiTxDMA(pSpiInfo, pData, numBytes);
    }

    return status;
}
int32_t EvbAdeSpiReceiveAsync(void *hEvb, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = -1;
    MAX_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.adeSpiInfo;
    if (hEvb != NULL)
    {
        status = MaxStartSpiRxDMA(pSpiInfo, pData, numBytes);
    }

    return status;
}
int32_t EvbAdeSpiSetFrequency(void *hEvb, uint32_t spiFrequency)
{
    int32_t status = -1;
    MAX_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.adeSpiInfo;
    if (hEvb != NULL)
    {
        status = MaxSetSpiFrequency(pSpiInfo, spiFrequency);
    }

    return status;
}

int32_t InitAdeSpi(ADI_EVB_SPI_CONFIG *pConfig)
{
    int32_t status = 0;
#if APP_CFG_ENABLE_ADE9178_SPI == 1
    MAX_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.adeSpiInfo;
    int32_t txChannel;
    int32_t rxChannel;
    evbSpiInfo.pfAdeSpiRxCallback = pConfig->pfAdeSpiRxCallback;
    evbSpiInfo.pfAdeSpiTxCallback = pConfig->pfAdeSpiTxCallback;
    txChannel = MXC_DMA_AcquireChannel();
    rxChannel = MXC_DMA_AcquireChannel();
    if ((txChannel < 0) || ((rxChannel < 0)))
    {
        status = ADI_EVB_STATUS_DMA_CHANNEL_ACQUIRE_ERROR;
    }
    else
    {

        pSpiInfo->txDmaDesc.channel = txChannel;
        pSpiInfo->rxDmaDesc.channel = rxChannel;
        pSpiInfo->numSlaves = 1;
        pSpiInfo->rxDmaDesc.source = MaxGetSpiDMAReqSelRx(BOARD_CFG_ADE9178_SPI);
        pSpiInfo->txDmaDesc.source = MaxGetSpiDMAReqSelTx(BOARD_CFG_ADE9178_SPI);
        pSpiInfo->spiSpeed = APP_CFG_ADE9178_SPI_SPEED;
        pSpiInfo->pSpi = BOARD_CFG_ADE9178_SPI;
        pSpiInfo->spiMode = APP_CFG_ADE9178_SPI_MODE;
        pSpiInfo->master = 1;
        status = MaxInitSpi(pSpiInfo);
    }

#endif /* APP_CFG_ENABLE_ADE9178_SPI */

    return status;
}

void SetupSpiIsr(void)
{
    IRQn_Type irqn;
    MAX_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.adeSpiInfo;

    /* SPI Isr */
    irqn = MXC_SPI_GET_IRQ(MXC_SPI_GET_IDX(BOARD_CFG_ADE9178_SPI));
    NVIC_EnableIRQ(irqn);
    NVIC_SetVector(irqn, (uint32_t)SpiIrqHandler);
    NVIC_SetPriority(irqn, APP_CFG_ADE9178_SPI_INT_PRIORITY);

    /* Spi DMA Tx ISR*/
    irqn = MaxGetDMAIRQn(pSpiInfo->txDmaDesc.channel);
    NVIC_EnableIRQ(irqn);
    NVIC_SetPriority(irqn, APP_CFG_ADE9178_SPI_TX_INT_PRIORITY);
    NVIC_SetVector(irqn, (uint32_t)SpiTxIrqHandler);

    /* Spi DMA Rx ISR*/
    irqn = MaxGetDMAIRQn(pSpiInfo->rxDmaDesc.channel);
    NVIC_EnableIRQ(irqn);
    NVIC_SetVector(irqn, (uint32_t)SpiRxIrqHandler);
    NVIC_SetPriority(irqn, APP_CFG_ADE9178_SPI_RX_INT_PRIORITY);
}

void SpiIrqHandler(void)
{
    uint32_t intStatus;
    MAX_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.adeSpiInfo;

    intStatus = pSpiInfo->pSpi->intfl;
    pSpiInfo->pSpi->intfl = intStatus; // clearing all pending interrupts(write 1 to clear)
}

void SpiTxIrqHandler(void)
{
    MAX_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.adeSpiInfo;

    pSpiInfo->txError = 0;
    /* 0x50 is the mask for timeout and Bus Error bits */
    if (MXC_DMA->ch[pSpiInfo->txDmaDesc.channel].status & 0x50)
    {
        pSpiInfo->txError = 1;
    }
    /* 0x5C is the mask for status register */
    MXC_DMA->ch[pSpiInfo->txDmaDesc.channel].status |= 0x5C;
    if (evbSpiInfo.pfAdeSpiTxCallback != NULL)
    {
        evbSpiInfo.pfAdeSpiTxCallback();
    }
}
void SpiRxIrqHandler(void)
{
    MAX_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.adeSpiInfo;

    pSpiInfo->rxError = 0;
    /* 0x50 is the mask for timeout and Bus Error bits */
    if (MXC_DMA->ch[pSpiInfo->rxDmaDesc.channel].status & 0x50)
    {
        pSpiInfo->rxError = 1;
    }
    if (evbSpiInfo.pfAdeSpiRxCallback != NULL)
    {
        evbSpiInfo.pfAdeSpiRxCallback();
    }
    /* 0x5C is the mask for status register */
    MXC_DMA->ch[pSpiInfo->rxDmaDesc.channel].status |= 0x5C;
}

/**
 * @}
 */
