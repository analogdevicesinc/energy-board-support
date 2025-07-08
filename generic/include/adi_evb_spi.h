/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/
/**
 * @file        adi_evb_spi.h
 * @brief       SPI APIs for ADE9178 Eval board
 * @addtogroup SPI drivers
 * @{
 */

#ifndef __ADI_EVB_SPI_H__
#define __ADI_EVB_SPI_H__

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
 * @brief SPI Tx API for ADE9178 Eval board
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  pData  - pointer to the data to be sent
 * @param[in]  numBytes  - num of bytes to send
 *
 * @return  error or success
 */
int32_t EvbAdeSpiTransmitAsync(void *hEvb, uint8_t *pData, uint32_t numBytes);

/**
 * @brief SPI Rx API for ADE9178 Eval board
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  pData  - pointer to the data to be received
 * @param[in]  numBytes  - num of bytes to receive
 * @return  error or success
 */
int32_t EvbAdeSpiReceiveAsync(void *hEvb, uint8_t *pData, uint32_t numBytes);

/**
 * @brief SPI Rx API for ADE9178 Eval board
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  pTxData  - pointer to the data to be received
 * @param[in]  pRxData  - pointer to the data to be received
 * @param[in]  numBytes  - num of bytes to receive
 * @return  error or success
 */

int32_t EvbAdeSpiTransceiveAsync(void *hEvb, uint8_t *pTxData, uint8_t *pRxData, uint32_t numBytes);

/**
 * @brief Start SPI TxRx in blocking mode. This function will block until the transfer is
 * complete.
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  pTxData  - pointer to the data to be received
 * @param[in]  pRxData  - pointer to the data to be received
 * @param[in]  numBytes  - num of bytes to receive
 * @param[in]  timeOutCount  - timeout count to wait in a blocking state.
 * @return  error or success
 */
int32_t EvbAdeSpiTransceive(void *hEvb, uint8_t *pTxData, uint8_t *pRxData, uint32_t numBytes,
                            uint32_t timeOutCount);

/**
 * @brief Sets SPI frequency
 * @param[in]  hEvb - Evb handle obtained from EvbInit
 * @param[in]  spiFrequency  - frequency in Hz
 * @return  success or error
 */
int32_t EvbAdeSpiSetFrequency(void *hEvb, uint32_t spiFrequency);

/**
 * @brief Abort spi
 * @return status
 */
int32_t EvbAbortHostSpi(void);

/* Following shall be implemented by the applications*/
/**
 * @brief Host Spi Callback
 * @param[in] aborted  - aborted flag
 */
void EvbHostSpiIrqCallback(uint8_t aborted);

/**
 * @brief Tx dma Callback
 */
void EvbAdeSpiTransmitIrqCallback(void);

/**
 * @brief Rx dma Callback
 */
void EvbAdeSpiReceiveIrqCallback(void);

/**
 * @brief Sets SPI freq
 * @param[in] hEvb - pointer to board handle
 * @param[in] spiFrequency - one of the possible baudrate prescaler values
 * @return 0 -  success
 */
int32_t EvbSetAdcSpiFrequency(void *hEvb, uint32_t spiFrequency);

/**
 * @brief Clears SPI Interrupt
 * @param[in] hUser - pointer to user handle
 */
void EvbAdcClearSpiInterrupt(void *hUser);

/**
 * @brief Configure the GPIO pin for SPI chip select
 */
void EvbAdeSpiSetChipSelect(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADI_EVB_SPI_H__ */

/** @} */
