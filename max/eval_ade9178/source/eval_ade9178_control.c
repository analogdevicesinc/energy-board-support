/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     eval_ade9178_control.c
 * @brief
 * @{
 */

/*=============  I N C L U D E S   =============*/

#include "adi_evb.h"
#include "gpio.h"
#include "mxc_delay.h"
#include <stdint.h>

/*=============  D E F I N I T I O N S  =============*/

/** Duration in ms of the low pulse. */
#define RESET_PULSE_MSEC 10
/**Duration in ms to wait after the low pulse. */
#define RESET_WAIT_MSEC 200

/*=============  D A T A  =============*/

// The Keil pack uses the DFP pack provided by Keil. The MAX32670 pack is not updated with the
// latest MSDK changes.
#ifdef DISABLE_MSDK
/** Array of pins used to reset other devices on the evaluation board. */
static const mxc_gpio_cfg_t resetPins[] = {
    {BOARD_CFG_ADEIRQ_PORT, BOARD_CFG_ADC_RESET_PIN, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_PULL_UP,
     MXC_GPIO_VSSEL_VDDIO},
    {BOARD_CFG_ADEIRQ_PORT, BOARD_CFG_ADE_RESET_PIN, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_PULL_UP,
     MXC_GPIO_VSSEL_VDDIO}};
/** Array of pins used to control LEDs on the evaluation board. */
static const mxc_gpio_cfg_t ledPins[] = {
    {BOARD_CFG_ADEIRQ_PORT, BOARD_CFG_LED1_PIN, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_PULL_UP,
     MXC_GPIO_VSSEL_VDDIO},
    {BOARD_CFG_ADEIRQ_PORT, BOARD_CFG_LED2_PIN, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_PULL_UP,
     MXC_GPIO_VSSEL_VDDIO}};
#else
/** Array of pins used to reset other devices on the evaluation board. */
static const mxc_gpio_cfg_t resetPins[] = {
    {BOARD_CFG_ADEIRQ_PORT, BOARD_CFG_ADC_RESET_PIN, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_PULL_UP,
     MXC_GPIO_VSSEL_VDDIO, MXC_GPIO_DRVSTR_3},
    {BOARD_CFG_ADEIRQ_PORT, BOARD_CFG_ADE_RESET_PIN, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_PULL_UP,
     MXC_GPIO_VSSEL_VDDIO, MXC_GPIO_DRVSTR_3}};
/** Array of pins used to control LEDs on the evaluation board. */
static const mxc_gpio_cfg_t ledPins[] = {
    {BOARD_CFG_ADEIRQ_PORT, BOARD_CFG_LED1_PIN, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_PULL_UP,
     MXC_GPIO_VSSEL_VDDIO, MXC_GPIO_DRVSTR_3},
    {BOARD_CFG_ADEIRQ_PORT, BOARD_CFG_LED2_PIN, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_PULL_UP,
     MXC_GPIO_VSSEL_VDDIO, MXC_GPIO_DRVSTR_3}};
#endif

/** Number of reset pins on the evaluation board. */
static const uint32_t numReset = (sizeof(resetPins) / sizeof(mxc_gpio_cfg_t));
/** Number of LEDs on the evaluation board. */
static const uint32_t numLed = (sizeof(ledPins) / sizeof(mxc_gpio_cfg_t));

/*=============  C O D E  =============*/

/**
 * @brief       Toggles an active-low reset pin.
 * @param[in]   idx         Index of the reset pin to toggle.
 * @param[in]   pulseMs     Duration in ms of the low pulse.
 * @param[in]   waitMs      Duration in ms to wait after the low pulse.
 * @return      ADI_EVB_STATUS_SUCCESS on success.
 */
int32_t ToggleResetB(uint32_t idx, uint32_t pulseMs, uint32_t waitMs);

int32_t EvbDelayMs(uint32_t delayMs)
{
    MXC_Delay(MXC_DELAY_MSEC(delayMs));
    return ADI_EVB_STATUS_SUCCESS;
}

int32_t EvbResetAde(void)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
    status = ToggleResetB(1, RESET_PULSE_MSEC, RESET_WAIT_MSEC);
    EvbDelayMs(RESET_WAIT_MSEC);
    return status;
}

int32_t EvbResetAdcs(void)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
    status = ToggleResetB(0, RESET_PULSE_MSEC, RESET_WAIT_MSEC);
    return status;
}

int32_t EvbResetAll(void)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
#if BOARD_CFG_RESET_TYPE == 0
    // Both for Ade9178 and Adc reset pin is tied together.
    status = EvbResetAde();
#else
    status = EvbResetAdcs();
    status = EvbResetAde();
#endif
    return status;
}

int32_t EvbLedOn(uint32_t idx)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
    /* Turn on the LED if a valid index is provided. */
    if (idx < numLed)
    {
        MXC_GPIO_OutClr(ledPins[idx].port, ledPins[idx].mask);
    }
    else
    {
        status = ADI_EVB_STATUS_INVALID_INDEX;
    }
    return status;
}

int32_t EvbLedOff(uint32_t idx)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
    /* Turn off the LED if a valid index is provided. */
    if (idx < numLed)
    {
        MXC_GPIO_OutSet(ledPins[idx].port, ledPins[idx].mask);
    }
    else
    {
        status = ADI_EVB_STATUS_INVALID_INDEX;
    }
    return status;
}

int32_t EvbLedFlash(uint32_t idx, uint32_t numFlashes, uint32_t flashMs)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
    uint32_t i;
    for (i = 0; i < numFlashes; i++)
    {
        MXC_GPIO_OutClr(ledPins[idx].port, ledPins[idx].mask);
        EvbDelayMs(flashMs);
        MXC_GPIO_OutSet(ledPins[idx].port, ledPins[idx].mask);
        EvbDelayMs(flashMs);
    }
    return status;
}

int32_t ToggleResetB(uint32_t idx, uint32_t pulseMs, uint32_t waitMs)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
    if (idx < numReset)
    {
        MXC_GPIO_OutClr(resetPins[idx].port, resetPins[idx].mask);
        EvbDelayMs(pulseMs);
        MXC_GPIO_OutSet(resetPins[idx].port, resetPins[idx].mask);
        EvbDelayMs(waitMs);
    }
    else
    {
        status = ADI_EVB_STATUS_INVALID_INDEX;
    }
    return status;
}

/**
 * @}
 */
