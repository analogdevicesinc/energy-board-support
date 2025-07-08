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

/** Evb crc info */
typedef struct
{
    /** callback */
    ADI_EVB_CALLBACK pfCallback;
    /** lock crc */
    uint8_t lock;
    /** dma channel reserved for CRC */
    uint8_t dmaChannel;
    /** dma error flag */
    uint8_t dmaError;
    /** variable to check status of CRC transaction*/
    volatile bool isComplete;
    /** CRC output */
    volatile uint32_t crc;
    /** crc request struct */
    mxc_crc_req_t crcReq;
    /** Seed value */
    uint32_t seed;
    /** big endian flag */
    bool isBigEndian;
    /** CRC type */
    uint8_t crcType;
    /** isReversed flag */
    uint8_t isReversed;
} EVB_CRC_INFO;

/*=============  D A T A  =============*/

static EVB_CRC_INFO evbCrcInfo;

/*============= F U N C T I O N S =============*/

static int32_t ConfigCrcDma(mxc_crc_reva_req_t *pReq);

static int32_t SetupCrcDma(uint8_t dmaChannel);

static void CrcDmaIrqHandler(void);

static void CrcDmaCallBack(int ch, int error);

static int32_t FormatSeed(uint8_t crcType, uint8_t isBigEndian, uint32_t seed);

static int32_t FormatResult(uint8_t crcType, uint8_t isBigEndian, volatile uint32_t *pData);

/**
 * Enums of CRC type.
 */
typedef enum
{
    /** CRC-8. */
    ADI_CRC_TYPE_CRC8 = 0u,
    /** CRC-16. */
    ADI_CRC_TYPE_CRC16,
    /** CRC-32. */
    ADI_CRC_TYPE_CRC32,

} ADI_CRC_TYPE;

/*=============  C O D E  =============*/

int32_t EvbInitCrc(void **phCrc, ADI_EVB_CRC_CONFIG *pConfig)
{
    int32_t status = 0;
    uint32_t crcBuff = 0x00;
    evbCrcInfo.isBigEndian = 1;
    uint32_t polynomial = 0x1021;
    evbCrcInfo.crcType = ADI_CRC_TYPE_CRC16;
    evbCrcInfo.seed = 0xFFFF;
    evbCrcInfo.dmaChannel = (uint8_t)MXC_DMA_AcquireChannel();
    status = SetupCrcDma(evbCrcInfo.dmaChannel);
    MXC_CRC_Init();
    status = FormatSeed(evbCrcInfo.crcType, evbCrcInfo.isBigEndian, evbCrcInfo.seed);
    evbCrcInfo.pfCallback = pConfig->pfCallback;
    if (status == 0)
    {
        MXC_CRC->ctrl |= (uint32_t)((evbCrcInfo.isReversed << 3) | (evbCrcInfo.isBigEndian << 2));
        // Load CRC polynomial into crc polynomial register
        MXC_CRC_SetPoly(polynomial << 16);
        evbCrcInfo.crcReq.dataBuffer = &crcBuff;
        evbCrcInfo.crcReq.dataLen = 1;
        // Initialises the DMA and CRC. Refer section 15.5 of MAX32670 user
        // guide.
        status = ConfigCrcDma((mxc_crc_reva_req_t *)&evbCrcInfo.crcReq);
    }
    else
    {
        status = -1;
    }

    return status;
}

int32_t EvbCrcCalculate(void *hEvb, uint8_t *pData, uint32_t numBytes)
{
    (void)hEvb; /* Dummy use of argument */
    evbCrcInfo.isComplete = false;
    /* 0x5C is the mask for status register */
    MXC_DMA->ch[evbCrcInfo.dmaChannel].status |= 0x5C;
    MXC_CRC->ctrl |= MXC_F_CRC_CTRL_EN;
    MXC_CRC->val = evbCrcInfo.seed;
    /* Set start address and initiate DMA*/
    MXC_DMA->ch[evbCrcInfo.dmaChannel].src = (unsigned int)pData;
    MXC_DMA->ch[evbCrcInfo.dmaChannel].cnt = numBytes;
    MXC_DMA->ch[evbCrcInfo.dmaChannel].ctrl |= (MXC_F_DMA_CTRL_EN | MXC_F_DMA_CTRL_CTZ_IE);
    return 0;
}

int32_t EvbCrcGetValue(void *hEvb, uint32_t *pData)
{
    int32_t status = 0;
    (void)hEvb; /* Dummy use of argument */
    if (evbCrcInfo.dmaError == 1)
    {
        status = -1;
        evbCrcInfo.dmaError = 0;
    }
    else if (evbCrcInfo.isComplete == true)
    {
        *pData = evbCrcInfo.crc;
        status = FormatResult(evbCrcInfo.crcType, evbCrcInfo.isBigEndian, pData);
    }
    else
    {
        *pData = 0xFFFFFFFF;
        status = -1;
    }
    return status;
}

static int32_t SetupCrcDma(uint8_t dmaChannel)
{
    int32_t status = 0;
    IRQn_Type dmaChannelIRQn;

    dmaChannelIRQn = MaxGetDMAIRQn(dmaChannel);
    if (dmaChannelIRQn != MXC_IRQ_EXT_COUNT)
    {
        NVIC_SetPriority(dmaChannelIRQn, APP_CFG_CRC_DMA_INT_PRIORITY);
        NVIC_SetVector(dmaChannelIRQn, (uint32_t)CrcDmaIrqHandler);
        NVIC_EnableIRQ(dmaChannelIRQn);
    }
    else
    {
        /* Acquired DMA Channel is not in the range of 0 to 8*/
        status = -1;
    }
    return status;
}

static void CrcDmaIrqHandler(void)
{

    /* 0x50 is the mask for timeout and Bus Error bits */
    if (MXC_DMA->ch[evbCrcInfo.dmaChannel].status & 0x50)
    {
        evbCrcInfo.dmaError = true;
    }
    /* 0x5C is the mask for status register */
    MXC_DMA->ch[evbCrcInfo.dmaChannel].status |= 0x5C;
    CrcDmaCallBack(evbCrcInfo.dmaChannel, 0);
}

void EvbCrcReset(void)
{

    MXC_CRC->ctrl &= ~MXC_F_CRC_CTRL_EN;
    /* 0x5C is the mask for status register */
    MXC_DMA->ch[evbCrcInfo.dmaChannel].status |= 0x5C;
}

void EvbCrcClearInterrupt(void)
{
    MXC_CRC->ctrl &= ~MXC_F_CRC_CTRL_EN;
    /* 0x5C is the mask for status register */
    MXC_DMA->ch[evbCrcInfo.dmaChannel].status |= 0x5C;
}

static int32_t ConfigCrcDma(mxc_crc_reva_req_t *pReq)
{
    int32_t status = 0;
    mxc_dma_config_t config;
    mxc_dma_srcdst_t srcdst;

    config.reqsel = MXC_DMA_REQUEST_CRCTX;

    config.ch = evbCrcInfo.dmaChannel;

    config.srcwd = MXC_DMA_WIDTH_BYTE;
    config.dstwd = MXC_DMA_WIDTH_BYTE;

    config.srcinc_en = 1;
    config.dstinc_en = 0;

    srcdst.ch = evbCrcInfo.dmaChannel;
    srcdst.source = (uint8_t *)pReq->dataBuffer; // transfering bytes
    srcdst.len = (int)(pReq->dataLen * 4);       // number of bytes
    srcdst.dest = NULL;

    MXC_CRC->ctrl |= MXC_F_CRC_CTRL_DMA_EN;
    MXC_CRC->ctrl |= MXC_F_CRC_CTRL_EN;

    MXC_DMA_ConfigChannel(config, srcdst);
    MXC_DMA_SetCallback(evbCrcInfo.dmaChannel, NULL);
    MXC_DMA_EnableInt(evbCrcInfo.dmaChannel);
    return status;
}

static void CrcDmaCallBack(int ch, int error)
{
    int32_t waitCount = 0;
    int32_t timeOutCount = APP_CFG_CRC_TIMEOUT_COUNT;
    while (((MXC_CRC->ctrl >> MXC_F_CRC_CTRL_BUSY_POS) & 1) && (waitCount < timeOutCount))
    {
        // Wait for the crc computation to complete
        waitCount++;
    }
    if ((error == E_NO_ERROR) && (ch == evbCrcInfo.dmaChannel) && (waitCount < timeOutCount))
    {
        evbCrcInfo.crc = MXC_CRC->val;
    }
    MXC_CRC->ctrl &= ~MXC_F_CRC_CTRL_EN;
    evbCrcInfo.isComplete = true;
    if (evbCrcInfo.pfCallback != NULL)
    {
        evbCrcInfo.pfCallback();
    }
}

static int32_t FormatSeed(uint8_t crcType, uint8_t isBigEndian, uint32_t seed)
{

    int32_t status = 0;

    // Format the initial value.
    if ((crcType == ADI_CRC_TYPE_CRC8) && (isBigEndian == true))
    {
        evbCrcInfo.seed = (uint32_t)(seed << 24);
    }
    else if ((crcType == ADI_CRC_TYPE_CRC16) && (isBigEndian == true))
    {
        evbCrcInfo.seed = (uint32_t)(seed << 16);
    }
    else if ((crcType == ADI_CRC_TYPE_CRC32) && (isBigEndian == true))
    {
        // No change to initial value
        evbCrcInfo.seed = (uint32_t)(seed);
    }
    else
    {
        // Other case needs to considered in future.
        status = -1;
    }

    return status;
}

static int32_t FormatResult(uint8_t crcType, uint8_t isBigEndian, volatile uint32_t *pData)
{
    int32_t status = 0;

    // Format the CRC result.
    if ((crcType == ADI_CRC_TYPE_CRC8) && (isBigEndian == true))
    {
        *pData = (uint32_t)(*pData >> 24);
    }
    else if ((crcType == ADI_CRC_TYPE_CRC16) && (isBigEndian == true))
    {
        *pData = (uint32_t)(*pData >> 16);
    }
    else if ((crcType == ADI_CRC_TYPE_CRC32) && (isBigEndian == true))
    {
        // No change to initial value
        *pData = (uint32_t)*pData;
    }
    else
    {
        // Other case needs to considered in future.
        status = -1;
    }

    return status;
}

/**
 * @}
 */
