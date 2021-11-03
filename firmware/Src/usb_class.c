/*
 *******************************************************************************
 * File:            usb_class.c
 * Date:            2020-11-14
 * Author:          Phillip Durdaut
 * Summary:         USB custom class driver
 * Notes:           -
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_class.h"
#include "usbd_ctlreq.h"
#include "usb_data.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define USB_CLASS_CONFIGURATION_DESCRIPTOR_SIZE_BYTES  			32
#define USB_CLASS_NUMBER_OF_INTERFACES							1

#define USB_CLASS_NUMBER_OF_ENDPOINTS_IN						1
#define USB_CLASS_NUMBER_OF_ENDPOINTS_OUT						1
#define USB_CLASS_NUMBER_OF_ENDPOINTS							(USB_CLASS_NUMBER_OF_ENDPOINTS_IN + USB_CLASS_NUMBER_OF_ENDPOINTS_OUT)

#define USB_CLASS_ENDPOINT_1_OUT								0x01
#define USB_CLASS_ENDPOINT_1_IN									0x81

#define USB_CLASS_ENDPOINT_1_OUT_BUFFER_SIZE_BYTES				128

/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
uint8_t USB_CLASS_Init(USBD_HandleTypeDef * pdev, uint8_t cfgidx);
uint8_t USB_CLASS_DeInit(USBD_HandleTypeDef * pdev, uint8_t cfgidx);
uint8_t USB_CLASS_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
uint8_t USB_CLASS_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
uint8_t * USB_CLASS_GetConfigDescriptor(uint16_t * length);
uint8_t * USB_CLASS_GetDeviceQualifierDescriptor(uint16_t * length);

/* Private variables ---------------------------------------------------------*/
uint8_t USB_CLASS_BufferEndpoint1Out[USB_CLASS_ENDPOINT_1_OUT_BUFFER_SIZE_BYTES] = { 0 };

USBD_ClassTypeDef USB_CLASS_ClassUSBD =
{
	USB_CLASS_Init,								/* Init */
	USB_CLASS_DeInit,							/* DeInit */
	NULL,										/* Setup */
	NULL,										/* EP0_TxSent */
	NULL,										/* EP0_RxReady */
	USB_CLASS_DataIn,							/* DataIn */
	USB_CLASS_DataOut,							/* DataOut */
	NULL,										/* SOF */
	NULL,										/* IsoINIncomplete */
	NULL,										/* IsoOUTIncomplete */
	USB_CLASS_GetConfigDescriptor,				/* GetHSConfigDescriptor */
	USB_CLASS_GetConfigDescriptor,				/* GetFSConfigDescriptor */
	USB_CLASS_GetConfigDescriptor,				/* GetOtherSpeedConfigDescriptor */
	USB_CLASS_GetDeviceQualifierDescriptor,		/* GetDeviceQualifierDescriptor */
};

__ALIGN_BEGIN uint8_t USB_CLASS_ConfigurationDescriptor[USB_CLASS_CONFIGURATION_DESCRIPTOR_SIZE_BYTES] __ALIGN_END =
{
	/* Configuration descriptor */
	0x09,                          							/* bLength (9 bytes) */
	USB_DESC_TYPE_CONFIGURATION,   							/* bDescriptorType (2 - configuration descriptor) */
	LOBYTE(USB_CLASS_CONFIGURATION_DESCRIPTOR_SIZE_BYTES),	/* wTotalLength */
	HIBYTE(USB_CLASS_CONFIGURATION_DESCRIPTOR_SIZE_BYTES),	/* wTotalLength (configuration descriptor + interface descriptor + endpoint descriptors) */
	USB_CLASS_NUMBER_OF_INTERFACES,							/* bNumInterfaces */
	0x01,                          							/* bConfigurationValue */
	0,                          							/* iConfiguration (no string) */
	0x80 | 0x00,											/* bmAttributes (not self-powered) */
	0xFA,													/* bMaxPower (500 mA) */

	/* Interface descriptor */
	0x09,                          							/* bLength (9 bytes) */
	USB_DESC_TYPE_INTERFACE,								/* bDescriptorType (4 - interface descriptor) */
	0x00,													/* bInterfaceNumber */
	0x00,													/* bAlternateSetting */
	USB_CLASS_NUMBER_OF_ENDPOINTS,							/* bNumEndpoints */
	0xff,													/* bInterfaceClass */
	0xff,													/* bInterfaceSubClass */
	0xff,													/* bInterfaceProtocol */
	0,														/* iInterface (no string) */

	/* Endpoint 1 out */
	0x07,                          							/* bLength (7 bytes) */
	USB_DESC_TYPE_ENDPOINT,									/* bDescriptorType (5 - endpoint descriptor) */
	USB_CLASS_ENDPOINT_1_OUT,             					/* bEndpointAddress */
	USBD_EP_TYPE_BULK,										/* bmAttributes */
	LOBYTE(USB_HS_MAX_PACKET_SIZE),							/* wMaxPacketSize */
	HIBYTE(USB_HS_MAX_PACKET_SIZE),							/* wMaxPacketSize */
	0x00,													/* bInterval */

	/* Endpoint 1 in */
	0x07,                          							/* bLength (7 bytes) */
	USB_DESC_TYPE_ENDPOINT,									/* bDescriptorType (5 - endpoint descriptor) */
	USB_CLASS_ENDPOINT_1_IN,             					/* bEndpointAddress */
	USBD_EP_TYPE_BULK,										/* bmAttributes */
	LOBYTE(USB_HS_MAX_PACKET_SIZE),							/* wMaxPacketSize */
	HIBYTE(USB_HS_MAX_PACKET_SIZE),							/* wMaxPacketSize */
	0x00													/* bInterval */
};

__ALIGN_BEGIN uint8_t USB_CLASS_DeviceQualifierDescriptor[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
	USB_LEN_DEV_QUALIFIER_DESC,								/* bLength (10 bytes) */
	USB_DESC_TYPE_DEVICE_QUALIFIER,							/* bDescriptorType (6 - device qualifier descriptor) */
	LOBYTE(0x0200),											/* bcdUSB */
	HIBYTE(0x0200),											/* bcdUSB (USB 2.0) */
	0xff,                       							/* bDeviceClass (vendor specific) */
	0xff,                       							/* bDeviceSubClass */
	0xff,                       							/* bDeviceProtocol (vendor specific) */
	USB_MAX_EP0_SIZE,           							/* bMaxPacketSize (64 bytes) */
	USBD_MAX_NUM_CONFIGURATION,								/* bNumConfigurations (1) */
	0x00													/* bReserved */
};

/* Program code --------------------------------------------------------------*/

extern USBD_ClassTypeDef * USB_CLASS_GetHandleClass_USBD(void)
{
	return &USB_CLASS_ClassUSBD;
}

extern uint8_t USB_CLASS_Transmit(USBD_HandleTypeDef * pdev, uint8_t * buf, uint16_t length, uint8_t epnum)
{
	if (epnum == 1)
	{
		return USBD_LL_Transmit(pdev, USB_CLASS_ENDPOINT_1_IN, buf, length);
	}

	return USBD_FAIL;
}

uint8_t USB_CLASS_Init(USBD_HandleTypeDef * pdev, uint8_t cfgidx)
{
	USBD_LL_OpenEP(pdev, USB_CLASS_ENDPOINT_1_OUT, USBD_EP_TYPE_BULK, USB_HS_MAX_PACKET_SIZE);
	USBD_LL_OpenEP(pdev, USB_CLASS_ENDPOINT_1_IN, USBD_EP_TYPE_BULK, USB_HS_MAX_PACKET_SIZE);

	USBD_LL_PrepareReceive(pdev, USB_CLASS_ENDPOINT_1_OUT, USB_CLASS_BufferEndpoint1Out, USB_HS_MAX_PACKET_SIZE);

	return USBD_OK;
}

uint8_t USB_CLASS_DeInit(USBD_HandleTypeDef * pdev, uint8_t cfgidx)
{
	USBD_LL_CloseEP(pdev, USB_CLASS_ENDPOINT_1_OUT);
	USBD_LL_CloseEP(pdev, USB_CLASS_ENDPOINT_1_IN);

	return USBD_OK;
}

uint8_t USB_CLASS_DataIn(USBD_HandleTypeDef * pdev, uint8_t epnum)
{
	return USBD_OK;
}

uint8_t USB_CLASS_DataOut(USBD_HandleTypeDef * pdev, uint8_t epnum)
{
	if (epnum == 1)
	{
		USB_DATA_RxEndpoint1(USB_CLASS_BufferEndpoint1Out, (uint8_t)USBD_LL_GetRxDataSize(pdev, epnum));
		USBD_LL_PrepareReceive(pdev, USB_CLASS_ENDPOINT_1_OUT, USB_CLASS_BufferEndpoint1Out, USB_HS_MAX_PACKET_SIZE);
	}

	return USBD_OK;
}

uint8_t * USB_CLASS_GetConfigDescriptor(uint16_t * length)
{
	*length = sizeof(USB_CLASS_ConfigurationDescriptor);
	return USB_CLASS_ConfigurationDescriptor;
}

uint8_t * USB_CLASS_GetDeviceQualifierDescriptor(uint16_t * length)
{
	*length = sizeof(USB_CLASS_DeviceQualifierDescriptor);
	return USB_CLASS_DeviceQualifierDescriptor;
}
