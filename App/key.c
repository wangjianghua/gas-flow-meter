#include "includes.h"


KEY_PARA g_key_para;

void  GUI_X_Init (void) 
{
    g_key_para.msg.GPIOx    = KEY_PORT_NULL;
    g_key_para.msg.GPIO_Pin = KEY_MSG_NONE;
}

int  GUI_X_GetKey (void) 
{
    int r;


    r                       = g_key_para.msg.GPIO_Pin;
    g_key_para.msg.GPIOx    = KEY_PORT_NULL;
    g_key_para.msg.GPIO_Pin = KEY_MSG_NONE;
    
    return (r);
}

int  GUI_X_WaitKey (void) 
{
    int   r;
    INT8U err;


    r = KEY_MSG_NONE;
    
    if((KEY_PORT_NULL == g_key_para.msg.GPIOx) || 
       (KEY_MSG_NONE == g_key_para.msg.GPIO_Pin)) 
    {
        OSSemPend(g_key_para.sem, 0, &err);
    }

    if(OS_ERR_NONE == err)
    {
        if(RESET == GPIO_ReadInputDataBit(g_key_para.msg.GPIOx, g_key_para.msg.GPIO_Pin))
        {
            OSTimeDlyHMSM(0, 0, 0, 30);

            if(RESET == GPIO_ReadInputDataBit(g_key_para.msg.GPIOx, g_key_para.msg.GPIO_Pin))
            {
                r = g_key_para.msg.GPIO_Pin;
            }
        }
    }

    g_key_para.msg.GPIOx    = KEY_PORT_NULL;
    g_key_para.msg.GPIO_Pin = KEY_MSG_NONE;
    
    return (r);
}

void  GUI_X_StoreKey (GPIO_TypeDef *GPIOx, INT16U GPIO_Pin) 
{
    g_key_para.msg.GPIOx = GPIOx;
    g_key_para.msg.GPIO_Pin = GPIO_Pin;

    OSSemPost(g_key_para.sem);
}

void  GUI_X_KeyProc (int k)
{
    int form_msg;
    INT32U key_msg;


    key_msg = k;

    if(NULL != form_list[form_id].fnct)
    {
        if(FORM_MSG_NONE != (form_msg = (*form_list[form_id].fnct)(key_msg, FORM_MSG_KEY)))
        {
            (*form_list[form_id].fnct)(key_msg, form_msg);
        }
    }
}

/*
*********************************************************************************************************
*                                             App_TaskKey()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskKey()' by 'OSTaskCreateExt()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  App_TaskKey (void *p_arg)
{
    INT32U key_msg;

    
    (void)p_arg; 
    
    while (DEF_TRUE) {  
        key_msg = GUI_X_WaitKey();

        GUI_X_KeyProc(key_msg);
    }
}

