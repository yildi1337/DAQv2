/*
 *******************************************************************************
 * File:			max4820.h
 * Date:			2021-10-20
 * Author:			Phillip Durdaut
 * Summary:			Functions for controlling the MAX4820 relay driver
 * Notes:			-
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAX4820_H
#define __MAX4820_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Public defines ------------------------------------------------------------*/
/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
typedef enum
{
	MAX4820_CHANNEL_1 = 0,
	MAX4820_CHANNEL_2 = 1
} MAX4820_CHANNEL_t;

typedef enum
{
	MAX4820_INPUT_COUPLING_DC,
	MAX4820_INPUT_COUPLING_AC,
	MAX4820_INPUT_COUPLING_UNDEFINED
} MAX4820_INPUT_COUPLING_t;

typedef enum
{
	MAX4820_INPUT_TYPE_S,
	MAX4820_INPUT_TYPE_D,
	MAX4820_INPUT_TYPE_UNDEFINED
} MAX4820_INPUT_TYPE_t;

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
extern void MAX4820_Init(void);
extern MAX4820_INPUT_COUPLING_t MAX4820_GetInputCoupling(MAX4820_CHANNEL_t channel);
extern MAX4820_INPUT_TYPE_t MAX4820_GetInputType(MAX4820_CHANNEL_t channel);
extern uint8_t * MAX4820_GetInputCouplingString(MAX4820_CHANNEL_t channel);
extern uint8_t * MAX4820_GetInputTypeString(MAX4820_CHANNEL_t channel);
extern void MAX4820_SetInputCoupling(MAX4820_CHANNEL_t channel, MAX4820_INPUT_COUPLING_t inputCoupling);
extern void MAX4820_SetInputType(MAX4820_CHANNEL_t channel, MAX4820_INPUT_TYPE_t inputType);

#endif /* __MAX4820_H */
