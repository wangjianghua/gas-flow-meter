#ifndef __BEEP_H__
#define __BEEP_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define BUZZER_PORT             GPIOB
#define BUZZER_PIN              GPIO_Pin_10

#define BUZZER_ON()             GPIO_SetBits(BUZZER_PORT, BUZZER_PIN)
#define BUZZER_OFF()            GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN) 

#define BEEP_PERIOD            (2000 / 50)

extern INT32U g_beep_count;

void BEEP_Init(void);
void beep(void);
void beep_on(void); 
void beep_off(void);


#ifdef __cplusplus
}
#endif

#endif
