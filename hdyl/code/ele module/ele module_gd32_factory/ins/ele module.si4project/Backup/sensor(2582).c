#include "sensor.h"
#include "modbus.h"
#include "sys.h"
#include "bsp_cpu_flash.h"
#include "pid.h"

#if CPU == ST
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;
#endif
sensor_stru sensor;
uint32_t adc1_val_buf[2];

void SystemParamsSave()
{
    uint32_t addr,len;
    unsigned char tmp;
    __disable_irq();
    addr = 0;
    addr = addr;
    FlashEease(addr);
    addr = 0;
    addr = addr;
    tmp = 0x5a; //
    bsp_WriteCpuFlash(addr, &tmp, 1);
    addr = 1;

    bsp_WriteCpuFlash(addr, &GetModbusPayLoad()->RS485_Addr, 1);
    addr = addr + len;
    __enable_irq() ;
}
void SystemParamsRead()
{
    uint32_t addr;
    unsigned char tmp;
    unsigned int i;
    addr = 0;
    //addr = addr+FLASH_BASE_ADDR;
    bsp_ReadCpuFlash(addr, &tmp, 1);
    if(tmp == 0x5a||tmp == 0x5b)
    {

        bsp_ReadCpuFlash(addr+i,&GetModbusPack()->RS485_Addr, 1);
    }
    else
    {
        SystemParamsSave();
    }


}


void adcInit()
{

#if CPU == ST
    HAL_ADC_Start_DMA(&hadc1,(uint32_t*) adc1_val_buf, 2);
#endif

}
uint32_t *get_adc_buf()
{
    return adc1_val_buf;
}

float vcc;
void GetEle_EleCurr()
{

    static float curr_v,curr_I,curr_1000_I;
    float result;
    static unsigned char init;

    registerTick(CURR_TICK,10,1,0);

    if(GetTickResult(CURR_TICK)==1)
    {
        adc_software_trigger_enable(ADC0, ADC_INSERTED_CHANNEL);
        /* delay a time in milliseconds */
        delay_ms(10);

        vcc = ADC_IDATA1(ADC0);
        vcc = ( 4095 *1.2/vcc);
        //vcc = 1.2*4095;
        vcc = 3.3;


        result = adc1_val_buf[0];
        result = result*vcc;
        result = result/4095;

        if(init == 0)
            curr_v = result;
        else
            curr_v =curr_v -curr_v /FIR_NUM+result/FIR_NUM;
        init = 1;
        // sensor.ele_curr = sensor.ele_curr/vcc;
        //电流= （2*x-2.5）*15=30x-37.5
        curr_I = 30*curr_v-37.5;
        //curr_I = 1000*curr_I;
        // sensor.ele_curr = sensor.ele_curr- sensor.ele_curr /FIR_NUM+curr_I/FIR_NUM;
        curr_1000_I =  curr_1000_I- curr_1000_I /FIR_NUM+curr_I/FIR_NUM;
        sensor.ele_curr =  curr_1000_I*1000;
        if(sensor.ele_curr<400)
            sensor.ele_curr = 0;
        registerTick(CURR_TICK,0,0,1);
        //sensor.ele_curr = 2324;
    }


}

sensor_stru *GetSensor()
{
    return &sensor;
}
void GetModbusSens(unsigned char len,unsigned char func,unsigned int reg,unsigned int regCount,unsigned char *buf,unsigned char datcount)
{
    static uint32_t timeout;


    if((HAL_GetTick()-timeout)>= TX_PERIOD)
    {
        GetModbusPack()->RS485_Addr = GetModbusPayLoad()->RS485_Addr;
        GetModbusPack()->payload_len = 2;
        GetModbusPack()->func = func;
        GetModbusPack()->startaddr = reg;
        //GetModbusPack()->regnum = regCount;
        //   if(func == 0x10)
        memcpy(GetModbusPack()->modbus_txdata,buf,datcount);
        Modbus_Pack( GetModbusPack());
        timeout = HAL_GetTick();
    }
}
void Soft_Reset(void)
{
    __set_FAULTMASK(1);		  //关闭所有中断
    NVIC_SystemReset();			//复位系统
}
void analysis_process()
{
    unsigned int tmp;
    unsigned char curr_tmp[2];
    unsigned char result;
    result = RS485_Service();
    if( result== 2||result== 3)
    {

        if( GetModbusPack()->func == FUNC_READ)//读命令响应
        {


            if( GetModbusPack()->startaddr  == 0x0040 )//ele status
            {
                GetModbusSens(0x02,FUNC_READ,0x0040,0,(unsigned char *)(&sensor.ele_status),2);

                //sensor.ele_status = 1;
            }


            if(GetModbusPack()->startaddr  == 0x0041 )//addr
            {
                unsigned char tmp[2];
                tmp[0] =0;
                tmp[1] = GetModbusPayLoad()->RS485_Addr;
                GetModbusSens(0x02,FUNC_READ,0x0041,0,tmp,2);
            }

            if( GetModbusPack()->startaddr	== 0x0042 )//status
            {
                GetModbusSens(0x02,FUNC_READ,0x0042,0,(unsigned char *)(&sensor.wash),2);
                sensor.online = 1;
            }

            if(GetModbusPack()->startaddr  == 0x0043 )//current
            {
                tmp = (unsigned int)(sensor.ele_curr/10);
                curr_tmp[0]=tmp>>8;
                curr_tmp[1]=tmp;
                GetModbusSens(0x02,FUNC_READ,0x0043,0,(unsigned char *)(&curr_tmp),2);
            }


            //if(GetModbusPack()->startaddr  == 0x0044 )//addr
            //   GetModbusSens(0x02,FUNC_READ,0x0044,0,&GetModbusPack()->RS485_Addr,1);

        }
        else if(GetModbusPack()->func == FUNC_WRITE)//写命令响应
        {
            if( GetModbusPack()->startaddr  == 0x0040 )
                sensor.ele_status = GetModbusPayLoad()->RS485_RX_BUFF[5];
            if( GetModbusPack()->startaddr  == 0x0045 )
                sensor.inverEle = GetModbusPayLoad()->RS485_RX_BUFF[5];
            if(GetModbusPack()->startaddr  == 0x0041 )//addr
            {
                sensor.addr = GetModbusPayLoad()->RS485_RX_BUFF[5];
                GetModbusPack()->RS485_Addr = sensor.addr;
                GetModbusPayLoad()->RS485_Addr = sensor.addr;
                SystemParamsSave();
                sensor.online = 2;
            }

            if(GetModbusPack()->startaddr  == 0x0044 )//reset
            {
                sensor.reset = GetModbusPayLoad()->RS485_RX_BUFF[5];
                Soft_Reset();
            }

        }

    }

}
#if CPU == ST
extern TIM_HandleTypeDef htim3;
#endif
/***************************************************************
                  功能：控制电机
**************************************************************/

void pwmCtrl()
{

    static int32_t pwm1;
    static unsigned int dstCurr = 13000;
    pwm1 = dstCurr/100 - sensor.ele_curr/100;

    pwm1 =	pwm1*500;

    if(pwm1>=65535)
        pwm1 = 65535;
    if(pwm1<0)
        pwm1 = 0;


    if(sensor.ele_curr >= MAX_CURR_VALUE)
    {
        pwm1 =	100;
    }
    else  if(sensor.ele_curr <= MIN_CURR_VALUE)
    {
        pwm1 =	65535;


    }
    else
    {

        ;//DcMotorCtrl(0,pwm1);//泵1调整流量
        //DcMotorCtrl(1,pwm2);//泵2调整流量

    }


#if CPU == ST
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, pwm1);
#endif

}

void RelayCtrl(unsigned char dir)
{

#if CPU == ST
    if(dir == FORWARD)
    {
        HAL_GPIO_WritePin(Ele_ConA_GPIO_Port, Ele_ConA_Pin, GPIO_PIN_SET);
        HAL_Delay(20);
        HAL_GPIO_WritePin(Ele_ConB_GPIO_Port, Ele_ConB_Pin, GPIO_PIN_RESET);

    }
    else if(dir == BACKWARD)
    {
        HAL_GPIO_WritePin(Ele_ConA_GPIO_Port, Ele_ConA_Pin, GPIO_PIN_RESET);
        HAL_Delay(20);
        HAL_GPIO_WritePin(Ele_ConB_GPIO_Port, Ele_ConB_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(Ele_ConA_GPIO_Port, Ele_ConA_Pin, GPIO_PIN_RESET);
        HAL_Delay(20);
        HAL_GPIO_WritePin(Ele_ConB_GPIO_Port, Ele_ConB_Pin, GPIO_PIN_RESET);
    }
#else
    if(dir == FORWARD)
    {
        gpio_bit_write(Ele_ConA_GPIO_Port, Ele_ConA_Pin, SET);
        delay_ms(20);
        gpio_bit_write(Ele_ConB_GPIO_Port, Ele_ConB_Pin, RESET);

    }
    else if(dir == BACKWARD)
    {
        gpio_bit_write(Ele_ConA_GPIO_Port, Ele_ConA_Pin, RESET);
        delay_ms(20);
        gpio_bit_write(Ele_ConB_GPIO_Port, Ele_ConB_Pin, SET);
    }
    else
    {
        gpio_bit_write(Ele_ConA_GPIO_Port, Ele_ConA_Pin, RESET);
        delay_ms(20);
        gpio_bit_write(Ele_ConB_GPIO_Port, Ele_ConB_Pin, RESET);
    }

#endif
}

void ele_ctrl(unsigned char mode)
{

    GetEle_EleCurr();
    if(mode == ON)
    {
        // pwmCtrl();
        //pid_proc(sensor.ele_curr);
#if CPU == ST
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, pid_proc(sensor.ele_curr));
        RelayCtrl(FORWARD);
#else
        if(sensor.inverEle == 0)
            RelayCtrl(FORWARD);
        else if (sensor.inverEle == 1)
            RelayCtrl(BACKWARD);
        timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,pid_proc(sensor.ele_curr));

#endif
    }
    else
    {
#if CPU == ST
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 0);
        HAL_Delay(10);
        RelayCtrl(3);
#else
        RelayCtrl(3);
        timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,0);
#endif
    }

}
void params_Init()
{
    RS485_Init();
    SystemParamsRead();

    pid_init(DES_CURR_VALUE);


}
void work_process()
{
    static uint32_t timeout_tick;
    if(sensor.ele_status == 1)//电解模式
    {
        registerTick(LOWCURR_TICK,60000,1,0);
        if(GetTickResult(LOWCURR_TICK)==1)
        {
            if(sensor.ele_curr <= MIN_CURR_VALUE||sensor.ele_curr >= MAX_CURR_VALUE)//电流 异常保护
            {
                ele_ctrl(OFF);
                pid_init(DES_CURR_VALUE);
                sensor.ele_status =0;

            }
            else if(sensor.ele_curr <= MIN_CURR_VALUE)//倒极
            {

            }
            else
            {
                if(sensor.inverEle == 1)
                {
                    registerTick(WASH_TICK_NO,WASH_TIME_SETTING,1,0);
                    if(GetTickResult(WASH_TICK_NO)==1)
                    {
                        sensor.wash_time =  sensor.wash_time + 1;
                        registerTick(WASH_TICK_NO,0,0,1);
                    }
                }
                else
                {
                    if(sensor.wash_time >0)
                        sensor.wash_time =  sensor.wash_time - 1;
                    registerTick(WASH_TICK_NO,0,0,1);
                }
                ele_ctrl(ON);
                registerTick(LOWCURR_TICK,0,0,1);
            }
        }
    }
    else if(sensor.ele_status == 0||sensor.wash ==1||sensor.reset== 1)//非电解模式
    {
        ele_ctrl(OFF);
        if(sensor.reset==1)
        {

            /* set FAULTMASK */
            __set_FAULTMASK(1);
            NVIC_SystemReset();
        }
        timeout_tick = HAL_GetTick();

    }
}
void sensor_process()
{

    analysis_process();
    work_process();
}
