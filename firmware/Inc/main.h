/*
 *******************************************************************************
 * File:            main.h
 * Date:            2020-11-02
 * Author:          Phillip Durdaut
 * Summary:         Main include file with main program definitions
 * Notes:           -
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <float.h>
#include <ctype.h>

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_it.h"

/* Public defines ------------------------------------------------------------*/

/* ADC */
#define MAIN_ADC_SAMPLES_PER_SECOND                     256e3
#define MAIN_ADC_SAMPLES_PER_MILLI_SECOND               256
#define MAIN_ADC_NUM_CHANNELS                           4
#define MAIN_ADC_BYTES_PER_SAMPLE_PER_CHANNEL           4
#define MAIN_ADC_BYTES_PER_SAMPLE                       (MAIN_ADC_NUM_CHANNELS * MAIN_ADC_BYTES_PER_SAMPLE_PER_CHANNEL)
#define MAIN_ADC_BYTES_PER_SECOND_PER_CHANNEL           (MAIN_ADC_BYTES_PER_SAMPLE_PER_CHANNEL * MAIN_ADC_SAMPLES_PER_SECOND)
#define MAIN_ADC_BYTES_PER_SECOND                       (MAIN_ADC_NUM_CHANNELS * MAIN_ADC_BYTES_PER_SAMPLE_PER_CHANNEL * MAIN_ADC_SAMPLES_PER_SECOND)
#define MAIN_ADC_BYTES_PER_MILLI_SECOND_PER_CHANNEL     (MAIN_ADC_BYTES_PER_SAMPLE_PER_CHANNEL * MAIN_ADC_SAMPLES_PER_MILLI_SECOND)
#define MAIN_ADC_BYTES_PER_MILLI_SECOND                 (MAIN_ADC_NUM_CHANNELS * MAIN_ADC_BYTES_PER_SAMPLE_PER_CHANNEL * MAIN_ADC_SAMPLES_PER_MILLI_SECOND)

/* USB */
#define MAIN_USB_VENDOR_ID                              0x1337
#define MAIN_USB_PRODUCT_ID                             0x3030
#define MAIN_USB_LANG_ID_STRING_US                      0x0409
#define MAIN_USB_LANG_ID_STRING_DE                      0x0407
#define MAIN_USB_LANG_ID_STRING                         MAIN_USB_LANG_ID_STRING_US
#define MAIN_USB_MANUFACTURER_STRING                    "pd"
#define MAIN_USB_PRODUCT_STRING                         "DAQ v2"
#define MAIN_USB_SERIAL_STRING                          "0000"
#define MAIN_USB_CONFIGURATION_STRING                   "DAQ v2 Config"
#define MAIN_USB_INTERFACE_STRING                       "DAQ v2 Interface"
#define MAIN_USB_DEVICE_VERSION                         0x0100

/* Interrupt priorities */
#define IRQ_UART1_PREEMPT_PRIORITY              2
#define IRQ_UART1_SUB_PRIORITY                  2
#define IRQ_DMA1_STREAM0_PREEMPT_PRIORITY       10
#define IRQ_DMA1_STREAM0_SUB_PRIORITY           10
#define IRQ_OTG_HS_EP1_OUT_PREEMPT_PRIORITY     5
#define IRQ_OTG_HS_EP1_OUT_SUB_PRIORITY         5
#define IRQ_OTG_HS_EP1_IN_PREEMPT_PRIORITY      5
#define IRQ_OTG_HS_EP1_IN_SUB_PRIORITY          5
#define IRQ_OTG_HS_PREEMPT_PRIORITY             5
#define IRQ_OTG_HS_SUB_PRIORITY                 5

/* Pins and ports */
#define DEBUG_1_Pin                             GPIO_PIN_10
#define DEBUG_1_Port                            GPIOE
#define DEBUG_2_Pin                             GPIO_PIN_11
#define DEBUG_2_Port                            GPIOE
#define DEBUG_3_Pin                             GPIO_PIN_12
#define DEBUG_3_Port                            GPIOE
#define DEBUG_4_Pin                             GPIO_PIN_13
#define DEBUG_4_Port                            GPIOE

#define UART1_TX_Pin                            GPIO_PIN_6
#define UART1_TX_Port                           GPIOB
#define UART1_TX_AF                             GPIO_AF7_USART1
#define UART1_RX_Pin                            GPIO_PIN_7
#define UART1_RX_Port                           GPIOB
#define UART1_RX_AF                             GPIO_AF7_USART1

#define SPI3_SCK_Pin                            GPIO_PIN_10
#define SPI3_SCK_Port                           GPIOC
#define SPI3_SCK_AF                             GPIO_AF6_SPI3
#define SPI3_MISO_Pin                           GPIO_PIN_11
#define SPI3_MISO_Port                          GPIOC
#define SPI3_MISO_AF                            GPIO_AF6_SPI3
#define SPI3_MOSI_Pin                           GPIO_PIN_6
#define SPI3_MOSI_Port                          GPIOD
#define SPI3_MOSI_AF                            GPIO_AF5_SPI3

#define BUTTON1_Pin                             GPIO_PIN_2
#define BUTTON1_Port                            GPIOG
#define BUTTON2_Pin                             GPIO_PIN_3
#define BUTTON2_Port                            GPIOG

#define RD_NRESET_Pin                           GPIO_PIN_6
#define RD_NRESET_Port                          GPIOG
#define RD_NCS_Pin                              GPIO_PIN_7
#define RD_NCS_Port                             GPIOG

#define DAC_NLDAC_Pin                           GPIO_PIN_9
#define DAC_NLDAC_Port                          GPIOA
#define DAC_NCS_Pin                             GPIO_PIN_10
#define DAC_NCS_Port                            GPIOA

#define ADC_SPI_NRESET_Pin                      GPIO_PIN_0
#define ADC_SPI_NRESET_Port                     GPIOD
#define ADC_SPI_NCS_Pin                         GPIO_PIN_12
#define ADC_SPI_NCS_Port                        GPIOC

#define SPI2_NSS_Pin                            GPIO_PIN_9
#define SPI2_NSS_Port                           GPIOB
#define SPI2_NSS_AF                             GPIO_AF5_SPI2
#define SPI2_SCK_Pin                            GPIO_PIN_3
#define SPI2_SCK_Port                           GPIOD
#define SPI2_SCK_AF                             GPIO_AF5_SPI2
#define SPI2_MOSI_Pin                           GPIO_PIN_15
#define SPI2_MOSI_Port                          GPIOB
#define SPI2_MOSI_AF                            GPIO_AF5_SPI2

#define USB_OTG_HS_ULPI_STP_Pin                 GPIO_PIN_0
#define USB_OTG_HS_ULPI_STP_Port                GPIOC
#define USB_OTG_HS_ULPI_STP_AF                  GPIO_AF10_OTG2_HS
#define USB_OTG_HS_ULPI_DIR_Pin                 GPIO_PIN_2
#define USB_OTG_HS_ULPI_DIR_Port                GPIOC
#define USB_OTG_HS_ULPI_DIR_AF                  GPIO_AF10_OTG2_HS
#define USB_OTG_HS_ULPI_NXT_Pin                 GPIO_PIN_3
#define USB_OTG_HS_ULPI_NXT_Port                GPIOC
#define USB_OTG_HS_ULPI_NXT_AF                  GPIO_AF10_OTG2_HS
#define USB_OTG_HS_ULPI_CK_Pin                  GPIO_PIN_5
#define USB_OTG_HS_ULPI_CK_Port                 GPIOA
#define USB_OTG_HS_ULPI_CK_AF                   GPIO_AF10_OTG2_HS
#define USB_OTG_HS_ULPI_D0_Pin                  GPIO_PIN_3
#define USB_OTG_HS_ULPI_D0_Port                 GPIOA
#define USB_OTG_HS_ULPI_D0_AF                   GPIO_AF10_OTG2_HS
#define USB_OTG_HS_ULPI_D1_Pin                  GPIO_PIN_0
#define USB_OTG_HS_ULPI_D1_Port                 GPIOB
#define USB_OTG_HS_ULPI_D1_AF                   GPIO_AF10_OTG2_HS
#define USB_OTG_HS_ULPI_D2_Pin                  GPIO_PIN_1
#define USB_OTG_HS_ULPI_D2_Port                 GPIOB
#define USB_OTG_HS_ULPI_D2_AF                   GPIO_AF10_OTG2_HS
#define USB_OTG_HS_ULPI_D3_Pin                  GPIO_PIN_10
#define USB_OTG_HS_ULPI_D3_Port                 GPIOB
#define USB_OTG_HS_ULPI_D3_AF                   GPIO_AF10_OTG2_HS
#define USB_OTG_HS_ULPI_D4_Pin                  GPIO_PIN_11
#define USB_OTG_HS_ULPI_D4_Port                 GPIOB
#define USB_OTG_HS_ULPI_D4_AF                   GPIO_AF10_OTG2_HS
#define USB_OTG_HS_ULPI_D5_Pin                  GPIO_PIN_12
#define USB_OTG_HS_ULPI_D5_Port                 GPIOB
#define USB_OTG_HS_ULPI_D5_AF                   GPIO_AF10_OTG2_HS
#define USB_OTG_HS_ULPI_D6_Pin                  GPIO_PIN_13
#define USB_OTG_HS_ULPI_D6_Port                 GPIOB
#define USB_OTG_HS_ULPI_D6_AF                   GPIO_AF10_OTG2_HS
#define USB_OTG_HS_ULPI_D7_Pin                  GPIO_PIN_5
#define USB_OTG_HS_ULPI_D7_Port                 GPIOB
#define USB_OTG_HS_ULPI_D7_AF                   GPIO_AF10_OTG2_HS

#define I2C4_SCL_Port                           GPIOD
#define I2C4_SCL_Pin                            GPIO_PIN_12
#define I2C4_SCL_AF                             GPIO_AF4_I2C4
#define I2C4_SDA_Port                           GPIOD
#define I2C4_SDA_Pin                            GPIO_PIN_13
#define I2C4_SDA_AF                             GPIO_AF4_I2C4

/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
typedef uint8_t flag_t;
typedef enum
{
	CHANNEL_1 = 0,
	CHANNEL_2 = 1,
	CHANNEL_NONE
} channel_t;
typedef enum
{
	CHANNEL_STATE_1_INPUTCOUPLING_DC_INPUTTYPE_S,
	CHANNEL_STATE_2_INPUTCOUPLING_DC_INPUTTYPE_D,
	CHANNEL_STATE_3_INPUTCOUPLING_AC_INPUTTYPE_S,
	CHANNEL_STATE_4_INPUTCOUPLING_AC_INPUTTYPE_D
} channel_state_t;

/* Public variables ----------------------------------------------------------*/
extern flag_t MAIN_MeasurementRunning;
extern flag_t MAIN_SamplesRequested;

/* Public function prototypes ------------------------------------------------*/
extern void Error_Handler(void);
extern void MAIN_ChangeChannelConfiguration(channel_t channel);

#endif /* __MAIN_H */
