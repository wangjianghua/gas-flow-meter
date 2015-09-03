#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define LED_RUN                     0u

#define LED_RUN_PORT                GPIOA
#define LED_RUN_PIN                 GPIO_Pin_12

#define LED_RUN_TOGGLE_PERIOD      (300 / 50)

#define LED_RUN_TOGGLE()            LED_Toggle(LED_RUN)

void LED_On(INT8U led_id);
void LED_Off(INT8U led_id);
void LED_Toggle(INT8U led_id);
void LED_Init(void);


#ifdef __cplusplus
}
#endif

#endif
