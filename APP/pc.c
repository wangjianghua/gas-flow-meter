#include "includes.h"


OS_EVENT *g_sem_pc;

DL645_FRAME pc_frame_send;
DL645_FRAME pc_frame_recv;
DL645_FRAME_STAT pc_frame_stat;

INT16U pc_uart_send(INT8U *buf, INT16U len)
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

    pMsg->msg_header.end_id = PC_COM_PORT;

    pMsg->msg_header.need_buffer_free = TRUE;

    return (End_send(pMsg));
}

INT16U PC_postProcess(PVOID handle)
{
    P_MSG_INFO pMsg = (P_MSG_INFO)handle;
    INT8U *mBuf = (INT8U *)(pMsg->msg_buffer);
    INT16U mLen = pMsg->msg_header.msg_len;
    OS_CPU_SR_ALLOC();


    OS_ENTER_CRITICAL();
    memcpy(&pc_frame_recv, mBuf, mLen);
    OS_EXIT_CRITICAL();

    OSSemPost(g_sem_pc);

    return (TRUE);
}

/*
*********************************************************************************************************
*                                             App_TaskPC()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskPC()' by 'OSTaskCreateExt()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  App_TaskPC (void *p_arg)
{
    const INT8U addr[6] = {0x99, 0x99, 0x99, 0x99, 0x99, 0x99};
    INT8U err;
    INT16U send_len;
    INT32U data_item;
    

    (void)p_arg;
    
    while (DEF_TRUE) {
        OSSemPend(g_sem_pc, 0, &err);

        if(OS_ERR_NONE == err)
        {
            if(DL645_FRAME_OK == Analysis_DL645_Frame((INT8U *)addr, (INT8U *)&pc_frame_recv, &pc_frame_stat))
            {
                memcpy(&pc_frame_send, &pc_frame_recv, sizeof(DL645_FRAME));
                
                if((FRM_CTRW_07_READ_SLVS_DATA == (pc_frame_stat.Ctrl & CCTT_CONTROL_CODE_MASK)) ||
                   (FRM_CTRW_07_WRITE_SLVS_DATA == (pc_frame_stat.Ctrl & CCTT_CONTROL_CODE_MASK)) || 
                   (FRM_CTRW_07_READ_SLVS_FOLLOW_DATA == (pc_frame_stat.Ctrl & CCTT_CONTROL_CODE_MASK)))
                {
                    data_item = ((INT32U)pc_frame_recv.Data[3] << 24) | ((INT32U)pc_frame_recv.Data[2] << 16) | ((INT32U)pc_frame_recv.Data[1] << 8) | ((INT32U)pc_frame_recv.Data[0] << 0);
                
                    switch(data_item)
                    {
                    case PC_SHAKE_HANDS_CMD:
                        pc_frame_send.Len = DL645_07_DATA_ITEM_LEN;
                        pc_frame_send.Ctrl = 0x91;
        
                        send_len = Create_DL645_Frame((INT8U *)addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
        
                        while(OSSemAccept(g_sem_pc));
        
                        pc_uart_send((INT8U *)&pc_frame_send, send_len);
                        break;

                    default:
                        break;
                    }
                }
            }
        }
    }
}

