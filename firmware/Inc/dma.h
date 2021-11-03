/*
 *******************************************************************************
 * File:			dma.c
 * Date:			2020-11-11
 * Author:			Phillip Durdaut
 * Summary:			Functions for DMA handling
 * Notes:			-
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DMA_H
#define __DMA_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Public defines ------------------------------------------------------------*/
#define DMA_SPI2_RX_DMA1_STREAM0_BUFFER_SIZE_MILLI_SECONDS		5
#define DMA_SPI2_RX_DMA1_STREAM0_BUFFER_SIZE_BYTES				(MAIN_ADC_BYTES_PER_MILLI_SECOND * DMA_SPI2_RX_DMA1_STREAM0_BUFFER_SIZE_MILLI_SECONDS)
#define DMA_SPI2_RX_DMA1_STREAM0_CIRCULAR_BUFFER_SIZE_BYTES		(2 * DMA_SPI2_RX_DMA1_STREAM0_BUFFER_SIZE_BYTES)

/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
typedef enum
{
	DMA_SPI2_RX_DMA1_STREAM0
} DMA_t;

/* Public variables ----------------------------------------------------------*/
extern __attribute__((section(".dma_buffer"))) uint8_t DMA_SPI2_RX_DMA1_STREAM0_CircularBuffer[2][DMA_SPI2_RX_DMA1_STREAM0_BUFFER_SIZE_BYTES];
extern __attribute__((section(".dma_buffer"))) uint8_t DMA_SPI2_RX_DMA1_STREAM0_FinalDataBuffer[DMA_SPI2_RX_DMA1_STREAM0_BUFFER_SIZE_BYTES];

/* Public function prototypes ------------------------------------------------*/
extern void DMA_Init(DMA_t dma);
extern DMA_HandleTypeDef * DMA_GetHandle(DMA_t dma);
extern void DMA_LockFinalDataBuffer(DMA_t dma);
extern void DMA_UnlockFinalDataBuffer(DMA_t dma);
extern void DMA_WaitForUnlockedFinalDataBuffer(DMA_t dma);

#endif /* __DMA_H */
