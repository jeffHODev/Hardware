#include "app.h"
#include "modbus.h"
#include "eeprom.h"
#include "adc.h"
#include "FdrAlgorithm.h"
extern factor_stru factor_usr;//ʪ���㷨ϵ��
extern modbus_stru modbus_usr;//modbus����
extern sensor_stru sensor_usr;//adc�ɼ�ת������
/************************************************
���ܣ��洢�ؼ�����
*************************************************/
void fator_save_proc(unsigned char addr)
{
    uint32_t tmp;
    tmp = FloatToCharProc(factor_usr.a0,10);//ʪ��ϵ��1
    flash_write(addr++,tmp>>24,1);
    flash_write(addr++,(tmp>>16&0x000000ff),1);
    flash_write(addr++,(tmp>>8&0x000000ff),1);
    flash_write(addr++,(tmp&0x000000ff),1);

    tmp = FloatToCharProc(factor_usr.a1,10);//ʪ��ϵ��2
    flash_write(addr++,tmp>>24,1);
    flash_write(addr++,(tmp>>16&0x000000ff),1);
    flash_write(addr++,(tmp>>8&0x000000ff),1);
    flash_write(addr++,(tmp&0x000000ff),1);

    tmp = FloatToCharProc(factor_usr.a2,10);//ʪ��ϵ��3
    flash_write(addr++,tmp>>24,1);
    flash_write(addr++,(tmp>>16&0x000000ff),1);
    flash_write(addr++,(tmp>>8&0x000000ff),1);
    flash_write(addr++,(tmp&0x000000ff),1);

    tmp = FloatToCharProc(sensor_usr.ADC_REF,1000);//�ο���ѹ
    flash_write(addr++,tmp>>24,1);
    flash_write(addr++,(tmp>>16&0x000000ff),1);
    flash_write(addr++,(tmp>>8&0x000000ff),1);
    flash_write(addr++,(tmp&0x000000ff),1);

    tmp = FloatToCharProc(sensor_usr.CalibrationProbeVref,1000);//̽ͷ�ղ��ѹ
    flash_write(addr++,tmp>>24,1);
    flash_write(addr++,(tmp>>16&0x000000ff),1);
    flash_write(addr++,(tmp>>8&0x000000ff),1);
    flash_write(addr++,(tmp&0x000000ff),1);

    flash_write(addr++,factor_usr.status,1);//У׼��ɱ�־λ

}
/**************************************************************
������������ʼ��
***************************************************************/
void params_init()
{
    uint32_t p[26];

    unsigned char addr;
    addr = 0;
    flash_read(addr,p,25);
    modbus_usr.RS485_Addr =0x01;
    modbus_usr.RS485_Baudrate = 9600;
    modbus_usr.RS485_Parity = 0;
    sensor_usr.CalibrationT = 0;
    sensor_usr.CalibrationR = 0;
    if(p[0]==0x0000005a)//�����洢����ֱ�Ӷ�ȡ����
    {
        modbus_usr.RS485_Addr=p[1];
        modbus_usr.RS485_Baudrate=p[2];
        modbus_usr.RS485_Parity=p[3];

        factor_usr.a0 = DataMinusProc(&p[4],1,10);
        factor_usr.a1 =  DataMinusProc(&p[8],1,10);
        factor_usr.a2 =  DataMinusProc(&p[12],1,10);
        if(p[16]!=0xffffffff)
            sensor_usr.ADC_REF = DataMinusProc(&p[16],1,1000);
        if(p[20]!=0xffffffff)
            sensor_usr.CalibrationProbeVref = DataMinusProc(&p[20],1,1000);
        if(p[24]!=0xffffffff)
            factor_usr.status = p[24];
        if(p[25]!=0xffffffff)
            sensor_usr.CalibrationT = p[25];



    }
    else
    {
        //��ʼ����
        flash_init();
        factor_usr.a0 = 82.93;
        factor_usr.a1 = -27.688;
        factor_usr.a2 = 12.822;
        factor_usr.status = 0;
        flash_write(addr++,0x5a,1);
        flash_write(addr++,modbus_usr.RS485_Addr,1);
        flash_write(addr++,modbus_usr.RS485_Baudrate,1);
        flash_write(addr++,modbus_usr.RS485_Parity,1);
        fator_save_proc(addr);

        flash_write(addr+12+4+4+1,sensor_usr.CalibrationT,1);
    }


}
void params_save()
{
    unsigned char addr;
    addr = 0;
    flash_init();
    flash_write(addr++,0x5a,1);
    flash_write(addr++,modbus_usr.RS485_Addr,1);
    flash_write(addr++,modbus_usr.RS485_Baudrate,1);
    flash_write(addr++,modbus_usr.RS485_Parity,1);
    fator_save_proc(addr);
    flash_write(addr+12+4+4+1,sensor_usr.CalibrationT,1);		//�¶�У׼
    //flash_write(addr+12+4+4+1,factor_usr.status,1); 	//У׼��ɱ�־λ

}
void app_loop()
{

    RS485_Service();//������
}
