/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     app_mcu_h5_control.c
 * @brief
 */

/*=============  I N C L U D E S   =============*/

#include "adi_evb.h"
#include "stm32h5xx.h"
#include "stm32h5xx_hal.h"
#include <stdint.h>

/*=============  D E F I N I T I O N S  =============*/

/** Duration in ms of the low pulse. */
#define PULSE_MSEC 10
/**Duration in ms to wait after the low pulse. */
#define WAIT_MSEC 200

/*=============  D A T A  =============*/
/**
 * @brief STM gpio config struct
 */
typedef struct
{
    /** config */
    GPIO_InitTypeDef gpioConfig;
    /** port */
    GPIO_TypeDef *gpioPort;
} EVB_GPIO_INFO;

/** Array of pins used to reset other devices on the evaluation board. */
static const EVB_GPIO_INFO resetPins[1] = {
    {{BOARD_CFG_ADC_RESET_PIN, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0},
     BOARD_CFG_ADC_RESET_PORT}};
/** Array of pins used to control LEDs on the evaluation board. */
static const EVB_GPIO_INFO ledPins[3] = {
    {{BOARD_CFG_LED1_PIN, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0},
     BOARD_CFG_LED_PORT},
    {{BOARD_CFG_LED2_PIN, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0},
     BOARD_CFG_LED_PORT},
    {{BOARD_CFG_LED3_PIN, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, 0},
     BOARD_CFG_LED_PORT},
};
/** Number of reset pins on the evaluation board. */
static const uint32_t numReset = (sizeof(resetPins) / sizeof(resetPins[0]));
/** Number of LEDs on the evaluation board. */
static const uint32_t numLed = (sizeof(ledPins) / sizeof(ledPins[0]));

/*=============  C O D E  =============*/

/**
 * @brief  Resets the Evaluation Board.
 * @return 0 on success.
 */
int32_t InitReset(void);

/**
 * @brief       Initializes the LEDs on the evaluation board.
 * @return      0 on success.
 */
int32_t InitLed(void);

/**
 * @brief       Toggles an active-low reset pin.
 * @param[in]   idx         Index of the reset pin to toggle.
 * @param[in]   pulseMs     Duration in ms of the low pulse.
 * @param[in]   waitMs      Duration in ms to wait after the low pulse.
 * @return      0 on success.
 */
int32_t ToggleResetB(uint32_t idx, uint32_t pulseMs, uint32_t waitMs);

int32_t EvbInitReset(void)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
    status = InitReset();
    if (status != ADI_EVB_STATUS_SUCCESS)
    {
        return status;
    }
    status = InitLed();
    if (status == ADI_EVB_STATUS_SUCCESS)
    {
        EvbLedFlash(0, 10, 25);
        EvbLedFlash(1, 10, 25);
    }

    return status;
}

int32_t EvbResetAde(void)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
    status = ToggleResetB(1, PULSE_MSEC, WAIT_MSEC);
    EvbDelayMs(WAIT_MSEC);
    return status;
}

int32_t EvbResetAdcs(void)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
    status = ToggleResetB(0, PULSE_MSEC, WAIT_MSEC);
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

int32_t EvbSetStandby(uint8_t mode)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
    if (mode == 0)
    {
        HAL_GPIO_WritePin(BOARD_CFG_STDBY_PORT, (uint16_t)BOARD_CFG_STDBY_PIN, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(BOARD_CFG_STDBY_PORT, (uint16_t)BOARD_CFG_STDBY_PIN, GPIO_PIN_SET);
    }
    return status;
}

int32_t EvbLedOn(uint32_t idx)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
    /* Turn on the LED if a valid index is provided. */
    if (idx < numLed)
    {
        HAL_GPIO_WritePin(ledPins[idx].gpioPort, (uint16_t)ledPins[idx].gpioConfig.Pin,
                          GPIO_PIN_RESET);
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
        HAL_GPIO_WritePin(ledPins[idx].gpioPort, (uint16_t)ledPins[idx].gpioConfig.Pin,
                          GPIO_PIN_SET);
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
        HAL_GPIO_WritePin(ledPins[idx].gpioPort, (uint16_t)ledPins[idx].gpioConfig.Pin,
                          GPIO_PIN_RESET);
        EvbDelayMs(flashMs);
        HAL_GPIO_WritePin(ledPins[idx].gpioPort, (uint16_t)ledPins[idx].gpioConfig.Pin,
                          GPIO_PIN_SET);
        EvbDelayMs(flashMs);
    }
    return status;
}

int32_t InitReset(void)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
    uint32_t i;
    /* Reset Pins GPIO Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /* Setup GPIOs for the Reset pins */
    for (i = 0; i < numReset; i++)
    {
        HAL_GPIO_Init(resetPins[i].gpioPort, &resetPins[i].gpioConfig);
    }

    return status;
}

int32_t InitLed(void)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
    uint32_t i;
    /* Led Pins GPIO Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();

    /* Setup GPIOs for the Reset pins */
    for (i = 0; i < numLed; i++)
    {
        HAL_GPIO_Init(ledPins[i].gpioPort, &ledPins[i].gpioConfig);
    }

    return status;
}

int32_t ToggleResetB(uint32_t idx, uint32_t pulseMs, uint32_t waitMs)
{
    int32_t status = ADI_EVB_STATUS_SUCCESS;
    if (idx < numReset)
    {
        HAL_GPIO_WritePin(resetPins[idx].gpioPort, (uint16_t)resetPins[idx].gpioConfig.Pin,
                          GPIO_PIN_RESET);
        EvbDelayMs(pulseMs);
        HAL_GPIO_WritePin(resetPins[idx].gpioPort, (uint16_t)resetPins[idx].gpioConfig.Pin,
                          GPIO_PIN_SET);
        EvbDelayMs(waitMs);
    }
    else
    {
        status = ADI_EVB_STATUS_INVALID_INDEX;
    }
    return status;
}

/**
 */
