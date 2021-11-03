/*
 *******************************************************************************
 * File:			lcd.c
 * Date:			2021-10-20
 * Author:			Phillip Durdaut
 * Summary:			Functions for controlling the 16x2 LCD (HD44780) over I2C
 * Notes:			This driver is specifically designed for the combination of
 * 					16x2 LCD (HD44780) driven by an I2C 8-bit I/O expander
 * 					(PCF8574). Here the adapter board from AZ-Delivery
 * 					(https://www.az-delivery.de/products/serielle-schnittstelle
 * 					?variant=27476225289) is used, which uses the following pin
 * 					interconnection (HD44780 4-bit mode):
 *
 * 					PCF8574			LCD
 * 					P7				D7
 * 					P6				D6
 * 					P5				D5
 * 					P4				D4
 *					P3				"Backlight"
 *					P2				E
 *					P1				RW
 * 					P0				RS
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lcd.h"
#include "i2c.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define LCD_I2C_SLAVE_ADDRESS	0x4E

/* Private macros ------------------------------------------------------------*/
#define LCD_I2C_WRITE(x)		(0xFE & x)
#define LCD_I2C_READ(x)			(0x01 | x)

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void LCD_Send8BitCommandIn4BitMode(uint8_t cmd);
void LCD_Send8BitDataIn4BitMode(uint8_t data);

/* Program code --------------------------------------------------------------*/

extern void LCD_Init(void)
{
	HAL_Delay(50);							/* Wait for >15 ms */
	LCD_Send8BitCommandIn4BitMode(0x30);	/* Initial sequence */
	HAL_Delay(10);							/* Wait for >4.1 ms for writing DATA into RAM */
	LCD_Send8BitCommandIn4BitMode(0x30);	/* Initial sequence */
	HAL_Delay(10);
	LCD_Send8BitCommandIn4BitMode(0x30);	/* Initial sequence */
	HAL_Delay(10);
	LCD_Send8BitCommandIn4BitMode(0x20);	/* Set 4-bit mode */
	HAL_Delay(10);
	LCD_Send8BitCommandIn4BitMode(0x28);	/* Set 4-bit mode, 2 lines, 5x8 dots */
	HAL_Delay(10);
	LCD_Send8BitCommandIn4BitMode(0x08);	/* Set display off */
	HAL_Delay(10);
	LCD_Send8BitCommandIn4BitMode(0x01);	/* Clear display */
	HAL_Delay(10);
	LCD_Send8BitCommandIn4BitMode(0x06);	/* Set entry mode */
	HAL_Delay(10);
	LCD_Send8BitCommandIn4BitMode(0x0C);	/* Set display on */
	HAL_Delay(10);
}

extern void LCD_Clear(void)
{
	LCD_SetCursor(LCD_ROW_0, 0);
	LCD_SendString("                ");

	LCD_SetCursor(LCD_ROW_1, 0);
	LCD_SendString("                ");
}

extern void LCD_SetCursor(LCD_ROW_t row, int col)
{
    switch(row)
    {
        case LCD_ROW_0:
            col |= 0x80;
            break;
        case LCD_ROW_1:
            col |= 0xC0;
            break;
    }

   LCD_Send8BitCommandIn4BitMode(col);
}

extern void LCD_SendString(char * pStr)
{
	while(*pStr)
		LCD_Send8BitDataIn4BitMode(*pStr++);
}

void LCD_Send8BitCommandIn4BitMode(uint8_t cmd)
{
	uint8_t data_tx[4];

	data_tx[0] = ((cmd << 0) & 0xF0) | 0x08 | 0x04;  /* Backlight = 1, E = 1, RS = 0 */
	data_tx[1] = ((cmd << 0) & 0xF0) | 0x08 | 0x00;  /* Backlight = 1, E = 0, RS = 0 */
	data_tx[2] = ((cmd << 4) & 0xF0) | 0x08 | 0x04;  /* Backlight = 1, E = 1, RS = 0 */
	data_tx[3] = ((cmd << 4) & 0xF0) | 0x08 | 0x00;  /* Backlight = 1, E = 0, RS = 0 */

	HAL_I2C_Master_Transmit(I2C_GetHandle(I2C_4), LCD_I2C_WRITE(LCD_I2C_SLAVE_ADDRESS), (uint8_t *)data_tx, 4, 100);
}

void LCD_Send8BitDataIn4BitMode(uint8_t data)
{
	uint8_t data_tx[4];

	data_tx[0] = ((data << 0) & 0xF0) | 0x08 | 0x04 | 0x01;  /* Backlight = 1, E = 1, RS = 1 */
	data_tx[1] = ((data << 0) & 0xF0) | 0x08 | 0x00 | 0x01;  /* Backlight = 1, E = 0, RS = 1 */
	data_tx[2] = ((data << 4) & 0xF0) | 0x08 | 0x04 | 0x01;  /* Backlight = 1, E = 1, RS = 1 */
	data_tx[3] = ((data << 4) & 0xF0) | 0x08 | 0x00 | 0x01;  /* Backlight = 1, E = 0, RS = 1 */

	HAL_I2C_Master_Transmit(I2C_GetHandle(I2C_4), LCD_I2C_WRITE(LCD_I2C_SLAVE_ADDRESS), (uint8_t *)data_tx, 4, 100);
}
