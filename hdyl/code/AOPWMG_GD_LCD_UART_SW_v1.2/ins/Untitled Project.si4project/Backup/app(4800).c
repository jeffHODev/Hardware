#include "app.h"
#include "app_lcd.h"
#include "sensor.h"
#include "devCtrl.h"
#include "sys.h"
#include "bsp_beep.h"
#include "inout.h"
#include "app_esp8266.h"
#include "app_lcd.h"
#include "bsp_cpu_flash.h"
#include "pid.h"
#include<stdlib.h>
#include "cmd_process.h"
#include <string.h>
#include "bsp_beep.h"

void SystemParamsSave(void);
void SystemParamsRead(void);


//目标tds 2.5----500
//目标tds 3 ------650
//目标tds 3.5 ------950   +30----0.1
//目标tds 4 ------1100


uint32_t dstTds=1100;//目标tds 2.5----500
//unsigned char module_reset_flag=0;
work_params_stru work_params;
static unsigned char shunt_flag=0;

void app_init()
{
    // SystemParamsRead();
    //adcInit();
    RS485_Init();
    // inout_Init();
    // work_params.work_mode = ON;

}
/*********************************************************************
function:功能异常超时判断
params:
**********************************************************************/
unsigned char TickTimeoutAb(unsigned char TickNum,unsigned char BitOper,uint32_t timeout)
{
    if(GetTickStatus(TickNum)!=1&& GetTickNum(TickNum)==1)//从正常转为异常
    {
        registerTick(TickNum, 0, 0,1);//定时器复位
        registerTick(TickNum, timeout, 1,0);//超时计时开始
    }
    else
    {
        if(GetTickNum(TickNum)==0||GetTickStatus(TickNum)==2)
        {
            registerTick(TickNum, 0, 0,1);//定时器复位
            registerTick(TickNum, timeout, 1,0);//超时计时开始
        }

    }
    if( GetTickResult(TickNum)==1&&GetTickStatus(TickNum)==1)
    {
        // status = status | BitOper;
        registerTick(TickNum, 0, 0,1);//定时器复位
        return BitOper;
    }
    else
    {

        return 0;
    }

}

unsigned char TickTimeoutNor(unsigned char TickNum,unsigned char BitOper,uint32_t timeout)
{
    if(GetTickStatus(TickNum)==1&&GetTickResult(TickNum)==1)//正常
    {
        registerTick(TickNum, 0, 0,1);//定时器复位
        registerTick(TickNum, timeout, 2,0);//超时计时开始
        if(GetTickStatus(TickNum)==0)
            return BitOper;
    }
    else
    {
        if(GetTickNum(TickNum)==0||GetTickStatus(TickNum)==1)
        {
            registerTick(TickNum, 0, 0,1);//定时器复位
            registerTick(TickNum, timeout, 2,0);//超时计时开始
        }

    }
    if( GetTickResult(TickNum)==1&&GetTickStatus(TickNum)==2)
    {
        registerTick(TickNum, 0, 0,1);//定时器复位
        return BitOper;
    }

    return 0xff;

}

unsigned char abnormalDec()
{
    static unsigned char status;

    if(work_params.init_flag == 0)//上电初始化，电解水从废水通道排出
        registerTick(StART_TICK_NO, INIT_WASH_TIME_SETTING,1, 0);
    if( GetTickResult(StART_TICK_NO)==1)
    {
        work_params.init_flag = 1;//时间到，走正常逻辑工作
        registerTick(StART_TICK_NO, 0,0,1);
    }


    //********************************************************************************

    if(GetSensor()->flow >= MAX_FLOW|| GetSensor()->flow <= MIN_FLOW)//流量异常
    {
        if(GetSensor()->status[HSW_INDEX]==0&&GetInOut()->key_cali_mode == 0)//水龙头开的情况下流量异常
        {
            status = status | TickTimeoutAb(FLOW_TICK_NO,0x01,30000);
        }

        else
            status = status & 0xfe;
    }
    else
    {
        status = status & TickTimeoutNor(FLOW_TICK_NO,0xfe,1000);
    }

//********************************************************************************
    if(GetSensor()->tds1>= MAX_TDS1_VALUE|| GetSensor()->tds1 <= MIN_TDS1_VALUE)//原水tds异常
    {

        status = status | TickTimeoutAb(TDS1_TICK_NO,0x40,1000);

    }
    else
    {
        status = status & TickTimeoutNor(TDS1_TICK_NO,0xbf,MAX_SHORT_TICK);
    }

    if(dstTds<150)
        dstTds = 150;
    if( GetSensor()->tds2 <= MIN_TDS_VALUE||(GetSensor()->tds2 <=dstTds-150))//tds异常
    {
        //GetSensor()->tds2 >= MAX_TDS_VALUE||

        status = status | TickTimeoutAb(TDS_TICK_NO,0x02,MAX_TICK);
        if(status &0x02)
        {
            GetSensor()->err_flag =GetSensor()->err_flag |0x01;//超时计时

        }
        if(GetSensor()->err_flag &0x01==1)//3分钟稳定认定为参数不合格
        {
            if((HAL_GetTick()-GetSensor()->delay_timeout )>=WASH_TIME)
            {
                GetSensor()->err_flag = GetSensor()->err_flag & 0xfe;
                GetSensor()->err_flag = GetSensor()->err_flag | 0x10;//超时
            }
        }
    }
    else
    {
        GetSensor()->err_flag = 0 ;
        status = status & TickTimeoutNor(TDS_TICK_NO,0xfd,MAX_SHORT_TICK);
        if((status & 0x02) == 0)
            GetSensor()->delay_timeout = HAL_GetTick();
    }
    //********************************************************************************

    if(GetSensor()->ph>= MAX_PH||GetSensor()->ph <= MIN_PH)//ph异常
    {
        status = status | TickTimeoutAb(PH_TICK_NO,0x04,MAX_TICK);
    }
    else
    {
        /* registerTick(PH_TICK_NO, 0, 1);
         status = status & 0xfb;*/
        status = status & TickTimeoutNor(PH_TICK_NO,0xfb,MAX_SHORT_TICK);
    }
    //********************************************************************************

    if(GetSensor()->orp >= MAX_ORP||GetSensor()->orp <= MIN_ORP)//orp异常
    {
        status = status | TickTimeoutAb(ORP_TICK_NO,0x08,MAX_TICK);
    }
    else
    {
        status = status & TickTimeoutNor(ORP_TICK_NO,0xf7,MAX_SHORT_TICK);
    }
    //********************************************************************************

    if(GetSensor()->water_level == WATER_F	||GetSensor()->water_level == WATER_L
            ||GetSensor()->water_level == WATER_M)//水位异常
    {
        if(GetSensor()->water_level != WATER_M)
        {
            status = status | 0x20;//TickTimeoutAb(WATER_TICK_NO,0x20,WATER_LSHORT_TICK);

        }
        else
        {
            status = status |TickTimeoutAb(WATER_TICK_NO,0x20,WATER_L_DELAY_TICK);//

        }

        GetSensor()->salt_water_timeout++;

    }
    else
    {
        GetSensor()->salt_water_timeout=0;
        status = status & 0xdf;
    }
    //********************************************************************************

    if(GetSensor()->swH ==ON)//高压开关异常
    {
        status = status | 0x10;
        //status = status | TickTimeoutAb(SW_TICK_NO,0x10,400);
    }
    else
    {
        //status = status & TickTimeoutNor(SW_TICK_NO,0xef,200);
        status = status & 0xef;
    }


    if(GetSensor()->salt_water_timeout>= SALT_TIMEOUT)//falut prevent  from water sensor fault
    {
        status = status & 0xdf;
    }

    return status;
}


/***********************************************************************
功能：根据流量范围调整进水tds

************************************************************************/
static float flow_switch;// 0-1  1-2  2-3  3-4

void Flow_Init()
{
    // dstTds = 1100;
    static unsigned char start_flag=0;
    if(GetSensor()->flow<=0)
    {
        start_flag = 0;
        registerTick(TDS_SETTING_TICK_NO, 0,0,1);
        switch(FLOW_SIZE)
        {
        case 1:
            flow_switch = TDS_LEVEL1 ;
            break;
        case 2:
            flow_switch = TDS_LEVEL1 ;
            break;
        case 3:
            flow_switch = TDS_LEVEL2 ;
            break;
        case 4:
            flow_switch = TDS_LEVEL3 ;
            break;
        default:
            flow_switch = TDS_LEVEL4 ;
            break;
        }
    }
    else
    {

        registerTick(TDS_SETTING_TICK_NO, 5000,1,0);
        //目标tds 2.5----500
        //目标tds 3 ------650
        //目标tds 3.5 ------950	+30----0.1
        //目标tds 4 ------1100
        //2.5-----500
        //2.5-3--- 500
        //3-3.5----650
        //3.5-4---	950

        if(GetTickResult(TDS_SETTING_TICK_NO)==1)
        {
            /*if(GetSensor()->flow<=2.5)
                flow_switch = TDS_LEVEL1 ;
            else if(GetSensor()->flow<=3)//2.5---3  500
            {
                flow_switch = (GetSensor()->flow-2.5)*300+TDS_LEVEL1;
                if(flow_switch<TDS_LEVEL1)
                    flow_switch = TDS_LEVEL1;
            }
            else if(GetSensor()->flow<=3.5)//3---3.5 650
            {
                flow_switch = (GetSensor()->flow-3)*600+TDS_LEVEL2;
                if(flow_switch<TDS_LEVEL2)
                    flow_switch = TDS_LEVEL2;
            }

            else if(GetSensor()->flow<=4)//3.5---4    950	+30----0.1
            {
                flow_switch = (GetSensor()->flow-3.5)*300+TDS_LEVEL3;
                if(flow_switch<TDS_LEVEL3)
                    flow_switch = TDS_LEVEL3;
            }

            else
                flow_switch = TDS_LEVEL4 ;*/
            if(GetSensor()->flow<=2.5)//450
                flow_switch = TDS_LEVEL1 ;
            else if(GetSensor()->flow<=3)//2.5---3  450--500
            {
                flow_switch = (GetSensor()->flow-2.5)*100+TDS_LEVEL1;
                if(flow_switch<TDS_LEVEL1)
                    flow_switch = TDS_LEVEL1;
            }
            else if(GetSensor()->flow<=3.5)//3---3.5 500-620
            {
                flow_switch = (GetSensor()->flow-3)*300+TDS_LEVEL2;
                if(flow_switch<TDS_LEVEL2)
                    flow_switch = TDS_LEVEL2;
            }

            else //(GetSensor()->flow<=4)//3.5---4    620-920	+30----0.1
            {
                flow_switch = (GetSensor()->flow-3.5)*600+TDS_LEVEL3;
                if(flow_switch>=TDS_LEVEL4)//1000
                   flow_switch = TDS_LEVEL4;
            }
            //else
               // flow_switch = TDS_LEVEL5 ;//1000
            registerTick(TDS_SETTING_TICK_NO, 0,0,1);

        }
        else
        {
            if(start_flag == 0)
            {
                start_flag = 1;
                switch(FLOW_SIZE)
                {
                case 1:
                    flow_switch = TDS_LEVEL1 ;
                    break;
                case 2:
                    flow_switch = TDS_LEVEL1 ;
                    break;
                case 3:
                    flow_switch = TDS_LEVEL2 ;
                    break;
                case 4:
                    flow_switch = TDS_LEVEL3 ;
                    break;
                default:
                    flow_switch = TDS_LEVEL4 ;
                    break;
                }
            }
        }

    }


    dstTds = flow_switch;
    pid_init_flow(dstTds);

}
static int32_t pwm2;
unsigned char pid_cnt;
extern unsigned char tds_flag;
void FlowCtrl()
{
    double tds_out;
    static uint32_t pid_cnt;
    //dstTds = 1100;
    tds_out    = GetSensor()->tds2;
    if(PID_TYPE == 2)
    {

        registerTick(PID_SETTING_TICK_NO, 20, 1,0);//超时计时开始

        // if(GetTickResult(PID_SETTING_TICK_NO)==0)
          //  return ;
        pwm2 = pid_proc(tds_out);
        registerTick(PID_SETTING_TICK_NO, 0, 0,1);//定时器复位

    }
    if(PID_TYPE != 2)
    {
        registerTick(PID_SETTING_TICK_NO, 1000, 1,0);//超时计时开始

        if(GetTickResult==0)
            return ;

        {
            pid_cnt++;
            pid_cnt = 0;
            pwm2 = pid_proc(tds_out);
            tds_flag = 0;
            registerTick(PID_SETTING_TICK_NO, 0, 0,1);//定时器复位

        }


    }


    //pwm2 = 65535;
    if(pwm2>0)
    {
       ; //DcMotorCtrl(3,PUMP_VALUE);
       // DcMotorCtrl(1,PUMP_VALUE);
    }

    DcMotorCtrl(2,pwm2);//泵2调整流量
}
void fill_pro(unsigned char x,unsigned char y,unsigned char len,unsigned char maxlen)
{
    unsigned char strlen;
    if(strlen<maxlen)
        GUI_RectangleFill(x+len, y, maxlen-len,16	,0xf7be);

}


void module_dis_proc(unsigned char module_num)
{
    unsigned char *str;
    char ptr[20];
    unsigned char str_len;
    unsigned char last_len;

    if(module_num>FLOW_SIZE)
    {
        str = "无";

#if LCD_ORI == 1
        SetLableValue(Main_PAGE,module_num+54,(unsigned char *)str);


#else
        SetLableValueColor(Main_PAGE,module_num+54, BACK_COLOR,FORCE_COLOR, str);

#endif
    }

    else
    {
        if(GetSensor()->ele_MOnLine[module_num-1] == 1)
        {
            sprintf(ptr,"%4.2f",GetSensor()->ele_Mcurr[module_num-1]);
            str_len = strlen(ptr);
            if(last_len!=str_len)
            {
                last_len=str_len;
                // GUI_RectangleFill(x+2*str_len, y, 64,16  ,BACK_COLOR);
            }
#if LCD_ORI == 1
            SetLableValue(Main_PAGE,module_num+54,(unsigned char *)ptr);


#else
            SetLableValueColor(Main_PAGE,module_num+54, BACK_COLOR,FORCE_COLOR, ptr);

#endif


            //   Display_String(x, y,1,0x10,0,back_color,force_color, (unsigned char *)ptr);//c1


        }
        else
        {
            str = "离线";
#if LCD_ORI == 1
            SetLableValue(Main_PAGE,module_num+54,(unsigned char *)str);


#else
            SetLableValueColor(Main_PAGE,module_num+54, BACK_COLOR,FORCE_COLOR, str);

#endif


            // Display_String(x, y,1,0x10,0,back_color,force_color, (unsigned char *)str);//c1

        }

    }



}

void Mode_dis_proc()
{
    unsigned char *str,j;
//    char ptr[20];
//    static unsigned char str_len;
//    static unsigned char last_len;
    static unsigned char status_tmp,i;


    static unsigned char polling_count;
polling_status:

    polling_count++;
    if(i<WORK_STATUS_SIZE)
        status_tmp = GetSensor()->status[i++];
    else
        i = 0;
    if(GetSensor()->status[0] == 0)
    {
        if(status_tmp==0)
        {

            if(i<WORK_STATUS_SIZE)
            {
                //i=0;
                goto polling_status;
            }
            else
                ;


        }
    }
    else
        i=0;



    //while(str==NULL)
    str =malloc(26);
    //str2 = str;
    for(j=0; j<26; j++)
        str[j] = '\0';
    polling_count = 0;
    switch(status_tmp)// 0:正常  1：tds1 2:tds2 3：流量 4：orp
        //  5:高压开关6：水位开关 7:电解中
    {
        /*	str = "正常          ";
        	str = "原水TDS异常	 ";
        	str = "缺盐		 	 ";
        	str = "流量异常 	 ";
        	str = "盐水箱注水中";
        	str = "清洗中		 ";
        	str = "缺水			 ";
        	str = "系统故障 	 ";
        	str = "正常			 ";
        	str = "正常			 ";*/





    case 20:
        str = "正常";

        break;
    case TDS1_INDEX:
        str = "原水TDS异常";

        break;
    case TDS2_INDEX:
    case SHUNT_INDEX:
        if(GetSensor()->tds2<dstTds&&status_tmp==TDS2_INDEX)
            str = "缺盐";
		else
			

        break;
    case FLOW_INDEX:
        str = "流量异常";

        break;

    case WATER_LEVEL_INDEX:
        str = "盐水箱注水中";
        break;

    case WASH_INDEX:
        str = "清洗中";

        break;

    case NOWATER_INDEX:
        str = "缺水";
        break;
    case SYSTEM_INDEX:
        str = "系统故障";
        break;
    case FACTORY_INDEX:
        str = "正常";

        break;
    default:
        str = "正常";
        break;
    }
    //str = "进水TDS异常";
    if(status_tmp!=0)
#if LCD_ORI == 1
        SetLableValue(Main_PAGE,LABEL_WORK_ID,str);
#else
        //  if(status_tmp!=SYSTEM_INDEX&&status_tmp!=FLOW_INDEX&&status_tmp!=TDS2_INDEX&&status_tmp!=TDS1_INDEX&&status_tmp!=SHUNT_INDEX)
        //     SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
        //  else
        //      SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,WARN_BACK_COLOR, str);



        if(status_tmp==20||status_tmp==WASH_INDEX||status_tmp==WATER_LEVEL_INDEX||
                status_tmp==PH_INDEX||status_tmp==ORP_INDEX)
            SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
        else if(status_tmp==SYSTEM_INDEX||status_tmp==FLOW_INDEX||
                status_tmp==TDS2_INDEX||status_tmp==TDS1_INDEX||status_tmp==SHUNT_INDEX||
                status_tmp==NOWATER_INDEX)
            SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,WARN_BACK_COLOR, str);

#endif
    //
    //free(str2);

}
lcd_test()
{
    unsigned char *str,j;
    str =malloc(20);
    for(j=0; j<20; j++)
        str[j] = '\0';

    str = "正常            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "原水TDS异常     ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "缺盐            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "流量异常        ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "盐水箱注水中";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "清洗中          ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "缺水            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "系统故障        ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
    str = "正常            ";
    SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);


}
void lcd_proc()
{
//    static unsigned char i;
//    unsigned char *str;
    char ptr[20];
    lcd_boot();
    touch_pro();
    registerTick(LCD_TICK,500,1,0);
    if(GetTickResult(LCD_TICK)==1)
    {
        printf("消毒水总量及电解时间：	%0.2f(升)	   %0.1f(小时)\n",GetSensor()->water_quantity,GetSensor()->sum_ele_time);


        //GetSensor()->water_quantity = GetSensor()->water_quantity + 0.2;
        if(getTouch()->next_page == Main_PAGE)//主页面
        {
#if DEV_TYPE == 1

#if LCD_ORI == 1
            sprintf(ptr,"%4.1f",GetSensor()->ph);
            SetLableValue(Main_PAGE,LABEL_PH_ID,(unsigned char *)ptr);
            sprintf(ptr,"%4.0f",GetSensor()->orp);
            SetLableValue(Main_PAGE,LABEL_ORP_ID,(unsigned char *)ptr);//orp

#else
            //SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,FORCE_COLOR, (unsigned char *)ptr);


            sprintf(ptr,"%4.1f",GetSensor()->ph);
            SetLableValueColor(Main_PAGE,LABEL_PH_ID,BACK_COLOR,FORCE_COLOR, (unsigned char *)ptr);
            sprintf(ptr,"%4.0f",GetSensor()->orp);
            SetLableValueColor(Main_PAGE,LABEL_ORP_ID,BACK_COLOR,FORCE_COLOR, (unsigned char *)ptr);//orp

#endif


#endif

            module_dis_proc(1);
            module_dis_proc(2);
            module_dis_proc(3);
            module_dis_proc(4);

#if LCD_ORI == 1
            sprintf(ptr,"%4.2f",GetSensor()->flow);
            SetLableValue(Main_PAGE,LABEL_FLOW_ID,(unsigned char *)ptr);
            // module_dis_proc(2);//c2


            sprintf(ptr,"%4.0f",GetSensor()->tds1);
            SetLableValue(Main_PAGE,LABEL_OWTDS_ID,(unsigned char *)ptr);
            // module_dis_proc(3);//c3


            sprintf(ptr,"%4.0f",GetSensor()->tds2);
            SetLableValue(Main_PAGE,LABEL_ITDS_ID,(unsigned char *)ptr);
            // module_dis_proc(4);//c4


#else
            sprintf(ptr,"%4.2f",GetSensor()->flow);
            SetLableValueColor(Main_PAGE,LABEL_FLOW_ID,BACK_COLOR,FORCE_COLOR,(unsigned char *)ptr);
            // module_dis_proc(2);//c2


            sprintf(ptr,"%4.0f",GetSensor()->tds1);
            SetLableValueColor(Main_PAGE,LABEL_OWTDS_ID,BACK_COLOR,FORCE_COLOR,(unsigned char *)ptr);
            // module_dis_proc(3);//c3


            sprintf(ptr,"%4.0f",GetSensor()->tds2);
            SetLableValueColor(Main_PAGE,LABEL_ITDS_ID,BACK_COLOR,FORCE_COLOR,(unsigned char *)ptr);
            // module_dis_proc(4);//c4

#endif


            Mode_dis_proc();
            // lcd_test();




        }
#if DEV_TYPE == 1
        else if(getTouch()->next_page == CALIBRATION_PAGE)//校准页面
        {
            if(getTouch()->control_id == PH1_CAL_ID)
            {
                sprintf(ptr,"%4.2f",GetSensor()->orp_ph_adc);
                SetEditValue(CALIBRATION_PAGE,PH1_EDIT_ID,(unsigned char *)ptr);//ph1

            }
            if(getTouch()->control_id == PH2_CAL_ID)
            {
                sprintf(ptr,"%4.2f",GetSensor()->orp_ph_adc);
                SetEditValue(CALIBRATION_PAGE,PH2_EDIT_ID,(unsigned char *)ptr);//ph2

            }
            //if(getTouch()->control_id == ORP_CAL_ID&&getTouch()->key !=ENTER_ID)

            if(getTouch()->key ==ENTER_ID)
            {
                sprintf(( char*)ptr,"%4.2f",GetSensor()->orp_ph_adc);
                SetEditValue(CALIBRATION_PAGE,ORP_EDIT_ID,(unsigned char *)ptr);//ORP
            }
        }
        else  if(getTouch()->next_page == SETTING_PAGE)//设置页面
        {
            SetPage(SETTING_PAGE);//
        }
#endif
        else  if(getTouch()->next_page == REBOOT_PAGE)//重启页面
        {
            SetPage(REBOOT_PAGE);//主页面Id号是4
        }
#if DEV_TYPE == 1

        else
        {
            SetPage(Main_PAGE);//主页面Id号是4
            getTouch()->page_id = Main_PAGE;
            getTouch()->next_page == Main_PAGE;

        }
#endif

        registerTick(LCD_TICK,0,0,1);
    }


}

unsigned char repeat_flag=0;
static unsigned char addr_tmp = M1_ADDR;
unsigned char get_reset_status()
{
    return repeat_flag;
}
module_reset(unsigned char mode)
{
    static unsigned char buf[2];//,tmp;


    buf[0] = 0x00;
    buf[1] = 0x00;
    //  tmp = 0;

    registerTick(MODULE_RESET_TICK, 300, 1,0);//
    if(GetTickResult(MODULE_RESET_TICK)==1)
    {
        if(repeat_flag<3)//reapte 4 times
        {

            if(addr_tmp<=M4_ADDR) //ele module reset
            {
                if(GetSensor()->ele_MOnLine[addr_tmp-3])
                {
                    if(mode == 1)
                        GetModbusSens(addr_tmp,FUNC_WRITE,0x0044,0x0002,buf,2);//复位
                    else
                        GetModbusSens(addr_tmp,FUNC_WRITE,0x0040,0x0002,buf,2);	//停止电解

                }
                addr_tmp++;
                //delay_ms(100);
            }
            else
            {
                repeat_flag ++ ;
                addr_tmp = M1_ADDR;

            }

        }
        else
        {
            ;//module_reset_flag = 0;
        }
        registerTick(MODULE_RESET_TICK, 0, 0,1);//

    }



}
void water_levelAbnormal_proc()
{
    EleSwCtrl(WATER_SW,ON);//原水进水阀开
    EleSwCtrl(SALT_SW,ON);//盐盒进水阀开
#if WATER_L_IGNORE == 0
    EleSwCtrl(WASTE_SW,OFF);//废水出水阀开
    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
    EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关
    DcMotorCtrl(2,0);//关泵2
    DcMotorCtrl(3,0);
    DcMotorCtrl(1,0);
#endif
    GetSensor()->water_status = 1;
    // delay_ms(500);

}
void dev_init_ctrl()
{
    EleSwCtrl(WATER_SW,ON);//原水进水阀开
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
#endif
    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀开
    EleSwCtrl(HCILO_SW,ON);//消毒水出水阀关
    // delay_ms(500);

}
void dev_flow_Abnormal_ctrl()
{
    if(abnormalDec()&0x01)
    {
        GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//流量异常
        //GetSensor()->status[NOWATER_INDEX] = NOWATER_INDEX;//缺水

    }



    EleSwCtrl(WATER_SW,ON);//原水进水阀开
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
#endif
    EleSwCtrl(WASTE_SW,OFF);//废水出水阀关
    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
    EleSwCtrl(HCILO_SW,ON);//消毒水出水阀关
    //DcMotorCtrl(1,OFF);//关所有电机
    DcMotorCtrl(2,OFF);//关所有电机
}
void dev_normal_ctrl()
{
    GetSensor()->status[FLOW_INDEX] = 0;//
    GetSensor()->status[NORMAL_INDEX] = 20;//无异常
    EleSwCtrl(WATER_SW,ON);//原水进水阀开
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
#endif
    EleSwCtrl(WASTE_SW,ON);//废水出水阀开
    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
    EleSwCtrl(HCILO_SW,ON);//消毒水出水阀
    // FlowCtrl();
    // delay_ms(500);

}

void dev_wash_ctrl()
{
    GetSensor()->wash_time =3;//GetSensor()->wash_time +1 ;//大于3次停机
    GetSensor()->err_flag = 0;
    // GetSensor()->status[TDS2_INDEX] = 0;//tds2异常
    //GetSensor()->status[ORP_INDEX] = 0;//orp异常
    // GetSensor()->status[PH_INDEX] = 0;//ph异常
    EleSwCtrl(WATER_SW,ON);//原水进水阀开
    EleSwCtrl(WASTE_SW,OFF);//废水出水阀开
#if WATER_L_IGNORE == 0
    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
#endif


    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
    EleSwCtrl(HCILO_SW,ON);//消毒水出水阀
    GetSensor()->wash_time = 3;



    registerTick(WASH_TICK, 0,0,1);
    registerTick(TDS_TICK_NO, 0,0,1);//
    if((abnormalDec()&0x08)) //orp abnormal
        registerTick(ORP_TICK_NO, 0,0,1);//
    if((abnormalDec()&0x04))	//ph abnormal
        registerTick(PH_TICK_NO, 0,0,1);//

}

void power_off()
{
    EleSwCtrl(WATER_SW,OFF);//原水进水阀开
    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
    EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关
    EleSwCtrl(WASTE_SW,OFF);//废水出水阀关
    DcMotorCtrl(7,OFF);//关所有电机

}
void pump_ctrl(unsigned char mode)
{
    static unsigned char pump_flag=0,init_flag;
    if(mode == 0)
    {
        pump_flag = 0;
        init_flag = 0;

        registerTick(PUMP_TICK_NO, 0,0, 1);
    }
    if(mode == 1)
    {
        if(init_flag == 0)
        {
            init_flag = 1;
            pump_flag = 0;
            registerTick(PUMP_TICK_NO, 0,0, 1);

        }

    }

    registerTick(PUMP_TICK_NO, 30000,1, 0);
    if( GetTickResult(PUMP_TICK_NO)==1)
    {
        if(GetSensor()->flow == 0)//&&mode==1
        {
            if(pump_flag==0)
                pump_flag =1;
            //else
            //	pump_flag = 0;
            // DcMotorCtrl(3,0);
        }
        registerTick(PUMP_TICK_NO, 0,0, 1);
    }
    else
    {
        if(pump_flag==0)
            DcMotorCtrl(3,PUMP_VALUE);
        else
            DcMotorCtrl(3,0);

    }

}
//uint32_t time_out_sw=0;
//static unsigned char module_flag=0;
static uint32_t flow_low_cnt;
static unsigned char hsw_flag=0;
void hsw_proc()
{
    // EleSwCtrl(6,OFF);//关所有阀
    EleSwCtrl(WATER_SW,OFF);//原水进水阀开
    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
    EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关
    EleSwCtrl(WASTE_SW,OFF);//废水出水阀关
    DcMotorCtrl(7,OFF);//关所有电机

    GetSensor()->status[HSW_INDEX] = 5;//高压开关异常
    registerTick(FLOW_TICK_NO2, 20000,1,0);//

    if(GetTickResult(FLOW_TICK_NO2))//流量异常
    {
        if(GetSensor()->flow==0)
        {
            GetSensor()->status[FLOW_INDEX] = 0;//流量正常
            GetSensor()->status[NOWATER_INDEX] = 0;//流量正常
        }
        else
        {
            GetSensor()->status[NOWATER_INDEX] = 0;//流量正常
            GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//流量异常
        }
        registerTick(FLOW_TICK_NO2, 0,0,1);//

    }
    else
    {
        GetSensor()->status[FLOW_INDEX] = 0;//流量正常
        GetSensor()->status[NOWATER_INDEX] = 0;//流量正常

    }

    if(GetSensor()->status[FLOW_INDEX])
        GetSensor()->status[HSW_INDEX] =    5;//高压开关异常
    else
        GetSensor()->status[HSW_INDEX] =    0;//

    if(repeat_flag <3&&GetInOut()->key_cali_mode == 0)
    {
        module_reset(0);
    }

    work_params.init_flag = 1;
    GetSensor()->err_flag=0;
    hsw_flag = 1;
    registerTick(TDS_TICK_NO, 0,0,1);//
    registerTick(FLOW_TICK_NO, 0, 0,1);//超时计时开始

}
void normal_proc()
{
    //float flow_tmp;

    if( hsw_flag == 1)
    {
        flow_low_cnt = 0;
        hsw_flag = 0;
        registerTick(TDS_TICK_NO, 0,0,1);//
        registerTick(FLOW_TICK_NO, 0, 0,1);//超时计时开始
    }

    registerTick(FLOW_TICK_NO2, 0,0,1);
    if(abnormalDec()&0x10==0)//水位异常
        GetSensor()->status[HSW_INDEX] = 0;//高压开关异常

    if(GetSensor()->status[SHUNT_INDEX] == 0)//非停机模式
    {



        if(abnormalDec()&0x20)//水位异常
        {


            pump_ctrl(0);
            flow_low_cnt = 0;
            GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//水位异常
            GetSensor()->status[FLOW_INDEX] = 0;//流量异常
            GetSensor()->status[NOWATER_INDEX] = 0;//缺水
            GetSensor()->status[TDS2_INDEX] = 0;//缺水
            GetSensor()->status[TDS1_INDEX] = 0;//缺水

            if(GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M)
            {
                water_levelAbnormal_proc();

            }
            else //水位开关异常
            {
                EleSwCtrl(SALT_SW,OFF);//关阀2
            }

        }//end 水位异常
#if WATER_L_IGNORE == 1
        else//水位正常，其他参数检测才有意义
        {
            EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
            GetSensor()->water_status = 0;//水位异常重新加水加满状态标志位
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//水位无异常

        }
#endif


#if WATER_L_IGNORE == 0
        else//水位正常，其他参数检测才有意义
#endif
        {
            
            if(GetSensor()->flow > 0)
            {
                pump_ctrl(0);
            }
			else
			{
				pump_ctrl(1);

			}


#if WATER_L_IGNORE == 0

            EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
            GetSensor()->water_status = 0;//水位异常重新加水加满状态标志位
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//水位无异常
#endif


            if(work_params.init_flag == 0)//上电开机
            {
                dev_init_ctrl();
                if(GetSensor()->flow > 0)
                {
                    EleSwCtrl(WASTE_SW,ON);//废水出水阀开
                }
                else
                    EleSwCtrl(WASTE_SW,OFF);//废水出水阀开
                if(abnormalDec()&0x01||GetSensor()->flow == 0)//流量异常
                {
                    //GetSensor()->flow = 0;
                    //registerTick(FLOW_TICK_NO, 0, 0,1);//定时器复位
                    if(GetSensor()->flow == 0)//&&hsw_flag == 1
                    {
                        dev_flow_Abnormal_ctrl();
						pid_init2(dstTds);
                    }
                }
                else
                {
                    if(GetSensor()->flow != 0)//流量异常
                        FlowCtrl();
                    else
                    {
						DcMotorCtrl(2,0);//泵2调整流量
						pid_init2(dstTds);

					}


                }

            }
            else  if(abnormalDec()&0x01||GetSensor()->flow == 0)//流量异常
            {

                if(GetSensor()->flow == 0)//&&hsw_flag == 1
                {
                    dev_flow_Abnormal_ctrl();
									   pid_init2(dstTds);
                }
                else
                {
                    EleSwCtrl(WASTE_SW,ON);//废水出水阀开
                }

            }
            else  //流量正常
            {
                if(GetSensor()->flow != 0)//流量异常
                    FlowCtrl();
                else
                {
                    // Flow_Init() ;
                    pid_init2(dstTds);
                    DcMotorCtrl(2,0);//泵2调整流量
                    EleSwCtrl(WASTE_SW,OFF);//废水出水阀开

                }


                //GetSensor()->status[NOWATER_INDEX] = 0;//流量异常
                // GetSensor()->status[NOWATER_INDEX] = 0;//缺水
                if((abnormalDec()&0x73)==0)//无异常，走正常逻辑 0x73 0x33
                {

                    dev_normal_ctrl();

                }
                else //参数有异常进行调节控制
                {
                    if(abnormalDec()&0x40)
                    {
                        GetSensor()->status[TDS1_INDEX] = TDS1_INDEX;//tds1异常
                    }
                    else
                    {
                        GetSensor()->status[TDS1_INDEX] = 0;//tds1异常
                        if(GetSensor()->err_flag&0x10)//tds报警
                        {
                            dev_wash_ctrl();

                        }//tds报警,自动清洗
                        else
                        {
                            GetSensor()->status[TDS2_INDEX] = 0;//tds2异常
                            EleSwCtrl(WATER_SW,ON);//原水进水阀开
                            EleSwCtrl(WASTE_SW,ON);//废水出水阀开
                            EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
                            EleSwCtrl(HCILO_SW,ON);//消毒水出水阀关
                        }

                    }//tds2异常


                }//参数有异常进行调节控制


            } //流量正常
            //if(((GetSensor()->flow-FLOW_SIZE)>=FLOW_SIZE*0.1)||((GetSensor()->flow-FLOW_SIZE)<=-FLOW_SIZE*0.1))    //水流量不足


            if(GetSensor()->flow<FLOW_VALUE)	//水流量不足
            {
                flow_low_cnt++;
                if(flow_low_cnt>=500000)
                {
                    flow_low_cnt = 0;
                    if(GetSensor()->flow==0)
                        GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//流量异常
                    else
                        GetSensor()->status[FLOW_INDEX] = 0;//流量异常
                    GetSensor()->status[NOWATER_INDEX] = NOWATER_INDEX;//缺水
                }
            }
            else
            {
                GetSensor()->status[NOWATER_INDEX] = 0;//缺水
                flow_low_cnt =0;

            }

        }// end 水位正常，其他参数检测才有意义

    }//非停机模式
    else //停机模式
    {
        if(abnormalDec()&0x20)//水位异常
        {


            pump_ctrl(0);
            flow_low_cnt = 0;
            GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//水位异常
            GetSensor()->status[FLOW_INDEX] = 0;//流量异常
            GetSensor()->status[NOWATER_INDEX] = 0;//缺水
            GetSensor()->status[TDS2_INDEX] = 0;//缺水
            GetSensor()->status[TDS1_INDEX] = 0;//缺水

            if(GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M)
            {
                water_levelAbnormal_proc();

            }
            else //水位开关异常
            {
                EleSwCtrl(SALT_SW,OFF);//关阀2
            }

        }//end 水位异常
#if WATER_L_IGNORE == 1
        else//水位正常，其他参数检测才有意义
        {
            EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
            GetSensor()->water_status = 0;//水位异常重新加水加满状态标志位
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//水位无异常

        }
#endif



        //if(abnormalDec()&0x02==0)//tds无异常
        {
            GetSensor()->status[TDS1_INDEX] = 0;//tds2无异常
            GetSensor()->status[TDS2_INDEX] = 0;//tds2无异常

            GetSensor()->status[ORP_INDEX] = 0;//tds2无异常
            GetSensor()->status[PH_INDEX] = 0;//tds2无异常
            GetSensor()->wash_time =0;
            //GetSensor()->status[SHUNT_INDEX] = 0;
        }
    }


}
/*************************************************************************
启动逻辑：高压阀正常--->水位正常--->电解流程
          高压阀异常--->停机
          水位异常-->停机
***************************************************************************/
void ele_dev_proc()
{

    static unsigned char retry_flag=0;


    if(GetSensor()->wash_time>=3&&shunt_flag ==0||GetSensor()->status[TDS1_INDEX]==TDS1_INDEX||GetSensor()->status[SYSTEM_INDEX]==SYSTEM_INDEX)//
    {
        shunt_flag = 1;
        if(retry_flag==0)
        {
            while(get_reset_status() <3)
            {
                module_reset(1);
            }
            retry_flag = 1;
            repeat_flag = 0;
        }

        //module_reset(1);
        if(GetSensor()->status[SYSTEM_INDEX] == SYSTEM_INDEX)
        {
            GetSensor()->status[TDS1_INDEX] = 0;
            GetSensor()->status[TDS2_INDEX] = 0;
            GetSensor()->status[FLOW_INDEX] = 0;
            GetSensor()->status[ORP_INDEX] = 0;
            GetSensor()->status[PH_INDEX] = 0;
            GetSensor()->status[NOWATER_INDEX] = 0;
        }
        else
        {
            if(GetSensor()->wash_time>=3)
            {
                GetSensor()->status[SHUNT_INDEX] = 0;//停机
                GetSensor()->status[SHUNT_INDEX] = SHUNT_INDEX;//停机
                if(GetSensor()->tds2>=MAX_TDS_VALUE)
                    GetSensor()->status[TDS2_INDEX] = 0;//tds2异常
                else if(GetSensor()->tds2 <=dstTds-150||GetSensor()->tds2 <=MIN_TDS_VALUE)

                    GetSensor()->status[TDS2_INDEX] = TDS2_INDEX;//tds2异常

				else
					GetSensor()->status[SHUNT_INDEX] = 0;//停机
					
            }

        }

        EleSwCtrl(6,OFF);//关所有阀
        DcMotorCtrl(7,OFF);//关所有电机

        //delay_ms(500);
    }
    else
    {
        retry_flag = 0;
        if(abnormalDec()&0x10||GetSensor()->temperature<=5)//高压阀异常,低温不工作
        {

            hsw_proc();
        }
        else //正常工作模式
        {
            //state_flag=1;
            Flow_Init();
            normal_proc();

        }

    }



}
static unsigned char state_flag=0;
/*************************************************************************

**************************************************************************/
void ele_process()
{
    unsigned char i=0;
    static uint32_t params_tick;



    if(GetInOut()->key_reset_mode )//复位重启
    {
        EleSwCtrl(6,OFF);//关所有阀
        DcMotorCtrl(7,OFF);//关所有电机
        // delay_ms(500);
        while(get_reset_status()!=3)
            module_reset(1);//主机和模块均复位
        Soft_Reset();
    }

    if(GetInOut()->key_cali_mode != 0||getTouch()->next_page!=Main_PAGE)
    {
        GetInOut()->key_wash_mode = 0;
        // registerTick(WASH_TICK, 0,0,1);
        //GetSensor()->status[WASH_INDEX] = 0;
        EleSwCtrl(6,OFF);//关所有阀
        DcMotorCtrl(7,OFF);//关所有电机
        registerTick(FLOW_TICK_NO, 0, 0,1);//定时器复位
        //delay_ms(500);

    }
    else if(GetInOut()->key_wash_mode)//冲洗状态
    {
        pump_ctrl(0);
        if(abnormalDec()&0x20)//水位异常
        {

            GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//水位异常
            GetSensor()->status[FLOW_INDEX] = 0;//流量异常
            GetSensor()->status[NOWATER_INDEX] = 0;//缺水
            GetSensor()->status[TDS2_INDEX] = 0;//
            GetSensor()->status[TDS1_INDEX] = 0;//

            if(GetSensor()->water_level == WATER_L||GetSensor()->water_level == WATER_M)
            {
                water_levelAbnormal_proc();

            }
            else //水位开关异常
            {
                EleSwCtrl(SALT_SW,OFF);//关阀2
            }

        }//end 水位异常
#if WATER_L_IGNORE == 1
        else//水位正常，其他参数检测才有意义
        {
            EleSwCtrl(SALT_SW,OFF);//盐盒进水阀关
            GetSensor()->water_status = 0;//水位异常重新加水加满状态标志位
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//水位无异常
        }
#endif

#if WATER_L_IGNORE == 0
        else
#endif
        {

            EleSwCtrl(WATER_SW,ON);//开阀1
#if WATER_L_IGNORE == 0
            EleSwCtrl(SALT_SW,OFF);//开阀2
            GetSensor()->water_status = 0;//水位异常重新加水加满状态标志位
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//水位无异常

#endif
            EleSwCtrl(WASTE_SW,ON);//开阀3
            EleSwCtrl(WASH_SW,ON);//开阀4
            EleSwCtrl(HCILO_SW,OFF);//开阀5
            GetSensor()->status[TDS1_INDEX] = 0;//tds1异常
            GetSensor()->status[TDS2_INDEX] = 0;//
            if(GetSensor()->flow != 0)//流量异常
            {
                Flow_Init();
                FlowCtrl();

            }
            else
            {
               pid_init2(dstTds);
                DcMotorCtrl(2,0);//泵2调整流量
            }

            registerTick(WASH_TICK, WASH_TIME_SETTING,1, 0);
            if( GetTickResult(WASH_TICK)==1)
            {

                GetInOut()->key_wash_mode = 0;
                registerTick(WASH_TICK, 0,0,1);
                GetSensor()->status[WASH_INDEX] = 0;//washing
                EleSwCtrl(WASTE_SW,OFF);//开阀3
                //DcMotorCtrl(2,0);//关泵2


            }

        }

    }

    else
        ele_dev_proc();

    if((GetSensor()->status[NORMAL_INDEX]==20&&GetSensor()->flow>0&&  //加速启动电解速度
            GetInOut()->key_cali_mode==0)||GetInOut()->key_wash_mode
      )//高压开关和水位正常且不在校准模式时启动电解
    {
        if(state_flag == 0)
        {
            state_flag = 1;
            setState(4, M1_ADDR);
            setTickResult(SENSOR_TICK_NO,1);

        }
    }
    else
    {
        state_flag = 0;
    }
    for(i=1; i<WORK_STATUS_SIZE; i++)    //故障状态处理
    {
        //GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;

        if(GetSensor()->status[i]!=0)
        {
#if WATER_L_IGNORE == 0
            if(i!=ORP_INDEX&&i!=PH_INDEX)
#else
            if(i!=ORP_INDEX&&i!=PH_INDEX)
#endif

            {
                GetSensor()->status[NORMAL_INDEX] = 0;
                if(i==NORMAL_INDEX||i==WASH_INDEX)
                {
                    shunt_flag=0;
                    //GetSensor()->status[NORMAL_INDEX] = 20;
                    GetSensor()->status[SHUNT_INDEX] = 0;
                    GetSensor()->wash_time =0;
                }
                // if(i!=WATER_LEVEL_INDEX&&i!=WASH_INDEX&&GetInOut()->key_cali_mode == 0)
                //   beep_pro(6,0);
                break;
            }

        }
        else
        {
            if(i>=(WORK_STATUS_SIZE-1))
            {
                GetSensor()->status[NORMAL_INDEX] = 20;

                //beep_pro(3,1);

            }


        }

    }
    /*****************************************风扇控制***********************************************/
    if(GetSensor()->flow>0&&(GetSensor()->status[WASH_INDEX]||GetSensor()->status[NORMAL_INDEX] == 20))
    {

        DcMotorCtrl(1, 65535);
        if((HAL_GetTick()-params_tick)>=120000)
        {
					SystemParamsSave();
					//GetSensor()->water_quantity=GetSensor()->water_quantity+0.15;
					//GetSensor()->sum_ele_time= GetSensor()->sum_ele_time+0.1;
					params_tick =  HAL_GetTick();
        }
    }
    else
    {
        params_tick =  HAL_GetTick();
        DcMotorCtrl(1, 0);

    }


}
/*void work_process()
{
    if(work_params.work_mode == ON)
    {

        //delay_ms(100);
        //setAddr_ORP_PH(0x02);
        //	recovery_ORP_PH(ORP_ADDR);
        //recovery_ORP_PH(PH_ADDR);
        //DcMotorCtrl(1,65535);

        sensor_process();//
        ele_process();
        lcd_proc();

    }
}*/
void app_test()
{
    EleSwCtrl(WATER_SW,ON);//原水进水阀开
    EleSwCtrl(WASTE_SW,ON);//废水出水阀开
    EleSwCtrl(HCILO_SW,ON);//消毒水出水阀关
    EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关
    // EleSwCtrl(WASH_SW,ON);//消毒水排出到废水阀关
    //EleSwCtrl(SALT_SW,ON);//盐盒进水阀开
    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀开
    //EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关


    // EleSwCtrl(WASTE_SW,OFF);//废水出水阀开

    EleSwCtrl(WATER_SW,ON);//原水进水阀开
    // EleSwCtrl(SALT_SW,ON);//盐盒进水阀开
    //  EleSwCtrl(WASTE_SW,ON);//废水出水阀开

    //  EleSwCtrl(WASH_SW,ON);//消毒水排出到废水阀关
    // EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
    EleSwCtrl(HCILO_SW,ON);//消毒水出水阀关
    //  EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关
    //  EleSwCtrl(WATER_SW,OFF);//原水进水阀开
//
//
//
//    EleSwCtrl(SALT_SW,OFF);//盐盒进水阀开
//    EleSwCtrl(WASTE_SW,OFF);//废水出水阀开
//    EleSwCtrl(WASH_SW,OFF);//消毒水排出到废水阀关
//    EleSwCtrl(HCILO_SW,OFF);//消毒水出水阀关
//  EleSwCtrl(WATER_SW,OFF);//原水进水阀开
//    DcMotorCtrl(2,0);//关泵2


}


uint32_t tmp3;
void SystemParamsSave()
{
    uint32_t addr;
   uint32_t tmp; 

    FlashEease(0);
    addr = 0;
    tmp = 0x5a; //配网标志，代表已经配过网络
    bsp_WriteCpuFlash(addr, &tmp, 1);
    addr = addr +4;
    //GetSensor()->water_quantity = 0;
    bsp_WriteCpuFlash(addr,(uint32_t *)(&GetSensor()->water_quantity), 1);
    addr = addr +4;
    //GetSensor()->sum_ele_time = 0.1111111;
    tmp3 = GetSensor()->sum_ele_time*10;
    GetSensor()->sum_ele_time = tmp3/10.0;
    bsp_WriteCpuFlash(addr,(uint32_t *)(&GetSensor()->sum_ele_time), 1);

}
void SystemParamsRead()
{
    uint32_t addr;
    uint32_t tmp;


    addr = 0;
    bsp_ReadCpuFlash(addr, &tmp, 1);
    if(tmp == 0x5a||tmp == 0x5b)
    {
        addr = addr +4;
        bsp_ReadCpuFlash(addr,(uint32_t *)(&GetSensor()->last_water_quantity), 1);
        addr = addr +4;
        bsp_ReadCpuFlash(addr,(uint32_t *)(&GetSensor()->last_sum_ele_time), 1);


    }
    else
    {
        SystemParamsSave();
    }


}
unsigned char factory_mode()
{
    unsigned char result ;
    static unsigned char flag;
    result = 1;
    if(GetTouchInOut()->factory_count>=8)
    {
        beep_pro(6,0);
        if(flag == 0)
        {
            flag = 1;
            while(get_reset_status()!=3)
                module_reset(1);//主机和模块均复位
        }

        //Soft_Reset();

        GetSensor()->status[FACTORY_INDEX] = FACTORY_INDEX;
        EleSwCtrl(6,OFF);//所有阀
        DcMotorCtrl(7,OFF);//所有电机
        //GetTouchInOut()->factory_count = 0;
        result = 0;
    }
    return result;

}
void app()
{
    static unsigned char flag;
    if(flag == 0)
    {
        GetSensor()->temperature = 6;
        flag = 1;
    }


    sensor_process();//
    if(factory_mode() == 1)
        ele_process();
    lcd_proc();



}
