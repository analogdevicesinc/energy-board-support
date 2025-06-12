/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     max3267x_uart_config.c
 * @brief    UART communication configuration
 *           in MAX32670
 * @{
 */

/*=============  I N C L U D E S   =============*/
#include "max3267x_uart_config.h"
#include "dma.h"
#include "mxc_device.h"
#include "nvic_table.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*============= D A T A T Y P E S ===========*/

/*============= P R O T O T Y P E S =============*/

/*=============  D A T A  =============*/
/** uart request */
static mxc_uart_req_t readReq;
/** DMA configuration */
static mxc_dma_config_t config;
/** DMA source/destination */
mxc_dma_srcdst_t srcdst;

/*=============  C O D E  =============*/

/**
 * @brief Serial communication initialization function.
 * @return 0 -  Success, <0 = Failed to initialize
 */
int32_t MaxInitUart(MAX_UART_INSTANCE *pUartInfo)
{
    int32_t status = 0;

    status = MXC_UART_Init(pUartInfo->pUart, pUartInfo->baudrate, MXC_UART_APB_CLK);
    if (status == 0)
    {
        MXC_DMA->inten |=
            ((1 << pUartInfo->txDmaDesc.channel) | (1 << pUartInfo->rxDmaDesc.channel));
    }

    return status;
}

int32_t MaxStartUartRx(MAX_UART_INSTANCE *pUartInfo, uint8_t *pData, uint32_t length)
{
    int32_t status;
    mxc_uart_req_t *pReadReq = &readReq;

    pReadReq->uart = pUartInfo->pUart;
    pReadReq->rxData = pData;
    pReadReq->rxLen = length;
    pReadReq->txLen = 0;
    pReadReq->txData = NULL;
    pReadReq->callback = pUartInfo->pfRxCallback;

    status = MXC_UART_TransactionAsync(pReadReq);

    return status;
}

int32_t MaxStartUartTxDMA(MAX_UART_INSTANCE *pUartInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 0;

    srcdst.ch = pUartInfo->txDmaDesc.channel;
    srcdst.source = pData;
    srcdst.dest = NULL;
    srcdst.len = numBytes;

    config.ch = pUartInfo->txDmaDesc.channel;
    config.reqsel = pUartInfo->txDmaDesc.source;
    config.srcwd = MXC_DMA_WIDTH_BYTE;
    config.srcinc_en = 1;
    config.dstinc_en = 0;

    MXC_DMA_ConfigChannel(config, srcdst);
    MXC_DMA_SetSrcDst(srcdst);
    MXC_DMA_SetChannelInterruptEn(pUartInfo->txDmaDesc.channel, true, true);

    // Enable the Tx DMA and set the DMA Tx thd.
    pUartInfo->pUart->dma |= MXC_F_UART_DMA_TX_EN;
    pUartInfo->pUart->dma |= (2 << MXC_F_UART_DMA_TX_THD_VAL_POS);

    MXC_DMA_Start(pUartInfo->txDmaDesc.channel);

    return status;
}
int32_t MaxStartUartRxDMA(MAX_UART_INSTANCE *pUartInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 0;

    srcdst.ch = pUartInfo->rxDmaDesc.channel;
    srcdst.source = NULL;
    srcdst.dest = pData;
    srcdst.len = numBytes;

    config.ch = pUartInfo->rxDmaDesc.channel;
    config.reqsel = pUartInfo->rxDmaDesc.source;
    config.srcwd = MXC_DMA_WIDTH_BYTE;
    config.srcinc_en = 0;
    config.dstinc_en = 1;

    MXC_DMA_ConfigChannel(config, srcdst);
    MXC_DMA_SetSrcDst(srcdst);
    MXC_DMA_EnableInt(pUartInfo->rxDmaDesc.channel);
    MXC_DMA_SetChannelInterruptEn(pUartInfo->rxDmaDesc.channel, true, true);

    // Enable the Rx DMA and set the DMA Rx thd.
    pUartInfo->pUart->dma |= MXC_F_UART_DMA_RX_EN;
    pUartInfo->pUart->dma |= (1 << MXC_F_UART_DMA_RX_THD_VAL_POS);

    MXC_DMA_Start(pUartInfo->rxDmaDesc.channel);

    return status;
}

int32_t MaxSetUartBaudrate(MAX_UART_INSTANCE *pUartInfo, uint32_t baudRate)
{
    int32_t status = 0;
    MXC_UART_SetFrequency(pUartInfo->pUart, baudRate, MXC_UART_APB_CLK);
    // Flush the Rx Data that's pending after changing the baudrate
    pUartInfo->pUart->ctrl |= MXC_F_UART_CTRL_RX_FLUSH;
    return status;
}

/**
 * @}
 */
