/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        board_cfg.h
 * @brief       configuration file for STM32H573xx
 * @addtogroup SPI drivers
 * @{
 */

#ifndef __BOARD_CFG_H__
#define __BOARD_CFG_H__

/*============= I N C L U D E S =============*/

#include "app_cfg.h"
#include "stm32h5xx.h"
#include "stm32h5xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============  D E F I N I T I O N S  =============*/

/** Gpio Port for Reset pins*/
#define BOARD_CFG_ADC_RESET_PORT GPIOD
/** Gpio Port for Reset pins*/
#define BOARD_CFG_ADC_DREADY_PORT GPIOB
/** Gpio Port for Led pins*/
#define BOARD_CFG_LED_PORT GPIOE
/** Gpio Port for stdby pins*/
#define BOARD_CFG_STDBY_PORT GPIOD
/**  Port - ADEMA127_SPI_CS */
#define BOARD_CFG_SS_PORT GPIOA
/**  Port - AMP_PD */
#define BOARD_CFG_AMP_PD_PORT GPIOD
/** Duplicate macro */
#define BOARD_CFG_OUTPUT_PORT BOARD_CFG_ADC_RESET_PORT
/** Pin - ADC reset */
#define BOARD_CFG_ADC_RESET_PIN GPIO_PIN_14
/** Pin - ADC dready */
#define BOARD_CFG_ADC_DREADY_PIN GPIO_PIN_2
/** ADE Reset Pin*/
#define BOARD_CFG_ADE_RESET_PIN 0
/** Pin - LED 1 */
#define BOARD_CFG_LED1_PIN GPIO_PIN_8
/** Pin - LED 2 */
#define BOARD_CFG_LED2_PIN GPIO_PIN_10
/** Pin - LED 3 */
#define BOARD_CFG_LED3_PIN GPIO_PIN_12
/**  Pin - CF2 */
#define BOARD_CFG_CF2_PIN (0)
/**  Pin - CF3 */
#define BOARD_CFG_CF3_PIN (0)
/**  Pin - ADEMA127_SPI_CS */
#define BOARD_CFG_SS_PIN GPIO_PIN_4
/**  Pin - AMP_PD */
#define BOARD_CFG_AMP_PD_PIN GPIO_PIN_12
/**  Pin - STDBY*/
#define BOARD_CFG_STDBY_PIN GPIO_PIN_13
/** ADEMA127 SPI */
#define BOARD_CFG_ADC_SPI SPI1

#if APP_CFG_USE_ANSI_FRAM == 1
/**  Pin - FRAM_SPI_CS */
#define BOARD_CFG_FRAM_SS_PIN GPIO_PIN_15
/** FRAM SPI */
#define BOARD_CFG_FRAM_SPI SPI3
#else
/**  Pin - FRAM_SPI_CS */
#define BOARD_CFG_FRAM_SS_PIN GPIO_PIN_0
/** FRAM SPI */
#define BOARD_CFG_FRAM_SPI    SPI6
#endif

/** ADEMA127 SPI Speed*/
#define BOARD_CFG_ADC_SPI_PRESCALER SPI_BAUDRATEPRESCALER_16
/** ADEMA127 SPI clk polarity*/
#define BOARD_CFG_ADC_SPI_CLK_POL SPI_POLARITY_HIGH
/** ADEMA127 SPI clk phase*/
#define BOARD_CFG_ADC_SPI_CLK_PHA SPI_PHASE_2EDGE
/** Slave index */
#define BOARD_CFG_ADC_SS_INDEX 0
/** Slave index */
#define BOARD_CFG_FRAM_SS_INDEX 0
/**  Reset type */
#define BOARD_CFG_RESET_TYPE 1
/**  Timer  clock type */
#define BOARD_CFG_SYSTEM_TIMER_CLOCK_TYPE 0
/** ADEMA127 Host UART */
#define BOARD_CFG_HOST_UART UART5
/**  product id of FRAM */
#define BOARD_CFG_MB85RS_PRODUCT_ID 0x03480000

/** STM config macros for no OS layer */
/** SPI ID */
#define BOARD_CFG_NO_OS_ADC_SPI 1 // SPI1
/** CS PIN NUMBER */
#define BOARD_CFG_NO_OS_SS_PIN 4 // PA4
/** CS PORT NUMBER */
#define BOARD_CFG_NO_OS_SS_PORT 0 // PORTA
/** SPI ID */
#define BOARD_CFG_NO_OS_SPI_SPEED 15625000 // 250 MHz/SPI_BAUDRATEPRESCALER_16 = 15625000 Hz
/** ADC Rx DMA IRQ */
#define BOARD_CFG_NO_OS_ADC_SPI_RX_DMA_IRQ GPDMA1_Channel7_IRQn
/** ADC Tx DMA Channel */
#define BOARD_CFG_NO_OS_ADC_SPI_RX_DMA_CHANNEL GPDMA1_Channel7
/** ADC Rx DMA Channel */
#define BOARD_CFG_NO_OS_ADC_SPI_TX_DMA_CHANNEL GPDMA1_Channel6
/** No of DMA Channel */
#define BOARD_CFG_NO_OS_ADC_DMA_NUM_CHANNELS 2

/** Macro to switch between New and Old App MCU Board */
#ifdef USE_OLD_MCUBOARD
/**  Timer */
#define BOARD_CFG_SYSTEM_TIMER TIM5
/**  PWM Timer for CF1 */
#define BOARD_CFG_SYSTEM_CF1_TIMER TIM2
/**  CF1 Timer Channel */
#define BOARD_CFG_CF1_TIM_CHAN TIM_CHANNEL_1
/**  Pin - CF1 */
#define BOARD_CFG_CF1_PIN GPIO_PIN_15
/**  Port - CF */
#define BOARD_CFG_CF1_PORT GPIOA
/** CF Pin Alternate Function */
#define CF1_PIN_ALT_FUNC GPIO_AF1_TIM2
/** CF IRQ Number */
#define CF1_TIM_IRQn TIM2_IRQn
#else
/**  Timer */
#define BOARD_CFG_SYSTEM_TIMER     TIM2
/**  PWM Timer for CF1 */
#define BOARD_CFG_SYSTEM_CF1_TIMER TIM5
/**  Timer */
#define BOARD_CFG_LP_TIMER         LPTIM2
/**  CF1 Timer Channel */
#define BOARD_CFG_CF1_TIM_CHAN     TIM_CHANNEL_4
/**  Pin - CF1 */
#define BOARD_CFG_CF1_PIN          GPIO_PIN_3
/**  Port - CF */
#define BOARD_CFG_CF1_PORT         GPIOA
/** CF Pin Alternate Function */
#define CF1_PIN_ALT_FUNC           GPIO_AF2_TIM5
/** CF IRQ Number */
#define CF1_TIM_IRQn               TIM5_IRQn
#endif

/** I2C Instance */
#define BOARD_CFG_I2C_INSTANCE I2C2

/** Empty define - The EVK doesn't require a section attribute */
#define SECTION(SECTION_NAME)
/** Empty define - The EVK doesn't require DMA Buffer section for the linker */
#define DMA_BUFFER
/** Empty define - The EVK doesn't require FRAM Buffer section for the linker */
#define FRAM_BUFFER

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_CFG_H__ */

/** @} */
