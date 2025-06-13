/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     message.c
 * @brief    This file contains API's to print different types of messages to
 * user
 * @{
 */

/*=============  I N C L U D E S   =============*/
#include "message.h"
#include "adi_evb.h"
#include <stdio.h>
#include <string.h>

/*=============  D A T A  =============*/

/** Buffer to store message string */
SECTION(DMA_BUFFER) static char msgString[MAX_MSG_SIZE];
/** Buffer to store message string */
SECTION(DMA_BUFFER) static char msgStringToCopy[MAX_MSG_SIZE];
/** Buffer to write and store the message */
SECTION(DMA_BUFFER) static uint8_t messageBuffer0[MAX_MSG_STORAGE_SIZE];
/** Buffer to write and store the message */
SECTION(DMA_BUFFER) static uint8_t messageBuffer1[MAX_MSG_STORAGE_SIZE];
/** Buffer to write message */
static uint8_t *pBufferToWrite = &messageBuffer0[0];
/** message bytes stored */
static uint32_t msgBytesStored = 0;
/** dummy data to be sent for the HostUartTxAsync API. As this API checks whether the first argument
 * of API is NULL or not. */
static int32_t dummyUartData = 0;

/*============= F U N C T I O N  P R O T O T Y P E S =============*/

/**
 * @details CopyToBuffer
 * @param[in] pMessage - pointer to pMessage.
 * @return[in] status - 0 on Success
 *                    - 1 on Failure
 */
static int32_t CopyToBuffer(char *pMessage);

/*=============  C O D E  =============*/

/**
 *@brief Function Definition section
 */

void EvbInitMessageBuffer(void)
{
    msgBytesStored = 0;
}

/**
 * @details Prints message
 */
// added this to avoid warning format sring is not a literal.
__attribute__((__format__(__printf__, 2, 0))) int32_t PrintMessage(char *pMsgType, char *pFormat,
                                                                   ...)
{
    int32_t status = 0;
    int32_t ret = 0;
    va_list pArgs;
    va_start(pArgs, pFormat);
    vsnprintf(msgString, MAX_MSG_SIZE, pFormat, pArgs);

    if ((strcmp(pMsgType, "RAW") == 0) || (strcmp(pMsgType, "DBGRAW") == 0))
    {
        va_end(pArgs);
        /* Just print the message as such*/
        status = CopyToBuffer(msgString);
    }
    else
    {
        ret = snprintf(msgStringToCopy, MAX_MSG_SIZE, "%s\n\r", msgString);
        va_end(pArgs);
        if (ret >= 0)
        {

            CopyToBuffer(pMsgType);
            status = CopyToBuffer(msgStringToCopy);
        }
        else
        {
            status = 1;
        }
    }

    return status;
}

/**
 * @details Copy pMessage into message buffer
 * @param[in] pMessage - pointer to pMessage buffer.
 * @return[in] status - 0 on Success, 1 on Failure
 */
int32_t CopyToBuffer(char *pMessage)
{
    int32_t space = (int32_t)(MAX_MSG_STORAGE_SIZE - msgBytesStored - 1);
    int32_t bytesToWrite = (int32_t)strlen(pMessage);
    int32_t status = 0;

    if (space >= bytesToWrite)
    {
        memcpy(&pBufferToWrite[msgBytesStored], pMessage, (size_t)bytesToWrite);
        msgBytesStored += (uint32_t)bytesToWrite;
    }
    else
    {
        status = 1;
    }

    return status;
}

int32_t EvbFlushMessages(void)
{
    int32_t status = 0;
    static int32_t bufId = 0;
    int32_t isTxComplete;

    isTxComplete = EvbGetTxStatus();
    /* Do not flush until previous transfer is complete*/
    if ((msgBytesStored > 0) && (isTxComplete == true))
    {
        EvbStartHostUartTxAsync(&dummyUartData, pBufferToWrite, msgBytesStored);
        if (bufId == 0)
        {
            pBufferToWrite = &messageBuffer1[0];
        }
        else
        {
            pBufferToWrite = &messageBuffer0[0];
        }
        // Toggle the bufID
        bufId ^= 0x1;
        msgBytesStored = 0;
    }
    isTxComplete = EvbGetTxStatus();
    if ((isTxComplete == false) || (msgBytesStored > 0))
    {
        status = 1;
    }
    return status;
}

uint32_t EvbGetFreeMessageSpace(void)
{
    uint32_t freeSpace;
    freeSpace = MAX_MSG_STORAGE_SIZE - msgBytesStored;
    return freeSpace;
}

/**
 * @}
 */
