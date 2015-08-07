#ifndef __BEEP_H__
#define __BEEP_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define BUZZER_PIN            GPIO_Pin_5
#define BUZZER_PORT           GPIOB

#define BUZZER_ON()           GPIO_SetBits(BUZZER_PORT, BUZZER_PIN)
#define BUZZER_OFF()          GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN) 

#define DEFAULT_BEEP_PERIOD  (5000 / (1000 / SYS_TICKS_PER_SEC))

extern u32 g_beep_cnt;

void init_beep(void);
void beep(void);
void beep_on(void); 
void beep_off(void);


#ifdef __cplusplus
}
#endif

#endif
