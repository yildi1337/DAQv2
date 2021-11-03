/*
 *******************************************************************************
 * File:            uart.h
 * Date:            2020-11-03
 * Author:          Phillip Durdaut
 * Summary:         Functions for UART communication handling
 * Notes:           -
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H
#define __UART_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Public defines ------------------------------------------------------------*/
#define UART_UART1_BUFFER_SIZE_BYTES    256

/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
typedef enum
{
	UART_1,
} UART_t;

typedef struct {
	uint8_t command[UART_UART1_BUFFER_SIZE_BYTES];
	uint8_t length;
	bool available;
	bool acknowledged;
} UART_UART1_RECEIVED_COMMAND_t;

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
extern void UART_Init(UART_t uart);
extern UART_HandleTypeDef * UART_GetHandle(UART_t uart);
extern void UART_EnableRXInterrupt(UART_t uart);
extern void UART_DisableRXInterrupt(UART_t uart);
extern void UART_TransmitData(UART_t uart, uint8_t * pBuffer, uint32_t numberOfBytes);
extern void UART_ReceiveCommand(UART_t uart);
extern UART_UART1_RECEIVED_COMMAND_t * GetReferenceOnReceivedCommandStruct(UART_t uart);

#endif /* __UART_H */
