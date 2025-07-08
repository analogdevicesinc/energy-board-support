/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 *  @file   message.h
 * @addtogroup ADI_CLI
 *  @brief  Defines for message
 * @{
 */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

/*============= I N C L U D E S =============*/
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============= D E F I N E S =============*/
/** Maximum message size value */
#define MAX_MSG_SIZE 512
/* FIXME : Maximum buffer size can be reduced based on FlushMessages*/
/** Maximum stored message size value */
#define MAX_MSG_STORAGE_SIZE 1024 * 10
/** Prints Info  Message as such without new line */
#define printf_raw(...) PrintMessage("RAW", __VA_ARGS__);
/** Prints Info  Message */
#define printf(...) PrintMessage("", __VA_ARGS__);
/** Prints warn message */
#define printf_warn(...) PrintMessage("Warn : ", __VA_ARGS__);
/** Prints error message */
#define printf_error(...) PrintMessage("Error : ", __VA_ARGS__);
/** Prints debug message */
#ifdef ENABLE_DEBUG
#define printf_debug(...) PrintMessage("Debug : ", __VA_ARGS__);
#else
#define printf_debug(...)
#endif
/** Prints debug message as such without new line */
#ifdef ENABLE_DEBUG
#define printf_debug_raw(...) PrintMessage("DBGRAW", __VA_ARGS__);
#else
#define printf_debug_raw(...) NULL
#endif

/*============= F U N C T I O N  P R O T O T Y P E S =============*/

/**
 * @details Prints message
 * @param[in] pMsgType - Type of message to be printed
 * @param[in] pFormat  - Format specifier for the message
 * @return status      - SUCCESS - 0
 *                     - FAILURE - 1
 */
int32_t PrintMessage(char *pMsgType, char *pFormat, ...);

/**
 * @details FlushMessages
 * @return success when messages are flushed.
 */
int32_t EvbFlushMessages(void);
/**
 * @details Initializes message buffer
 */
void EvbInitMessageBuffer(void);
/** Gets free space in the message buffer*/
uint32_t EvbGetFreeMessageSpace(void);

#ifdef __cplusplus
}
#endif

#endif /* __MESSAGE_H__ */

/**
 * @}
 */
