#ifndef DEVCTRL_H
#define DEVCTRL_H
#include "main.h"
#define FORWARD   0
#define BACKWARD  1
#define SHUNT     2

#define WATER_SW  1
#define SALT_SW   2
#define WASTE_SW  3
#define WASH_SW   4
#define HCILO_SW  5


void EleCtrl(unsigned char dir,unsigned char ctrl);
void DcMotorCtrl(unsigned char motorNo,uint32_t pwm);
void EleSwCtrl(unsigned char devNO,unsigned char ctrl);

#endif 

