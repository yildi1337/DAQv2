/*
 *******************************************************************************
 * File:			usb_debug.c
 * Date:			2020-11-03
 * Author:			Phillip Durdaut
 * Summary:			Functions for communicating over the USB debug interface
 * Notes:			-
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_debug.h"
#include "uart.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define USB_DEBUG_FLOAT_MAX_LENGTH_CHARS		64

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USB_DEBUG_COMMAND_t USB_DEBUG_Command = USB_DEBUG_COMMAND_NONE;
USB_DEBUG_PARAMETER_t USB_DEBUG_Parameter = 0.0;

/* Private function prototypes -----------------------------------------------*/
bool USB_DEBUG_CheckCommand(const char * pCommand);
USB_DEBUG_PARAMETER_t USB_DEBUG_StringToParameter(char * pString);

/* Program code --------------------------------------------------------------*/

extern void USB_DEBUG_Init(void)
{
	return;
}

extern void USB_DEBUG_TransmitString(uint8_t * pBuffer)
{
	UART_TransmitData(UART_1, pBuffer, strlen((const char *)pBuffer));
}

extern void USB_DEBUG_TransmitFloat(float data)
{
	char str[32];
	sprintf(str, "%.3f\n", data);
	USB_DEBUG_TransmitString((uint8_t *)str);
}

extern void USB_DEBUG_WelcomeScreen(void)
{
	USB_DEBUG_ClearTerminal();
	USB_DEBUG_TransmitString((uint8_t *)"--------------------------------------------------------------------------\n");
	USB_DEBUG_TransmitString((uint8_t *)"                        DAQ v2 USB Debug Interface                        \n");
	USB_DEBUG_TransmitString((uint8_t *)"                                 pd, 2021                                 \n");
	USB_DEBUG_TransmitString((uint8_t *)"--------------------------------------------------------------------------\n");
	USB_DEBUG_TransmitString((uint8_t *)"  System reset:                                            SYSTEM_RESET;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"                                                                          \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Get this list of commands:                                   GET_COMM;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"                                                                          \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Get output voltage of DAC 1 [0.1 - 1.1] [Volt]:         GET_VOLT_DAC1;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Get output voltage of DAC 2 [0.1 - 1.1] [Volt]:         GET_VOLT_DAC2;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"                                                                          \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Get gain of VGA 1 [0 - 80] [dB]:                        GET_GAIN_VGA1;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Get gain of VGA 2 [0 - 80] [dB]:                        GET_GAIN_VGA2;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Set gain of VGA 1 [0 - 80] [dB]:                      SET_GAIN_VGA1 x;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Set gain of VGA 2 [0 - 80] [dB]:                      SET_GAIN_VGA2 x;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"                                                                          \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Get input coupling CH 1 [AC / DC]:                GET_INPUT_COUPL_CH1;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Get input coupling CH 2 [AC / DC]:                GET_INPUT_COUPL_CH2;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Set input coupling CH 1 to AC:                 SET_INPUT_COUPL_CH1_AC;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Set input coupling CH 1 to DC:                 SET_INPUT_COUPL_CH1_DC;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Set input coupling CH 2 to AC:                 SET_INPUT_COUPL_CH2_AC;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Set input coupling CH 2 to DC:                 SET_INPUT_COUPL_CH2_DC;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"                                                                          \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Get input type CH 1 [S / D]:                       GET_INPUT_TYPE_CH1;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Get input type CH 2 [S / D]:                       GET_INPUT_TYPE_CH2;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Set input type CH 1 to S:                        SET_INPUT_TYPE_CH1_S;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Set input type CH 1 to D:                        SET_INPUT_TYPE_CH1_D;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Set input type CH 2 to S:                        SET_INPUT_TYPE_CH2_S;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"  Set input type CH 2 to D:                        SET_INPUT_TYPE_CH2_D;  \n");
	USB_DEBUG_TransmitString((uint8_t *)"--------------------------------------------------------------------------\n");
}

extern void USB_DEBUG_ClearTerminal(void)
{
	USB_DEBUG_TransmitString((uint8_t *)"\n");
	USB_DEBUG_TransmitString((uint8_t *)"\n");
	USB_DEBUG_TransmitString((uint8_t *)"\n");
	USB_DEBUG_TransmitString((uint8_t *)"\n");
	USB_DEBUG_TransmitString((uint8_t *)"\n");
}

extern bool USB_DEBUG_CommandAvailable(void)
{
	UART_UART1_RECEIVED_COMMAND_t * pReceivedCommandStruct = GetReferenceOnReceivedCommandStruct(UART_1);

	if (pReceivedCommandStruct->available == true && pReceivedCommandStruct->acknowledged == false)
		return true;
	else
		return false;
}

extern USB_DEBUG_COMMAND_t USB_DEBUG_GetCommand(void)
{
	USB_DEBUG_Command = USB_DEBUG_COMMAND_NONE;
	UART_UART1_RECEIVED_COMMAND_t * pReceivedCommandStruct = GetReferenceOnReceivedCommandStruct(UART_1);

	if (USB_DEBUG_CommandAvailable())
	{
		if (USB_DEBUG_CheckCommand("SYSTEM_RESET;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_SYSTEM_RESET;

		else if (USB_DEBUG_CheckCommand("GET_COMM;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_GET_COMMANDS;

		else if (USB_DEBUG_CheckCommand("GET_VOLT_DAC1;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_GET_VOLTAGE_DAC1;
		else if (USB_DEBUG_CheckCommand("GET_VOLT_DAC2;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_GET_VOLTAGE_DAC2;

		else if (USB_DEBUG_CheckCommand("GET_GAIN_VGA1;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_GET_GAIN_VGA1;
		else if (USB_DEBUG_CheckCommand("GET_GAIN_VGA2;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_GET_GAIN_VGA2;

		else if (USB_DEBUG_CheckCommand("SET_GAIN_VGA"))
		{
			if (pReceivedCommandStruct->command[12] == '1')
				USB_DEBUG_Command = USB_DEBUG_COMMAND_SET_GAIN_VGA1;
			else if (pReceivedCommandStruct->command[12] == '2')
				USB_DEBUG_Command = USB_DEBUG_COMMAND_SET_GAIN_VGA2;
			else
				USB_DEBUG_Command = USB_DEBUG_COMMAND_NONE;

			if (pReceivedCommandStruct->command[13] == ' ')
				USB_DEBUG_Parameter = USB_DEBUG_StringToParameter((char *)&(pReceivedCommandStruct->command[14]));
			else
				USB_DEBUG_Command = USB_DEBUG_COMMAND_NONE;
		}

		else if (USB_DEBUG_CheckCommand("GET_INPUT_COUPL_CH1;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_GET_INPUT_COUPL_CH1;
		else if (USB_DEBUG_CheckCommand("GET_INPUT_COUPL_CH2;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_GET_INPUT_COUPL_CH2;

		else if (USB_DEBUG_CheckCommand("SET_INPUT_COUPL_CH1_AC;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_SET_INPUT_COUPL_CH1_AC;
		else if (USB_DEBUG_CheckCommand("SET_INPUT_COUPL_CH1_DC;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_SET_INPUT_COUPL_CH1_DC;
		else if (USB_DEBUG_CheckCommand("SET_INPUT_COUPL_CH2_AC;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_SET_INPUT_COUPL_CH2_AC;
		else if (USB_DEBUG_CheckCommand("SET_INPUT_COUPL_CH2_DC;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_SET_INPUT_COUPL_CH2_DC;

		else if (USB_DEBUG_CheckCommand("GET_INPUT_TYPE_CH1;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_GET_INPUT_TYPE_CH1;
		else if (USB_DEBUG_CheckCommand("GET_INPUT_TYPE_CH2;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_GET_INPUT_TYPE_CH2;

		else if (USB_DEBUG_CheckCommand("SET_INPUT_TYPE_CH1_S;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_SET_INPUT_TYPE_CH1_S;
		else if (USB_DEBUG_CheckCommand("SET_INPUT_TYPE_CH1_D;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_SET_INPUT_TYPE_CH1_D;
		else if (USB_DEBUG_CheckCommand("SET_INPUT_TYPE_CH2_S;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_SET_INPUT_TYPE_CH2_S;
		else if (USB_DEBUG_CheckCommand("SET_INPUT_TYPE_CH2_D;"))
			USB_DEBUG_Command = USB_DEBUG_COMMAND_SET_INPUT_TYPE_CH2_D;

		else
		{
			/* Acknowledge not supported command to reactivate the receiver */
			USB_DEBUG_AcknowledgeCommand();
		}
	}

	return USB_DEBUG_Command;
}

extern USB_DEBUG_PARAMETER_t USB_DEBUG_GetParameter(void)
{
	return USB_DEBUG_Parameter;
}

extern void USB_DEBUG_AcknowledgeCommand(void)
{
	UART_UART1_RECEIVED_COMMAND_t * pReceivedCommandStruct = GetReferenceOnReceivedCommandStruct(UART_1);

	pReceivedCommandStruct->available = false;
	pReceivedCommandStruct->acknowledged = true;

	USB_DEBUG_Command = USB_DEBUG_COMMAND_NONE;
	USB_DEBUG_Parameter = 0.0;
}

bool USB_DEBUG_CheckCommand(const char * pCommand)
{
	UART_UART1_RECEIVED_COMMAND_t * pReceivedCommandStruct = GetReferenceOnReceivedCommandStruct(UART_1);

	if (strncmp((const char *)pReceivedCommandStruct->command, pCommand, strlen(pCommand)) == 0)
		return true;
	else
		return false;
}

USB_DEBUG_PARAMETER_t USB_DEBUG_StringToParameter(char * pString)
{
	char * pStart = pString;
	char * pEnd = NULL;

	for (uint8_t i = 0; i < USB_DEBUG_FLOAT_MAX_LENGTH_CHARS; i++)
	{
		if (pStart[1+i] == ';')
		{
			pEnd = &(pStart[1+i]);
		}
	}
	uint8_t length = pEnd - pStart;

	uint8_t tempBuffer[USB_DEBUG_FLOAT_MAX_LENGTH_CHARS] = { 0 };
	strncpy((char *)tempBuffer, (char *)pStart, length);
	return (USB_DEBUG_PARAMETER_t)atof((const char*)tempBuffer);
}
