/**
  ******************************************************************************
  * @file    stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
    if(RESET != EXTI_GetITStatus(EXTI_Line0)) 
    { 
        EXTI_ClearITPendingBit(EXTI_Line0);
        
        GUI_X_StoreKey(KEY_PORT, KEY_MSG_PAUSE);
    }    
}

/**
  * @brief  This function handles External line 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
    if(RESET != EXTI_GetITStatus(EXTI_Line1)) 
    { 
        EXTI_ClearITPendingBit(EXTI_Line1);
        
        GUI_X_StoreKey(KEY_PORT, KEY_MSG_CLEAR);
    }    
}

/**
  * @brief  This function handles External line 4 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
    if(RESET != EXTI_GetITStatus(EXTI_Line4)) 
    { 
        EXTI_ClearITPendingBit(EXTI_Line4);
        
        GUI_X_StoreKey(KEY_PORT, KEY_MSG_CONFIRM);
    }    
}

/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
    if(RESET != EXTI_GetITStatus(EXTI_Line7)) 
    { 
        EXTI_ClearITPendingBit(EXTI_Line7);
        
        GUI_X_StoreKey(KEY_PORT, KEY_MSG_SET);
    }
    else if(RESET != EXTI_GetITStatus(EXTI_Line6)) 
    { 
        EXTI_ClearITPendingBit(EXTI_Line6);
        
        GUI_X_StoreKey(KEY_PORT, KEY_MSG_DOWN);
    }
    else if(RESET != EXTI_GetITStatus(EXTI_Line5)) 
    { 
        EXTI_ClearITPendingBit(EXTI_Line5);
        
        GUI_X_StoreKey(KEY_PORT, KEY_MSG_UP);
    }
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
