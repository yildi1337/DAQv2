/*
 *******************************************************************************
 * File:            max4820.c
 * Date:            2021-10-20
 * Author:          Phillip Durdaut
 * Summary:         Functions for controlling the MAX4820 relay driver
 * Notes:           -
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "max4820.h"
#include "spi.h"
#include "lcd.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define MAX4820_RESET_PULSE_LENGTH_MS							1
#define MAX4820_STARTUP_TIME_AFTER_RESET_MS						10

#define MAX4820_RELAY_PULSE_TIME_MS								100

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
MAX4820_INPUT_COUPLING_t MAX4820_InputCoupling[2] = { MAX4820_INPUT_COUPLING_UNDEFINED, MAX4820_INPUT_COUPLING_UNDEFINED };
MAX4820_INPUT_COUPLING_t MAX4820_InputType[2] = { MAX4820_INPUT_TYPE_UNDEFINED, MAX4820_INPUT_TYPE_UNDEFINED };

/* Private function prototypes -----------------------------------------------*/
void MAX4820_Reset(void);
void MAX4820_ChipSelect(void);
void MAX4820_ChipUnselect(void);
void MAX4820_SPIWrite(uint8_t data);

/* Program code --------------------------------------------------------------*/

extern void MAX4820_Init(void)
{
	/* Configure GPIO pins */
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	GPIO_InitStruct.Pin = RD_NRESET_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RD_NRESET_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = RD_NCS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RD_NCS_Port, &GPIO_InitStruct);

	/* Set standard voltage levels of output pins */
	HAL_GPIO_WritePin(RD_NRESET_Port, RD_NRESET_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RD_NCS_Port, RD_NCS_Pin, GPIO_PIN_SET);

	/* Reset */
	MAX4820_Reset();
}

extern MAX4820_INPUT_COUPLING_t MAX4820_GetInputCoupling(MAX4820_CHANNEL_t channel)
{
	return MAX4820_InputCoupling[channel];
}

extern MAX4820_INPUT_TYPE_t MAX4820_GetInputType(MAX4820_CHANNEL_t channel)
{
	return MAX4820_InputType[channel];
}

extern uint8_t * MAX4820_GetInputCouplingString(MAX4820_CHANNEL_t channel)
{
	if (MAX4820_GetInputCoupling(channel) == MAX4820_INPUT_COUPLING_DC)
		return ((uint8_t *)"DC\n");
	else if (MAX4820_GetInputCoupling(channel) == MAX4820_INPUT_COUPLING_AC)
		return ((uint8_t *)"AC\n");

	return NULL;
}

extern uint8_t * MAX4820_GetInputTypeString(MAX4820_CHANNEL_t channel)
{
	if (MAX4820_GetInputType(channel) == MAX4820_INPUT_TYPE_D)
		return ((uint8_t *)"D\n");
	else if (MAX4820_GetInputType(channel) == MAX4820_INPUT_TYPE_S)
		return ((uint8_t *)"S\n");

	return NULL;
}

extern void MAX4820_SetInputCoupling(MAX4820_CHANNEL_t channel, MAX4820_INPUT_COUPLING_t inputCoupling)
{
	if (channel == MAX4820_CHANNEL_1) {

		if (inputCoupling == MAX4820_INPUT_COUPLING_DC)
			MAX4820_SPIWrite(0b00000001);

		if (inputCoupling == MAX4820_INPUT_COUPLING_AC)
			MAX4820_SPIWrite(0b00000010);
	}

	if (channel == MAX4820_CHANNEL_2) {

		if (inputCoupling == MAX4820_INPUT_COUPLING_DC)
			MAX4820_SPIWrite(0b00000100);

		if (inputCoupling == MAX4820_INPUT_COUPLING_AC)
			MAX4820_SPIWrite(0b00001000);
	}

	HAL_Delay(MAX4820_RELAY_PULSE_TIME_MS);
	MAX4820_SPIWrite(0b00000000);

	/* Update LCD */
	LCD_SetCursor((LCD_ROW_t)channel, 0);

	if (inputCoupling == MAX4820_INPUT_COUPLING_DC)
		LCD_SendString("DC");

	if (inputCoupling == MAX4820_INPUT_COUPLING_AC)
		LCD_SendString("AC");

	/* Update private (member) variable */
	MAX4820_InputCoupling[channel] = inputCoupling;
}

extern void MAX4820_SetInputType(MAX4820_CHANNEL_t channel, MAX4820_INPUT_TYPE_t inputType)
{
	if (channel == MAX4820_CHANNEL_1) {

		if (inputType == MAX4820_INPUT_TYPE_S)
			MAX4820_SPIWrite(0b00100000);

		if (inputType == MAX4820_INPUT_TYPE_D)
			MAX4820_SPIWrite(0b00010000);
	}

	if (channel == MAX4820_CHANNEL_2) {

		if (inputType == MAX4820_INPUT_TYPE_S)
			MAX4820_SPIWrite(0b10000000);

		if (inputType == MAX4820_INPUT_TYPE_D)
			MAX4820_SPIWrite(0b01000000);
	}

	HAL_Delay(MAX4820_RELAY_PULSE_TIME_MS);
	MAX4820_SPIWrite(0b00000000);

	/* Update LCD */
	LCD_SetCursor((LCD_ROW_t)channel, 5);

	if (inputType == MAX4820_INPUT_TYPE_S)
		LCD_SendString("S");

	if (inputType == MAX4820_INPUT_TYPE_D)
		LCD_SendString("D");

	/* Update private (member) variable */
	MAX4820_InputType[channel] = inputType;
}

extern void MAX4820_Reset(void)
{
	HAL_GPIO_WritePin(RD_NRESET_Port, RD_NRESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(MAX4820_RESET_PULSE_LENGTH_MS);
	HAL_GPIO_WritePin(RD_NRESET_Port, RD_NRESET_Pin, GPIO_PIN_SET);
	HAL_Delay(MAX4820_STARTUP_TIME_AFTER_RESET_MS);
}

void MAX4820_ChipSelect(void)
{
	/* Initialize SPI3 */
	SPI_Init(SPI_3, SPI_DATA_SIZE_8BIT);

	HAL_GPIO_WritePin(RD_NCS_Port, RD_NCS_Pin, GPIO_PIN_RESET);
}

void MAX4820_ChipUnselect(void)
{
	HAL_GPIO_WritePin(RD_NCS_Port, RD_NCS_Pin, GPIO_PIN_SET);
}

void MAX4820_SPIWrite(uint8_t data)
{
	MAX4820_ChipSelect();
	SPI_Write_8bit(SPI_3, data);
	MAX4820_ChipUnselect();
}
