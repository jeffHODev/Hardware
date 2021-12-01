#ifndef _CMD_PROCESS_H
#define _CMD_PROCESS_H
#include "hmi_driver.h"
#include "main.h"

#define REBOOT_ID  0x07
#define WASH_ID    0x05

#define REBOOT_PAGE				    5
#define Main_PAGE					3
#define CALIBRATION_PAGE			1
#define SETTING_PAGE			    4


#define PROGRESS_ID       17
#define START_TEXT_ID   18
#define PH1_CAL_ID   25
#define PH2_CAL_ID   26
#define ORP_CAL_ID   33
#define PH1_CAL_OK_ID   37
#define PH2_CAL_OK_ID   38
#define ORP_CAL_OK_ID   39
#define SETTING_ID   6
#define CALIBRATION_ID   23
#define SETTING_RE_ID   24
#define CALIBRATION_OK_ID   27
#define CALIBRATION_RE_ID   28

#define LABEL_PH_ID  50



#define NOTIFY_TOUCH_BUTTON		0x62
#define NOTIFY_TOUCH_CHECKBOX		0x60
#define NOTIFY_TOUCH_SLIDER		0x67
#define NOTIFY_TOUCH_EDIT		0x7B

#define NOTIFY_GET_EDIT			0x75
#define NOTIFY_GET_PAGE			0x71
#define NOTIFY_GET_CHECKBOX		0x64
#define NOTIFY_GET_SLIDER			0x68
#define NOTIFY_GET_TOUCH_EDIT			0x7C

#define NOTIFY_SET_PAGE			0x70
#define NOTIFY_SET_NUMBER			0x6E

#define NOTIFY_SET_PROGRESS		0x6F
#define NOTIFY_SET_CHECKOUT		0x63
#define NOTIFY_SET_CIRCLEGAUGE 	0x7a
#define NOTIFY_SET_BARGAUGE 		0x61
#define NOTIFY_SET_WATERGAUGE 	0x77
#define NOTIFY_SET_THERMOMETER 	0x6d
#define NOTIFY_SET_WAVEFORM 		0x66
#define NOTIFY_SET_BATTERY 		0x79

#define NOTIFY_CLEAR_WAVEFORM 		0x76
#define NOTIFY_INSER_WAVEFORM_DATA	0x6a
#define NOTIFY_REFRESS_WAVEFORM		0x6b


#define  CHAR						0X30
#define  CLEAR						0X31
#define  BACK_SPACE					0X32
#define  SPACE 						0X33
#define  ENTER						0X34
#define  CHANGE_PAGE				0X35
#define  UPLOAD_CONTROL_ID			0X36

#define SUCCESS						0x6f
#define FAILURE						0x65

#define GET_EDIT_VALUE				0X01
#define GET_TOUCH_EDIT_VALUE 		0x7c





 #define BarGauge_PAGE    155


#define CircleGauge_PAGE			2
#define Keyboard_PAGE				3

#define Setting_PAGE					5
#define Slider_PAGE					6
#define Temperture_PAGE				7
#define Text_PAGE					8
#define Waveform_PAGE				9


#define KEY_DOWN					0x44
#define KEY_RELEASE 				0x55

#define SELECT						0X53
#define UNSELECT					0X55


typedef struct
{
	uint8    cmd_head;  //帧头
       uint8    cmd_length;
	uint8    cmd_type;  //命令类型
	uint8    control_id;  //产生消息的控件ID
	uint8    page_id;  //产生消息的画面ID
	uint8    status;
	uint8    key_type; //控件类型
       uint8    key_value;
	uint8    cmd_tail;   //帧尾
}CTRL_MSG,*PCTRL_MSG;


typedef struct
{
	uint8    cmd_head;  //帧头
       uint8    cmd_length;
	uint8    cmd_type;  //命令类型
	uint8    control_id;  //产生消息的控件ID
	uint8    page_id;  //产生消息的画面ID
	uint8    status;
	uint8    param[57];//可变长度参数，最多个字节
	uint8    cmd_tail;   //帧尾
}EDIT_MSG,*PEDIT_MSG;


void ProcessMessage( PCTRL_MSG msg, uint16 size );




void NotifySetPage(uint8 status);
void NotifyGetPage(uint8 page_id,uint8 status);
void NotifyTouchButton(uint8 page_id, uint8 control_id, uint8  state,uint8 type,uint8 value);
void NotifyTouchCheckbox(uint8 page_id, uint8 control_id, uint8  state,uint8 type,uint8 value);
void NotifyTouchSlider(uint8 page_id, uint8 control_id, uint8  state,uint8 type,uint8 value);
void NotifyGetEdit(PEDIT_MSG msg, uint16 size);
void NotifyGetTouchEdit(PEDIT_MSG msg, uint16 size);
void NotifyGetCheckbox(uint8 page_id, uint8 control_id, uint8  state,uint8 type,uint8 value);
void NotifyGetSlider(uint8 page_id, uint8 control_id, uint8  state,uint8 type,uint8 value);
void NotifyLable(uint8 page_id, uint8 control_id,uint8 status);
void NotifyNumber(uint8 page_id, uint8 control_id,uint8 status);
void NotifyProgressbar(uint8 page_id, uint8 control_id, uint8 status);
void NotifyCircleGauge(uint8 page_id, uint8 control_id, uint8 status);
void NotifyBarGauge(uint8 page_id, uint8 control_id, uint8  status);
void NotifyThermometer(uint8 page_id, uint8 control_id, uint8  status);
void NotifyWaterGauge(uint8 page_id, uint8 control_id, uint8  status);
void NotifyBattery(uint8 page_id, uint8 control_id, uint8  status);
void NotifySetWaveformValue(uint8 page_id, uint8 control_id, uint8  status);
void NotifyWaveformDataInsert(uint8 page_id, uint8 control_id, uint8  status);
void NotifyTouchEdit(uint8 page_id, uint8 control_id, uint8  state,uint8 type,uint8 value);
#endif
