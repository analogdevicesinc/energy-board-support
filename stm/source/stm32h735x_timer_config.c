/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     stm32h735x_timer_config.c
 * @brief    Timer configuration
 * @{
 */
/*============= I N C L U D E S =============*/

#include "adi_evb.h"
#include "app_cfg.h"
#include "stm32h735xx_timer_config.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_lptim.h"
#include "stm32h7xx_hal_tim.h"
#include <stdint.h>

/*=============  D E F I N I T I O N S  =============*/
/** Handler for Timer */
static TIM_HandleTypeDef hTim;
/** Handler for Timer */
static LPTIM_HandleTypeDef hLpTim2;
/** Handler for PWM Timer */
static TIM_HandleTypeDef hPwmTim;
/** GPIO Initialization for CF Timer */
static void StmCf1GpioInit(TIM_HandleTypeDef *pCf1Tim);

/*=============  C O D E  =============*/
uint32_t StmTimerInit(TIM_TypeDef *pTmr)
{
    HAL_StatusTypeDef halStatus;
    hTim.Instance = pTmr;
    // 275 MHz/(10+1) = 25 MHz, where 275 MHz is the
    // i/p clk signal frequency to APB1 timers.
    hTim.Init.Prescaler = 10;
    hTim.Init.CounterMode = TIM_COUNTERMODE_UP;
    hTim.Init.Period = 0xFFFFFFFE;
    hTim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    hTim.Init.RepetitionCounter = 0;
    hTim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    halStatus = HAL_TIM_Base_Init(&hTim);
    return (uint32_t)halStatus;
}

uint32_t StmGetTime(TIM_TypeDef *pTmr)
{
    uint32_t timerCnt = 0;
    if (pTmr != NULL)
    {
        timerCnt = __HAL_TIM_GET_COUNTER(&hTim);
    }
    return timerCnt;
}

void StmStartTimer(TIM_TypeDef *pTmr)
{
    if (pTmr != NULL)
    {
        HAL_TIM_Base_Start(&hTim);
    }
}

void StmStopTimer(TIM_TypeDef *pTmr)
{
    if (pTmr != NULL)
    {
        HAL_TIM_Base_Stop(&hTim);
    }
}

uint32_t StmCf1TimerInit(void)
{
    HAL_StatusTypeDef halStatus;
    TIM_OC_InitTypeDef config;
    config.OCMode = TIM_OCMODE_PWM1;
    // CCRx value -> pwm
    config.Pulse = 0;
    config.OCPolarity = TIM_OCPOLARITY_HIGH;
    config.OCFastMode = TIM_OCFAST_DISABLE;

    hPwmTim.Instance = BOARD_CFG_SYSTEM_CF1_TIMER;
    // 275 MHz/(10+1) = 25 MHz, where 275 MHz is the
    // i/p clk signal frequency to APB1 timers.
    hPwmTim.Init.Prescaler = 10;
    hPwmTim.Init.CounterMode = TIM_COUNTERMODE_UP;
    // ARR Value -> cmp
    hPwmTim.Init.Period = 0xFFFFFFFE;
    hPwmTim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    hPwmTim.Init.RepetitionCounter = 0;
    hPwmTim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    halStatus = HAL_TIM_PWM_Stop(&hPwmTim, BOARD_CFG_CF1_TIM_CHAN);
    halStatus = HAL_TIM_PWM_Init(&hPwmTim);
    StmCf1GpioInit(&hPwmTim);
    halStatus = HAL_TIM_PWM_ConfigChannel(&hPwmTim, &config, BOARD_CFG_CF1_TIM_CHAN);

    __HAL_TIM_DISABLE_OCxPRELOAD(&hPwmTim, BOARD_CFG_CF1_TIM_CHAN);
    HAL_NVIC_EnableIRQ(CF1_TIM_IRQn);
    HAL_NVIC_SetPriority(CF1_TIM_IRQn, APP_CFG_CF_TIMER_INTR_PRIORITY, 0);
    __HAL_TIM_ENABLE_IT(&hPwmTim, TIM_IT_UPDATE);

    return (uint32_t)halStatus;
}

uint32_t StmReadCf1Timer(void)
{
    uint32_t timerCnt = __HAL_TIM_GET_COUNTER(&hPwmTim);
    return timerCnt;
}

void StmUpdateCf1TimerEndTime(uint32_t endCount)
{
    __HAL_TIM_SET_AUTORELOAD(&hPwmTim, endCount);
}

void StmStartCf1Timer(uint32_t startCount, uint32_t endCount)
{
    __HAL_TIM_SET_COUNTER(&hPwmTim, 0);
    __HAL_TIM_SET_COMPARE(&hPwmTim, BOARD_CFG_CF1_TIM_CHAN, startCount);
    __HAL_TIM_SET_AUTORELOAD(&hPwmTim, endCount);

    HAL_TIM_PWM_Start(&hPwmTim, BOARD_CFG_CF1_TIM_CHAN);
}

void StmStopCf1Timer(void)
{
    HAL_TIM_PWM_Stop(&hPwmTim, BOARD_CFG_CF1_TIM_CHAN);
}

uint32_t StmCf1Handler(void)
{
    uint32_t intrFlag = 0;
    if (__HAL_TIM_GET_FLAG(&hPwmTim, TIM_FLAG_UPDATE))
    {
        __HAL_TIM_CLEAR_FLAG(&hPwmTim, TIM_FLAG_UPDATE);
        intrFlag = 1;
    }
    return intrFlag;
}

void StmCf1GpioInit(TIM_HandleTypeDef *pCf1Tim)
{
    __HAL_RCC_GPIOH_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /** GPIO Configuration
        PH10  ------>  TIM5_CH1
    */
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    if (pCf1Tim->Instance == BOARD_CFG_SYSTEM_CF1_TIMER)
    {
        GPIO_InitStruct.Pin = BOARD_CFG_CF1_PIN;
        GPIO_InitStruct.Alternate = CF1_PIN_ALT_FUNC;
        HAL_GPIO_Init(BOARD_CFG_CF1_PORT, &GPIO_InitStruct);
    }
}

uint32_t StmLpTimerInit(LPTIM_TypeDef *pLpTmr)
{
    HAL_StatusTypeDef halStatus;
    __HAL_RCC_LPTIM2_CONFIG(RCC_LPTIM2CLKSOURCE_LSI);
    __HAL_RCC_LSI_ENABLE();
    __HAL_RCC_LPTIM2_CLK_ENABLE();

    hLpTim2.Instance = pLpTmr;
    hLpTim2.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
    hLpTim2.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
    hLpTim2.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
    hLpTim2.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
    hLpTim2.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
    hLpTim2.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
    halStatus = HAL_LPTIM_Init(&hLpTim2);
    HAL_NVIC_EnableIRQ(LPTIM2_IRQn);
    HAL_NVIC_SetPriority(LPTIM2_IRQn, 8, 0);
    __HAL_LPTIM_ENABLE_IT(&hLpTim2, LPTIM_IT_ARRM);
    return (uint32_t)halStatus;
}

void StmSetLptimOutput(void)
{
    /* Deinit GPIO as SS pin - Not required since we are using S/W based Chip Select.
     * But we should do this if we are using hardware based Chip Select.
     */
    GPIO_InitTypeDef ssGpioConfig;
    __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA clock

    HAL_GPIO_DeInit(BOARD_CFG_SS_PORT, BOARD_CFG_SS_PIN);

    /* Configure PA.4 as GPIO output pin
     * There are no timer output pins on PA4 in STM32H735IGK6.
     */
    ssGpioConfig.Mode = GPIO_MODE_OUTPUT_PP;
    ssGpioConfig.Pull = GPIO_PULLUP;
    ssGpioConfig.Pin = BOARD_CFG_SS_PIN;
    ssGpioConfig.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BOARD_CFG_SS_PORT, &ssGpioConfig);
    HAL_GPIO_WritePin(BOARD_CFG_SS_PORT, BOARD_CFG_SS_PIN, GPIO_PIN_SET);
}
void StmStartLPTimer(LPTIM_TypeDef *pLpTmr)
{
    if (pLpTmr != NULL)
    {
        /*
         * Set the auto-reload register (ARR) to 32767.
         * With LPTIM2 running from the LSI clock (typically 32.768 kHz),
         * the timer counts from 0 to ARR (32767), which is 32768 counts.
         * Period = (ARR + 1) / LSI frequency = 32768 / 32768 Hz = 1 second.
         * This sets the PWM period to 1 second.
         */
        HAL_LPTIM_Counter_Start_IT(&hLpTim2, 32767);
    }
}

void StmStopLPTimer(LPTIM_TypeDef *pLpTmr)
{
    if (pLpTmr != NULL)
    {
        HAL_LPTIM_Counter_Stop_IT(&hLpTim2);
    }
}

void StmLPTimerCallback()
{
    if (__HAL_LPTIM_GET_FLAG(&hLpTim2, LPTIM_FLAG_ARRM))
    {
        __HAL_LPTIM_CLEAR_FLAG(&hLpTim2, LPTIM_FLAG_ARRM);
        HAL_GPIO_WritePin(BOARD_CFG_SS_PORT, BOARD_CFG_SS_PIN, GPIO_PIN_RESET);
        EvbDelayMs(60);
        HAL_GPIO_WritePin(BOARD_CFG_SS_PORT, BOARD_CFG_SS_PIN, GPIO_PIN_SET);
    }
}

/**
 * @}
 */
