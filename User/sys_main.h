#ifndef __SYS_MAIN_H__
#define __SYS_MAIN_H__

#ifdef __cplusplus
 extern "C" {
#endif


typedef void (*EVENT_PROC_PTR)();

#define SYS_EVENT_MAX     (sizeof(_sys_evt_proc) / sizeof(EVENT_PROC_PTR))
#define SYS_EVENT_MASK    ((1 << SYS_EVENT_MAX) - 1)
#define SYS_EVT_PROC(pos)  _sys_evt_proc[pos]()


#ifdef __cplusplus
}
#endif

#endif
