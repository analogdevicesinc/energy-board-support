/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     stm_timer.c
 * @brief    Timer config for STM platform.
 */

/*============= I N C L U D E S =============*/
#include "adi_evb.h"
#include "app_cfg.h"
#include "board_cfg.h"
#ifdef STM32H735xx
#include "stm32h735xx_timer_config.h"
#else
#include "stm32h563xx_timer_config.h"
#endif

/*========================== C O D E ==========================*/
uint32_t EvbCf1TimerInit(void)
{
    uint32_t status = StmCf1TimerInit();
    return status;
}

uint32_t EvbReadCf1Timer(void)
{
    return StmReadCf1Timer();
}

void EvbUpdateCf1TimerEndTime(uint32_t endCount)
{
    StmUpdateCf1TimerEndTime(endCount);
}

void EvbStartCf1Timer(uint32_t startCount, uint32_t endCount)
{
    StmStartCf1Timer(startCount, endCount);
}

void EvbStopCf1Timer(void)
{
    StmStopCf1Timer();
}

uint32_t EvbTimerInit(void)
{
    uint32_t status;
    status = StmTimerInit(BOARD_CFG_SYSTEM_TIMER);
    return status;
}

uint32_t EvbLowPowerTimerInit(void)
{
    uint32_t status;
    status = StmLpTimerInit(BOARD_CFG_LP_TIMER);
    return status;
}

uint32_t EvbGetTime(void)
{
    return StmGetTime(BOARD_CFG_SYSTEM_TIMER);
}
void EvbStartTimer(void)
{
    StmStartTimer(BOARD_CFG_SYSTEM_TIMER);
}

void EvbStartLpTimer(void)
{
    StmStartLPTimer(BOARD_CFG_LP_TIMER);
}

void EvbStopLpTimer(void)
{
    StmStopLPTimer(BOARD_CFG_LP_TIMER);
}

void EvbStopTimer(void)
{
    StmStopTimer(BOARD_CFG_SYSTEM_TIMER);
}

void EvbSetLptimOutput(void)
{
    StmSetLptimOutput();
}

/**
 */
