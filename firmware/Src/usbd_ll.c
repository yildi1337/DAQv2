/*
 *******************************************************************************
 * File:            usbd_ll.c
 * Date:            2020-11-15
 * Author:          Phillip Durdaut
 * Summary:         Interface for calling HAL functions from the lower layer
 *                  functions
 * Notes:           -
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_data.h"
#include "usbd_core.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
USBD_StatusTypeDef USBD_LL_GetUSBD_Status(HAL_StatusTypeDef hal_status);

/* Program code --------------------------------------------------------------*/

USBD_StatusTypeDef USBD_LL_GetUSBD_Status(HAL_StatusTypeDef hal_status)
{
	if (hal_status == HAL_OK)
		return USBD_OK;
	else if (hal_status == HAL_BUSY)
		return USBD_BUSY;
	else
		return USBD_FAIL;
}

USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef * pdev)
{
	USB_DATA_GetHandlePCD()->pData = pdev;
	pdev->pData = USB_DATA_GetHandlePCD();

	HAL_PCDEx_SetRxFiFo(USB_DATA_GetHandlePCD(), USB_DATA_RXFIFO_ALL_SIZE_BYTES / 4);

	HAL_PCDEx_SetTxFiFo(USB_DATA_GetHandlePCD(), 0, USB_DATA_TXFIFO_ENDPOINT_0_SIZE_BYTES / 4);
	HAL_PCDEx_SetTxFiFo(USB_DATA_GetHandlePCD(), 1, USB_DATA_TXFIFO_ENDPOINT_1_SIZE_BYTES / 4);

	return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef * pdev)
{
	return USBD_LL_GetUSBD_Status(HAL_PCD_DeInit(pdev->pData));
}

USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef * pdev)
{
	return USBD_LL_GetUSBD_Status(HAL_PCD_Start(pdev->pData));
}

USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef * pdev)
{
	return USBD_LL_GetUSBD_Status(HAL_PCD_Stop(pdev->pData));
}

USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr, uint8_t ep_type, uint16_t ep_mps)
{
	HAL_StatusTypeDef hal_status = HAL_PCD_EP_Open(pdev->pData, ep_addr, ep_mps, ep_type);
	pdev->ep_in[ep_addr & 0x7F].is_used = 1;
	return USBD_LL_GetUSBD_Status(hal_status);
}

USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
	HAL_StatusTypeDef hal_status = HAL_PCD_EP_Close(pdev->pData, ep_addr);
	pdev->ep_in[ep_addr & 0x7F].is_used = 0;
	return USBD_LL_GetUSBD_Status(hal_status);
}

USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
	return USBD_LL_GetUSBD_Status(HAL_PCD_EP_Flush(pdev->pData, ep_addr));
}

USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
	return USBD_LL_GetUSBD_Status(HAL_PCD_EP_SetStall(pdev->pData, ep_addr));
}

USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
	return USBD_LL_GetUSBD_Status(HAL_PCD_EP_ClrStall(pdev->pData, ep_addr));
}

uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
	PCD_HandleTypeDef * hpcd = pdev->pData;
	return ep_addr & 0x80 ? hpcd->IN_ep[ep_addr & 0x7F].is_stall : hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
}

USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef * pdev, uint8_t dev_addr)
{
	return USBD_LL_GetUSBD_Status(HAL_PCD_SetAddress(pdev->pData, dev_addr));
}

USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef * pdev, uint8_t ep_addr, uint8_t * pbuf, uint16_t size)
{
	return USBD_LL_GetUSBD_Status(HAL_PCD_EP_Transmit(pdev->pData, ep_addr, pbuf, size));
}

USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef * pdev, uint8_t ep_addr, uint8_t * pbuf, uint16_t size)
{
	return USBD_LL_GetUSBD_Status(HAL_PCD_EP_Receive(pdev->pData, ep_addr, pbuf, size));
}

uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef * pdev, uint8_t ep_addr)
{
	return HAL_PCD_EP_GetRxCount((PCD_HandleTypeDef*)pdev->pData, ep_addr);
}

void USBD_LL_Delay(uint32_t Delay)
{
	HAL_Delay(Delay);
}
