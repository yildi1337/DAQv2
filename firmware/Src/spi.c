/*
 *******************************************************************************
 * File:			spi.c
 * Date:			2020-11-03
 * Author:			Phillip Durdaut
 * Summary:			Functions for SPI communication handling
 * Notes:			-
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA_HandleTypeDef DMA_HandleSPI2RxDMA1Stream0;
SPI_HandleTypeDef SPI_HandleSPI2;
SPI_HandleTypeDef SPI_HandleSPI3;

/* Private function prototypes -----------------------------------------------*/

/* Program code --------------------------------------------------------------*/

extern void SPI_Init(SPI_t spi, SPI_DATA_SIZE_t dataSize)
{
	if (spi == SPI_2)
	{
		/* Enable clock */
		__HAL_RCC_SPI2_CLK_ENABLE();

		/* Configure GPIO pins */
		GPIO_InitTypeDef GPIO_InitStruct = { 0 };

		GPIO_InitStruct.Pin = SPI2_NSS_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = SPI2_NSS_AF;
		HAL_GPIO_Init(SPI2_NSS_Port, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = SPI2_SCK_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = SPI2_SCK_AF;
		HAL_GPIO_Init(SPI2_SCK_Port, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = SPI2_MOSI_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = SPI2_MOSI_AF;
		HAL_GPIO_Init(SPI2_MOSI_Port, &GPIO_InitStruct);

		/* Configure SPI */
		SPI_HandleSPI2.Instance = SPI2;
		SPI_HandleSPI2.Init.Mode = SPI_MODE_SLAVE;
		SPI_HandleSPI2.Init.Direction = SPI_DIRECTION_2LINES_RXONLY;

		if (dataSize == SPI_DATA_SIZE_8BIT)
			SPI_HandleSPI2.Init.DataSize = SPI_DATASIZE_8BIT;
		else if (dataSize == SPI_DATA_SIZE_16BIT)
			SPI_HandleSPI2.Init.DataSize = SPI_DATASIZE_16BIT;
		else if (dataSize == SPI_DATA_SIZE_32BIT)
			SPI_HandleSPI2.Init.DataSize = SPI_DATASIZE_32BIT;

		SPI_HandleSPI2.Init.CLKPolarity = SPI_POLARITY_LOW;
		SPI_HandleSPI2.Init.CLKPhase = SPI_PHASE_2EDGE;
		SPI_HandleSPI2.Init.NSS = SPI_NSS_HARD_INPUT;
		SPI_HandleSPI2.Init.FirstBit = SPI_FIRSTBIT_MSB;
		SPI_HandleSPI2.Init.TIMode = SPI_TIMODE_DISABLE;
		SPI_HandleSPI2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
		SPI_HandleSPI2.Init.CRCPolynomial = 0x0;
		SPI_HandleSPI2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
		SPI_HandleSPI2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
		SPI_HandleSPI2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
		SPI_HandleSPI2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
		SPI_HandleSPI2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
		SPI_HandleSPI2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
		SPI_HandleSPI2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
		SPI_HandleSPI2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
		SPI_HandleSPI2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
		SPI_HandleSPI2.Init.IOSwap = SPI_IO_SWAP_DISABLE;

		if (HAL_SPI_Init(&SPI_HandleSPI2) != HAL_OK)
		{
			Error_Handler();
		}
	}

	if (spi == SPI_3)
	{
		/* Enable clock */
		__HAL_RCC_SPI3_CLK_ENABLE();

		/* Configure GPIO pins */
		GPIO_InitTypeDef GPIO_InitStruct = { 0 };

		GPIO_InitStruct.Pin = SPI3_SCK_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = SPI3_SCK_AF;
		HAL_GPIO_Init(SPI3_SCK_Port, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = SPI3_MISO_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = SPI3_MISO_AF;
		HAL_GPIO_Init(SPI3_MISO_Port, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = SPI3_MOSI_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = SPI3_MOSI_AF;
		HAL_GPIO_Init(SPI3_MOSI_Port, &GPIO_InitStruct);

		/* Configure SPI */
		SPI_HandleSPI3.Instance = SPI3;
		SPI_HandleSPI3.Init.Mode = SPI_MODE_MASTER;
		SPI_HandleSPI3.Init.Direction = SPI_DIRECTION_2LINES;

		if (dataSize == SPI_DATA_SIZE_8BIT)
			SPI_HandleSPI3.Init.DataSize = SPI_DATASIZE_8BIT;
		else if (dataSize == SPI_DATA_SIZE_16BIT)
			SPI_HandleSPI3.Init.DataSize = SPI_DATASIZE_16BIT;
		else if (dataSize == SPI_DATA_SIZE_32BIT)
			SPI_HandleSPI3.Init.DataSize = SPI_DATASIZE_32BIT;

		SPI_HandleSPI3.Init.CLKPolarity = SPI_POLARITY_LOW;
		SPI_HandleSPI3.Init.CLKPhase = SPI_PHASE_1EDGE;
		SPI_HandleSPI3.Init.NSS = SPI_NSS_SOFT;
		SPI_HandleSPI3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
		SPI_HandleSPI3.Init.FirstBit = SPI_FIRSTBIT_MSB;
		SPI_HandleSPI3.Init.TIMode = SPI_TIMODE_DISABLE;
		SPI_HandleSPI3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
		SPI_HandleSPI3.Init.CRCPolynomial = 0x0;
		SPI_HandleSPI3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
		SPI_HandleSPI3.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
		SPI_HandleSPI3.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
		SPI_HandleSPI3.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
		SPI_HandleSPI3.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
		SPI_HandleSPI3.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
		SPI_HandleSPI3.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
		SPI_HandleSPI3.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
		SPI_HandleSPI3.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
		SPI_HandleSPI3.Init.IOSwap = SPI_IO_SWAP_DISABLE;

		if (HAL_SPI_Init(&SPI_HandleSPI3) != HAL_OK)
		{
			Error_Handler();
		}
	}
}

extern uint8_t SPI_Read_8bit(SPI_t spi)
{
	uint8_t data = 0;

	if (spi == SPI_3)
	{
		/* Read 1 x 8 bit */
		HAL_SPI_Receive(&SPI_HandleSPI3, &data, 1, 1000);
	}

	return data;
}

extern uint16_t SPI_Read_16bit(SPI_t spi)
{
	uint16_t data = 0;

	return data;
}

extern void SPI_Write_8bit(SPI_t spi, uint8_t data)
{
	if (spi == SPI_3)
	{
		/* Write 1 x 8 bit */
		HAL_SPI_Transmit(&SPI_HandleSPI3, &data, 1, 1000);
	}
}

extern void SPI_Write_16bit(SPI_t spi, uint16_t data)
{
	if (spi == SPI_3)
	{
		/* Write 1 x 16 bit */
		HAL_SPI_Transmit(&SPI_HandleSPI3, (uint8_t *)&data, 1, 1000);
	}
}

extern SPI_HandleTypeDef * SPI_GetHandle(SPI_t spi)
{
	SPI_HandleTypeDef * pHandle;

	if (spi == SPI_2)
	{
		pHandle = &SPI_HandleSPI2;
	}

	if (spi == SPI_3)
	{
		pHandle = &SPI_HandleSPI3;
	}

	return pHandle;
}

