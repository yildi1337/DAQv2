/*
 *******************************************************************************
 * File:			uart.c
 * Date:			2020-11-03
 * Author:			Phillip Durdaut
 * Summary:			Functions for UART communication handling
 * Notes:			-
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uart.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef UART_HandleUART1;
uint8_t UART_RXBufferUART1[UART_UART1_BUFFER_SIZE_BYTES] = { 0 };
uint8_t UART_RXBufferIndexUART1 = 0;
UART_UART1_RECEIVED_COMMAND_t UART_ReceivedCommandUART1;

/* Private function prototypes -----------------------------------------------*/

/* Program code --------------------------------------------------------------*/

extern void UART_Init(UART_t uart)
{
	if (uart == UART_1)
	{
		/* Reset status in received command struct */
		UART_ReceivedCommandUART1.available = false;
		UART_ReceivedCommandUART1.acknowledged = false;

		/* Enable clock */
		__HAL_RCC_USART1_CLK_ENABLE();

		/* Configure GPIO pins */
		GPIO_InitTypeDef GPIO_InitStruct = { 0 };

		GPIO_InitStruct.Pin = UART1_TX_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = UART1_TX_AF;
		HAL_GPIO_Init(UART1_TX_Port, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = UART1_RX_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = UART1_RX_AF;
		HAL_GPIO_Init(UART1_RX_Port, &GPIO_InitStruct);

		/* Configure UART */
		UART_HandleUART1.Instance = USART1;
		UART_HandleUART1.Init.BaudRate = 115200;
		UART_HandleUART1.Init.WordLength = UART_WORDLENGTH_8B;
		UART_HandleUART1.Init.StopBits = UART_STOPBITS_1;
		UART_HandleUART1.Init.Parity = UART_PARITY_NONE;
		UART_HandleUART1.Init.Mode = UART_MODE_TX_RX;
		UART_HandleUART1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		UART_HandleUART1.Init.OverSampling = UART_OVERSAMPLING_16;
		UART_HandleUART1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
		UART_HandleUART1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
		UART_HandleUART1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_AUTOBAUDRATE_INIT;
		UART_HandleUART1.AdvancedInit.AutoBaudRateEnable = UART_ADVFEATURE_AUTOBAUDRATE_ENABLE;
		UART_HandleUART1.AdvancedInit.AutoBaudRateMode = UART_ADVFEATURE_AUTOBAUDRATE_ONSTARTBIT;

		if (HAL_UART_Init(&UART_HandleUART1) != HAL_OK)
		{
			Error_Handler();
		}

		if (HAL_UARTEx_SetTxFifoThreshold(&UART_HandleUART1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
		{
			Error_Handler();
		}

		if (HAL_UARTEx_SetRxFifoThreshold(&UART_HandleUART1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
		{
			Error_Handler();
		}

		if (HAL_UARTEx_DisableFifoMode(&UART_HandleUART1) != HAL_OK)
		{
			Error_Handler();
		}

		/* Enable receive data register not empty interrupt */
		UART_EnableRXInterrupt(UART_1);

		/* Configure interrupts */
		HAL_NVIC_SetPriority(USART1_IRQn, IRQ_UART1_PREEMPT_PRIORITY, IRQ_UART1_SUB_PRIORITY);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
}

extern UART_HandleTypeDef * UART_GetHandle(UART_t uart)
{
	UART_HandleTypeDef * pHandle;

	if (uart == UART_1)
	{
		pHandle = &UART_HandleUART1;
	}

	return pHandle;
}

extern void UART_EnableRXInterrupt(UART_t uart)
{
	if (uart == UART_1)
	{
		__HAL_UART_ENABLE_IT(&UART_HandleUART1, UART_IT_RXNE);
	}
}

extern void UART_DisableRXInterrupt(UART_t uart)
{
	if (uart == UART_1)
	{
		__HAL_UART_DISABLE_IT(&UART_HandleUART1, UART_IT_RXNE);
	}
}

extern void UART_TransmitData(UART_t uart, uint8_t * pBuffer, uint32_t numberOfBytes)
{
	if (uart == UART_1)
	{
		HAL_UART_Transmit(&UART_HandleUART1, pBuffer, numberOfBytes, 1000);
	}
}

extern void UART_ReceiveCommand(UART_t uart)
{
	if (uart == UART_1)
	{
		/* Receive one byte */
		uint8_t byte;
		HAL_UART_Receive(&UART_HandleUART1, &byte, 1, 10);

		/* Drop received byte if last command not acknowledged yet */
		if (UART_ReceivedCommandUART1.available == true && UART_ReceivedCommandUART1.acknowledged == false)
			return;

		/* Drop received byte if not a valid character (0-9, ;, _, -,  , ., A-Z) */
		if ( !((byte >= 48 && byte <= 57) || byte == ';' || byte == '_' || byte == '-' || byte == ' ' || byte == '.' || (byte >= 65 && byte <= 90)) )
			return;

		/* Append byte to buffer */
		UART_RXBufferUART1[UART_RXBufferIndexUART1] = byte;
		UART_RXBufferIndexUART1++;

		/* Reached end of command? */
		if (byte == ';')
		{
			/* Save the length of the received command and reset RX buffer index */
			UART_ReceivedCommandUART1.length = UART_RXBufferIndexUART1;
			UART_RXBufferIndexUART1 = 0;

			/* Copy RX buffer to received command buffer */
			for (uint8_t i = 0; i < UART_ReceivedCommandUART1.length; i++)
				UART_ReceivedCommandUART1.command[i] = UART_RXBufferUART1[i];

			/* Received command available now but not yet acknowledged */
			UART_ReceivedCommandUART1.available = true;
			UART_ReceivedCommandUART1.acknowledged = false;
		}
	}
}

extern UART_UART1_RECEIVED_COMMAND_t * GetReferenceOnReceivedCommandStruct(UART_t uart)
{
	return &UART_ReceivedCommandUART1;
}
