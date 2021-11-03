/*
 *******************************************************************************
 * File:            vga.h
 * Date:            2020-11-07
 * Author:          Phillip Durdaut
 * Summary:         Functions for controlling the VGAs
 * Notes:           -
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VGA_H
#define __VGA_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Public defines ------------------------------------------------------------*/
/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
typedef enum
{
	VGA_1 = 0,
	VGA_2 = 1
} VGA_t;

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
extern void VGA_Init(void);
extern void VGA_SetGain(VGA_t vga, float gain);
extern float VGA_GetGain(VGA_t vga);

#endif /* __VGA_H */
