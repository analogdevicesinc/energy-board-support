/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     stm32h563x_uart_config.c
 * @brief    UART communication configuration for STM32H563xx
 * @{
 */

/*=============  I N C L U D E S   =============*/
#include "adi_evb.h"
#include "app_cfg.h"
#include "stm32h563xx_uart_config.h"
#include "stm32h5xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*============= D A T A T Y P E S ===========*/

/*============= P R O T O T Y P E S =============*/

/*=============  D A T A  =============*/
/** creating uart instance*/
extern UART_HandleTypeDef hUART;
/** creating dma uarttx instance*/
extern DMA_HandleTypeDef hDMAUARTTx;

/*=============  C O D E  =============*/

/**
 * @brief Serial communication initialization function.
 * @return 0 -  Success, <0 = Failed to initialize
 */
int32_t StmInitUart(STM_UART_INSTANCE *pUartInfo)
{
    int32_t status = 0;
    HAL_StatusTypeDef halStatus;
    hUART.Instance = pUartInfo->pUsart;
    hUART.Init.OverSampling = UART_OVERSAMPLING_16;
    hUART.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    hUART.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    hUART.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    hUART.Init.BaudRate = pUartInfo->baudrate;
    hUART.Init.WordLength = UART_WORDLENGTH_8B;
    hUART.Init.StopBits = UART_STOPBITS_1;
    hUART.Init.Parity = UART_PARITY_NONE;
    hUART.Init.Mode = UART_MODE_TX_RX;
    hUART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    halStatus = HAL_UART_Init(&hUART);
    if (halStatus == HAL_OK)
    {
        halStatus = HAL_UARTEx_SetTxFifoThreshold(&hUART, UART_TXFIFO_THRESHOLD_1_8);
    }
    if (halStatus == HAL_OK)
    {
        halStatus = HAL_UARTEx_SetRxFifoThreshold(&hUART, UART_RXFIFO_THRESHOLD_1_8);
    }
    if (halStatus == HAL_OK)
    {
        halStatus = HAL_UARTEx_DisableFifoMode(&hUART);
    }
    if (halStatus == HAL_OK)
    {
        halStatus = HAL_ICACHE_Enable();
    }
    if (halStatus == HAL_OK)
    {
        status = 0;
    }
    else
    {
        status = 1;
    }

    return status;
}

int32_t StmStartUartTx(STM_UART_INSTANCE *pUartInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 0;
    HAL_StatusTypeDef halStatus = HAL_OK;
    if (pUartInfo != NULL)
    {
        halStatus = HAL_UART_Transmit_DMA(&hUART, pData, (uint16_t)numBytes);
    }
    else
    {
        status = 1;
    }
    if (status == 0)
    {
        if (halStatus == HAL_OK)
        {
            status = 0;
        }
        else
        {
            status = 1;
        }
    }

    return status;
}
int32_t StmStartUartRx(STM_UART_INSTANCE *pUartInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 0;
    HAL_StatusTypeDef halStatus;
    if (pUartInfo != NULL)
    {
        halStatus = HAL_UART_Receive_IT(&hUART, pData, (uint16_t)numBytes);

        if (halStatus == HAL_OK)
        {
            status = 0;
        }
        else
        {
            status = -1;
        }
    }
    return status;
}

int32_t StmSetUartBaudrate(STM_UART_INSTANCE *pUartInfo, uint32_t baudRate)
{
    int32_t status = 0;
    if (pUartInfo != NULL || baudRate != 0)
    {
    }
    return status;
}

/**
 * @}
 */
