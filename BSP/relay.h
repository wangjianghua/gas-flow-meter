#ifndef __RELAY_H__
#define __RELAY_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define RELAY_PORT                  GPIOB
#define RELAY_PIN                   GPIO_Pin_1

#define RELAY_ON()                  GPIO_SetBits(RELAY_PORT, RELAY_PIN)
#define RELAY_OFF()                 GPIO_ResetBits(RELAY_PORT, RELAY_PIN)

void RELAY_Init(void);
void relay_turn_on(void);
void relay_turn_off(void);


#ifdef __cplusplus
}
#endif

#endif
