/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     max3267x_dma_config.c
 * @brief    User defined Dma APIs for max3267x platform
 * the protocol.
 * @{
 */

/*=============  I N C L U D E S   =============*/
#include "max3267x_dma_config.h"
#include "mxc_device.h"
#include <stdint.h>
/*=============  D A T A  =============*/

/*=============  D E F I N I T I O N S  =============*/

/*=============  C O D E  =============*/

void MaxEnableDMAChannel(MAX_DMA_DESC *pDesc)
{
    MXC_DMA->ch[pDesc->channel].ctrl &= ~(MXC_F_DMA_CTRL_EN | MXC_F_DMA_CTRL_CTZ_IE);
    MXC_DMA->ch[pDesc->channel].ctrl = pDesc->ctrl;
    /* We are good assigning both as src is ignored for Rx channel
         and dst is ignored for Tx channel */
    MXC_DMA->ch[pDesc->channel].src = (unsigned int)(pDesc->pMem);
    MXC_DMA->ch[pDesc->channel].dst = (unsigned int)(pDesc->pMem);
    MXC_DMA->ch[pDesc->channel].cnt = pDesc->numBytes;
    MXC_DMA->ch[pDesc->channel].ctrl |= (MXC_F_DMA_CTRL_EN | MXC_F_DMA_CTRL_CTZ_IE);
}

IRQn_Type MaxGetDMAIRQn(int channel)
{
    IRQn_Type irqN;
    switch (channel)
    {
    case 0:
        irqN = DMA0_IRQn;
        break;
    case 1:
        irqN = DMA1_IRQn;
        break;
    case 2:
        irqN = DMA2_IRQn;
        break;
    case 3:
        irqN = DMA3_IRQn;
        break;
    case 4:
        irqN = DMA4_IRQn;
        break;
    case 5:
        irqN = DMA5_IRQn;
        break;
    case 6:
        irqN = DMA6_IRQn;
        break;
    case 7:
        irqN = DMA7_IRQn;
        break;
    default:
        irqN = MXC_IRQ_EXT_COUNT;
        break;
    }
    return irqN;
}

mxc_dma_reqsel_t MaxGetSpiDMAReqSelRx(mxc_spi_regs_t *pSpi)
{
    int32_t spiNum;
    mxc_dma_reqsel_t reqSel;
    spiNum = MXC_SPI_GET_IDX(pSpi);
    switch (spiNum)
    {
    case 0:
        reqSel = MXC_DMA_REQUEST_SPI0RX;
        break;

    case 1:
        reqSel = MXC_DMA_REQUEST_SPI1RX;
        break;

    case 2:
        reqSel = MXC_DMA_REQUEST_SPI2RX;
        break;

    default:
        reqSel = MXC_DMA_REQUEST_SPI0RX;
        break;
    }

    return reqSel;
}

mxc_dma_reqsel_t MaxGetUartDMAReqSelRx(mxc_uart_regs_t *pUart)
{
    int32_t uartNum;
    mxc_dma_reqsel_t reqSel;
    uartNum = MXC_UART_GET_IDX(pUart);
    switch (uartNum)
    {
    case 0:
        reqSel = MXC_DMA_REQUEST_UART0RX;
        break;

    case 1:
        reqSel = MXC_DMA_REQUEST_UART1RX;
        break;

    case 2:
        reqSel = MXC_DMA_REQUEST_UART2RX;
        break;
    default:
        reqSel = MXC_DMA_REQUEST_UART0RX;
        break;
    }

    return reqSel;
}

mxc_dma_reqsel_t MaxGetSpiDMAReqSelTx(mxc_spi_regs_t *pSpi)
{
    int32_t spiNum;
    mxc_dma_reqsel_t reqSel;
    spiNum = MXC_SPI_GET_IDX(pSpi);
    switch (spiNum)
    {
    case 0:
        reqSel = MXC_DMA_REQUEST_SPI0TX;
        break;

    case 1:
        reqSel = MXC_DMA_REQUEST_SPI1TX;
        break;

    case 2:
        reqSel = MXC_DMA_REQUEST_SPI2TX;
        break;

    default:
        reqSel = MXC_DMA_REQUEST_SPI0TX;
        break;
    }

    return reqSel;
}

mxc_dma_reqsel_t MaxGetUartDMAReqSelTx(mxc_uart_regs_t *pUart)
{
    int32_t uartNum;
    mxc_dma_reqsel_t reqSel;
    uartNum = MXC_UART_GET_IDX(pUart);
    switch (uartNum)
    {
    case 0:
        reqSel = MXC_DMA_REQUEST_UART0TX;
        break;

    case 1:
        reqSel = MXC_DMA_REQUEST_UART1TX;
        break;

    case 2:
        reqSel = MXC_DMA_REQUEST_UART2TX;
        break;
    default:
        reqSel = MXC_DMA_REQUEST_UART0TX;
        break;
    }

    return reqSel;
}

/**
 * @}
 */
