/*
 *******************************************************************************
 * File:			button.c
 * Date:			2021-10-20
 * Author:			Phillip Durdaut
 * Summary:			Functions for controlling the buttons
 * Notes:			-
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "button.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Program code --------------------------------------------------------------*/

extern void BUTTON_Init(void)
{
	/* Configure GPIO pins */
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	GPIO_InitStruct.Pin = BUTTON1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(BUTTON1_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = BUTTON2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(BUTTON2_Port, &GPIO_InitStruct);

	/* Configure EXTI interrupts */
	HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
	HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);

	/* Enable interrupts */
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}
