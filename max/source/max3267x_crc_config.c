/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     max3267x_crc_config.c
 * @brief    This file contains the routines for initializing and calculating
 *           CRC value using MAX32670 CRC Module.
 * @{
 */

/*=============  I N C L U D E S   =============*/
#include "adi_crc.h"
#include "adi_crc_cfg.h"
#include "adi_crc_hw.h"
#include "adi_evb.h"
#include "app_cfg.h"
#include "crc.h"
#include "dma.h"
#include "dma_reva.h"
#include "max32670.h"
#include "max3267x_dma_config.h"
#include "nvic_table.h"
#include <crc_reva.h>
#include <crc_reva_regs.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
/*=============  D E F I N I T I O N S  =============*/

/**
 * @brief Crc info struct for max32670
 */
typedef struct
{
    /** CRC Handle */
    ADI_CRC_HANDLE hCrc;
    /** lock crc */
    uint8_t lock;
    /** dma channel reserved for CRC */
    uint8_t dmaChannel;
    /** dma error flag */
    uint8_t dmaError;
    /** variable to check status of CRC transaction*/
    volatile bool isComplete;
    /** CRC output */
    uint32_t crc;
    /** crc request struct */
    mxc_crc_req_t crcReq;

} CRC_HW_INFO;

/*=============  D A T A  =============*/

static CRC_HW_INFO crcHw;

/*============= F U N C T I O N S =============*/

static ADI_CRC_RESULT CrcSetConfig(ADI_CRC_DATA *pData);

static ADI_CRC_RESULT ConfigCrcDma(mxc_crc_reva_req_t *pReq);

static int32_t SetupCrcDma(uint8_t dmaChannel);

static void CrcDmaIrqHandler(void);

static void CrcDmaCallBack(int ch, int error);

static ADI_CRC_RESULT FormatSeed(ADI_CRC_CONFIG *pCrcConfig);

static ADI_CRC_RESULT FormatResult(volatile ADI_CRC_CONFIG *pCrcConfig, volatile uint32_t *pData);

/*=============  C O D E  =============*/

ADI_CRC_RESULT EvbInitCrc(void **phCrc)
{
    ADI_CRC_RESULT status = ADI_CRC_RESULT_SUCCESS;
    crcHw.hCrc = *phCrc;
    crcHw.dmaChannel = (uint8_t)MXC_DMA_AcquireChannel();
    status = SetupCrcDma(crcHw.dmaChannel);

    MXC_CRC_Init();

    return status;
}

int32_t EvbCrcSetConfig(ADI_CRC_DATA *pData)
{
    int32_t status = 0;
    uint32_t crcBuff = 0x00;
    status = FormatSeed(&pData->crcCfg);
    if (status == 0)
    {
        if (pData->crcCfg.poly != 0u)
        {
            MXC_CRC->ctrl |=
                (uint32_t)((pData->crcCfg.reversed << 3) | (pData->crcCfg.bigEndian << 2));
            // Load CRC polynomial into crc polynomial register
            MXC_CRC_SetPoly((uint32_t)pData->crcCfg.poly << 16);
            crcHw.crcReq.dataBuffer = &crcBuff;
            crcHw.crcReq.dataLen = 1;
            // Initialises the DMA and CRC. Refer section 15.5 of MAX32670 user
            // guide.
            status = ConfigCrcDma((mxc_crc_reva_req_t *)&crcHw.crcReq);
        }
    }
    else
    {
        status = ADI_CRC_RESULT_FAILURE;
    }

    return status;
}

void EvbCalculateCrc(ADI_CRC_DATA *pCrcData, uint8_t *pData, uint16_t offset, uint32_t numBytes)
{
    crcHw.isComplete = false;
    /* 0x5C is the mask for status register */
    MXC_DMA->ch[crcHw.dmaChannel].status |= 0x5C;
    MXC_CRC->ctrl |= MXC_F_CRC_CTRL_EN;
    MXC_CRC->val = pCrcData->crcCfg.seed;
    /* Set start address and initiate DMA*/
    MXC_DMA->ch[crcHw.dmaChannel].src = (unsigned int)(pData + offset);
    MXC_DMA->ch[crcHw.dmaChannel].cnt = numBytes;
    MXC_DMA->ch[crcHw.dmaChannel].ctrl |= (MXC_F_DMA_CTRL_EN | MXC_F_DMA_CTRL_CTZ_IE);
}

ADI_CRC_RESULT EvbGetCrc(ADI_CRC_HANDLE hCrc, uint32_t *pData)
{
    ADI_CRC_RESULT status = ADI_CRC_RESULT_SUCCESS;
    volatile ADI_CRC_DATA *pCrcData = (ADI_CRC_DATA *)hCrc;

    if (crcHw.dmaError == 1)
    {
        status = ADI_CRC_RESULT_RUN_TIME_ERROR;
        crcHw.dmaError = 0;
    }
    else if (crcHw.isComplete == true)
    {
        *pData = pCrcData->crcValue;
        status = FormatResult(&pCrcData->crcCfg, pData);
    }
    else
    {
        *pData = 0xFFFFFFFF;
        status = ADI_CRC_RESULT_NOT_READY;
    }
    return status;
}

static int32_t SetupCrcDma(uint8_t dmaChannel)
{
    ADI_CRC_RESULT status = ADI_CRC_RESULT_SUCCESS;
    IRQn_Type dmaChannelIRQn;

    dmaChannelIRQn = MaxGetDMAIRQn(dmaChannel);
    if (dmaChannelIRQn != MXC_IRQ_EXT_COUNT)
    {
        NVIC_SetPriority(dmaChannelIRQn, ADI_CRC_CFG_DMA_INT_PRIORITY);
        NVIC_SetVector(dmaChannelIRQn, (uint32_t)CrcDmaIrqHandler);
        NVIC_EnableIRQ(dmaChannelIRQn);
    }
    else
    {
        /* Acquired DMA Channel is not in the range of 0 to 8*/
        status = ADI_CRC_RESULT_INIT_FAILURE;
    }
    return status;
}

static void CrcDmaIrqHandler(void)
{

    /* 0x50 is the mask for timeout and Bus Error bits */
    if (MXC_DMA->ch[crcHw.dmaChannel].status & 0x50)
    {
        crcHw.dmaError = true;
    }
    /* 0x5C is the mask for status register */
    MXC_DMA->ch[crcHw.dmaChannel].status |= 0x5C;
    CrcDmaCallBack(crcHw.dmaChannel, 0);
}

void EvbResetHw(void)
{

    MXC_CRC->ctrl &= ~MXC_F_CRC_CTRL_EN;
    /* 0x5C is the mask for status register */
    MXC_DMA->ch[crcHw.dmaChannel].status |= 0x5C;
}

void EvbCrcClearInterrupt(void)
{
    MXC_CRC->ctrl &= ~MXC_F_CRC_CTRL_EN;
    /* 0x5C is the mask for status register */
    MXC_DMA->ch[crcHw.dmaChannel].status |= 0x5C;
}

static ADI_CRC_RESULT ConfigCrcDma(mxc_crc_reva_req_t *pReq)
{
    ADI_CRC_RESULT status = ADI_CRC_RESULT_SUCCESS;
    mxc_dma_config_t config;
    mxc_dma_srcdst_t srcdst;

    if ((pReq == NULL) || (pReq->dataBuffer == NULL))
    {
        status = ADI_CRC_RESULT_NULL_PTR;
    }

    if (status == ADI_CRC_RESULT_SUCCESS)
    {
        if (pReq->dataLen == 0)
        {
            status = ADI_CRC_RESULT_INIT_FAILURE;
        }
    }

    if (status == ADI_CRC_RESULT_SUCCESS)
    {

        config.reqsel = MXC_DMA_REQUEST_CRCTX;

        config.ch = crcHw.dmaChannel;

        config.srcwd = MXC_DMA_WIDTH_BYTE;
        config.dstwd = MXC_DMA_WIDTH_BYTE;

        config.srcinc_en = 1;
        config.dstinc_en = 0;

        srcdst.ch = crcHw.dmaChannel;
        srcdst.source = (uint8_t *)pReq->dataBuffer; // transfering bytes
        srcdst.len = (int)(pReq->dataLen * 4);       // number of bytes
        srcdst.dest = NULL;

        MXC_CRC->ctrl |= MXC_F_CRC_CTRL_DMA_EN;
        MXC_CRC->ctrl |= MXC_F_CRC_CTRL_EN;

        MXC_DMA_ConfigChannel(config, srcdst);
        MXC_DMA_SetCallback(crcHw.dmaChannel, NULL);
        MXC_DMA_EnableInt(crcHw.dmaChannel);
    }
    return status;
}

static void CrcDmaCallBack(int ch, int error)
{
    ADI_CRC_DATA *pCrcData = (ADI_CRC_DATA *)crcHw.hCrc;
    int32_t waitCount = 0;
    int32_t timeOutCount = ADI_CRC_CFG_TIMEOUT_COUNT;
    while (((MXC_CRC->ctrl >> MXC_F_CRC_CTRL_BUSY_POS) & 1) && (waitCount < timeOutCount))
    {
        // Wait for the crc computation to complete
        waitCount++;
    }
    if ((error == E_NO_ERROR) && (ch == crcHw.dmaChannel) && (waitCount < timeOutCount))
    {
        pCrcData->crcValue = MXC_CRC->val;
    }
    MXC_CRC->ctrl &= ~MXC_F_CRC_CTRL_EN;
    crcHw.isComplete = true;
    if (pCrcData->crcCfg.pfCallback != NULL)
    {
        pCrcData->crcCfg.pfCallback(pCrcData->crcCfg.pCBData);
    }
}

static ADI_CRC_RESULT FormatSeed(ADI_CRC_CONFIG *pCrcConfig)
{

    ADI_CRC_RESULT status = ADI_CRC_RESULT_SUCCESS;

    // Format the initial value.
    if ((pCrcConfig->crcType == ADI_CRC_TYPE_CRC8) && (pCrcConfig->bigEndian == true))
    {
        pCrcConfig->seed = (uint32_t)(pCrcConfig->seed << 24);
    }
    else if ((pCrcConfig->crcType == ADI_CRC_TYPE_CRC16) && (pCrcConfig->bigEndian == true))
    {
        pCrcConfig->seed = (uint32_t)(pCrcConfig->seed << 16);
    }
    else if ((pCrcConfig->crcType == ADI_CRC_TYPE_CRC32) && (pCrcConfig->bigEndian == true))
    {
        // No change to initial value
        pCrcConfig->seed = (uint32_t)(pCrcConfig->seed);
    }
    else
    {
        // Other case needs to considered in future.
        status = ADI_CRC_RESULT_FAILURE;
    }

    return status;
}

static ADI_CRC_RESULT FormatResult(volatile ADI_CRC_CONFIG *pCrcConfig, volatile uint32_t *pData)
{
    ADI_CRC_RESULT status = ADI_CRC_RESULT_SUCCESS;

    // Format the CRC result.
    if ((pCrcConfig->crcType == ADI_CRC_TYPE_CRC8) && (pCrcConfig->bigEndian == true))
    {
        *pData = (uint32_t)(*pData >> 24);
    }
    else if ((pCrcConfig->crcType == ADI_CRC_TYPE_CRC16) && (pCrcConfig->bigEndian == true))
    {
        *pData = (uint32_t)(*pData >> 16);
    }
    else if ((pCrcConfig->crcType == ADI_CRC_TYPE_CRC32) && (pCrcConfig->bigEndian == true))
    {
        // No change to initial value
        *pData = (uint32_t)*pData;
    }
    else
    {
        // Other case needs to considered in future.
        status = ADI_CRC_RESULT_FAILURE;
    }

    return status;
}

/**
 * @}
 */
