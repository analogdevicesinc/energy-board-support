/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/
/**
 * @file        adi_evb_timer.h
 * @brief       SPI APIs for ADE9178 Eval board
 * @addtogroup SPI drivers
 * @{
 */

#ifndef __ADI_EVB_TIMER_H__
#define __ADI_EVB_TIMER_H__

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
 * @brief get timer value
 * @return 0 -  success
 */
uint32_t EvbGetTime(void);

/**
 * @brief get maximum timer value
 * @return 0 -  success
 */
uint32_t EvbGetMaxTime(void);

/**
 * @brief  start timer
 */
void EvbStartTimer(void);

/**
 * @brief  start timer
 */
void EvbStartLpTimer(void);

/**
 * @brief  start timer
 */
void EvbStopLpTimer(void);

/**
 * @brief  stop timer
 */
void EvbStopTimer(void);

/**
 * @brief Initializes PWM timer
 * @return 0 -  success
 *
 */
uint32_t EvbCf1TimerInit(void);
/**
 * @brief Read PWM timer count value
 * @returns  PWM timer counter
 *
 */
uint32_t EvbReadCf1Timer(void);
/**
 * @brief Update end time for PWM pulse
 * @param[in] endCount	  - Timer count when the pulse ends
 */
void EvbUpdateCf1TimerEndTime(uint32_t endCount);

/**
 * @brief Start PWM timer
 * @param[in] startCount  - Timer count when the pulse is triggered
 * @param[in] endCount	  - Timer count when the pulse ends
 *
 */
void EvbStartCf1Timer(uint32_t startCount, uint32_t endCount);

/**
 * @brief Stop PWM timer
 */
void EvbStopCf1Timer(void);

/**
 * @brief Set the LPTIM output pin to be used for TDM
 */
void EvbSetLptimOutput(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADI_EVB_TIMER_H__ */

/** @} */
