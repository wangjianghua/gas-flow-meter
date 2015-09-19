#include "includes.h"


OS_EVENT *g_sem_mems;

MEMS_FRAME mems_frame_send;
MEMS_FRAME mems_frame_recv;
MEMS_FRAME_STAT mems_frame_stat;

MEMS_PARA g_mems_para;

void MEMS_CSPack(INT8U *buf)
{
    INT8U i, len, CS;
    
        
    len = buf[1];
    CS = buf[0];
            
    for(i = 1; i <= (len + 1); i++) 
    {
        CS ^= buf[i];
    }
    
    buf[len + 2] = CS;
    
    buf[len + 3] = 0x0D;
}

INT16U MEMS_CheckFrame(INT8U *buf)
{
    INT8U i, offset, len, CS;
    
    
    for(offset = 0; offset < 12; offset++)
    {
        if(0x9D == buf[offset])
        {
            break;
        }
    }
    
    if(offset > 10)
    {
        return (MEMS_FRAME_ERROR);
    }

    len = buf[offset + 2];    
    
    if(0x0D != buf[offset + len + 4])
    {
        return (MEMS_FRAME_ERROR);
    }
    
    CS = buf[offset + 1];
    
    for(i = 2; i <= (len + 2); i++)
    {
        CS ^= buf[offset + i];
    }
    
    if(CS != buf[offset + len + 3])
    {
        return (MEMS_FRAME_ERROR);
    }
    
    return (offset);
}

INT16U MEMS_CreateFrame(INT8U cmd, INT8U len, P_MEMS_FRAME p_mems_frame)
{
    p_mems_frame->start = 0x9D;
    p_mems_frame->cmd = cmd;
    p_mems_frame->len = len;
    MEMS_CSPack(&p_mems_frame->cmd);

    return (len + 5);
}

INT16U MEMS_AnalysisFrame(INT8U *buf, P_MEMS_FRAME_STAT p_mems_frame_stat)
{
    INT16U i;
    P_MEMS_FRAME p_mems_frame;

    
    p_mems_frame_stat->status = 0;

    i = MEMS_CheckFrame(buf);

    if(MEMS_FRAME_ERROR == i)
    {
        return (MEMS_FRAME_ERROR);
    }

    p_mems_frame_stat->status = 1;

    p_mems_frame = (P_MEMS_FRAME)(buf + i);

    p_mems_frame_stat->cmd = p_mems_frame->cmd;

    return (MEMS_FRAME_OK);
}

INT16U mems_uart_send(INT8U *buf, INT16U len)
{
    P_MSG_INFO pMsg = NULL;
    

    if((NULL == buf) || (0 == len))
    {
        return (FALSE);
    }

    if(!(pMsg = (P_MSG_INFO)alloc_send_buffer(MSG_SHORT)))
    {
        return (FALSE);
    }

    memcpy(pMsg->msg_buffer, buf, len);

    pMsg->msg_header.msg_len = len;

    pMsg->msg_header.end_id = MEMS_COM_PORT;

    pMsg->msg_header.need_buffer_free = TRUE;

    return (End_send(pMsg));
}

INT16U MEMS_postProcess(PVOID handle)
{
    P_MSG_INFO pMsg = (P_MSG_INFO)handle;
    INT8U *mBuf = (INT8U *)(pMsg->msg_buffer);
    INT16U mLen = pMsg->msg_header.msg_len;
    OS_CPU_SR_ALLOC();


    OS_ENTER_CRITICAL();
    memcpy(&mems_frame_recv, mBuf, mLen);
    OS_EXIT_CRITICAL();

    OSSemPost(g_sem_mems);

    return (TRUE);
}

void mems_read_instant_flow(void)
{
    INT8U cmd, len, buf[8];
    INT16U send_len;

    
    cmd = MEMS_READ_INSTANT_FLOW_CMD;
    len = 0x01;
    buf[0] = 0x08;
    memcpy(mems_frame_send.data, buf, len);

    send_len = MEMS_CreateFrame(cmd, len, &mems_frame_send);

    mems_uart_send((INT8U *)&mems_frame_send, send_len);
}

/*
*********************************************************************************************************
*                                             App_TaskMEMS()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskMEMS()' by 'OSTaskCreateExt()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  App_TaskMEMS (void *p_arg)
{
    INT8U err, FRH, FRM, FRL;
    

    (void)p_arg;
    
    while (DEF_TRUE) {        
        while(OSSemAccept(g_sem_mems));
        
        mems_read_instant_flow();
        
        OSSemPend(g_sem_mems, OS_TICKS_PER_SEC, &err);

        if(OS_ERR_NONE == err)
        {
            LED_MEMS_ON();
            
            if(MEMS_FRAME_OK == MEMS_AnalysisFrame((INT8U *)&mems_frame_recv, &mems_frame_stat))
            {
                switch(mems_frame_stat.cmd)
                {
                case MEMS_READ_INSTANT_FLOW_CMD:
                    FRH = mems_frame_recv.data[0];
                    FRM = mems_frame_recv.data[1];
                    FRL = mems_frame_recv.data[2];

                    g_mems_para.instant_flow = (INT32U)FRH * 65536 + (INT32U)FRM * 256 + (INT32U)FRL;
                    
                    g_mems_para.disp_flow = g_mems_para.instant_flow + 50;          

                    g_mems_para.disp_flow_int_part = g_mems_para.disp_flow / 1000;
                    g_mems_para.disp_flow_dec_part = (g_mems_para.disp_flow - (g_mems_para.disp_flow_int_part * 1000)) / 100;                    
                    break;

                default:
                    break;
                }
            }
        }
        else
        {
            LED_MEMS_OFF();
        }
    }
}

