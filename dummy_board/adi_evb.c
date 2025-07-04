/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     adi_evb.c
 * @brief    Template file for ADC Service board dependencies.
 * @{
 */

/*=============  I N C L U D E S   =============*/
#include "adi_evb.h"
#include <stdint.h>

/*=============  C O D E  =============*/

int32_t EvbInit(void **phEvb, ADI_EVB_CONFIG *pConfig)
{
    /*
        Initialize SPI connected to ADC in mode 3
        Setup ISR to  pConfig->spiConfig.pfAdeSpiRxCallback on SPI RX completion

        Initialize GPIO connected to DREADY pin of ADC as input
        Enable interrupt for GPIO on falling edge
        Call pConfig->gpioConfig.pfGpioCallback from the ISR
    */
    (void)phEvb;   /* Dummy use of argument */
    (void)pConfig; /* Dummy use of argument */

    return 0;
}

int32_t EvbResetAdcs(void)
{
    /* Toggle pin connected to the reset pin of ADC*/
    return 0;
}

int32_t EvbAdeSpiTransmitAsync(void *hEvb, uint8_t *pData, uint32_t numBytes)
{
    (void)hEvb;     /* Dummy use of argument */
    (void)pData;    /* Dummy use of argument */
    (void)numBytes; /* Dummy use of argument */

    return 0;
}

int32_t EvbResetAde(void)
{
    /* Toggle pin connected to the reset pin of ADC and ADE9178*/
    return 0;
}

int32_t EvbAdeSpiReceiveAsync(void *hEvb, uint8_t *pData, uint32_t numBytes)
{
    (void)hEvb;     /* Dummy use of argument */
    (void)pData;    /* Dummy use of argument */
    (void)numBytes; /* Dummy use of argument */

    return 0;
}

int32_t EvbAdeSpiTransceive(void *hEvb, uint8_t *pTxData, uint8_t *pRxData, uint32_t numBytes,
                            uint32_t timeoutCount)
{
    (void)hEvb;         /* Dummy use of argument */
    (void)pTxData;      /* Dummy use of argument */
    (void)pRxData;      /* Dummy use of argument */
    (void)numBytes;     /* Dummy use of argument */
    (void)timeoutCount; /* Dummy use of argument */

    return 0;
}

int32_t EvbLedOn(uint32_t idx)
{
    (void)idx; /* Dummy use of argument */

    /* For error indication*/
    return 0;
}

int32_t EvbDelayMs(uint32_t delayMs)
{
    (void)delayMs; /* Dummy use of argument */

    return 0;
}

int32_t EvbHostUartTransmitAsync(void *hEvb, uint8_t *pBuffer, uint32_t numBytes)
{
    /* Not needed for ADC service.
    Only to transfer data to host*/
    (void)hEvb;     /* Dummy use of argument */
    (void)pBuffer;  /* Dummy use of argument */
    (void)numBytes; /* Dummy use of argument */

    return 0;
}

void EvbEnableAllGPIOIrq()
{
    /* Enable all GPIO interrupts */
    // This function should enable all GPIO interrupts as per the board design.
    // Implementation depends on the specific hardware and GPIO library used.
}

uint32_t EvbGetTime()
{
    return 0;
}

int32_t EvbGetPinState(uint32_t port, uint32_t flag)
{
    (void)port; /* Dummy use of argument */
    (void)flag; /* Dummy use of argument */

    return 0;
}

int32_t EvbAdeWfsUartSetBaudrate(void *hEvb, uint32_t baudRate)
{
    (void)hEvb;     /* Dummy use of argument */
    (void)baudRate; /* Dummy use of argument */
    return 0;
}
int32_t EvbAdeWfsUartReceiveAsync(void *hEvb, uint8_t *pBuffer, uint32_t numBytes)
{
    (void)hEvb;     /* Dummy use of argument */
    (void)pBuffer;  /* Dummy use of argument */
    (void)numBytes; /* Dummy use of argument */
    return 0;
}

int32_t EvbAdeSpiTransceiveAsync(void *hEvb, uint8_t *pTxData, uint8_t *pRxData, uint32_t numBytes)
{
    (void)hEvb;     /* Dummy use of argument */
    (void)pTxData;  /* Dummy use of argument */
    (void)pRxData;  /* Dummy use of argument */
    (void)numBytes; /* Dummy use of argument */

    return 0;
}

int32_t EvbEnableDreadyIrq(uint8_t enable)
{
    (void)enable; /* Dummy use of argument */

    return 0;
}

/**
 * @}
 */
