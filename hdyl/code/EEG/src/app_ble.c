#include "app_ble.h"
#include "ble.h"
#include "myQueue.h"
#include "usart.h"
#include "ADS129x.h"
#include "bsp.h"
#include "ecg.h"
#include "protocol.h"
#include "mmr901mx.h"
#include "nibp_if.h"
#include "ecg.h"
#include "sensor.h"
#include "ble_set.h"

QueueInfo *UART_Queue;//���л�����ָ��
extern u8 UART3_DMA_Finish;

void dev_init()
{
    power_manage(BLE_PWR,ON);//��Դʹ��
    BleModeSetting(CONFIG_MODE);//����ģʽ
     BleConSeting(WAKUP);
    BleConSeting(DIS_CONNECT);
    usart_baudrate_set(((UART3)), 460800);//�������ڲ�����
    while(sendCommand("AT+BAUD?", "460800",100, 3)== Failure)//������)
    {
        usart_baudrate_set(((UART3)), 115200);
        sendCommand("AT+BAUD = 460800", "+OK",100, 1);//������
        sendCommand("AT+LINKMAST = 1", "+OK",100, 1);//����1�����豸
        //sendCommand("AT+DEVMANUF = HDYL", "+OK",100, 3);;//�豸��
    }
    power_manage(BLE_PWR,OFF);//����������Ч
    delay_ms(200);
    power_manage(BLE_PWR,ON);
    usart_baudrate_set(((UART3)), 460800);
    //sendCommand("AT+ADVDAT1 = HDYLEEGDEVADV", "+OK",100, 3);//�㲥����

}
void app_init(void)
{
    dev_init();

}
//�����ڷ���
void Send_UART3(void)
{
    u8 i,sum,res;
    //printf("�����ڷ���\r\n");
    //TIM_Cmd(TIM4, DISABLE);
/////////////////////////////////////////////////////////////////
    //��ʼ��129X
#if ADS129xDEVICE == ADS1299
    ADS129x_REG_BUF[1]=0x95; //500������
#elif ADS129xDEVICE == ADS1298
    ADS129x_REG_BUF[1]=0x86; //500������
#endif
    ADS129x_ReInit(0);//��ʼ��1299
/////////////////////////////////////////////////////////////////
    //Main_printf("�ڴ浱ǰռ�� %d\r\n",mem_perused());
    UART_Queue=queue_init(UART_QUEUE_SIZE,UART_QUEUE_LENGTH) ;//ѭ�����г�ʼ����С
    if(UART_Queue != NULL)
        ;//Main_printf("�ڴ�����ɹ�����ǰռ�� %d\r\n",mem_perused());
    else
        ;//Main_printf("�ڴ�����ʧ��\r\n");
/////////////////////////////////////////////////////////////////
    //printf("�������ڼ�DMA�ж�\r\n");
    //uart1_init(460800);//���ڳ�ʼ�� HC05����
    usart_baudrate_set(((UART3)), 460800);
    dma_periph_address_config(DMA0, DMA_CH3, (uint32_t)USART_DATA(UART3));
    dma_memory_address_config(DMA0, DMA_CH3, 0, (uint32_t)&data_to_send);
    dma_transfer_number_config(DMA0, DMA_CH3, 11);
    dma_channel_disable(DMA0, DMA_CH3);
    usart_dma_transmit_config(UART3, USART_DENT_DISABLE);
    //	DMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)data_to_send);
    //UART1_DMA_TX_NVIC_Config(1);//�������� DMA�ж�
    //	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //DMA
    UART3_DMA_Finish=1;
/////////////////////////////////////////////////////////////////
    //printf("��ʼ��������\r\n");
    data_to_send[0]=0xAA;
    data_to_send[1]=0xAA;
    data_to_send[2]=0xF1;
    data_to_send[3]=33;	//���͸�8ͨ�������� + 1�ֽڶ�������
//		LED_1=LED_ON;
//		LED_2=LED_ON;
//		delay_s(1);
//		LED_1=LED_OFF;
//		LED_2=LED_OFF;
//		KEY_TYPE=0;
    //TIM_Cmd(TIM4, ENABLE);
    //EXTI->IMR |= ADS129X_DRDY_LINE;//��DRDY�ж�
    nvic_irq_enable(EXTI1_IRQn, 2U, 0U);
    nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);
/////////////////////////////////////////////////////////////////
    while(work_state==SEND_BULE)
    {
        //	Key_Scan();
        //	Change_Mode();

        if(UART3_DMA_Finish==1)
        {
            if(serch_queue_data(UART_Queue))//����������
            {
                //�Ӷ�����ȡ�����ݲ�����
                for(i=0; i<8; i++) //����8��ͨ��������
                {
                    cannle[i] = *(*(UART_Queue->databuf + UART_Queue->front)+0+i*3)<<16
                                | *(*(UART_Queue->databuf + UART_Queue->front)+1+i*3)<<8
                                | *(*(UART_Queue->databuf + UART_Queue->front)+2+i*3);//��ȡԭʼ����
                    p_Temp[i] = get_volt(cannle[i]);	//ת��32λ�з�����
                    cannle[i] = p_Temp[i];//��ֵ���޷�������׼���߼�����
                    data_to_send[4+i*4]=cannle[i]>>24;	//25-32λ
                    data_to_send[5+i*4]=cannle[i]>>16;  //17-24
                    data_to_send[6+i*4]=cannle[i]>>8;		//9-16
                    data_to_send[7+i*4]=cannle[i];			//1-8
                }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //���������ź�
                data_to_send[ (ADS129X_USE_CANNLE+1)*4 ]=res++;//0-255
                sum=0;
                for(i=0; i< (ADS129X_USE_CANNLE+1)*4 +1 ; i++)
                    sum += data_to_send[i];
                data_to_send[(ADS129X_USE_CANNLE+1)*4 +1] = sum;	//У���

                dma_transfer_number_config(DMA0, DMA_CH3, (ADS129X_USE_CANNLE+1)*4 +2);
                dma_channel_enable(DMA0, DMA_CH3);
                usart_dma_transmit_config(UART3, USART_DENT_ENABLE);
                //DMA_Enable(DMA1_Channel4,(ADS129X_USE_CANNLE+1)*4 +2);//����DMA
                UART3_DMA_Finish=0;
                UART_Queue->front = (UART_Queue->front+1) % UART_Queue->capacity;//ȡ���ݣ���ͷ�����������ݣ���β����
            }
        }
    }
    nvic_irq_disable(EXTI1_IRQn);
    nvic_irq_disable(EXTI10_15_IRQn);


    queue_Deinit(UART_Queue);//ѭ������ע��
    //Main_printf("�ڴ浱ǰռ�� %d\r\n",mem_perused());
}

void app(void)
{
    if(BleConSeting(CONNECT_STA)==0)//�����Ѿ�����
    {
        Send_UART3();
        protocol_process();
		sensor_proc();
        // MMR_process();
        nibp_if_process();

    }
    else
    {

    }
}
