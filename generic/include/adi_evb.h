/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/
/**
 * @file        adi_evb.h
 * @brief       SPI APIs for ADE9178 Eval board
 * @addtogroup SPI drivers
 * @{
 */

#ifndef __ADI_EVB_H__
#define __ADI_EVB_H__

/*============= I N C L U D E S =============*/

#ifdef USE_CRC
#include "adi_evb_crc.h"
#endif

#include "adi_evb_gpio.h"
#include "adi_evb_i2c.h"
#include "adi_evb_spi.h"
#include "adi_evb_timer.h"
#include "adi_evb_uart.h"
#include "board_cfg.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============  D E F I N I T I O N S  =============*/

/** Macro to denote NULL handle */
#define ADI_EVB_STATUS_NULL_HANDLE -1
/** Macro to denote success  */
#define ADI_EVB_STATUS_SUCCESS 0
/** Macro to denote  spi init error*/
#define ADI_EVB_STATUS_SPI_INIT_ERROR 1
/** Macro to denote uart init error in waveform streaming  */
#define ADI_EVB_STATUS_WFRM_UART_INIT_ERROR 2
/** Macro to denote uart init error  */
#define ADI_EVB_STATUS_HOST_UART_INIT_ERROR 3
/** Macro to denote gpio init error  */
#define ADI_EVB_STATUS_GPIO_INIT_ERROR 4
/** Error in acquiring DMA channel */
#define ADI_EVB_STATUS_DMA_CHANNEL_ACQUIRE_ERROR 5
/** Invlaid LED index */
#define ADI_EVB_STATUS_INVALID_INDEX 7
/** General  falure */
#define ADI_EVB_STATUS_INIT_FAILURE 8

/** Typedef for callbacks from EVB */
typedef void (*ADI_EVB_CALLBACK)(void);
/** Typedef for callbacks from EVB */
typedef void (*ADI_EVB_GPIO_CALLBACK)(uint32_t port, uint32_t flag);
/** Typedef for EVB status */
typedef int32_t ADI_EVB_STATUS;

/**
 * @brief Non-build time configuration for UART
 *  Use app_cfg.h for build time configuration
 */
typedef struct
{
    /** Wfs rx callback */
    ADI_EVB_CALLBACK pfWfsUartRxCallback;
    /** Host Wfs rx callback */
    ADI_EVB_CALLBACK pfHostUartRxCallback;
    /** Host Wfs tx callback */
    ADI_EVB_CALLBACK pfHostUartTxCallback;
} ADI_EVB_UART_CONFIG;

/**
 * @brief Non-build time configuration for SPI
 *  Use app_cfg.h for build time configuration
 */
typedef struct
{
    /** Spi rx callback */
    ADI_EVB_CALLBACK pfAdeSpiRxCallback;
    /** Spi tx callback */
    ADI_EVB_CALLBACK pfAdeSpiTxCallback;
    /** Spi rx callback */
    ADI_EVB_CALLBACK pfHostSpiRxCallback;
    /** Spi tx callback */
    ADI_EVB_CALLBACK pfHostSpiTxCallback;
    /** Spi rx callback */
    ADI_EVB_CALLBACK pfFramSpiRxCallback;
    /** Spi tx callback */
    ADI_EVB_CALLBACK pfFramSpiTxCallback;
} ADI_EVB_SPI_CONFIG;

/**
 * @brief Non-build time configuration for GPIOs
 *  Use app_cfg.h for build time configuration
 */
typedef struct
{
    /** uart config */
    ADI_EVB_GPIO_CALLBACK pfGpioCallback;
} ADI_EVB_GPIO_CONFIG;

/**
 * @brief Top level configuration structure
 *
 */
typedef struct
{
    /** uart config */
    ADI_EVB_UART_CONFIG uartConfig;
    /** spi config */
    ADI_EVB_SPI_CONFIG spiConfig;
    /** gpio config */
    ADI_EVB_GPIO_CONFIG gpioConfig;

} ADI_EVB_CONFIG;

/**
 * @brief EVB private data holder
 * Do not access this structure from application
 */
typedef struct
{
    /** user spi handle */
    void *hSpi;
    /** user uart handle */
    void *hUart;
    /** user gpio handle */
    void *hGpio;
    /** board config */
    ADI_EVB_CONFIG config;

} ADI_EVB_INFO;

/*======= P U B L I C   P R O T O T Y P E S ========*/

/**
 * @brief       Delays execution by the specified number of milliseconds.
 * @param[in]   delayMs     Delay duration in ms.
 * @return      ADI_EVB_STATUS_SUCCESS on success. Refer to #ADI_EVB_STATUS for other return codes.
 */
ADI_EVB_STATUS EvbDelayMs(uint32_t delayMs);

/**
 * @brief Initialzes peripherals. Refer to board_cfg.h for
 * static board configurations. USe app_cfg.h for application
 * specific configurations
 * @param phEvb  - Pointer to store EVB handle
 * @param pConfig - Pointer configuration structure. This can be NULL if
 * there is nothing configure
 * @return int32_t
 */
int32_t EvbInit(void **phEvb, ADI_EVB_CONFIG *pConfig);

/**
 * @brief Initialises UART
 * @param[out] phUart - Pointer to store handle
 * @param[in] pConfig - Pointer configuration structure. This can be NULL if
 * there is nothing configure

 * @return  error or success
 */
int32_t EvbInitUart(void **phUart, ADI_EVB_UART_CONFIG *pConfig);

/**
 * @brief Initialises Gpios of a board
 * @param[out] phGpio - Pointer to store handle
 * @param pConfig - Pointer configuration structure. This can be NULL if
 * there is nothing configure
 * @return  error or success
 */
int32_t EvbInitGpio(void **phGpio, ADI_EVB_GPIO_CONFIG *pConfig);

/**
 * @brief Initialises SPI of a board
 * @param[out] phSpi - Pointer to store handle
 * @param pConfig - Pointer configuration structure. This can be NULL if
 * there is nothing configure
 * @return  error or success
 */
int32_t EvbInitAdeSpi(void **phSpi, ADI_EVB_SPI_CONFIG *pConfig);

/**
 * @brief Initialises SPI of a board
 * @param[out] phSpi - Pointer to store handle
 * @param pConfig - Pointer configuration structure. This can be NULL if
 * there is nothing configure
 * @return  error or success
 */
int32_t EvbInitSpi(void **phSpi, ADI_EVB_SPI_CONFIG *pConfig);

/**
 * @brief Initialize timer
 * @return 0 -  success
 */
uint32_t EvbTimerInit(void);

/**
 * @brief Initialize timer
 * @return 0 -  success
 */
uint32_t EvbLowPowerTimerInit(void);
/**
 * @brief Function to reset EVB
 * @return 0 -  success
 */
int32_t EvbInitReset(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADI_EVB_H__ */

/** @} */
