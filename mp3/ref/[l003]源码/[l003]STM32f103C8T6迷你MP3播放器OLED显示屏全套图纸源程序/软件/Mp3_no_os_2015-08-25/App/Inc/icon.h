
/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  BEGIN OF FILE  ********************************/

/**********************************************************************
* file           : icon.h
* author         : waft_wind
* version        : V1.0
* date           : 2014-11-6
* brief          : Header for icon.c module
* Description    : --
**********************************************************************/

/* Define to prevent recursive inclusion ----------------------------*/
#ifndef __ICON_H
#define __ICON_H

 

/* Includes  == 文件包含 --------------------------------------------*/
#include "stm32f10x.h"



/* Private typedef    ==  私有类型 ----------------------------------*/
/* Private define     ==  私有定义 ----------------------------------*/

//-----------------------------------------//
const u8 ICON_Setting_Select[6*48] = 
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x70,0xF8,0xFC,0xF8,0xF0,
	0xF0,0xE0,0xE0,0xE0,0xF0,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xF0,0xE0,0xE0,0xE0,
	0xF0,0xF0,0xF8,0xFC,0xF8,0x70,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xE0,0xE0,0xF1,0xFF,0xFF,0xFF,
	0xFF,0x1F,0x07,0x83,0xE3,0xF3,0xF1,0xF9,0xF9,0xF9,0xF1,0xF3,0xE3,0x83,0x07,0x1F,
	0xFF,0xFF,0xFF,0xFF,0xF1,0xE0,0xE0,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x07,0x07,0x0F,0x0F,0x1F,0xFF,0xFF,0xFF,
	0xFF,0xF0,0xC0,0x83,0x8F,0x1F,0x3F,0x3F,0x3F,0x3F,0x3F,0x1F,0x8F,0x83,0xC0,0xF0,
	0xFF,0xFF,0xFF,0xFF,0x1F,0x0F,0x0F,0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x1C,0x3F,0x7F,0x3F,0x1F,
	0x1F,0x0F,0x0F,0x1F,0x1F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x1F,0x0F,0x0F,0x0F,
	0x1F,0x1F,0x3F,0x7F,0x3F,0x1C,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const u8 ICON_Setting_Notselect[6*48] = 
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x70,0xD8,0x0C,0x18,0x10,
	0x30,0x20,0x20,0x20,0x30,0x1C,0x87,0x80,0x80,0x80,0x8F,0x1C,0x30,0x20,0x20,0x20,
	0x30,0x10,0x08,0x0C,0x98,0x70,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x40,0x40,0x60,0x20,0x31,0x1F,0x00,0xC0,
	0xF0,0x1C,0x06,0x83,0xE3,0x31,0x11,0x19,0x09,0x19,0x11,0x13,0xE3,0x82,0x06,0x18,
	0xF0,0x80,0x00,0x1F,0x31,0x20,0x60,0x40,0x40,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x04,0x04,0x0C,0x08,0x18,0xF0,0x00,0x07,
	0x1F,0x70,0xC0,0x83,0x8E,0x18,0x30,0x20,0x20,0x20,0x20,0x18,0x8C,0x87,0xC0,0x70,
	0x1F,0x0F,0x00,0xF0,0x18,0x0C,0x0C,0x04,0x04,0x07,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x1C,0x33,0x61,0x20,0x10,
	0x18,0x08,0x08,0x19,0x11,0x63,0xC3,0x03,0x03,0x03,0xE3,0x71,0x19,0x08,0x08,0x08,
	0x18,0x10,0x30,0x61,0x37,0x1C,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x02,0x02,0x02,0x03,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const u8 ICON_Floder_Select[6*48] = 
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFE,0x02,0x7A,0x3A,0xBA,0xBA,0xBA,0xBA,
	0xBA,0x82,0x9E,0x98,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,
	0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0xF0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x80,0xF8,0x7F,0x01,0xFD,0xFD,0xFD,
	0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,
	0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0x01,0xF3,0xFF,0x1E,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x07,0xF0,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0xFE,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x80,0xBE,0xBF,0xBF,0xBF,0xBF,0xBF,
	0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,0xBF,
	0xBF,0xBF,0xBF,0xBF,0xBF,0x8F,0xE0,0xFE,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const u8 ICON_Floder_Notselect[6*48] = 
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFE,0x02,0x02,0x02,0x82,0x82,0x82,0x82,
	0x82,0x82,0x9E,0x98,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,
	0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0xF0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x80,0xF8,0x7F,0x01,0x01,0x01,0x01,
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xF3,0xFF,0x1E,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x07,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,0x80,0x80,0x80,0x80,0x80,0xE0,0xFE,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

const u8 ICON_Power_00[2*32] = 
{
	0x00,0x00,0x00,0x00,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
	0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xC0,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
	0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0F,0x01,0x01,0x00,0x00,0x00,0x00	
};

const u8 ICON_Power_10[2*32] = 
{
	0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
	0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xC0,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
	0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0F,0x01,0x01,0x00,0x00,0x00,0x00	
};

const u8 ICON_Power_20[2*32] = 
{
	0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
	0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xC0,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x0F,0x0F,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
	0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0F,0x01,0x01,0x00,0x00,0x00,0x00	
};

const u8 ICON_Power_30[2*32] = 
{
	0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0x08,0x08,0x08,0x08,0x08,
	0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xC0,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x08,0x08,0x08,0x08,0x08,
	0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0F,0x01,0x01,0x00,0x00,0x00,0x00	
};

const u8 ICON_Power_40[2*32] = 
{
	0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0x08,0x08,0x08,
	0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xC0,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x08,0x08,0x08,
	0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0F,0x01,0x01,0x00,0x00,0x00,0x00,	
};

const u8 ICON_Power_50[2*32] = 
{
	0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0x08,
	0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xC0,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x08,
	0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0F,0x01,0x01,0x00,0x00,0x00,0x00	
};

const u8 ICON_Power_60[2*32] = 
{
	0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
	0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xC0,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0F,0x01,0x01,0x00,0x00,0x00,0x00	
};

const u8 ICON_Power_70[2*32] = 
{
	0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
	0xF8,0xF8,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0xC0,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x08,0x08,0x08,0x08,0x08,0x08,0x0F,0x01,0x01,0x00,0x00,0x00,0x00	
};

const u8 ICON_Power_80[2*32] = 
{
	0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
	0xF8,0xF8,0xF8,0xF8,0xF8,0x08,0x08,0x08,0x08,0xF8,0xC0,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x08,0x08,0x08,0x08,0x0F,0x01,0x01,0x00,0x00,0x00,0x00	
};

const u8 ICON_Power_90[2*32] = 
{
	0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
	0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0x08,0x08,0xF8,0xC0,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x08,0x08,0x0F,0x01,0x01,0x00,0x00,0x00,0x00	
};

const u8 ICON_Power_100[2*32] = 
{
	0x00,0x00,0x00,0x00,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
	0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xC0,0xC0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x01,0x01,0x00,0x00,0x00,0x00	
};

const u8 ICON_AllCyc[2*32] = 
{
	0x00,0x00,0x00,0xE0,0xF0,0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
	0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x38,0xF0,0xE0,0x00,0x00,0x00,0xff,
	0x00,0x00,0x00,0x07,0x0F,0x1C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,
	0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x18,0x18,0x1C,0x0F,0x07,0x00,0x00,0x00,0xff
}; 

const u8 ICON_OneCyc[2*32] = 
{
	0x00,0x00,0x00,0xE0,0xF0,0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xD8,
	0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x38,0xF0,0xE0,0x00,0x00,0x00,0xff,
	0x00,0x00,0x00,0x07,0x0F,0x1C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x07,
	0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x18,0x18,0x1C,0x0F,0x07,0x00,0x00,0x00,0xff
}; 

const u8 ICON_AllOrder[2*32] = 
{
	0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xE0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0xff,
	0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x07,0x03,0x03,0x01,0x01,0x00,0x00,0x00,0xff
};

const u8 ICON_OneOrder[2*32] = 
{
	0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0xF0,
	0x00,0x80,0x80,0x80,0x80,0x80,0x80,0xE0,0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0xff,
	0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x0F,
	0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x07,0x03,0x03,0x01,0x01,0x00,0x00,0x00,0xff
};

const u8 ICON_Play[2*16] = 
{
	0xFF,0x01,0x01,0x01,0xF9,0xF9,0xF1,0xE1,0xC1,0xC1,0x81,0x81,0x01,0x01,0x01,0xFF,
	0xFF,0x80,0x80,0x80,0x9F,0x9F,0x8F,0x87,0x83,0x83,0x81,0x81,0x80,0x80,0x80,0xFF
};

const u8 ICON_Stop[2*16] = 
{
	0xFF,0x01,0x01,0x01,0xF9,0xF9,0xF9,0x01,0x01,0xF9,0xF9,0xF9,0x01,0x01,0x01,0xFF,
	0xFF,0x80,0x80,0x80,0x9F,0x9F,0x9F,0x80,0x80,0x9F,0x9F,0x9F,0x80,0x80,0x80,0xFF
};



/* Private macro      ==  私有宏 ------------------------------------*/
/* Private variables  ==  私有变量 ----------------------------------*/



/* Private function prototypes  ==  私有函数声明 --------------------*/



/* Define to prevent recursive inclusion ----------------------------*/
#endif /* __ICON_H */




/*********************************  (C) COPYRIGHT 2014 waft_wind  *********  END OF FILE  **********************************/
