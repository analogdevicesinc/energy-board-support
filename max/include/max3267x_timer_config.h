/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file  max3267x_timer_config.h
 * @brief User defined Timer APIs for max3267x platform
 * @defgroup    SPI drivers
 * @{
 */

#ifndef __MAX3267X_TIMER_H__
#define __MAX3267X_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

/*============= I N C L U D E S =============*/
#include "tmr.h"
#include <stdint.h>

/**
 * @brief Initialses Timer
 * @param[in]  pTmr  - pointer to timer registers
 * @return  error or success
 */
int32_t MaxTimerInit(mxc_tmr_regs_t *pTmr);

/**
 * @brief Gets Timer valu
 * @param[in]  pTmr  - pointer to timer registers
 * @return  error or success
 */
uint32_t MaxGetTime(mxc_tmr_regs_t *pTmr);

/**
 * @brief Gets Maximum Timer value
 * @return  error or success
 */
uint32_t MaxGetMaximumTime();

/**
 * @brief Starts Timer
 * @param[in]  pTmr  - pointer to timer registers
 */
void MaxStartTimer(mxc_tmr_regs_t *pTmr);

/**
 * @brief Stops Timer
 * @param[in]  pTmr  - pointer to timer registers
 */
void MaxStopTimer(mxc_tmr_regs_t *pTmr);

#ifdef __cplusplus
}
#endif

#endif /* __MAX3267X_TIMER_H__ */

/**
 * @}
 */
