#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define LED_RUN                     0u
#define LED_MEMS                    1u
#define LED_UART                    2u

#define LED_RUN_PORT                GPIOA
#define LED_RUN_PIN                 GPIO_Pin_12

#define LED_MEMS_PORT               GPIOB
#define LED_MEMS_PIN                GPIO_Pin_4

#define LED_UART_PORT               GPIOB
#define LED_UART_PIN                GPIO_Pin_5

#define LED_RUN_TOGGLE()            LED_Toggle(LED_RUN)

#define LED_MEMS_ON()               LED_On(LED_MEMS)
#define LED_MEMS_OFF()              LED_Off(LED_MEMS)

#define LED_UART_ON()               LED_On(LED_UART)
#define LED_UART_OFF()              LED_Off(LED_UART)

void LED_On(INT8U led_id);
void LED_Off(INT8U led_id);
void LED_Toggle(INT8U led_id);
void LED_Init(void);


#ifdef __cplusplus
}
#endif

#endif
