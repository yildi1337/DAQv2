/*
 *******************************************************************************
 * File:			main.c
 * Date:			2021-10-20
 * Author:			Phillip Durdaut
 * Summary:			Main program file
 * Notes:			-
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "led.h"
#include "button.h"
#include "uart.h"
#include "usb_debug.h"
#include "max4820.h"
#include "spi.h"
#include "i2c.h"
#include "lcd.h"
#include "mcp4822.h"
#include "vga.h"
#include "ad7768.h"
#include "dma.h"
#include "usb_data.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define MAIN_MOVING_LIGHT_ENABLE			true
#define MAIN_MOVING_LIGHT_DELAY_MS			50

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
channel_state_t MAIN_ChannelState[] = {
		CHANNEL_STATE_3_INPUTCOUPLING_AC_INPUTTYPE_S,
		CHANNEL_STATE_3_INPUTCOUPLING_AC_INPUTTYPE_S
};
flag_t MAIN_ChangeChannelConfigurationRequested[] = {
		false,
		false
};
channel_t MAIN_ChannelConfigurationToChange = CHANNEL_NONE;

/* Public variables ----------------------------------------------------------*/
flag_t MAIN_MeasurementRunning = false;
flag_t MAIN_SamplesRequested = false;

/* Private function prototypes -----------------------------------------------*/
int main(void);
void MAIN_InitClocks(void);

/* Program code --------------------------------------------------------------*/

int main(void)
{
	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick */
	HAL_Init();

	/* Configure clocks */
	MAIN_InitClocks();


	/* Initialize LEDs */
	LED_Init();

	/* Initialize buttons */
	BUTTON_Init();


	/* Initialize USART module */
	UART_Init(UART_1);

	/* Initialize USB_DEBUG module */
	USB_DEBUG_Init();


	/* Initialize I2C4 for communicating with LCD */
	I2C_Init(I2C_4);

	/* Initialize LCD module */
	LCD_Init();
	LCD_Clear();
	LCD_SetCursor(LCD_ROW_0, 0);
	LCD_SendString("   |   |        ");
	LCD_SetCursor(LCD_ROW_1, 0);
	LCD_SendString("   |   |        ");


	/* Initialize MAX4820 relay driver */
	MAX4820_Init();
	MAX4820_SetInputCoupling(MAX4820_CHANNEL_1, MAX4820_INPUT_COUPLING_AC);
	MAX4820_SetInputCoupling(MAX4820_CHANNEL_2, MAX4820_INPUT_COUPLING_AC);
	MAX4820_SetInputType(MAX4820_CHANNEL_1, MAX4820_INPUT_TYPE_S);
	MAX4820_SetInputType(MAX4820_CHANNEL_2, MAX4820_INPUT_TYPE_S);


	/* Initialize MCP4822 DAC */
	MCP4822_Init();

	/* Initialize VGAs */
	VGA_Init();
	VGA_SetGain(VGA_1, 20);
	VGA_SetGain(VGA_2, 20);


	/* Initialize AD7768 ADC */
	AD7768_Init();

	/* Initialize circular DMA for receiving ADC data via SPI */
	DMA_Init(DMA_SPI2_RX_DMA1_STREAM0);

	/* Initialize SPI2 for receiving ADC data */
	SPI_Init(SPI_2, SPI_DATA_SIZE_8BIT);

	/* Initialize USB HS interface */
	USB_DATA_Init();

	/* Start receiving ADC data via SPI and circular DMA */
	HAL_SPI_Receive_DMA(SPI_GetHandle(SPI_2), (uint8_t *)DMA_SPI2_RX_DMA1_STREAM0_CircularBuffer, DMA_SPI2_RX_DMA1_STREAM0_CIRCULAR_BUFFER_SIZE_BYTES);


	/* Infinite loop -----------------------------------------------------------*/
	while (1)
	{
		if (MAIN_MOVING_LIGHT_ENABLE)
		{
			HAL_Delay(MAIN_MOVING_LIGHT_DELAY_MS);
			LED_Toggle(LED_1);
			HAL_Delay(MAIN_MOVING_LIGHT_DELAY_MS);
			LED_Toggle(LED_2);
			HAL_Delay(MAIN_MOVING_LIGHT_DELAY_MS);
			LED_Toggle(LED_3);
			HAL_Delay(MAIN_MOVING_LIGHT_DELAY_MS);
			LED_Toggle(LED_4);
		}

		if (MAIN_ChangeChannelConfigurationRequested[CHANNEL_1] || MAIN_ChangeChannelConfigurationRequested[CHANNEL_2])
		{
			if (MAIN_ChannelConfigurationToChange == CHANNEL_1 || MAIN_ChannelConfigurationToChange == CHANNEL_2)
			{
				switch(MAIN_ChannelState[MAIN_ChannelConfigurationToChange])
				{
					case CHANNEL_STATE_1_INPUTCOUPLING_DC_INPUTTYPE_S:
						MAX4820_SetInputCoupling((MAX4820_CHANNEL_t)MAIN_ChannelConfigurationToChange, MAX4820_INPUT_COUPLING_DC);
						MAX4820_SetInputType((MAX4820_CHANNEL_t)MAIN_ChannelConfigurationToChange, MAX4820_INPUT_TYPE_S);
						MAIN_ChannelState[MAIN_ChannelConfigurationToChange] = CHANNEL_STATE_2_INPUTCOUPLING_DC_INPUTTYPE_D;
						break;

					case CHANNEL_STATE_2_INPUTCOUPLING_DC_INPUTTYPE_D:
						MAX4820_SetInputCoupling((MAX4820_CHANNEL_t)MAIN_ChannelConfigurationToChange, MAX4820_INPUT_COUPLING_DC);
						MAX4820_SetInputType((MAX4820_CHANNEL_t)MAIN_ChannelConfigurationToChange, MAX4820_INPUT_TYPE_D);
						MAIN_ChannelState[MAIN_ChannelConfigurationToChange] = CHANNEL_STATE_3_INPUTCOUPLING_AC_INPUTTYPE_S;
						break;

					case CHANNEL_STATE_3_INPUTCOUPLING_AC_INPUTTYPE_S:
						MAX4820_SetInputCoupling((MAX4820_CHANNEL_t)MAIN_ChannelConfigurationToChange, MAX4820_INPUT_COUPLING_AC);
						MAX4820_SetInputType((MAX4820_CHANNEL_t)MAIN_ChannelConfigurationToChange, MAX4820_INPUT_TYPE_S);
						MAIN_ChannelState[MAIN_ChannelConfigurationToChange] = CHANNEL_STATE_4_INPUTCOUPLING_AC_INPUTTYPE_D;
						break;

					case CHANNEL_STATE_4_INPUTCOUPLING_AC_INPUTTYPE_D:
						MAX4820_SetInputCoupling((MAX4820_CHANNEL_t)MAIN_ChannelConfigurationToChange, MAX4820_INPUT_COUPLING_AC);
						MAX4820_SetInputType((MAX4820_CHANNEL_t)MAIN_ChannelConfigurationToChange, MAX4820_INPUT_TYPE_D);
						MAIN_ChannelState[MAIN_ChannelConfigurationToChange] = CHANNEL_STATE_1_INPUTCOUPLING_DC_INPUTTYPE_S;
						break;
				}

				MAIN_ChangeChannelConfigurationRequested[MAIN_ChannelConfigurationToChange] = false;

				/* Reenable interrupt after debounce delay */
				HAL_Delay(500);
				if (MAIN_ChannelConfigurationToChange == CHANNEL_1)
					HAL_NVIC_EnableIRQ(EXTI2_IRQn);
				if (MAIN_ChannelConfigurationToChange == CHANNEL_2)
					HAL_NVIC_EnableIRQ(EXTI3_IRQn);
			}
		}

		if (USB_DEBUG_CommandAvailable())
		{
			switch(USB_DEBUG_GetCommand())
			{
				case USB_DEBUG_COMMAND_SYSTEM_RESET:
					HAL_NVIC_SystemReset();
					break;

				case USB_DEBUG_COMMAND_GET_COMMANDS:
					USB_DEBUG_WelcomeScreen();
					break;

				case USB_DEBUG_COMMAND_GET_VOLTAGE_DAC1:
					USB_DEBUG_TransmitFloat(MCP4822_GetVoltageFloat(MCP4822_CHANNEL_A));
					break;
				case USB_DEBUG_COMMAND_GET_VOLTAGE_DAC2:
					USB_DEBUG_TransmitFloat(MCP4822_GetVoltageFloat(MCP4822_CHANNEL_B));
					break;

				case USB_DEBUG_COMMAND_GET_GAIN_VGA1:
					USB_DEBUG_TransmitFloat(VGA_GetGain(VGA_1));
					break;
				case USB_DEBUG_COMMAND_GET_GAIN_VGA2:
					USB_DEBUG_TransmitFloat(VGA_GetGain(VGA_2));
					break;

				case USB_DEBUG_COMMAND_SET_GAIN_VGA1:
					VGA_SetGain(VGA_1, USB_DEBUG_GetParameter());
					USB_DEBUG_TransmitString((uint8_t *)"OK\n");
					break;
				case USB_DEBUG_COMMAND_SET_GAIN_VGA2:
					VGA_SetGain(VGA_2, USB_DEBUG_GetParameter());
					USB_DEBUG_TransmitString((uint8_t *)"OK\n");
					break;

				case USB_DEBUG_COMMAND_GET_INPUT_COUPL_CH1:
					USB_DEBUG_TransmitString((uint8_t *)MAX4820_GetInputCouplingString(MAX4820_CHANNEL_1));
					break;
				case USB_DEBUG_COMMAND_GET_INPUT_COUPL_CH2:
					USB_DEBUG_TransmitString((uint8_t *)MAX4820_GetInputCouplingString(MAX4820_CHANNEL_2));
					break;

				case USB_DEBUG_COMMAND_SET_INPUT_COUPL_CH1_AC:
					MAX4820_SetInputCoupling(MAX4820_CHANNEL_1, MAX4820_INPUT_COUPLING_AC);
					USB_DEBUG_TransmitString((uint8_t *)"OK\n");
					break;
				case USB_DEBUG_COMMAND_SET_INPUT_COUPL_CH1_DC:
					MAX4820_SetInputCoupling(MAX4820_CHANNEL_1, MAX4820_INPUT_COUPLING_DC);
					USB_DEBUG_TransmitString((uint8_t *)"OK\n");
					break;
				case USB_DEBUG_COMMAND_SET_INPUT_COUPL_CH2_AC:
					MAX4820_SetInputCoupling(MAX4820_CHANNEL_2, MAX4820_INPUT_COUPLING_AC);
					USB_DEBUG_TransmitString((uint8_t *)"OK\n");
					break;
				case USB_DEBUG_COMMAND_SET_INPUT_COUPL_CH2_DC:
					MAX4820_SetInputCoupling(MAX4820_CHANNEL_2, MAX4820_INPUT_COUPLING_DC);
					USB_DEBUG_TransmitString((uint8_t *)"OK\n");
					break;

				case USB_DEBUG_COMMAND_GET_INPUT_TYPE_CH1:
					USB_DEBUG_TransmitString((uint8_t *)MAX4820_GetInputTypeString(MAX4820_CHANNEL_1));
					break;
				case USB_DEBUG_COMMAND_GET_INPUT_TYPE_CH2:
					USB_DEBUG_TransmitString((uint8_t *)MAX4820_GetInputTypeString(MAX4820_CHANNEL_2));
					break;

				case USB_DEBUG_COMMAND_SET_INPUT_TYPE_CH1_S:
					MAX4820_SetInputType(MAX4820_CHANNEL_1, MAX4820_INPUT_TYPE_S);
					USB_DEBUG_TransmitString((uint8_t *)"OK\n");
					break;
				case USB_DEBUG_COMMAND_SET_INPUT_TYPE_CH1_D:
					MAX4820_SetInputType(MAX4820_CHANNEL_1, MAX4820_INPUT_TYPE_D);
					USB_DEBUG_TransmitString((uint8_t *)"OK\n");
					break;
				case USB_DEBUG_COMMAND_SET_INPUT_TYPE_CH2_S:
					MAX4820_SetInputType(MAX4820_CHANNEL_2, MAX4820_INPUT_TYPE_S);
					USB_DEBUG_TransmitString((uint8_t *)"OK\n");
					break;
				case USB_DEBUG_COMMAND_SET_INPUT_TYPE_CH2_D:
					MAX4820_SetInputType(MAX4820_CHANNEL_2, MAX4820_INPUT_TYPE_D);
					USB_DEBUG_TransmitString((uint8_t *)"OK\n");
					break;

				default:
					break;
			}

			USB_DEBUG_AcknowledgeCommand();
		}
	}

	/* Never reached (end of program) ------------------------------------------*/
	return 0;
}

void MAIN_InitClocks(void)
{
	/* The LDO regulator supplies the Vcore power domains. The SMPS regulator is bypassed. */
	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

	/* Configure the main internal regulator output voltage */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

	/* Wait for voltage scaling (VOS) being finished */
	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY));

	/* Initializes the RCC oscillators */
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 5;
	RCC_OscInitStruct.PLL.PLLN = 192;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/* Initializes the CPU, AHB and APB buses clocks */
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |
								  RCC_CLOCKTYPE_SYSCLK |
								  RCC_CLOCKTYPE_PCLK1 |
								  RCC_CLOCKTYPE_PCLK2 |
								  RCC_CLOCKTYPE_D3PCLK1 |
								  RCC_CLOCKTYPE_D1PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}

	/* Initialize peripheral clocks */
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_SPI2 | RCC_PERIPHCLK_SPI3 | RCC_PERIPHCLK_I2C4;
	PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
	PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
	PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
	PeriphClkInitStruct.I2c4ClockSelection = RCC_I2C4CLKSOURCE_D3PCLK1;

	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/* Enable SYSCFG clock */
	__HAL_RCC_SYSCFG_CLK_ENABLE();

	/* Enable GPIO clocks */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
}

extern void Error_Handler(void)
{
	/* Nothing yet */
}

extern void MAIN_ChangeChannelConfiguration(channel_t channel)
{
	MAIN_ChannelConfigurationToChange = channel;
	MAIN_ChangeChannelConfigurationRequested[channel] = true;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *pFileName, uint32_t line)
{
	/* Reports the name of the source file and the source line number where the assert_param error has occurred */
	/* printf("assert_failed: file %s on line %d\r\n", pFileName, line) */
}
#endif /* USE_FULL_ASSERT */
