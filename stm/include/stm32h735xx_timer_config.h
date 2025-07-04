/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file  stm32h735xx_timer_config.h
 * @brief Timer sample configuration for STMH735xx
 * @addtogroup    SPI drivers
 * @{
 */

#ifndef __STM32H735xx_TIMER_CONFIG_H__
#define __STM32H735xx_TIMER_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/*============= I N C L U D E S =============*/
#include "stm32h7xx_hal.h"
#include <stdint.h>

/**
 * @brief       Timer init
 * @return      status
 */
uint32_t StmTimerInit(TIM_TypeDef *pTmr);

/**
 * @brief  Get timer value (in ticks)
 * @return  status
 */
uint32_t StmGetTime(TIM_TypeDef *pTmr);

/**
 * @brief  start timer
 */
void StmStartTimer(TIM_TypeDef *pTmr);

/**
 * @brief  stop timer
 */
void StmStopTimer(TIM_TypeDef *pTmr);

/**
 * @brief       PWM Timer init
 * @return      status
 */
uint32_t StmCf1TimerInit(void);
/**
 * @brief  Get PWM timer value
 * @return  status
 */
uint32_t StmReadCf1Timer(void);
/**
 * @brief  Update end time of PWM timer
 */
void StmUpdateCf1TimerEndTime(uint32_t endCount);

/**
 * @brief Start PWM timer
 * @param[in] startCount  - Timer count when the pulse is triggered
 * @param[in] endCount	  - Timer count when the pulse ends
 *
 */
void StmStartCf1Timer(uint32_t startCount, uint32_t endCount);
/**
 * @brief  stop PWM timer
 */
void StmStopCf1Timer(void);

/**
 * @brief IRQ Handler for CF1 timer
 *
 */
uint32_t StmCf1Handler(void);

/**
 * @brief LPTIMER Output pin configuration
 *
 */
void StmSetLptimOutput(void);

/**
 * @brief       Low Power Timer init
 * @return      status
 */
uint32_t StmLpTimerInit(LPTIM_TypeDef *pLpTmr);

/**
 * @brief  start timer
 */
void StmStartLPTimer(LPTIM_TypeDef *pLpTmr);

/**
 * @brief  stop timer
 */
void StmStopLPTimer(LPTIM_TypeDef *pLpTmr);

/**
 * @brief  LPTIMER callback function
 */
void StmLPTimerCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32H735xx_TIMER_CONFIG_H__ */

/**
 * @}
 */
