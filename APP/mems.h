#ifndef __MEMS_H__
#define __MEMS_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define MAX_MEMS_FLOW               50u

typedef struct 
{
    INT16U int_part;
    INT16U dec_part;
} FLOW_PARA, *P_FLOW_PARA;

typedef struct 
{
    INT16U target_int_part;
    INT16U target_dec_part;
    INT32U target;
    INT32U flow;
} MEMS_PARA, *P_MEMS_PARA;

extern MEMS_PARA g_mems_para;


#ifdef __cplusplus
}
#endif

#endif
