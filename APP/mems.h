#ifndef __MEMS_H__
#define __MEMS_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define MAX_MEMS_FLOW                          50u

#define MEMS_FLOW_GAIN                       1000u

#define MEMS_READ_INSTANT_FLOW_CMD            0xF0

#define MEMS_SAMPLE_NUM_DEFAULT                 1u
#define MEMS_CAL_COEFFICIENT_DEFAULT            1u

#define MEMS_FRAME_MAX_DATA_LEN               257u

#define MEMS_FRAME_OK                           0u
#define MEMS_FRAME_ERROR                      127u

typedef struct
{
	INT8U status;
	INT8U cmd;
} MEMS_FRAME_STAT, *P_MEMS_FRAME_STAT;

#pragma pack(push)
#pragma pack(1)
typedef struct
{
    INT8U start;
    INT8U cmd;
    INT8U len;
    INT8U data[MEMS_FRAME_MAX_DATA_LEN];
} MEMS_FRAME, *P_MEMS_FRAME;
#pragma pack(pop)

typedef struct 
{    
    INT32U instant_flow;

    INT32U sum;
    INT32U sample_num;

    FP32 average_flow;

    INT32U measure_flow;
        
    INT32U disp_flow;
    INT32U disp_flow_int_part;
    INT32U disp_flow_dec_part;
} MEMS_PARA, *P_MEMS_PARA;

extern OS_EVENT *g_sem_mems;
extern MEMS_PARA g_mems_para;

INT16U MEMS_CreateFrame(INT8U cmd, INT8U len, P_MEMS_FRAME p_mems_frame);
INT16U MEMS_AnalysisFrame(INT8U *buf, P_MEMS_FRAME_STAT p_mems_frame_stat);
INT16U MEMS_postProcess(PVOID handle);
BOOLEAN mems_flow_cal(INT32U standard_flow);
void App_TaskMEMS(void *p_arg);


#ifdef __cplusplus
}
#endif

#endif
