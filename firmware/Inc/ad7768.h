/*
 *******************************************************************************
 * File:            ad7768.h
 * Date:            2020-11-08
 * Author:          Phillip Durdaut
 * Summary:         Functions for controlling the AD7768(-4BSTZ) ADC
 * Notes:           Mode B registers not used
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AD7768_H
#define __AD7768_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Public defines ------------------------------------------------------------*/
/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
typedef enum
{
	AD7768_CHANNEL_0,
	AD7768_CHANNEL_1,
	AD7768_CHANNEL_2,
	AD7768_CHANNEL_3
} AD7768_CHANNEL_t;

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
extern void AD7768_Init(void);
extern void AD7768_Reset(void);
extern uint8_t AD7768_GetRevisionID(void);

#endif /* __AD7768_H */
