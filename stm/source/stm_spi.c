/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     stm_spi.c
 * @brief    SPI config and communication for ADC
 */

/*============= I N C L U D E S =============*/
#include "adi_evb.h"
#include "app_cfg.h"
#ifdef STM32H735xx
#include "stm32h735xx_spi_config.h"
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_spi.h"
#include "stm32h7xx_it.h"
#else
#include "stm32h563xx_spi_config.h"
#include "stm32h5xx.h"
#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_spi.h"
#include "stm32h5xx_it.h"
#endif
#include <stdint.h>
#include <string.h>

/*=============  D E F I N I T I O N S  =============*/
/** Evb spi info */
typedef struct
{
    /** maxim spi instance info */
    STM_SPI_INSTANCE adcSpiInfo;
    /** callback */
    ADI_EVB_CALLBACK pfAdeSpiRxCallback;
    /** callback */
    ADI_EVB_CALLBACK pfAdeSpiTxCallback;

} EVB_SPI_INFO;

/** Handle to SPI1*/
SPI_HandleTypeDef hSPI;

/** creating dma spitx instance*/
DMA_HandleTypeDef hAdcDMASPITx;
/** creating dma spirx instance*/
DMA_HandleTypeDef hAdcDMASPIRx;
static void AdeSpiTxRxCallBack(SPI_HandleTypeDef *hspi);

/*============= F U N C T I O N S =============*/

/**
 * @brief Initializes GPIO ports and pins for slave select
 */
int32_t EvbAdeSlaveSelectConfig(void);

/** Evb spi init */
static int32_t InitAdeSpi(ADI_EVB_SPI_CONFIG *pConfig);

/*=============  D A T A  =============*/

/** Flag to indicate whether blocking is enabled so that the ISR can suspend it */
static volatile uint8_t isSuspendCalled = 0;
/** Evb spi info */
static EVB_SPI_INFO evbSpiInfo;

/*========================== C O D E ==========================*/

int32_t EvbInitAdeSpi(void **phSpi, ADI_EVB_SPI_CONFIG *pConfig)
{
    int32_t status = 0;

    status = InitAdeSpi(pConfig);

    if (status == 0)
    {
        /* Just a keeping a API provision to support multiple SPIs later*/
        *phSpi = &evbSpiInfo;
    }

    return status;
}

int32_t EvbAdeSpiTransceive(void *hEvb, uint8_t *pTxData, uint8_t *pRxData, uint32_t numBytes,
                            uint32_t timeOutCount)
{
    int32_t status = -1;
    STM_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.adcSpiInfo;
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

int32_t EvbAdeSpiTransceiveAsync(void *hEvb, uint8_t *pTxData, uint8_t *pRxData, uint32_t numBytes)
{
    int32_t status = -1;
    STM_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.adcSpiInfo;
    if (hEvb != NULL)
    {
        status = StmStartSpiTxRxDMA(pSpiInfo, pTxData, pRxData, numBytes);
    }

    return status;
}

int32_t EvbAdeSpiTransmitAsync(void *hEvb, uint8_t *pTxData, uint32_t numBytes)
{
    int32_t status = -1;
    STM_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.adcSpiInfo;
    if (hEvb != NULL)
    {
        status = StmStartSpiTxDMA(pSpiInfo, pTxData, numBytes);
    }

    return status;
}

int32_t EvbAdeSpiSetFrequency(void *hEvb, uint32_t spiFrequency)
{
    int32_t status = -1;
    STM_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.adcSpiInfo;
    if (hEvb != NULL)
    {
        status = StmSetSpiFrequency(pSpiInfo, spiFrequency);
    }

    return status;
}

int32_t InitAdeSpi(ADI_EVB_SPI_CONFIG *pConfig)
{
    int32_t status = 0;
    STM_SPI_INSTANCE *pStmSpiInfo = &evbSpiInfo.adcSpiInfo;
    evbSpiInfo.pfAdeSpiRxCallback = pConfig->pfAdeSpiRxCallback;
    evbSpiInfo.pfAdeSpiTxCallback = pConfig->pfAdeSpiTxCallback;
    pStmSpiInfo->pSpi = BOARD_CFG_ADC_SPI;
    pStmSpiInfo->prescaler = BOARD_CFG_ADC_SPI_PRESCALER;
    pStmSpiInfo->master = SPI_MODE_MASTER;
    pStmSpiInfo->phSpi = &hSPI;
    pStmSpiInfo->clkPol = BOARD_CFG_ADC_SPI_CLK_POL;
    pStmSpiInfo->clkPha = BOARD_CFG_ADC_SPI_CLK_PHA;
    pStmSpiInfo->suspendState = 1;
    pStmSpiInfo->ssPin = BOARD_CFG_SS_PIN;
    pStmSpiInfo->ssPort = BOARD_CFG_SS_PORT;
    status = StmInitSpi(pStmSpiInfo);
    HAL_SPI_RegisterCallback(&hSPI, HAL_SPI_TX_RX_COMPLETE_CB_ID, AdeSpiTxRxCallBack);
    if (status == 0)
    {
        status = EvbAdeSlaveSelectConfig();
    }
    return status;
}

int32_t EvbAdeSlaveSelectConfig(void)
{
    int32_t status = 0;
    GPIO_InitTypeDef ssGpioConfig;
    /* Enable GPIOA clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure PA.4 ss pin*/
    ssGpioConfig.Mode = GPIO_MODE_OUTPUT_PP;
    ssGpioConfig.Pull = GPIO_PULLUP;
    ssGpioConfig.Pin = BOARD_CFG_SS_PIN;
    ssGpioConfig.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BOARD_CFG_SS_PORT, &ssGpioConfig);
    // HAL_GPIO_Init makes the (CS) pin low for some reason, so make it high.
    HAL_GPIO_WritePin(BOARD_CFG_SS_PORT, BOARD_CFG_SS_PIN, GPIO_PIN_SET);
    return status;
}

void EvbAdeSpiSetChipSelect(void)
{
    StmSetChipSelect();
}

void AdeSpiTxRxCallBack(SPI_HandleTypeDef *hspi)
{
    if (hspi != NULL)
    {
        STM_SPI_INSTANCE *pSpiInfo = &evbSpiInfo.adcSpiInfo;
        if (evbSpiInfo.pfAdeSpiTxCallback != NULL)
        {
            evbSpiInfo.pfAdeSpiTxCallback();
        }
        if (evbSpiInfo.pfAdeSpiRxCallback != NULL)
        {
            if (isSuspendCalled == 1)
            {
                pSpiInfo->suspendState = 0;
            }
            else
            {
                evbSpiInfo.pfAdeSpiRxCallback();
            }
        }
        HAL_GPIO_WritePin((GPIO_TypeDef *)BOARD_CFG_SS_PORT, BOARD_CFG_SS_PIN, GPIO_PIN_SET);
    }
}

/**
 */
