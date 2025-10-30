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
    STM_UART_INSTANCE hostUartInfo;
    /** callback */
    ADI_EVB_HOST_RX_CALLBACK pfHostRxCallback;
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

/*=============  C O D E  =============*/

/** Initialse uart for host */
static int32_t InitHostUart(ADI_EVB_HOST_COMM_CONFIG *pConfig);

int32_t EvbInitHostComm(void **phHostComm, ADI_EVB_HOST_COMM_CONFIG *pConfig)
{
    int32_t status = 0;
    status = InitHostUart(pConfig);
    if (status == 0)
    {
        *phHostComm = &evbUartInfo;
    }

    return status;
}

int32_t EvbHostCommReceive(void *hEvb, uint8_t *pBuffer, uint32_t numBytes, uint32_t timeoutCount)
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

int32_t EvbHostCommReceiveAsync(void *hEvb, uint8_t *pBuffer, uint32_t numBytes)
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

int32_t EvbHostCommTransmit(void *hEvb, uint8_t *pBuffer, uint32_t numBytes, uint32_t timeoutCount)
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

int32_t EvbHostCommTransmitAsync(void *hEvb, uint8_t *pBuffer, uint32_t numBytes)
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

int32_t EvbHostCommGetTxStatus(void)
{
    STM_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;

    return (int32_t)pUartInfo->isTxComplete;
}

void EvbHostCommTasksRun(void)
{
    // This function is not required for UART implementation as UART events are handled by
    // interrupts.
}

int32_t InitHostUart(ADI_EVB_HOST_COMM_CONFIG *pConfig)
{
    int32_t status = 0;
    STM_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    evbUartInfo.pfHostRxCallback = pConfig->pfRxCallback;
    evbUartInfo.pfHostTxCallback = pConfig->pfTxCallback;

    pUartInfo->pUsart = BOARD_CFG_HOST_UART;
    pUartInfo->baudrate = APP_CFG_HOST_UART_SPEED;
    pUartInfo->isTxComplete = 1;
    pUartInfo->isRxComplete = 1;
    status = StmInitUart(pUartInfo);
    HAL_UART_RegisterCallback(&hUART, HAL_UART_TX_COMPLETE_CB_ID, AdeUartTxCallBack);
    HAL_UART_RegisterCallback(&hUART, HAL_UART_RX_COMPLETE_CB_ID, AdeUartRxCallBack);
    return status;
}

void AdeUartRxCallBack(UART_HandleTypeDef *huart)
{
    STM_UART_INSTANCE *pUartInfo = &evbUartInfo.hostUartInfo;
    pUartInfo->isRxComplete = 1;
    if (huart != NULL)
    {
        if (evbUartInfo.pfHostRxCallback != NULL)
        {
            // Calculate the start of RX buffer: current pointer - received size
            uint8_t *pRxBufferStart = huart->pRxBuffPtr - huart->RxXferSize;
            evbUartInfo.pfHostRxCallback(pRxBufferStart, huart->RxXferSize);
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
