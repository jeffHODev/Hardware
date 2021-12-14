#include "inout.h"
extern UART_HandleTypeDef huart3;

uint8_t LED_Ram[16]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};				// 数码显示屏 8888
uint8_t LED_Pilot=0x00;							// LED指示灯  bit 0 电解中   bit 1 雾化中 bit 2 缺水  bit3 min  bit 4 原水页面高 bit 5 废水页面高
uint8_t LED_Aper=20;								// LED光圈
uint8_t Speak_Flag=0;					/* 蜂鸣器      0 关闭  1 开机   2 按键音  3 结束音  4 报警音 */
inout_stru inout_usr;
/*
*********************************************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 配置LED指示灯相关的GPIO,  该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void Soft_Reset(void)
{
    __set_FAULTMASK(1);		  //关闭所有中断
    NVIC_SystemReset();			//复位系统
}

void inout_Init()
{
    //HAL_UART_Receive_IT(&huart3, &res, 1);
    //__HAL_UART_CLEAR_FLAG(&huart3,UART_IT_RXNE);

#if CPU == ST
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);	//使能空闲中断
    __HAL_UART_CLEAR_FLAG(&huart3,UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart3,inout_usr.Usart3_NewFax,UART3_SIZE);
#else
    usart_interrupt_enable(USART3, USART_INT_IDLE);
    usart_flag_clear(USART3, USART_INT_IDLE);
    usart_dma_receive_config(USART3, USART_DENR_ENABLE);
    nvic_irq_enable(DMA1_Channel4_IRQn, 0, 0);


    dma_single_data_parameter_struct dma_init_struct;


    /* deinitialize DMA1 channel2 (USART0 rx) */
    dma_deinit(DMA1, DMA_CH4);
    dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.memory0_addr = (uint32_t)inout_usr.Usart3_NewFax;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.number = UART3_SIZE;
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART3);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH4, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH4);
    dma_channel_subperipheral_select(DMA1, DMA_CH4, DMA_SUBPERI4);
    /* enable DMA1 channel2 transfer complete interrupt */
    dma_interrupt_enable(DMA1, DMA_CH4, DMA_CHXCTL_FTFIE);
    /* enable DMA1 channel2 */
    dma_channel_enable(DMA1, DMA_CH4);
#endif
}
/***********************************************************************************
 * 函数名：EM4305_Read_Data(uint8_t page,uint8_t data[4])
 * 描述  ：从卡中读取一页数据
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用
**********************************************************************************/
/* 添加对显示板上LED灯的控制 LED_Ram[16]---LED_[24] 其中1表示量 0表示灭  ---20210722 */

uint8_t Send_LED_Data(void)
{
    static uint8_t send_time;
    uint8_t buf[10+14];
    uint8_t i,err=0,temp;

    if(send_time++<2) return 0;
    send_time=0;

    // 填写数据
    buf[0]=0xAA;
    buf[1]=0x0A;

    buf[2]=LED_Ram[3];
    buf[3]=LED_Ram[2];
    buf[4]=LED_Ram[1];
    buf[5]=LED_Ram[0];

    buf[6]=LED_Ram[7];
    buf[7]=LED_Ram[6];
    buf[8]=LED_Ram[5];
    buf[9]=LED_Ram[4];

    buf[10]=LED_Ram[11];
    buf[11]=LED_Ram[10];
    buf[12]=LED_Ram[9];
    buf[13]=LED_Ram[8];

    buf[14]=LED_Ram[15];
    buf[15]=LED_Ram[14];
    buf[16]=LED_Ram[13];
    buf[17]=LED_Ram[12];

    buf[18]=LED_Pilot;
    buf[19]=LED_Aper;
    buf[20]=Speak_Flag;
    Speak_Flag=0;

    //buf[21]=


    // 校验
    for(i=1; i<21; i++)
    {
        err^=buf[i];
    }

    buf[21]=err;
    buf[22]=0xBB;

    // 发送数据
    temp=HAL_UART_Transmit(&huart3,buf,23,10);


    return temp;
}
void inout_callback()
{
    inout_usr.receivedFlag =1;
    HAL_UART_DMAStop(&huart3);
    HAL_UART_DMAResume(&huart3)	;
    HAL_UART_Receive_DMA(&huart3,inout_usr.Usart3_NewFax,UART3_SIZE);

}
inout_stru *GetInOut()
{
    return &inout_usr;
}

/***********************************************************************************
 * 函数名：void System_Read_Fax(void)
 * 描述  ：从卡中读取一页数据
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用 AA 0A RAM0 RAM1 ...RAM15 PILOT APER KEY ECC BB
**********************************************************************************/
uint8_t System_Read_Fax(void)
{
    uint8_t err=0,i=0;

    // 获取到1帧数据
    if(inout_usr.receivedFlag !=0)												// 读取到数据
    {
        inout_usr.receivedFlag=0;													// 清除标志
    }
    else																						// 串口未收到有效数据
    {
        return 1;																			// 退出该程序
    }

    // 解析
    if(inout_usr.Usart3_NewFax[0]!=0xAA) 											// 校验
    {
        return 1;
    }

    if(inout_usr.Usart3_NewFax[1]!=0x0A) 											// 校验
    {
        return 1;
    }

    if(inout_usr.Usart3_NewFax[23]!=0xBB)											// 判断结束符
    {
        return 1;
    }

    // 校验
    for(i=1; i<23; i++)																// 校验
    {
        err^=inout_usr.Usart3_NewFax[i];
    }

//	if(err!=0) 																			// 异常
//	{
//		return 0;
//	}

    // 按键判断
    inout_usr.key_wash_mode=inout_usr.Usart3_NewFax[19];
    inout_usr.key_reset_mode=inout_usr.Usart3_NewFax[20];
    inout_usr.key_wifi_mode=inout_usr.Usart3_NewFax[21];

    return 0;
}


