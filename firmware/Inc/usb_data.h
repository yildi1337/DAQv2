/*
 *******************************************************************************
 * File:            usb_data.h
 * Date:            2020-11-13
 * Author:          Phillip Durdaut
 * Summary:         Functions for communicating high-speed data over the USB
 *                  interface
 * Notes:           -
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DATA_H
#define __USB_DATA_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbd_def.h"

/* Public defines ------------------------------------------------------------*/
#define USB_DATA_RXFIFO_ALL_SIZE_BYTES              1024
#define USB_DATA_TXFIFO_ENDPOINT_0_SIZE_BYTES       1024
#define USB_DATA_TXFIFO_ENDPOINT_1_SIZE_BYTES       2048

/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
typedef enum
{
	USB_DATA_COMMAND_NONE,
	USB_DATA_COMMAND_MEASUREMENT_START,
	USB_DATA_COMMAND_MEASUREMENT_STOP,
	USB_DATA_COMMAND_GET_SAMPLES
} USB_DATA_COMMAND_t;

typedef float USB_DATA_PARAMETER_t;

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
extern void USB_DATA_Init(void);
extern PCD_HandleTypeDef * USB_DATA_GetHandlePCD(void);
extern USBD_HandleTypeDef * USB_DATA_GetHandleUSBD(void);
extern void USB_DATA_RxEndpoint1(uint8_t * pBuffer, uint8_t numberOfBytes);
extern void USB_DATA_TxEndpoint1(uint8_t * pBuffer, uint32_t numberOfBytes);

#endif /* __USB_DATA_H */
