/*
 *******************************************************************************
 * File:			usb_class.h
 * Date:			2020-11-14
 * Author:			Phillip Durdaut
 * Summary:			USB custom class driver
 * Notes:			-
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CLASS_H
#define __USB_CLASS_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbd_ioreq.h"

/* Public defines ------------------------------------------------------------*/
/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/
extern USBD_ClassTypeDef * USB_CLASS_GetHandleClass_USBD(void);
extern uint8_t USB_CLASS_Transmit(USBD_HandleTypeDef * pdev, uint8_t * buf, uint16_t length, uint8_t epnum);

#endif /* __USB_CLASS_H */
