/*
 *******************************************************************************
 * File:            dma.c
 * Date:            2020-11-11
 * Author:          Phillip Durdaut
 * Summary:         Functions for DMA handling
 * Notes:           -
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DMA_HandleTypeDef DMA_HandleSPI2RxDMA1Stream0;
__attribute__((section(".dma_buffer"))) uint8_t DMA_SPI2_RX_DMA1_STREAM0_CircularBuffer[2][DMA_SPI2_RX_DMA1_STREAM0_BUFFER_SIZE_BYTES] = { 0 };
__attribute__((section(".dma_buffer"))) uint8_t DMA_SPI2_RX_DMA1_STREAM0_FinalDataBuffer[DMA_SPI2_RX_DMA1_STREAM0_BUFFER_SIZE_BYTES] = { 0 };
flag_t DMA_SPI2_RX_DMA1_STREAM0_FinalDataBufferLock = false;

/* Private function prototypes -----------------------------------------------*/

/* Program code --------------------------------------------------------------*/

extern void DMA_Init(DMA_t dma)
{
	if (dma == DMA_SPI2_RX_DMA1_STREAM0)
	{
		/* Enable clock */
		__HAL_RCC_DMA1_CLK_ENABLE();

		/* Configure DMA */
		DMA_HandleSPI2RxDMA1Stream0.Instance = DMA1_Stream0;
		DMA_HandleSPI2RxDMA1Stream0.Init.Request = DMA_REQUEST_SPI2_RX;
		DMA_HandleSPI2RxDMA1Stream0.Init.Direction = DMA_PERIPH_TO_MEMORY;
		DMA_HandleSPI2RxDMA1Stream0.Init.PeriphInc = DMA_PINC_DISABLE;
		DMA_HandleSPI2RxDMA1Stream0.Init.MemInc = DMA_MINC_ENABLE;
		DMA_HandleSPI2RxDMA1Stream0.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		DMA_HandleSPI2RxDMA1Stream0.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		DMA_HandleSPI2RxDMA1Stream0.Init.Mode = DMA_CIRCULAR;
		DMA_HandleSPI2RxDMA1Stream0.Init.Priority = DMA_PRIORITY_HIGH;
		DMA_HandleSPI2RxDMA1Stream0.Init.FIFOMode = DMA_FIFOMODE_ENABLE;

		if (HAL_DMA_Init(&DMA_HandleSPI2RxDMA1Stream0) != HAL_OK)
		{
			Error_Handler();
		}

		__HAL_LINKDMA(SPI_GetHandle(SPI_2), hdmarx, DMA_HandleSPI2RxDMA1Stream0);

		/* Configure interrupts */
		HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, IRQ_DMA1_STREAM0_PREEMPT_PRIORITY, IRQ_DMA1_STREAM0_SUB_PRIORITY);
		HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
	}
}

extern DMA_HandleTypeDef * DMA_GetHandle(DMA_t dma)
{
	DMA_HandleTypeDef * pHandle;

	if (dma == DMA_SPI2_RX_DMA1_STREAM0)
	{
		pHandle = &DMA_HandleSPI2RxDMA1Stream0;
	}

	return pHandle;
}

extern void DMA_LockFinalDataBuffer(DMA_t dma)
{
	if (dma == DMA_SPI2_RX_DMA1_STREAM0)
	{
		DMA_SPI2_RX_DMA1_STREAM0_FinalDataBufferLock = true;
	}
}

extern void DMA_UnlockFinalDataBuffer(DMA_t dma)
{
	if (dma == DMA_SPI2_RX_DMA1_STREAM0)
	{
		DMA_SPI2_RX_DMA1_STREAM0_FinalDataBufferLock = false;
	}
}

extern void DMA_WaitForUnlockedFinalDataBuffer(DMA_t dma)
{
	if (dma == DMA_SPI2_RX_DMA1_STREAM0)
	{
		while(DMA_SPI2_RX_DMA1_STREAM0_FinalDataBufferLock);
	}
}
