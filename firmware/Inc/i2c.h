/*
 *******************************************************************************
 * File:            i2c.h
 * Date:            2020-12-10
 * Author:          Phillip Durdaut
 * Summary:         Functions for I2C communication handling
 * Notes:           -
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Public defines ------------------------------------------------------------*/
/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
typedef enum
{
	I2C_4
} I2C_t;

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
extern void I2C_Init(I2C_t i2c);
extern I2C_HandleTypeDef * I2C_GetHandle(I2C_t i2c);

#endif /* __I2C_H */
