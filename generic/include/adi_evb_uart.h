/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/
/**
 * @file        adi_evb_uart.h
 * @brief       SPI APIs for ADE9178 Eval board
 * @addtogroup SPI drivers
 * @{
 */

#ifndef __ADI_EVB_UART_H__
#define __ADI_EVB_UART_H__

/*============= I N C L U D E S =============*/

#include "board_cfg.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============  D E F I N I T I O N S  =============*/

/*======= P U B L I C   P R O T O T Y P E S ========*/

/**
 * @brief Sets uart baudrate for waveform streaming
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  baudRate  - baud rate
 * @return  success or error
 */
int32_t EvbAdeWfsUartSetBaudrate(void *hEvb, uint32_t baudRate);

/**
 * @brief Starts receiving data through ADE9178 WFS uart
 * @param[in]  pBuffer  -pointer to the array of data to be sent
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  numBytes  - num of bytes to receive
 * @return  success or error
 */
int32_t EvbAdeWfsUartReceiveAsync(void *hEvb, uint8_t *pBuffer, uint32_t numBytes);
/**
 * @brief Starts receiving data through Host UART and waits till the data is received.
 * @param[in]  pBuffer  -pointer to the array of data to be sent
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  numBytes  - num of bytes to stream
 * @param[in]  timeoutCount  - timeout count to wait in a blocking state.
 * @return  success or error
 */
int32_t EvbHostUartReceive(void *hEvb, uint8_t *pBuffer, uint32_t numBytes, uint32_t timeoutCount);

/**
 * @brief Starts sending data through Host UART and waits till the completion
 * @param[in]  pBuffer  -pointer to the array of data to be sent
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  numBytes  - num of bytes to stream
 * @param[in]  timeoutCount  - timeout count to wait in a blocking state.
 * @return  success or error
 */
int32_t EvbHostUartTransmit(void *hEvb, uint8_t *pBuffer, uint32_t numBytes, uint32_t timeoutCount);

/**
 * @brief Returns the Tx completion status
 *
 * @return  0 - Not complete , 1 complete
 */
int32_t EvbGetTxStatus(void);

/**
 * @brief Starts sending data through Host UART
 * @param[in]  pBuffer  -pointer to the array of data to be sent
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  numBytes  - num of bytes to stream
 * @return  success or error
 */
int32_t EvbHostUartTransmitAsync(void *hEvb, uint8_t *pBuffer, uint32_t numBytes);

/**
 * @brief Initiates data transmission through Host UART and waits for completion. This API
 * internally calls EvbHostUartTransmitAsync function. It is primarily added for the MAX32670, as
 * sending characters requires some delay between them.
 * @param[in]  pBuffer  -pointer to the array of data to be sent
 * @param[in]  numBytes  - num of bytes to stream
 * @param[in]  timeoutCount  - timeout count to wait in a blocking state.
 * @return  success or error
 */
int32_t EvbPutBuffer(uint8_t *pBuffer, uint32_t numBytes, uint32_t timeoutCount);

/**
 * @brief Initiates data transmission through Host UART and waits for completion. This API
 * internally calls EvbHostUartTransmitAsync function. It is primarily added for the MAX32670, as
 * sending characters requires some delay between them.
 * @param[in]  pBuffer  -pointer to the array of data to be sent
 * @param[in]  numBytes  - num of bytes to stream
 * @return  success or error
 */
int32_t EvbPutBufferNb(uint8_t *pBuffer, uint32_t numBytes);

/**
 * @brief Starts receiving data through ADE9178 WFS uart
 * @param[in]  pBuffer  -pointer to the array of data to be sent
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  numBytes  - num of bytes to receive
 * @return  success or error
 */
int32_t EvbHostUartReceiveAsync(void *hEvb, uint8_t *pBuffer, uint32_t numBytes);

#ifdef __cplusplus
}
#endif

#endif /* __ADI_EVB_UART_H__ */

/** @} */
