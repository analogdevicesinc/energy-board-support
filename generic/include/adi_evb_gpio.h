/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/
/**
 * @file        adi_evb_gpio.h
 * @brief       SPI APIs for ADE9178 Eval board
 * @addtogroup SPI drivers
 * @{
 */

#ifndef __ADI_EVB_GPIO_H__
#define __ADI_EVB_GPIO_H__

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
 * @brief       Resets the ADE.
 * @return      int32_t_SUCCESS on success.
 */
int32_t EvbResetAde(void);

/**
 * @brief       Resets the ADCs.
 * @return      int32_t_SUCCESS on success.
 */
int32_t EvbResetAdcs(void);

/**
 * @brief       Resets all devices.
 * @details     ADCs are reset before ADE9178.
 * @return      int32_t_SUCCESS on success.
 */
int32_t EvbResetAll(void);

/**
 *  @brief      Flashes an LED.
 *  @param[in]  idx         Index of the LED to flash.
 *  @param[in]  numFlashes  Number of times to flash the LED.
 *  @param[in]  flashMs     Duration in ms for which the LED will be on.
 *  @details    The LED will flash with a 50% duty cycle.
 *  @return     int32_t_SUCCESS on success.
 */
int32_t EvbLedFlash(uint32_t idx, uint32_t numFlashes, uint32_t flashMs);

/**
 *  @brief      Turns on an LED.
 *  @param[in]  idx         Index of the LED to turn on.
 *  @return     int32_t_SUCCESS on success.
 */
int32_t EvbLedOn(uint32_t idx);

/**
 *  @brief      Configures Adc Stand by mode.
 *  @param[in]  mode 1:Exit Stand By  0:Enter Stand By
 *  @return     int32_t_SUCCESS on success.
 */
int32_t EvbSetStandby(uint8_t mode);
/**
 *  @brief      Turns off an LED.
 *  @param[in]  idx         Index of the LED to turn off.
 *  @return     int32_t_SUCCESS on success.
 */
int32_t EvbLedOff(uint32_t idx);

/**
 * Enable GPIO Irq
 * @param port - Port of the GPIO
 * @param pin - Pin for the GPIO
 */
void EvbEnableGPIOIrq(uint32_t port, uint32_t pin);

/**
 * Disable GPIO Irq
 * @param port - Port of the GPIO
 * @param pin - Pin for the GPIO
 */
void EvbDisableGPIOIrq(uint32_t port, uint32_t pin);

/**
 * Enable/Disable DREADY Irq
 * @param enable - 1 to enable and 0 to disable
 */
int32_t EvbEnableDreadyIrq(uint8_t enable);

/**
 * @brief Function to handle GPIO IRQ.
 */
void EvbEnableAllGPIOIrq(void);
/**
 * @brief Function to perform hardware reset of the ADC.
 */
void EvbAdcReset(void);

/**
 * @brief Function to delay for a specified time in milliseconds.
 * @param delayMsec - Time to delay in milliseconds.
 */
void OsalDelay(int32_t delayMsec);

/**
 * Get the pinstate
 * @param port - Port of the GPIO
 * @param flag - mask of the GPIO
 */
int32_t EvbGetPinState(uint32_t port, uint32_t flag);

#ifdef __cplusplus
}
#endif

#endif /* __ADI_EVB_GPIO_H__ */

/** @} */
