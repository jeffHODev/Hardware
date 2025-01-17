#include "adc.h"
#include "ntc.h"
#include "math.h"
extern ADC_HandleTypeDef hadc1;
//存放6个adc通道的数据
uint16_t ADC_DataBuf[10][2]= {0};



//ADC模式初始化
void ADCx_Mode_Init(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_DataBuf, 20);

}


//二分法查找NTC阻值对应的温度
//传入参数value:此时NTC的阻值 单位kΩ
//返回值：对应温度数组的下标，也即此时的温度
static uint8_t Algorithm_dichotomy(float value)
{
    uint8_t low=0,high=100,middle=0;

    if(value>=NTC[0])		//边界处理
        return 0;
    if(value<=NTC[100])
        return 100;

    while(low<=high)
    {
        middle=low+(high-low)/2;		//二分中点=数组左边界+(右边界-左边界)/2
        if(value>NTC[middle])				//值在中点左边
        {
            high=middle-1;
        }
        else if(value<NTC[middle+1])	//值在中点右边 +1是因为value不一定等于数组值 在NTC[middle]~NTC[middle+1]的值均为NTC[middle]
        {
            low=middle+1;
        }
        else					//中点值在即查找到值 value值在NTC[middle]~NTC[middle+1]范围内
        {
            return middle;
        }
    }
    return 0;
}

//获取温度值
//返回对应的温度值
uint8_t Get_Temperature(void)
{
    float ADC_Temp=0,volt_temp;
    float NTC_value=0;
    uint8_t i=0;

    for(i=0; i<10; i++)					//10次ADC值求和
    {
        ADC_Temp+=ADC_DataBuf[i][0];
    }
    ADC_Temp=ADC_Temp/10;			//取平均值滤波处理
//	printf("ADC_Temp=%f ADC_refer=%f\r\n",ADC_DataBuf[0][0]*3.3/4096,ADC_DataBuf[0][1]*3.3/4096);
    volt_temp=ADC_Temp*3.0/4096;							//计算NTC电阻上的电压值
    NTC_value=10*volt_temp/(3.0-volt_temp);		//计算NTC电阻值 单位：kΩ

    return (Algorithm_dichotomy(NTC_value));
}

//获取档位百分比 0.0-1.0
float Get_Precent(void)
{
    uint8_t i=0;
    float ADC_gear=0;
    for(i=0; i<10; i++)
    {
        ADC_gear+=ADC_DataBuf[i][1];
    }
    ADC_gear/=10;		//取平均值

    if(ADC_gear<200)	//为了防止抖动，当电压小于0.16v时认为关闭
        ADC_gear=0;

//	printf("ADC=%f\r\n",ADC_gear*3.0/4096);
    return (float)(ADC_gear/4096);	//返回档位数值 百分比
}
extern Params_stru Params;






unsigned char adc_process(unsigned char flag)
{
    static unsigned char result;
    if(flag == 1)
    {
        result = result&0xfe;
    }
    if(flag == 2)
    {
        result = result&(0xfd);
    }
    if(flag == 0)
    {
        Params.temperature=Get_Temperature();	  //获取温度数值
        if((HAL_GetTick()-Params.temperature_period)>=T_PERIOD)
        {
            Params.temperature_period = HAL_GetTick();

            result = result |0x01;

        }

        if((HAL_GetTick()-Params.gear_period)>=G_PERIOD)
        {
            //档位调节与显示
            float tmp;
            Params.precent=Get_Precent();		  //获取调节旋钮调节的百分比值
            tmp=10*Params.precent+0.5;
            Params.gear=round(tmp)-1;
            if(Params.gear<0)
                Params.gear=0;
            if(Params.gear>9)
                Params.gear=9;
            Params.gear_period = HAL_GetTick();
            result = result|0x02;


        }
    }


    return result;
}





