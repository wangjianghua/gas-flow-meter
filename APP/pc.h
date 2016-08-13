#ifndef __PC_H__
#define __PC_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define PC_SHAKE_HANDS_CMD                      0xFF000001
#define PC_READ_TIME_CMD                        0xFF000002
#define PC_WRITE_TIME_CMD                       0xFF000003
#define PC_READ_VERSION_CMD                     0xFF000004
#define PC_RESTORE_CMD                          0xFF000005
#define PC_READ_MEMS_PARA_CMD                   0xFF000006
#define PC_WRITE_MEMS_AVERAGE_TIMES_CMD         0xFF000007
#define PC_WRITE_MEMS_CAL_COEFFICIENT_CMD       0xFF000008

INT16U PC_postProcess(PVOID handle);
void App_TaskPC(void *p_arg);

extern OS_EVENT *g_sem_pc;


#ifdef __cplusplus
}
#endif

#endif
