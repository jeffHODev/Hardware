#ifndef APP_H
#define APP_H
#include "main.h"






typedef struct
{
	unsigned char work_mode;
	unsigned char init_flag;
}work_params_stru;
void app(void);
void Flow_Init(void);
unsigned char module_stop_ele(void);
unsigned char get_reset_status(void);
void power_off(void);

#endif

