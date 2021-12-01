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


void SystemParamsRead(void);


//Ŀ��tds 2.5----500
//Ŀ��tds 3 ------650
//Ŀ��tds 3.5 ------950   +30----0.1
//Ŀ��tds 4 ------1100


uint32_t dstTds=1100;//Ŀ��tds 2.5----500
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
function:�����쳣��ʱ�ж�
params:
**********************************************************************/
unsigned char TickTimeoutAb(unsigned char TickNum,unsigned char BitOper,uint32_t timeout)
{
    if(GetTickStatus(TickNum)!=1&& GetTickNum(TickNum)==1)//������תΪ�쳣
    {
        registerTick(TickNum, 0, 0,1);//��ʱ����λ
        registerTick(TickNum, timeout, 1,0);//��ʱ��ʱ��ʼ
    }
    else
    {
        if(GetTickNum(TickNum)==0||GetTickStatus(TickNum)==2)
        {
            registerTick(TickNum, 0, 0,1);//��ʱ����λ
            registerTick(TickNum, timeout, 1,0);//��ʱ��ʱ��ʼ
        }

    }
    if( GetTickResult(TickNum)==1&&GetTickStatus(TickNum)==1)
    {
        // status = status | BitOper;
        registerTick(TickNum, 0, 0,1);//��ʱ����λ
        return BitOper;
    }
    else
    {

        return 0;
    }

}

unsigned char TickTimeoutNor(unsigned char TickNum,unsigned char BitOper,uint32_t timeout)
{
    if(GetTickStatus(TickNum)==1&&GetTickResult(TickNum)==1)//����
    {
        registerTick(TickNum, 0, 0,1);//��ʱ����λ
        registerTick(TickNum, timeout, 2,0);//��ʱ��ʱ��ʼ
        if(GetTickStatus(TickNum)==0)
            return BitOper;
    }
    else
    {
        if(GetTickNum(TickNum)==0||GetTickStatus(TickNum)==1)
        {
            registerTick(TickNum, 0, 0,1);//��ʱ����λ
            registerTick(TickNum, timeout, 2,0);//��ʱ��ʱ��ʼ
        }

    }
    if( GetTickResult(TickNum)==1&&GetTickStatus(TickNum)==2)
    {
        registerTick(TickNum, 0, 0,1);//��ʱ����λ
        return BitOper;
    }

    return 0xff;

}

unsigned char abnormalDec()
{
    static unsigned char status;

    if(work_params.init_flag == 0)//�ϵ��ʼ�������ˮ�ӷ�ˮͨ���ų�
        registerTick(StART_TICK_NO, INIT_WASH_TIME_SETTING,1, 0);
    if( GetTickResult(StART_TICK_NO)==1)
    {
        work_params.init_flag = 1;//ʱ�䵽���������߼�����
        registerTick(StART_TICK_NO, 0,0,1);
    }


    //********************************************************************************

    if(GetSensor()->flow >= MAX_FLOW|| GetSensor()->flow <= MIN_FLOW)//�����쳣
    {
        if(GetSensor()->status[HSW_INDEX]==0&&GetInOut()->key_cali_mode == 0)//ˮ��ͷ��������������쳣
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
    if(GetSensor()->tds1>= MAX_TDS1_VALUE|| GetSensor()->tds1 <= MIN_TDS1_VALUE)//ԭˮtds�쳣
    {

        status = status | TickTimeoutAb(TDS1_TICK_NO,0x40,1000);

    }
    else
    {
        status = status & TickTimeoutNor(TDS1_TICK_NO,0xbf,MAX_SHORT_TICK);
    }

    if(GetSensor()->tds2 >= MAX_TDS_VALUE|| GetSensor()->tds2 <= MIN_TDS_VALUE)//tds�쳣
    {

        status = status | TickTimeoutAb(TDS_TICK_NO,0x02,MAX_TICK);
        if(status &0x02)
        {
            GetSensor()->err_flag =GetSensor()->err_flag |0x01;//��ʱ��ʱ

        }
        if(GetSensor()->err_flag &0x01==1)//3�����ȶ��϶�Ϊ�������ϸ�
        {
            if((HAL_GetTick()-GetSensor()->delay_timeout )>=WASH_TIME)
            {
                GetSensor()->err_flag = GetSensor()->err_flag & 0xfe;
                GetSensor()->err_flag = GetSensor()->err_flag | 0x10;//��ʱ
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

    if(GetSensor()->ph>= MAX_PH||GetSensor()->ph <= MIN_PH)//ph�쳣
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

    if(GetSensor()->orp >= MAX_ORP||GetSensor()->orp <= MIN_ORP)//orp�쳣
    {
        status = status | TickTimeoutAb(ORP_TICK_NO,0x08,MAX_TICK);
    }
    else
    {
        status = status & TickTimeoutNor(ORP_TICK_NO,0xf7,MAX_SHORT_TICK);
    }
    //********************************************************************************

    if(GetSensor()->water_level == WATER_F	||GetSensor()->water_level == WATER_L)//ˮλ�쳣
    {
        status = status | 0x20;//TickTimeoutAb(WATER_TICK_NO,0x20,WATER_LSHORT_TICK);
        GetSensor()->salt_water_timeout++;

    }
    else
    {
        GetSensor()->salt_water_timeout=0;
        status = status & 0xdf;
    }
    //********************************************************************************

    if(GetSensor()->swH ==ON)//��ѹ�����쳣
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
���ܣ�����������Χ������ˮtds

************************************************************************/

void Flow_Init()
{
    // dstTds = 1100;
    static unsigned int flow_switch;// 0-1  1-2  2-3  3-4
    if(GetSensor()->flow<=0)
    {
        registerTick(TDS_SETTING_TICK_NO, 0,0,1);
        switch(FLOW_SIZE)
        {
        case 1:
            flow_switch = TDS_LEVEL1 ;
            break;
        case 2:
            flow_switch = TDS_LEVEL2 ;
            break;
        case 3:
            flow_switch = TDS_LEVEL3 ;
            break;
        case 4:
            flow_switch = TDS_LEVEL4 ;
            break;
        default:
            flow_switch = TDS_LEVEL4 ;
            break;
        }
    }
    else
    {
        registerTick(TDS_SETTING_TICK_NO, 5000,1,0);
        //Ŀ��tds 2.5----500
        //Ŀ��tds 3 ------650
        //Ŀ��tds 3.5 ------950	+30----0.1
        //Ŀ��tds 4 ------1100

        if(GetTickResult(TDS_SETTING_TICK_NO)==1)
        {
            if(GetSensor()->flow<=2.5)
                flow_switch = TDS_LEVEL1 ;
            else if(GetSensor()->flow<=3)//2.5---3    950	+30----0.1
                flow_switch = TDS_LEVEL2 ;
            else if(GetSensor()->flow<=3.5)//3---3.5  950	+30----0.1
                flow_switch = TDS_LEVEL3 ;
            else if(GetSensor()->flow<=4)//3.5---4    950	+30----0.1
            {
                flow_switch = (GetSensor()->flow-3.5)*300+TDS_LEVEL3;

            }

            else
                flow_switch = TDS_LEVEL5 ;
            registerTick(TDS_SETTING_TICK_NO, 0,0,1);

        }

    }
    //dstTds = flow_switch;
    pid_init_flow(dstTds);

}
static int32_t pwm2;
extern unsigned char tds_flag;
void FlowCtrl()
{
    double tds_out;
	static uint32_t pid_cnt;
    //dstTds = 1100;
    tds_out    = GetSensor()->tds2;

	//if(pid_cnt>=100)
		//{
//	if( tds_flag==1)
	{
    	pwm2 = pid_proc(tds_out);
		tds_flag = 0;

	}

	//	pid_cnt = 0;

	//	}	

	//else
	//	pid_cnt++;
    //pwm2 = 65535;
	  if(pwm2>0)
		{
	  DcMotorCtrl(3,65535);	
	  DcMotorCtrl(1,65535);			
		}

    DcMotorCtrl(2,pwm2);//��2��������
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
        str = "��";

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
            str = "����";
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
    unsigned char *str,*str2,j;
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
    str =malloc(20);
    //str2 = str;
    for(j=0; j<20; j++)
        str[j] = '\0';
    polling_count = 0;
    switch(status_tmp)// 0:����  1��tds1 2:tds2 3������ 4��orp
        //  5:��ѹ����6��ˮλ���� 7:�����
    {

    case 20:
        str = "����";;
        // str[8]='\0';
        break;
    case TDS1_INDEX:
        str = "ԭˮTDS�쳣";
        // str[11]='\0';
        break;
    case TDS2_INDEX:
    case SHUNT_INDEX:
		if(GetSensor()->tds2<dstTds)
        str ="ȱ��";
		else
		{
		if(GetSensor()->flow == 0)
		str ="ȱˮ";	 
		else
		str ="ȱˮ";	 			

		}
		
 		
        //str[11]='\0';
        break;
    case FLOW_INDEX:
		if(DEV_TYPE==0)
        str ="ȱˮ";
		else
		str ="�����쳣";
        //str[8]='\0';
        break;
        /* case 4:
             str = "ORP�쳣";
             str[7]='\0';
             break;*/
    case HSW_INDEX:
        str = "ˮѹ�쳣";
        //str[8]='\0';
        break;
    case WATER_LEVEL_INDEX:
        str = "��ˮ��עˮ��";
        //str[8]='\0';
        break;
        //  case ELE_INDEX:
        //  str = "��⹤����";
        // str[10]='\0';
        //   break;
    case WASH_INDEX:
        str = "��ϴ��";
        // str[6]='\0';
        break;
        //case 10:
        //  str = "������";
        //str[6]='\0';
        // break;
    case NOWATER_INDEX:
        str = "ȱˮ";
        //str[6]='\0';
        break;
    case SYSTEM_INDEX:
        str = "ϵͳ����";
        //str[6]='\0';
        break;

    case 255:
        str = "�����쳣";
        // str[8]='\0';
        break;
        // default:
        //    str = "����";;
        // str[8]='\0';
        //    break;
    }
    //str = "��ˮTDS�쳣";
    if(status_tmp!=0)
#if LCD_ORI == 1
        SetLableValue(Main_PAGE,LABEL_WORK_ID,str);
#else
        if(status_tmp==20||status_tmp==WASH_INDEX||status_tmp==WATER_LEVEL_INDEX)
            SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,STATLUS_FORCE_COLOR, str);
        else
            SetLableValueColor(Main_PAGE,LABEL_WORK_ID, BACK_COLOR,WARN_BACK_COLOR, str);
#endif
    //
    //free(str2);

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


        if(getTouch()->next_page == Main_PAGE)//��ҳ��
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
            /*  switch(FLOW_SIZE)
              {
              case 1:
                  module_dis_proc(1);
                  break;
              case 2:
                  module_dis_proc(1);
                  module_dis_proc(2);
                  break;
              case 3:
                  module_dis_proc(1);
                  module_dis_proc(2);
                  module_dis_proc(3);
                  break;*/
            //case 4:
            module_dis_proc(1);
            module_dis_proc(2);
            module_dis_proc(3);
            module_dis_proc(4);
            // break;


            //}



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




        }
#if DEV_TYPE == 1
        else if(getTouch()->next_page == CALIBRATION_PAGE)//У׼ҳ��
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
        else  if(getTouch()->next_page == SETTING_PAGE)//����ҳ��
        {
            SetPage(SETTING_PAGE);//
        }
#endif
        else  if(getTouch()->next_page == REBOOT_PAGE)//����ҳ��
        {
            SetPage(REBOOT_PAGE);//��ҳ��Id����4
        }
#if DEV_TYPE == 1

        else
        {
            SetPage(Main_PAGE);//��ҳ��Id����4
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
    static unsigned char buf[2],tmp;


    buf[0] = 0x00;
    buf[1] = 0x00;
    tmp = 0;

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
                        GetModbusSens(addr_tmp,FUNC_WRITE,0x0044,0x0002,buf,2);//��λ
                    else
                        GetModbusSens(addr_tmp,FUNC_WRITE,0x0040,0x0002,buf,2);	//ֹͣ���

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
    EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
    EleSwCtrl(SALT_SW,ON);//�κн�ˮ����
    EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����
    DcMotorCtrl(2,0);//�ر�2
	DcMotorCtrl(3,0);	
	DcMotorCtrl(1,0);			
    GetSensor()->water_status = 1;
    // delay_ms(500);

}
void dev_init_ctrl()
{
    EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
    EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ����
    // delay_ms(500);

}
void dev_flow_Abnormal_ctrl()
{
    if(abnormalDec()&0x01)
    {
        GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//�����쳣
        //GetSensor()->status[NOWATER_INDEX] = NOWATER_INDEX;//ȱˮ

    }



    EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
    EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
    EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ����
    DcMotorCtrl(7,OFF);//�����е��
}
void dev_normal_ctrl()
{
    GetSensor()->status[FLOW_INDEX] = 0;//
    GetSensor()->status[NORMAL_INDEX] = 20;//���쳣
    EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
    EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
    EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����
    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ��
    // FlowCtrl();
    // delay_ms(500);

}

void dev_wash_ctrl()
{
    GetSensor()->wash_time =3;//GetSensor()->wash_time +1 ;//����3��ͣ��
    GetSensor()->err_flag = 0;
   // GetSensor()->status[TDS2_INDEX] = 0;//tds2�쳣
    //GetSensor()->status[ORP_INDEX] = 0;//orp�쳣
   // GetSensor()->status[PH_INDEX] = 0;//ph�쳣
							     EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
								 EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
								 EleSwCtrl(SALT_SW,OFF);//��ˮ��ˮ����
								 EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
								 EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ��
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
    EleSwCtrl(WATER_SW,OFF);//ԭˮ��ˮ����
    EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����
    EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
    DcMotorCtrl(7,OFF);//�����е��

}

//uint32_t time_out_sw=0;
//static unsigned char module_flag=0;
static uint32_t flow_low_cnt;
static unsigned char hsw_flag=0;
void hsw_proc()
{
    // EleSwCtrl(6,OFF);//�����з�
    EleSwCtrl(WATER_SW,OFF);//ԭˮ��ˮ����
    EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����
    EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
    DcMotorCtrl(7,OFF);//�����е��

    GetSensor()->status[HSW_INDEX] = 5;//��ѹ�����쳣
    registerTick(FLOW_TICK_NO2, 20000,1,0);//

    if(GetTickResult(FLOW_TICK_NO2))//�����쳣
    {
        if(GetSensor()->flow==0)
        {
            GetSensor()->status[FLOW_INDEX] = 0;//��������
            GetSensor()->status[NOWATER_INDEX] = 0;//��������
        }
        else
        {
            GetSensor()->status[NOWATER_INDEX] = 0;//��������
            GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//�����쳣
        }
        registerTick(FLOW_TICK_NO2, 0,0,1);//

    }
    else
    {
        GetSensor()->status[FLOW_INDEX] = 0;//��������
        GetSensor()->status[NOWATER_INDEX] = 0;//��������

    }

    if(GetSensor()->status[FLOW_INDEX])
        GetSensor()->status[HSW_INDEX] =    5;//��ѹ�����쳣
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
    registerTick(FLOW_TICK_NO, 0, 0,1);//��ʱ��ʱ��ʼ

}
void normal_proc()
{
    float flow_tmp;

    if( hsw_flag == 1)
    {
        flow_low_cnt = 0;
        hsw_flag = 0;
        registerTick(TDS_TICK_NO, 0,0,1);//
        registerTick(FLOW_TICK_NO, 0, 0,1);//��ʱ��ʱ��ʼ
    }
    registerTick(FLOW_TICK_NO2, 0,0,1);
    if(abnormalDec()&0x10==0)//ˮλ�쳣
        GetSensor()->status[HSW_INDEX] = 0;//��ѹ�����쳣

    if(GetSensor()->status[SHUNT_INDEX] == 0)//��ͣ��ģʽ
    {
        if(abnormalDec()&0x20)//ˮλ�쳣
        {
            flow_low_cnt = 0;
            GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//ˮλ�쳣
            GetSensor()->status[FLOW_INDEX] = 0;//�����쳣
            GetSensor()->status[NOWATER_INDEX] = 0;//ȱˮ

            if(GetSensor()->water_level == WATER_L)
            {
                water_levelAbnormal_proc();

            }
            else //ˮλ�����쳣
            {
                EleSwCtrl(SALT_SW,OFF);//�ط�2
            }

        }//end ˮλ�쳣
        else//ˮλ������������������������
        {

            EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
            GetSensor()->water_status = 0;//ˮλ�쳣���¼�ˮ����״̬��־λ
            GetSensor()->status[WATER_LEVEL_INDEX] = 0;//ˮλ���쳣

            if(work_params.init_flag == 0)//�ϵ翪��
            {
                dev_init_ctrl();
                if(GetSensor()->flow > 0)
                {
                    EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����
                }
                else
                    EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
                if(abnormalDec()&0x01||GetSensor()->flow == 0)//�����쳣
                {
                    //GetSensor()->flow = 0;
                    //registerTick(FLOW_TICK_NO, 0, 0,1);//��ʱ����λ
                    if(GetSensor()->flow == 0)//&&hsw_flag == 1
                    {
                        dev_flow_Abnormal_ctrl();
                    }
                }
                else
                {
                    if(GetSensor()->flow != 0)//�����쳣
                        FlowCtrl();
                    else
                        DcMotorCtrl(2,0);//��2��������

                }

            }
            else  if(abnormalDec()&0x01||GetSensor()->flow == 0)//�����쳣
            {

                if(GetSensor()->flow == 0)//&&hsw_flag == 1
                {
                    dev_flow_Abnormal_ctrl();
                }
                else
                {
                    EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����
                }

            }
            else  //��������
            {
                if(GetSensor()->flow != 0)//�����쳣
                    FlowCtrl();
                else
                    DcMotorCtrl(2,0);//��2��������

                GetSensor()->status[NOWATER_INDEX] = 0;//�����쳣
                GetSensor()->status[NOWATER_INDEX] = 0;//ȱˮ
                if((abnormalDec()&0x73)==0)//���쳣���������߼� 0x73 0x33
                {

                    dev_normal_ctrl();

                }
                else //�������쳣���е��ڿ���
                {
                    if(abnormalDec()&0x40)
                    {
                        GetSensor()->status[TDS1_INDEX] = TDS1_INDEX;//tds1�쳣
                    }
                    else
                    {
                        if(GetSensor()->err_flag&0x10)//tds����
                        {
                            // registerTick(WASH_TICK, WASH_TIME_SETTING,1, 0);
                            //  if( GetTickResult(WASH_TICK)==1)//��ϴʱ�䵽
                            {
                                dev_wash_ctrl();

								// GetSensor()->status[SHUNT_INDEX] = 0;
								// shunt_flag = 0;
								 //DcMotorCtrl(2,0);//�ر�2


                            }
                            /* else//��ϴʱ��δ��
                             {
                                 if(abnormalDec()&0x02==0)//tds�ָ�����
                                 {
                                     GetSensor()->status[TDS2_INDEX] = 0;//tds2�쳣
                                     EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
                                     EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����
                                     EleSwCtrl(SALT_SW,OFF);//��ˮ��ˮ����
                                     EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
                                     EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ��
                                     GetSensor()->wash_time = 0;
                                     GetSensor()->status[SHUNT_INDEX] = 0;
                                     shunt_flag = 0;
                                     //DcMotorCtrl(2,0);//�ر�2
                                 }
                                 else
                                 {
                                     if(GetSensor()->wash_time<3)
                                     {
                                         if(GetSensor()->status[HSW_INDEX]==0)
                                             GetSensor()->status[TDS2_INDEX] = 2;//tds2�쳣
                                         EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
                                         EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����
                                         EleSwCtrl(WASH_SW,ON);//����ˮ�ų�����ˮ����
                            													EleSwCtrl(SALT_SW,OFF);//��ˮ��ˮ����
                                         EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ��
                                         //DcMotorCtrl(2,0);//�ر�2

                                     }

                                 }
                                 // FlowCtrl();
                             }//��ϴʱ��δ��*/

                        }//tds����,�Զ���ϴ
                        else
                        {
                            // if(GetSensor()->flow >0)
                            {
                                GetSensor()->status[TDS2_INDEX] = 0;//tds2�쳣
                                EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
                                EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����
                                EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
                                EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ����
                            }

                            //  if(GetSensor()->wash_time>0)
                            //   GetSensor()->wash_time =GetSensor()->wash_time -1 ;
                        }

                    }//tds2�쳣


                }//�������쳣���е��ڿ���


            } //��������
            //if(((GetSensor()->flow-FLOW_SIZE)>=FLOW_SIZE*0.1)||((GetSensor()->flow-FLOW_SIZE)<=-FLOW_SIZE*0.1))    //ˮ��������


            if(GetSensor()->flow<3)	//ˮ��������
            {
                flow_low_cnt++;
                if(flow_low_cnt>=1000000)
                {
                    flow_low_cnt = 0;
                    if(GetSensor()->flow==0)
                        GetSensor()->status[FLOW_INDEX] = FLOW_INDEX;//�����쳣
                    else
                        GetSensor()->status[FLOW_INDEX] = 0;//�����쳣
                    GetSensor()->status[NOWATER_INDEX] = NOWATER_INDEX;//ȱˮ
                }
            }
            else
            {

                flow_low_cnt =0;

            }

        }// end ˮλ������������������������

    }//��ͣ��ģʽ
    else //ͣ��ģʽ
    {
        //if(abnormalDec()&0x02==0)//tds���쳣
        {
            GetSensor()->status[TDS1_INDEX] = 0;//tds2���쳣
            GetSensor()->status[TDS2_INDEX] = 0;//tds2���쳣

            GetSensor()->status[ORP_INDEX] = 0;//tds2���쳣
            GetSensor()->status[PH_INDEX] = 0;//tds2���쳣
            GetSensor()->wash_time =0;
            //GetSensor()->status[SHUNT_INDEX] = 0;
        }


    }


}
/*************************************************************************
�����߼�����ѹ������--->ˮλ����--->�������
          ��ѹ���쳣--->ͣ��
          ˮλ�쳣-->ͣ��
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
                GetSensor()->status[SHUNT_INDEX] = 0;//ͣ��
                GetSensor()->status[SHUNT_INDEX] = SHUNT_INDEX;//ͣ��
                GetSensor()->status[TDS2_INDEX] = TDS2_INDEX;//tds2�쳣

            }


        }

        EleSwCtrl(6,OFF);//�����з�
        DcMotorCtrl(7,OFF);//�����е��

        //delay_ms(500);
    }
    else
    {
        retry_flag = 0;
        if(abnormalDec()&0x10||GetSensor()->temperature<=5)//��ѹ���쳣,���²�����
        {

            hsw_proc();
        }
        else //��������ģʽ
        {
            //state_flag=1;
            //Flow_Init();
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



    if(GetInOut()->key_reset_mode )//��λ����
    {
        EleSwCtrl(6,OFF);//�����з�
        DcMotorCtrl(7,OFF);//�����е��
        // delay_ms(500);
        while(get_reset_status()!=3)
            module_reset(1);//������ģ�����λ
        Soft_Reset();
    }
    if(GetInOut()->key_wash_mode)
    {
        if(abnormalDec()&0x20)//ˮλ�쳣
        {

            GetSensor()->status[WATER_LEVEL_INDEX] = WATER_LEVEL_INDEX;//ˮλ�쳣
            GetSensor()->status[FLOW_INDEX] = 0;//�����쳣
            GetSensor()->status[NOWATER_INDEX] = 0;//ȱˮ

            if(GetSensor()->water_level == WATER_L)
            {
                water_levelAbnormal_proc();

            }
            else //ˮλ�����쳣
            {
                EleSwCtrl(SALT_SW,OFF);//�ط�2
            }

        }//end ˮλ�쳣
        else
        {

            EleSwCtrl(WATER_SW,ON);//����1
            EleSwCtrl(SALT_SW,OFF);//����2
            EleSwCtrl(WASTE_SW,ON);//����3
            EleSwCtrl(WASH_SW,ON);//����4
            EleSwCtrl(HCILO_SW,OFF);//����5
            GetSensor()->status[TDS1_INDEX] = 0;//tds1�쳣
            GetSensor()->status[TDS2_INDEX] = 0;//tds1�쳣
            if(GetSensor()->flow != 0)//�����쳣
                FlowCtrl();

            else
                DcMotorCtrl(2,0);//��2��������
            registerTick(WASH_TICK, WASH_TIME_SETTING,1, 0);
            if( GetTickResult(WASH_TICK)==1)
            {
                GetInOut()->key_wash_mode = 0;
                registerTick(WASH_TICK, 0,0,1);
                GetSensor()->status[WASH_INDEX] = 0;//washing
                EleSwCtrl(WASTE_SW,OFF);//����3
                //DcMotorCtrl(2,0);//�ر�2


            }

        }

    }
    else if(GetInOut()->key_cali_mode != 0||getTouch()->next_page!=Main_PAGE)
    {
        GetInOut()->key_wash_mode = 0;
        // registerTick(WASH_TICK, 0,0,1);
        GetSensor()->status[8] = 0;
        EleSwCtrl(6,OFF);//�����з�
        DcMotorCtrl(7,OFF);//�����е��
        registerTick(FLOW_TICK_NO, 0, 0,1);//��ʱ����λ
        //delay_ms(500);

    }

    else
        ele_dev_proc();
    if((GetSensor()->status[NORMAL_INDEX]==20&&GetSensor()->flow>0&&  //������������ٶ�
            GetInOut()->key_cali_mode==0)||GetInOut()->key_wash_mode
      )//��ѹ���غ�ˮλ�����Ҳ���У׼ģʽʱ�������
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
    for(i=1; i<WORK_STATUS_SIZE; i++)    //����״̬����
    {

        if(GetSensor()->status[i]!=0)
        {

            if(i!=ORP_INDEX&&i!=PH_INDEX)
            {
                GetSensor()->status[NORMAL_INDEX] = 0;
                if(i==NORMAL_INDEX||i==WASH_INDEX)
                {
                    shunt_flag=0;
                    GetSensor()->status[SHUNT_INDEX] = 0;
                    GetSensor()->wash_time =0;
                }
                if(i!=WATER_LEVEL_INDEX&&i!=WASH_INDEX&&GetInOut()->key_cali_mode == 0)
                    beep_pro(6,0);
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




}
void work_process()
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
}
void app_test()
{
    EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
    EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����		
	  EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ����
	  EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����	
   // EleSwCtrl(WASH_SW,ON);//����ˮ�ų�����ˮ����
    //EleSwCtrl(SALT_SW,ON);//�κн�ˮ����	
    EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
    //EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����
	
    
   // EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
	
    EleSwCtrl(WATER_SW,ON);//ԭˮ��ˮ����
   // EleSwCtrl(SALT_SW,ON);//�κн�ˮ����
  //  EleSwCtrl(WASTE_SW,ON);//��ˮ��ˮ����
	
  //  EleSwCtrl(WASH_SW,ON);//����ˮ�ų�����ˮ����	
   // EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
    EleSwCtrl(HCILO_SW,ON);//����ˮ��ˮ����
  //  EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����	
  //  EleSwCtrl(WATER_SW,OFF);//ԭˮ��ˮ����
//	
//	
//  
//    EleSwCtrl(SALT_SW,OFF);//�κн�ˮ����
//    EleSwCtrl(WASTE_SW,OFF);//��ˮ��ˮ����
//    EleSwCtrl(WASH_SW,OFF);//����ˮ�ų�����ˮ����
//    EleSwCtrl(HCILO_SW,OFF);//����ˮ��ˮ����
//  EleSwCtrl(WATER_SW,OFF);//ԭˮ��ˮ����
//    DcMotorCtrl(2,0);//�ر�2


}



void SystemParamsSave()
{
//    uint32_t addr,len;
//    unsigned char tmp;
//    if(GetEspStatus()->update == 1)
//    {
//        FlashEease(0);
//        addr = 0;
//        tmp = 0x5b; //������־�������Ѿ��������
//        bsp_WriteCpuFlash(addr, &tmp, 1);
//        addr = 1;

//        len = strlen(GetEspStatus()->User_ESP8266_Apssid);
//        bsp_WriteCpuFlash(addr, GetEspStatus()->User_ESP8266_Apssid, len);
//        addr = addr + len;
//        len = strlen(GetEspStatus()->User_ESP8266_ApPwd);
//        bsp_WriteCpuFlash(addr, GetEspStatus()->User_ESP8266_ApPwd, len);

//        addr = addr + len;
//        len = strlen(GetEspStatus()->User_Server_IP);
//        bsp_WriteCpuFlash(addr, GetEspStatus()->User_Server_IP, len);
//        addr = addr + len;
//        len = strlen(GetEspStatus()->User_Server_Port);
//        bsp_WriteCpuFlash(addr, GetEspStatus()->User_Server_Port, len);

//        GetEspStatus()->update = 0 ;
//        GetEspStatus()->status = 1;
//    }
//    else if(GetEspStatus()->update > 1)
//    {


//        addr = 0;
//        bsp_ReadCpuFlash(addr, &tmp, 1);
//        if(tmp == 0x5b)
//            tmp = 0x5b;
//        else
//            tmp = 0x5a;

//        FlashEease(0);
//        addr = 0;
//        bsp_WriteCpuFlash(addr, &tmp, 1);
//        addr = 1;

//        len = strlen(GetEspStatus()->User_ESP8266_Apssid);
//        bsp_WriteCpuFlash(addr, GetEspStatus()->User_ESP8266_Apssid, len);
//        addr = addr + len;
//        len = strlen(GetEspStatus()->User_ESP8266_ApPwd);
//        bsp_WriteCpuFlash(addr, GetEspStatus()->User_ESP8266_ApPwd, len);

//        addr = addr + len;
//        len = strlen(GetEspStatus()->User_Server_IP);
//        bsp_WriteCpuFlash(addr, GetEspStatus()->User_Server_IP, len);
//        addr = addr + len;
//        len = strlen(GetEspStatus()->User_Server_Port);
//        bsp_WriteCpuFlash(addr, GetEspStatus()->User_Server_Port, len);

//        GetEspStatus()->update = 0 ;
//        GetEspStatus()->status = 0;

//    }
}
void SystemParamsRead()
{
    uint32_t addr;
    unsigned char tmp;
    // unsigned int i;

    addr = 0;
    bsp_ReadCpuFlash(addr, &tmp, 1);
    if(tmp == 0x5a||tmp == 0x5b)
    {
//        if(tmp == 0x5b)
//            GetEspStatus()->status = 1;
//        addr = addr + 1;

//        for(i=0; i<ESP_BUF_SIZE; i++)
//        {
//            bsp_ReadCpuFlash(addr+i,&tmp, 1);
//            GetEspStatus()->User_ESP8266_Apssid[i] =tmp;
//            if(tmp=='\0')
//            {
//                break;
//            }
//        }
//        len = i;
//        addr = addr + len;
//        for(i=0; i<ESP_BUF_SIZE; i++)
//        {
//            bsp_ReadCpuFlash(addr+i,&tmp, 1);
//            GetEspStatus()->User_ESP8266_ApPwd[i] =tmp;
//            if(tmp=='\0')
//            {
//                break;
//            }
//        }
//        len = i;
//        addr = addr + len;


//        for(i=0; i<ESP_BUF_SIZE; i++)
//        {
//            bsp_ReadCpuFlash(addr+i,&tmp, 1);
//            GetEspStatus()->User_Server_IP[i] =tmp;
//            if(tmp=='\0')
//            {
//                break;
//            }
//        }
//        len = i;
//        addr = addr + len;
//        for(i=0; i<ESP_BUF_SIZE; i++)
//        {
//            bsp_ReadCpuFlash(addr+i,&tmp, 1);
//            GetEspStatus()->User_Server_Port[i] =tmp;
//            if(tmp=='\0')
//            {
//                break;
//            }
//        }
//        len = i;
//        addr = addr + len;

    }
    else
    {
        //   SystemParamsSave();


    }


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
    ele_process();
    lcd_proc();

}
