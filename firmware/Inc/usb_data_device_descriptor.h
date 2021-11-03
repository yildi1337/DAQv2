/*
 *******************************************************************************
 * File:            usb_data_device_descriptor.h
 * Date:            2020-11-14
 * Author:          Phillip Durdaut
 * Summary:         Device descriptor for USB data (HS) communication
 * Notes:           -
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DATA_DEVICE_DESCRIPTOR_H
#define __USB_DATA_DEVICE_DESCRIPTOR_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbd_def.h"

/* Public defines ------------------------------------------------------------*/
/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
extern USBD_DescriptorsTypeDef * USB_DATA_DEVICE_DESCRIPTOR_GetHandleUSBD_Descriptors(void);

#endif /* __USB_DATA_DEVICE_DESCRIPTOR_H */
