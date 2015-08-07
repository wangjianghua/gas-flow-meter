#ifndef __RELAY_H__
#define __RELAY_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define RELAY_N_ON()             GPIO_ResetBits(GPIOA, GPIO_Pin_0)
#define RELAY_N_OFF()              GPIO_SetBits(GPIOA, GPIO_Pin_0)

#define RELAY_L_ON()             GPIO_ResetBits(GPIOB, GPIO_Pin_1)
#define RELAY_L_OFF()              GPIO_SetBits(GPIOB, GPIO_Pin_1)

void init_relay(void);


#ifdef __cplusplus
}
#endif

#endif
