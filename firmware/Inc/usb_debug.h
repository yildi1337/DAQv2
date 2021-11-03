/*
 *******************************************************************************
 * File:			usb_debug.h
 * Date:			2020-11-03
 * Author:			Phillip Durdaut
 * Summary:			Functions for communicating over the USB debug interface
 * Notes:			-
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DEBUG_H
#define __USB_DEBUG_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Public defines ------------------------------------------------------------*/
/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
typedef enum
{
	USB_DEBUG_COMMAND_NONE,

	USB_DEBUG_COMMAND_SYSTEM_RESET,

	USB_DEBUG_COMMAND_GET_COMMANDS,

	USB_DEBUG_COMMAND_GET_VOLTAGE_DAC1,
	USB_DEBUG_COMMAND_GET_VOLTAGE_DAC2,

	USB_DEBUG_COMMAND_GET_GAIN_VGA1,
	USB_DEBUG_COMMAND_GET_GAIN_VGA2,
	USB_DEBUG_COMMAND_SET_GAIN_VGA1,
	USB_DEBUG_COMMAND_SET_GAIN_VGA2,

	USB_DEBUG_COMMAND_GET_INPUT_COUPL_CH1,
	USB_DEBUG_COMMAND_GET_INPUT_COUPL_CH2,
	USB_DEBUG_COMMAND_SET_INPUT_COUPL_CH1_AC,
	USB_DEBUG_COMMAND_SET_INPUT_COUPL_CH1_DC,
	USB_DEBUG_COMMAND_SET_INPUT_COUPL_CH2_AC,
	USB_DEBUG_COMMAND_SET_INPUT_COUPL_CH2_DC,

	USB_DEBUG_COMMAND_GET_INPUT_TYPE_CH1,
	USB_DEBUG_COMMAND_GET_INPUT_TYPE_CH2,
	USB_DEBUG_COMMAND_SET_INPUT_TYPE_CH1_S,
	USB_DEBUG_COMMAND_SET_INPUT_TYPE_CH1_D,
	USB_DEBUG_COMMAND_SET_INPUT_TYPE_CH2_S,
	USB_DEBUG_COMMAND_SET_INPUT_TYPE_CH2_D,
} USB_DEBUG_COMMAND_t;

typedef float USB_DEBUG_PARAMETER_t;

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
extern void USB_DEBUG_Init(void);
extern void USB_DEBUG_TransmitString(uint8_t * pBuffer);
extern void USB_DEBUG_TransmitFloat(float data);
extern void USB_DEBUG_WelcomeScreen(void);
extern void USB_DEBUG_ClearTerminal(void);
extern bool USB_DEBUG_CommandAvailable(void);
extern USB_DEBUG_COMMAND_t USB_DEBUG_GetCommand(void);
extern USB_DEBUG_PARAMETER_t USB_DEBUG_GetParameter(void);
extern void USB_DEBUG_AcknowledgeCommand(void);

#endif /* __USB_DEBUG_H */
