/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     max3267x_timer_config.c
 * @brief    Timer configuration
 * @{
 */
/*============= I N C L U D E S =============*/

#include "max3267x_timer_config.h"
#include "app_cfg.h"
#include "mxc_device.h"
#include "nvic_table.h"
#include "tmr.h"
#include "tmr_revb.h"
#include <stdint.h>

/*=============  D E F I N I T I O N S  =============*/
#if APP_CFG_SYSTEM_TIMER_CLOCK_TYPE == 1
/** timer clock */
#define ADE_TIMER_CLOCK MXC_TMR_8M_CLK
/** timer clock source*/
#define ADE_TIMER_CLOCK_SOURCE MXC_TMR_CLK2
/** count for convert timer count to us */
#define TIMER_COUNT_US 7.3728f // 8 for 8Mhz and 50 for 50Mhz
#else
/** timer clock */
#define ADE_TIMER_CLOCK        MXC_TMR_APB_CLK
/** timer clock source*/
#define ADE_TIMER_CLOCK_SOURCE MXC_TMR_CLK0
/** count for convert timer count to us */
#define TIMER_COUNT_US         50.0f // 8 for 8Mhz and 50 for 50Mhz
#endif

/**
 * @brief  Pulse 1 Timer interrupt handler.
 *
 */
static void TimerHandler(void);

/**
 * Read timer counter
 */
static inline unsigned int ReadTimerCountRegister(mxc_tmr_regs_t *pTmr);

/** timer registers */
static mxc_tmr_regs_t *pSysTmr;
/**  Define the timer width (16-bit or 32-bit) */
#define TIMER_WIDTH 32 // Change to 16 for 16-bit timers

/*=============  C O D E  =============*/
int32_t MaxTimerInit(mxc_tmr_regs_t *pTmr)
{
    IRQn_Type irqn;
    int32_t status;
    mxc_tmr_cfg_t config;
    MXC_TMR_Shutdown(pTmr);

    config.pres = TMR_PRES_1;
    config.mode = TMR_MODE_CONTINUOUS;
    config.clock = ADE_TIMER_CLOCK;
    config.cmp_cnt = 0xffffffff;
    config.pol = 0;
    config.bitMode = TMR_BIT_MODE_32;

    irqn = MXC_TMR_GET_IRQ(MXC_TMR_GET_IDX(pTmr));
    pSysTmr = pTmr;

    NVIC_SetVector(irqn, (uint32_t)TimerHandler);
    NVIC_EnableIRQ(irqn);
    status = MXC_TMR_Init(pTmr, &config, false);
    pTmr->ctrl1 |= (ADE_TIMER_CLOCK_SOURCE << MXC_F_TMR_REVB_CTRL1_CLKSEL_B_POS);
    MXC_TMR_EnableInt(pTmr);

    return status;
}

uint32_t MaxGetTime(mxc_tmr_regs_t *pTmr)
{
    uint32_t timerCnt = ReadTimerCountRegister(pTmr);
    return (uint32_t)(((float)timerCnt / TIMER_COUNT_US));
}

uint32_t MaxGetMaximumTime()
{
    uint32_t timerCnt = (uint32_t)((1ULL << TIMER_WIDTH) - 1);
    return (uint32_t)(((float)timerCnt / TIMER_COUNT_US));
}

void MaxStartTimer(mxc_tmr_regs_t *pTmr)
{
    MXC_TMR_ClearFlags(pTmr);
    pTmr->cnt = 0x01;
    MXC_TMR_Start(pTmr);
}

void MaxStopTimer(mxc_tmr_regs_t *pTmr)
{
    MXC_TMR_Stop(pTmr);
}

static void TimerHandler(void)
{
    MXC_TMR_ClearFlags(pSysTmr);
}

static inline unsigned int ReadTimerCountRegister(mxc_tmr_regs_t *pTmr)
{
#if APP_CFG_SYSTEM_TIMER_CLOCK_TYPE == 1
    uint32_t cnt;
    uint32_t cnt0;
    uint32_t cnt1;
    uint32_t cnt2;
    uint32_t diff20;
    uint32_t diff21;
    uint32_t isCnt2Good;
    uint32_t isCnt1Good;
    cnt0 = pTmr->cnt;
    cnt1 = pTmr->cnt;
    cnt2 = pTmr->cnt;
    diff21 = cnt2 - cnt1;
    diff20 = cnt2 - cnt0;
    /* We make an assumption that only one out of 3 consecutive read will be wrong
        So either cnt2 or cnt1 is correct. If both are correct we will use cnt2
     */
    /* Unsigned numbers  - only diff of 0, 1 meets the criteria*/
    isCnt2Good = (diff20 <= 1) || (diff21 <= 1);
    isCnt1Good = !isCnt2Good;
    cnt = isCnt2Good * cnt2 + isCnt1Good * cnt1;
    return cnt;
#else
    return pTmr->cnt;
#endif
}

/**
 * @}
 */
