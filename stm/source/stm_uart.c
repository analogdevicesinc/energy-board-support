/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     stm_uart.c
 * @brief    This file contains the routines for initializing,configuring and
 * staring DMA for receiving waveform samples.
 * @{
 */

/*=============  I N C L U D E S   =============*/

#include "adi_evb.h"
#include "app_cfg.h"
#ifdef STM32H735xx
#include "stm32h735xx_uart_config.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_it.h"
#else
#include "stm32h563xx_uart_config.h"
#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_uart.h"
#include "stm32h5xx_it.h"
#endif
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/** Evb uart info */
typedef struct
{
    /** maxim uart info */
    STM_UART_INSTANCE wfsUartInfo;
    /** maxim uart info */
    STM_UART_INSTANCE hostUartInfo;
    /** callback */
    ADI_EVB_CALLBACK pfWfsRxCallback;
    /** callback */
    ADI_EVB_CALLBACK pfHostRxCallback;
    /** callback */
    ADI_EVB_CALLBACK pfHostTxCallback;

} EVB_UART_INFO;

/*=============  D A T A  =============*/

static EVB_UART_INFO evbUartInfo;
static void AdeUartRxCallBack(UART_HandleTypeDef *huart);
static void AdeUartTxCallBack(UART_HandleTypeDef *huart);

/** creating uart instance*/
UART_HandleTypeDef hUART;
/** creating dma uarttx instance*/
DMA_HandleTypeDef hDMAUARTTx;
/**
 * @brief - Rx Transfer completed callback.
 * @param[in] huart - Pointer to a UART_HandleTypeDef structure that contains
 * the configuration information for the specified UART module.
 * \ref HAL_UART_RxCpltCallback
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

/**
 * @brief Tx Transfer completed callbacks.
 * @param[in] huart - Pointer to a UART_HandleTypeDef structure that contains
 *                    the configuration information for the specified UART
 * module. \ref HAL_UART_TxCpltCallback
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);

/** This is only used in HAL. We are not doing anything here.
 If there is an error  from use this for debugging.
 */
void ErrorHandler(void);

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
/** Initialse uart for host */
static int32_t InitHostUart(ADI_EVB_UART_CONFIG *pConfig);

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
    STM_UART_INSTANCE *pUartInfo = &evbUartInfo.wfsUartInfo;
    int32_t status = -1;
    pUartInfo->isRxComplete = 0;
    if (hEvb != NULL)
    {
        status = StmStartUartRx(pUartInfo, pBuffer, numBytes);
    }

    return status;
}

int32_t EvbAdeWfsUartSetBaudrate(void *hEvb, uint32_t baudRate)
{
    STM_UART_INSTANCE *pUartInfo = &evbUartInfo.wfsUartInfo;
    int32_t status = -1;
    if (hEvb != NULL)
    {
        status = StmSetUartBaudrate(pUartInfo, baudRate);
    }

    return status;
}
int32_t EvbHostUartReceive(void *hEvb, uint8_t *pBuffer, uint32_t numBytes, uint32_t timeoutCount)
{
    STM_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    int32_t status = -1;
    uint32_t waitCount = 0;
    pUartInfo->isRxComplete = 0;
    if (hEvb != NULL)
    {
        status = StmStartUartRx(pUartInfo, pBuffer, numBytes);
        while ((pUartInfo->isRxComplete == 0) && (waitCount < timeoutCount))
        {
            // Wait for the previous transmission to complete.
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
    STM_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    int32_t status = -1;
    if (hEvb != NULL)
    {
        while (pUartInfo->isRxComplete == 0)
        {
            // Wait for the previous transmission to complete.
        }
        pUartInfo->isRxComplete = 0;
        status = StmStartUartRx(pUartInfo, pBuffer, numBytes);
    }

    return status;
}

int32_t EvbHostUartTransmit(void *hEvb, uint8_t *pBuffer, uint32_t numBytes, uint32_t timeoutCount)
{
    STM_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    int32_t status = -1;
    (void)timeoutCount; /* Dummy use of argument */
    pUartInfo->isTxComplete = 0;
    int32_t bytesRemaining = (uint32_t)numBytes;
    int32_t blockSize = 0xFFFF;

    if (hEvb != NULL)
    {
        while (bytesRemaining > 0)
        {
            if (bytesRemaining < blockSize)
            {
                /*Last block*/
                blockSize = bytesRemaining;
            }
            status = StmStartUartTx(pUartInfo, pBuffer, (uint32_t)blockSize);
            while (pUartInfo->isTxComplete == 0)
            {
                // Wait for the previous transmission to complete.
            }

            bytesRemaining -= blockSize;
            pBuffer += blockSize;
        }
    }

    return status;
}

int32_t EvbHostUartTransmitAsync(void *hEvb, uint8_t *pBuffer, uint32_t numBytes)
{
    STM_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    int32_t status = -1;
    if (hEvb != NULL)
    {
        // Previous transaction should be complete before starting a new one. In RTOS environment,
        // this will be replaced with pend.
        while (pUartInfo->isTxComplete == 0)
        {
            // Wait for the previous transmission to complete.
        }
        pUartInfo->isTxComplete = 0;
        status = StmStartUartTx(pUartInfo, pBuffer, numBytes);
    }

    return status;
}

int32_t EvbGetTxStatus(void)
{
    STM_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;

    return (int32_t)pUartInfo->isTxComplete;
}

#if APP_CFG_ENABLE_HOST_UART == 1
int32_t InitHostUart(ADI_EVB_UART_CONFIG *pConfig)
{
    int32_t status = 0;
    STM_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    evbUartInfo.pfHostRxCallback = pConfig->pfHostUartRxCallback;
    evbUartInfo.pfHostTxCallback = pConfig->pfHostUartTxCallback;

    pUartInfo->pUsart = BOARD_CFG_HOST_UART;
    pUartInfo->baudrate = APP_CFG_HOST_UART_SPEED;
    pUartInfo->isTxComplete = 1;
    pUartInfo->isRxComplete = 1;
    status = StmInitUart(pUartInfo);
    HAL_UART_RegisterCallback(&hUART, HAL_UART_TX_COMPLETE_CB_ID, AdeUartTxCallBack);
    HAL_UART_RegisterCallback(&hUART, HAL_UART_RX_COMPLETE_CB_ID, AdeUartRxCallBack);
    return status;
}
#endif /* APP_CFG_ENABLE_HOST_UART */

/** This is only used in HAL. We are not doing anything here.
 If there is an error  from use this for debugging.*/
void __attribute__((noreturn)) ErrorHandler(void)
{
    while (1)
    {
    }
}

#if APP_CFG_ENABLE_WFS_UART == 1
int32_t InitWfsUart(ADI_EVB_UART_CONFIG *pConfig)
{
    int32_t status = 0;
    int32_t channel;
    STM_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    evbUartInfo.pfHostRxCallback = pConfig->pfHostUartRxCallback;
    evbUartInfo.pfHostTxCallback = pConfig->pfHostUartTxCallback;
    status = StmInitUart(pUartInfo);
    return status;
}

#endif /* APP_CFG_ENABLE_WFS_UART */

void AdeUartRxCallBack(UART_HandleTypeDef *huart)
{
    STM_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    pUartInfo->isRxComplete = 1;
    if (huart != NULL)
    {
        if (evbUartInfo.pfHostRxCallback != NULL)
        {
            evbUartInfo.pfHostRxCallback();
        }
    }
}

void AdeUartTxCallBack(UART_HandleTypeDef *huart)
{
    STM_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    pUartInfo->isTxComplete = 1;
    pUartInfo->txError = 0;

    if (huart != NULL)
    {
        if (evbUartInfo.pfHostTxCallback != NULL)
        {
            evbUartInfo.pfHostTxCallback();
        }
    }
}

/**
 * @}
 */
