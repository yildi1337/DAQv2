/*
 *******************************************************************************
 * File:            i2c.c
 * Date:            2020-12-10
 * Author:          Phillip Durdaut
 * Summary:         Functions for I2C communication handling
 * Notes:           -
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef I2C_HandleI2C4;

/* Private function prototypes -----------------------------------------------*/

/* Program code --------------------------------------------------------------*/

extern void I2C_Init(I2C_t i2c)
{
	if (i2c == I2C_4)
	{
		/* Enable clock */
		__HAL_RCC_I2C4_CLK_ENABLE();

		/* Configure GPIO pins */
		GPIO_InitTypeDef GPIO_InitStruct = { 0 };

		GPIO_InitStruct.Pin = I2C4_SCL_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = I2C4_SCL_AF;
		HAL_GPIO_Init(I2C4_SCL_Port, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = I2C4_SDA_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = I2C4_SDA_AF;
		HAL_GPIO_Init(I2C4_SDA_Port, &GPIO_InitStruct);

		/* Configure I2C */
		I2C_HandleI2C4.Instance = I2C4;
		I2C_HandleI2C4.Init.Timing = 0x307075B1;
		I2C_HandleI2C4.Init.OwnAddress1 = 0;
		I2C_HandleI2C4.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
		I2C_HandleI2C4.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		I2C_HandleI2C4.Init.OwnAddress2 = 0;
		I2C_HandleI2C4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
		I2C_HandleI2C4.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		I2C_HandleI2C4.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

		if (HAL_I2C_Init(&I2C_HandleI2C4) != HAL_OK)
		{
			Error_Handler();
		}

		if (HAL_I2CEx_ConfigAnalogFilter(&I2C_HandleI2C4, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
		{
			Error_Handler();
		}

		if (HAL_I2CEx_ConfigDigitalFilter(&I2C_HandleI2C4, 0) != HAL_OK)
		{
			Error_Handler();
		}
	}
}

extern I2C_HandleTypeDef * I2C_GetHandle(I2C_t i2c)
{
	I2C_HandleTypeDef * pHandle;

	if (i2c == I2C_4)
	{
		pHandle = &I2C_HandleI2C4;
	}

	return pHandle;
}
