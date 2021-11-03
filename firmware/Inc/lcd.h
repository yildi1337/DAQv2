/*
 *******************************************************************************
 * File:            lcd.h
 * Date:            2021-10-20
 * Author:          Phillip Durdaut
 * Summary:         Functions for controlling the 16x2 LCD (HD44780) over I2C
 * Notes:           This driver is specifically designed for the combination of
 *                  16x2 LCD (HD44780) driven by an I2C 8-bit I/O expander
 *                  (PCF8574). Here the adapter board from AZ-Delivery
 *                  (https://www.az-delivery.de/products/serielle-schnittstelle
 *                  ?variant=27476225289) is used, which uses the following pin
 *                  interconnection (HD44780 4-bit mode):
 *
 *                  PCF8574         LCD
 *                  P7              D7
 *                  P6              D6
 *                  P5              D5
 *                  P4              D4
 *                  P3              "Backlight"
 *                  P2              E
 *                  P1              RW
 *                  P0              RS
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Public defines ------------------------------------------------------------*/
/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
typedef enum
{
	LCD_ROW_0 = 0,
	LCD_ROW_1 = 1
} LCD_ROW_t;

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
extern void LCD_Init(void);
extern void LCD_Clear(void);
extern void LCD_SetCursor(LCD_ROW_t row, int col);
extern void LCD_SendString(char * pStr);

#endif /* __LCD_H */
