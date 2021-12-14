#ifndef _CMD_PROCESS_H
#define _CMD_PROCESS_H
#include "hmi_driver.h"
#define REBOOT_ID  0x07
#define WASH_ID    0x05

#define REBOOT_PAGE				    5
#define Main_PAGE					    3
#define CALIBRATION_PAGE			1
#define SETTING_PAGE			   4


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

#define CMD_DATA_LENGTH		6

typedef struct
{
	uint8_t    cmd_type;  //命令类型
	uint8_t    control_id;  //产生消息的控件ID
	uint8_t    page_id;  //产生消息的画面ID
	uint8_t    status;
	uint8_t    key_type; //控件类型
       uint8_t    key_value;
	uint8_t    cmd_tail;   //帧尾
}CTRL_MSG,*PCTRL_MSG;


typedef struct
{
	uint8_t    cmd_type;  //命令类型
	uint8_t    control_id;  //产生消息的控件ID
	uint8_t    page_id;  //产生消息的画面ID
	uint8_t    status;
	uint8_t    param[57];//可变长度参数，最多个字节
	uint8_t    cmd_tail;   //帧尾
}EDIT_MSG,*PEDIT_MSG;


void ProcessMessage( PCTRL_MSG msg, uint16_t size );




void NotifySetPage(uint8_t status);
void NotifyGetPage(uint8_t page_id,uint8_t status);
void NotifyTouchButton(uint8_t page_id, uint8_t control_id, uint8_t  state,uint8_t type,uint8_t value);
void NotifyTouchCheckbox(uint8_t page_id, uint8_t control_id, uint8_t  state,uint8_t type,uint8_t value);
void NotifyTouchSlider(uint8_t page_id, uint8_t control_id, uint8_t  state,uint8_t type,uint8_t value);
void NotifyGetEdit(PEDIT_MSG msg, uint16_t size);
void NotifyGetTouchEdit(PEDIT_MSG msg, uint16_t size);
void NotifyGetCheckbox(uint8_t page_id, uint8_t control_id, uint8_t  state,uint8_t type,uint8_t value);
void NotifyGetSlider(uint8_t page_id, uint8_t control_id, uint8_t  state,uint8_t type,uint8_t value);
void NotifyLable(uint8_t page_id, uint8_t control_id,uint8_t status);
void NotifyNumber(uint8_t page_id, uint8_t control_id,uint8_t status);
void NotifyProgressbar(uint8_t page_id, uint8_t control_id, uint8_t status);
void NotifyCircleGauge(uint8_t page_id, uint8_t control_id, uint8_t status);
void NotifyBarGauge(uint8_t page_id, uint8_t control_id, uint8_t  status);
void NotifyThermometer(uint8_t page_id, uint8_t control_id, uint8_t  status);
void NotifyWaterGauge(uint8_t page_id, uint8_t control_id, uint8_t  status);
void NotifyBattery(uint8_t page_id, uint8_t control_id, uint8_t  status);
void NotifySetWaveformValue(uint8_t page_id, uint8_t control_id, uint8_t  status);
void NotifyWaveformDataInsert(uint8_t page_id, uint8_t control_id, uint8_t  status);
void NotifyTouchEdit(uint8_t page_id, uint8_t control_id, uint8_t  state,uint8_t type,uint8_t value);
#endif
