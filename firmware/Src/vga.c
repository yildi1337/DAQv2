/*
 *******************************************************************************
 * File:			vga.c
 * Date:			2020-11-07
 * Author:			Phillip Durdaut
 * Summary:			Functions for controlling the VGAs
 * Notes:			-
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "vga.h"
#include "mcp4822.h"
#include "lcd.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define VGA_GAIN_MIN					0.0
#define VGA_GAIN_MAX					80.0

#define VGA_GAIN_VMIN					0.1
#define VGA_GAIN_VMAX					1.1

#define VGA_VOLTAGE_PER_GAIN			((VGA_GAIN_VMAX-VGA_GAIN_VMIN)/(VGA_GAIN_MAX-VGA_GAIN_MIN))		/* 12.5 mV / 1 dB */

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
float VGA_Gain1 = VGA_GAIN_MIN;
float VGA_Gain2 = VGA_GAIN_MIN;
float VGA_Gain3 = VGA_GAIN_MIN;
float VGA_Gain4 = VGA_GAIN_MIN;

/* Private function prototypes -----------------------------------------------*/
float VGA_ConvertGainToVoltage(float gain);

/* Program code --------------------------------------------------------------*/

extern void VGA_Init(void)
{
	MCP4822_SetVoltageFloat(MCP4822_CHANNEL_A, VGA_GAIN_MIN);
	MCP4822_SetVoltageFloat(MCP4822_CHANNEL_B, VGA_GAIN_MIN);
}

extern void VGA_SetGain(VGA_t vga, float gain)
{
	/* Limit given voltage */
	if (gain < VGA_GAIN_MIN)
		gain = VGA_GAIN_MIN;
	if (gain > VGA_GAIN_MAX)
		gain = VGA_GAIN_MAX;

	/* Convert given gain into voltage */
	float voltage = VGA_ConvertGainToVoltage(gain);

	/* Set gain */
	switch(vga)
	{
		case VGA_1:
			VGA_Gain1 = gain;
			MCP4822_SetVoltageFloat(MCP4822_CHANNEL_A, voltage);
			break;
		case VGA_2:
			VGA_Gain2 = gain;
			MCP4822_SetVoltageFloat(MCP4822_CHANNEL_B, voltage);
			break;
	}

	/* Update LCD */
	char pStr[16];
	if (gain < 10) {
		sprintf(pStr, " %.1f dB", gain);
		LCD_SetCursor((LCD_ROW_t)vga, 9);
		LCD_SendString(pStr);
	}
	else {
		sprintf(pStr, "%.1f dB", gain);
		LCD_SetCursor((LCD_ROW_t)vga, 9);
		LCD_SendString(pStr);
	}
}

extern float VGA_GetGain(VGA_t vga)
{
	float gain = 0.0;

	switch(vga)
	{
		case VGA_1:
			gain = VGA_Gain1;
			break;
		case VGA_2:
			gain = VGA_Gain2;
			break;
	}

	return gain;
}

float VGA_ConvertGainToVoltage(float gain)
{
	return VGA_GAIN_VMIN + (VGA_VOLTAGE_PER_GAIN * gain);
}
