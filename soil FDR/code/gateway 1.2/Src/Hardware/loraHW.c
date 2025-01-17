#include "loraHW.h"
#include "loraHAL.h"
#include "gateway.h"
#include "register.h"
#include "EEPROM.h"
#include "config.h"
#include "rtc.h"
extern RTC_HandleTypeDef hrtc;
extern loraUart_stru loraUart;
extern LORAHW_stru lorahw;;
extern uint32_t last_addr;
extern ADC_HandleTypeDef hadc1;
extern LORA_Params_stru loraParams;
extern UART_HandleTypeDef huart1,huart3,huart2,huart5,huart6;
extern DMA_HandleTypeDef hdma_usart2_rx;
void ParamsSave(unsigned char datasrc)//参数保存
{
    unsigned char p;
    REG_val_stru *q;
    unsigned char tmp;
    uint32_t addr;
    data_in_flash_stru *dataInFlash;
    if(datasrc == 0)
    {
        p = 0x5a;  //写入标志
        // register_init();//寄存器初始化
        q =  getRegAddr();

        flash_init(0);
        addr = 0;
        flash_write(addr++,&p,1);//写入数据头
        flash_write(addr++,&(q->bindCount),1);//写入绑定设备数量

        dataInFlash = getDataFrameAddr();
        tmp = (u8)(dataInFlash->lastRead_addr>>8);//数据帧上次读地址
        flash_write(addr++,&(tmp),1);
        tmp = (u8)(dataInFlash->lastRead_addr);
        flash_write(addr++,&(tmp),1);

        tmp = (u8)(dataInFlash->lastWrite_addr>>24);//数据帧上次读地址
        flash_write(addr++,&(tmp),1);
        tmp = (u8)(dataInFlash->lastWrite_addr>>16);
        flash_write(addr++,&(tmp),1);
        tmp = (u8)(dataInFlash->lastWrite_addr>>8);//数据帧上次读地址
        flash_write(addr++,&(tmp),1);
        tmp = (u8)(dataInFlash->lastWrite_addr);
        flash_write(addr++,&(tmp),1);

        flash_write(addr,q->value,REG_SIZE_U);//寄存器数值
        addr = addr +REG_SIZE_U;


        unsigned char *tmp;

        if(ROLE == GATEWAY)
        {

            tmp = ReadRegister(0x4006);//
            getConfig()->srcaddr[0] = tmp[0];
            getConfig()->srcaddr[1] = tmp[1];
            getConfig()->srcaddr[2] = tmp[2];
            getConfig()->srcaddr[3] = tmp[3];
            getConfig()->srcaddr[4] = tmp[4];
            getConfig()->srcaddr[5] = tmp[5];
            getConfig()->srcaddr[6] = tmp[6];
            getConfig()->srcaddr[7] = tmp[7];

            tmp = ReadRegister(0x44f1);//节点地址高2字节
            getConfig()->airrate = tmp[1];
            getConfig()->channel = tmp[3];

            tmp = ReadRegister(0x45f0);//
            getConfig()->TcpServer[0] = tmp[1];
            getConfig()->TcpServer[1] = tmp[3];
            getConfig()->TcpServer[2] = tmp[5];
            getConfig()->TcpServer[3] = tmp[7];
            getConfig()->TcpServerPort = ((u16)(tmp[8])<<8)+tmp[9];

            //WriteOneRegister(0x1006,57);//主动上报

        }
        else
        {
            tmp = ReadRegister(0xf007);//
            getConfig()->srcaddr[0] = tmp[0];
            getConfig()->srcaddr[1] = tmp[1];
            getConfig()->srcaddr[2] = tmp[2];
            getConfig()->srcaddr[3] = tmp[3];
            getConfig()->srcaddr[4] = tmp[4];
            getConfig()->srcaddr[5] = tmp[5];
            getConfig()->srcaddr[6] = tmp[6];
            getConfig()->srcaddr[7] = tmp[7];

            tmp = ReadRegister(0x1200);//
            getConfig()->destaddr[0] = tmp[0];
            getConfig()->destaddr[1] = tmp[1];
            getConfig()->destaddr[2] = tmp[2];
            getConfig()->destaddr[3] = tmp[3];
            getConfig()->destaddr[4] = tmp[4];
            getConfig()->destaddr[5] = tmp[5];
            getConfig()->destaddr[6] = tmp[6];
            getConfig()->destaddr[7] = tmp[7];

            tmp = ReadRegister(0x1205);//节点地址高2字节
            getConfig()->airrate = tmp[1];
            getConfig()->channel = tmp[3];

            tmp = ReadRegister(0x1207);//
            getConfig()->TcpServer[0] = tmp[1];
            getConfig()->TcpServer[1] = tmp[3];
            getConfig()->TcpServer[2] = tmp[5];
            getConfig()->TcpServer[3] = tmp[7];
            getConfig()->TcpServerPort = ((u16)(tmp[8])<<8)+tmp[9];

        }
        //free(tmp);


    }
    else
    {

    }
}
/**************************************************************
数据存储结构：
               数据头        绑定设备数量           寄存器数值
长度（字节）          1             1                2048
***************************************************************/
void ParamsRead(void)
{
    unsigned char p;
    unsigned char tmp;
    REG_val_stru *q;
    uint32_t addr;
    data_in_flash_stru *last;

    last = getDataFrameAddr();
    addr = 0;
    flash_read(addr++,&p,1);
    if(p!=0x5a)
    {

        p = 0x5a;  //写入标志
        configInit();
        register_init();//寄存器初始化
        q =  getRegAddr();

        flash_init(0);
        addr = 0;
        flash_write(addr++,&p,1);//写入数据头
        flash_write(addr++,&(q->bindCount),1);//写入绑定设备数量

        tmp = (u8)((last->lastRead_addr)>>8);//数据帧上次写地址
        flash_write(addr++,&(tmp),1);
        tmp = (u8)(last->lastRead_addr);
        flash_write(addr++,&(tmp),1);

        tmp = (u8)(last->lastWrite_addr>>24);//数据帧上次读地址
        flash_write(addr++,&(tmp),1);
        tmp = (u8)(last->lastWrite_addr>>16);
        flash_write(addr++,&(tmp),1);
        tmp = (u8)(last->lastWrite_addr>>8);//数据帧上次读地址
        flash_write(addr++,&(tmp),1);
        tmp = (u8)(last->lastWrite_addr);
        flash_write(addr++,&(tmp),1);


        flash_write(addr,q->value,REG_SIZE_U);//寄存器数值
        addr = addr +REG_SIZE_U;
    }
    else
    {

        p = 0x5a;
        q =  getRegAddr();
        addr = 1;
        flash_read(addr++,&(q->bindCount),1);	//读出绑定设备数量

        flash_read(addr++,&(tmp),1);//读出上次数据帧地址
        last->lastRead_addr = tmp<<8;
        flash_read(addr++,&(tmp),1);
        last->lastRead_addr =last->lastRead_addr+ tmp;

        flash_read(addr++,&(tmp),1);//读出上次数据帧地址
        last->lastWrite_addr = tmp<<24;
        flash_read(addr++,&(tmp),1);
        last->lastWrite_addr =last->lastWrite_addr+ tmp<<16;
        flash_read(addr++,&(tmp),1);
        last->lastWrite_addr =last->lastWrite_addr+ tmp<<8;
        flash_read(addr++,&(tmp),1);
        last->lastWrite_addr =last->lastWrite_addr+ tmp;


        flash_read(addr,q->value,REG_SIZE_U);	//寄存器数值





        unsigned char *tmp;
        if(ROLE == GATEWAY)
        {

            tmp = ReadRegister(0x4006);//
            getConfig()->srcaddr[0] = tmp[0];
            getConfig()->srcaddr[1] = tmp[1];
            getConfig()->srcaddr[2] = tmp[2];
            getConfig()->srcaddr[3] = tmp[3];
            getConfig()->srcaddr[4] = tmp[4];
            getConfig()->srcaddr[5] = tmp[5];
            getConfig()->srcaddr[6] = tmp[6];
            getConfig()->srcaddr[7] = tmp[7];

            tmp = ReadRegister(0x44f1);//节点地址高2字节
            getConfig()->airrate = tmp[1];
            getConfig()->channel = tmp[3];

            tmp = ReadRegister(0x45f0);//
            getConfig()->TcpServer[0] = tmp[1];
            getConfig()->TcpServer[1] = tmp[3];
            getConfig()->TcpServer[2] = tmp[5];
            getConfig()->TcpServer[3] = tmp[7];
            getConfig()->TcpServerPort = ((u16)(tmp[8])<<8)+tmp[9];

            //WriteOneRegister(0x1006,57);//主动上报

        }
        else
        {
            tmp = ReadRegister(0xf007);//
            getConfig()->srcaddr[0] = tmp[0];
            getConfig()->srcaddr[1] = tmp[1];
            getConfig()->srcaddr[2] = tmp[2];
            getConfig()->srcaddr[3] = tmp[3];
            getConfig()->srcaddr[4] = tmp[4];
            getConfig()->srcaddr[5] = tmp[5];
            getConfig()->srcaddr[6] = tmp[6];
            getConfig()->srcaddr[7] = tmp[7];

            tmp = ReadRegister(0x1200);//
            getConfig()->destaddr[0] = tmp[0];
            getConfig()->destaddr[1] = tmp[1];
            getConfig()->destaddr[2] = tmp[2];
            getConfig()->destaddr[3] = tmp[3];
            getConfig()->destaddr[4] = tmp[4];
            getConfig()->destaddr[5] = tmp[5];
            getConfig()->destaddr[6] = tmp[6];
            getConfig()->destaddr[7] = tmp[7];

            tmp = ReadRegister(0x1205);//节点地址高2字节
            getConfig()->airrate = tmp[1];
            getConfig()->channel = tmp[3];

            tmp = ReadRegister(0x1207);//
            getConfig()->TcpServer[0] = tmp[1];
            getConfig()->TcpServer[1] = tmp[3];
            getConfig()->TcpServer[2] = tmp[5];
            getConfig()->TcpServer[3] = tmp[7];
            getConfig()->TcpServerPort = ((u16)(tmp[8])<<8)+tmp[9];

        }
        free(tmp);
        /* flash_init();
        	addr = 0;
         flash_write(addr++,&p,1);
         flash_write(addr++,&(q->bindCount),1);

         tmp = (u8)(last_addr>>8);
         flash_write(addr++,&(tmp),1);
         tmp = (u8)(last_addr);
         flash_write(addr++,&(tmp),1);

         flash_write(addr,q->value,2048);
         addr = addr +2048;*/
        //loraset(4,&p,9);
    }

    //loraModuleInit();
}


void LoraUartInit()
{

    __HAL_UART_CLEAR_IDLEFLAG(&huart2);
    __HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
    HAL_UART_DMAStop(&huart2);
    HAL_UART_DMAResume(&huart2);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
    HAL_UART_Receive_DMA(&huart2,loraUart.lora1RxBuffer,LORA_BUFFER_SIZE);

//		__HAL_UART_CLEAR_IDLEFLAG(&huart6);
//	//__HAL_UART_DISABLE_IT(&huart6, UART_IT_IDLE);	//使能空闲中断
//	//HAL_UART_DMAStop(&huart6);
//	__HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);	//使能空闲中断
//	HAL_UART_Receive_DMA(&huart6,loraUart.lora1RxBuffer,LORA_BUFFER_SIZE);
}
uint32_t cn;
void Lora_RxCpltCallback(unsigned char uartNo)
{
    unsigned char temp;

    if(uartNo == 2)
    {


        temp=__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE);
        if((temp!=RESET))
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart2);
            temp=huart2.Instance->SR;
            temp=huart2.Instance->DR;
            loraUart.rxSize = LORA_BUFFER_SIZE -hdma_usart2_rx.Instance->NDTR;
            loraUart.receivedFlag1 = 1;
            HAL_UART_DMAStop(&huart2);
            HAL_UART_DMAResume(&huart2);
            HAL_UART_Receive_DMA(&huart2,loraUart.lora1RxBuffer,LORA_BUFFER_SIZE);
            if(loraUart.ack ==1)
                loraUart.ack = 2;

        }
    }
    else if(uartNo == 6)
    {
        loraUart.receivedFlag2 = 1;
    }
}

void loraUartInterrupt(LORAHW_stru *p)
{
    if(p->loraNo == 0)
    {
        UART_HandleTypeDef huart2;
        if(p->enable == 0)
        {

            __HAL_UART_CLEAR_IDLEFLAG(&huart2);
            __HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
            HAL_UART_DMAStop(&huart2);
            HAL_UART_Receive_DMA(&huart2,loraUart.lora1RxBuffer,LORA_BUFFER_SIZE);
            __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
        }
        else
        {

            __HAL_UART_CLEAR_IDLEFLAG(&huart2);
            __HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
            HAL_UART_DMAStop(&huart2);
            //HAL_UART_Receive_DMA(&huart2,loraUart.lora1RxBuffer,LORA_BUFFER_SIZE);
            //	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
        }

    }
    else
    {
        UART_HandleTypeDef huart6;
        if(p->enable == 0)
        {

            __HAL_UART_CLEAR_IDLEFLAG(&huart6);
            __HAL_UART_DISABLE_IT(&huart6, UART_IT_IDLE);	//使能空闲中断
            HAL_UART_DMAStop(&huart6);
            HAL_UART_Receive_DMA(&huart6,loraUart.lora2RxBuffer,LORA_BUFFER_SIZE);
            __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);	//使能空闲中断
        }
        else
        {

            __HAL_UART_CLEAR_IDLEFLAG(&huart6);
            __HAL_UART_DISABLE_IT(&huart6, UART_IT_IDLE);	//使能空闲中断
            HAL_UART_DMAStop(&huart6);
            //HAL_UART_Receive_DMA(&huart2,loraUart.lora1RxBuffer,LORA_BUFFER_SIZE);
            //	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
        }
    }
}
/*******************************************************
功能描述：lora串口设置
返回值：
*********************************************************/
void loraUartSet(LORAHW_stru *p)
{
    if(p->loraNo == 0)
    {
        UART_HandleTypeDef huart2;
        huart2.Instance = USART2;
        huart2.Init.BaudRate = p->baudrate;
        huart2.Init.WordLength = UART_WORDLENGTH_8B;
        huart2.Init.StopBits = UART_STOPBITS_1;
        huart2.Init.Parity = UART_PARITY_NONE;
        huart2.Init.Mode = UART_MODE_TX_RX;
        huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart2.Init.OverSampling = UART_OVERSAMPLING_16;
        switch(p->Parity)
        {
        case 0:
            huart2.Init.Parity=UART_PARITY_NONE;
            break;//无校验
        case 1:
            huart2.Init.Parity=UART_PARITY_ODD;
            break;//奇校验
        case 2:
            huart2.Init.Parity=UART_PARITY_EVEN;
            break;//偶校验
        }
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
            Error_Handler();
        }

    }
    else
    {
        UART_HandleTypeDef huart6;
        huart6.Instance = USART6;
        huart6.Init.BaudRate = p->baudrate;
        huart6.Init.WordLength = UART_WORDLENGTH_8B;
        huart6.Init.StopBits = UART_STOPBITS_1;
        huart6.Init.Parity = UART_PARITY_NONE;
        huart6.Init.Mode = UART_MODE_TX_RX;
        huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart6.Init.OverSampling = UART_OVERSAMPLING_16;
        switch(p->Parity)
        {
        case 0:
            huart6.Init.Parity=UART_PARITY_NONE;
            break;//无校验
        case 1:
            huart6.Init.Parity=UART_PARITY_ODD;
            break;//奇校验
        case 2:
            huart6.Init.Parity=UART_PARITY_EVEN;
            break;//偶校验
        }
        if (HAL_UART_Init(&huart6) != HAL_OK)
        {
            Error_Handler();
        }
    }

}
/*******************************************************
功能描述：根据aux引脚判断lora模块是否是空闲状态
返回值：
        0：空闲
				1:非空闲
*********************************************************/
unsigned char loraIdle(LORAHW_stru *p)
{
    uint32_t idleTimeout;
    if(p->loraNo == 0)
    {
        idleTimeout = HAL_GetTick();
        while(HAL_GPIO_ReadPin(GPIOB,LORA_AUX_Pin)==0)
        {
            if((HAL_GetTick()-idleTimeout)>=3000)
                return 1;
        }
        return 0;
    }
    else
    {
        idleTimeout = HAL_GetTick();
        while(HAL_GPIO_ReadPin(GPIOB,LORA_AUX_Pin)==0)
        {
            if((HAL_GetTick()-idleTimeout)>=3000)
                return 1;
        }
        return 0;
    }
}
/*******************************************************
功能描述：lora工作模式切换
参数:
					p->mode:工作模式
          p->loraNo：lora模块选择
返回值：
        0：切换成功
				1:切换不成功
*********************************************************/
unsigned char  loraGpioset(LORAHW_stru *p)
{

    if(p->loraNo == 0)
    {
        while(loraIdle(p) == 1)
            ;
        switch(p->mode)
        {
        case TransmitMode:
            HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_RESET);
            break;
        case WorMOde:
            HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_SET);
            break;
        case ConfigMOde:
            HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_RESET);
            break;
        case SleepMode:
            HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_SET);
            break;
        default:
            HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_RESET);
            break;

        }

        while(loraIdle(p) == 1)
            return 1;
        HAL_Delay(2);
        return 0;
    }
    else
    {
        switch(p->mode)
        {
        case TransmitMode:
            HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_RESET);
            break;
        case WorMOde:
            HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_SET);
            break;
        case ConfigMOde:
            HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_RESET);
            break;
        case SleepMode:
            HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_SET);
            break;
        default:
            HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_RESET);
            break;
        }

        while(loraIdle(p) == 1)
            return 1;
        HAL_Delay(2);
        return 0;
    }
}


void loraset(unsigned char num,unsigned char *p,unsigned char len)
{

    if(num == 0)//1 地址设置
    {
        loraParams.addrH = p[0];
        loraParams.addrL = p[1];

    }
    else if(num == 1)//1 功率设置
    {
        loraParams.reg1 = loraParams.reg1 &0xfc;
        if(p[1]>=0x01&&p[1]<=0x03)
            loraParams.reg1 = loraParams.reg1 |0x03;
        else if(p[1]>=0x04&&p[1]<=0x05)
            loraParams.reg1 = loraParams.reg1 |0x02;
        else if(p[1]>=0x06&&p[1]<=0x07)
            loraParams.reg1 = loraParams.reg1 |0x01;
        else if(p[1]>=0x08&&p[1]<=0x10)
            loraParams.reg1 = loraParams.reg1 |0x00;

    }
    else if(num == 2)//2 空中速率
    {

        loraParams.reg0 = loraParams.reg0 &0xf8;
        switch(p[1])
        {
        case 1:
            loraParams.reg0 = loraParams.reg1 |0x00;
        case 2:
            loraParams.reg0 = loraParams.reg1 |0x00;
        case 3:
            loraParams.reg0 = loraParams.reg1 |0x00;
            break;
        case 4:
            loraParams.reg0 = loraParams.reg1 |0x01;
            break;
        case 5:
            loraParams.reg0 = loraParams.reg1 |0x02;
            break;
        case 6:
            loraParams.reg0 = loraParams.reg1 |0x03;
            break;
        case 7:
            loraParams.reg0 = loraParams.reg1 |0x04;
            break;
        case 8:
            loraParams.reg0 = loraParams.reg1 |0x05;
            break;
        case 9:
            loraParams.reg0 = loraParams.reg1 |0x06;
            break;
        case 10:
            loraParams.reg0 = loraParams.reg1 |0x07;
            break;
        }
    }
    else if(num == 3)//3 lora信道0-83
    {
        loraParams.reg2 = p[1];
    }
    else if(num == 4)
    {
        unsigned char *q;
        if(ROLE !=GATEWAY)
            q = ReadRegister(0xf00a);
        else
            q = ReadRegister(0x4409);

        loraParams.addrH = q[0];
        loraParams.addrL = q[1];

        loraParams.netid = 0;

        loraParams.reg0 =loraParams.reg0 &0x1f;
        loraParams.reg0 = loraParams.reg0|0x60;//波特率设置，bit7,6,

        loraParams.reg0 = loraParams.reg0 &0xe7;
        loraParams.reg0 = loraParams.reg0|0x00;//校验位设置bit4,3
        if(ROLE !=GATEWAY)
            q = ReadRegister(0x1205);//空速
        else
            q = ReadRegister(0x44f1);
        loraParams.reg0 = loraParams.reg0 &0xf8;

        switch(q[1])
        {
        case 1:
            loraParams.reg0 = loraParams.reg0 |0x00;
        case 2:
            loraParams.reg0 = loraParams.reg0 |0x00;
        case 3:
            loraParams.reg0 = loraParams.reg0 |0x00;
            break;
        case 4:
            loraParams.reg0 = loraParams.reg0 |0x01;
            break;
        case 5:
            loraParams.reg0 = loraParams.reg0 |0x02;
            break;
        case 6:
            loraParams.reg0 = loraParams.reg0 |0x03;
            break;
        case 7:
            loraParams.reg0 = loraParams.reg0 |0x04;
            break;
        case 8:
            loraParams.reg0 = loraParams.reg0 |0x05;
            break;
        case 9:
            loraParams.reg0 = loraParams.reg0 |0x06;
            break;
        case 10:
            loraParams.reg0 = loraParams.reg0 |0x07;
            break;
        }//空中速度设置bit2,1,0

        loraParams.reg1=loraParams.reg1 &0x3f;
        loraParams.reg1 = loraParams.reg1|00;//分包长度，bit7,6

        loraParams.reg1 =loraParams.reg1 &0xdf;
        loraParams.reg1 = loraParams.reg1&0;//环境噪声，bit5

        q = ReadRegister(0xf005);//功率
        loraParams.reg1 = loraParams.reg1 &0xfc;
        if(q[1]>=0x01&&q[1]<=0x03)
            loraParams.reg1 = loraParams.reg1 |0x03;
        else if(q[1]>=0x04&&q[1]<=0x05)
            loraParams.reg1 = loraParams.reg1 |0x02;
        else if(q[1]>=0x06&&q[1]<=0x07)
            loraParams.reg1 = loraParams.reg1 |0x01;
        else if(q[1]>=0x08&&q[1]<=0x10)
            loraParams.reg1 = loraParams.reg1 |0x00;

        if(ROLE !=GATEWAY)
            q = ReadRegister(0x1206);//信道
        else
            q = ReadRegister(0x44f2);

        loraParams.reg2 = q[1];//信道

        loraParams.reg3 = loraParams.reg3&0x7f;//rssi,bit7
        loraParams.reg3 = loraParams.reg3|00;//rssi,bit7


        loraParams.reg3 = loraParams.reg3&0xbf;
        loraParams.reg3 = loraParams.reg3|0x40;//定点传输,bit6

        loraParams.reg3 = loraParams.reg3&0xdf;//中继功能传输,bit5
        loraParams.reg3 = loraParams.reg3|0;//中继功能传输,bit5

        loraParams.reg3 = loraParams.reg3&0xef;//中继功能传输,bit5
        loraParams.reg3 = loraParams.reg3|0x10;//lbt,bit4

        loraParams.reg3 = loraParams.reg3&0xf7;//wor接收,bit3
        loraParams.reg3 = loraParams.reg3|0;//wor接收,bit3

        loraParams.reg3 = loraParams.reg3&0xf8;//wor周期,bit2,1,0
        loraParams.reg3 = loraParams.reg3|0;//wor周期,bit2,1,0

        loraParams.cryptH = 0x64;//秘钥高字节
        loraParams.cryptL = 0x46;//秘钥低字节

    }
    else if(num == 6)
    {
        loraParams.addrH = p[0];
        loraParams.addrL = p[1];
        loraParams.netid = p[2];
        loraParams.reg0 = p[3];
        loraParams.reg1 = p[4];
        loraParams.reg2 = p[5];
        loraParams.reg3 = p[6];
        loraParams.cryptH = p[7];
        loraParams.cryptL = p[8];
    }
    if(num<6)
    {
        lorahw.mode = 2;
        LoraSetPayloadPackTx(0xc0,0,9);//写lora模块

    }


}

/*******************************************************
功能描述：lora发送数据
参数:
					p->mode:工作模式
          p->loraNo：lora模块选择
					buffer:待发送数据
					len：待发送数据长度
返回值：
        0：切换成功
				1:切换不成功
*********************************************************/

unsigned char loraSend(LORAHW_stru *p,unsigned char *buffer,unsigned int len)
{
    unsigned char txbuffer[255];
    //unsigned char *q;
    static unsigned char tmp;
    //memcpy(q,buffer,len);
    if(p->loraNo == 0)
    {
        if(loraGpioset(p) == 0)
        {

            memcpy(txbuffer,buffer,len);
            HAL_Delay(100);
            //
            if(tmp == 0)
            {
                tmp = 1;


                HAL_UART_Transmit(&huart2,txbuffer,len,200);
            }
            else
            {
                //HAL_UART_DeInit(&huart2);
            HAL_UART_Transmit(&huart2,txbuffer,len,200);
        }

        //HAL_Delay(1000);
        return 0;
    }
    else
    {
        return 1;
    }
}
else
{
    if(loraGpioset(p) == 0)
    {
        //HAL_Delay(50);
        HAL_UART_Transmit(&huart6,buffer,len,5);
        HAL_Delay(5);
        return 0;
    }
    else
    {
        return 1;
    }


    }
}

unsigned char loraReg[9];
unsigned char *LoraPayloadToArray()
{

    unsigned char i;
    i=0;
    loraReg[i++] = loraParams.addrH;
    loraReg[i++] = loraParams.addrL;
    loraReg[i++] = loraParams.netid;
    loraReg[i++] = loraParams.reg0;
    loraReg[i++] = loraParams.reg1;
    loraReg[i++] = loraParams.reg2;
    loraReg[i++] = loraParams.reg3;
    loraReg[i++] = loraParams.cryptH;
    loraReg[i++] = loraParams.cryptL;
    return loraReg;
}
void LoraSetPayloadPackTx(unsigned cmd,unsigned char startaddr,unsigned char len)
{
    unsigned char p[255];
    unsigned char *q;
    unsigned char tmp;
    q = LoraPayloadToArray();

    p[0] = cmd;
    p[1] = startaddr;
    p[2] = len;
    if(cmd == 0xc0)
    {
        memcpy(&p[3],q+startaddr,len);
        tmp = len +3;
    }
    else
    {
        tmp = 3;
    }


    LORAHW_stru lorahw;
    lorahw.loraNo = 0;
    lorahw.mode = ConfigMOde;
    loraSend(&lorahw,p,tmp);
	wirelessTimoutStart(1);
}
static uint32_t loraTimeout;
void LoraSendPayloadPackTx(unsigned char *buffer,unsigned char len)
{

    unsigned char p[255];
    unsigned char *q;
    if(ROLE != GATEWAY)
        q = ReadRegister(0x1203);//目标lora地址
    else
        q = &buffer[6];//目标lora地址
    p[0] = q[0];
    p[1] = q[1];
    if(ROLE != GATEWAY)
        q = ReadRegister(0x1206);//信道
    else
        q = ReadRegister(0x44f2);//信道
    p[2] = q[1];
    memcpy(p+3,buffer,len);
    LORAHW_stru lorahw;
    lorahw.loraNo = 0;
    lorahw.mode = TransmitMode;
    loraSend(&lorahw,p,len+3);
    getloraProcStatus()->ack = 1;
    loraTimeout = HAL_GetTick();
    wirelessTimoutStart(1);
    //free(p);
}
unsigned char loraReapte(void)
{
    static unsigned char TimeoutCount;
    if(getloraProcStatus()->ack ==1)
    {
        if((HAL_GetTick()-loraTimeout)>=5000)
        {
            if(TimeoutCount <3)
            {
                TimeoutCount = TimeoutCount + 1;
                getloraProcStatus()->ack =0;
                return 0;
            }
            else
            {
                TimeoutCount = 0;
                getloraProcStatus()->ack =0;
                return 1;

            }
        }
    }
    else
        loraTimeout = HAL_GetTick();
    if(getloraProcStatus()->ack ==2)
        TimeoutCount = 0;
    return 1;
}
//unsigned char loraSendRery()
//{
//    static uint32_t reryTime;
//	static unsigned char reryCount;
//	if((HAL_GetTick()-reryTime)>=2)
//	{
//		if(getloraProcStatus()->ack == 1)
//		{
//			;//LoraSendPayloadPackTx(unsigned char *buffer,unsigned char len);
//		}

//	}


//}
void loraModuleInit()
{



#if DEBUG
    if(ROLE !=GATEWAY)
        p = ReadRegister(0xf00a);
    else
        p = ReadRegister(0x4409);

    loraParams.addrH = p[0];
    loraParams.addrL = p[1];

    loraParams.netid = 0;

    loraParams.reg0 = loraParams.reg0 &0x1f;
    loraParams.reg0 = loraParams.reg0|0x60;//波特率设置，bit7,6,

    loraParams.reg0 = loraParams.reg0 &0xe7;
    loraParams.reg0 = loraParams.reg0|0x00;//校验位设置bit4,3
    if(ROLE !=GATEWAY)
        p = ReadRegister(0x1205);
    else
        p = ReadRegister(0x44f1);

    loraParams.reg0 =loraParams.reg0 &0xf8;
    switch(p[1])//空中速度
    {
    case 1:
        loraParams.reg0 = loraParams.reg1 |0x00;
    case 2:
        loraParams.reg0 = loraParams.reg1 |0x00;
    case 3:
        loraParams.reg0 = loraParams.reg1 |0x00;
        break;
    case 4:
        loraParams.reg0 = loraParams.reg1 |0x01;
        break;
    case 5:
        loraParams.reg0 = loraParams.reg1 |0x02;
        break;
    case 6:
        loraParams.reg0 = loraParams.reg1 |0x03;
        break;
    case 7:
        loraParams.reg0 = loraParams.reg1 |0x04;
        break;
    case 8:
        loraParams.reg0 = loraParams.reg1 |0x05;
        break;
    case 9:
        loraParams.reg0 = loraParams.reg1 |0x06;
        break;
    case 10:
        loraParams.reg0 = loraParams.reg1 |0x07;
        break;
    }





    loraParams.reg1 = loraParams.reg1 &0x3f;
    loraParams.reg1 = loraParams.reg1|0x00;//分包长度，bit7,6

    loraParams.reg1 = loraParams.reg1 &0xdf;
    loraParams.reg1 = loraParams.reg1&0x00;//环境噪声，bit5

    loraParams.reg1 = loraParams.reg1 &0xfc;
    loraParams.reg1 = loraParams.reg1&0x00;//发射功率，bit1,0
    if(ROLE !=GATEWAY)
        p = ReadRegister(0x1206);//信道
    else
        p = ReadRegister(0x44f2);
    loraParams.reg2 = p[1];

    loraParams.reg3 = loraParams.reg3&0x7f;//rssi,bit7
    loraParams.reg3 = loraParams.reg3|00;//rssi,bit7

    loraParams.reg3 = loraParams.reg3&0xbf;//rssi,bit6
    loraParams.reg3 = loraParams.reg3|0x40;//定点传输,bit6

    loraParams.reg3 = loraParams.reg3&0xdf;//中继功能传输,bit5
    loraParams.reg3 = loraParams.reg3|0x00;//定点传输,bit5

    loraParams.reg3 = loraParams.reg3&0xef;//lbt,bit4
    loraParams.reg3 = loraParams.reg3|0x10;//lbt,bit4

    loraParams.reg3 = loraParams.reg3&0xf7;//wor接收,bit3
    loraParams.reg3 = loraParams.reg3|0;//wor接收,bit3

    loraParams.reg3 = loraParams.reg3&0xf8;//wor周期,bit2,1,0
    loraParams.reg3 = loraParams.reg3|0x00;//wor周期,bit2,1,0

    loraParams.cryptH = 0x64;//秘钥高字节
    loraParams.cryptL = 0x46;//秘钥低字节
    LoraSetPayloadPackTx(0xc0,0,9);//写lora模块
    ww = 49;
    RS485_SendData(&ww,1,0);
    //LoraSetPayloadPackTx(0xc1,0,9);//度lora模块参数
    lorahw.mode = 2;
#else
    loraset(4,0,0);
#endif




}
void Reconfig(void)
{

}
void LoraTest()
{
    //loraModuleInit();
    //LoraSetPayloadPackTx(0xc0,0,9);//写lora模块
   /* unsigned char p[240],i;
    for(i=0; i<210; i++)
        p[i] = i+1;
    LoraSendPayloadPackTx(p,210);*/
}

unsigned char EnterStop(void)
{
    unsigned char result ;
    //RTC_Init();
    LORAHW_stru p;
    if(ROLE !=GATEWAY)
    {
        p.loraNo = 0;
        p.mode =  SleepMode;
        loraGpioset(&p);

        p.loraNo = 1;
        p.mode =  SleepMode;
        loraGpioset(&p);

    }

//******************************gpio****************************************************


    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */



    /*Configure GPIO pins : DTR_Pin EN_3_3V_Pin SIM_PWR_Pin CTRL3__Pin
                             CTRL1__Pin */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
                          GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|
                          GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15;//CTRL1OUT11_Pin|MODE1_Pin|MODE2_Pin|CTRL1OUT21_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;//CTRL1OUT11_Pin|MODE1_Pin|MODE2_Pin|CTRL1OUT21_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);



    GPIO_InitStruct.Pin =GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = GPIO_PIN_3;//CTRL1OUT11_Pin|MODE1_Pin|MODE2_Pin|CTRL1OUT21_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

    GPIO_InitStruct.Pin =GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = GPIO_PIN_All;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);







    GPIO_InitStruct.Pin =GPIO_PIN_All;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);


    HAL_GPIO_WritePin(GPIOC, EN_3_3V_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, EN_5V_Pin, GPIO_PIN_RESET);



    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();
    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_DMA1_CLK_DISABLE();
    __HAL_RCC_DMA2_CLK_DISABLE();
    __HAL_RCC_USART1_CLK_DISABLE();
    __HAL_RCC_USART2_CLK_DISABLE();
    __HAL_RCC_UART5_CLK_DISABLE();
    __HAL_RCC_USART6_CLK_DISABLE();
    __HAL_RCC_ADC1_CLK_DISABLE();
    HAL_PWREx_EnableFlashPowerDown();

    RTC_WAKEUP_Init();

    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_SuspendTick();
    __HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG();
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    //   HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    HAL_PWR_EnterSTANDBYMode();
    __HAL_RTC_WAKEUPTIMER_EXTI_CLEAR_FLAG();
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    HAL_ResumeTick();
//	system_config();
//	HAL_Delay(5000);
//	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
//	HAL_Delay(1000);
    // __HAL_RTC_WAKEUPTIMER_DISABLE_IT(&hrtc,RTC_IT_WUT);

    result = 1;

    //__set_FAULTMASK(1);
    //NVIC_SystemReset();
    return result;

}

