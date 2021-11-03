/*
 *******************************************************************************
 * File:            usb_data.c
 * Date:            2020-11-13
 * Author:          Phillip Durdaut
 * Summary:         Functions for communicating high-speed data over the USB
 *                  interface
 * Notes:           -
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_class.h"
#include "usb_data_device_descriptor.h"
#include "usb_data.h"
#include "usbd_core.h"
#include "usbd_def.h"
#include "led.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define USB_DATA_FLOAT_MAX_LENGTH_CHARS                 64
#define USB_DATA_ENDPOINT_1                             1

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PCD_HandleTypeDef USB_DATA_HandlePCD;
USBD_HandleTypeDef USB_DATA_HandleUSBD;

USB_DATA_COMMAND_t USB_DATA_Command = USB_DATA_COMMAND_NONE;
USB_DATA_PARAMETER_t USB_DATA_Parameter = 0.0;

/* Private function prototypes -----------------------------------------------*/
bool USB_DATA_CheckCommand(const char * pBuffer, const char * pCommand);
USB_DATA_PARAMETER_t USB_DATA_StringToParameter(char * pString);

/* Program code --------------------------------------------------------------*/

extern void USB_DATA_Init(void)
{
	/* Enable clocks */
	__HAL_RCC_USB_OTG_HS_CLK_ENABLE();
	__HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE();

	/* Configure GPIO pins */
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	GPIO_InitStruct.Pin = USB_OTG_HS_ULPI_STP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = USB_OTG_HS_ULPI_STP_AF;
	HAL_GPIO_Init(USB_OTG_HS_ULPI_STP_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = USB_OTG_HS_ULPI_DIR_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = USB_OTG_HS_ULPI_DIR_AF;
	HAL_GPIO_Init(USB_OTG_HS_ULPI_DIR_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = USB_OTG_HS_ULPI_NXT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = USB_OTG_HS_ULPI_NXT_AF;
	HAL_GPIO_Init(USB_OTG_HS_ULPI_NXT_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = USB_OTG_HS_ULPI_CK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = USB_OTG_HS_ULPI_CK_AF;
	HAL_GPIO_Init(USB_OTG_HS_ULPI_CK_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = USB_OTG_HS_ULPI_D0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = USB_OTG_HS_ULPI_D0_AF;
	HAL_GPIO_Init(USB_OTG_HS_ULPI_D0_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = USB_OTG_HS_ULPI_D1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = USB_OTG_HS_ULPI_D1_AF;
	HAL_GPIO_Init(USB_OTG_HS_ULPI_D1_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = USB_OTG_HS_ULPI_D2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = USB_OTG_HS_ULPI_D2_AF;
	HAL_GPIO_Init(USB_OTG_HS_ULPI_D2_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = USB_OTG_HS_ULPI_D3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = USB_OTG_HS_ULPI_D3_AF;
	HAL_GPIO_Init(USB_OTG_HS_ULPI_D3_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = USB_OTG_HS_ULPI_D4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = USB_OTG_HS_ULPI_D4_AF;
	HAL_GPIO_Init(USB_OTG_HS_ULPI_D4_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = USB_OTG_HS_ULPI_D5_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = USB_OTG_HS_ULPI_D5_AF;
	HAL_GPIO_Init(USB_OTG_HS_ULPI_D5_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = USB_OTG_HS_ULPI_D6_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = USB_OTG_HS_ULPI_D6_AF;
	HAL_GPIO_Init(USB_OTG_HS_ULPI_D6_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = USB_OTG_HS_ULPI_D7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = USB_OTG_HS_ULPI_D7_AF;
	HAL_GPIO_Init(USB_OTG_HS_ULPI_D7_Port, &GPIO_InitStruct);

	/* Configure PCD */
	USB_DATA_HandlePCD.Instance = USB_OTG_HS;
	USB_DATA_HandlePCD.Init.dev_endpoints = 9;
	USB_DATA_HandlePCD.Init.speed = PCD_SPEED_HIGH;
	USB_DATA_HandlePCD.Init.dma_enable = DISABLE;
	USB_DATA_HandlePCD.Init.phy_itface = USB_OTG_ULPI_PHY;
	USB_DATA_HandlePCD.Init.Sof_enable = DISABLE;
	USB_DATA_HandlePCD.Init.low_power_enable = DISABLE;
	USB_DATA_HandlePCD.Init.lpm_enable = DISABLE;
	USB_DATA_HandlePCD.Init.vbus_sensing_enable = DISABLE;
	USB_DATA_HandlePCD.Init.use_dedicated_ep1 = DISABLE;
	USB_DATA_HandlePCD.Init.use_external_vbus = DISABLE;

	if (HAL_PCD_Init(&USB_DATA_HandlePCD) != HAL_OK)
	{
		Error_Handler();
	}

	/* Configure interrupts */
	HAL_NVIC_SetPriority(OTG_HS_EP1_OUT_IRQn, IRQ_OTG_HS_EP1_OUT_PREEMPT_PRIORITY, IRQ_OTG_HS_EP1_OUT_SUB_PRIORITY);
	HAL_NVIC_EnableIRQ(OTG_HS_EP1_OUT_IRQn);

	HAL_NVIC_SetPriority(OTG_HS_EP1_IN_IRQn, IRQ_OTG_HS_EP1_IN_PREEMPT_PRIORITY, IRQ_OTG_HS_EP1_IN_SUB_PRIORITY);
	HAL_NVIC_EnableIRQ(OTG_HS_EP1_IN_IRQn);

	HAL_NVIC_SetPriority(OTG_HS_IRQn, IRQ_OTG_HS_PREEMPT_PRIORITY, IRQ_OTG_HS_SUB_PRIORITY);
	HAL_NVIC_EnableIRQ(OTG_HS_IRQn);

	/* Initialize USB core */
	USBD_Init(&USB_DATA_HandleUSBD, USB_DATA_DEVICE_DESCRIPTOR_GetHandleUSBD_Descriptors(), 0);

	/* Register class */
	USBD_RegisterClass(&USB_DATA_HandleUSBD, USB_CLASS_GetHandleClass_USBD());

	/* Start USB connection */
	USBD_Start(&USB_DATA_HandleUSBD);
}

extern PCD_HandleTypeDef * USB_DATA_GetHandlePCD(void)
{
	return &USB_DATA_HandlePCD;
}

extern USBD_HandleTypeDef * USB_DATA_GetHandleUSBD(void)
{
	return &USB_DATA_HandleUSBD;
}

extern void USB_DATA_RxEndpoint1(uint8_t * pBuffer, uint8_t numberOfBytes)
{
	/* Parse incoming data */
	if (USB_DATA_CheckCommand((const char *)pBuffer, "START_MEASUREMENT;"))
		USB_DATA_Command = USB_DATA_COMMAND_MEASUREMENT_START;
	else if (USB_DATA_CheckCommand((const char *)pBuffer, "STOP_MEASUREMENT;"))
		USB_DATA_Command = USB_DATA_COMMAND_MEASUREMENT_STOP;
	else if (USB_DATA_CheckCommand((const char *)pBuffer, "GET_SAMPLES;"))
		USB_DATA_Command = USB_DATA_COMMAND_GET_SAMPLES;
	else
		USB_DATA_Command = USB_DATA_COMMAND_NONE;

	/* Process received command */
	switch (USB_DATA_Command)
	{
		case USB_DATA_COMMAND_MEASUREMENT_START:
			MAIN_MeasurementRunning = true;
			break;

		case USB_DATA_COMMAND_MEASUREMENT_STOP:
			MAIN_MeasurementRunning = false;
			break;

		case USB_DATA_COMMAND_GET_SAMPLES:
			MAIN_SamplesRequested = true;
			break;

		default:
			break;
	}
}

extern void USB_DATA_TxEndpoint1(uint8_t * pBuffer, uint32_t numberOfBytes)
{
	if (MAIN_MeasurementRunning && MAIN_SamplesRequested)
	{
		USB_CLASS_Transmit(&USB_DATA_HandleUSBD, pBuffer, numberOfBytes, USB_DATA_ENDPOINT_1);
		MAIN_SamplesRequested = false;
	}
}

bool USB_DATA_CheckCommand(const char * pBuffer, const char * pCommand)
{
	if (strncmp(pBuffer, pCommand, strlen(pCommand)) == 0)
		return true;
	else
		return false;
}

USB_DATA_PARAMETER_t USB_DATA_StringToParameter(char * pString)
{
	char * pStart = pString;
	char * pEnd = NULL;

	for (uint8_t i = 0; i < USB_DATA_FLOAT_MAX_LENGTH_CHARS; i++)
	{
		if (pStart[1+i] == ';')
		{
			pEnd = &(pStart[1+i]);
		}
	}
	uint8_t length = pEnd - pStart;

	uint8_t tempBuffer[USB_DATA_FLOAT_MAX_LENGTH_CHARS] = { 0 };
	strncpy((char *)tempBuffer, (char *)pStart, length);
	return (USB_DATA_PARAMETER_t)atof((const char*)tempBuffer);
}
