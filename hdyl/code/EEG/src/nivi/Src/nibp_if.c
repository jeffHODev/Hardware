#include "nibp_if.h"
#include "stdio.h"
#include "stdlib.h"
#include "bsp.h"
#include "kfifo.h"
#include "protocol.h"
//extern UART_HandleTypeDef huart2;
//extern TIM_HandleTypeDef htim4;

static uint8_t buffer_rx[64];
static uint32_t lastcounter;

static struct __kfifo rxfifo;
static uint8_t rxfifobuf[256];

static uint8_t parity(uint8_t in)
{
    return(0xD6 + (in & 0x0F) + (in >> 4));
}

static void nibp_tst_init(void)
{
    // HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
}

void nibp_if_init(void)
{
// nibp_tst_init();
    uint8_t *pbuf;
    dma_single_data_parameter_struct dma_single_data_parameter;
    __kfifo_init(&rxfifo, rxfifobuf, sizeof(rxfifobuf), sizeof(uint8_t));

    rcu_periph_clock_enable(RCU_DMA0);

    /* deinitialize DMA channel1 */
    dma_deinit(DMA0,DMA_CH6);
    dma_single_data_parameter.direction = DMA_MEMORY_TO_PERIPH;
    dma_single_data_parameter.memory0_addr = NULL;
    dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_single_data_parameter.periph_memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_single_data_parameter.number = 1;
    dma_single_data_parameter.periph_addr = (uint32_t)&USART_DATA(USART1);
    dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_single_data_parameter.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH6, &dma_single_data_parameter);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH6);
    dma_channel_subperipheral_select(DMA0, DMA_CH6, DMA_SUBPERI4);
    dma_interrupt_enable(DMA0, DMA_CH6, DMA_CHXCTL_FTFIE);

   NVIC_EnableIRQ(DMA0_Channel6_IRQn);

   dma_interrupt_flag_clear(DMA0, DMA_CH6, DMA_INT_FLAG_FTF);
   dma_flag_clear(DMA0,DMA_CH6,DMA_INTF_FTFIF);
   usart_dma_transmit_config(USART1, USART_DENT_DISABLE); //使能串口DMA发送
   dma_channel_disable(DMA0, DMA_CH6);



   
   // rcu_periph_clock_enable(RCU_DMA0);

    /* wait DMA Channel transfer complete */
	//
	//
	//dma_interrupt_flag_clear(DMA0, DMA_CH6, DMA_INT_FLAG_FTF);
	//
	//dma_memory_address_config(DMA0, DMA_CH6,DMA_MEMORY_0, &pb[0]); //设置要发送数据的内存地址
	//dma_transfer_number_config(DMA0, DMA_CH6,  0);					   //一共发多少个数据
	//dma_transfer_number_config(DMA0, DMA_CH4,  2);
	
	//dma_channel_enable(DMA0, DMA_CH6);
	/* USART DMA enable for transmission and reception */
	//usart_dma_transmit_config(USART1, USART_DENT_ENABLE); //使能串口DMA发送















    /* deinitialize DMA1 channel2 (USART0 rx) */
    dma_deinit(DMA0, DMA_CH5);
    dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;
    dma_single_data_parameter.memory0_addr = (uint32_t)buffer_rx;
    dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_single_data_parameter.number = sizeof(buffer_rx);
    dma_single_data_parameter.periph_addr = (uint32_t)&USART_DATA(USART1);
    dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_single_data_parameter.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH5, &dma_single_data_parameter);

    /* configure DMA mode */
    dma_circulation_enable(DMA0, DMA_CH5);
    dma_channel_subperipheral_select(DMA0, DMA_CH5, DMA_SUBPERI4);
    /* enable DMA1 channel2 transfer complete interrupt */
    dma_interrupt_enable(DMA0, DMA_CH5, DMA_CHXCTL_FTFIE);
    /* enable DMA1 channel2 */
    dma_channel_enable(DMA0, DMA_CH5);
		uint8_t pb[1];
		uart1_dma_tx(pb,1);
lastcounter = dma_transfer_number_get(DMA0, DMA_CH5);
	//uart1_dma_tx(0,1);

}

static void nibp_if_recv_process(void)
{
    uint32_t counter = dma_transfer_number_get(DMA0, DMA_CH5);
    if((counter != 0) && (lastcounter != counter))
    {
        if(lastcounter > counter)
        {
            __kfifo_in(&rxfifo, &buffer_rx[sizeof(buffer_rx) - lastcounter], lastcounter - counter);
        }
        else
        {
            __kfifo_in(&rxfifo, &buffer_rx[sizeof(buffer_rx) - lastcounter], lastcounter);
            __kfifo_in(&rxfifo, &buffer_rx[0], sizeof(buffer_rx) - counter);
        }
        lastcounter = counter;
    }
}

int8_t nibp_if_cmd(uint8_t cmd)
{
#if 0
    static uint8_t buffer[8];
    if(usart_flag_get(USART1, USART_FLAG_BSY) != SET)
    {
        return -1;
    }
    buffer[0] = 0x02;
    sprintf((char *)&buffer[1], "%02X;;%02X", cmd, parity(cmd));
    buffer[7] = 0x03;
    uart1_dma_tx(buffer,8);
    //HAL_UART_Transmit_DMA(&huart2, buffer, 8);
    return 0;
#else
    static uint8_t buffer[8];
    if(usart_flag_get(USART1, USART_FLAG_BSY) != SET)
    {
        return -1;
    }
    buffer[0] = 0x02;
    sprintf((char *)&buffer[1], "%02X;;%02X", cmd, parity(cmd));
    buffer[7] = 0x03;
    uart1_dma_tx(buffer,8);
    //HAL_UART_Transmit_DMA(&huart2, buffer, 8);
    return 0;

#endif
}
int8_t nibp_if_speccmd(uint8_t cmd,uint8_t *buf,uint8_t len)
{
#if 0
    static uint8_t buffer[8];
    if(usart_flag_get(USART1, USART_FLAG_BSY) != SET)
    {
        return -1;
    }
    buffer[0] = 0x02;
    sprintf((char *)&buffer[1], "%02X;;%02X", cmd, parity(cmd));
    buffer[7] = 0x03;
    uart1_dma_tx(buffer,8);
    //HAL_UART_Transmit_DMA(&huart2, buffer, 8);
    return 0;
#else
    static uint8_t buffer[8];
   uint8_t j = 0;

   
    if(usart_flag_get(USART1, USART_FLAG_BSY) != SET)
    {
    //    return -1;
    }
    buffer[0] = 0x3a;
    buffer[1] = cmd;
    if(len == 0)
    {
        buffer[2] = 0;

    }
    else
        memcpy(&buffer[2],buf,len);
   unsigned int tmp=0;
    for(j=0; j<(len+2);j++)
    {
        tmp = tmp+ (unsigned char)buffer[j];
			
    }

            tmp = 0x100-tmp;
			buffer[len+2] = tmp;	


uart1_dma_tx(buffer,len+3);
return 0;

#endif
}

int8_t nibp_if_stop(void)
{
    static uint8_t cmd = 'x';
    if(usart_flag_get(USART1, USART_FLAG_BSY) != SET)
    {
        return -1;
    }
    uart1_dma_tx(&cmd,1);
    //HAL_UART_Transmit_DMA(&huart2, &cmd, 1);
    return 0;
}

static uint8_t buffer1[8];

int8_t nibp_if_cmd_SPEC(uint8_t cmd,uint8_t *pbuf)
{

//    if(usart_flag_get(USART1, USART_FLAG_BSY) != SET)
//    {
//        return -1;
//    }


//    buffer1[0] = 0x3a;

//    sprintf((char *)&buffer1[1], "%02X", cmd);
//    buffer1[3] = pbuf[1];
//    buffer1[4] = pbuf[0];
//    sum_ck = 0;

//    for(i=1; i<5; i++)
//        sum_ck =sum_ck+ buffer1[i];
//    sprintf((char *)&buffer1[5], "%02X", sum_ck);

//    buffer1[7] = 0x03;
//    HAL_UART_Transmit_DMA(&huart2, buffer1, 8);



    return 0;
}

static uint16_t g_pressure = 0;
static uint16_t g_sdp = 0, g_dbp = 0, g_mbp = 0, g_pr = 0;

uint16_t nibp_if_getpressure(void)
{
    return g_pressure;
}

uint8_t ifCheckSum(uint8_t *buffer,uint8_t n)
{
	uint8_t i,chksum=0;
	for(i=0;i<n;i++)
		chksum += buffer[i];
	chksum = 0x100-chksum;
	if(chksum == 00)
		return 0xff;
	else
		return 0;
}

static void nibp_if_sparse_process(void)
{
    uint8_t buffer[128];
    uint16_t len;
    len = __kfifo_len(&rxfifo);
    if(len < 1)
    {
        return;
    }
    __kfifo_out_peek(&rxfifo, buffer, len);
//    if(buffer[0] != 0x3e)
//    {
//        __kfifo_out(&rxfifo, buffer, 1);
//        return;
//    }

		 len = __kfifo_len(&rxfifo);
    if(len >= 5)
    {
        __kfifo_out_peek(&rxfifo, buffer, 5);
        if(buffer[0] == 0x3e && buffer[1] == 0x05 && ifCheckSum(buffer,buffer[1]) == 0xff)//袖带压
        {
            __kfifo_out(&rxfifo, buffer, 5);
            char buf[4];
           // buf[0] = buffer[3];
           // buf[1] = buffer[2];
           // buf[2] =0;
           // buf[3] = 0;
            g_pressure = buffer[3]<<8;
            g_pressure = g_pressure+buffer[2];					
 __kfifo_out(&rxfifo, buffer, buffer[1]);
        }
//        else if(buffer[0] == 0x3e && ifCheckSum(buffer,buffer[1]) == 0xff)//袖带压
//        {
//           

//        }		
    }
     /*len = __kfifo_len(&rxfifo);
   if(len >= 10)
    {
        __kfifo_out_peek(&rxfifo, buffer, 10);
        if(buffer[0] == 0x3e && buffer[1] == 0x18 && ifCheckSum(buffer,buffer[1]) == 0xff)//
        {
            __kfifo_out(&rxfifo, buffer, 0x18);
            char buf[4];
            buf[0] = buffer[2];
            buf[1] = buffer[1];
            buf[2] = 0;
            buf[3] = 0;
            g_pressure = atoi(buf);
        }
    }*/
    len = __kfifo_len(&rxfifo);
    if(len >= 0x18)
    {
        __kfifo_out_peek(&rxfifo, buffer, 0x18);
        if(buffer[0] == 0x3e && buffer[1] == 0x18 && ifCheckSum(buffer,buffer[1]) == 0xff)//系统状态返
        {
            __kfifo_out(&rxfifo, buffer, 0x18);
            int sta, mode, mmode, err, sbp, dbp, mbp, pr;
           // int count = sscanf((char *)&buffer[1], "S%1d;A%1d;C%02d;M%02d;P%03d%03d%03d;R%03d;", \
                               &sta, &mode, &mmode, &err, &sbp, &dbp, &mbp, &pr);
            //printf("%d,%d,%d,%d,%d,%d,%d,%d,%d\n", count, sta, mode, mmode, err, sbp, dbp, mbp, pr);
           // if(count == 8 && (err == 0 || err == 3))
            {  
                g_sdp = buffer[3]<<8;
                g_sdp = g_sdp+buffer[2];	
                g_dbp = buffer[5]<<8;							
                g_dbp = buffer[4]+g_dbp;
                g_mbp = buffer[17]<<8;
                g_mbp = buffer[16]+g_mbp;
                g_pr =  buffer[19]<<8;
                g_pr =  buffer[18]+g_pr;
            }
        }
    }
		    len = __kfifo_len(&rxfifo);
    if(len >= 4)
		{
			 if(buffer[0] == 0x3e && buffer[1] == 0x04&& ifCheckSum(buffer,buffer[1]) == 0xff)//系统状态返
			 {
				 protocol_ack_send(getacksn(), &buffer[2], 1);
				 __kfifo_out(&rxfifo, buffer,  buffer[1]);

			 }
           


   }
}

static void nibp_if_parse_process(void)
{
    uint8_t buffer[128];
    uint16_t len;
    len = __kfifo_len(&rxfifo);
    if(len < 1)
    {
        return;
    }
    __kfifo_out_peek(&rxfifo, buffer, 1);
    if(buffer[0] != 0x02)
    {
        __kfifo_out(&rxfifo, buffer, 1);
        return;
    }
    len = __kfifo_len(&rxfifo);
    if(len >= 6)
    {
        __kfifo_out_peek(&rxfifo, buffer, 6);
        if(buffer[4] == 0x03 && buffer[5] == 0x0D)//测量结束
        {
            __kfifo_out(&rxfifo, buffer, 6);
            nibp_if_cmd(0x18);
        }
    }
    len = __kfifo_len(&rxfifo);
    if(len >= 10)
    {
        __kfifo_out_peek(&rxfifo, buffer, 10);
        if(buffer[8] == 0x03 && buffer[9] == 0x0D)//袖带压返
        {
            __kfifo_out(&rxfifo, buffer, 10);
            char buf[4];
            buf[0] = buffer[1];
            buf[1] = buffer[2];
            buf[2] = buffer[3];
            buf[3] = 0;
            g_pressure = atoi(buf);
        }
    }
    len = __kfifo_len(&rxfifo);
    if(len >= 42)
    {
        __kfifo_out_peek(&rxfifo, buffer, 42);
        if(buffer[40] == 0x03 && buffer[41] == 0x0D)//系统状态返
        {
            __kfifo_out(&rxfifo, buffer, 42);
            int sta, mode, mmode, err, sbp, dbp, mbp, pr;
            int count = sscanf((char *)&buffer[1], "S%1d;A%1d;C%02d;M%02d;P%03d%03d%03d;R%03d;", \
                               &sta, &mode, &mmode, &err, &sbp, &dbp, &mbp, &pr);
            //printf("%d,%d,%d,%d,%d,%d,%d,%d,%d\n", count, sta, mode, mmode, err, sbp, dbp, mbp, pr);
            if(count == 8 && (err == 0 || err == 3))
            {
                g_sdp = sbp;
                g_dbp = dbp;
                g_mbp = mbp;
                g_pr = pr;
            }
        }
    }
}

static void nibp_if_send_process(void)
{
    if(g_pr != 0 && protocol_is_datasend_busy() == 0)
    {
        uint8_t buffer[5];
        buffer[0] = 0x04;
        buffer[1] = g_sdp;
        buffer[2] = g_dbp;
        buffer[3] = g_mbp;
        buffer[4] = g_pr;
        g_pr = 0;
        protocol_data_send(buffer, 5, 2);
    }
}



static int16_t dstpressure = -1;

void nibp_tst_start(uint16_t dst)
{
#if 0
//    dstpressure = dst;
    // HAL_GPIO_WritePin(VALVE_GPIO_Port, VALVE_Pin, GPIO_PIN_RESET);
#else
    dstpressure = dst;

#endif
}

void nibp_tst_stop(void)
{
    //  dstpressure = -1;
    //  HAL_GPIO_WritePin(VALVE_GPIO_Port, VALVE_Pin, GPIO_PIN_SET);
    // __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
}

static void nibp_tst_process(void)
{
//  if(dstpressure == -1)
//  {
//    return;
//  }
//  int32_t pwm;
//  pwm = (dstpressure - MMR_get_pressure() / 100) * 5000;
//  uint16_t pwmmin;
//  pwmmin = 0x2000 + dstpressure * 30;
//  if(pwm < pwmmin)
//  {
//    pwm = pwmmin;
//  }
//  if(pwm > 65535)
//  {
//    pwm = 65535;
//  }
//  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, pwm);
//  if(dstpressure - MMR_get_pressure() / 100 < (-dstpressure / 70))
//  {
//    dstpressure = -1;
//    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
//  }
}

void nibp_tst_valve_set(uint8_t stat)
{
//  if(stat == 1)
//  {
//    HAL_GPIO_WritePin(VALVE_GPIO_Port, VALVE_Pin, GPIO_PIN_SET);
//  }
//  else
//  {
//    HAL_GPIO_WritePin(VALVE_GPIO_Port, VALVE_Pin, GPIO_PIN_RESET);
//  }
}

void nibp_tst_motor_set(uint16_t data)
{
// __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, data);
}

void nibp_if_process(void)
{
    nibp_if_recv_process();
    //nibp_if_parse_process();
	nibp_if_sparse_process();
    nibp_if_send_process();
    nibp_tst_process();
}
















