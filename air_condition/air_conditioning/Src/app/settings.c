#include "settings.h"
#include "adc.h"
#include "key.h"
#include "io.h"
#include "display.h"
#include "pid.h"
#include "EEPROM.h"
#include "machine.h"
params_stru settings_params;
extern adc_io_str adc_io;
mode_stru mode;
PID xPID, yPID;

void write_dipay_ram(unsigned char mode);


void init_Pid()
{
   xPID.Kp = 30;
   xPID.Ki = 10;
   xPID.Kd = 1;
   yPID.Kp = 30;
   yPID.Ki = 10;
   yPID.Kd = 1;

}
mode_stru *get_params_mode()
{
    mode_stru *tmp;
    tmp = &mode;

    return tmp;
}
void airMachine_ctrl(unsigned char umode)
{
/*
adc_io.adc_result[0];//humid
adc_io.adc_result[1];//tar_env_temper
adc_io.adc_result[2];//equip_env_temper
adc_io.adc_result[3];//water temperature
adc_io.adc_result[4];machine_air_temper
adc_io.adc_result[5];target_wind_temper
adc_io.adc_result[6];

*/

    settings_params.tar_env_temper =adc_io.adc_result[1];
    settings_params.equip_env_temper = adc_io.adc_result[2];
    settings_params.machine_air_temper = adc_io.adc_result[4];
    settings_params.target_wind_temper =adc_io.adc_result[5];
	   settings_params.humid[0] = adc_io.adc_result[0];
 	  settings_params.humid[1] = adc_io.adc_result[0];	


    float ca,cb;
    int16_t xError, yError;
    int16_t xPWM, yPWM;
	if(umode == ON)
	{
		ca = settings_params.tar_set_temper[0]-settings_params.tar_env_temper;
		cb = settings_params.tar_set_temper[0]-settings_params.target_wind_temper;
		// PID calculation
		xError = ca;
		
		yError = cb;
		xPWM = calcPID(&xPID, xError);
		yPWM = calcPID(&yPID, yError);
		
		if(ca>=5)
		{
			if(cb<=1)
			{
				TransmitCommand(xPWM,umode);
			}
			else if(cb>=2)
			{
				if((xPWM+yPWM)>=MAX_SPEED)
					TransmitCommand(MAX_SPEED,umode);
				else
					TransmitCommand(xPWM+yPWM,umode);
			}
			else
			{
				TransmitCommand(xPWM,umode);
			}
		}
		else if(ca<5&&ca>=0)
		{
			if(cb<=0)
			{
				TransmitCommand(xPWM,umode);
			}
			else if(cb>=1)
			{
				if((xPWM+yPWM)>=MAX_SPEED)
					TransmitCommand(MAX_SPEED,umode);
				else
					TransmitCommand(xPWM+yPWM,umode);
			}
			else
			{
				TransmitCommand(yPWM,umode);
			}
		
		
		}
		else if(ca<0&&ca>=-0.5)
		{
			TransmitCommand(MIN_SPEED,umode);
		
		}
		else if(ca<-0.5)
		{
			TransmitCommand(MIN_SPEED,OFF);
			device_ctrl(DEV_ALL,OFF);
		
		}
		else if(ca>=0.5)
		{
			TransmitCommand(MIN_SPEED,ON);
			mode.flag_mark = 0;
		
		}
		if(settings_params.machine_air_temper>=MAX_MACHINE_AIRT)
		{
			TransmitCommand(MIN_SPEED,OFF);
		}

	}
	else
	{
		TransmitCommand(MIN_SPEED,OFF);

	}



}
unsigned char command_tx_time_loop(unsigned char umode)
{
    static uint32_t gettime;


    if(mode.flag_mark == 0x00)//2s启动风扇计时标志
    {
        mode.flag_mark = 1;
        gettime = HAL_GetTick();
        mode.last_mode_no  = umode;
		return 0;
    }

    if((HAL_GetTick() -gettime)>=FAN_RUN_TIME&&mode.flag_mark==1)
    {
        mode.flag_mark=2;
        return 1;
    }
    if((HAL_GetTick() -gettime)>=TX_COMMAND_TIME&&mode.flag_mark==2)
    {
        gettime = HAL_GetTick();
        return 2;
    }

}
void machine_mode_control(unsigned char umode)
{

    if(mode.flag_mark == 0x00)
        command_tx_time_loop(umode);

    if(mode.flag_mark)
    {
        if(umode == COLD)
        {
            if(mode.last_mode_no == COLD)
            {

                device_ctrl(FAN_COOL,ON);
                if(command_tx_time_loop(umode)==1)
                {
                    mode.flag_mark==2;
                    settings_params.present_status = 0x02;
                    airMachine_ctrl(ON);
                    device_ctrl(FAN_HOT,ON);
					
                }
                if(command_tx_time_loop(umode)==2)
                {
                    settings_params.present_status = 0x02;
                }

            }
        }
        else if(umode == HOT)
        {
            if(mode.last_mode_no == HOT)
            {
                device_ctrl(FAN_HOT,ON);
                if(command_tx_time_loop(umode)==1)
                {
                    settings_params.present_status = 0x02;
                    device_ctrl(FAN_COOL,ON);
                    airMachine_ctrl(ON);
                }
                if(command_tx_time_loop(umode)==2)
                {
                    settings_params.present_status = 0x02;
                }

            }

        }
        else if(umode == COLD_HUM_COLD||umode == COLD_HUM_HUM)
        {
            if(mode.last_mode_no == COLD_HUM_COLD||mode.last_mode_no == COLD_HUM_HUM)
            {
                device_ctrl(PUMP,ON);
                if(command_tx_time_loop(umode)==1)
                {
                    settings_params.present_status = 0x02;
                    device_ctrl(FAN_COOL,ON);
                    device_ctrl(HUMID,ON);	
                    airMachine_ctrl(ON);
					device_ctrl(PUMP,ON);

                }
                if(command_tx_time_loop(umode)==2)
                {
                    settings_params.present_status = 0x02;
                }

            }

        }
        else if(umode == FAST_COLD||umode == FAST_COLD_HUM)
        {
            if(mode.last_mode_no == FAST_COLD||mode.last_mode_no == FAST_COLD_HUM)
            {
                device_ctrl(PUMP,ON);
                if(command_tx_time_loop(umode)==1)
                {
                    settings_params.present_status = 0x02;
                    device_ctrl(FAN_COOL,ON);
                    airMachine_ctrl(ON);
				    device_ctrl(HUMID,ON);	
					device_ctrl(PUMP,ON);

                }
                if(command_tx_time_loop(umode)==2)
                {
                    settings_params.present_status = 0x02;
                }

            }

        }
        else
        {

        }

    }



}

unsigned char tar_set_temper[6];//0:target cold temp;1:target hot temp;2:target cold and humid temp;3:fast colde temp;
unsigned char tar_env_temper[4];//4:target cold and humid humid 5:fast humid
void write_params()
{
	uint32_t p[7];
	//if(p[0]==0x5a)
	{   
	   flash_write(0,0x5a,1); 
	   flash_write(1,mode.modeNo,1); 
	   flash_write(2,mode.last_mode_no,1); 
	   flash_write(3,settings_params.tar_set_temper[0],1); 
	   flash_write(4,settings_params.tar_set_temper[1],1); 
	   flash_write(5,settings_params.tar_set_temper[2],1); 
	   flash_write(6,settings_params.tar_set_temper[3],1); 
	}

}
void init_params()
{
  uint32_t p[7];
    flash_read(0,p,7); 
	if(p[0]==0x5a)
	{
		mode.modeNo =  p[1];
		mode.last_mode_no =  p[2];
		settings_params.tar_set_temper[0] = p[3];//0:target cold temp;
		settings_params.tar_set_temper[1]=p[4];
		settings_params.tar_set_temper[2]=p[5];
		settings_params.tar_set_temper[3]=p[6];
	}
	else
	{
		mode.modeNo =  STOP_MODE;
		mode.last_mode_no =  COLD;
		settings_params.tar_set_temper[0] = 25;//0:target cold temp;
		settings_params.tar_set_temper[1]=18;
		settings_params.tar_set_temper[2]=25;
		settings_params.tar_set_temper[3]=25;
		settings_params.humid[0]=INIT_SET_HUM;
		settings_params.humid[1]=INIT_SET_HUM;
		write_params();

	}



}
void work_mode_process()
{
    uint32_t gettime;
    key_stru *key_tmp;
    key_tmp=key_process();

    if(key_tmp->key_status == 0x01)    //key1 setting or display mode setting
    {
        if(mode.status == WORK_OFF||mode.status == POWER_ON)//开机状态并且系统未运行时可以进行参数设置
        {
            mode.mode = SETTING_MODE;    //setting mode

            if(mode.modeNo<(MAX_MODE_NO-1))
            {
                mode.modeNo = mode.modeNo + 1;
                mode.last_mode_no = mode.modeNo;
            }
            else
            {
                mode.mode = NORMAL;	 //normal mode
                mode.last_mode_no = mode.modeNo;
                mode.modeNo = STAND_MODE;
            }

        }
        else if(mode.status == WORK_ON)//开机状态并且系统运行时可以查询参数
        {
            if(mode.manual == 0&&mode.modeNo != RECYCLE_DISPLAY)
                mode.manual = 1;
        }
    }
    else if(key_tmp->key_status == 0x02)
    {
        if(mode.status == WORK_ON)
        {
            mode.mode = NORMAL;    //normal mode
            if(mode.modeNo !=RECYCLE_DISPLAY)
            {
                mode.last_mode_no = mode.modeNo;
                mode.modeNo = RECYCLE_DISPLAY;
                mode.manual = 0;
            }
            else
            {
                mode.modeNo =  mode.last_mode_no;
                mode.manual = 0;

            }
        }
        else if(mode.status == WORK_OFF||mode.status == POWER_ON)
        {
            mode.modeNo = mode.last_mode_no;
            //mode.last_mode_no = 0;
            mode.mode = NORMAL;
        }

    }
    static unsigned char i,j;
    switch(mode.modeNo)
    {
    case COLD:
        i=0;
        j=2;
        break;
    case HOT:
        i=1;
        j=2;
        break;
    case COLD_HUM_COLD:
        i=2;
        j=2;
        break;
    case COLD_HUM_HUM:
        i=5;
        j=0;
        break;
    case FAST_COLD:
        i=3;
        j=2;
        break;
    case FAST_COLD_HUM:
        i=5;
        j=1;
        break;
    case RECYCLE_DISPLAY:
        break;
        //case COLD:i=0;break;
    }

    if(key_tmp->key_status == 0x03)  //kye 2 inc key
    {
        if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD_HUM)
        {

            if(i<5)
            {
                if(settings_params.tar_set_temper[i]<=MAX_SET_TEMPER)
                    settings_params.tar_set_temper[i] = settings_params.tar_set_temper[i] + 1;
                else
                    settings_params.tar_set_temper[i] = 0;
            }
            else
            {
                if(settings_params.humid[i]<=MAX_SET_HUM)
                    settings_params.humid[j] = settings_params.humid[j] + 1;
                else
                    settings_params.humid[j] = 0;

            }

        }
    }
    else if(key_tmp->key_status == 0x04)
    {
        if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD_HUM)
        {
            while(1)
            {
                if((HAL_GetTick()-gettime)>=300)
                {
                    gettime = HAL_GetTick();
                    if(i<5)
                    {
                        if(settings_params.tar_set_temper[i]<=MAX_SET_TEMPER)
                            settings_params.tar_set_temper[i] = settings_params.tar_set_temper[i] + 1;
                        else
                            settings_params.tar_set_temper[i] = 0;
                    }
                    else
                    {
                        if(settings_params.humid[j]<=MAX_SET_HUM)
                            settings_params.humid[j] = settings_params.humid[j] + 1;
                        else
                            settings_params.humid[j] = 0;

                    }
                    write_dipay_ram(2);

                    if(get_key_level(2)==1)
                        break;
                }

            }
        }

    }

    if(key_tmp->key_status == 0x05)//key3
    {
        if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD_HUM)
        {

            if(i<5)
            {
                if(settings_params.tar_set_temper[i]>MIN_SET_TEMPER)
                    settings_params.tar_set_temper[i] = settings_params.tar_set_temper[i] - 1;
                else
                    settings_params.tar_set_temper[i] = MIN_SET_TEMPER;

            }

        }
    }
    else if(key_tmp->key_status == 0x06)
    {
        if(mode.modeNo >=COLD && mode.modeNo<=FAST_COLD_HUM)
        {
            while(1)
            {
                if((HAL_GetTick()-gettime)>=300)
                {
                    gettime = HAL_GetTick();
                    if(i<5)
                    {
                        if(settings_params.tar_set_temper[i]>MIN_SET_TEMPER)
                            settings_params.tar_set_temper[i] = settings_params.tar_set_temper[i] - 1;
                        else
                            settings_params.tar_set_temper[i] = MIN_SET_TEMPER;

                    }
                    write_dipay_ram(2);

                    if(get_key_level(3)==1)
                        break;

                }

            }
        }

    }
    if(key_tmp->key_status == 0x07)//key4 shot hit
    {
        if(mode.status == WORK_OFF)
        {
            mode.status = WORK_ON;  //system start
           
            if(mode.last_mode_no)
                mode.modeNo = mode.last_mode_no;
            else
                mode.modeNo = COLD;
        }
        else if(mode.status == WORK_ON)
        {
            mode.status = WORK_OFF;//system stop
            mode.last_mode_no = mode.modeNo;
            mode.modeNo = STAND_MODE;

        }
        else if(mode.status == POWER_ON)
        {
            mode.status = WORK_ON;//system stop
            if(mode.last_mode_no)
                mode.modeNo = mode.last_mode_no;
            else
                mode.modeNo = STAND_MODE;

        }
        mode.mode = NORMAL;	 //normal mode
        init_seg();
    }
    else if(key_tmp->key_status == 0x08)//key4 long hit,power off or power on
    {
        if(mode.status == WORK_OFF)
        {
            mode.status = POWER_OFF;  //power off
            mode.mode = NORMAL;
            if(mode.last_mode_no)
                mode.last_mode_no=mode.last_mode_no;
            else
                mode.last_mode_no=mode.modeNo;
            mode.modeNo = STOP_MODE;

        }
        else if(mode.status == WORK_ON)
        {
            mode.mode = NORMAL;
            mode.status = POWER_OFF; //power off
            mode.last_mode_no=mode.modeNo;
            mode.modeNo = STOP_MODE;

        }
        else if(mode.status == POWER_OFF)
        {
            mode.status = POWER_ON;
//			 if(mode.last_mode_no == STOP_MODE)
//			 	mode.modeNo=STAND_MODE;
//			 else
//			 mode.modeNo=mode.last_mode_no;
            mode.modeNo = STAND_MODE;

        }
        else if(mode.status == POWER_ON)
        {
            mode.mode = NORMAL;
            mode.status = POWER_OFF;
            mode.last_mode_no=mode.modeNo;
            mode.modeNo = STOP_MODE;

        }
        init_seg();

    }


    SetKeyNum();


}

void recycle_dis_deal(unsigned char mode_sel,unsigned char cycle_flag)
{
    unsigned char tmp[5];
    switch(cycle_flag)
    {
    case 0:
        if(mode_sel ==COLD)
        {
            tmp[0] = SET_ST;
            display_dat_deal(settings_params.tar_set_temper[0],tmp,3);
        }
        else if(mode_sel ==HOT)
        {
            tmp[0] = SET_ST;
            display_dat_deal(settings_params.tar_set_temper[1],tmp,3);
        }
        else if(mode_sel ==COLD_HUM_COLD||mode_sel ==COLD_HUM_HUM)
        {
            tmp[0] = SET_ST;
            display_dat_deal(settings_params.tar_set_temper[2],tmp,3);
        }
        else if(mode_sel ==FAST_COLD||mode_sel ==FAST_COLD_HUM)
        {
            tmp[0] = SET_ST;
            display_dat_deal(settings_params.tar_set_temper[3],tmp,3);
        }
        break;
    case 1:
        if(mode_sel ==COLD)
        {
            tmp[0] = SET_AT;
            display_dat_deal(settings_params.tar_env_temper,tmp,3);
        }
        else if(mode_sel ==HOT)
        {
            tmp[0] = SET_AT;
            display_dat_deal(settings_params.tar_env_temper,tmp,3);
        }
        else if(mode_sel ==COLD_HUM_COLD||mode_sel ==COLD_HUM_HUM)
        {
            tmp[0] = SET_AT;
            display_dat_deal(settings_params.tar_env_temper,tmp,3);
        }
        else if(mode_sel ==FAST_COLD||mode_sel ==FAST_COLD_HUM)
        {
            tmp[0] = SET_AT;
            display_dat_deal(settings_params.tar_env_temper,tmp,3);
        }
        break;
    case 2:
        tmp[0] = SET_AT;
        display_dat_deal(settings_params.machine_air_temper,tmp,3);
        break;
    case 3:
        tmp[0] = SET_AT;
        display_dat_deal(settings_params.speed,tmp,1);
        break;
    case 4:
        tmp[0] = SET_I;
        display_dat_deal(settings_params.current,tmp,3);
        break;

    }
}
void write_dipay_ram(unsigned char umode)
{
   static uint32_t gettime;
    unsigned char tmp[4];
    unsigned char tcode[9]= { SET_CM, SET_CHM,SET_HM,SET_FHM,SET_H,SET_ST,SET_WL,SET_I,SET_AT};
    static unsigned char toggle_flag=0;

    if(umode == 0)
    {
        if((HAL_GetTick()-gettime)>=RECYCLE_TIME)
        {

            gettime = HAL_GetTick();
            switch(mode.modeNo)
            {

            case STAND_MODE:
                tmp[0]=SEG_R;
                tmp[1]=SEG_E;
                tmp[2]=SEG_A;
                tmp[3]=SEG_D;
                display_dat_deal(settings_params.tar_env_temper,tmp,4);
                break;
            case COLD:
                tmp[0]=SET_AT;
                display_dat_deal(settings_params.tar_env_temper,tmp,3);
                break;
            case HOT:
                tmp[0]=SET_AT;
                display_dat_deal(settings_params.tar_env_temper,tmp,3);
                break;
            case COLD_HUM_COLD:
            case COLD_HUM_HUM:
                if(toggle_flag ==0)
                {
                    tmp[0] = SET_AT;
                    display_dat_deal(settings_params.tar_env_temper,tmp,3);
                }
                else
                {
                    tmp[0] = SET_H;
                    display_dat_deal(settings_params.humid[0],tmp,3);
                }
                toggle_flag = ~toggle_flag;
                break;

            case FAST_COLD:
            case FAST_COLD_HUM:
                if(toggle_flag ==0)
                {
                    tmp[0] = SET_AT;
                    display_dat_deal(settings_params.tar_env_temper,tmp,3);
                }
                else
                {
                    tmp[0] = SET_H;
                    display_dat_deal(settings_params.humid[1],tmp,3);
                }
                toggle_flag = ~toggle_flag;
                break;

            }

        }

    }
    else if(umode == 1)
    {
        if((HAL_GetTick()-gettime)>=RECYCLE_TIME)
        {
            if(mode.manual == 1)
                mode.manual = 0;
            gettime = HAL_GetTick();
            switch(mode.last_mode_no)
            {
            case STAND_MODE:
                recycle_dis_deal(COLD,toggle_flag);
                toggle_flag = toggle_flag +1;
                break;

            case COLD:
                recycle_dis_deal(COLD,toggle_flag);
                toggle_flag = toggle_flag +1;
                break;
            case HOT:
                recycle_dis_deal(HOT,toggle_flag);
                toggle_flag = toggle_flag +1;
                break;
            case COLD_HUM_COLD:
            case COLD_HUM_HUM:
                recycle_dis_deal(COLD_HUM_COLD,toggle_flag);
                toggle_flag = toggle_flag +1;
                break;


            case FAST_COLD:
            case FAST_COLD_HUM:
                recycle_dis_deal(FAST_COLD,toggle_flag);
                toggle_flag = toggle_flag +1;
                break;

            }
            if(toggle_flag>=5)
                toggle_flag = 0;

        }

    }
    else if(umode == 2)
    {
        if((HAL_GetTick()-gettime)>=RECYCLE_TIME)
        {
            gettime = HAL_GetTick();
            switch(mode.modeNo)
            {
            case STAND_MODE:
                display_dat_deal(settings_params.tar_set_temper[0],&tcode[0],3);
                break;
            case COLD:
                display_dat_deal(settings_params.tar_set_temper[0],&tcode[0],3);
                break;
            case HOT:
                display_dat_deal(settings_params.tar_set_temper[1],&tcode[2],3);
                break;
            case COLD_HUM_COLD:
                display_dat_deal(settings_params.tar_set_temper[2],&tcode[1],3);
                break;
            case COLD_HUM_HUM:
                display_dat_deal(settings_params.humid[0],&tcode[4],3);
                break;
            case FAST_COLD:
                display_dat_deal(settings_params.tar_set_temper[3],&tcode[3],3);
                break;
            case FAST_COLD_HUM:
                display_dat_deal(settings_params.humid[1],&tcode[4],3);
                break;
            case RECYCLE_DISPLAY:
                break;
                //case COLD:i=0;break;
            }

        }

    }
    else if(umode == 3)
    {
        tmp[0] = MINUS;
        tmp[1] = MINUS;
        tmp[2] = MINUS;
        tmp[3] = MINUS;

        display_dat_deal(settings_params.tar_env_temper,tmp,4);

    }
}
void run_process()
{

    /**************************************设备控制*************************************************/
    work_mode_process();//按键处理
    adc_process();//模拟量采集
    get_io();
    settings_params.tar_env_temper =adc_io.adc_result[1];
    settings_params.equip_env_temper = adc_io.adc_result[2];
    settings_params.machine_air_temper = adc_io.adc_result[3];
    settings_params.target_wind_temper =adc_io.adc_result[4];
	   settings_params.humid[0] = adc_io.adc_result[0];
 	  settings_params.humid[1] = adc_io.adc_result[0];	
    // if( adc_io.fault_status ==0&&mode.status == WORK_ON)//启动正常
    if( mode.status == WORK_ON)//启动正常
    {
		machine_mode_control(mode.modeNo);
		if(HAL_GPIO_ReadPin(GPIOA,SW1_Pin)==1)
			device_ctrl(GATE,ON);
		else
			device_ctrl(GATE,OFF);				
    }
	else
	{
	   mode.flag_mark = 0;
		airMachine_ctrl(OFF);
		device_ctrl(DEV_ALL,OFF);

	}
/*****************************************显示控制******************************************************/
    if(get_params_mode()->mode ==NORMAL&&get_params_mode()->status >=WORK_OFF)//正常模式
    {
        /*if( adc_io.fault_status!=0)
        {

           display_dat_deal(adc_io.fault_status,1,2);	//显示故障码
        }
        else*/                                  //显示正常数据
        {
            if(mode.modeNo == RECYCLE_DISPLAY||mode.manual)//自动循环显示正常数据
            {

                write_dipay_ram(1);
            }
            else
            {
                write_dipay_ram(0);
            }
        }
    }
    else if(get_params_mode()->mode ==SETTING_MODE)//显示设置数据
    {
        write_dipay_ram(2);

    }
    if(mode.modeNo==STOP_MODE)
    {
        write_dipay_ram(3);

    }

}
