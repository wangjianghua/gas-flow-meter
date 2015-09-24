#include "includes.h"


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

void STM32_SoftReset(void)
{
    __set_FAULTMASK(SET);
    
    NVIC_SystemReset();
}

uint32_t STM32_EncodePriority(uint32_t PreemptPriority, uint32_t SubPriority)
{
    uint32_t prioritygroup = 0x0;
    

    /* Check the parameters */
    assert_param(IS_NVIC_PREEMPTION_PRIORITY(PreemptPriority));
    assert_param(IS_NVIC_SUB_PRIORITY(SubPriority));

    prioritygroup = NVIC_GetPriorityGrouping();
    
    return (NVIC_EncodePriority(prioritygroup, PreemptPriority, SubPriority));
}

/**
  * @brief  Set System clock frequency to 72MHz and configure HCLK, PCLK2 
  *         and PCLK1 prescalers. 
  * @param  None
  * @retval None
  */
static void SetSysClockTo72(void)
{
  ErrorStatus HSEStartUpStatus;
  
  
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

#ifdef STM32F10X_CL
    /* Configure PLLs *********************************************************/
    /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    RCC_PREDIV2Config(RCC_PREDIV2_Div5);
    RCC_PLL2Config(RCC_PLL2Mul_8);

    /* Enable PLL2 */
    RCC_PLL2Cmd(ENABLE);

    /* Wait till PLL2 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
    {}

    /* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */ 
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#else
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
#endif

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock configuration.
       User can add here some code to deal with this error */    

    /* Go to infinite loop */
    while (1)
    {
    }
  }
}

void RCC_Configuration(void)
{
    /* Set System clock frequency to 72MHz and configure HCLK, PCLK2 and PCLK1 prescalers */
    SetSysClockTo72();

    /* Enable the GPIO Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);     

    /* Enable the AFIO Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   

    /* Enable the USART1 Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);    

    /* Enable the USART2 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);       
}

void GPIO_Configuration(void)
{
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}

void NVIC_Configuration(void)
{
    /* Configure 2 bits for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /*
      0x00~0x03，设置SysTick为抢占优先级0  
      0x04~0x07，设置SysTick为抢占优先级1  
      0x08~0x0B，设置SysTick为抢占优先级2  
      0x0C~0x0F，设置SysTick为抢占优先级3  
     */
    NVIC_SetPriority(SysTick_IRQn, STM32_EncodePriority(SYS_TICK_PREEMPT_PRIO, SYS_TICK_SUB_PRIO));
}

void IWDG_Init(void)
{
    /* Enable the LSI OSC */
    RCC_LSICmd(ENABLE);
    
    /* Wait till LSI is ready */
    while(RESET == RCC_GetFlagStatus(RCC_FLAG_LSIRDY));
    
    /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
       dispersion) */
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);   
    
    /* IWDG counter clock: 40KHz(LSI) / 32 = 1.25KHz */
    IWDG_SetPrescaler(IWDG_Prescaler_32);
    
    /* Set counter reload value to obtain 250ms IWDG TimeOut.
       Counter Reload Value = 250ms / IWDG counter clock period
                            = 250ms / (LSI / 32)
                            = 0.25s / (LsiFreq / 32)
                            = LsiFreq / (32 * 4)
                            = LsiFreq / 128
     */
    IWDG_SetReload(1250); /* 1250 / 1.25KHz = 1000ms */
    
    /* Reload IWDG counter */
    IWDG_ReloadCounter();
    
    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
}

void LED_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
      

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);    
}

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    

    /* Configure Button GPIOx Pin as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /* Connect EXTIx Line to Button GPIOx Pin */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);

    /* Configure EXTIx Line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Configure EXTIx Line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Configure EXTIx Line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure); 

    /* Configure EXTIx Line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Configure EXTIx Line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Configure EXTIx Line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Assign ISR handler */
	BSP_IntVectSet(BSP_INT_ID_EXTI0, EXTI0_IRQHandler);

    /* Assign ISR priority */
    BSP_IntPrioSet(BSP_INT_ID_EXTI0, STM32_EncodePriority(EXTI_PREEMPT_PRIO, EXTI_SUB_PRIO));

    /* Enable EXTIx Interrupt */
	BSP_IntEn(BSP_INT_ID_EXTI0);

    /* Assign ISR handler */
	BSP_IntVectSet(BSP_INT_ID_EXTI1, EXTI1_IRQHandler);

    /* Assign ISR priority */
    BSP_IntPrioSet(BSP_INT_ID_EXTI1, STM32_EncodePriority(EXTI_PREEMPT_PRIO, EXTI_SUB_PRIO));

    /* Enable EXTIx Interrupt */
	BSP_IntEn(BSP_INT_ID_EXTI1);

    /* Assign ISR handler */
	BSP_IntVectSet(BSP_INT_ID_EXTI2, EXTI2_IRQHandler);

    /* Assign ISR priority */
    BSP_IntPrioSet(BSP_INT_ID_EXTI2, STM32_EncodePriority(EXTI_PREEMPT_PRIO, EXTI_SUB_PRIO));

    /* Enable EXTIx Interrupt */
	BSP_IntEn(BSP_INT_ID_EXTI2);

    /* Assign ISR handler */
	BSP_IntVectSet(BSP_INT_ID_EXTI3, EXTI3_IRQHandler);

    /* Assign ISR priority */
    BSP_IntPrioSet(BSP_INT_ID_EXTI3, STM32_EncodePriority(EXTI_PREEMPT_PRIO, EXTI_SUB_PRIO));

    /* Enable EXTIx Interrupt */
	BSP_IntEn(BSP_INT_ID_EXTI3);

    /* Assign ISR handler */
	BSP_IntVectSet(BSP_INT_ID_EXTI4, EXTI4_IRQHandler);

    /* Assign ISR priority */
    BSP_IntPrioSet(BSP_INT_ID_EXTI4, STM32_EncodePriority(EXTI_PREEMPT_PRIO, EXTI_SUB_PRIO));

    /* Enable EXTIx Interrupt */
	BSP_IntEn(BSP_INT_ID_EXTI4);

    /* Assign ISR handler */
	BSP_IntVectSet(BSP_INT_ID_EXTI9_5, EXTI9_5_IRQHandler);

    /* Assign ISR priority */
    BSP_IntPrioSet(BSP_INT_ID_EXTI9_5, STM32_EncodePriority(EXTI_PREEMPT_PRIO, EXTI_SUB_PRIO));

    /* Enable EXTIx Interrupt */
	BSP_IntEn(BSP_INT_ID_EXTI9_5);        
}

void LCD_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);   
}

void RTC_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);   
}

void USART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;


    /* Configure USART1 Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
    /* Configure USART1 Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Disable the USART1 */
    USART_Cmd(USART1, DISABLE);     

    /* Disable USART1 Receive and Transmit interrupts */
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);    

    /* Assign ISR handler */
	BSP_IntVectSet(BSP_INT_ID_USART1, USART1_IRQHandler);

    /* Assign ISR priority */
    BSP_IntPrioSet(BSP_INT_ID_USART1, STM32_EncodePriority(USART1_PREEMPT_PRIO, USART1_SUB_PRIO));

    /* Enable USART1 Interrupt */
	BSP_IntEn(BSP_INT_ID_USART1);    

    /* USART1 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */
    USART_StructInit(&USART_InitStructure);
    
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    
    /* Configure USART1 */
    USART_Init(USART1, &USART_InitStructure);   

    /* Enable the USART1 */
    USART_Cmd(USART1, ENABLE);  
}

void USART2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;


    /* Configure USART2 Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
    /* Configure USART2 Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Disable the USART2 */
    USART_Cmd(USART2, DISABLE);     

    /* Disable USART2 Receive and Transmit interrupts */
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
    USART_ITConfig(USART2, USART_IT_TXE, DISABLE);    

    /* Assign ISR handler */
	BSP_IntVectSet(BSP_INT_ID_USART2, USART2_IRQHandler);

    /* Assign ISR priority */
    BSP_IntPrioSet(BSP_INT_ID_USART2, STM32_EncodePriority(USART2_PREEMPT_PRIO, USART2_SUB_PRIO));

    /* Enable USART2 Interrupt */
	BSP_IntEn(BSP_INT_ID_USART2);    

    /* USART2 configured as follow:
        - BaudRate = 38400 baud  
        - Word Length = 9 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */
    USART_StructInit(&USART_InitStructure);
    
    USART_InitStructure.USART_BaudRate = 38400;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    
    /* Configure USART2 */
    USART_Init(USART2, &USART_InitStructure);   

    /* Enable the USART2 */
    USART_Cmd(USART2, ENABLE);  
}

void BEEP_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
      

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);   
}

void RELAY_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
      

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);   
}

void BSP_Init(void)
{
    /* System clocks configuration ---------------------------------------------*/
    RCC_Configuration();

    /* GPIO configuration ------------------------------------------------------*/
    GPIO_Configuration();   

    /* NVIC configuration ------------------------------------------------------*/
    NVIC_Configuration();   

#if (WDT_EN > 0u)
    IWDG_Init();
#endif

    LED_Init();

    KEY_Init();

    RTC_Init();

    USART1_Init();

    USART2_Init();

    BEEP_Init();

    RELAY_Init();
}

/*
*********************************************************************************************************
*                                       BSP_CPU_ClkFreq()
*
* Description : This function reads CPU registers to determine the CPU clock frequency of the chip in KHz.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/
CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    static  RCC_ClocksTypeDef  rcc_clocks;


    RCC_GetClocksFreq(&rcc_clocks);

    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                                         OS CORTEX-M3 FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         OS_CPU_SysTickClkFreq()
*
* Description : Get system tick clock frequency.
*
* Argument(s) : none.
*
* Return(s)   : Clock frequency (of system tick).
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
INT32U  OS_CPU_SysTickClkFreq (void)
{
    INT32U  freq;


    freq = BSP_CPU_ClkFreq();
    
    return (freq);
}

/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/
