/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   IAR STM32-SK Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_PRIO               2u
#define  APP_CFG_TASK_KEY_PRIO                 3u
#define  APP_CFG_TASK_END_TICK_PRIO            4u
#define  APP_CFG_TASK_END_PROC_PRIO            5u
#define  APP_CFG_TASK_TIME_PRIO                6u
#define  APP_CFG_TASK_PC_PRIO                  7u
#define  APP_CFG_TASK_MEMS_PRIO                8u
#define  OS_TASK_TMR_PRIO             (OS_LOWEST_PRIO - 2)

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_STK_SIZE         128u
#define  APP_CFG_TASK_KEY_STK_SIZE           128u
#define  APP_CFG_TASK_END_TICK_STK_SIZE      128u
#define  APP_CFG_TASK_END_PROC_STK_SIZE      128u
#define  APP_CFG_TASK_TIME_STK_SIZE          128u
#define  APP_CFG_TASK_PC_STK_SIZE            128u
#define  APP_CFG_TASK_MEMS_STK_SIZE          128u

/*
*********************************************************************************************************
*                                          OTHER CONFIGURATION
*********************************************************************************************************
*/


#endif
