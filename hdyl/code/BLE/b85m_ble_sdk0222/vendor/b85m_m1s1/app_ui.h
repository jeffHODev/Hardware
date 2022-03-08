/********************************************************************************************************
 * @file	app_ui.h
 *
 * @brief	This is the header file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#ifndef APP_UI_H_
#define APP_UI_H_


extern  int key_not_released;
extern 	int	master_pairing_enable;
extern 	int master_unpair_enable;

extern	int master_disconnect_connhandle;

extern	int master_auto_connect;
extern	int user_manual_pairing;
#define BUTTON_LONG_TIME   300
#define BUTTON_FILTER_TIME  5
#define KEY_START_HOLD   1
#define KEY_START_DOWN   2
#define KEY_PAIR         3
#define KEY_UNPAIR       4

#define AUTO_PAIR    0

#define LED_PAIR   0
#define LED_UNPAIR 1
#define LED_NORMAL 2
#define LED_DISCON 3
#define LED_CON 4

#define NORMAL 0
#define SETTING 1

#define ON  0
#define OFF 1
#define DEBUG_BLE 0
#define ACK_TIME_OUT     120*1000*1000
#define SLEEP_TIME_OUT   0xffffffff
#define TIMEOUT_PERIOD 100*1000
#define M_ON_PERIOD 100*1000
#define M_OFF_PERIOD 500*1000
#define PKT_HEAD  0xfe
#define MEASURE_PERIOD 40*1000
#define CON_TIME_OUT  120*1000*1000
typedef struct
{
	u8 start;
	u8 Mstart;
	u8 stop;
	u16 dis;
	u32 time;
	u16 sum;
	u32 tick;
	u32 motor_tick;
	u8 key;
	u8 key_update;
	u8 key_down_flag;
	u8 key_down_cnt;
	u32 timeout;
	u8 timeoutFlag;
	u8  power_status;
	u32 rx_time;
	u8 mac[6];
	u8 ack_sig;
	u16 timeout_cnt;
	u8 mode;
}measure_stru;
typedef struct
{
	u8 status;
	u32 tick;
}led_stru;

typedef struct
{
	u8 connection;
	u8 disconnection;
}ble_stru;

/**
 * @brief      keyboard task handler
 * @param[in]  e    - event type
 * @param[in]  p    - Pointer point to event parameter.
 * @param[in]  n    - the length of event parameter.
 * @return     none.
 */
void proc_keyboard (u8 e, u8 *p, int n);

/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_ENTER"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
void  app_set_kb_wakeup (u8 e, u8 *p, int n);



/**
 * @brief      keyboard initialization
 * @param[in]  none
 * @return     none.
 */
void keyboard_init(void);



/**
 * @brief   BLE Unpair handle for master
 * @param   none.
 * @return  none.
 */
void proc_master_role_unpair(void);
void user_gpio_init(void);
void measure_start(void);
void mesure_proc(void);
void ble_status(u8 flag);
ble_stru *GetBle_status(void);
void parase(u8 tmp);
u8 getsn(void);
u8 master_conect_status(void);
void key_proc(void);
void sensor_power(u8 flag);
void led_ctrl(void);
void ui_proc(void);
measure_stru *getmeasrue(void);
void init_measure(void);
void cal_rx_time(void);
void led_mode_set(u8 status);
void pkt_pack(u8 ucmd);


#endif /* APP_UI_H_ */
