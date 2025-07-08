/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     app_mcu_h5_spi.c
 * @brief    SPI config and communication for FRAM and ADC
 */

/*============= I N C L U D E S =============*/
#include "adi_evb.h"
#include "app_cfg.h"
#include "stm32h563xx_spi_config.h"
#include "stm32h5xx.h"
#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_spi.h"
#include "stm32h5xx_it.h"
#include <stdint.h>
#include <string.h>

/*=============  D E F I N I T I O N S  =============*/

/** Evb spi info */
typedef struct
{
    /** maxim spi instance info */
    STM_SPI_INSTANCE framSpiInfo;
    /** callback */
    ADI_EVB_CALLBACK pfFramSpiRxCallback;
    /** callback */
    ADI_EVB_CALLBACK pfFramSpiTxCallback;

} EVB_SPI_INFO;

/** Handle to SPI6 */
SPI_HandleTypeDef hFramSpi;
/** creating dma spitx instance*/
DMA_HandleTypeDef hFramDMASPITx;
/** creating dma spirx instance*/
DMA_HandleTypeDef hFramDMASPIRx;
/** Flag to indicate whether blocking is enabled so that the ISR can suspend it */
static volatile uint8_t isSuspendCalled = 0;
/**
 * @brief Initualizes GPIO ports and pins for slave select
 */
int32_t EvbFramSlaveSelectConfig(void);

static int32_t InitFramSpi(ADI_EVB_SPI_CONFIG *pConfig);
static void FramSpiTxRxCallBack(SPI_HandleTypeDef *hspi);
static void FramSpiTxCallBack(SPI_HandleTypeDef *hspi);

/*=============  D A T A  =============*/

/** Evb spi info */
static EVB_SPI_INFO evbSpiInfo;

/*========================== C O D E ==========================*/

int32_t EvbInitSpi(void **phSpi, ADI_EVB_SPI_CONFIG *pConfig)
{
    int32_t status = 0;

    status = EvbInitAdeSpi(phSpi, pConfig);
    status = InitFramSpi(pConfig);
    if (status == 0)
    {
        /* Just a keeping a API provision to support multiple SPIs later*/
        *phSpi = &evbSpiInfo;
    }

    return status;
}

int32_t EvbNvmRead(void *hEvb, uint8_t *pTxData, uint8_t *pRxData, uint32_t numBytes,
                   uint32_t timeOutCount)
{
    int32_t status = -1;
    STM_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.framSpiInfo;
    uint32_t waitCount = 0;
    if (hEvb != NULL)
    {
        isSuspendCalled = 1;
        status = StmStartSpiTxRxDMA(pSpiInfo, pTxData, pRxData, numBytes);
        if (status == 0)
        {
            while ((pSpiInfo->suspendState == 1) && (waitCount < timeOutCount))
            {
                waitCount++;
            }
            pSpiInfo->suspendState = 1;
            if (waitCount == timeOutCount)
            {
                status = 1;
            }
        }
        else
        {
            status = -1;
        }
        isSuspendCalled = 0;
    }
    return status;
}

int32_t EvbNvmWrite(void *hEvb, uint8_t *pData, uint32_t numBytes, uint32_t timeOutCount)
{
    int32_t status = -1;
    STM_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.framSpiInfo;
    uint32_t waitCount = 0;
    if (hEvb != NULL)
    {
        isSuspendCalled = 1;
        status = StmStartSpiTxDMA(pSpiInfo, pData, numBytes);
        if (status == 0)
        {
            while ((pSpiInfo->suspendState == 1) && (waitCount < timeOutCount))
            {
                waitCount++;
            }
            pSpiInfo->suspendState = 1;
            if (waitCount == timeOutCount)
            {
                status = 1;
            }
        }
        else
        {
            status = -1;
        }
        isSuspendCalled = 0;
    }

    return status;
}

int32_t InitFramSpi(ADI_EVB_SPI_CONFIG *pConfig)
{
    int32_t status = 0;
    STM_SPI_INSTANCE *pStmSpiInfo = &evbSpiInfo.framSpiInfo;
    evbSpiInfo.pfFramSpiRxCallback = pConfig->pfFramSpiRxCallback;
    evbSpiInfo.pfFramSpiTxCallback = pConfig->pfFramSpiTxCallback;
    pStmSpiInfo->pSpi = BOARD_CFG_FRAM_SPI;
    pStmSpiInfo->prescaler = BOARD_CFG_ADC_SPI_PRESCALER;
    pStmSpiInfo->master = SPI_MODE_MASTER;
    pStmSpiInfo->clkPol = BOARD_CFG_ADC_SPI_CLK_POL;
    pStmSpiInfo->clkPha = BOARD_CFG_ADC_SPI_CLK_PHA;
    pStmSpiInfo->phSpi = &hFramSpi;
    pStmSpiInfo->master = SPI_MODE_MASTER;
    pStmSpiInfo->suspendState = 1;
    pStmSpiInfo->ssPin = BOARD_CFG_FRAM_SS_PIN;
    pStmSpiInfo->ssPort = BOARD_CFG_SS_PORT;
    status = StmInitSpi(pStmSpiInfo);
    HAL_SPI_RegisterCallback(&hFramSpi, HAL_SPI_TX_RX_COMPLETE_CB_ID, FramSpiTxRxCallBack);
    HAL_SPI_RegisterCallback(&hFramSpi, HAL_SPI_TX_COMPLETE_CB_ID, FramSpiTxCallBack);
    if (status == 0)
    {
        status = EvbFramSlaveSelectConfig();
    }
    return status;
}

int32_t EvbFramSlaveSelectConfig(void)
{
    int32_t status = 0;
    GPIO_InitTypeDef ssGpioConfig;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /* Configure PA.4 ss pin*/
    ssGpioConfig.Mode = GPIO_MODE_OUTPUT_PP;
    ssGpioConfig.Pull = GPIO_PULLUP;
    ssGpioConfig.Pin = BOARD_CFG_FRAM_SS_PIN;
    ssGpioConfig.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BOARD_CFG_SS_PORT, &ssGpioConfig);
    // HAL_GPIO_Init makes the (CS) pin low for some reason, so make it high.
    HAL_GPIO_WritePin(BOARD_CFG_SS_PORT, BOARD_CFG_FRAM_SS_PIN, GPIO_PIN_SET);
    return status;
}

void FramSpiTxRxCallBack(SPI_HandleTypeDef *hspi)
{
    if (hspi != NULL)
    {
        STM_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.framSpiInfo;
        if (isSuspendCalled == 1)
        {
            pSpiInfo->suspendState = 0;
        }
        if (evbSpiInfo.pfFramSpiRxCallback != NULL)
        {
            evbSpiInfo.pfFramSpiRxCallback();
        }
        HAL_GPIO_WritePin((GPIO_TypeDef *)BOARD_CFG_SS_PORT, BOARD_CFG_FRAM_SS_PIN, GPIO_PIN_SET);
    }
}

void FramSpiTxCallBack(SPI_HandleTypeDef *hspi)
{
    if (hspi != NULL)
    {
        STM_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.framSpiInfo;
        if (isSuspendCalled == 1)
        {
            pSpiInfo->suspendState = 0;
        }
        if (evbSpiInfo.pfFramSpiTxCallback != NULL)
        {

            evbSpiInfo.pfFramSpiTxCallback();
        }
    }
    HAL_GPIO_WritePin((GPIO_TypeDef *)BOARD_CFG_SS_PORT, BOARD_CFG_FRAM_SS_PIN, GPIO_PIN_SET);
}

/**
 */
