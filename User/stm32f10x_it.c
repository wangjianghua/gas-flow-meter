/**
  ******************************************************************************
  * @file    ADC/ADC1_DMA/stm32f10x_it.c 
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
#include "stm32f10x_it.h"
#include "includes.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup ADC_ADC1_DMA
  * @{
  */ 

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
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    sys_evt_add(SYS_EVT_TICK);
}

/**
  * @brief  This function handles RTC IRQ Handler.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
    if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        /* Clear the RTC Second interrupt */
        RTC_ClearITPendingBit(RTC_IT_SEC);

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();

        sys_evt_add(SYS_EVT_TIME);
    }    
}

/**
  * @brief  This function handles EXTI0 IRQ Handler.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) 
    { 
        EXTI_ClearITPendingBit(EXTI_Line0); 
        
        g_key_param.msg = KEY_MSG_CLEAR;
        sys_evt_add(SYS_EVT_KEY_PRESS);
    }     
}

/**
  * @brief  This function handles EXTI4 IRQ Handler.
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4) != RESET) 
    { 
        EXTI_ClearITPendingBit(EXTI_Line4); 

        g_key_param.msg = KEY_MSG_SET;
        sys_evt_add(SYS_EVT_KEY_PRESS);        
    }     
}

/**
  * @brief  This function handles EXTI9_5 IRQ Handler.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5) != RESET) 
    { 
        EXTI_ClearITPendingBit(EXTI_Line5); 

        g_key_param.msg = KEY_MSG_UP;
        sys_evt_add(SYS_EVT_KEY_PRESS);        
    }     

    if(EXTI_GetITStatus(EXTI_Line6) != RESET) 
    { 
        EXTI_ClearITPendingBit(EXTI_Line6); 

        g_key_param.msg = KEY_MSG_DOWN;
        sys_evt_add(SYS_EVT_KEY_PRESS); 
    }     

    if(EXTI_GetITStatus(EXTI_Line7) != RESET) 
    { 
        EXTI_ClearITPendingBit(EXTI_Line7); 

        g_key_param.msg = KEY_MSG_PAUSE;
        sys_evt_add(SYS_EVT_KEY_PRESS);        
    }         
}

/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
    u8 rx_data;

    
    if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
    {
        USART_ClearFlag(USART2, USART_FLAG_ORE);
        
        USART_ReceiveData(USART2);
    }    
    
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);

        rx_data = USART_ReceiveData(USART2);

        if(MAX_UART_RECV_BUF_LEN == g_flow_param.rx.bytes) //缓冲区已满，使缓冲区保持最新数据
        {
#if (UART_DEBUG_EN > 0u)
            if(!g_flow_param.recv_pkt_overflow)
            {
                g_flow_param.recv_pkt_overflow = TRUE; //数据溢出，置缓冲区溢出标志
            }
#endif

            *g_flow_param.rx.write_index = rx_data;
            g_flow_param.rx.write_index++;
            
            g_flow_param.rx.read_index++;

            if(g_flow_param.rx.write_index >= (g_flow_param.rx.buf + MAX_UART_RECV_BUF_LEN))
            {
                g_flow_param.rx.write_index = g_flow_param.rx.buf;
            }

            if(g_flow_param.rx.read_index >= (g_flow_param.rx.buf + MAX_UART_RECV_BUF_LEN))
            {
                g_flow_param.rx.read_index = g_flow_param.rx.buf;
            }
        }
        else //缓冲区未满
        {
            *g_flow_param.rx.write_index = rx_data;
            g_flow_param.rx.write_index++;

            if(g_flow_param.rx.write_index >= (g_flow_param.rx.buf + MAX_UART_RECV_BUF_LEN))
            {
                g_flow_param.rx.write_index = g_flow_param.rx.buf;
            }

            g_flow_param.rx.bytes++;
        }		        
    }

    if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_TC);

        if(g_flow_param.tx.bytes > 0)
        {
            USART_SendData(USART2, *g_flow_param.tx.addr);
            g_flow_param.tx.addr++;
            g_flow_param.tx.bytes--;
        }
        else
        {
            uart_send_end_callback();
        }           
    }
}

/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        g_app_tick++;
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
