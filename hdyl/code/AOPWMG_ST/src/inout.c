#include "inout.h"
extern UART_HandleTypeDef huart3;

uint8_t LED_Ram[16]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};				// ������ʾ�� 8888
uint8_t LED_Pilot=0x00;							// LEDָʾ��  bit 0 �����   bit 1 ���� bit 2 ȱˮ  bit3 min  bit 4 ԭˮҳ��� bit 5 ��ˮҳ���
uint8_t LED_Aper=20;								// LED��Ȧ
uint8_t Speak_Flag=0;					/* ������      0 �ر�  1 ����   2 ������  3 ������  4 ������ */
inout_stru inout_usr;
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Soft_Reset(void)
{
    __set_FAULTMASK(1);		  //�ر������ж�
    NVIC_SystemReset();			//��λϵͳ
}

void inout_Init()
{
    //HAL_UART_Receive_IT(&huart3, &res, 1);
    //__HAL_UART_CLEAR_FLAG(&huart3,UART_IT_RXNE);

#if CPU == ST
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);	//ʹ�ܿ����ж�
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
 * ��������EM4305_Read_Data(uint8_t page,uint8_t data[4])
 * ����  ���ӿ��ж�ȡһҳ����
 * ����  ����
 * ���  : ��ȡ�����ݷ���1 δ��ȡ�����򷵻�0
 * ����  ���ⲿ����
**********************************************************************************/
/* ��Ӷ���ʾ����LED�ƵĿ��� LED_Ram[16]---LED_[24] ����1��ʾ�� 0��ʾ��  ---20210722 */

uint8_t Send_LED_Data(void)
{
    static uint8_t send_time;
    uint8_t buf[10+14];
    uint8_t i,err=0,temp;

    if(send_time++<2) return 0;
    send_time=0;

    // ��д����
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


    // У��
    for(i=1; i<21; i++)
    {
        err^=buf[i];
    }

    buf[21]=err;
    buf[22]=0xBB;

    // ��������
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
 * ��������void System_Read_Fax(void)
 * ����  ���ӿ��ж�ȡһҳ����
 * ����  ����
 * ���  : ��ȡ�����ݷ���1 δ��ȡ�����򷵻�0
 * ����  ���ⲿ���� AA 0A RAM0 RAM1 ...RAM15 PILOT APER KEY ECC BB
**********************************************************************************/
uint8_t System_Read_Fax(void)
{
    uint8_t err=0,i=0;

    // ��ȡ��1֡����
    if(inout_usr.receivedFlag !=0)												// ��ȡ������
    {
        inout_usr.receivedFlag=0;													// �����־
    }
    else																						// ����δ�յ���Ч����
    {
        return 1;																			// �˳��ó���
    }

    // ����
    if(inout_usr.Usart3_NewFax[0]!=0xAA) 											// У��
    {
        return 1;
    }

    if(inout_usr.Usart3_NewFax[1]!=0x0A) 											// У��
    {
        return 1;
    }

    if(inout_usr.Usart3_NewFax[23]!=0xBB)											// �жϽ�����
    {
        return 1;
    }

    // У��
    for(i=1; i<23; i++)																// У��
    {
        err^=inout_usr.Usart3_NewFax[i];
    }

//	if(err!=0) 																			// �쳣
//	{
//		return 0;
//	}

    // �����ж�
    inout_usr.key_wash_mode=inout_usr.Usart3_NewFax[19];
    inout_usr.key_reset_mode=inout_usr.Usart3_NewFax[20];
    inout_usr.key_wifi_mode=inout_usr.Usart3_NewFax[21];

    return 0;
}


