/*
******************************************************************************************************
 * @file	app_ui.c
 *
 * @brief	This is the source file for BLE SDK
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
 ******************************************************************************************************/
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "app.h"
#include "app_att.h"
#include "app_ui.h"

#include "application/keyboard/keyboard.h"
#include "application/usbstd/usbkeycode.h"
#include "config_usr.h"

measure_stru measure_usr;
u8 tx_buf[8];
int	master_pairing_enable = 0;
int master_unpair_enable = 0;

_attribute_ble_data_retention_ int master_disconnect_connhandle;   //mark the master connection which is in un_pair disconnection flow



u32 rx_tick;

int master_auto_connect = 0;
int user_manual_pairing = 0;





#if (UI_KEYBOARD_ENABLE)

_attribute_ble_data_retention_	int 	key_not_released;


#define CONSUMER_KEY   	   			1
#define KEYBOARD_KEY   	   			2
#define PAIR_UNPAIR_KEY   	   		3
led_stru led_usr;

_attribute_ble_data_retention_	u8 		key_type;

/*
 * @brief   Check changed key value.
 * @param   none.
 * @return  none.

*/
/*void key_change_proc(void)
{

	u8 key0 = kb_event.keycode[0];
//	u8 key_buf[8] = {0,0,0,0,0,0,0,0};

	key_not_released = 1;
	if (kb_event.cnt == 2)   //two key press
	{

	}
	else if(kb_event.cnt == 1)
	{
		if(key0 >= CR_VOL_UP )  //volume up/down
		{
			key_type = CONSUMER_KEY;
			u16 consumer_key;
			if(key0 == CR_VOL_UP){  	//volume up
				consumer_key = MKEY_VOL_UP;
				my_dump_str_data(APP_DUMP_EN, "UI send Vol+", 0, 0);
			}
			else if(key0 == CR_VOL_DN){ //volume down
				consumer_key = MKEY_VOL_DN;
				my_dump_str_data(APP_DUMP_EN, "UI send Vol-", 0, 0);
			}


			Here is just Telink Demonstration effect. Cause the demo board has limited key to use, when Vol+/Vol- key pressed, we
			send media key "Vol+" or "Vol-" to master for all slave in connection.
			For users, you should known that this is not a good method, you should manage your device and GATT data transfer
			according to  conn_dev_list[]

			for(int i=MASTER_MAX_NUM; i < (MASTER_MAX_NUM + SLAVE_MAX_NUM); i++){ //slave index is from "MASTER_MAX_NUM" to "MASTER_MAX_NUM + SLAVE_MAX_NUM - 1"
				if(conn_dev_list[i].conn_state){
					blc_gatt_pushHandleValueNotify (conn_dev_list[i].conn_handle, HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
				}
			}
		}
		else{
			key_type = PAIR_UNPAIR_KEY;

			if(key0 == BTN_PAIR)   //Manual pair triggered by Key Press
			{
				master_pairing_enable = 1;
				my_dump_str_data(APP_DUMP_EN, "UI PAIR begin", 0, 0);
			}
			else if(key0 == BTN_UNPAIR) //Manual un_pair triggered by Key Press
			{
				Here is just Telink Demonstration effect. Cause the demo board has limited key to use, only one "un_pair" key is
				 available. When "un_pair" key pressed, we will choose and un_pair one device in connection state
				if(conn_master_num){ //at least 1 master connection exist

					if(!master_disconnect_connhandle){  //if one master un_pair disconnection flow not finish, here new un_pair not accepted

						// choose one master connection to disconnect
						for(int i=0; i < MASTER_MAX_NUM; i++){ //slave index is from 0 to "MASTER_MAX_NUM - 1"
							if(conn_dev_list[i].conn_state){
								master_unpair_enable = conn_dev_list[i].conn_handle;  //mark connHandle on master_unpair_enable
								my_dump_str_data(APP_DUMP_EN, "UI UNPAIR", &master_unpair_enable, 2);
								break;
							}
						}
					}
				}
			}

		}

	}
	else   //kb_event.cnt == 0,  key release
	{
		key_not_released = 0;
		if(key_type == CONSUMER_KEY)
		{
			u16 consumer_key = 0;
			//Here is just Telink Demonstration effect. for all slave in connection, send release for previous "Vol+" or "Vol-" to master
			for(int i=MASTER_MAX_NUM; i < (MASTER_MAX_NUM + SLAVE_MAX_NUM); i++){ //slave index is from "MASTER_MAX_NUM" to "MASTER_MAX_NUM + SLAVE_MAX_NUM - 1"
				if(conn_dev_list[i].conn_state){
					blc_gatt_pushHandleValueNotify ( conn_dev_list[i].conn_handle, HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
				}
			}
		}
		else if(key_type == KEYBOARD_KEY)
		{

		}
		else if(key_type == PAIR_UNPAIR_KEY){
			if(master_pairing_enable){
				master_pairing_enable = 0;
				my_dump_str_data(APP_DUMP_EN, "UI PAIR end", 0, 0);
			}

			if(master_unpair_enable){
				master_unpair_enable = 0;
			}
		}
	}


}*/

void key_change_proc(void)
{

    u8 key0 = kb_event.keycode[0];
//	u8 key_buf[8] = {0,0,0,0,0,0,0,0};

    key_not_released = 1;
    if(measure_usr.key_update  == 1)
    {
        measure_usr.key_update  = 0;

        key_type = PAIR_UNPAIR_KEY;

        if(measure_usr.key == KEY_PAIR)   //Manual pair triggered by Key Press
        {
            master_pairing_enable = 1;
            my_dump_str_data(APP_DUMP_EN, "UI PAIR begin", 0, 0);
        }
        else if(measure_usr.key == KEY_UNPAIR) //Manual un_pair triggered by Key Press
        {
            if(conn_master_num)  //at least 1 master connection exist
            {

                if(!master_disconnect_connhandle)   //if one master un_pair disconnection flow not finish, here new un_pair not accepted
                {


                    for(int i=0; i < MASTER_MAX_NUM; i++)  //slave index is from 0 to "MASTER_MAX_NUM - 1"
                    {
                        if(conn_dev_list[i].conn_state)
                        {
                            master_unpair_enable = conn_dev_list[i].conn_handle;  //mark connHandle on master_unpair_enable
                            my_dump_str_data(APP_DUMP_EN, "UI UNPAIR", &master_unpair_enable, 2);
                            break;
                        }
                    }

                }
            }
        }
        else if(measure_usr.key == KEY_START_DOWN)
        {
            key_not_released = 0;
            if(key_type == PAIR_UNPAIR_KEY)
            {
                if(master_pairing_enable)
                {
                    master_pairing_enable = 0;
                    my_dump_str_data(APP_DUMP_EN, "UI PAIR end", 0, 0);
                }

                if(master_unpair_enable)
                {
                    master_unpair_enable = 0;
                }
            }
        }

    }

}





#define GPIO_WAKEUP_KEYPROC_CNT				3
_attribute_ble_data_retention_	static u32 keyScanTick = 0;
_attribute_ble_data_retention_	static int gpioWakeup_keyProc_cnt = 0;

/*
 * @brief      keyboard task handler
 * @param[in]  e    - event type
 * @param[in]  p    - Pointer point to event parameter.
 * @param[in]  n    - the length of event parameter.
 * @return     none.
*/
void proc_keyboard (u8 e, u8 *p, int n)
{
#if 0
    //when key press GPIO wake_up sleep, process key_scan at least GPIO_WAKEUP_KEYPROC_CNT times
    if(e == BLT_EV_FLAG_GPIO_EARLY_WAKEUP)
    {
        gpioWakeup_keyProc_cnt = GPIO_WAKEUP_KEYPROC_CNT;
    }
    else if(gpioWakeup_keyProc_cnt)
    {
        gpioWakeup_keyProc_cnt --;
    }

    if(gpioWakeup_keyProc_cnt || clock_time_exceed(keyScanTick, 10 * 1000))  //keyScan interval: 10mS
    {
        keyScanTick = clock_time();
    }
    else
    {
        return;
    }

    kb_event.keycode[0] = 0;
    int det_key = kb_scan_key (0, 1);


    if (det_key)
    {
        key_change_proc();
    }
#endif

#if ROLE == MASTER
    static u32 key_delay_tick,key_time_start;
    if(clock_time_exceed(key_delay_tick, 10 * 1000))//10ms扫描一次
    {
        key_delay_tick = clock_time();
        //
        // START按键按下

        if(gpio_read(KB))											  // SET按键按下
        {


            if(key_time_start++>=BUTTON_FILTER_TIME) 	 // 按键通过滤波检测
            {

               // printf("kd\n");
            	if(measure_usr.key_down_flag == 0)
                measure_usr.key_down_flag = 1;
            }

            if(key_time_start++>=BUTTON_LONG_TIME)		  // 按键通过滤波检测
            {
                if(measure_usr.key_down_flag == 1)
                {
                   // printf("kh\n");
                    measure_usr.key_update=1;
                    key_time_start = BUTTON_LONG_TIME;
                    measure_usr.key_down_flag = 2;
                    measure_usr.key = KEY_START_HOLD;
                }

            }
        }

        else
        {
            if( measure_usr.key_down_flag == 1)
            {
                //if( measure_usr.key != KEY_PAIR)
                {
                    measure_usr.key_update=1;
                    measure_usr.key_down_flag = 0;
                    measure_usr.key = KEY_START_DOWN;
                    printf("kd2\n");

                }

            }

            key_time_start = 0;
            measure_usr.key_down_flag = 0;
        }

    }
    else
    {
        //printf("key2\n");
        return ;
    }

#endif
}

void key_proc()
{
    proc_keyboard (0,0,0);
    if(measure_usr.key_update)
    {
        if(measure_usr.key == KEY_START_HOLD)
        {
            measure_usr.key_down_cnt = 0;
            if(measure_usr.power_status == ON)
            {
                measure_usr.power_status =OFF;
                printf("off\n");
				gpio_write(GPIO_LED_RED,1);
                //power sleep
               // blc_pm_setSleepMask(PM_SLEEP_LEG_ADV | PM_SLEEP_LEG_SCAN | PM_SLEEP_ACL_SLAVE | PM_SLEEP_ACL_MASTER);
				#if ROLE == MASTER
                cpu_set_gpio_wakeup (KB, Level_High, 1);
                cpu_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_PAD, 0);  //deepsleep
				#endif
            }
            else
            {
            	printf("on\n");
                measure_usr.power_status =ON;
                led_mode_set(LED_NORMAL);
				//gpio_write(GPIO_LED_RED,0);
                //power on
            }


        }
        else if(measure_usr.key == KEY_START_DOWN)
        {
            if(measure_usr.key_down_cnt <= 3)
            {
                measure_usr.key_down_cnt ++;

            }
            else
            {
                if(master_pairing_enable == 0)
                {printf("on33\n");
                    master_pairing_enable = 1;
                    master_unpair_enable = 0;
                    led_mode_set(LED_PAIR);

                }
                else
                {printf("on44\n");
                    master_pairing_enable = 0;
                    master_unpair_enable = 1;
                    led_mode_set(LED_UNPAIR);

                }

                measure_usr.key_down_cnt = 0;

            }
        }
        measure_usr.key = 0;
        measure_usr.key_update = 0;
    }

}


/*
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_ENTER"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
*/
_attribute_ram_code_ void  app_set_kb_wakeup (u8 e, u8 *p, int n)
{
#if (BLE_APP_PM_ENABLE)
    suspend time > 50ms.add GPIO wake_up
    if(((u32)(blc_pm_getWakeupSystemTick() - clock_time())) > 100 * SYSTEM_TIMER_TICK_1MS)
    {
        blc_pm_setWakeupSource(PM_WAKEUP_PAD);  //GPIO PAD wake_up
    }
#endif
}


/**
 * @brief      keyboard initialization
 * @param[in]  none
 * @return     none.*/

void keyboard_init(void)
{
#if (BLE_APP_PM_ENABLE)
    /////////// keyboard GPIO wakeup init ////////
    u32 pin[] = KB_DRIVE_PINS;
    for (int i=0; i<(sizeof (pin)/sizeof(*pin)); i++)
    {
        cpu_set_gpio_wakeup (pin[i], Level_High, 1);  //drive pin pad high level wakeup deepsleep
    }

    blc_ll_registerTelinkControllerEventCallback (BLT_EV_FLAG_SLEEP_ENTER, &app_set_kb_wakeup);
    blc_ll_registerTelinkControllerEventCallback (BLT_EV_FLAG_GPIO_EARLY_WAKEUP, &proc_keyboard);
#endif
}


/*
*
 * @brief   BLE Unpair handle for master
 * @param   none.
 * @return  none.*/

void proc_master_role_unpair(void)
{
#if (!BLE_MASTER_SMP_ENABLE)
    if(blm_manPair.manual_pair && clock_time_exceed(blm_manPair.pair_tick, 2000000))   //@@
    {
        blm_manPair.manual_pair = 0;
    }
#endif


    //terminate and un_pair process, Telink demonstration effect: triggered by "un_pair" key press
    if(master_unpair_enable)
    {

        dev_char_info_t* dev_char_info = dev_char_info_search_by_connhandle(master_unpair_enable); //connHandle has marked on on master_unpair_enable

        if( dev_char_info )  //un_pair device in still in connection state
        {

            if(blc_ll_disconnect(master_unpair_enable, HCI_ERR_REMOTE_USER_TERM_CONN) == BLE_SUCCESS)
            {

                master_disconnect_connhandle = master_unpair_enable; //mark conn_handle

                master_unpair_enable = 0;  //every "un_pair" key can only triggers one connection disconnect


#if (BLE_MASTER_SIMPLE_SDP_ENABLE)
                // delete ATT handle storage on flash
                dev_char_info_delete_peer_att_handle_by_peer_mac(dev_char_info->peer_adrType, dev_char_info->peer_addr);
#endif


                // delete this device information(mac_address and distributed keys...) on FLash
#if (BLE_MASTER_SMP_ENABLE)
                blc_smp_deleteBondingSlaveInfo_by_PeerMacAddress(dev_char_info->peer_adrType, dev_char_info->peer_addr);
#else
                user_tbl_slave_mac_delete_by_adr(dev_char_info->peer_adrType, dev_char_info->peer_addr);
#endif
            }

        }
        else  //un_pair device can not find in device list, it's not connected now
        {

            master_unpair_enable = 0;  //every "un_pair" key can only triggers one connection disconnect
        }

    }
}



#endif   //end of UI_KEYBOARD_ENABLE

void user_gpio_init()
{
    sleep_ms(2000);

    //	gpio_set_func(GPIO_PB4 ,AS_GPIO);                      //设置GPIO功能
    //gpio_set_output_en(GPIO_PB4, 1); 		//输出使能
    //gpio_set_input_en(GPIO_PB4 ,0);			//输入失能
//   gpio_set_func(GPIO_LED_RED,AS_GPIO);					    //设置GPIO功能
    //  gpio_set_output_en(GPIO_LED_RED, 1);		//输出使能
    // gpio_set_input_en(GPIO_LED_RED,0); 		//输入失能


    //1.init the LED pin,for indication
    gpio_set_func(GPIO_LED_RED,AS_GPIO);                       //设置GPIO功能
    gpio_set_output_en(GPIO_LED_RED, 1); 		//输出使能
    gpio_set_input_en(GPIO_LED_RED,0);			//输入失能
    gpio_setup_up_down_resistor(GPIO_LED_RED, PM_PIN_PULLUP_10K);

    gpio_write(GPIO_LED_RED, 0);              	//LED On

//   gpio_set_func(KB,AS_GPIO);                       //设置GPIO功能
//   gpio_set_output_en(KB, 0); 		//输出使能
//   gpio_set_input_en(KB,1);			//输入失能

    //gpio_write(GPIO_LED_RED, 0);              	//LED On
    gpio_set_func(KB,AS_GPIO);
    gpio_set_output_en(KB, 0); 			//enable output
    gpio_set_input_en(KB,1);				//disable input
    gpio_setup_up_down_resistor(KB, PM_PIN_PULLDOWN_100K);
    gpio_set_interrupt(KB,POL_RISING);

    // gpio_write(KB,1);			//输入失能


    gpio_set_func(ECHO,AS_GPIO);
    gpio_set_output_en(ECHO, 0); 			//enable output
    gpio_set_input_en(ECHO,1);				//disable input
    gpio_setup_up_down_resistor(ECHO, PM_PIN_PULLUP_10K);
    // gpio_set_interrupt(ECHO, POL_FALLING);
    gpio_set_interrupt_risc0(ECHO, POL_FALLING);


    gpio_set_func(M_EN,AS_GPIO);                       //设置GPIO功能
    gpio_set_output_en(M_EN, 1); 		//输出使能
    gpio_set_input_en(M_EN,0);			//输入失能
    gpio_setup_up_down_resistor(M_EN, PM_PIN_PULLUP_10K);
    gpio_write(M_EN,1);			//输入失能



    gpio_set_func(CS102_EN,AS_GPIO);                       //设置GPIO功能
    gpio_set_output_en(CS102_EN, 1); 		//输出使能
    gpio_set_input_en(CS102_EN,0);			//输入失能
    gpio_setup_up_down_resistor(CS102_EN, PM_PIN_PULLUP_10K);

    gpio_write(CS102_EN, 1);

    gpio_set_func(CS102_T,AS_GPIO);                       //设置GPIO功能
    gpio_set_output_en(CS102_T, 1); 		//输出使能
    gpio_set_input_en(CS102_T,0);			//输入失能
    gpio_write(CS102_T, 0);




    /*

        	//2.init the SW1 pin,for trigger interrupt
        #if (GPIO_MODE == GPIO_IRQ )
        	gpio_set_func(SW1 ,AS_GPIO);
        	gpio_set_output_en(SW1, 0); 			//enable output
        	gpio_set_input_en(SW1 ,1);				//disable input
        	gpio_setup_up_down_resistor(SW1, PM_PIN_PULLUP_10K);
        	gpio_set_interrupt(SW1, POL_FALLING);
        #elif(GPIO_MODE == GPIO_IRQ_RSIC0)
        	gpio_set_func(SW2 ,AS_GPIO);
        	gpio_set_output_en(SW2, 0); 			//enable output
        	gpio_set_input_en(SW2 ,1);				//disable input
        	gpio_setup_up_down_resistor(SW2, PM_PIN_PULLUP_10K);
        	gpio_set_interrupt_risc0(SW2, POL_FALLING);

        #elif(GPIO_MODE == GPIO_IRQ_RSIC1)
        	gpio_set_func(SW2 ,AS_GPIO);
        	gpio_set_output_en(SW2, 0); 			//enable output
        	gpio_set_input_en(SW2 ,1);				//disable input
        	gpio_setup_up_down_resistor(SW2, PM_PIN_PULLUP_10K);
        	gpio_set_interrupt_risc1(SW2, POL_FALLING);

        #elif(GPIO_MODE == GPIO_TOGGLE)
        	gpio_write(LED1, !gpio_read(LED1));
        	gpio_toggle(LED1);

        #elif(GPIO_MODE == GPIO_HIGH_RESISTOR)
        	gpio_shutdown(GPIO_ALL);				//set all gpio as high resistor except sws and mspi

        #endif
    */
}
static u8 ack_sig=0;
extern u16 handle_m;
void ack_res(u8 ack)
{

    ack_sig = ack;
}


/*函数名：ack_proc()
功能：超时未收到握手信号主动断开连接*/

void ack_proc()
{
    static u32 ack_timeout=0;
    if(ack_sig==0)
    {

        if(clock_time_exceed(ack_timeout, ACK_TIME_OUT)==0)//超时时间内
        {

	            //主机在超时时间内发送握手信号
			#if ROLE == MASTER
	            static u32  send_acktime;
	            if(clock_time_exceed(send_acktime,  1*1000*1000)&& master_conect_status()==1)
	            {
	                printf("sig2\n");
	                send_acktime = clock_time();
	                pkt_pack(0x5a);
	                //gpio_toggle(GPIO_LED_RED);
	                blc_gatt_pushWriteCommand (handle_m, SPP_CLIENT_TO_SERVER_DP_H,tx_buf,tx_buf[2]+5);
	                //printf("ack t\n");measure_usr.start == 1
	            }

			#endif

        }
        /*        static u32  send_acktime;
                if(clock_time_exceed(send_acktime,  1*1000*1000))
                {
                    // printf("sig2\n");
                    send_acktime = clock_time();
                    pkt_pack(0x5a);
                    //gpio_toggle(GPIO_LED_RED);
                    blc_gatt_pushWriteCommand (handle_m, SPP_CLIENT_TO_SERVER_DP_H,tx_buf,tx_buf[2]+5);
                    //printf("ack t\n");
                    measure_start();
                    measure_usr.stop = 0;
                    sensor_power(1);
                    rx_tick = clock_time();
                    measure_start();
                }*/
    }
    else  //握手成功
    {
        // ack = 1;
        ack_timeout = clock_time();
    }
	#if ROLE==SLAVE
	    /*    static u32	send_acktime;
	        if(clock_time_exceed(send_acktime,  1*1000*1000))
	        {
	            // printf("sig2\n");
	            send_acktime = clock_time();
	            // pkt_pack(0x5a);
	            gpio_toggle(GPIO_LED_RED);
	            //blc_gatt_pushWriteCommand (handle_m, SPP_CLIENT_TO_SERVER_DP_H,tx_buf,tx_buf[2]+5);
	            //printf("ack t\n");
	            measure_start();
	            measure_usr.stop = 0;
	            sensor_power(1);
	            rx_tick = clock_time();
	            measure_start();
	        }*/

	    if( GetBle_status()->connection ==0)
	    {

	        if(ack_sig == 1)
	        {
	            printf("ssig\n");
	            ack_sig = 0;

	        }


	    }
	#else
	     if( master_conect_status()==0)
	    {

	        if(ack_sig == 1)
	        {
	            printf("msig\n");
	            ack_sig = 0;

	        }


	    }
#endif
}
void led_mode_set(u8 status)
{
    switch(status)
    {
    case LED_PAIR:
    {
        led_usr.tick = 100000;
        led_usr.status = ON;
    }
    break;
    case LED_UNPAIR:
    {
        led_usr.tick = 500000;
        led_usr.status = ON;
    }
    break;
    case LED_NORMAL:
    {
        led_usr.tick = 1000000;
        led_usr.status = ON;

    }
    break;
    }
}
void led_ctrl()
{
    static u32 led_tick;
    if(led_usr.status == ON)
    {
        if(clock_time_exceed(led_tick, led_usr.tick))
        {
        	// printf("lon\n");
            gpio_toggle(GPIO_LED_RED);
            led_tick= clock_time();
        }
    }
    else
    {

        gpio_write(GPIO_LED_RED,1);
        led_tick= clock_time();

    }
}
extern u16 handle_s;

void parase(u8 tmp)
{
    switch(tmp)
    {
    case 0x4b://通知主机开始测量
    {
        deviceTimeout(0);
        measure_start();
        measure_usr.stop = 0;
        sensor_power(1);
    }
    break;
    case 0x5a://主从机到主机握手
    {

        ack_res(1);
#if ROLE == SLAVE
        if( GetBle_status()->connection ==1)
        {
            printf("sack\n");
            pkt_pack(0x5a);
            blc_gatt_pushHandleValueNotify (handle_s,SPP_SERVER_TO_CLIENT_DP_H, tx_buf,tx_buf[2]+5);

        }
#else
        printf("mack\n");

#endif
    }
    break;
    case 0x5b://配对指示灯
    {
        led_mode_set(LED_PAIR);
    }
    case 0x5c://解绑对指示灯
    {
        led_mode_set(LED_UNPAIR);
    }

    break;

    }
}
void deviceTimeout(unsigned char time)
{
    if(time == 0)
    {
        measure_usr.timeoutFlag = 1;
        measure_usr.timeout = clock_time();

    }
    else
    {
        measure_usr.timeout = clock_time()-measure_usr.timeout;
    }

}
void measure_start()
{
    rx_tick = clock_time();
    measure_usr.start = 1;
    measure_usr.dis = MAX_DIS;
    measure_usr.tick = clock_time();
}
u8 getsn()
{
    static u8 sn;
    sn++;
    return sn;
}


void pkt_pack(u8 ucmd)
{
    u16 calCRC;
    tx_buf[0] = PKT_HEAD;
    tx_buf[1] =  getsn();
    tx_buf[2] =  1;
    tx_buf[3] =  ucmd;
    calCRC =   CRC_Compute(&tx_buf[1],tx_buf[2]+2);
    tx_buf[4] =  (calCRC>>8)&0xFF;;
    tx_buf[5] =  (calCRC)&0xFF;

}

void measure_stop()
{
    measure_usr.stop = 1;
}

cal_rx_time()
{
    measure_usr.rx_time = clock_time()-rx_tick;
    measure_usr.rx_time = measure_usr.rx_time /1000;
    printf("rx:%d\n",measure_usr.rx_time);
}
void sensor_power(u8 flag)
{
    if(flag == 0)//
    {
        gpio_write(CS102_EN, 1);
        gpio_write(CS102_T, 0);
    }
    else //启动
    {
        gpio_write(CS102_EN, 0);
        gpio_write(CS102_T, 0);
        sleep_us(10);
        gpio_write(CS102_T, 1);
        sleep_ms(10);
        gpio_write(CS102_T, 0);

    }
}
void mesure_proc()
{
    static u32 tick_tmp;
    //ack_proc();

#if ROLE == MASTER//for master

    tick_tmp = clock_time()-measure_usr.tick;
    if(measure_usr.start == 1)
    {
        if(tick_tmp>=TIMEOUT_PERIOD)//测量超时
        {
            printf("timeout\n");
            measure_usr.dis = MAX_DIS+1;
            measure_usr.start = 0;
            measure_usr.sum  = 0;
            sensor_power(0);
            measure_stop();
        }
        else
        {
            printf("notimeout\n");
            if(measure_usr.stop == 1)
            {
                //  printf("m3\n");
                measure_usr.time = tick_tmp/1000; 
                measure_usr.dis = measure_usr.time*17;
                printf("dis = %d\n",measure_usr.dis);
                if(measure_usr.dis>=MAX_DIS)
                    measure_usr.dis = MAX_DIS + 2;
                if(measure_usr.dis<=MIN_DIS)
                {
                    printf("warn\n");
                    measure_usr.sum  = measure_usr.sum + 1;
                    measure_usr.dis = MAX_DIS + 3;
                }
                else
                {
                    printf("normal\n");
                    measure_usr.sum  = 0;
                    measure_usr.dis = MAX_DIS + 4;

                }
                measure_usr.start = 0;
                sensor_power(0);
                measure_stop();

            }
        }
    }
    else
    {
        //printf("m6\n");
        sensor_power(0);
        measure_stop();
    }
    if(measure_usr.sum>=10)//超过10次报警，震动
    {
        printf("m7\n");
		if(clock_time_exceed( measure_usr.motor_tick, M_OFF_PERIOD))
		{
			measure_usr.motor_tick = clock_time();
			gpio_write(M_EN,1); 		//输入失能	500 100
			
		}
		else
		{
			if(clock_time_exceed( measure_usr.motor_tick, M_ON_PERIOD)==0)
			{
				gpio_write(M_EN,1); 	   //输入失能

			}
		}
		
    }
    else
    {
        gpio_write(M_EN,1); 		//输入失能

    }
#else//for salve

    deviceTimeout(1);//休眠倒计时
    if(ack_sig == 1)
    {
        if(measure_usr.timeout < SLEEP_TIME_OUT)//震动开关无振动超时判断，大于设置时间进入低功耗休眠
        {
            if(GetBle_status()->connection == 1&&ack_sig==1)
            {

                if(clock_time_exceed( measure_usr.tick, MEASURE_PERIOD))
                {
                    extern u16 handle_s;
                   // gpio_toggle(GPIO_LED_RED);
                    pkt_pack(0x4b);
                    blc_gatt_pushHandleValueNotify (handle_s,SPP_SERVER_TO_CLIENT_DP_H, tx_buf,tx_buf[2]+5);
                    sleep_us(100);
                    measure_start();
                    sensor_power(1);
                    printf("m8\n");
                    tick_tmp = 0;

                }

            }
            else
            {
                // printf("m9\n");
                //power off
                sensor_power(0);
                measure_stop();
            }
        }

    }



#endif
}
ble_stru ble_usr;

void ble_status(u8 flag)
{
    if(flag == 0)
    {
        ble_usr.connection = 0;
    }
    else
        ble_usr.connection = 1;
}

ble_stru *GetBle_status()
{
    return &ble_usr;
}

measure_stru *getmeasrue()
{
    return &measure_usr;

}
void init_measure()
{
#if ROLE == MASTER
    measure_usr.power_status = OFF;
#endif
}
void ui_proc()
{

    ack_proc();
    led_ctrl();
    //if(ack_sig==1)
    mesure_proc();
}




