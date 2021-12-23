#include "ble.h"
#include "bsp.h"

void BleModeSetting(unsigned char mode)
{
    switch(mode)
    {
    case CONFIG_MODE:
        gpio_bit_write(BMOD_GPIO_Port, BMOD_Pin, RESET);

        break;
    case TRANSMIMT_MODE:
        gpio_bit_write(BMOD_GPIO_Port, BMOD_Pin, SET);
        break;
	default: ;break;

    }
}
unsigned char BleConSeting(unsigned char mode)
{
    unsigned char result;
    result = NULL;
    switch(mode)
    {
    case DIS_CONNECT:
    {
        gpio_bit_write(BDISC_GPIO_Port, BDISC_Pin, SET);//透传模式
        delay_ms(200);
        gpio_bit_write(BDISC_GPIO_Port, BDISC_Pin, RESET);//配置模式
        //gpio_bit_write(BDISC_GPIO_Port, BDISC_Pin, RESET);
        result = NULL;;


    }
    break;
    case CONNECT_STA:
    {
        if(gpio_input_bit_get(BLINK_GPIO_Port, BLINK_Pin))
        {
            result =  1;
        }
        else
            result = 0;//有蓝牙连接
    }
    break;
    case DATA_STA:
    {
        if(gpio_input_bit_get(BDATA_GPIO_Port, BDATA_Pin))
        {
            result =  1;
        }
        else
            result =  0; //有数据在正在发送
    }
    break;
    case WAKUP:
    {   
        gpio_bit_write(BWKP_GPIO_Port, BWKP_Pin, SET);//
        delay_ms(200);
        gpio_bit_write(BWKP_GPIO_Port, BWKP_Pin, RESET);//
        result =  0; //有数据在正在发送
    }
    break;
    case SLEEP:
    {   
        gpio_bit_write(BWKP_GPIO_Port, BWKP_Pin, RESET);//
        delay_ms(200);
        gpio_bit_write(BWKP_GPIO_Port, BWKP_Pin, SET);//
        result =  0; //有数据在正在发送
    }

	default: result = NULL;break;

    }
    return result;
}
unsigned int sendCommand(char *Command, char *Response, uint32_t Timeout, unsigned char Retry)
{
    unsigned char *USARTX_RX_BUF;


    static unsigned char RetryCount;
    static uint32_t timeout;

    uint32_t tmp;
    tmp = HAL_GetTick()-timeout;
    if((tmp)>=Timeout)
    {
        if(RetryCount<=Retry)
        {
            if(Retry>0)
            {
                SendStr(Command);
                RetryCount = RetryCount +1;
                timeout = HAL_GetTick();
            }
        }
        else
        {
            RetryCount = 0;
        }

        if (strstr(USARTX_RX_BUF, Response) != NULL)
        {
            RetryCount = 0;
            return Success;
        }
        else
        {
            return Failure;
        }

    }
    else
    {
        if (strstr(USARTX_RX_BUF, Response) != NULL)
        {
            RetryCount = 0;
            return Success;
        }
        else
        {
            return Failure;
        }

    }

}

