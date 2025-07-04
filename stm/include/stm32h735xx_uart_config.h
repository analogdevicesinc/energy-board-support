/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file  stm32h735xx_uart_config.h
 * @brief UART configuration for H7 MCU Board
 * @addtogroup    UART drivers
 * @{
 */

#ifndef __STM32H735xx_UART_CONFIG_H__
#define __STM32H735xx_UART_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/*============= I N C L U D E S =============*/
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include <stdint.h>

/** STM Uart instance*/
typedef struct
{
    /** pointer to uart registers struct */
    USART_TypeDef *pUsart;
    /** baudrate */
    uint32_t baudrate;
    /** rx complete flag */
    volatile uint8_t isRxComplete;
    /** tx complete flag */
    volatile uint8_t isTxComplete;
    /** tx error flag */
    volatile uint8_t txError;
    /** rx error flag */
    volatile uint8_t rxError;
} STM_UART_INSTANCE;

/**
 * @brief       Initialise uart
 * @param[in]   pUartInfo -  Pointer to uart instance struct
 * @return      status
 */
int32_t StmInitUart(STM_UART_INSTANCE *pUartInfo);

/**
 * @brief       Start uart tx
 * @param[in]   pUartInfo - Pointer to uart instance struct
 * @param[in] pData - pointer to data
 * @param[in] numBytes - no of bytes
 * @return   status
 */
int32_t StmStartUartTx(STM_UART_INSTANCE *pUartInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief       Start uart rx
 * @param[in]   pUartInfo - Pointer to uart instance struct
 * @param[in] pData - pointer to data
 * @param[in] numBytes - no of bytes
 * @return   status
 */
int32_t StmStartUartRx(STM_UART_INSTANCE *pUartInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief       Set uart baudrate
 * @param[in]   pUartInfo - Pointer to uart instance struct
 * @param[in] baudRate - baudrate
 * @return   status
 */
int32_t StmSetUartBaudrate(STM_UART_INSTANCE *pUartInfo, uint32_t baudRate);

#ifdef __cplusplus
}
#endif

#endif /* __STM32H735xx_UART_CONFIG_H__ */

/**
 * @}
 */
