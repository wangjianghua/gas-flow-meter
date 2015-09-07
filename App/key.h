#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define KEY_PORT_NULL           NULL
#define KEY_MSG_NONE           (!(GPIO_Pin_All))

#define KEY_PORT                GPIOA

#define KEY_MSG_SET             GPIO_Pin_7
#define KEY_MSG_DEC             GPIO_Pin_6
#define KEY_MSG_INC             GPIO_Pin_5
#define KEY_MSG_CONFIRM         GPIO_Pin_4
#define KEY_MSG_CLEAR           GPIO_Pin_1
#define KEY_MSG_PAUSE           GPIO_Pin_0

typedef struct {
    GPIO_TypeDef *GPIOx;
    INT16U GPIO_Pin;
} KEY_MSG, *P_KEY_MSG;

typedef struct {
    KEY_MSG msg;
    OS_EVENT *sem;
} KEY_PARA, *P_KEY_PARA;

extern KEY_PARA g_key_para;

void GUI_X_Init(void);
int GUI_X_GetKey(void);
int GUI_X_WaitKey(void); 
void GUI_X_StoreKey(GPIO_TypeDef *GPIOx, INT16U GPIO_Pin); 
void GUI_X_KeyProc(int k);
void App_TaskKey(void *p_arg);


#ifdef __cplusplus
}
#endif

#endif
