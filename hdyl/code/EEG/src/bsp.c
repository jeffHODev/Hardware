#include "bsp.h"
#include "main.h"
#include "ble_set.h"
bsp_sru bsp_usr;
spi_sru spi_usr;
void  light_Ired(void)
{
    /* OUT1_0;
     OUT2_1;
     OUT3_1;
     OUT4_0;	*/ //红光
    gpio_bit_write(OUT1_GPIO_Port, OUT1_Pin, RESET);
    gpio_bit_write(OUT2_GPIO_Port, OUT2_Pin, SET);
    gpio_bit_write(OUT3_GPIO_Port, OUT3_Pin, SET);
    gpio_bit_write(OUT4_GPIO_Port, OUT4_Pin, RESET);
}
//打开红外光
void  light_red(void)
{
    /* OUT1_1;
     OUT2_0;
     OUT3_0;
     OUT4_1;*/	 //红外光
    gpio_bit_write(OUT1_GPIO_Port, OUT1_Pin, SET);
    gpio_bit_write(OUT2_GPIO_Port, OUT2_Pin, RESET);
    gpio_bit_write(OUT3_GPIO_Port, OUT3_Pin, RESET);
    gpio_bit_write(OUT4_GPIO_Port, OUT4_Pin, SET);

}
//光熄灭
void  light_stop(void)
{

    /* OUT1_1;
     OUT2_1;
     OUT3_0;
     OUT4_0;	*/ //停止
    gpio_bit_write(OUT1_GPIO_Port, OUT1_Pin, SET);
    gpio_bit_write(OUT2_GPIO_Port, OUT2_Pin, SET);
    gpio_bit_write(OUT3_GPIO_Port, OUT3_Pin, RESET);
    gpio_bit_write(OUT4_GPIO_Port, OUT4_Pin, RESET);

}

bsp_sru *getAdcBuf()
{
    return &bsp_usr;
}
spi_sru *getSpiBuf()
{
    return &spi_usr;
}
void adc_start()
{
    /* enable ADC software trigger */
	dma_channel_enable(DMA1, DMA_CH0);
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);


}
void power_manage(unsigned char pwr,unsigned char ctrl)
{
 
    if(ctrl == OFF)
    {

		switch(pwr)
		{
		case  BLE_PWR:gpio_bit_write(BLE_EN_GPIO_Port, BLE_EN_Pin, RESET);
		gpio_bit_write(BRST_GPIO_Port, BRST_Pin, RESET);delay_ms(100);
		gpio_bit_write(BRST_GPIO_Port, BRST_Pin, SET);
			break;
		case  EN_3V3_PWR:gpio_bit_write(POW_EN_3V3_GPIO_Port, POW_EN_3V3_Pin, RESET);
			break;
		case  EN_5V_PWR:gpio_bit_write(POW_EN_5V_GPIO_Port, POW_EN_5V_Pin, RESET);
			break;
		case  PRESS__PWR:gpio_bit_write(PRESS_EN_GPIO_Port, PRESS_EN_Pin, RESET);
			break;
		case  POW_HEA_PWR:gpio_bit_write(POW_HEA_GPIO_Port, POW_HEA_Pin, RESET);
			break;
		default:
			//gpio_bit_write(BLE_EN_GPIO_Port, BLE_EN_Pin, RESET);
			gpio_bit_write(POW_EN_3V3_GPIO_Port, POW_EN_3V3_Pin, RESET);
			gpio_bit_write(POW_EN_5V_GPIO_Port, POW_EN_5V_Pin, RESET);
			gpio_bit_write(PRESS_EN_GPIO_Port, PRESS_EN_Pin, RESET);
			gpio_bit_write(POW_HEA_GPIO_Port, POW_HEA_Pin, RESET);

			break;
		
		}
    }
    else
    {
		switch(pwr)
		{
		case  BLE_PWR:gpio_bit_write(BLE_EN_GPIO_Port, BLE_EN_Pin, SET);
			gpio_bit_write(BRST_GPIO_Port, BRST_Pin, RESET);delay_ms(100);
		gpio_bit_write(BRST_GPIO_Port, BRST_Pin, SET);		
			break;
		case  EN_3V3_PWR:gpio_bit_write(POW_EN_3V3_GPIO_Port, POW_EN_3V3_Pin, SET);
			break;
		case  EN_5V_PWR:gpio_bit_write(POW_EN_5V_GPIO_Port, POW_EN_5V_Pin, SET);
			break;
		case  PRESS__PWR: gpio_bit_write(PRESS_EN_GPIO_Port, PRESS_EN_Pin, SET);
			break;
		case  POW_HEA_PWR:
			gpio_bit_write(POW_HEA_GPIO_Port, POW_HEA_Pin, SET);break;
		default: 
				gpio_bit_write(BLE_EN_GPIO_Port, BLE_EN_Pin, SET);
				gpio_bit_write(POW_EN_3V3_GPIO_Port, POW_EN_3V3_Pin, SET);
				gpio_bit_write(POW_EN_5V_GPIO_Port, POW_EN_5V_Pin, SET);
				gpio_bit_write(PRESS_EN_GPIO_Port, PRESS_EN_Pin, SET);
				gpio_bit_write(POW_HEA_GPIO_Port, POW_HEA_Pin, SET);
				break;
		
		}

    }
}
void chip_sel(unsigned char spix_chipx,unsigned char oper)
{
    if(oper == 0)
    {
        if(spix_chipx == 1)
        {
            gpio_bit_write(ADS129x_CS2_GPIO_Port, ADS129x_CS2_Pin, SET);
            gpio_bit_write(ADS129x_CS_GPIO_Port, ADS129x_CS_Pin, RESET);



        }
        else
        {
            gpio_bit_write(ADS129x_CS2_GPIO_Port, ADS129x_CS2_Pin, RESET);
             gpio_bit_write(ADS129x_CS_GPIO_Port, ADS129x_CS_Pin, SET);

        }

    }
    else if(oper == 1)
    {
        if(spix_chipx == 2)
        {
            gpio_bit_write(ADS129x_CS2_GPIO_Port, ADS129x_CS2_Pin, SET);
            //gpio_bit_write(ADS129x_CS_GPIO_Port, ADS129x_CS_Pin, RESET);



        }
        else
        {
            //gpio_bit_write(ADS129x_CS2_GPIO_Port, ADS129x_CS2_Pin, RESET);
            gpio_bit_write(ADS129x_CS_GPIO_Port, ADS129x_CS_Pin, SET);

        }

    }



}
u8 spi_TransmitReceive(uint16_t buf)
{

//    uint16_t i ;
   // i = 0;
    /*if(spix_chipx == 1)
    {
        gpio_bit_write(ADS129x_CS2_GPIO_Port, ADS129x_CS2_Pin, SET);
        gpio_bit_write(ADS129x_CS_GPIO_Port, ADS129x_CS_Pin, RESET);



    }
    else
    {
        gpio_bit_write(ADS129x_CS2_GPIO_Port, ADS129x_CS2_Pin, RESET);
        gpio_bit_write(ADS129x_CS_GPIO_Port, ADS129x_CS_Pin, SET);

    }*/
    //while(i<len)
    {

        while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE));
        spi_i2s_data_transmit(SPI1, buf);
        while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE));
        return  spi_i2s_data_receive(SPI1);
    }
    //gpio_bit_write(ADS129x_CS2_GPIO_Port, ADS129x_CS2_Pin, SET);
   // gpio_bit_write(ADS129x_CS_GPIO_Port, ADS129x_CS_Pin, SET);
}
void spi_receive(unsigned char spix_chipx,uint32_t len,uint16_t buf)
{

    uint16_t i ;
    i = 0;
    if(spix_chipx == 1)
    {
        gpio_bit_write(ADS129x_CS2_GPIO_Port, ADS129x_CS2_Pin, SET);
        gpio_bit_write(ADS129x_CS_GPIO_Port, ADS129x_CS_Pin, RESET);
    }
    else
    {
        gpio_bit_write(ADS129x_CS2_GPIO_Port, ADS129x_CS2_Pin, RESET);
        gpio_bit_write(ADS129x_CS_GPIO_Port, ADS129x_CS_Pin, SET);

    }
    while(i<len)
    {

        while(RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE));
        spi_usr.rx_array[i++] = spi_i2s_data_receive(SPI1);
    }
    gpio_bit_write(ADS129x_CS2_GPIO_Port, ADS129x_CS2_Pin, SET);
    gpio_bit_write(ADS129x_CS_GPIO_Port, ADS129x_CS_Pin, SET);
}

void SendStr(uint8_t *str)
{
    unsigned char i,len;
    i = 0;
    len = strlen(str);
    //HAL_UART_Transmit(&huart1, str, len, 1000);
    while(i<len)
    {
        usart_data_transmit(UART3, str[i++]);
        while(RESET == usart_flag_get(UART3, USART_FLAG_TBE));
    }
}

void uart3_dma_tx(uint32_t *pb,uint32_t len)
{
    dma_single_data_parameter_struct dma_single_data_parameter;
  uint8_t *pbuf;

  


   
	dma_flag_clear(DMA0,DMA_CH4,DMA_INTF_FTFIF);
	NVIC_EnableIRQ(DMA0_Channel4_IRQn);
	dma_interrupt_flag_clear(DMA0, DMA_CH4, DMA_INT_FLAG_FTF);
	dma_channel_disable(DMA0, DMA_CH4);
	dma_memory_address_config(DMA0, DMA_CH4,DMA_MEMORY_0, &pb[0]); //设置要发送数据的内存地址
	dma_transfer_number_config(DMA0, DMA_CH4,  len);					   //一共发多少个数据
	//dma_transfer_number_config(DMA0, DMA_CH4,  2);
	
	dma_channel_enable(DMA0, DMA_CH4);
	/* USART DMA enable for transmission and reception */
	usart_dma_transmit_config(UART3, USART_DENT_ENABLE); //使能串口DMA发送


}
void uart1_dma_tx(uint32_t *pb,uint32_t len)
{
    dma_single_data_parameter_struct dma_single_data_parameter;

    rcu_periph_clock_enable(RCU_DMA0);

    /* wait DMA Channel transfer complete */
	usart_dma_transmit_config(USART1, USART_DENT_DISABLE); //使能串口DMA发送
    dma_interrupt_flag_clear(DMA0, DMA_CH6, DMA_INT_FLAG_FTF);
	dma_flag_clear(DMA0,DMA_CH6,DMA_INTF_FTFIF);
	NVIC_EnableIRQ(DMA0_Channel6_IRQn);
	dma_interrupt_flag_clear(DMA0, DMA_CH6, DMA_INT_FLAG_FTF);
	dma_channel_disable(DMA0, DMA_CH6);
	dma_memory_address_config(DMA0, DMA_CH6,DMA_MEMORY_0, &pb[0]); //设置要发送数据的内存地址
	dma_transfer_number_config(DMA0, DMA_CH6,  len);					   //一共发多少个数据
	//dma_transfer_number_config(DMA0, DMA_CH4,  2);
	
	dma_channel_enable(DMA0, DMA_CH6);
	/* USART DMA enable for transmission and reception */
	usart_dma_transmit_config(USART1, USART_DENT_ENABLE); //使能串口DMA发送

}
void power_sleep(void)
{
	power_manage(5,OFF);
	BleConSeting(SLEEP);
	nvic_irq_disable(EXTI10_15_IRQn);
	nvic_irq_disable(EXTI1_IRQn);


    /*rcu_periph_clock_enable(RCU_PMU);
    /* 
    gd_eval_key_init(KEY_WAKEUP,KEY_MODE_GPIO);

    gd_eval_key_init(KEY_TAMPER,KEY_MODE_EXTI);
     if(RESET == gpio_input_bit_get(WAKEUP_KEY_GPIO_PORT, WAKEUP_KEY_PIN))
     pmu_to_deepsleepmode(PMU_LDO_LOWPOWER, WFI_CMD);*/


	
}


