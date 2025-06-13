/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        board_cfg.h
 * @brief       configuration file for ADE9178 eval board
 * @addtogroup SPI drivers
 * @{
 */

#ifndef __BOARD_CFG_H__
#define __BOARD_CFG_H__

/*============= I N C L U D E S =============*/

#include "gpio.h"
#include "max32670.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============  D E F I N I T I O N S  =============*/

/** Port for HOST_RDY and HOST_ERR pins  */
#define BOARD_CFG_ADECOMM_PORT MXC_GPIO0
/** Port for IRQ pins  */
#define BOARD_CFG_ADEIRQ_PORT MXC_GPIO0
/** Number of port 0 GPIOs */
#define BOARD_CFG_NUM_PORT0_GPIOS 8
/** dummy macro */
#define BOARD_CFG_MB85RS_PRODUCT_ID 0
/** Pin - HOST_RDY */
#define BOARD_CFG_HOST_RDY_PIN MXC_GPIO_PIN_22
/** Pin - HOST_ERR */
#define BOARD_CFG_HOST_ERR_PIN MXC_GPIO_PIN_21
/**  Pin  -IRQ0*/
#define BOARD_CFG_IRQ0_PIN MXC_GPIO_PIN_27
/**  Pin -  IRQ1 */
#define BOARD_CFG_IRQ1_PIN MXC_GPIO_PIN_26
/**  Pin - IRQ2 */
#define BOARD_CFG_IRQ2_PIN MXC_GPIO_PIN_25
/**  Pin - IRQ3 */
#define BOARD_CFG_IRQ3_PIN MXC_GPIO_PIN_23
/**  Pin - CF1 */
#define BOARD_CFG_CF1_PIN MXC_GPIO_PIN_6
/**  Pin - CF2 */
#define BOARD_CFG_CF2_PIN MXC_GPIO_PIN_20
/**  Pin - HOST_CS */
#define BOARD_CFG_SS_PIN MXC_GPIO_PIN_5
/** ADE9178 SPI */
#define BOARD_CFG_ADE9178_SPI MXC_SPI0
/** ADE9178 WFS UART */
#define BOARD_CFG_WFS_UART MXC_UART1
/** ADE9178 Host UART */
#define BOARD_CFG_HOST_UART MXC_UART0
/** Number of SPI0 slaves  */
#define BOARD_CFG_ADE9178_SPI_NUM_SLAVES 1
/** Slave index */
#define BOARD_CFG_ADE9178_SS_INDEX 0
/**  Pin -  ADC reset */
#define BOARD_CFG_ADC_RESET_PIN MXC_GPIO_PIN_18
/**  Pin -  ADE reset */
#define BOARD_CFG_ADE_RESET_PIN MXC_GPIO_PIN_19
/**  Pin -  led */
#define BOARD_CFG_LED1_PIN MXC_GPIO_PIN_7
/**  Pin -  led */
#define BOARD_CFG_LED2_PIN MXC_GPIO_PIN_30
/**  Reset type either for EVB or EVK */
#define BOARD_CFG_RESET_TYPE 0
/**  Timer  clock type */
#define BOARD_CFG_SYSTEM_TIMER_CLOCK_TYPE 0
/**  Timer */
#define BOARD_CFG_SYSTEM_TIMER MXC_TMR0

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
