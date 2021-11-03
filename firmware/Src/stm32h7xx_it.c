/*
 *******************************************************************************
 * File:			stm32h7xx_it.c
 * Date:			2020-11-03
 * Author:			Phillip Durdaut
 * Summary:			Interrupt service routines
 * Notes:			-
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
#include "uart.h"
#include "spi.h"
#include "dma.h"
#include "usb_data.h"
#include "led.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Program code --------------------------------------------------------------*/

void NMI_Handler(void)
{

}

void HardFault_Handler(void)
{
	while (1)
	{

	}
}

void MemManage_Handler(void)
{
	while (1)
	{

	}
}

void BusFault_Handler(void)
{
	while (1)
	{

	}
}

void UsageFault_Handler(void)
{
	while (1)
	{

	}
}

void SVC_Handler(void)
{

}

void DebugMon_Handler(void)
{

}

void PendSV_Handler(void)
{

}

void SysTick_Handler(void)
{
	HAL_IncTick();
}

void DMA1_Stream0_IRQHandler(void)
{
	flag_t DMAHalfTransferComplete = 0;
	flag_t DMAFullTransferComplete = 0;

	if ((DMA1->LISR & DMA_FLAG_HTIF0_4) == DMA_FLAG_HTIF0_4)
		DMAHalfTransferComplete = 1;
	else if ((DMA1->LISR & DMA_FLAG_TCIF0_4) == DMA_FLAG_TCIF0_4)
		DMAFullTransferComplete = 1;

	HAL_DMA_IRQHandler(DMA_GetHandle(DMA_SPI2_RX_DMA1_STREAM0));

	if (DMAHalfTransferComplete)
	{
		if (MAIN_MeasurementRunning)
		{
			memcpy(DMA_SPI2_RX_DMA1_STREAM0_FinalDataBuffer, DMA_SPI2_RX_DMA1_STREAM0_CircularBuffer[0], DMA_SPI2_RX_DMA1_STREAM0_BUFFER_SIZE_BYTES);
			USB_DATA_TxEndpoint1(DMA_SPI2_RX_DMA1_STREAM0_FinalDataBuffer, DMA_SPI2_RX_DMA1_STREAM0_BUFFER_SIZE_BYTES);
		}
	}
	else if (DMAFullTransferComplete)
	{
		if (MAIN_MeasurementRunning)
		{
			memcpy(DMA_SPI2_RX_DMA1_STREAM0_FinalDataBuffer, DMA_SPI2_RX_DMA1_STREAM0_CircularBuffer[1], DMA_SPI2_RX_DMA1_STREAM0_BUFFER_SIZE_BYTES);
			USB_DATA_TxEndpoint1(DMA_SPI2_RX_DMA1_STREAM0_FinalDataBuffer, DMA_SPI2_RX_DMA1_STREAM0_BUFFER_SIZE_BYTES);
		}
	}

	return;
}

void SPI2_IRQHandler(void)
{
	HAL_SPI_IRQHandler(SPI_GetHandle(SPI_2));
}

void USART1_IRQHandler(void)
{
	/* RX buffer not empty interrupt */
	if (__HAL_UART_GET_FLAG(UART_GetHandle(UART_1), UART_FLAG_RXNE) == SET)
	{
		/* Receive command */
		UART_ReceiveCommand(UART_1);

		/* Clear RX buffer not empty flag */
		__HAL_UART_CLEAR_FLAG(UART_GetHandle(UART_1), UART_FLAG_RXNE);
	}
}

void OTG_HS_EP1_OUT_IRQHandler(void)
{
	HAL_PCD_IRQHandler(USB_DATA_GetHandlePCD());
}

void OTG_HS_EP1_IN_IRQHandler(void)
{
	HAL_PCD_IRQHandler(USB_DATA_GetHandlePCD());
}

void OTG_HS_IRQHandler(void)
{
	if (__HAL_PCD_GET_FLAG(USB_DATA_GetHandlePCD(), USB_OTG_GINTSTS_IEPINT))
	{
		/* Nothing needed here yet */
	}

	HAL_PCD_IRQHandler(USB_DATA_GetHandlePCD());
}

void EXTI2_IRQHandler(void)
{
	if (__HAL_GPIO_EXTI_GET_IT(BUTTON1_Pin) != 0x00u)
	{
		/* Disable interrupt */
		HAL_NVIC_DisableIRQ(EXTI2_IRQn);

		/* Assign change of channel configuration */
		MAIN_ChangeChannelConfiguration(CHANNEL_1);

		/* Clear flag */
		__HAL_GPIO_EXTI_CLEAR_IT(BUTTON1_Pin);
	}
}

void EXTI3_IRQHandler(void)
{
	if (__HAL_GPIO_EXTI_GET_IT(BUTTON2_Pin) != 0x00u)
	{
		/* Disable interrupt */
		HAL_NVIC_DisableIRQ(EXTI3_IRQn);

		/* Assign change of channel configuration */
		MAIN_ChangeChannelConfiguration(CHANNEL_2);

		/* Clear flag */
		__HAL_GPIO_EXTI_CLEAR_IT(BUTTON2_Pin);
	}
}

