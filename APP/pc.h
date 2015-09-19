#ifndef __PC_H__
#define __PC_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define PC_SHAKE_HANDS_CMD                 0xFF000001

INT16U PC_postProcess(PVOID handle);
void App_TaskPC(void *p_arg);

extern OS_EVENT *g_sem_pc;


#ifdef __cplusplus
}
#endif

#endif
