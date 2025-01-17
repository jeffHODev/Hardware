#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"

#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 320

#else
#define LCD_W 320
#define LCD_H 240
#endif





//-----------------LCD端口定义---------------- 

#define LCD_SCLK_Clr() HAL_GPIO_WritePin(SCK_GPIO_Port,SCK_Pin,GPIO_PIN_RESET)//CLK
#define LCD_SCLK_Set() HAL_GPIO_WritePin(SCK_GPIO_Port,SCK_Pin,GPIO_PIN_SET)

#define LCD_MOSI_Clr() HAL_GPIO_WritePin(MOSI_GPIO_Port,MOSI_Pin,GPIO_PIN_RESET)//DIN
#define LCD_MOSI_Set() HAL_GPIO_WritePin(MOSI_GPIO_Port,MOSI_Pin,GPIO_PIN_SET)

#define LCD_RES_Clr()  HAL_GPIO_WritePin(RES_GPIO_Port,RES_Pin,GPIO_PIN_RESET)//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(RES_GPIO_Port,RES_Pin,GPIO_PIN_SET)

#define LCD_DC_Clr()   HAL_GPIO_WritePin(DC_GPIO_Port,DC_Pin,GPIO_PIN_RESET)//DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(DC_GPIO_Port,DC_Pin,GPIO_PIN_SET)

#define LCD_BLK_Clr() HAL_GPIO_WritePin(BLK_GPIO_Port,BLK_Pin,GPIO_PIN_RESET)//CS
#define LCD_BLK_Set() HAL_GPIO_WritePin(BLK_GPIO_Port,BLK_Pin,GPIO_PIN_SET)




void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




