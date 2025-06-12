/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file  max3267x_uart_config.h
 * @brief User defined Uart APIs for max3267x platform
 * @defgroup    SPI drivers
 * @{
 */

#ifndef __MAX3267X_UART_CONFIG__
#define __MAX3267X_UART_CONFIG__

#ifdef __cplusplus
extern "C" {
#endif

/*============= I N C L U D E S =============*/
#include "max3267x_dma_config.h"
#include "uart.h"
#include <stdint.h>

/*=============  D E F I N I T I O N S  =============*/
/** Maxim uart instance info*/
typedef struct
{
    /** Rx dma channel */
    MAX_DMA_DESC rxDmaDesc;
    /** Tx dma channel */
    MAX_DMA_DESC txDmaDesc;
    /** Uart registers */
    mxc_uart_regs_t *pUart;
    /** baud rate */
    uint32_t baudrate;
    /** Rx dma call back */
    mxc_uart_complete_cb_t pfRxCallback;
    /** rx complete flag */
    volatile uint8_t isRxComplete;
    /** Tx complete flag */
    volatile uint8_t isTxComplete;
    /** tx error flag*/
    volatile uint8_t txError;
    /** rx error flag*/
    volatile uint8_t rxError;
} MAX_UART_INSTANCE;

/*======= P U B L I C   P R O T O T Y P E S ========*/
/**
 * @brief Initialses uart
 * @param[in]  pUartInfo  - pointer to the uart info struct
 * @return  error or success
 */
int32_t MaxInitUart(MAX_UART_INSTANCE *pUartInfo);

/**
 * @brief Start Uart Tx
 * @param[in]  pUartInfo  - pointer to the uart info struct
 * @param[in] pData - pointer to data
 * @param[in] numBytes - num of bytes
 * @return  error or success
 */
int32_t MaxStartUartTxDMA(MAX_UART_INSTANCE *pUartInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief Start Uart rx
 * @param[in]  pUartInfo  - pointer to the uart info struct
 * @param[in] pData - pointer to data
 * @param[in] numBytes - num of bytes
 * @return  error or success
 */
int32_t MaxStartUartRx(MAX_UART_INSTANCE *pUartInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief Start Uart rx
 * @param[in]  pUartInfo  - pointer to the uart info struct
 * @param[in] pData - pointer to data
 * @param[in] numBytes - num of bytes
 * @return  error or success
 */
int32_t MaxStartUartRxDMA(MAX_UART_INSTANCE *pUartInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief Set baudrate for uart
 * @param[in]  pUartInfo  - pointer to the uart info struct
 * @param[in] baudRate - baudrate
 * @return  error or success
 */
int32_t MaxSetUartBaudrate(MAX_UART_INSTANCE *pUartInfo, uint32_t baudRate);

#ifdef __cplusplus
}
#endif

#endif /* __MAX3267X_UART_CONFIG__ */

/**
 * @}
 */
