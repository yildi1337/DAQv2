/*
 *******************************************************************************
 * File:			hal_pcd.c
 * Date:			2020-11-14
 * Author:			Phillip Durdaut
 * Summary:			Interface for calling lower layer functions from the HAL.
 * 					(Historically ST provides two different ways to implement
 * 					those callback functions. You either overwrite a weak symbol
 * 					buried somewhere in the HAL driver or you register the
 * 					callback functions in the peripheral handler (which requires
 * 					enabling a macro). Here, the first option is used and all
 * 					the callback functions with weak symbols inside
 * 					stm32h7xx_hal_pcd.c are copied here.)
 * Notes:			-
 *******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbd_core.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Program code --------------------------------------------------------------*/

void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
	USBD_LL_DataOutStage(hpcd->pData, epnum, hpcd->OUT_ep[epnum].xfer_buff);
}

void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
	USBD_LL_DataInStage(hpcd->pData, epnum, hpcd->IN_ep[epnum].xfer_buff);
}

void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{
	USBD_LL_SetupStage(hpcd->pData, (uint8_t *)hpcd->Setup);
}

void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
	USBD_LL_SOF(hpcd->pData);
}

void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
{
	USBD_LL_SetSpeed(hpcd->pData, USBD_SPEED_HIGH);
	USBD_LL_Reset(hpcd->pData);
}

void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
{
	USBD_LL_Suspend(hpcd->pData);
}

void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
{
	USBD_LL_Resume(hpcd->pData);
}

void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
	USBD_LL_IsoOUTIncomplete(hpcd->pData, epnum);
}

void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
	USBD_LL_IsoINIncomplete(hpcd->pData, epnum);
}

void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
{
	USBD_LL_DevConnected(hpcd->pData);
}

void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
{
	USBD_LL_DevDisconnected(hpcd->pData);
}
