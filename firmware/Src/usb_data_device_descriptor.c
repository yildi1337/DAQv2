/*
 *******************************************************************************
 * File:			usb_data_device_descriptor.c
 * Date:			2020-11-14
 * Author:			Phillip Durdaut
 * Summary:			Device descriptor for USB data (HS) communication
 * Notes:			-
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_data_device_descriptor.h"
#include "usbd_core.h"
#include "usbd_conf.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define USB_DATA_DEVICE_DESCRIPTOR_VENDOR_ID					MAIN_USB_VENDOR_ID
#define USB_DATA_DEVICE_DESCRIPTOR_PRODUCT_ID					MAIN_USB_PRODUCT_ID
#define USB_DATA_DEVICE_DESCRIPTOR_LANG_ID_STRING            	MAIN_USB_LANG_ID_STRING
#define USB_DATA_DEVICE_DESCRIPTOR_MANUFACTURER_STRING      	MAIN_USB_MANUFACTURER_STRING
#define USB_DATA_DEVICE_DESCRIPTOR_PRODUCT_STRING        		MAIN_USB_PRODUCT_STRING
#define USB_DATA_DEVICE_DESCRIPTOR_SERIAL_STRING     			MAIN_USB_SERIAL_STRING
#define USB_DATA_DEVICE_DESCRIPTOR_CONFIGURATION_STRING  		MAIN_USB_CONFIGURATION_STRING
#define USB_DATA_DEVICE_DESCRIPTOR_INTERFACE_STRING      		MAIN_USB_INTERFACE_STRING
#define USB_DATA_DEVICE_DESCRIPTOR_DEVICE_VERSION				MAIN_USB_DEVICE_VERSION
#define USB_DATA_DEVICE_DESCRIPTOR_MAX_STRING_DESCRIPTOR_SIZE	0x100

/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
uint8_t * USB_DATA_DEVICE_DESCRIPTOR_GetDeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t * length);
uint8_t * USB_DATA_DEVICE_DESCRIPTOR_GetLangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t * length);
uint8_t * USB_DATA_DEVICE_DESCRIPTOR_GetManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t * length);
uint8_t * USB_DATA_DEVICE_DESCRIPTOR_GetProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t * length);
uint8_t * USB_DATA_DEVICE_DESCRIPTOR_GetSerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t * length);
uint8_t * USB_DATA_DEVICE_DESCRIPTOR_GetConfigurationStrDescriptor(USBD_SpeedTypeDef speed, uint16_t * length);
uint8_t * USB_DATA_DEVICE_DESCRIPTOR_GetInterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t * length);

/* Private variables ---------------------------------------------------------*/
USBD_DescriptorsTypeDef USB_DATA_DEVICE_DESCRIPTOR_HandleUSBD_Descriptors =
{
	USB_DATA_DEVICE_DESCRIPTOR_GetDeviceDescriptor,
	USB_DATA_DEVICE_DESCRIPTOR_GetLangIDStrDescriptor,
	USB_DATA_DEVICE_DESCRIPTOR_GetManufacturerStrDescriptor,
	USB_DATA_DEVICE_DESCRIPTOR_GetProductStrDescriptor,
	USB_DATA_DEVICE_DESCRIPTOR_GetSerialStrDescriptor,
	USB_DATA_DEVICE_DESCRIPTOR_GetConfigurationStrDescriptor,
	USB_DATA_DEVICE_DESCRIPTOR_GetInterfaceStrDescriptor,
};

__ALIGN_BEGIN uint8_t USB_DATA_DEVICE_DESCRIPTOR_DeviceDescriptor[USB_LEN_DEV_DESC] __ALIGN_END =
{
	0x12,														/* bLength (18 bytes) */
	USB_DESC_TYPE_DEVICE,										/* bDescriptorType (1 - device descriptor) */
	LOBYTE(0x0200),												/* bcdUSB */
	HIBYTE(0x0200),												/* bcdUSB (USB 2.0) */
	0xff,                       								/* bDeviceClass (vendor specific) */
	0xff,                       								/* bDeviceSubClass */
	0xff,                       								/* bDeviceProtocol (vendor specific) */
	USB_MAX_EP0_SIZE,           								/* bMaxPacketSize (64 bytes) */
	LOBYTE(USB_DATA_DEVICE_DESCRIPTOR_VENDOR_ID),           	/* idVendor */
	HIBYTE(USB_DATA_DEVICE_DESCRIPTOR_VENDOR_ID),           	/* idVendor */
	LOBYTE(USB_DATA_DEVICE_DESCRIPTOR_PRODUCT_ID),           	/* idProduct */
	HIBYTE(USB_DATA_DEVICE_DESCRIPTOR_PRODUCT_ID),           	/* idProduct */
	LOBYTE(USB_DATA_DEVICE_DESCRIPTOR_DEVICE_VERSION),			/* bcdDevice */
	HIBYTE(USB_DATA_DEVICE_DESCRIPTOR_DEVICE_VERSION),			/* bcdDevice */
	USBD_IDX_MFC_STR,											/* iManufacturer (1) */
	USBD_IDX_PRODUCT_STR,										/* iProduct (2) */
	USBD_IDX_SERIAL_STR,										/* iSerialNumber (3) */
	USBD_MAX_NUM_CONFIGURATION									/* bNumConfigurations (1) */
};

__ALIGN_BEGIN uint8_t USB_DATA_DEVICE_DESCRIPTOR_LangIDStrDescriptor[USB_LEN_LANGID_STR_DESC] __ALIGN_END =
{
	USB_LEN_LANGID_STR_DESC,
	USB_DESC_TYPE_STRING,
	LOBYTE(USB_DATA_DEVICE_DESCRIPTOR_LANG_ID_STRING),
	HIBYTE(USB_DATA_DEVICE_DESCRIPTOR_LANG_ID_STRING),
};

__ALIGN_BEGIN uint8_t USB_DATA_DEVICE_DESCRIPTOR_ManufacturerStrDescriptor[USB_DATA_DEVICE_DESCRIPTOR_MAX_STRING_DESCRIPTOR_SIZE] __ALIGN_END;
__ALIGN_BEGIN uint8_t USB_DATA_DEVICE_DESCRIPTOR_ProductStrDescriptor[USB_DATA_DEVICE_DESCRIPTOR_MAX_STRING_DESCRIPTOR_SIZE] __ALIGN_END;
__ALIGN_BEGIN uint8_t USB_DATA_DEVICE_DESCRIPTOR_SerialStrDescriptor[USB_DATA_DEVICE_DESCRIPTOR_MAX_STRING_DESCRIPTOR_SIZE] __ALIGN_END;
__ALIGN_BEGIN uint8_t USB_DATA_DEVICE_DESCRIPTOR_ConfigurationStrDescriptor[USB_DATA_DEVICE_DESCRIPTOR_MAX_STRING_DESCRIPTOR_SIZE] __ALIGN_END;
__ALIGN_BEGIN uint8_t USB_DATA_DEVICE_DESCRIPTOR_InterfaceStrDescriptor[USB_DATA_DEVICE_DESCRIPTOR_MAX_STRING_DESCRIPTOR_SIZE] __ALIGN_END;

/* Program code --------------------------------------------------------------*/

extern USBD_DescriptorsTypeDef * USB_DATA_DEVICE_DESCRIPTOR_GetHandleUSBD_Descriptors(void)
{
	return &USB_DATA_DEVICE_DESCRIPTOR_HandleUSBD_Descriptors;
}

uint8_t * USB_DATA_DEVICE_DESCRIPTOR_GetDeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t * length)
{
	*length = sizeof(USB_DATA_DEVICE_DESCRIPTOR_DeviceDescriptor);
	return (uint8_t *)USB_DATA_DEVICE_DESCRIPTOR_DeviceDescriptor;
}

uint8_t * USB_DATA_DEVICE_DESCRIPTOR_GetLangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t * length)
{
	*length = sizeof(USB_DATA_DEVICE_DESCRIPTOR_LangIDStrDescriptor);
	return (uint8_t *)USB_DATA_DEVICE_DESCRIPTOR_LangIDStrDescriptor;
}

uint8_t * USB_DATA_DEVICE_DESCRIPTOR_GetManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t * length)
{
	USBD_GetString((uint8_t *)USB_DATA_DEVICE_DESCRIPTOR_MANUFACTURER_STRING, USB_DATA_DEVICE_DESCRIPTOR_ManufacturerStrDescriptor, length);
	return USB_DATA_DEVICE_DESCRIPTOR_ManufacturerStrDescriptor;
}

uint8_t * USB_DATA_DEVICE_DESCRIPTOR_GetProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t * length)
{
	USBD_GetString((uint8_t *)USB_DATA_DEVICE_DESCRIPTOR_PRODUCT_STRING, USB_DATA_DEVICE_DESCRIPTOR_ProductStrDescriptor, length);
	return USB_DATA_DEVICE_DESCRIPTOR_ProductStrDescriptor;
}

uint8_t * USB_DATA_DEVICE_DESCRIPTOR_GetSerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t * length)
{
	USBD_GetString((uint8_t *)USB_DATA_DEVICE_DESCRIPTOR_SERIAL_STRING, USB_DATA_DEVICE_DESCRIPTOR_SerialStrDescriptor, length);
	return USB_DATA_DEVICE_DESCRIPTOR_SerialStrDescriptor;
}

uint8_t * USB_DATA_DEVICE_DESCRIPTOR_GetConfigurationStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
	USBD_GetString((uint8_t *)USB_DATA_DEVICE_DESCRIPTOR_CONFIGURATION_STRING, USB_DATA_DEVICE_DESCRIPTOR_ConfigurationStrDescriptor, length);
	return USB_DATA_DEVICE_DESCRIPTOR_ConfigurationStrDescriptor;
}

uint8_t * USB_DATA_DEVICE_DESCRIPTOR_GetInterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t * length)
{
	USBD_GetString((uint8_t *)USB_DATA_DEVICE_DESCRIPTOR_INTERFACE_STRING, USB_DATA_DEVICE_DESCRIPTOR_InterfaceStrDescriptor, length);
	return USB_DATA_DEVICE_DESCRIPTOR_InterfaceStrDescriptor;
}
