/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/*============= I N C L U D E S =============*/
#include "adi_evb.h"
#include "data_rdy_timer.h"
#include "stm32h5xx_hal.h"

/*=============  D E F I N I T I O N S  =============*/
/** evb gpio info*/
typedef struct
{
    /** callback*/
    ADI_EVB_GPIO_CALLBACK pfDrdyGpioCallback;

} EVB_DRDY_GPIO_INFO;

/** evb gpio info*/
static EVB_DRDY_GPIO_INFO drdyGpioInfo;

/** Frequency of generating DReady interrupt */
#define DREADY_FREQ 8000 // (Hz)
/** Specifies the timer to be used for generating DREADY interrupt*/
#define DATA_RDY_TIMER TIM3

/*=============  D A T A  =============*/
/** Stores number of data ready interrupts which have occured since the last
 *  block of CF position were received */
static uint8_t countFromLastBlock = 0;
/** Handler for Timer TIM3 */
static TIM_HandleTypeDef hTimDready;
/*=============  C O D E  =============*/

void EvbDataRdyTimerHandler(void) // Ensure this matches the existing declaration
{
    __HAL_TIM_CLEAR_FLAG(&hTimDready, TIM_FLAG_UPDATE);
    drdyGpioInfo.pfDrdyGpioCallback((uint32_t)BOARD_CFG_ADC_DREADY_PORT, BOARD_CFG_ADC_DREADY_PIN);
}

void EvbDataRdyTimerInit(ADI_EVB_GPIO_CONFIG *pConfig)
{

    // Initializing timer for DREADY interrupt
    uint32_t periodTicks = (uint32_t)(25 * (((float)(1.0f / DREADY_FREQ)) * 1000000));

    hTimDready.Instance = DATA_RDY_TIMER;
    hTimDready.Init.Prescaler = 9;
    hTimDready.Init.CounterMode = TIM_COUNTERMODE_UP;
    hTimDready.Init.Period = periodTicks - 1;
    hTimDready.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    hTimDready.Init.RepetitionCounter = 0;
    hTimDready.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    drdyGpioInfo.pfDrdyGpioCallback = pConfig->pfGpioCallback;
    HAL_NVIC_SetPriority(TIM3_IRQn, APP_CFG_DREADY_INTR_PRIORITY, 0);
    HAL_TIM_Base_Init(&hTimDready);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

void EvbDataRdyStartTimer(void)
{
    HAL_TIM_Base_Start_IT(&hTimDready);
}
