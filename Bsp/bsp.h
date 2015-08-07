#ifndef __BSP_H__
#define __BSP_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define SYS_TICK_PRIO              13 

#define RTC_PREEM_PRIO              0
#define RTC_SUB_PRIO                5

#define EXTI0_PREEM_PRIO            0
#define EXTI0_SUB_PRIO              3 

#define EXTI4_PREEM_PRIO            0
#define EXTI4_SUB_PRIO              3 

#define EXTI5_PREEM_PRIO            0
#define EXTI5_SUB_PRIO              3 

#define EXTI6_PREEM_PRIO            0
#define EXTI6_SUB_PRIO              3 

#define EXTI7_PREEM_PRIO            0
#define EXTI7_SUB_PRIO              3 

#define USART2_PREEM_PRIO           1
#define USART2_SUB_PRIO             1

#define TIM2_PREEM_PRIO             1
#define TIM2_SUB_PRIO               2

#define EI()      _enable_interrupt()
#define DI()     _disable_interrupt()

#ifndef RTC_CLK_SRC_LSE
#define RTC_CLK_SRC_LSI             
#endif

void bsp_init(void);
void _disble_rtc(void);
void _enable_rtc(void);


#ifdef __cplusplus
}
#endif

#endif
