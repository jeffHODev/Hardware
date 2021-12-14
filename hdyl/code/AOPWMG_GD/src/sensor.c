#include "sensor.h"
#include "timer.h"
#include "filter.h"
#include "modbus.h"
#include "sys.h"
#include "Inout.h"
#include "main.h"
#include "math.h"
// 20℃下TDS测试值该值为ADC采样过来的�?
const uint16_t TDS_Test_Vaule[TDS_NUM]=
{
    4000,
    3942,
    3886,
    3793,
    3682,
    3586,
    3494,
    3424,
    3335,
    3263,
    3183,
    3090,
    2998,
    2951,
    2903,
    2854,
    2784,
    2735,
    2663,
    2615,
    2578,
    2530,
    2487,
    2422,
    2367,
    2336,
    2272,
    2223,
    2166,
    2130,
    2102,
    2055,
    2016,
    1984,
    1956,
    1926,
    1885,
    1855,
    1836,
    1808,
    1770,
    1746,
    1718,
    1700,
    1679,
    1647,
    1623,
    1607,
    1600,
    1596,
    1571,
    1558,
    1543,
    1510,
    1479,
    1462,
    1440,
    1414,
    1380,
    1359,
    1334,
    1318,
    1302,
    1286,
    1260,
    1248,
    1234,
    1218,
    1215,
    1206,
    1186,
    1170,
    1147,
    1132,
    1120,
    1107,
    1094,
    1086,
    1046,
    1037,
    1030,
    1018,
    1007,
    1000,
    992 ,
    982 ,
    973 ,
    964 ,
    958 ,
    954 ,
    945 ,
    927 ,
    897 ,
    864 ,
    840 ,
    831 ,
    822 ,
    813 ,
    804 ,
    797 ,
    784 ,
    773 ,
    764 ,
    747 ,
    739 ,
    729 ,
    727 ,
    727 ,
    717 ,
    706 ,
    702 ,
    696

};

// 20℃下TDS值该值为TDS标定�?
const uint16_t TDS_Vaule[TDS_NUM]=
{
    5   ,
    6   ,
    10  ,
    15  ,
    20  ,
    27  ,
    32  ,
    38  ,
    43  ,
    47  ,
    54  ,
    61  ,
    69  ,
    74  ,
    77  ,
    83  ,
    89  ,
    95  ,
    100 ,
    106 ,
    113 ,
    117 ,
    123 ,
    128 ,
    138 ,
    147 ,
    156 ,
    164 ,
    171 ,
    179 ,
    186 ,
    195 ,
    202 ,
    208 ,
    217 ,
    226 ,
    233 ,
    241 ,
    249 ,
    258 ,
    266 ,
    274 ,
    283 ,
    290 ,
    300 ,
    307 ,
    314 ,
    319 ,
    327 ,
    333 ,
    337 ,
    348 ,
    360 ,
    376 ,
    395 ,
    408 ,
    423 ,
    433 ,
    443 ,
    462 ,
    473 ,
    483 ,
    492 ,
    505 ,
    517 ,
    529 ,
    538 ,
    548 ,
    558 ,
    562 ,
    577 ,
    596 ,
    614 ,
    628 ,
    642 ,
    658 ,
    668 ,
    680 ,
    687 ,
    696 ,
    704 ,
    713 ,
    724 ,
    735 ,
    750 ,
    761 ,
    773 ,
    785 ,
    802 ,
    810 ,
    825 ,
    856 ,
    891 ,
    950 ,
    955 ,
    1020,
    1050,
    1080,
    1100,
    1130,
    1150,
    1200,
    1220,
    1240,
    1260,
    1300,
    1350,
    1390,
    1440,
    1450,
    1470,
    1520

};

#if CPU == ST
#define TDS_P(a)		do{if (a)\
										HAL_GPIO_WritePin(TDS_P_GPIO_Port,TDS_P_Pin,GPIO_PIN_SET);\
										else\
										HAL_GPIO_WritePin(TDS_P_GPIO_Port,TDS_P_Pin,GPIO_PIN_RESET);}while(0)
#else
#define TDS_P(a)		do{ if (a)\
										gpio_bit_write(TDS_P_GPIO_Port,TDS_P_Pin,SET);\
										else\
										gpio_bit_write(TDS_P_GPIO_Port,TDS_P_Pin,RESET);} while(0)
#endif
// TDS-P
#if CPU == ST
#define TDS_N(a)		do{if (a)\
										HAL_GPIO_WritePin(TDS_N_GPIO_Port,TDS_N_Pin,GPIO_PIN_SET);\
										else\
										HAL_GPIO_WritePin(TDS_N_GPIO_Port,TDS_N_Pin,GPIO_PIN_RESET);}while(0)
#else
#define TDS_N(a)		do{ if (a)\
										gpio_bit_write(TDS_N_GPIO_Port,TDS_N_Pin,SET);\
										else\
										gpio_bit_write(TDS_N_GPIO_Port,TDS_N_Pin,RESET);} while(0)
#endif
// TDS-N

#if CPU == ST
extern ADC_HandleTypeDef hadc1;
#endif
sensor_stru sensor;
uint16_t adc1_val_buf[5];

uint16_t *get_adc_buf()
{
    return adc1_val_buf;

}

void adcInit()
{
#if CPU == ST
    HAL_GPIO_WritePin(TDS_N_GPIO_Port, TDS_N_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(TDS_P_GPIO_Port, TDS_P_Pin, GPIO_PIN_SET);
    HAL_ADC_Start_DMA(&hadc1,(uint32_t*) adc1_val_buf, 3);
#else

#endif

}
void adcResume()
{
#if CPU == ST
    HAL_ADC_Start_DMA(&hadc1,(uint32_t*) adc1_val_buf, 3);
#endif

}
/**************************************************************
  00: 满水
  01：故�?
  10：水位适中
  11：无�?

**************************************************************/
void GetWaterLevel()
{
    static unsigned char result;

#if CPU == ST
    if(HAL_GPIO_ReadPin(TEST_VA_GPIO_Port, TEST_VA_Pin)==1)//水压�?
#else
    if(gpio_input_bit_get(TEST_VA_GPIO_Port, TEST_VA_Pin)==1)//水压�?
#endif

    {
        sensor.swH = 0;
    }
    else
    {
        sensor.swH = 1;
    }

#if CPU == ST
    if(HAL_GPIO_ReadPin(SensorB_GPIO_Port, SensorB_Pin)==WATER_LEVEL)//水位低于最低�?
#else
    if(gpio_input_bit_get(SensorB_GPIO_Port, SensorB_Pin)==WATER_LEVEL)//水位低于最低�?
#endif
    {
        result = result & 0x02;
        result = result | 0x01;
    }
    else
    {
        result = result & 0xfe;
    }

#if CPU == ST
    if(HAL_GPIO_ReadPin(SensorA_GPIO_Port, SensorA_Pin)==WATER_LEVEL)//水位低于最高水�?
#else
    if(gpio_input_bit_get(SensorA_GPIO_Port, SensorA_Pin)==WATER_LEVEL)//水位大于等于于最高水�?
#endif
    {
        result = result & 0x01;
        result = result | 0x02;
    }
    else
    {
        result = result & 0xfd;
    }

    if(GetSensor()->water_status == 1)
    {
        if(result!=2)//水位低于最高水�?
        {
            result = 1;
        }
        else
        {
            GetSensor()->water_status == 0;
            result = 2;
        }

    }

    switch(result)
    {
    case 0:
        sensor.water_level = WATER_M;//水位适中
        break;
    case 1:
        sensor.water_level = WATER_L;//缺水
        break;
    case 2:
        sensor.water_level = WATER_H;//满水
        break;
    case 3:
        sensor.water_level = WATER_F;//传感器故�?
        break;
    default:
        sensor.water_level = WATER_M;//水位适中
        break;

    }

}
uint16_t adc_channel_sample(uint8_t channel)
{
    /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 0U, channel, ADC_SAMPLETIME_84);
    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);

    /* wait the end of conversion flag */
    while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
    /* clear the end of conversion flag */
    adc_flag_clear(ADC0, ADC_FLAG_EOC);
    /* return regular channel sample value */
    return (adc_regular_data_read(ADC0));
}
uint32_t tt;
void GetTds_EleCurr()
{
    static float result,averTds,averTds2;
    // uint32_t result;
    unsigned int j;
    static unsigned char init;
    float tds_tmp;

    adc1_val_buf[2] = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_0);
    adc1_val_buf[3] = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_1);
    adc1_val_buf[4] = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_2);
    adc_software_trigger_enable(ADC0, ADC_INSERTED_CHANNEL);




    result = adc1_val_buf[2];
    result = 1.2*4095/result;
    if(init == 0)
        sensor.vcc = result;
    else
        sensor.vcc =sensor.vcc -sensor.vcc /FIR_NUM+result/FIR_NUM;
    sensor.temperature = (1.43 - adc1_val_buf[4]*sensor.vcc/4096) * 1000 / 4.3 + 25;
//使用ph温度
    TDS_P(1);
    TDS_N(0);
    delay_us(7);// 开始TDS脉冲采样
    adc1_val_buf[1]=adc_channel_sample(ADC_CHANNEL_6);
    delay_us(7);
    adc1_val_buf[0]=adc_channel_sample(ADC_CHANNEL_14);
    result = adc1_val_buf[0];
    tt = result;
    //result  = result*vcc/4095;
    if(init == 0)
        averTds =  result;
    else
        averTds =averTds -averTds /FIR_NUM+result/FIR_NUM;
    // averTds =  result;
    //ADC_SoftwareStartConvCmd(ADC2,DISABLE);
    TDS_P(0);
    TDS_N(0);												// 电极倒极/
    delay_us(10);
    TDS_P(0);
    TDS_N(1);												// 电极倒极
    delay_us(7);
    TDS_P(0);
    TDS_N(0);												// 电极倒极/

   // result = filter(adc1_val_buf[1]);
    result = result*sensor.vcc/4095;
    if(init == 0)
        sensor.ele_curr = result;
    else
        sensor.ele_curr =sensor.ele_curr -sensor.ele_curr /FIR_NUM+result/FIR_NUM;
    // sensor.ele_curr = sensor.ele_curr/vcc;
    //电流= �?*x-2.5�?15=30x-37.5
    sensor.ele_curr = 30*sensor.ele_curr-37.5;


    if(sensor.ele_curr<0)
        sensor.ele_curr = 0;


    // 计算部分

    if(averTds>=TDS_Test_Vaule[0])
    {
        averTds2=TDS_Test_Vaule[0];
    }
    else if(averTds<=TDS_Test_Vaule[TDS_NUM-1])
        averTds2=TDS_Test_Vaule[TDS_NUM-1];
    else
        averTds2=averTds;
    for(j=0; j<TDS_NUM-1; j++)
    {
        if((averTds2>=TDS_Test_Vaule[j+1])&&(averTds2<TDS_Test_Vaule[j]))
        {
            break;
        }
    }
		
    if(j>=TDS_NUM)
        j=TDS_NUM-2;
    tds_tmp=TDS_Vaule[j]+(TDS_Vaule[j+1]-TDS_Vaule[j])*((float)(averTds2-TDS_Test_Vaule[j+1])/(float)(TDS_Test_Vaule[j]-TDS_Test_Vaule[j+1]));
    if(tds_tmp>=TDS_Vaule[TDS_NUM-1])
        tds_tmp = TDS_Vaule[TDS_NUM-1];
    if(tds_tmp<=0)
        tds_tmp = TDS_Vaule[0];

    // sensor.tds2 = 3030.9*(exp(-0.001*adc1_val_buf[0]));
     sensor.tds2 =sensor.tds2 -sensor.tds2 /FIR_NUM_MAX+tds_tmp/FIR_NUM_MAX;
    printf("       %d        %0.2f       %0.2f\n",adc1_val_buf[0],averTds,sensor.tds2);
    init = 1;
}

void GetFlow()
{

    static float flow_tmp;
    static unsigned init_flag;
    static uint32_t flow_tick;
    //流量 = (f+5)/24   19hz/s----1L/min
    if(*GetCapture() != -1)
    {
        if(init_flag == 0)
        {
            flow_tmp = *GetCapture();
            init_flag = 1;
        }

        else
        {
            flow_tmp = flow_tmp  - flow_tmp/FIR_NUM+*GetCapture()/FIR_NUM;
            sensor.flow = (flow_tmp+5)/24.0f;
            *GetCapture() = -1;
        }
        flow_tick = HAL_GetTick();
    }
    else//flow is zero
    {
        if((HAL_GetTick()-flow_tick)>=2000)
        {
            flow_tmp = flow_tmp  - flow_tmp/2;
            if(flow_tmp <= 0)
                sensor.flow = 0;
            else
                sensor.flow = (flow_tmp+5)/24.0f;
        }
    }

}

sensor_stru *GetSensor()
{
    return &sensor;
}
modbus_pack_stru modbus_pack_usr;

modbus_pack_stru *GetModbusPack()
{
    return  &modbus_pack_usr;
}
void GetModbusSens(unsigned char addr,unsigned char func,unsigned int reg,unsigned int regCount,unsigned char *buf,unsigned char datcount)
{
//    static uint32_t timeout;

    //  if((HAL_GetTick()-timeout)>= TX_PERIOD)
    {
        modbus_pack_usr.RS485_Addr = addr;
        modbus_pack_usr.func = func;
        modbus_pack_usr.startaddr = reg;
        modbus_pack_usr.regnum = regCount;
        modbus_pack_usr.datcount = datcount;
        if(modbus_pack_usr.startaddr == 0x07||modbus_pack_usr.startaddr == 0x08||modbus_pack_usr.startaddr == 0x16||
                modbus_pack_usr.startaddr == 0x19||modbus_pack_usr.startaddr == 0x1a||modbus_pack_usr.startaddr == 0x1b||
                modbus_pack_usr.startaddr == 0x34||modbus_pack_usr.startaddr == 0x36||modbus_pack_usr.startaddr == 0x38||
                modbus_pack_usr.startaddr == 0x3e||modbus_pack_usr.startaddr == 0x3f||modbus_pack_usr.startaddr == 0x66||
                modbus_pack_usr.startaddr == 0x40||modbus_pack_usr.startaddr == 0x41||modbus_pack_usr.startaddr == 0x42||
                modbus_pack_usr.startaddr == 0x43||modbus_pack_usr.startaddr == 0x44)
            modbus_pack_usr.datType =UINT_TYPE;
        else
            modbus_pack_usr.datType = FLOAT_TYPE;


        if(func == 0x10||func == 0x06)
            memcpy(modbus_pack_usr.modbus_txdata,buf,datcount);
        Modbus_Pack( modbus_pack_usr);
//        timeout = HAL_GetTick();
    }
}
void GetPH_ORP()
{
    static unsigned char addr_tmp=3,state=0;
    registerTick(SENSOR_TICK_NO,1000,1,0);
    if(GetTickResult(SENSOR_TICK_NO)==1)
    {
        switch(state)
        {
        case 0:
            GetModbusSens(ORP_ADDR,FUNC_READ,0x03,0x0002,0,0);//read temperature from orp sensor
            state++;
            break;

        case 1:
            GetModbusSens(ORP_ADDR,FUNC_READ,0x01,0x0002,0,0);
            state++;
            break;//读orp
        case 2:
            GetModbusSens(PH_ADDR,FUNC_READ,0x01,0x0002,0,0);
            state++;
            break;//read ph
        case 3:
					  if(GetSensor()->ele_MOnLine[addr_tmp-3]!=1)
							GetModbusSens(addr_tmp++,FUNC_READ,0x0042,0x0002,0,0);
						else
							addr_tmp++;//GetModbusSens(++addr_tmp,FUNC_READ,0x0042,0x0002,0,0);
            break;//read status on line
        case 4://电解
            if(GetSensor()->status[5]==0&&GetSensor()->status[6]==0)//高压开关和水位正常时启动电�?
            {
                unsigned char tx_buf[2];
                tx_buf[0] = 0;
                tx_buf[1] = 1;
                while(addr_tmp<M4_ADDR)
                {
                    GetModbusSens(addr_tmp++,FUNC_WRITE,0x0040,0x0002,tx_buf,2);
                    delay_ms(100);
                }
                state = 5;
                addr_tmp = M1_ADDR;
            }
            else
            {
                unsigned char tx_buf[2];
                tx_buf[0] = 0;
                tx_buf[1] = 0;
                while(addr_tmp<M4_ADDR)
                {
                    GetModbusSens(addr_tmp++,FUNC_WRITE,0x0040,0x0002,tx_buf,2);
                    delay_ms(100);
                }
                state = 5;
                addr_tmp = M1_ADDR;

            }
            break;//
        case 5:
            GetModbusSens(addr_tmp++,FUNC_READ,0x0043,0x0002,0,0);
            break;//read current
            // case 4:GetModbusSens(ORP_ADDR,FUNC_READ,0x01,0x0002,0,0);break;
        }
        if(addr_tmp>M4_ADDR)
        {
            if(state==3)
            {

                state = 4;//ele start
                addr_tmp = M1_ADDR;
            }

        }

        if(state==5)
        {
            if(addr_tmp>M4_ADDR)
            {
                addr_tmp = M1_ADDR;
                state = 0;//restart state machine
            }

        }
        registerTick(SENSOR_TICK_NO,0,0,1);
        delay_ms(100);
    }

}
void calibration_sensors(unsigned char state)
{

    static modbus_pack_stru modbus_cali;
    unsigned char buf[16];
    static unsigned char status;
    unsigned char *p;
    if(status == 0||state == 3||state ==6||state ==9)
        status = state;
    registerTick(CALIB_TICK,500,1,0);
    if(GetTickResult(CALIB_TICK)==1)
    {
        if(modbus_pack_usr.ack == 0)
        {
            switch(status)
            {
            case 1://6.86 ph calibration init
            {
                modbus_cali.RS485_Addr = PH_ADDR;
                modbus_cali.func = 0x06;
                modbus_cali.startaddr = 0x0036;
                modbus_cali.regnum = 0;
                buf[0] = 0x00;
                buf[1] = 0x01;
                memcpy(modbus_cali.modbus_txdata,buf,2);
                Modbus_Pack( modbus_cali);
                status = 2;
            }
            break;
            case 2://6.86 ph calibration start
            {
                modbus_cali.RS485_Addr = PH_ADDR;
                modbus_cali.func = 0x03;
                modbus_cali.startaddr = 0x0066;
                modbus_cali.regnum = 0;
                buf[0] = 0x00;
                buf[1] = 0x01;
                memcpy(modbus_cali.modbus_txdata,buf,2);
                Modbus_Pack( modbus_cali);
                status = 2;
            }
            break;
            case 3://6.86 ph calibration done
            {
                modbus_cali.RS485_Addr = PH_ADDR;
                modbus_cali.func = 0x06;
                modbus_cali.startaddr = 0x003e;
                modbus_cali.regnum = 0;
                buf[0] = 0x00;
                buf[1] = 0xff;
                memcpy(modbus_cali.modbus_txdata,buf,2);
                Modbus_Pack( modbus_cali);
                status = 00;
                // GetInOut()->key_cali_mode = 0;//6.86ph done
                GetInOut()->key_cali_value = 0;
            }
            break;

            case 4://4.01 ph calibration init
            {
                modbus_cali.RS485_Addr = PH_ADDR;
                modbus_cali.func = 0x06;
                modbus_cali.startaddr = 0x0038;
                modbus_cali.regnum = 0;
                buf[0] = 0x00;
                buf[1] = 0x01;
                memcpy(modbus_cali.modbus_txdata,buf,2);
                Modbus_Pack( modbus_cali);
                status = 5;
            }
            break;
            case 5://4.01 ph calibration start
            {
                modbus_cali.RS485_Addr = PH_ADDR;
                modbus_cali.func = 0x03;
                modbus_cali.startaddr = 0x0066;
                modbus_cali.regnum = 0;
                buf[0] = 0x00;
                buf[1] = 0x01;
                memcpy(modbus_cali.modbus_txdata,buf,2);
                Modbus_Pack( modbus_cali);
                status = 5;
            }
            break;
            case 6://4.01 ph calibration done
            {
                modbus_cali.RS485_Addr = PH_ADDR;
                modbus_cali.func = 0x06;
                modbus_cali.startaddr = 0x003f;
                modbus_cali.regnum = 0;
                buf[0] = 0x00;
                buf[1] = 0xff;
                memcpy(modbus_cali.modbus_txdata,buf,2);
                Modbus_Pack( modbus_cali);
                status = 0;
                GetInOut()->key_cali_value = 0;
            }
            break;
            case 7://orp
            {
                modbus_cali.RS485_Addr = ORP_ADDR;
                modbus_cali.func = 0x10;
                modbus_cali.startaddr = 0x0030;
                modbus_cali.regnum = 0x0002;
                p = (unsigned char *)(&(GetInOut()->orb_cali_value));
                buf[0] = 4;
                buf[1] = *(p+3);
                buf[2] =  *(p+2);
                buf[3] =  *(p);
                buf[4] =  *(p+1);
                memcpy(modbus_cali.modbus_txdata,buf,5);
                Modbus_Pack( modbus_cali);
                status = 8;
            }
            break;

            case 8://
            {
                modbus_cali.RS485_Addr = ORP_ADDR;
                modbus_cali.func = 0x03;
                modbus_cali.startaddr = 0x0066;
                modbus_cali.regnum = 0;
                buf[0] = 0x00;
                buf[1] = 0x01;
                memcpy(modbus_cali.modbus_txdata,buf,2);
                Modbus_Pack( modbus_cali);
                status = 8;
            }
            break;
            case 9://4.01 ph calibration done
            {
                modbus_cali.RS485_Addr = ORP_ADDR;
                modbus_cali.func = 0x06;
                modbus_cali.startaddr = 0x003f;
                modbus_cali.regnum = 0;
                buf[0] = 0x00;
                buf[1] = 0xff;
                memcpy(modbus_cali.modbus_txdata,buf,2);
                Modbus_Pack( modbus_cali);
                status = 0;
                GetInOut()->key_cali_value = 0;
            }
            break;


            }

        }
        registerTick(CALIB_TICK,0,0,1);

    }


}
void module_anlysis(unsigned char addr)
{
    unsigned char addr_tmp[4];
unsigned int curr_tmp;
    switch(addr)
    {
    case ORP_ADDR:
    {
        if(modbus_pack_usr.startaddr  == 0x0001 )//read orp
        {
            memcpy(addr_tmp,modbus_pack_usr.modbus_data,4);
            sensor.orp = *((float*)addr_tmp);
        }

        if(modbus_pack_usr.startaddr  == 0x0003 )//temperature
        {
            memcpy(addr_tmp,modbus_pack_usr.modbus_data,4);
            sensor.temperature = *((float*)addr_tmp);
        }

        if(modbus_pack_usr.startaddr  == 0x0007 )//warn
        {
            memcpy(addr_tmp,modbus_pack_usr.modbus_data,2);
            sensor.warn = *(unsigned int *)addr_tmp;
        }

        if(modbus_pack_usr.startaddr  == 0x0066 )//adc
        {
            memcpy(addr_tmp,modbus_pack_usr.modbus_data,4);
            sensor.orp_ph_adc = *(float *)addr_tmp;
        }
        if(modbus_pack_usr.startaddr  == 0x0043 )//module addr
        {
            memcpy(addr_tmp,modbus_pack_usr.modbus_data,4);
            sensor.orp_ph_adc = *(float *)addr_tmp;
        }

    }
    break;  //18057165928

    case PH_ADDR:
    {
        if(modbus_pack_usr.startaddr  == 0x01)
        {
            memcpy(addr_tmp,modbus_pack_usr.modbus_data,4);
            sensor.ph = *(float *)addr_tmp;
        }
        if(modbus_pack_usr.startaddr  == 0x0003 )//temperature
        {
            memcpy(addr_tmp,modbus_pack_usr.modbus_data,4);
            sensor.temperature = *(float *)addr_tmp;
        }
        if(modbus_pack_usr.startaddr  == 0x0007 )//warn
        {
            memcpy(addr_tmp,modbus_pack_usr.modbus_data,2);
            sensor.warn = *(unsigned int *)addr_tmp;
        }
        if(modbus_pack_usr.startaddr  == 0x0066 )//adc
        {
            memcpy(addr_tmp,modbus_pack_usr.modbus_data,4);
            sensor.orp_ph_adc = *(float *)addr_tmp;
        }


    }
    break;
    case M1_ADDR:
    case M2_ADDR:
    case M3_ADDR:
    case M4_ADDR:
    {
        if(modbus_pack_usr.startaddr  == 0x40)//ele status
        {
            memcpy(addr_tmp,modbus_pack_usr.modbus_data,2);
            sensor.ele_EleStatus[addr-3] = addr_tmp[1];
        }
        if(modbus_pack_usr.startaddr  == 0x0042 )//module on line
        {
            memcpy(addr_tmp,modbus_pack_usr.modbus_data,2);
            sensor.ele_MOnLine[addr-3]= 1;
        }
        if(modbus_pack_usr.startaddr  == 0x0043 )//ele current
        {
            memcpy(addr_tmp,modbus_pack_usr.modbus_data,2);
					  
					  curr_tmp = addr_tmp[0]<<8;
					curr_tmp = curr_tmp+addr_tmp[1];
            sensor.ele_Mcurr[addr-3] = ((curr_tmp)/100.0);
        }
    }
    break;

    }


}
void orp_ph_process(void)
{
//    unsigned char addr_tmp[4];
    unsigned char result;
    if(GetInOut()->key_cali_mode == 0)//
        GetPH_ORP();//2000ms调一�?
    else if(GetInOut()->key_cali_mode == 1)//calibration mode
    {
        static unsigned char tx_buf[2],addr_tmp = M1_ADDR,tx_count;
        tx_buf[0] = 0;
        tx_buf[1] = 0;
        if(tx_count<3)
        {
            GetModbusSens(addr_tmp++,FUNC_WRITE,0x0040,0x0002,tx_buf,2);
            if(addr_tmp>M4_ADDR)
                addr_tmp = M1_ADDR;
            tx_count ++;
        }



        switch(GetInOut()->key_cali_value )
        {
        case 1://6.86ph start
            calibration_sensors(1);
            break;
        case 10://6.86ph done
        {
            calibration_sensors(3);


        }
        break;
        case 4://4.01ph start
            calibration_sensors(4);
            break;
        case 11://4.01ph done
        {
            calibration_sensors(6);
            // GetInOut()->key_cali_mode = 0;
            // GetInOut()->key_cali_value = 0;

        }
        break;
        //4.01ph done
        default:
            calibration_sensors(0);
            break;

        }

    }
    else if(GetInOut()->key_cali_mode == 2)//orp calibration mode
    {
        switch(GetInOut()->key_cali_value )
        {
        case 1://orp start
            calibration_sensors(7);
            break;
        case 2://orp done
        {
            calibration_sensors(9);
            //GetInOut()->key_cali_mode = 0;
            // GetInOut()->key_cali_value = 0;
        }
        break;
        default:
            calibration_sensors(0);
            break;

        }
        registerTick(SENSOR_TICK_NO,0,0,1);
    }
    result = RS485_Service();
    if(result == 0||result == 2)
    {
        //  modbus_pack_usr.RS485_Addr;// =getModbusPayload()->RS485_Addr;
        module_anlysis(modbus_pack_usr.RS485_Addr);
    }

}
void sensor_process()
{


    GetTds_EleCurr();
    GetFlow();
    orp_ph_process();

    GetWaterLevel();
}
