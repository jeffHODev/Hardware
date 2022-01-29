#include "ble_set.h"
#include "bsp.h"
#include "sys.h"
#include "protocol.h"

unsigned char ble_res_flag;
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
    default:
        ;
        break;

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
    break;

    default:
        result = NULL;
        break;

    }
    return result;
}uint32_t tmp;
unsigned int sendCommand(char *Command, char *Response, uint32_t Timeout, unsigned char Retry)
{
    unsigned char *USARTX_RX_BUF;
    

    static unsigned char RetryCount;
    static uint32_t timeout;


    USARTX_RX_BUF = getBleUartBuf();
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
				if(ble_res_flag == 1)
				{
					ble_res_flag = 0;
             if (strstr(USARTX_RX_BUF, Response) != NULL)
        {
            RetryCount = 0;
					ClrBleUartBuf();
            return Success;
        }
        else
        {
					  ClrBleUartBuf();
            return Failure;
        }				
				}

 

    }
    else
    {
        if (strstr(USARTX_RX_BUF, Response) != NULL)
        {
            RetryCount = 0;
					ClrBleUartBuf();
            return Success;
        }
        else
        {
            return 2;
        }

    }

}
//while(sendCommand("AT+BAUD?", "460800",100000, 3)== Failure)//波特率)
//{
//    usart_baudrate_set(((UART3)), 115200);
//    sendCommand("AT+BAUD = 460800", "+OK",100000, 1);//波特率
//    sendCommand("AT+LINKMAST = 1", "+OK",100000, 1);//连接1个主设备
//    //sendCommand("AT+DEVMANUF = HDYL", "+OK",100, 3);;//设备厂
//}

void ble_proc(void)
{
    static unsigned char step;
    while(step!=3)
		{
    switch(step)
    {
    case 0:
    {
        // usart_baudrate_set(((UART3)), 115200);
		  	//if(sendCommand("AT", "OK",1000, 3)== Failure)
        if(sendCommand("AT+BAUD?", "+OK=13",5000, 3)== Failure)
        {
            usart_baudrate_set(((UART3)), 115200);
            step = 1;

        }
				else
					step = 2;
      }break;
    case 1:
    {
        if(sendCommand("AT+BAUD=13", "+OK",1000, 1)== Success)
            step = 2;
    }break;
    case 2:
    {
       // if(sendCommand("AT+LINKMAST=1", "+OK",1000, 1)== Success)
            step = 3;
    }break;

    }		
		}

}
