/*
 *******************************************************************************
 * File:            spi.h
 * Date:            2020-11-03
 * Author:          Phillip Durdaut
 * Summary:         Functions for SPI communication handling
 * Notes:           -
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H
#define __SPI_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Public defines ------------------------------------------------------------*/
/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
typedef enum
{
	SPI_2,
	SPI_3
} SPI_t;

typedef enum
{
	SPI_DATA_SIZE_8BIT,
	SPI_DATA_SIZE_16BIT,
	SPI_DATA_SIZE_32BIT
} SPI_DATA_SIZE_t;

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
extern void SPI_Init(SPI_t spi, SPI_DATA_SIZE_t dataSize);
extern uint8_t SPI_Read_8bit(SPI_t spi);
extern uint16_t SPI_Read_16bit(SPI_t spi);
extern void SPI_Write_8bit(SPI_t spi, uint8_t data);
extern void SPI_Write_16bit(SPI_t spi, uint16_t data);
extern SPI_HandleTypeDef * SPI_GetHandle(SPI_t spi);

#endif /* __SPI_H */
