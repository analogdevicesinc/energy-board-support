/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     eval_ade9178_uart.c
 * @brief    This file contains the routines for initializing,configuring and
 * staring DMA for receiving waveform samples.
 * @{
 */

/*=============  I N C L U D E S   =============*/

#include "adi_evb.h"
#include "app_cfg.h"
#include "dma.h"
#include "max3267x_uart_config.h"
#include "mxc_delay.h"
#include "nvic_table.h"
#include "uart.h"
#include "uart_revb_regs.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/*=============  D E F I N I T I O N S  =============*/
/** Evb uart info */
typedef struct
{
    /** maxim uart info */
    MAX_UART_INSTANCE wfsUartInfo;
    /** maxim uart info */
    MAX_UART_INSTANCE hostUartInfo;
    /** callback */
    ADI_EVB_CALLBACK pfWfsRxCallback;
    /** callback */
    ADI_EVB_CALLBACK pfHostRxCallback;
    /** callback */
    ADI_EVB_CALLBACK pfHostTxCallback;

} EVB_UART_INFO;

/*=============  D A T A  =============*/
/** Evb uart info */
static EVB_UART_INFO evbUartInfo;
static ADI_EVB_CALLBACK pUartRxCallBack;
/*=============  C O D E  =============*/

#if APP_CFG_ENABLE_WFS_UART == 1
/** Initialse uart for Wfs */
static int32_t InitWfsUart(ADI_EVB_UART_CONFIG *pConfig);
/** Set up uart isr */
static void SetupWfsUartIsr(void);
/** Uart rx dma handler */
static void WfsUartRxDmaIrqHandler(void);
/** Uart irq handler */
static void WfsUartIrqHandler(void);
#endif /* APP_CFG_ENABLE_HOST_UART */

#if APP_CFG_ENABLE_HOST_UART == 1
/** Uart rx dma handler */
static void HostUartRxIrqHandler(void);
/** Uart tx dma handler */
static void HostUartTxIrqHandler(void);
/** Initialse uart for host */
static int32_t InitHostUart(ADI_EVB_UART_CONFIG *pConfig);
/** Set up uart isr */
static void SetupHostUartIsr(void);
static void HostUartRxCallBack(mxc_uart_req_t *req, int result);

#endif /* APP_CFG_ENABLE_HOST_UART */

int32_t EvbInitUart(void **phUart, ADI_EVB_UART_CONFIG *pConfig)
{
    int32_t status = 0;
#if APP_CFG_ENABLE_HOST_UART == 1
    status = InitHostUart(pConfig);
#endif /* APP_CFG_ENABLE_HOST_UART */
#if APP_CFG_ENABLE_WFS_UART == 1
    if (status == 0)
    {
        status = InitWfsUart(pConfig);
    }
#endif /* APP_CFG_ENABLE_HOST_UART */
    if (status == 0)
    {
        *phUart = &evbUartInfo;
    }

    return status;
}

int32_t EvbAdeWfsUartReceiveAsync(void *hEvb, uint8_t *pBuffer, uint32_t numBytes)
{
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.wfsUartInfo;
    int32_t status = -1;
    pUartInfo->isRxComplete = 0;
    if (hEvb != NULL)
    {
        status = MaxStartUartRxDMA(pUartInfo, pBuffer, numBytes);
    }

    return status;
}

int32_t EvbAdeWfsUartSetBaudrate(void *hEvb, uint32_t baudRate)
{
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.wfsUartInfo;
    int32_t status = -1;
    if (hEvb != NULL)
    {
        status = MaxSetUartBaudrate(pUartInfo, baudRate);
    }

    return status;
}
int32_t EvbHostUartReceive(void *hEvb, uint8_t *pBuffer, uint32_t numBytes, uint32_t timeoutCount)
{
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    int32_t status = -1;
    uint32_t waitCount = 0;
    pUartInfo->isRxComplete = 0;
    if (hEvb != NULL)
    {
        status = MaxStartUartRx(pUartInfo, pBuffer, numBytes);
        while ((pUartInfo->isRxComplete == 0) && (waitCount < timeoutCount))
        {
            waitCount++;
        }
        if (waitCount == timeoutCount)
        {
            status = 1;
        }
    }

    return status;
}

int32_t EvbHostUartReceiveAsync(void *hEvb, uint8_t *pBuffer, uint32_t numBytes)
{
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    int32_t status = -1;

    if (hEvb != NULL)
    {
        while (pUartInfo->isRxComplete == 0)
        {
        }
        pUartInfo->isRxComplete = 0;
        status = MaxStartUartRx(pUartInfo, pBuffer, numBytes);
    }

    return status;
}

int32_t EvbHostUartTransmit(void *hEvb, uint8_t *pBuffer, uint32_t numBytes, uint32_t timeoutCount)
{
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    int32_t status = -1;
    pUartInfo->isTxComplete = 0;
    uint32_t waitCount = 0;
    if (hEvb != NULL)
    {
        status = MaxStartUartTxDMA(pUartInfo, pBuffer, numBytes);
        while ((pUartInfo->isTxComplete == 0) && (waitCount < timeoutCount))
        {
            waitCount++;
        }
        if (waitCount == timeoutCount)
        {
            status = 1;
        }
    }
    return status;
}

int32_t EvbHostUartTransmitAsync(void *hEvb, uint8_t *pBuffer, uint32_t numBytes)
{
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    int32_t status = -1;
    if (hEvb != NULL)
    {
        while (pUartInfo->isTxComplete == 0)
        {
        }
        pUartInfo->isTxComplete = 0;
        status = MaxStartUartTxDMA(pUartInfo, pBuffer, numBytes);
    }

    return status;
}

int32_t EvbPutBufferNb(uint8_t *pBuffer, uint32_t numBytes)
{
    int32_t status;
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    status = EvbHostUartTransmitAsync((void *)pUartInfo, pBuffer, numBytes);
    // FIXME : Without delay control sequences are displaying in raw message.
    MXC_Delay(MXC_DELAY_MSEC(1));
    return status;
}

int32_t EvbPutBuffer(uint8_t *pBuffer, uint32_t numBytes, uint32_t timeoutCount)
{
    int32_t status;
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;

    status = EvbHostUartTransmit((void *)pUartInfo, pBuffer, numBytes, timeoutCount);
    // FIXME : Without delay control sequences are displaying in raw message.
    MXC_Delay(MXC_DELAY_MSEC(1));
    return status;
}

int32_t EvbGetTxStatus()
{
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;

    return (int32_t)pUartInfo->isTxComplete;
}

#if APP_CFG_ENABLE_HOST_UART == 1
int32_t InitHostUart(ADI_EVB_UART_CONFIG *pConfig)
{
    int32_t status = 0;
    int32_t channel;
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    pUartInfo->isTxComplete = 1;
    pUartInfo->isRxComplete = 1;
    evbUartInfo.pfHostRxCallback = pConfig->pfHostUartRxCallback;
    evbUartInfo.pfHostTxCallback = pConfig->pfHostUartTxCallback;

    channel = MXC_DMA_AcquireChannel();
    if (channel < 0)
    {
        status = ADI_EVB_STATUS_DMA_CHANNEL_ACQUIRE_ERROR;
    }
    else
    {
        pUartInfo->txDmaDesc.channel = channel;
        pUartInfo->rxDmaDesc.channel = (uint32_t)NULL;
        pUartInfo->rxDmaDesc.source = (uint32_t)NULL;
        pUartInfo->txDmaDesc.source = MaxGetUartDMAReqSelTx(BOARD_CFG_HOST_UART);
        pUartInfo->baudrate = APP_CFG_HOST_UART_SPEED;
        pUartInfo->pUart = BOARD_CFG_HOST_UART;
        pUartRxCallBack = pConfig->pfHostUartRxCallback;
        pUartInfo->pfRxCallback = HostUartRxCallBack;

        SetupHostUartIsr();
        status = MaxInitUart(pUartInfo);
    }
    return status;
}

void SetupHostUartIsr(void)
{
    IRQn_Type irqn;
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;

    /* uart Isr */
    irqn = MXC_UART_GET_IRQ(MXC_UART_GET_IDX(BOARD_CFG_HOST_UART));
    NVIC_SetVector(irqn, (uint32_t)HostUartRxIrqHandler);
    NVIC_SetPriority(irqn, APP_CFG_UART_INTR_PRIORITY);
    NVIC_EnableIRQ(irqn);

    /* uart DMA Tx ISR*/
    irqn = MaxGetDMAIRQn(pUartInfo->txDmaDesc.channel);
    NVIC_SetPriority(irqn, APP_CFG_UART_INTR_PRIORITY);
    NVIC_SetVector(irqn, (uint32_t)HostUartTxIrqHandler);
    NVIC_EnableIRQ(irqn);
}

void HostUartRxIrqHandler(void)
{
    MXC_UART_AsyncHandler(BOARD_CFG_HOST_UART);
}

void HostUartTxIrqHandler(void)
{
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    MXC_DMA_Handler();
    pUartInfo->isTxComplete = 1;
    if (evbUartInfo.pfHostTxCallback != NULL)
    {
        evbUartInfo.pfHostTxCallback();
    }
}
void HostUartRxCallBack(mxc_uart_req_t *req, int result)
{
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    pUartInfo->isRxComplete = 1;
    // Added code to resolve warning
    if (req != NULL && result != 0)
    {
    }

    pUartRxCallBack();
}

#endif /* APP_CFG_ENABLE_HOST_UART */

#if APP_CFG_ENABLE_WFS_UART == 1
int32_t InitWfsUart(ADI_EVB_UART_CONFIG *pConfig)
{
    int32_t status = 0;
    int32_t channel;

    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.wfsUartInfo;
    evbUartInfo.pfWfsRxCallback = pConfig->pfWfsUartRxCallback;
    channel = MXC_DMA_AcquireChannel();
    if (channel < 0)
    {
        status = ADI_EVB_STATUS_DMA_CHANNEL_ACQUIRE_ERROR;
    }
    else
    {
        pUartInfo->rxDmaDesc.channel = channel;
        pUartInfo->txDmaDesc.channel = (uint32_t)NULL;
        pUartInfo->rxDmaDesc.source = MaxGetUartDMAReqSelRx(BOARD_CFG_WFS_UART);
        pUartInfo->txDmaDesc.source = (uint32_t)NULL;
        pUartInfo->baudrate = 115200;
        pUartInfo->pUart = BOARD_CFG_WFS_UART;

        status = MaxInitUart(pUartInfo);

        SetupWfsUartIsr();
    }
    return status;
}

void SetupWfsUartIsr(void)
{
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.wfsUartInfo;
    int32_t uartIndex;
    IRQn_Type irqn;
    uartIndex = MXC_UART_GET_IDX(BOARD_CFG_WFS_UART);
    /* SPI Isr */
    irqn = MXC_UART_GET_IRQ(uartIndex);
    NVIC_SetVector(irqn, (uint32_t)WfsUartIrqHandler);
    NVIC_SetPriority(irqn, APP_CFG_WFS_ERROR_INT_PRIORITY);
    NVIC_EnableIRQ(irqn);

    /* Spi DMA Tx ISR*/
    irqn = MaxGetDMAIRQn(pUartInfo->rxDmaDesc.channel);
    NVIC_SetPriority(irqn, APP_CFG_WFS_UART_INT_PRIORITY);
    NVIC_SetVector(irqn, (uint32_t)WfsUartRxDmaIrqHandler);
    NVIC_EnableIRQ(irqn);
}

void WfsUartIrqHandler(void)
{
    uint32_t intrpStatus;

    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.wfsUartInfo;

    intrpStatus = pUartInfo->pUart->int_fl;
    pUartInfo->pUart->int_fl = intrpStatus; // clearing all pending interrupts(write 1 to clear)
}

void WfsUartRxDmaIrqHandler(void)
{
    MAX_UART_INSTANCE *pUartInfo = &evbUartInfo.wfsUartInfo;
    MAX_DMA_DESC *pDesc = &pUartInfo->rxDmaDesc;
    pUartInfo->isRxComplete = 1;
    pUartInfo->rxError = 0;
    /* 0x50 is the mask for timeout and Bus Error bits */
    if (MXC_DMA->ch[pDesc->channel].status & 0x50)
    {
        pUartInfo->rxError = 1;
    }
    /* 0x5C is the mask for status register */
    MXC_DMA->ch[pDesc->channel].status |= 0x5C;
    if (evbUartInfo.pfWfsRxCallback != NULL)
    {
        evbUartInfo.pfWfsRxCallback();
    }
}
#endif /* APP_CFG_ENABLE_WFS_UART */

/**
 * @}
 */
