/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/*============= I N C L U D E S =============*/

#include "adi_evb.h"
#include "app_cfg.h"
#include "stm32h5xx.h"
#include "stm32h5xx_hal.h"
#include <stdint.h>
#include <stdio.h>

/*=============  D A T A  =============*/
/** evb gpio info*/
typedef struct
{
    /** callback*/
    ADI_EVB_GPIO_CALLBACK pfGpioCallback;

} EVB_GPIO_INFO;

/** evb gpio info*/
static EVB_GPIO_INFO gpioInfo;
/** Refer #EXTI2_IRQHandler */
void EXTI2_IRQHandler(void);
/*=============  C O D E  =============*/
int32_t EvbInitGpio(void **phGpio, ADI_EVB_GPIO_CONFIG *pConfig)
{
    int32_t status = 0;
    GPIO_InitTypeDef dreadyPinConfig;
    GPIO_InitTypeDef adcResetPinConfig;
    GPIO_InitTypeDef ledPinConfig;
    GPIO_InitTypeDef ampPdPinConfig;
    GPIO_InitTypeDef stdbyPinConfig;
    /* Enable GPIOD clock */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /* Enable GPIOB clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /* Enable GPIOE clock */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    /* Enable GPIOC clock */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /* Configure PB.2 pin as drdy pin */
    /* Configure PD.14 pin as adc reset pin */
    /* Configure LEDs PE.8 and PE.10 */

    adcResetPinConfig.Pull = GPIO_PULLUP;
    adcResetPinConfig.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    adcResetPinConfig.Pin = BOARD_CFG_ADC_RESET_PIN;
    adcResetPinConfig.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(BOARD_CFG_ADC_RESET_PORT, &adcResetPinConfig);
    HAL_GPIO_WritePin(BOARD_CFG_ADC_RESET_PORT, BOARD_CFG_ADC_RESET_PIN, GPIO_PIN_SET);

    ledPinConfig.Pull = GPIO_PULLUP;
    ledPinConfig.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    ledPinConfig.Pin = BOARD_CFG_LED1_PIN | BOARD_CFG_LED2_PIN | BOARD_CFG_LED3_PIN;
    ledPinConfig.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(BOARD_CFG_LED_PORT, &ledPinConfig);
    HAL_GPIO_WritePin(BOARD_CFG_LED_PORT, (uint16_t)ledPinConfig.Pin, GPIO_PIN_SET);

    dreadyPinConfig.Mode = GPIO_MODE_IT_FALLING;
    dreadyPinConfig.Pull = GPIO_PULLUP;
    dreadyPinConfig.Pin = BOARD_CFG_ADC_DREADY_PIN;
    HAL_GPIO_Init(BOARD_CFG_ADC_DREADY_PORT, &dreadyPinConfig);
    /* Enable and set EXTI line 0 Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(EXTI2_IRQn, APP_CFG_DREADY_INTR_PRIORITY, 0);

    // FIXME: PD12 driven high to enable the amplifier ICs for current channels
    __HAL_RCC_GPIOD_CLK_ENABLE();
    ampPdPinConfig.Mode = GPIO_MODE_OUTPUT_PP;
    ampPdPinConfig.Pull = GPIO_PULLUP;
    ampPdPinConfig.Pin = BOARD_CFG_AMP_PD_PIN;
    HAL_GPIO_Init(BOARD_CFG_AMP_PD_PORT, &ampPdPinConfig);
    HAL_GPIO_WritePin(BOARD_CFG_AMP_PD_PORT, BOARD_CFG_AMP_PD_PIN, GPIO_PIN_SET);

    stdbyPinConfig.Pull = GPIO_PULLUP;
    stdbyPinConfig.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    stdbyPinConfig.Pin = BOARD_CFG_STDBY_PIN;
    stdbyPinConfig.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(BOARD_CFG_STDBY_PORT, &stdbyPinConfig);
    HAL_GPIO_WritePin(BOARD_CFG_STDBY_PORT, (uint16_t)stdbyPinConfig.Pin, GPIO_PIN_SET);

    *phGpio = &gpioInfo;
    gpioInfo.pfGpioCallback = pConfig->pfGpioCallback;
    return status;
}

void EvbEnableGPIOIrq(uint32_t port, uint32_t pin)
{
    (void)port; // Dummy use of port argument

    __HAL_GPIO_EXTI_CLEAR_FALLING_IT(pin);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}

void EvbDisableGPIOIrq(uint32_t port, uint32_t pin)
{
    (void)port; // Dummy use of port argument
    (void)pin;  // Dummy use of pin argument

    HAL_NVIC_DisableIRQ(EXTI2_IRQn);
}

int32_t EvbEnableDreadyIrq(uint8_t enable)
{
    int32_t status = 0;
    uint32_t pin = BOARD_CFG_ADC_DREADY_PIN;
    if (enable == 1)
    {
        __HAL_GPIO_EXTI_CLEAR_FALLING_IT(pin);
        HAL_NVIC_EnableIRQ(EXTI2_IRQn);
    }
    else if (enable == 0)
    {
        HAL_NVIC_DisableIRQ(EXTI2_IRQn);
    }
    else
    {
        status = 1;
    }

    return status;
}

void EvbAdcReset(void)
{
    if (BOARD_CFG_ADC_RESET_PIN)
    {
        HAL_GPIO_WritePin(BOARD_CFG_ADC_RESET_PORT, BOARD_CFG_ADC_RESET_PIN, GPIO_PIN_RESET);
        EvbDelayMs(100);
        HAL_GPIO_WritePin(BOARD_CFG_ADC_RESET_PORT, BOARD_CFG_ADC_RESET_PIN, GPIO_PIN_SET);
    }
}

void EXTI2_IRQHandler(void)
{
    /* EXTI line interrupt detected */
    if (__HAL_GPIO_EXTI_GET_FALLING_IT(BOARD_CFG_ADC_DREADY_PIN) != 0U)
    {
        __HAL_GPIO_EXTI_CLEAR_FALLING_IT(BOARD_CFG_ADC_DREADY_PIN);
        gpioInfo.pfGpioCallback((uint32_t)BOARD_CFG_ADC_DREADY_PORT, BOARD_CFG_ADC_DREADY_PIN);
    }
}
