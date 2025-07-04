/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     stm_i2c.c
 * @brief    This file contains the I2C routines for STM platform.
 * @{
 */

/*============= I N C L U D E S =============*/
#include "adi_evb.h"
#include "stm32h563x_i2c_config.h"

/*=============  D A T A  =============*/

/** Evb I2C Info */
typedef struct
{
    /** I2C instance */
    STM_I2C_INSTANCE i2cInfo;
    /** I2C Transmit Completion Flag */
    volatile uint8_t isTxComplete;
} EVB_I2C_INFO;

static EVB_I2C_INFO evbI2cInfo;
/** i2c instance */
I2C_HandleTypeDef hI2C;
/** DMA I2C Handle Instance */
DMA_HandleTypeDef hDMAI2CTx;

uint32_t EvbInitI2C(void)
{
    uint32_t status = 0;
    STM_I2C_INSTANCE *pI2cInfo = &evbI2cInfo.i2cInfo;
    pI2cInfo->pI2c = BOARD_CFG_I2C_INSTANCE;
    status = StmI2CInit(pI2cInfo);
    evbI2cInfo.isTxComplete = 1;
    return status;
}

uint32_t EvbTransmitI2C(uint8_t *pData, uint16_t devAddr, uint16_t len)
{
    uint32_t status = 0;
    STM_I2C_INSTANCE *pI2cInfo = &evbI2cInfo.i2cInfo;
    status = StmI2CTransmit(pI2cInfo, devAddr, pData, len);
    evbI2cInfo.isTxComplete = 0;
    return status;
}

uint32_t EvbReceiveI2C(uint8_t *pData, uint16_t devAddr, uint16_t len)
{
    uint32_t status = 0;
    STM_I2C_INSTANCE *pI2cInfo = &evbI2cInfo.i2cInfo;
    status = StmI2CReceive(pI2cInfo, devAddr, pData, len);
    return status;
}

void EvbHandlerI2CTx(void)
{
    evbI2cInfo.isTxComplete = 1;
}

volatile uint8_t *EvbGetTxFlag(void)
{
    return &evbI2cInfo.isTxComplete;
}

/**
 * @}
 */