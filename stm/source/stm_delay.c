/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     stm_delay.c
 * @brief
 */

/*=============  I N C L U D E S   =============*/

#include "adi_evb.h"
#ifdef STM32H735xx
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#else
#include "stm32h5xx.h"
#include "stm32h5xx_hal.h"
#endif

/*=============  D E F I N I T I O N S  =============*/

/*=============  D A T A  =============*/

/*=============  C O D E  =============*/

int32_t EvbDelayMs(uint32_t delayMs)
{
    HAL_Delay((uint32_t)delayMs);
    return ADI_EVB_STATUS_SUCCESS;
}

/**
 */
