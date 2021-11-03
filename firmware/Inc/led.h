/*
 *******************************************************************************
 * File:            led.h
 * Date:            2020-11-02
 * Author:          Phillip Durdaut
 * Summary:         Functions for controlling the board's (yellow debug) LEDs
 * Notes:           -
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Public typedefs -----------------------------------------------------------*/
typedef enum
{
	LED_1,
	LED_2,
	LED_3,
	LED_4
} LED_t;

/* Public defines ------------------------------------------------------------*/
/* Public macros -------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
extern void LED_Init(void);
extern void LED_On(LED_t led);
extern void LED_Off(LED_t led);
extern void LED_Toggle(LED_t led);

#endif /* __LED_H */
