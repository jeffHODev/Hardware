#include "modbus.h"
#include "crc.h"
#include "sensor.h"
unsigned char Modbus_03_Solve(void);
unsigned char Modbus_06_Solve(void);
unsigned char Modbus_16_Solve(void);

modbus_stru modbus_usr;
extern UART_HandleTypeDef huart6;
unsigned int startRegAddr;
unsigned int RegNum;
unsigned int calCRC;
//modbus_pack_stru  modbus_pack_usr;

void RS485_Init()
{

   #if CPU == ST
    HAL_GPIO_WritePin(EN1_485_GPIO_Port, EN1_485_Pin, GPIO_PIN_RESET);
    modbus_usr.RS485_TX_EN=0;//Ĭ��Ϊ����ģʽ
    __HAL_UART_CLEAR_IDLEFLAG(&huart6);
    __HAL_UART_DISABLE_IT(&huart6, UART_IT_IDLE);	//ʹ�ܿ����ж�
    HAL_UART_DMAStop(&huart6);
    HAL_UART_DMAResume(&huart6);
    __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);	//ʹ�ܿ����ж�
    HAL_UART_Receive_DMA(&huart6,modbus_usr.RS485_RX_BUFF,MODBUS_BSIZE);
   #else

	usart_interrupt_enable(USART6, USART_INT_IDLE);
		usart_flag_clear(USART6, USART_INT_IDLE);
		usart_dma_receive_config(USART6, USART_DENR_ENABLE);
		nvic_irq_enable(DMA2_Channel5_IRQn, 0, 0);
		
		
		dma_single_data_parameter_struct dma_init_struct;
		
		
		/* deinitialize DMA1 channel2 (USART0 rx) */
		dma_deinit(DMA2, DMA_CH5);
		dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
		dma_init_struct.memory0_addr = (uint32_t)modbus_usr.RS485_RX_BUFF;
		dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
		dma_init_struct.number = MODBUS_BSIZE;
		dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART6);
		dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
		dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
		dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
		dma_single_data_mode_init(DMA2, DMA_CH5, &dma_init_struct);
		
		/* configure DMA mode */
		dma_circulation_disable(DMA2, DMA_CH5);
		dma_channel_subperipheral_select(DMA1, DMA_CH5, DMA_SUBPERI4);
		/* enable DMA1 channel2 transfer complete interrupt */
		dma_interrupt_enable(DMA2, DMA_CH5, DMA_CHXCTL_FTFIE);
		/* enable DMA1 channel2 */
		dma_channel_enable(DMA2, DMA_CH5);
   #endif
	
}
//////////////////////////////////////////////////////////////////////////////
//����n���ֽ�����
//buff:�������׵�ַ
//len�����͵��ֽ���
void RS485_SendData(u8 *buff,u8 len,unsigned char flag)
{
    unsigned int i;
   #if CPU == ST
    if(flag == 0)
        HAL_GPIO_WritePin(EN1_485_GPIO_Port, EN1_485_Pin, GPIO_PIN_SET);;
    modbus_usr.RS485_TX_EN=1;//�л�Ϊ����ģʽ
       HAL_UART_Transmit(&huart6,buff,len,300);
    HAL_GPIO_WritePin(EN1_485_GPIO_Port, EN1_485_Pin, GPIO_PIN_RESET);
   #else
   if(flag == 0)
   	
   /*Configure GPIO pin Output Level */
	  gpio_bit_set(EN1_485_GPIO_Port, EN1_485_Pin);
	 
   modbus_usr.RS485_TX_EN=1;//�л�Ϊ����ģʽ
   i=0;
   while(len--)
   usart_data_transmit(USART6, buff[i++]);
   gpio_bit_reset(EN1_485_GPIO_Port, EN1_485_Pin);
   #endif 
}
void Modbus_Pack(modbus_pack_stru p)
{


    unsigned char index;
    index = 0;



    if(p.func == 0x03)
    {
        modbus_usr.RS485_TX_BUFF[index++]=p.RS485_Addr;
        modbus_usr.RS485_TX_BUFF[index++]=p.func;
        modbus_usr.RS485_TX_BUFF[index++]=(u8)(p.startaddr>>8);
        modbus_usr.RS485_TX_BUFF[index++]=(u8)(p.startaddr);
        modbus_usr.RS485_TX_BUFF[index++]=(u8)(p.regnum>>8);
        modbus_usr.RS485_TX_BUFF[index++]=(u8)(p.regnum);
    }
    else
    {
        if(p.datType == FLOAT_TYPE)
        {
            modbus_usr.RS485_TX_BUFF[index++]=p.RS485_Addr;
            modbus_usr.RS485_TX_BUFF[index++]=p.func;
            modbus_usr.RS485_TX_BUFF[index++]=(u8)(p.startaddr>>8);
            modbus_usr.RS485_TX_BUFF[index++]=(u8)(p.startaddr);
            modbus_usr.RS485_TX_BUFF[index++]=(u8)(p.regnum>>8);
            modbus_usr.RS485_TX_BUFF[index++]=(u8)(p.regnum);
        }
        else
        {
            modbus_usr.RS485_TX_BUFF[index++]=p.RS485_Addr;
            modbus_usr.RS485_TX_BUFF[index++]=p.func;
            modbus_usr.RS485_TX_BUFF[index++]=(u8)(p.startaddr>>8);
            modbus_usr.RS485_TX_BUFF[index++]=(u8)(p.startaddr);

        }
    }

    unsigned char len;
    len = index;
    if(p.datcount)
    {
        modbus_usr.RS485_TX_BUFF[index++]=(u8)(p.datcount);
        memcpy(&modbus_usr.RS485_TX_BUFF[index++],p.modbus_txdata,p.datcount);
        len = len + p.datcount;

    }
    else
    {

        calCRC=CRC_Compute(modbus_usr.RS485_TX_BUFF,len);
        modbus_usr.RS485_TX_BUFF[len+1]=(calCRC)&0xFF;
        modbus_usr.RS485_TX_BUFF[len+2]=(calCRC>>8)&0xFF;
    }


    RS485_SendData(modbus_usr.RS485_TX_BUFF,len+2,0);
}

/*************************************************************
485���ڽ����жϻص�����
*************************************************************/
void RS485_RxCpltCallback()
{
    modbus_usr.RS485_FrameFlag=2;//��λ֡�������
    HAL_UART_DMAStop(&huart6);
    HAL_UART_DMAResume(&huart6);
    HAL_UART_Receive_DMA(&huart6,modbus_usr.RS485_RX_BUFF,MODBUS_BSIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
//RS485����������ڴ�����յ�������(������������ѭ������)

unsigned char RS485_Service(void)
{
    u16 recCRC;
    if(modbus_usr.RS485_FrameFlag==2)
    {
        HAL_GPIO_WritePin(EN1_485_GPIO_Port, EN1_485_Pin, GPIO_PIN_RESET);
        if(modbus_usr.RS485_RX_BUFF[0]>= MIN_ADDR &&
                modbus_usr.RS485_RX_BUFF[0]<= MAX_ADDR)//��ַ��ȷ
        {
            if((modbus_usr.RS485_RX_BUFF[1]==03)||(modbus_usr.RS485_RX_BUFF[1]==06)||(modbus_usr.RS485_RX_BUFF[1]==16))//��������ȷ
            {
                modbus_usr.RS485_RX_CNT = modbus_usr.RS485_RX_BUFF[2]+5;
                calCRC=CRC_Compute(modbus_usr.RS485_RX_BUFF,modbus_usr.RS485_RX_CNT-2);//�������������ݵ�CRC
                recCRC=modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT-2]|(((u16)modbus_usr.RS485_RX_BUFF[modbus_usr.RS485_RX_CNT-1])<<8);//���յ���CRC(���ֽ���ǰ�����ֽ��ں�)
                if(calCRC==recCRC)//CRCУ����ȷ
                {
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    switch(modbus_usr.RS485_RX_BUFF[1])//���ݲ�ͬ�Ĺ�������д���
                    {
                    case 3: //���Ĵ���
                    {
                        Modbus_03_Solve();
                        break;
                    }
                    case 6: //���Ĵ���
                    {
                        Modbus_06_Solve();
                        break;
                    }

                    case 16: //���Ĵ���
                    {
                        Modbus_16_Solve();
                        break;
                    }



                    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                }

            }
        }
    }
    return 1;

}
//return  0                                  2             3
//       orp or ph or temperature      warn or adc
unsigned char Modbus_03_Solve()
{
    unsigned char result;

  //  if(GetModbusPack()->datType == FLOAT_TYPE)
    {
        if(modbus_usr.RS485_RX_BUFF[2] == 4)//float type
        {
            GetModbusPack()->datType = FLOAT_TYPE;
            GetModbusPack()->modbus_data[0] = modbus_usr.RS485_RX_BUFF[5];
            GetModbusPack()->modbus_data[1] = modbus_usr.RS485_RX_BUFF[6];
            GetModbusPack()->modbus_data[2] = modbus_usr.RS485_RX_BUFF[3];
            GetModbusPack()->modbus_data[3] = modbus_usr.RS485_RX_BUFF[4];
            result = 0;
        }
        else
            result = 1;
    }
  //  else
    {
        if(modbus_usr.RS485_RX_BUFF[2] == 2)//unsigned int type
        {
            GetModbusPack()->datType = UINT_TYPE;
            GetModbusPack()->modbus_data[0] = modbus_usr.RS485_RX_BUFF[3];
            GetModbusPack()->modbus_data[1] = modbus_usr.RS485_RX_BUFF[4];
            result = 2;
        }
        else
            result = 1;
    }
    return result;

}

unsigned char Modbus_06_Solve()
{
    return 1;
}
unsigned char Modbus_16_Solve()
{
    return 1;

}
