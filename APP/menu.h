#ifndef __MENU_H__
#define __MENU_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define FORM_MSG_NONE               0u
#define FORM_MSG_REFRESH            1u
#define FORM_MSG_KEY                2u
#define FORM_MSG_DATA               3u

#define FORM_ID_MAIN                0u
#define FORM_ID_INFO                1u
#define FORM_ID_SET                 2u
#define MAX_FORM_NUM                3u

typedef int (*FORM_FNCT)(unsigned int key_msg, unsigned int form_msg);

typedef struct
{
    FORM_FNCT fnct;
} FORM, *P_FORM;

extern const FORM form_list[MAX_FORM_NUM];
extern unsigned int form_id;

void form_refresh(void);
int form_main(unsigned int key_msg, unsigned int form_msg);
int form_info(unsigned int key_msg, unsigned int form_msg);
int form_set(unsigned int key_msg, unsigned int form_msg);
void form_init(void);


#ifdef __cplusplus
}
#endif

#endif
