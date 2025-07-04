/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     stm32h563xx_spi_config.c
 * @brief    SPI configuration
 * @{
 */

/*============= I N C L U D E S =============*/
#include "stm32h563xx_spi_config.h"
#include "board_cfg.h"
#include "stm32h5xx.h"
#include "stm32h5xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern SPI_HandleTypeDef hSPI;
extern SPI_HandleTypeDef hFramSpi;

/*=============  C O D E  =============*/
int32_t StmInitSpi(STM_SPI_INSTANCE *pSpiInfo)
{
    HAL_StatusTypeDef halStatus;
    int32_t status = 0;
    SPI_HandleTypeDef *hspi = pSpiInfo->phSpi;
    hspi->Instance = pSpiInfo->pSpi;
    hspi->Init.Mode = pSpiInfo->master;
    hspi->Init.Direction = SPI_DIRECTION_2LINES;
    hspi->Init.DataSize = SPI_DATASIZE_8BIT;
    hspi->Init.CLKPolarity = pSpiInfo->clkPol;
    hspi->Init.CLKPhase = pSpiInfo->clkPha;
    hspi->Init.NSS = SPI_NSS_SOFT;
    hspi->Init.BaudRatePrescaler = pSpiInfo->prescaler;
    hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi->Init.TIMode = SPI_TIMODE_DISABLE;
    hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi->Init.CRCPolynomial = 0x11021;
    hspi->Init.CRCLength = SPI_CRC_LENGTH_16BIT;
    hspi->Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ONE_PATTERN;
    hspi->Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ONE_PATTERN;
    hspi->Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    hspi->Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
    hspi->Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
    hspi->Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
    hspi->Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    hspi->Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    hspi->Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
    hspi->Init.IOSwap = SPI_IO_SWAP_DISABLE;
    hspi->Init.ReadyMasterManagement = SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
    hspi->Init.ReadyPolarity = SPI_RDY_POLARITY_HIGH;
    halStatus = HAL_SPI_Init(hspi);

    if (halStatus != HAL_OK)
    {
        status = -1;
    }
    return status;
}

int32_t StmStartSpiTxDMA(STM_SPI_INSTANCE *pSpiInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 0;
    HAL_GPIO_WritePin(pSpiInfo->ssPort, pSpiInfo->ssPin, GPIO_PIN_RESET);
    status = HAL_SPI_Transmit_DMA(pSpiInfo->phSpi, pData, (uint16_t)numBytes);

    return status;
}
int32_t StmStartSpiRxDMA(STM_SPI_INSTANCE *pSpiInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 0;
    HAL_GPIO_WritePin(pSpiInfo->ssPort, pSpiInfo->ssPin, GPIO_PIN_RESET);

    status = HAL_SPI_TransmitReceive_DMA(pSpiInfo->phSpi, (uint8_t *)&pSpiInfo->dummyCmd, pData,
                                         (uint16_t)numBytes);

    return status;
}

int32_t StmStartSpiTxRxDMA(STM_SPI_INSTANCE *pSpiInfo, uint8_t *pTxData, uint8_t *pRxData,
                           uint32_t numBytes)
{
    int32_t status = 0;
    HAL_GPIO_WritePin(pSpiInfo->ssPort, pSpiInfo->ssPin, GPIO_PIN_RESET);
    status = HAL_SPI_TransmitReceive_DMA(pSpiInfo->phSpi, pTxData, pRxData, (uint16_t)numBytes);

    return status;
}

int32_t StmSetSpiFrequency(STM_SPI_INSTANCE *pSpiInfo, uint32_t spiFrequency)
{
    int32_t status = 0;
    uint32_t pclk = 0;
    uint32_t val = 0;
    if (pSpiInfo->phSpi->Instance == pSpiInfo->pSpi)
    {
        pclk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI1);
    }

    for (val = 0U; val < 8U; val++)
    {
        if (spiFrequency >= (pclk >> (val + 1U)))
        {
            break;
        }
    }
    if ((val == 8U) || (spiFrequency < (pclk >> (val + 1U))))
    {
        // Requested Bus Speed can not be configured
        status = -1;
    }
    if (status == 0)
    {
        // Save prescaler value
        pSpiInfo->phSpi->Init.BaudRatePrescaler = _VAL2FLD(SPI_CFG1_MBR, val);
        status = HAL_SPI_Init(pSpiInfo->phSpi);
    }
    return status;
}

void StmSetChipSelect(void)
{
    GPIO_InitTypeDef ssGpioConfig;
    /* Enable GPIOA clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Deinit GPIO as LPTIM2 output*/
    HAL_GPIO_DeInit(BOARD_CFG_SS_PORT, BOARD_CFG_SS_PIN);

    /* Configure PA.4 ss pin*/
    ssGpioConfig.Mode = GPIO_MODE_OUTPUT_PP;
    ssGpioConfig.Pull = GPIO_PULLUP;
    ssGpioConfig.Pin = BOARD_CFG_SS_PIN;
    ssGpioConfig.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BOARD_CFG_SS_PORT, &ssGpioConfig);
    // HAL_GPIO_Init makes the (CS) pin low for some reason, so make it high.
    HAL_GPIO_WritePin(BOARD_CFG_SS_PORT, BOARD_CFG_SS_PIN, GPIO_PIN_SET);
}

/**
 * @}
 */
