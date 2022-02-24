#include "app_ble.h"
#include "ble.h"
#include "myQueue.h"
#include "usart.h"
#include "ADS1292x.h"
#include "bsp.h"
#include "ecg.h"
#include "protocol.h"
#include "mmr901mx.h"
#include "nibp_if.h"
#include "ecg.h"
#include "sensor.h"
#include "ble_set.h"
#include "main2.h"
QueueInfo *UART_Queue;//队列缓冲区指针
extern u8 UART3_DMA_Finish;
extern	u8 data_to_send[50];//串口发送缓存

void dev_init()
{
    power_manage(BLE_PWR,ON);//电源使能
    BleModeSetting(CONFIG_MODE);//配置模式
    BleConSeting(WAKUP);
    BleConSeting(DIS_CONNECT);
    usart_baudrate_set(((UART3)), 460800);//蓝牙串口波特率
    /*   while(sendCommand("AT+BAUD?", "460800",100000, 3)== Failure)//波特率)
       {
           usart_baudrate_set(((UART3)), 115200);
           sendCommand("AT+BAUD = 460800", "+OK",100000, 1);//波特率
           sendCommand("AT+LINKMAST = 1", "+OK",100000, 1);//连接1个主设备
           //sendCommand("AT+DEVMANUF = HDYL", "+OK",100, 3);;//设备厂
       }**/
    ble_proc();
    power_manage(BLE_PWR,OFF);//配置重启生效
    delay_ms(200);
    power_manage(BLE_PWR,ON);
    usart_baudrate_set(((UART3)), 460800);
    BleModeSetting(TRANSMIMT_MODE);//透传模式
    //sendCommand("AT+ADVDAT1 = HDYLEEGDEVADV", "+OK",100, 3);//广播数据

}
void app_init(void)
{
    dev_init();

}
void chip_conver_timeout(void)
{
    static unsigned char chip_num_tmp=1;
    static uint32_t timeout;
    if(getSensor()->chip_num == chip_num_tmp)
    {
        if((HAL_GetTick()-timeout)>=1500)
        {
            timeout = HAL_GetTick();
					
            if(getSensor()->chip_num == 1)
            {
              //  ads1299NoSet(2);
                getSensor()->chip_num = 2;
				       // pushdata(1);
				//queue_data_push(UART_Queue,ads129x_Cache+3,ADS129x_info.Ads129x_Data_Move,ADS129x_info.Ads129x_Write_Num)
              //  nvic_irq_disable(EXTI1_IRQn);
               // nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);

            }
            else
            {
                ads1299NoSet(1);
				       // pushdata(2);
				//queue_data_push(UART_Queue,ads129x_Cache+27,ADS129x_info.Ads129x_Data_Move,ADS129x_info.Ads129x_Write_Num)
                getSensor()->chip_num = 1;
                nvic_irq_enable(EXTI1_IRQn, 2U, 0U);
                nvic_irq_disable(EXTI10_15_IRQn);

            }
        }
    }
    else
    {
        chip_num_tmp = getSensor()->chip_num;
        timeout = HAL_GetTick();
    }
}
//主串口发送
void Send_UART3(void)
{
    u8 i,sum;
    static u8 res;
    static u8 flag;
    //printf("主串口发送\r\n");
    //TIM_Cmd(TIM4, DISABLE);
    if(flag == 0)
    {
        flag =1;
        /////////////////////////////////////////////////////////////////
        //初始化129X
#if ADS129xDEVICE == ADS1299
        ADS129x_REG_BUF[1]=0x95; //500采样率
#elif ADS129xDEVICE == ADS1298
        ADS129x_REG_BUF[1]=0x86; //500采样率
#endif
        // ADS129x_ReInit(0);//初始化1299
        /////////////////////////////////////////////////////////////////
        //Main_printf("内存当前占用 %d\r\n",mem_perused());
        UART_Queue=queue_init(UART_QUEUE_SIZE,UART_QUEUE_LENGTH) ;//循环队列初始化大小
        if(UART_Queue != NULL)
            ;//Main_printf("内存申请成功，当前占用 %d\r\n",mem_perused());
        else
            ;//Main_printf("内存申请失败\r\n");
        /////////////////////////////////////////////////////////////////
        //printf("开启串口及DMA中断\r\n");
        //uart1_init(460800);//串口初始化 HC05串口
        usart_baudrate_set(((UART3)), 460800);
        dma_periph_address_config(DMA0, DMA_CH3, (uint32_t)USART_DATA(UART3));
        dma_memory_address_config(DMA0, DMA_CH3, 0, (uint32_t)&data_to_send);
        dma_transfer_number_config(DMA0, DMA_CH3, 11);
        dma_channel_disable(DMA0, DMA_CH3);
        usart_dma_transmit_config(UART3, USART_DENT_DISABLE);
        //	DMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)data_to_send);
        //UART1_DMA_TX_NVIC_Config(1);//开启串口 DMA中断
        //	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //DMA
        UART3_DMA_Finish=1;
        /////////////////////////////////////////////////////////////////
        //printf("开始发送数据\r\n");
#if BLE_DEBUG
        data_to_send[0]=0xAA;
        data_to_send[1]=0xAA;
        data_to_send[2]=0xF1;
        data_to_send[3]=33; //发送个8通道的数据 + 1字节丢包测试
#endif
        //		LED_1=LED_ON;
        //		LED_2=LED_ON;
        //		delay_s(1);
        //		LED_1=LED_OFF;
        //		LED_2=LED_OFF;
        //		KEY_TYPE=0;
        //TIM_Cmd(TIM4, ENABLE);
        //EXTI->IMR |= ADS129X_DRDY_LINE;//开DRDY中断

//#if ADS_CHANNEL <= 8
nvic_irq_enable(EXTI1_IRQn, 2U, 0U);
//#endif

      getSensor()->chip_num = 1;
        nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);
     

    }
 //#if ADS_CHANNEL>8
    //    chip_conver_timeout();
//#endif 

/////////////////////////////////////////////////////////////////
    if(work_state==SEND_BULE)
    {
        //	Key_Scan();
        //	Change_Mode();

        if(UART3_DMA_Finish==1)
        {
            if(serch_queue_data(UART_Queue))//队列有数据
            {

                // nvic_irq_disable(EXTI1_IRQn);
                // nvic_irq_disable(EXTI10_15_IRQn);
                //从队列中取出数据并处理
                for(i=0; i<ADS_CHANNEL; i++) //处理8个通道的数据  27   24
                {
                   if(i >= 9)
                   	{
				   cannle[i] = *(*(UART_Queue->databuf + UART_Queue->front)+0+i*3+3)<<16
							   | *(*(UART_Queue->databuf + UART_Queue->front)+1+i*3+3)<<8
							   | *(*(UART_Queue->databuf + UART_Queue->front)+2+i*3+3);//获取原始数据

				   }
				   else
				   	{
				   cannle[i] = *(*(UART_Queue->databuf + UART_Queue->front)+0+i*3)<<16
							   | *(*(UART_Queue->databuf + UART_Queue->front)+1+i*3)<<8
							   | *(*(UART_Queue->databuf + UART_Queue->front)+2+i*3);//获取原始数据

				   }
				   	

                    p_Temp[i] = get_volt(cannle[i]);	//转成32位有符号数
                    cannle[i] = p_Temp[i];//赋值给无符号数，准备逻辑右移
                    data_to_send[4+i*4]=cannle[i]>>24;	//25-32位
                    data_to_send[5+i*4]=cannle[i]>>16;  //17-24
                    data_to_send[6+i*4]=cannle[i]>>8;		//9-16
                    data_to_send[7+i*4]=cannle[i];			//1-8
                }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //递增测试信号
                data_to_send[ (ADS_CHANNEL+1)*4 ]=res++;//0-255
                sum=0;
                for(i=0; i< (ADS_CHANNEL+1)*4 +1 ; i++)
                    sum += data_to_send[i];
                data_to_send[(ADS_CHANNEL+1)*4 +1] = sum;	//校验和
                /*dma_interrupt_enable(DMA0, DMA_CH3, DMA_CHXCTL_FTFIE);

                dma_transfer_number_config(DMA0, DMA_CH3, (ADS129X_USE_CANNLE+1)*4 +2);
                dma_channel_enable(DMA0, DMA_CH3);
                usart_dma_transmit_config(UART3, USART_DENT_ENABLE);
                */
#if BLE_DEBUG

                dma_flag_clear(DMA0,DMA_CH4,DMA_INTF_FTFIF);
                NVIC_EnableIRQ(DMA0_Channel4_IRQn);
                dma_channel_disable(DMA0, DMA_CH4);
                dma_memory_address_config(DMA0, DMA_CH4,DMA_MEMORY_0, (uint32_t *)(&data_to_send[0])); //设置要发送数据的内存地址
                dma_transfer_number_config(DMA0, DMA_CH4,  (ADS129X_USE_CANNLE+1)*4 +2+2); 					   //一共发多少个数据
                //dma_transfer_number_config(DMA0, DMA_CH4,  2);

                dma_channel_enable(DMA0, DMA_CH4);
                /* USART DMA enable for transmission and reception */
                usart_dma_transmit_config(UART3, USART_DENT_ENABLE); //使能串口DMA发送





                /* dma_channel_disable(DMA0, DMA_CH4);
                 dma_memory_address_config(DMA0, DMA_CH4,DMA_MEMORY_0, (uint32_t *)(&data_to_send[0]));
                 dma_transfer_number_config(DMA0, DMA_CH4,  (ADS129X_USE_CANNLE+1)*4 +2);
                 /* enable DMA channel1 */
                /*/  dma_channel_enable(DMA0, DMA_CH4);
                  /* USART DMA enable for transmission and reception */
                /*  usart_dma_transmit_config(UART3, USART_DENT_ENABLE);*/

#endif

                UART3_DMA_Finish=0;

                UART_Queue->front = (UART_Queue->front+1) % UART_Queue->capacity;//取数据，队头自增，存数据，队尾自增

            }
        }
    }



    //queue_Deinit(UART_Queue);//循环队列注销
    //Main_printf("内存当前占用 %d\r\n",mem_perused());
}
uint32_t powerSleepTick;
void app(void)
{

    static unsigned char init_flag;
    if(getSensor()->key_status)
    {
        if(getSensor()->key_status==2)
        {

        }
        else
        {

        }
        getSensor()->key_status=0;
    }
    else
    {
        if(BleConSeting(CONNECT_STA)==0)//蓝牙已经连接
        {
            //*getstate()=SEND_BULE;
            if((HAL_GetTick()-powerSleepTick)>=30000)
            {
                if(*getstate()!=SEND_BULE)
                {
                    BleConSeting(DIS_CONNECT);
                    power_sleep();
                }
                else
                    powerSleepTick = HAL_GetTick();
            }
            if(*getstate()==SEND_BULE)
            {
                power_manage(5,ON);
                // MMR_process();

                if(init_flag == 0)
                {
                    init_flag = 1;
                    ADS129x_ReInit(0);
					nvic_irq_enable(EXTI1_IRQn, 2U, 0U);
       				 nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);
                }
                sensor_proc();
                Send_UART3();

            }
            else
            {
                nvic_irq_disable(EXTI1_IRQn);
                nvic_irq_disable(EXTI10_15_IRQn);

            }
            protocol_process();
            nibp_if_process();
        }
        else
        {
            // nvic_irq_disable(EXTI1_IRQn);
            // nvic_irq_disable(EXTI10_15_IRQn);
					init_flag = 0;
            powerSleepTick = HAL_GetTick();
            power_sleep();

        }

    }
}
