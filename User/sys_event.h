#ifndef __SYS_EVENT_H__
#define __SYS_EVENT_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define BIT0                      (0x0001)
#define BIT1                      (0x0002)
#define BIT2                      (0x0004)
#define BIT3                      (0x0008)
#define BIT4                      (0x0010)
#define BIT5                      (0x0020)
#define BIT6                      (0x0040)
#define BIT7                      (0x0080)
#define BIT8                      (0x0100)
#define BIT9                      (0x0200)
#define BITA                      (0x0400)
#define BITB                      (0x0800)
#define BITC                      (0x1000)
#define BITD                      (0x2000)
#define BITE                      (0x4000)
#define BITF                      (0x8000)

typedef unsigned char _sys_pos_t;
typedef unsigned long _sys_tick_t;
typedef unsigned long _sys_sec_t; 
typedef unsigned short _sys_event_t;

extern _sys_tick_t _sys_tick;
extern _sys_sec_t _sys_sec;
extern _sys_event_t _sys_event;

#define SYS_EVT_VAR     _sys_event
#define SYS_EVT_ADD(evt)  ( _sys_event |= evt )

#define clr_wdt()                  IWDG_ReloadCounter()

#define NO_TICK                   (0u  )
#define NO_SEC                    (0u  )
#define NO_EVT                    (0u  )

#define SYS_TICKS_PER_SEC         (100u)

#define _disable_interrupt()       __disable_irq()
#define _enable_interrupt()        __enable_irq()

void sys_evt_add(_sys_event_t evt);
void sys_evt_del(_sys_event_t evt);

#define SYS_EVT_TICK 		      (BIT0)
#define SYS_EVT_SECOND            (BIT1)
#define SYS_EVT_TIME              (BIT2)
#define SYS_EVT_KEY_PRESS         (BIT3)
#define SYS_EVT_UART_RECV         (BIT4)
#define SYS_EVT_FLOW              (BIT5)
#define SYS_EVT_RESERVE           (BIT6)

void on_tick(void);
void on_second(void);
void on_time(void);
void on_key_press(void);
void on_uart_recv(void);
void on_flow(void);


#ifdef __cplusplus
}
#endif

#endif
