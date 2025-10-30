/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/
/**
 * @file        adi_evb_host_comm.h
 * @brief       Host communication APIs (UART/USB).
 *              This wrapper selects UART or USB implementation based on application configuration.
 * @addtogroup  Host_Comm_Drivers
 * @{
 */

#ifndef __ADI_EVB_HOST_COMM_H__
#define __ADI_EVB_HOST_COMM_H__

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
 * @brief Returns the Tx completion status of Host UART/ USB
 *
 * @return  0 - Not complete , 1 complete
 */
int32_t EvbHostCommGetTxStatus(void);

/**
 * @brief Starts sending data through Host UART/ USB
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  pBuffer  -pointer to the array of data to be sent
 * @param[in]  numBytes  - num of bytes to stream
 * @return  success or error
 */
int32_t EvbHostCommTransmitAsync(void *hEvb, uint8_t *pBuffer, uint32_t numBytes);

/**
 * @brief Starts receiving data through Host UART/ USB
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  pBuffer  -pointer to the array of data to be received
 * @param[in]  numBytes  - num of bytes to receive
 * @return  success or error
 */
int32_t EvbHostCommReceiveAsync(void *hEvb, uint8_t *pBuffer, uint32_t numBytes);

/**
 * @brief Sends data through Host UART/USB in blocking mode. This function will block until the
 * transfer is complete or timeout occurs.
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  pBuffer  -pointer to the array of data to be sent
 * @param[in]  numBytes  - num of bytes to send
 * @param[in]  timeoutCount  - timeout count to wait in a blocking state.
 * @return  success or error
 */
int32_t EvbHostCommTransmit(void *hEvb, uint8_t *pBuffer, uint32_t numBytes, uint32_t timeoutCount);

/**
 * @brief Receives data through Host UART/USB in blocking mode. This function will block until the
 * transfer is complete or timeout occurs.
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  pBuffer  -pointer to the array of data to be received
 * @param[in]  numBytes  - num of bytes to receive
 * @param[in]  timeoutCount  - timeout count to wait in a blocking state.
 * @return  success or error
 */
int32_t EvbHostCommReceive(void *hEvb, uint8_t *pBuffer, uint32_t numBytes, uint32_t timeoutCount);

/**
 * @brief Runs host communication tasks.
 */
void EvbHostCommTasksRun(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADI_EVB_HOST_COMM_H__ */

/** @} */
