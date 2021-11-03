/*
 *******************************************************************************
 * File:			mcp4822.h
 * Date:			2020-11-05
 * Author:			Phillip Durdaut
 * Summary:			Functions for controlling the MCP822 DAC
 * Notes:			-
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCP4822_H
#define __MCP4822_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Public defines ------------------------------------------------------------*/
/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
typedef enum
{
	MCP4822_CHANNEL_A,
	MCP4822_CHANNEL_B
} MCP4822_CHANNEL_t;

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
extern void MCP4822_Init(void);
extern void MCP4822_SetVoltageInteger(MCP4822_CHANNEL_t mcp4822Channel, uint16_t data);
extern void MCP4822_SetVoltageFloat(MCP4822_CHANNEL_t mcp4822Channel, float voltage);
extern float MCP4822_GetVoltageFloat(MCP4822_CHANNEL_t mcp4822Channel);

#endif /* __MCP4822_H */
