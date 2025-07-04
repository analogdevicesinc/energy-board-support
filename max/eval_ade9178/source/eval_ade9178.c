/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     eval_ade9178.c
 * @brief
 * @{
 */

/*============= I N C L U D E S =============*/

#include "adi_evb.h"
#include "app_cfg.h"
#include "board_cfg.h"
#include "dma.h"
#include "icc.h"
#include "max3267x_timer_config.h"
#include "nvic_table.h"
#include <stdint.h>
#include <stdio.h>

/*=============  D A T A  =============*/
/** Mem init */
static int32_t MemInit(void);
/** evb info */
static ADI_EVB_INFO evbInfo;

/*=============  C O D E  =============*/
int32_t EvbInit(void **phEvb, ADI_EVB_CONFIG *pConfig)
{
    int32_t status = 0;
    ADI_EVB_INFO *pEvb = &evbInfo;

    if (pConfig == NULL)
    {
        pConfig = &pEvb->config;
    }

    MemInit();
    MXC_DMA_Init();

#if APP_CFG_ENABLE_ADE9178_SPI == 1

    status = EvbInitSpi(&pEvb->hSpi, &pConfig->spiConfig);
#endif

#if APP_CFG_ENABLE_GPIO == 1
    if (status == 0)
    {
        status = EvbInitGpio(&pEvb->hGpio, &pConfig->gpioConfig);
    }
#endif
    if (status == 0)
    {
        status = EvbInitUart(&pEvb->hUart, &pConfig->uartConfig);
    }
#if APP_CFG_ENABLE_HW_CRC == 1
    if (status == 0)
    {
        status = EvbInitCrc(&pEvb->hCrc, &pConfig->crcConfig);
    }
#endif

#if APP_CFG_ENABLE_NVM == 1
    if (status == 0)
    {
        status = EvbInitNvm(&pEvb->hNvm);
    }
#endif

#if APP_CFG_ENABLE_SYSTEM_TIMER == 1
    if (status == 0)
    {
        status = EvbTimerInit();
    }
#endif

    if (status == 0)
    {
        *phEvb = &evbInfo;
    }

    return status;
}

int32_t MemInit(void)
{
    int32_t status = 0;
    NVIC_SetRAM();
    MXC_ICC_Enable();
    // give high GPIO Drive Strength
    // P0.0 used for Single-Wire Debug I/O
    // P0.1,P0.2,P0.3,P0.4,P0.5 used for SPI0 (Drive Strength : 4mA)
    // if its high Strength then unble to communicate with the debug port
    MXC_GPIO0->ds0 = 0xFFFFFFC0;
    MXC_GPIO0->ds1 = 0xFFFFFFFE;
    return status;
}

#if APP_CFG_ENABLE_SYSTEM_TIMER == 1
uint32_t EvbTimerInit()
{
    int32_t status;
    status = MaxTimerInit(BOARD_CFG_SYSTEM_TIMER);
    return status;
}

uint32_t EvbGetTime()
{
    return MaxGetTime(BOARD_CFG_SYSTEM_TIMER);
}

uint32_t EvbGetMaxTime()
{
    return MaxGetMaximumTime();
}

void EvbStartTimer()
{
    MaxStartTimer(BOARD_CFG_SYSTEM_TIMER);
}

void EvbStopTimer()
{
    MaxStopTimer(BOARD_CFG_SYSTEM_TIMER);
}
#endif

/**
 * @}
 */
