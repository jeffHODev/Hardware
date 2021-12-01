#include "app.h"
#include "sensor.h"
#include "devCtrl.h"
#include "sys.h"
#include "bsp_beep.h"
#include "inout.h"
#include "app_esp8266.h"
#include "app_lcd.h"
#include "bsp_cpu_flash.h"
void SystemParamsRead(void);

work_params_stru work_params;



void app_init()
{
    SystemParamsRead();
    adcInit();
    inout_Init();
    work_params.work_mode = ON;

}
unsigned char TickTimeoutAb(unsigned char TickNum,unsigned char BitOper,uint32_t timeout)
{
    if(GetTickStatus(TickNum)!=1)//������תΪ�쳣
    {
        registerTick(TickNum, timeout, 1,0);//��ʱ��ʱ��ʼ
    }
    if( GetTickResult(TickNum)==1&&GetTickStatus(TickNum)==1)
    {
        // status = status | BitOper;
        registerTick(TickNum, 0, 0,1);//��ʱ����λ
        return BitOper;
    }
    else
        return 0;
}

unsigned char TickTimeoutNor(unsigned char TickNum,unsigned char BitOper,uint32_t timeout)
{
    if(GetTickStatus(TickNum)==1||GetTickStatus(TickNum)==0)//����
    {
        registerTick(TickNum, timeout, 2,0);//��ʱ��ʱ��ʼ
        if(GetTickStatus(TickNum)==0)
            return BitOper;
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
    unsigned char status;

    if(work_params.init_flag == 0)//�ϵ��ʼ�������ˮ�ӷ�ˮͨ���ų�
        registerTick(StART_TICK_NO, 180000,1, 0);
    if( GetTickResult(StART_TICK_NO)==1)
    {
        work_params.init_flag = 1;//3����ʱ�䵽���������߼�����
        registerTick(StART_TICK_NO, 0,0,1);
    }


    //********************************************************************************

    if(GetSensor()->flow >= MAX_FLOW|| GetSensor()->flow < MIN_FLOW)//�����쳣
    {
        /* if(GetTickStatus(FLOW_TICK_NO)��=1)//������תΪ�쳣
         	{
         		registerTick(FLOW_TICK_NO, MAX_TICK, 1,0);//��ʱ��ʱ��ʼ
         }
          if( GetTickResult(FLOW_TICK_NO)==1)
          {
              status = status | 0x01;
              registerTick(FLOW_TICK_NO, 0, 0,1);//��ʱ����λ
          }*/
        status = status | TickTimeoutAb(FLOW_TICK_NO,0x01,MAX_TICK);
    }
    else
    {
        /*if(GetTickStatus(FLOW_TICK_NO)==1||GetTickStatus(FLOW_TICK_NO)==0)//����
         {
        	 registerTick(FLOW_TICK_NO, MAX_TICK, 2,0);//��ʱ��ʱ��ʼ
             if(GetTickStatus(FLOW_TICK_NO)==0)
        	 	status = status & 0xfe;
        }
        if( GetTickResult(FLOW_TICK_NO)==1&&GetTickStatus(FLOW_TICK_NO)==2)
        {
            status = status | 0x01;
            registerTick(FLOW_TICK_NO, 0, 0,1);//��ʱ����λ
            status = status & 0xfe;
        }*/

        status = status & TickTimeoutNor(FLOW_TICK_NO,0xfe,MAX_TICK);
    }

//********************************************************************************
    if(GetSensor()->tds >= MAX_TDS_VALUE|| GetSensor()->tds < MIN_TDS_VALUE)//tds�쳣
    {
        /*egisterTick(TDS_TICK_NO, MAX_TICK, 0);
         if( GetTickResult(TDS_TICK_NO)==1)
         {
             status = status | 0x02;
             registerTick(TDS_TICK_NO, 0, 1);
             GetSensor()->err_flag =GetSensor()->err_flag |0x01;//��ʱ��ʱ

         }*/
        status = status | TickTimeoutAb(TDS_TICK_NO,0x02,MAX_TICK);
        if(status &0x02)
        {
            GetSensor()->err_flag =GetSensor()->err_flag |0x01;//��ʱ��ʱ

        }

        if((HAL_GetTick()-GetSensor()->timeout )>=WASH_TIME)
        {
            GetSensor()->err_flag = GetSensor()->err_flag & 0xfe;
            GetSensor()->err_flag = GetSensor()->err_flag | 0x10;//��ʱ
        }
    }
    else
    {
        status = status & TickTimeoutNor(FLOW_TICK_NO,0xfd,MAX_TICK);
        if((status & 0x02) == 0)
            GetSensor()->timeout = HAL_GetTick();
        /* if(GetSensor()->err_flag == 0)
         {
             status = status & 0xfd;
        	registerTick(TDS_TICK_NO, 0, 1);
        	GetSensor()->timeout = HAL_GetTick();

        }
        else
        {
             registerTick(TDS_TICK_NO, MAX_TICK, 0);
             if( GetTickResult(TDS_TICK_NO)==1)
             {
                 status = status & 0xfd;
                 registerTick(TDS_TICK_NO, 0, 1);
                 GetSensor()->err_flag =0;

             }

        }*/



    }
    //********************************************************************************

    if(GetSensor()->ph>= MAX_PH||GetSensor()->ph < MIN_PH)//ph�쳣
    {
        status = status | TickTimeoutAb(PH_TICK_NO,0x04,MAX_TICK);
        /*registerTick(PH_TICK_NO, MAX_TICK, 0);
        if( GetTickResult(PH_TICK_NO)==1)
        {
            status = status | 0x04;
            registerTick(PH_TICK_NO, 0, 1);
        }*/
    }
    else
    {
        /* registerTick(PH_TICK_NO, 0, 1);
         status = status & 0xfb;*/
        status = status & TickTimeoutNor(PH_TICK_NO,0xfb,MAX_TICK);
    }
    //********************************************************************************

    if(GetSensor()->orp >= MAX_ORP||GetSensor()->orp < MIN_ORP)//orp�쳣
    {
        status = status | TickTimeoutAb(ORP_TICK_NO,0x08,MAX_TICK);
        /* registerTick(ORP_TICK_NO, MAX_TICK, 0);
         if( GetTickResult(ORP_TICK_NO)==1)
         {
             status = status | 0x08;
             registerTick(ORP_TICK_NO, 0, 1);
         }*/
    }
    else
    {
        /*registerTick(ORP_TICK_NO, 0, 1);

        status = status & 0xf7;*/
        status = status & TickTimeoutNor(ORP_TICK_NO,0xf7,MAX_TICK);
    }
    //********************************************************************************

    if(GetSensor()->water_level == WATER_F	||GetSensor()->water_level == WATER_L)//ˮλ�쳣
    {
        status = status | TickTimeoutAb(WATER_TICK_NO,0x20,MAX_TICK);
        /*registerTick(WATER_TICK_NO, MAX_TICK, 0);
        if( GetTickResult(WATER_TICK_NO)==1)
        {
            status = status | 0x20;
            registerTick(WATER_TICK_NO, 0, 1);
        }*/

    }
    else
    {
        /* registerTick(WATER_TICK_NO, 0, 1);
         status = status & 0xdf;*/
        status = status & TickTimeoutNor(WATER_TICK_NO,0xdf,MAX_TICK);
    }
    //********************************************************************************

    if(GetSensor()->swH ==ON)//��ѹ�����쳣
    {
        status = status | TickTimeoutAb(SW_TICK_NO,0x10,MAX_TICK);
        /* registerTick(SW_TICK_NO, MAX_TICK, 0);
         if( GetTickResult(SW_TICK_NO)==1)
         {
             status = status | 0x10;
             registerTick(SW_TICK_NO, 0, 1);
         }*/
    }
    else
    {
        /*registerTick(SW_TICK_NO, 0, 1);
        status = status & 0xef;*/
        status = status & TickTimeoutNor(SW_TICK_NO,0xef,MAX_TICK);
    }


    return status;
}
uint32_t dstTds;
void FlowCtrl()
{

    static int32_t pwm1,pwm2;
    dstTds = 1100;
    pwm2 = dstTds - GetSensor()->tds;
    pwm1 =  GetSensor()->tds;
    pwm2 =  pwm2*10;
    pwm1 =  pwm1*10;

    if(pwm1>=65535)
        pwm1 = 65535;
    if(pwm1<0)
        pwm1 = 0;

    if(pwm2>=65535)
        pwm2 = 65535;
    if(pwm2<0)
        pwm2 = 100;

    if(GetSensor()->tds >= MAX_TDS_VALUE)
    {
        pwm1 =	GetSensor()->tds;
        pwm1 =	pwm1*10;
        EleSwCtrl(1,ON);
        DcMotorCtrl(0,pwm1);//��1��������
        DcMotorCtrl(1,pwm2);//��2��������
    }
    else  if(GetSensor()->tds <= MIN_TDS_VALUE)
    {
        pwm2 =	65535;
        EleSwCtrl(1,OFF);
        DcMotorCtrl(0,0);//��1��������
        DcMotorCtrl(1,pwm2);//��2��������

    }
    else
    {

        if(pwm1>0)
        {
            EleSwCtrl(1,ON);

        }
        else
            EleSwCtrl(1,OFF);

        DcMotorCtrl(0,pwm1);//��1��������
        DcMotorCtrl(1,pwm2);//��2��������

    }


}


void work_process()
{
    if(work_params.work_mode == ON)
    {


        sensor_process();//
        if(abnormalDec()&0x10)//��ѹ���쳣
        {
            EleCtrl(SHUNT,ON);//���ر�
            EleSwCtrl(6,OFF);//�����з�
            DcMotorCtrl(7,OFF);//�����е��
        }
        else //��������ģʽ
        {
            if(abnormalDec()&0x0c||work_params.init_flag == 0)//�����쳣,����ˮ�ų�;�ϵ翪������ˮ�ų�
            {

                EleSwCtrl(3,ON);//����3
                EleSwCtrl(4,ON);//����4
                EleSwCtrl(5,OFF);//����5
                FlowCtrl();
                //EleCtrl(FORWARD,1);
            }
            if(abnormalDec()&0x02)//tds�쳣
            {
                FlowCtrl();
                // EleCtrl(FORWARD,1);

            }

            if(abnormalDec()==0)//���쳣
            {
                //EleSwCtrl(1,OFF);//����1
                //EleSwCtrl(2,OFF);//����2
                EleSwCtrl(3,ON);//����3
                EleSwCtrl(4,OFF);//�ط�4
                EleSwCtrl(5,ON);//����5
                FlowCtrl();

            }
            EleCtrl(FORWARD,1);
            if(abnormalDec()&0x20)//ˮλ�쳣
            {

                if(abnormalDec()&0x20==WATER_L)//ˮλ��
                {
                    EleSwCtrl(1,ON);//����1
                    EleSwCtrl(2,ON);//����2
                    EleSwCtrl(3,ON);//����3
                    EleSwCtrl(4,ON);//����4
                    EleSwCtrl(5,OFF);//����5
                    DcMotorCtrl(1,2000);//����1
                    DcMotorCtrl(2,0);//�ر�2
                    GetSensor()->water_status = 1;

                }

                else //ˮλ�����쳣
                {
                    // EleSwCtrl(1,OFF);//�ط�1
                    EleSwCtrl(2,OFF);//�ط�2
                    //	DcMotorCtrl(3,0);//�ص��

                }
                EleCtrl(SHUNT,1);

            }


        }
        if(GetInOut()->key_reset_mode )//��λ����
        {
            Soft_Reset();
        }
        if(GetInOut()->key_wash_mode )
        {
            EleSwCtrl(1,ON);//����1
            EleSwCtrl(2,OFF);//����2
            EleSwCtrl(3,ON);//����3
            EleSwCtrl(4,OFF);//����4
            EleSwCtrl(5,OFF);//����5
            DcMotorCtrl(1,2000);//����1
            DcMotorCtrl(2,0);//�ر�2
            registerTick(WASH_TICK, 180000,1, 0);
            if( GetTickResult(WASH_TICK)==1)
            {
                GetInOut()->key_wash_mode = 0;
                registerTick(WASH_TICK, 0,0,1);
            }

        }
    if(GetSensor()->err_flag&0x10)//ˮ�������ⱨ��
    {
        registerTick(WASH_TICK, 180000,1, 0);
        if( GetTickResult(WASH_TICK)==1)
        {
            GetSensor()->err_flag = 0;
            registerTick(WASH_TICK, 0,0,1);
        }


        EleSwCtrl(1,ON);//����1
        EleSwCtrl(2,OFF);//����2
        EleSwCtrl(3,ON);//����3
        EleSwCtrl(4,OFF);//����4
        EleSwCtrl(5,OFF);//����5
        DcMotorCtrl(1,2000);//����1
        DcMotorCtrl(2,0);//�ر�2

    }

    }




}
void app_test()
{


    EleSwCtrl(6,OFF);
    HAL_Delay(1000);
    DcMotorCtrl(1,0);
    HAL_Delay(1000);
    DcMotorCtrl(2,0);
    HAL_Delay(1000);
    DcMotorCtrl(3,0);
    HAL_Delay(1000);
    DcMotorCtrl(4,0);
    HAL_Delay(1000);

    EleSwCtrl(1,OFF);
    HAL_Delay(1000);
    EleSwCtrl(2,OFF);
    HAL_Delay(1000);
    EleSwCtrl(3,OFF);
    HAL_Delay(1000);
    EleSwCtrl(4,OFF);
    HAL_Delay(1000);
    EleSwCtrl(5,OFF);
    HAL_Delay(1000);

    Send_LED_Data();


}



void SystemParamsSave()
{
    uint32_t addr,len;
    unsigned char tmp;
    if(GetEspStatus()->update == 1)
    {
        FlashEease(0);
        addr = 0;
        tmp = 0x5b; //������־�������Ѿ��������
        bsp_WriteCpuFlash(addr, &tmp, 1);
        addr = 1;

        len = strlen(GetEspStatus()->User_ESP8266_Apssid);
        bsp_WriteCpuFlash(addr, GetEspStatus()->User_ESP8266_Apssid, len);
        addr = addr + len;
        len = strlen(GetEspStatus()->User_ESP8266_ApPwd);
        bsp_WriteCpuFlash(addr, GetEspStatus()->User_ESP8266_ApPwd, len);

        addr = addr + len;
        len = strlen(GetEspStatus()->User_Server_IP);
        bsp_WriteCpuFlash(addr, GetEspStatus()->User_Server_IP, len);
        addr = addr + len;
        len = strlen(GetEspStatus()->User_Server_Port);
        bsp_WriteCpuFlash(addr, GetEspStatus()->User_Server_Port, len);

        GetEspStatus()->update = 0 ;
        GetEspStatus()->status = 1;
    }
    else if(GetEspStatus()->update > 1)
    {


        addr = 0;
        bsp_ReadCpuFlash(addr, &tmp, 1);
        if(tmp == 0x5b)
            tmp = 0x5b;
        else
            tmp = 0x5a;

        FlashEease(0);
        addr = 0;
        bsp_WriteCpuFlash(addr, &tmp, 1);
        addr = 1;

        len = strlen(GetEspStatus()->User_ESP8266_Apssid);
        bsp_WriteCpuFlash(addr, GetEspStatus()->User_ESP8266_Apssid, len);
        addr = addr + len;
        len = strlen(GetEspStatus()->User_ESP8266_ApPwd);
        bsp_WriteCpuFlash(addr, GetEspStatus()->User_ESP8266_ApPwd, len);

        addr = addr + len;
        len = strlen(GetEspStatus()->User_Server_IP);
        bsp_WriteCpuFlash(addr, GetEspStatus()->User_Server_IP, len);
        addr = addr + len;
        len = strlen(GetEspStatus()->User_Server_Port);
        bsp_WriteCpuFlash(addr, GetEspStatus()->User_Server_Port, len);

        GetEspStatus()->update = 0 ;
        GetEspStatus()->status = 0;

    }
}
void SystemParamsRead()
{
    uint32_t addr,len;
    unsigned char tmp;
    unsigned int i;

    addr = 0;
    bsp_ReadCpuFlash(addr, &tmp, 1);
    if(tmp == 0x5a||tmp == 0x5b)
    {
        if(tmp == 0x5b)
            GetEspStatus()->status = 1;
        addr = addr + 1;

        for(i=0; i<ESP_BUF_SIZE; i++)
        {
            bsp_ReadCpuFlash(addr+i,&tmp, 1);
            GetEspStatus()->User_ESP8266_Apssid[i] =tmp;
            if(tmp=='\0')
            {
                break;
            }
        }
        len = i;
        addr = addr + len;
        for(i=0; i<ESP_BUF_SIZE; i++)
        {
            bsp_ReadCpuFlash(addr+i,&tmp, 1);
            GetEspStatus()->User_ESP8266_ApPwd[i] =tmp;
            if(tmp=='\0')
            {
                break;
            }
        }
        len = i;
        addr = addr + len;


        for(i=0; i<ESP_BUF_SIZE; i++)
        {
            bsp_ReadCpuFlash(addr+i,&tmp, 1);
            GetEspStatus()->User_Server_IP[i] =tmp;
            if(tmp=='\0')
            {
                break;
            }
        }
        len = i;
        addr = addr + len;
        for(i=0; i<ESP_BUF_SIZE; i++)
        {
            bsp_ReadCpuFlash(addr+i,&tmp, 1);
            GetEspStatus()->User_Server_Port[i] =tmp;
            if(tmp=='\0')
            {
                break;
            }
        }
        len = i;
        addr = addr + len;

    }
    else
    {
        SystemParamsSave();


    }


}
void app()
{
    //sensor_process();
   // app_test();
    //esp8266_process();
    work_process();
    SystemParamsSave();
	// lcd_main();
}
