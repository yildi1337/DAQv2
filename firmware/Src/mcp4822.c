/*
 *******************************************************************************
 * File:			mcp4822.c
 * Date:			2020-11-05
 * Author:			Phillip Durdaut
 * Summary:			Functions for controlling the MCP822 DAC
 * Notes:			-
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "mcp4822.h"
#include "spi.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define MCP4822_VREF					2.048
#define MCP4822_LEVELS					4096
#define MCP4822_VLSB					(MCP4822_VREF/MCP4822_LEVELS)

#define MCP4822_VMIN					0.1
#define MCP4822_VMAX					1.1

#define MCP4822_WRITE_INTO_DACA			0x0000
#define MCP4822_WRITE_INTO_DACB			0x8000
#define MCP4822_SINGLE_VREF				0x2000
#define MCP4822_DOUBLE_VREF				0x0000		/* not implemented */
#define MCP4822_OUTPUT_DISABLED			0x0000		/* not implemented */
#define MCP4822_OUTPUT_ENABLED			0x1000

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t MCP4822_DataChannelA = 0;
uint16_t MCP4822_DataChannelB = 0;
float MCP4822_VoltageChannelA = 0.0;
float MCP4822_VoltageChannelB = 0.0;

/* Private function prototypes -----------------------------------------------*/
void MCP4822_ChipSelect(void);
void MCP4822_ChipUnselect(void);
void MCP4822_Latch(void);

/* Program code --------------------------------------------------------------*/

extern void MCP4822_Init(void)
{
	/* Configure GPIO pins */
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	GPIO_InitStruct.Pin = DAC_NCS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DAC_NCS_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = DAC_NLDAC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DAC_NLDAC_Port, &GPIO_InitStruct);

	/* Set standard voltage levels of output pins */
	HAL_GPIO_WritePin(DAC_NCS_Port, DAC_NCS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DAC_NLDAC_Pin, DAC_NLDAC_Pin, GPIO_PIN_SET);
}

extern void MCP4822_SetVoltageInteger(MCP4822_CHANNEL_t mcp4822Channel, uint16_t data)
{
	/* Select chip */
	MCP4822_ChipSelect();

	if (mcp4822Channel == MCP4822_CHANNEL_A)
	{
		SPI_Write_16bit(SPI_3, (MCP4822_WRITE_INTO_DACA | MCP4822_SINGLE_VREF | MCP4822_OUTPUT_ENABLED | (data & 0x0fff)));
	}

	if (mcp4822Channel == MCP4822_CHANNEL_B)
	{
		SPI_Write_16bit(SPI_3, (MCP4822_WRITE_INTO_DACB | MCP4822_SINGLE_VREF | MCP4822_OUTPUT_ENABLED | (data & 0x0fff)));
	}

	/* Unselect chip */
	MCP4822_ChipUnselect();

	/* Latch serial input data into DAC */
	MCP4822_Latch();

	/* Update member variables */
	if (mcp4822Channel == MCP4822_CHANNEL_A)
	{
		MCP4822_DataChannelA = (data & 0x0fff);
		MCP4822_VoltageChannelA = (float)((float)MCP4822_VLSB * (float)(data & 0x0fff));
	}
	if (mcp4822Channel == MCP4822_CHANNEL_B)
	{
		MCP4822_DataChannelB = (data & 0x0fff);
		MCP4822_VoltageChannelB = (float)((float)MCP4822_VLSB * (float)(data & 0x0fff));
	}
}

extern void MCP4822_SetVoltageFloat(MCP4822_CHANNEL_t mcp4822Channel, float voltage)
{
	/* Limit given voltage */
	if (voltage < MCP4822_VMIN)
		voltage = MCP4822_VMIN;
	if (voltage > MCP4822_VMAX)
		voltage = MCP4822_VMAX;

	/* Convert float voltage into integer */
	uint16_t data = (uint16_t)(voltage/(float)MCP4822_VLSB) & 0x0FFF;

	/* Set voltage */
	MCP4822_SetVoltageInteger(mcp4822Channel, data);
}

extern float MCP4822_GetVoltageFloat(MCP4822_CHANNEL_t mcp4822Channel)
{
	float voltage = 0.0;

	if (mcp4822Channel == MCP4822_CHANNEL_A)
		voltage = MCP4822_VoltageChannelA;
	if (mcp4822Channel == MCP4822_CHANNEL_B)
		voltage = MCP4822_VoltageChannelB;

	return voltage;
}

void MCP4822_ChipSelect(void)
{
	/* Initialize SPI3 */
	SPI_Init(SPI_3, SPI_DATA_SIZE_16BIT);

	HAL_GPIO_WritePin(DAC_NCS_Port, DAC_NCS_Pin, GPIO_PIN_RESET);
}

void MCP4822_ChipUnselect(void)
{
	HAL_GPIO_WritePin(DAC_NCS_Port, DAC_NCS_Pin, GPIO_PIN_SET);
}

void MCP4822_Latch(void)
{
	HAL_GPIO_WritePin(DAC_NLDAC_Port, DAC_NLDAC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DAC_NLDAC_Port, DAC_NLDAC_Pin, GPIO_PIN_SET);
}

