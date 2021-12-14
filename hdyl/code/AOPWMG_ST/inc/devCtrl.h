#ifndef DEVCTRL_H
#define DEVCTRL_H
#include "main.h"
#define FORWARD   0
#define BACKWARD  1
#define SHUNT     2

void EleCtrl(unsigned char dir,unsigned char ctrl);
void EleSwCtrl(unsigned char devNO,unsigned char ctrl);
void DcMotorCtrl(unsigned char motorNo,uint32_t pwm);

#endif 

