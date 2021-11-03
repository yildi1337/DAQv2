/*
 *******************************************************************************
 * File:            stm32h7xx_it.h
 * Date:            2020-11-03
 * Author:          Phillip Durdaut
 * Summary:         Interrupt service routines
 * Notes:           -
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32H7xx_IT_H
#define __STM32H7xx_IT_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Public defines ------------------------------------------------------------*/
/* Public macros -------------------------------------------------------------*/
/* Public typedefs -----------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void DMA1_Stream0_IRQHandler(void);
void SPI2_IRQHandler(void);
void USART1_IRQHandler(void);
void OTG_HS_EP1_OUT_IRQHandler(void);
void OTG_HS_EP1_IN_IRQHandler(void);
void OTG_HS_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);


#endif /* __STM32H7xx_IT_H */
