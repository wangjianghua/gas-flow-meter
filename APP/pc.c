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
    INT8U err, len, new_time[MAX_RTC_TIME_ITEM], buf[128];
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

                    case PC_READ_TIME_CMD:
                        memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN], &g_rtc_time, MAX_RTC_TIME_ITEM);
                        
                        pc_frame_send.Len = DL645_07_DATA_ITEM_LEN + MAX_RTC_TIME_ITEM;
                        
                        pc_frame_send.Ctrl = 0x91;
                        
                        send_len = Create_DL645_Frame((INT8U *)addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                                                
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((INT8U *)&pc_frame_send, send_len); 
                        break;

                    case PC_WRITE_TIME_CMD:
                        new_time[SEC] = Hex2Bcd(pc_frame_recv.Data[12]);
                        new_time[MIN] = Hex2Bcd(pc_frame_recv.Data[13]);
                        new_time[HOUR] = Hex2Bcd(pc_frame_recv.Data[14]);
                        new_time[WEEK] = Hex2Bcd(pc_frame_recv.Data[15]) % MAX_WEEK_ITEM;
                        new_time[DATE] = Hex2Bcd(pc_frame_recv.Data[16]);
                        new_time[MONTH] = Hex2Bcd(pc_frame_recv.Data[17]);
                        new_time[YEAR] = Hex2Bcd(pc_frame_recv.Data[18]);

                        RTC_WriteTime(new_time);

                        pc_frame_send.Len = 0;
                        
                        pc_frame_send.Ctrl = 0x94;
                        
                        send_len = Create_DL645_Frame((INT8U *)addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                                                
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((INT8U *)&pc_frame_send, send_len); 
                        break;

                    case PC_READ_VERSION_CMD:
                        memcpy(&buf[0], HARDWARE_VERSION, VERSION_LEN);
                        memcpy(&buf[VERSION_LEN], SOFTWARE_VERSION, VERSION_LEN);
                        
                        memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN], buf, 2 * VERSION_LEN);
                        
                        pc_frame_send.Len = DL645_07_DATA_ITEM_LEN + 2 * VERSION_LEN;
                        
                        pc_frame_send.Ctrl = 0x91;
                        
                        send_len = Create_DL645_Frame((INT8U *)addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                                                
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((INT8U *)&pc_frame_send, send_len); 
                        break;

                    case PC_RESTORE_CMD:
                        mem_para_restore();
                        
                        pc_frame_send.Len = 0;
                        
                        pc_frame_send.Ctrl = 0x94;
                        
                        send_len = Create_DL645_Frame((INT8U *)addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                                                
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((INT8U *)&pc_frame_send, send_len);
                        break;

                    case PC_READ_MEMS_CAL_CMD:
                        memcpy(&buf[0], &g_mem_para.mems_cal_state, sizeof(g_mem_para.mems_cal_state));
                        memcpy(&buf[SINGLE_PARA_SIZE], g_mem_para.mems_cal, sizeof(g_mem_para.mems_cal));

                        len = sizeof(g_mem_para.mems_cal_state) + sizeof(g_mem_para.mems_cal);
                        
                        memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN], buf, len);
                        
                        pc_frame_send.Len = DL645_07_DATA_ITEM_LEN + len;
                        
                        pc_frame_send.Ctrl = 0x91;
                        
                        send_len = Create_DL645_Frame((INT8U *)addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                                                
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((INT8U *)&pc_frame_send, send_len); 
                        break;

                    case PC_WRITE_MEMS_CAL_GRP1_CMD:
                        g_mem_para.mems_cal[MEMS_CAL_GRP_1][MEMS_CAL_ELEM_LOCAL] = g_mems_para.average_flow;
                        memcpy(&g_mem_para.mems_cal[MEMS_CAL_GRP_1][MEMS_CAL_ELEM_REMOTE], &pc_frame_recv.Data[12], SINGLE_PARA_SIZE);

                        mem_para_write();
                            
                        pc_frame_send.Len = 0;
                        
                        pc_frame_send.Ctrl = 0x94;
                        
                        send_len = Create_DL645_Frame((INT8U *)addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                                                
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((INT8U *)&pc_frame_send, send_len);                        
                        break;

                    case PC_WRITE_MEMS_CAL_GRP2_CMD:
                        g_mem_para.mems_cal[MEMS_CAL_GRP_2][MEMS_CAL_ELEM_LOCAL] = g_mems_para.average_flow;
                        memcpy(&g_mem_para.mems_cal[MEMS_CAL_GRP_2][MEMS_CAL_ELEM_REMOTE], &pc_frame_recv.Data[12], SINGLE_PARA_SIZE);

                        mem_para_write();
            
                        pc_frame_send.Len = 0;
                        
                        pc_frame_send.Ctrl = 0x94;
                        
                        send_len = Create_DL645_Frame((INT8U *)addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                                                
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((INT8U *)&pc_frame_send, send_len);                        
                        break;

                    case PC_WRITE_MEMS_CAL_GRP3_CMD:
                        g_mem_para.mems_cal[MEMS_CAL_GRP_3][MEMS_CAL_ELEM_LOCAL] = g_mems_para.average_flow;
                        memcpy(&g_mem_para.mems_cal[MEMS_CAL_GRP_3][MEMS_CAL_ELEM_REMOTE], &pc_frame_recv.Data[12], SINGLE_PARA_SIZE);

                        mem_para_write();
                            
                        pc_frame_send.Len = 0;
                        
                        pc_frame_send.Ctrl = 0x94;
                        
                        send_len = Create_DL645_Frame((INT8U *)addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                                                
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((INT8U *)&pc_frame_send, send_len);                        
                        break;

                    case PC_WRITE_MEMS_CAL_GRP4_CMD:
                        g_mem_para.mems_cal[MEMS_CAL_GRP_4][MEMS_CAL_ELEM_LOCAL] = g_mems_para.average_flow;
                        memcpy(&g_mem_para.mems_cal[MEMS_CAL_GRP_4][MEMS_CAL_ELEM_REMOTE], &pc_frame_recv.Data[12], SINGLE_PARA_SIZE);

                        mem_para_write();
                            
                        pc_frame_send.Len = 0;
                        
                        pc_frame_send.Ctrl = 0x94;
                        
                        send_len = Create_DL645_Frame((INT8U *)addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                                                
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((INT8U *)&pc_frame_send, send_len);                        
                        break;

                    case PC_WRITE_MEMS_CAL_GRP5_CMD:
                        g_mem_para.mems_cal[MEMS_CAL_GRP_5][MEMS_CAL_ELEM_LOCAL] = g_mems_para.average_flow;
                        memcpy(&g_mem_para.mems_cal[MEMS_CAL_GRP_5][MEMS_CAL_ELEM_REMOTE], &pc_frame_recv.Data[12], SINGLE_PARA_SIZE);

                        mem_para_write();
                            
                        pc_frame_send.Len = 0;
                        
                        pc_frame_send.Ctrl = 0x94;
                        
                        send_len = Create_DL645_Frame((INT8U *)addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                                                
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((INT8U *)&pc_frame_send, send_len);                        
                        break;

                    case PC_MEMS_CAL_CONFIRM_CMD:
                        memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN], &g_mem_para.mems_cal_state, SINGLE_PARA_SIZE);
                        
                        pc_frame_send.Len = DL645_07_DATA_ITEM_LEN + SINGLE_PARA_SIZE;
                        
                        pc_frame_send.Ctrl = 0x91;
                        
                        send_len = Create_DL645_Frame((INT8U *)addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                                                
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((INT8U *)&pc_frame_send, send_len);                        
                        break;

                    case PC_READ_MEMS_DEBOUNCE_CMD:
                        memcpy(&pc_frame_send.Data[DL645_07_DATA_ITEM_LEN], &g_mem_para.mems_debounce_threshold, SINGLE_PARA_SIZE);
                                               
                        pc_frame_send.Len = DL645_07_DATA_ITEM_LEN + SINGLE_PARA_SIZE;
                        
                        pc_frame_send.Ctrl = 0x91;
                        
                        send_len = Create_DL645_Frame((INT8U *)addr, pc_frame_send.Ctrl, pc_frame_send.Len, &pc_frame_send);
                                                
                        while(OSSemAccept(g_sem_pc));
                        
                        pc_uart_send((INT8U *)&pc_frame_send, send_len);                        
                        break;

                    case PC_WRITE_MEMS_DEBOUNCE_THRESHOLD_CMD:
                        memcpy(&g_mem_para.mems_debounce_threshold, &pc_frame_recv.Data[12], SINGLE_PARA_SIZE);

                        mem_para_write();
                            
                        pc_frame_send.Len = 0;
                        
                        pc_frame_send.Ctrl = 0x94;
                        
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

