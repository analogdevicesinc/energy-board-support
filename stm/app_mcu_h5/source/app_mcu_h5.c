/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     app_mcu_h5.c
 * @brief
 * @{
 */

/*============= I N C L U D E S =============*/

#include "adi_evb.h"
#include "app_cfg.h"
#include "stm32h563xx_spi_config.h"
#include "stm32h5xx.h"
#include "stm32h5xx_hal.h"
#include "stm32h5xx_it.h"
#include <stdint.h>
#include <stdio.h>

/*=============  D E F I N I T I O N S  =============*/

/*=============  D A T A  =============*/
/** evb info*/
static ADI_EVB_INFO evbInfo;

/**
 * Enable DMA controller clock
 */
static void MX_GPDMA1_Init(void);

static void MX_GPDMA2_Init(void);

/**
 * Enable ICACHE
 */
static void MX_ICACHE_Init(void);

/**
 * @brief System Clock Configuration
 * @return HAL error codes
 */
static HAL_StatusTypeDef SysClockConfig(void);

/*=============  C O D E  =============*/

int32_t EvbInit(void **phEvb, ADI_EVB_CONFIG *pConfig)
{
    int32_t status = 0;
    ADI_EVB_INFO *pEvb = &evbInfo;

    if (pConfig == NULL)
    {
        pConfig = &pEvb->config;
    }

    SysClockConfig();
    HAL_Init();
    MX_GPDMA1_Init();
    MX_GPDMA2_Init();
    MX_ICACHE_Init();

#if APP_CFG_ENABLE_SPI == 1
    status = EvbInitSpi(&pEvb->hSpi, &pConfig->spiConfig);
#endif
#if APP_CFG_ENABLE_GPIO == 1
    if (status == 0)
    {
        status = EvbInitGpio(&pEvb->hGpio, &pConfig->gpioConfig);
    }
#endif
#if APP_CFG_ENABLE_HOST_UART == 1
    if (status == 0)
    {
        status = EvbInitUart(&pEvb->hUart, &pConfig->uartConfig);
    }
#endif
#if APP_CFG_ENABLE_HW_CRC == 1
    if (status == 0)
    {
        status = EvbInitCrc(&pEvb->hCrc, &pConfig->crcConfig);
    }
#endif
#if APP_CFG_ENABLE_SYSTEM_TIMER == 1
    if (status == 0)
    {
        status = (int32_t)EvbTimerInit();
    }
#endif
#if APP_CFG_ENABLE_LP_TIMER == 1
    if (status == 0)
    {
        status = (int32_t)EvbLowPowerTimerInit();
    }
#endif
#if APP_CFG_ENABLE_DISPLAY == 1
    if (status == 0)
    {
        status = (int32_t)EvbInitI2C();
    }
#endif

    if (status == 0)
    {
        *phEvb = &evbInfo;
    }

    return status;
}

static void MX_ICACHE_Init(void)
{
    if (HAL_ICACHE_Enable() != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_GPDMA1_Init(void)
{
    __HAL_RCC_GPDMA1_CLK_ENABLE();
#if APP_CFG_ENABLE_DISPLAY == 1
    HAL_NVIC_SetPriority(GPDMA1_Channel4_IRQn, APP_CFG_I2C_INTR_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel4_IRQn);
#endif
    /* GPDMA1 interrupt Init */
#if APP_CFG_ENABLE_HOST_UART == 1
    HAL_NVIC_SetPriority(GPDMA1_Channel5_IRQn, APP_CFG_UART_INTR_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel5_IRQn);
#endif
#if APP_CFG_ENABLE_SPI == 1
    HAL_NVIC_SetPriority(GPDMA1_Channel6_IRQn, APP_CFG_ADC_SPI_INTR_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel6_IRQn);
    HAL_NVIC_SetPriority(GPDMA1_Channel7_IRQn, APP_CFG_ADC_SPI_INTR_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel7_IRQn);
#endif
}

static void MX_GPDMA2_Init(void)
{
#if APP_CFG_ENABLE_SPI == 1
    __HAL_RCC_GPDMA2_CLK_ENABLE();

    /* GPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA2_Channel6_IRQn, APP_CFG_FRAM_SPI_INTR_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(GPDMA2_Channel6_IRQn);
    HAL_NVIC_SetPriority(GPDMA2_Channel7_IRQn, APP_CFG_FRAM_SPI_INTR_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(GPDMA2_Channel7_IRQn);
#endif
}

HAL_StatusTypeDef SysClockConfig(void)
{
    HAL_StatusTypeDef status = 0;
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
    {
    }

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 2;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_3;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    status = HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                  RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

    if (status == HAL_OK)
    {
        status = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
    }

    /** Configure the programming delay */
    __HAL_FLASH_SET_PROGRAM_DELAY(FLASH_PROGRAMMING_DELAY_2);
    return status;
}

/**
 * @}
 */
