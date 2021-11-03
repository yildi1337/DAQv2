/*
 *******************************************************************************
 * File:            button.h
 * Date:            2021-10-20
 * Author:          Phillip Durdaut
 * Summary:         Functions for controlling the buttons
 * Notes:           -
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BUTTON_H
#define __BUTTON_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Public typedefs -----------------------------------------------------------*/
typedef enum
{
	BUTTON_1,
	BUTTON_2
} BUTTON_t;

/* Public defines ------------------------------------------------------------*/
/* Public macros -------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
extern void BUTTON_Init(void);

#endif /* __BUTTON_H */
