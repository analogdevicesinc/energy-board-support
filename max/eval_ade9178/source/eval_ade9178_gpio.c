/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     eval_ade9178_gpio.c
 * @brief
 * @{
 */
/*============= I N C L U D E S =============*/

#include "adi_evb.h"
#include "app_cfg.h"
#include "mxc_device.h"
#include <stdint.h>
#include <stdio.h>

/*=============  D A T A  =============*/
/** evb gpio info*/
typedef struct
{
    /** callback */
    ADI_EVB_GPIO_CALLBACK pfGpioCallback;

} EVB_GPIO_INFO;

/** irq pins */
static uint32_t inputPinsIrqFalling[] = {BOARD_CFG_IRQ0_PIN, BOARD_CFG_IRQ1_PIN, BOARD_CFG_IRQ2_PIN,
                                         BOARD_CFG_IRQ3_PIN, BOARD_CFG_CF1_PIN,  BOARD_CFG_CF2_PIN};

static uint32_t inputPinsIrqBoth[] = {BOARD_CFG_HOST_RDY_PIN, BOARD_CFG_HOST_ERR_PIN};

static uint32_t outputPins[] = {BOARD_CFG_ADC_RESET_PIN, BOARD_CFG_ADE_RESET_PIN,
                                BOARD_CFG_LED1_PIN, BOARD_CFG_LED2_PIN};

/** evb gpio info*/
static EVB_GPIO_INFO gpioInfo;

/** gpio irq handler*/
void GpioIrqHandler(void);
static void EvbConfigureOutPin(mxc_gpio_regs_t *pGpio, uint32_t mask);

/*=============  C O D E  =============*/

int32_t EvbInitGpio(void **phGpio, ADI_EVB_GPIO_CONFIG *pConfig)
{
    IRQn_Type irqn;
    int32_t status = 0;
    int32_t i;
    mxc_gpio_cfg_t config;
    int32_t numPins = sizeof(inputPinsIrqFalling) / sizeof(inputPinsIrqFalling[0]);

    config.port = BOARD_CFG_ADEIRQ_PORT;
    config.pad = MXC_GPIO_PAD_PULL_UP;
    config.func = MXC_GPIO_FUNC_IN;
    config.vssel = MXC_GPIO_VSSEL_VDDIOH;

    for (i = 0; i < numPins; i++)
    {
        config.mask = inputPinsIrqFalling[i];
        MXC_GPIO_Config(&config);
        MXC_GPIO_IntConfig(&config, MXC_GPIO_INT_FALLING);
    }

    numPins = sizeof(inputPinsIrqBoth) / sizeof(inputPinsIrqBoth[0]);
    for (i = 0; i < numPins; i++)
    {
        config.mask = inputPinsIrqBoth[i];
        MXC_GPIO_Config(&config);
        MXC_GPIO_IntConfig(&config, MXC_GPIO_INT_BOTH);
    }

    config.func = MXC_GPIO_FUNC_OUT;
    config.vssel = MXC_GPIO_VSSEL_VDDIO;

    numPins = sizeof(outputPins) / sizeof(outputPins[0]);
    for (i = 0; i < numPins; i++)
    {
        config.mask = outputPins[i];
        EvbConfigureOutPin(BOARD_CFG_ADECOMM_PORT, outputPins[i]);
    }

    irqn = MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(BOARD_CFG_ADECOMM_PORT));
    NVIC_SetPriority(irqn, APP_CFG_PORT0_GPIO_INT_PRIO);
    NVIC_SetVector(irqn, (uint32_t)GpioIrqHandler);
    NVIC_EnableIRQ(irqn);

    *phGpio = &gpioInfo;
    gpioInfo.pfGpioCallback = pConfig->pfGpioCallback;

    return status;
}

void EvbEnableAllGPIOIrq(void)
{
    int32_t idx;
    uint32_t pins[2] = {BOARD_CFG_HOST_RDY_PIN, BOARD_CFG_HOST_ERR_PIN};
    uint32_t irqPins[6] = {BOARD_CFG_IRQ0_PIN, BOARD_CFG_IRQ1_PIN, BOARD_CFG_IRQ2_PIN,
                           BOARD_CFG_IRQ3_PIN, BOARD_CFG_CF1_PIN,  BOARD_CFG_CF2_PIN};
    for (idx = 0; idx < 2; idx++)
    {
        EvbEnableGPIOIrq((uint32_t)BOARD_CFG_ADECOMM_PORT, pins[idx]);
    }

    for (idx = 0; idx < 6; idx++)
    {
        EvbEnableGPIOIrq((uint32_t)BOARD_CFG_ADEIRQ_PORT, irqPins[idx]);
    }
}

int32_t EvbGetPinState(uint32_t port, uint32_t flag)
{
    uint32_t pinState;
    pinState = (uint8_t)MXC_GPIO_InGet((mxc_gpio_regs_t *)port, flag);
    return pinState;
}

void EvbEnableGPIOIrq(uint32_t port, uint32_t pin)
{
    MXC_GPIO_EnableInt((mxc_gpio_regs_t *)port, pin);
}

void EvbDisableGPIOIrq(uint32_t port, uint32_t pin)
{
    MXC_GPIO_DisableInt((mxc_gpio_regs_t *)port, pin);
}

void GpioIrqHandler(void)
{
    uint32_t stat;
    stat = BOARD_CFG_ADECOMM_PORT->intfl;
    if (gpioInfo.pfGpioCallback != NULL)
    {
        gpioInfo.pfGpioCallback((uint32_t)BOARD_CFG_ADECOMM_PORT, stat);
    }
    BOARD_CFG_ADECOMM_PORT->intfl_clr = stat;
}

static void EvbConfigureOutPin(mxc_gpio_regs_t *pGpio, uint32_t mask)
{
    pGpio->en0_set = mask;
    pGpio->en1_clr = mask;
    pGpio->en2_clr = mask;
    pGpio->out_set = mask;
    pGpio->outen_set = mask;
}

/**
 * @}
 */
