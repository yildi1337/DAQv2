/*
 *******************************************************************************
 * File:			ad7768.c
 * Date:			2020-11-08
 * Author:			Phillip Durdaut
 * Summary:			Functions for controlling the AD7768(-4BSTZ) ADC
 * Notes:			Mode B registers not used
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ad7768.h"
#include "spi.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define AD7768_RESET_PULSE_LENGTH_MS							1
#define AD7768_STARTUP_TIME_AFTER_RESET_MS						10

#define AD7768_REGISTER_CHANNEL_STANDBY							0x00
#define AD7768_REGISTER_CHANNEL_MODE_A							0x01
#define AD7768_REGISTER_CHANNEL_MODE_B							0x02
#define AD7768_REGISTER_CHANNEL_MODE_SELECT						0x03
#define AD7768_REGISTER_POWER_MODE								0x04
#define AD7768_REGISTER_GENERAL_CONFIGURATION					0x05
#define AD7768_REGISTER_DATA_CONTROL							0x06
#define AD7768_REGISTER_INTERFACE_CONFIGURATION					0x07
#define AD7768_REGISTER_BIST_CONTROL							0x08
#define AD7768_REGISTER_DEVICE_STATUS							0x09
#define AD7768_REGISTER_REVISION_ID								0x0A
#define AD7768_REGISTER_GPIO_CONTROL							0x0E
#define AD7768_REGISTER_GPIO_WRITE_DATA							0x0F
#define AD7768_REGISTER_GPIO_READ_DATA							0x10
#define AD7768_REGISTER_PRECHARGE_BUFFER_1						0x11
#define AD7768_REGISTER_PRECHARGE_BUFFER_2						0x12
#define AD7768_REGISTER_POSITIVE_REFERENCE_PRECHARGE_BUFFER		0x13
#define AD7768_REGISTER_NEGATIVE_REFERENCE_PRECHARGE_BUFFER		0x14
#define AD7768_REGISTER_CHANNEL_0_OFFSET_MSB					0x1E
#define AD7768_REGISTER_CHANNEL_0_OFFSET_MID					0x1F
#define AD7768_REGISTER_CHANNEL_0_OFFSET_LSB					0x20
#define AD7768_REGISTER_CHANNEL_1_OFFSET_MSB					0x21
#define AD7768_REGISTER_CHANNEL_1_OFFSET_MID					0x22
#define AD7768_REGISTER_CHANNEL_1_OFFSET_LSB					0x23
#define AD7768_REGISTER_CHANNEL_2_OFFSET_MSB					0x2A
#define AD7768_REGISTER_CHANNEL_2_OFFSET_MID					0x2B
#define AD7768_REGISTER_CHANNEL_2_OFFSET_LSB					0x2C
#define AD7768_REGISTER_CHANNEL_3_OFFSET_MSB					0x2D
#define AD7768_REGISTER_CHANNEL_3_OFFSET_MID					0x2E
#define AD7768_REGISTER_CHANNEL_3_OFFSET_LSB					0x2F
#define AD7768_REGISTER_CHANNEL_0_GAIN_MSB						0x36
#define AD7768_REGISTER_CHANNEL_0_GAIN_MID						0x37
#define AD7768_REGISTER_CHANNEL_0_GAIN_LSB						0x38
#define AD7768_REGISTER_CHANNEL_1_GAIN_MSB						0x39
#define AD7768_REGISTER_CHANNEL_1_GAIN_MID						0x3A
#define AD7768_REGISTER_CHANNEL_1_GAIN_LSB						0x3B
#define AD7768_REGISTER_CHANNEL_2_GAIN_MSB						0x42
#define AD7768_REGISTER_CHANNEL_2_GAIN_MID						0x43
#define AD7768_REGISTER_CHANNEL_2_GAIN_LSB						0x44
#define AD7768_REGISTER_CHANNEL_3_GAIN_MSB						0x45
#define AD7768_REGISTER_CHANNEL_3_GAIN_MID						0x46
#define AD7768_REGISTER_CHANNEL_4_GAIN_LSB						0x47
#define AD7768_REGISTER_CHANNEL_0_SYNC_OFFSET					0x4E
#define AD7768_REGISTER_CHANNEL_1_SYNC_OFFSET					0x4F
#define AD7768_REGISTER_CHANNEL_2_SYNC_OFFSET					0x52
#define AD7768_REGISTER_CHANNEL_3_SYNC_OFFSET					0x53
#define AD7768_REGISTER_DIAGNOSTIC_RX							0x56
#define AD7768_REGISTER_DIAGNOSTIC_MUX_CONTROL					0x57
#define AD7768_REGISTER_MODULATOR_DELAY_CONTROL					0x58
#define AD7768_REGISTER_CHOP_CONTROL							0x59

/* Private macros ------------------------------------------------------------*/
#define AD7768_SPI_READ(x)			(0x80 | x)
#define AD7768_SPI_WRITE(x)			(0x7F & x)

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void AD7768_ChipSelect(void);
void AD7768_ChipUnselect(void);
uint8_t AD7768_SPIRead(uint8_t address);
void AD7768_SPIWrite(uint8_t address, uint8_t data);
void AD7768_SPISync(void);

/* Program code --------------------------------------------------------------*/

extern void AD7768_Init(void)
{
	/* Configure GPIO pins */
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	GPIO_InitStruct.Pin = ADC_SPI_NRESET_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ADC_SPI_NRESET_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ADC_SPI_NCS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ADC_SPI_NCS_Port, &GPIO_InitStruct);

	/* Set standard voltage levels of output pins */
	HAL_GPIO_WritePin(ADC_SPI_NRESET_Port, ADC_SPI_NRESET_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ADC_SPI_NCS_Port, ADC_SPI_NCS_Pin, GPIO_PIN_SET);

	/* Reset */
	AD7768_Reset();

	/* Configure ADC (channel mode B to save current) */
	AD7768_SPIWrite(AD7768_REGISTER_CHANNEL_MODE_B, 0x00);				/* sinc5 filter, x32 decimation rate */
	AD7768_SPIWrite(AD7768_REGISTER_CHANNEL_MODE_SELECT, 0x33);			/* channel mode B for all channels */
	AD7768_SPIWrite(AD7768_REGISTER_POWER_MODE, 0x33);					/* normal operation, fast, LVDS input clock disabled, fmod = MCLK/4 */
	AD7768_SPIWrite(AD7768_REGISTER_GENERAL_CONFIGURATION, 0x18);		/* SYNC_OUT disabled, VCM buffer powered down */
	AD7768_SPIWrite(AD7768_REGISTER_INTERFACE_CONFIGURATION, 0x03);		/* no CRC, DCLK = MCLK */

	/* SPI_SYNC (single shot mode disabled, no SPI_RESET) */
	AD7768_SPISync();
}

extern void AD7768_Reset(void)
{
	HAL_GPIO_WritePin(ADC_SPI_NRESET_Port, ADC_SPI_NRESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(AD7768_RESET_PULSE_LENGTH_MS);
	HAL_GPIO_WritePin(ADC_SPI_NRESET_Port, ADC_SPI_NRESET_Pin, GPIO_PIN_SET);
	HAL_Delay(AD7768_STARTUP_TIME_AFTER_RESET_MS);
}

extern uint8_t AD7768_GetRevisionID(void)
{
	return AD7768_SPIRead(AD7768_REGISTER_REVISION_ID);
}

void AD7768_ChipSelect(void)
{
	/* Initialize SPI3 */
	SPI_Init(SPI_3, SPI_DATA_SIZE_8BIT);

	HAL_GPIO_WritePin(ADC_SPI_NCS_Port, ADC_SPI_NCS_Pin, GPIO_PIN_RESET);
}

void AD7768_ChipUnselect(void)
{
	HAL_GPIO_WritePin(ADC_SPI_NCS_Port, ADC_SPI_NCS_Pin, GPIO_PIN_SET);
}

uint8_t AD7768_SPIRead(uint8_t address)
{
	AD7768_ChipSelect();

	SPI_Write_8bit(SPI_3, AD7768_SPI_READ(address));
	SPI_Read_8bit(SPI_3);

	AD7768_ChipUnselect();

	AD7768_ChipSelect();

	SPI_Write_8bit(SPI_3, AD7768_SPI_READ(address));
	uint8_t data = SPI_Read_8bit(SPI_3);

	AD7768_ChipUnselect();

	return data;
}

void AD7768_SPIWrite(uint8_t address, uint8_t data)
{
	AD7768_ChipSelect();

	SPI_Write_8bit(SPI_3, AD7768_SPI_WRITE(address));
	SPI_Write_8bit(SPI_3, data);

	AD7768_ChipUnselect();
}

void AD7768_SPISync(void)
{
	/* SPI_SYNC (single shot mode disabled, no SPI_RESET) */
	AD7768_SPIWrite(AD7768_REGISTER_DATA_CONTROL, 0x00);
	AD7768_SPIWrite(AD7768_REGISTER_DATA_CONTROL, 0x80);
}
