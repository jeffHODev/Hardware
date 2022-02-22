#include "ecg.h"
#include "string.h"
#include "bsp.h"
#include "kfifo.h"
#include "ADS1292x.h"
#include "sensor.h"
#include "spo.h"

//extern SPI_HandleTypeDef hspi2;
//extern TIM_HandleTypeDef htim2;
//extern TIM_HandleTypeDef htim1;
//extern ADC_HandleTypeDef hadc1;

struct __kfifo ecgfifo;
static uint8_t ecgfifobuf[sizeof(MISCDATA) * 1024];

//static void ecg_write_reg(uint8_t addr, uint8_t data)
//{
//  uint8_t buffer[2];
//  addr &= 0x7F;
//  HAL_GPIO_WritePin(ADS_CSB_GPIO_Port, ADS_CSB_Pin, GPIO_PIN_RESET);
//  buffer[0] = addr;
//  buffer[1] = data;
//  HAL_SPI_TransmitReceive(&hspi2, buffer, buffer, 2, 100);
//  HAL_GPIO_WritePin(ADS_CSB_GPIO_Port, ADS_CSB_Pin, GPIO_PIN_SET);
//}

//static uint8_t ecg_read_reg(uint8_t addr)
//{
//  uint8_t buffer[2];
//  addr |= 0x80;
//  HAL_GPIO_WritePin(ADS_CSB_GPIO_Port, ADS_CSB_Pin, GPIO_PIN_RESET);
//  buffer[0] = addr;
//  buffer[1] = 0xFF;
//  HAL_SPI_TransmitReceive(&hspi2, buffer, buffer, 2, 100);
//  HAL_GPIO_WritePin(ADS_CSB_GPIO_Port, ADS_CSB_Pin, GPIO_PIN_SET);
//  return buffer[1];
//}

//static void ecg_read_regs(uint8_t addr, uint8_t *pdata, uint8_t len)
//{
//  uint8_t buffer[256];
//  addr |= 0x80;
//  HAL_GPIO_WritePin(ADS_CSB_GPIO_Port, ADS_CSB_Pin, GPIO_PIN_RESET);
//  buffer[0] = addr;
//  memset(&buffer[1], 0xFF, sizeof(buffer) - 1);
//  HAL_SPI_TransmitReceive(&hspi2, buffer, buffer, len + 1, 100);
//  HAL_GPIO_WritePin(ADS_CSB_GPIO_Port, ADS_CSB_Pin, GPIO_PIN_SET);
//  memcpy(pdata, &buffer[1], len);
//}

void ecg_init(void)
{
    __kfifo_init(&ecgfifo, ecgfifobuf, sizeof(ecgfifobuf), sizeof(MISCDATA));
//  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
//  HAL_Delay(10);
//  ecg_write_reg(0x00, 0x00);
//  HAL_Delay(10);
//
////    ecg_write_reg(0x06, 0x02);//0x00000010
////    ecg_write_reg(0x07, 0x07);//0x00000111
////    ecg_write_reg(0x08, 0xff);//0x11
//
//
////  ecg_write_reg(0x01, 0x11);//0x11
////  ecg_write_reg(0x0A, 0x03);
////  ecg_write_reg(0x0C, 0x03);
////  ecg_write_reg(0x12, 0x06);
////  ecg_write_reg(0x14, 0x36);
////  ecg_write_reg(0x21, 0x02);
////  ecg_write_reg(0x22, 0x02);
////  ecg_write_reg(0x23, 0x02);
////  ecg_write_reg(0x27, 0x08);
////  ecg_write_reg(0x2F, 0x10);
////  ecg_write_reg(0x00, 0x01);
//    ecg_write_reg(0x01, 0x11);//0x11 Negative terminal connected to input IN1 010: Positive terminal connected to input IN2
//  ecg_write_reg(0x0A, 0x03);
//  ecg_write_reg(0x0C, 0x03);//Right-leg drive output connected to IN3
//  ecg_write_reg(0x12, 0x06);//Shut down internal oscillator and use external clock from CLK pin
//  ecg_write_reg(0x14, 0x00);//Shut down the sigma-delta modulator for Channel 3 Shut down the instrumentation amplifier for Channel 2
//  ecg_write_reg(0x21, 0x04);//R2 decimation rate 0x04 0x08
//  ecg_write_reg(0x22, 0x20);//R3 0x20
//  ecg_write_reg(0x23, 0x20);//R3 decimation rate for channel 2 6
//  ecg_write_reg(0x26, 0x00);//
//  ecg_write_reg(0x25, 0x00);//

//  ecg_write_reg(0x27, 0x08);//Driven by Channel 3 ECG
//  ecg_write_reg(0x2F, 0x10);//Enable DATA_CH1_ECG read back
//  ecg_write_reg(0x00, 0x01);//Conversion active
//  //ecg_enable(1);
//}
//uint8_t getRegister(uint8_t addr)
//{   ecg_write_reg(0x06, 0x02);//0x00000010
//    ecg_write_reg(0x07, 0x07);//0x00000111
//    ecg_write_reg(0x08, 0xff);//0x11
//  return  ecg_read_reg(addr);
//}
//uint8_t WriteRegister(uint8_t addr, uint8_t data)
//{
//  ecg_write_reg(addr,data);
}
void ecg_enable(uint8_t enable)
{
    if(enable)
    {
        dma_single_data_parameter_struct dma_single_data_parameter;
		//RCU_INT = 0;
        timer_interrupt_enable(TIMER3, TIMER_INT_UP); //update interrupt
		timer_enable(TIMER3);
		NVIC_EnableIRQ(TIMER3_IRQn);

        dma_deinit(DMA1, DMA_CH0);

        /* initialize DMA single data mode */
        dma_single_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
        dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
        dma_single_data_parameter.memory0_addr = (uint32_t)(getAdcBuf()->adc_value);
        dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
        dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
        dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;
        dma_single_data_parameter.number = 6;
        dma_single_data_parameter.priority = DMA_PRIORITY_HIGH;
        dma_single_data_mode_init(DMA1, DMA_CH0, &dma_single_data_parameter);
        dma_channel_subperipheral_select(DMA1, DMA_CH0, DMA_SUBPERI0);

        /* enable DMA circulation mode */
        dma_circulation_enable(DMA1, DMA_CH0);

        /* enable DMA channel */
        adc_config();

	//	RCU_INT = 1;



//
    }
    else

    {
        //adc_disable(ADC0);
        dma_channel_disable(DMA1, DMA_CH0);
        adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
        timer_interrupt_disable(TIMER3, TIMER_INT_UP); //update interrupt
    }
}
uint8_t error_flag;
uint32_t err_cnt;


extern __align(4) u8 data_to_send[76];
 
void HAL_TIM_PeriodElapsedCallback()
{
    static uint16_t adcbuf[3];
    uint8_t i=0,j;
    MISCDATA miscdata;
    uint32_t *buffer;
// uint8_t buffer[3];
    //error_flag =ecg_read_reg(0x19);
    if(error_flag ==0)
        ;//ecg_read_regs(0x50, buffer, 3);
    else
        err_cnt = err_cnt+1;
    buffer = get_ads();
    for(i=0; i<ADS_CHANNEL; i++)
    { 
			  j = data_to_send[6+i*4];
        miscdata.data[i*4+0] = data_to_send[4+i*4];
        miscdata.data[i*4+1] = data_to_send[5+i*4];
        miscdata.data[i*4+2] = data_to_send[6+i*4];
        miscdata.data[i*4+3] = data_to_send[7+i*4];
			

    }
    i=64;
    //data = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
    //data = (data - 0xB964F0 / 2) * 4 * 240000 / 0xB964F0 / 7;//0.01mV; 0.11287477uV / LSB
//    miscdata.data[i++] = (uint8_t)(buffer[0]>>16);//ecg
//    miscdata.data[i++] =(uint8_t)(buffer[0]>>8);
//    miscdata.data[i++] = (uint8_t)(buffer[0]);

//    miscdata.data[i++] = (uint8_t)(buffer[1]>>16);//eeg
//    miscdata.data[i++] =(uint8_t)(buffer[1]>>8);
//    miscdata.data[i++] = (uint8_t)(buffer[1]);

//    miscdata.data[i++] = (uint8_t)(buffer[2]>>16);//emg
//    miscdata.data[i++] =(uint8_t)(buffer[2]>>8);
//    miscdata.data[i++] = (uint8_t)(buffer[2]);


    /* miscdata.data[9] = (uint8_t)getSensor(getAdcBuf()->adc_value[5]>>8);//hea
     miscdata.data[10] =(uint8_t)( getAdcBuf()->adc_value[5]);


     miscdata.data[11] = (uint8_t)(*getSpo());//spo;
     miscdata.data[12] = (uint8_t)(*(getSpo()+1));//p;
     miscdata.data[13] = (uint8_t)(getAdcBuf()->adc_value[0]>>8);//t;
     miscdata.data[14] = (uint8_t)(getAdcBuf()->adc_value[1]>>8);//t;

     miscdata.data[15] = (uint8_t)(getAdcBuf()->adc_value[4]>>8);//br;
     miscdata.data[16] = (uint8_t)(getAdcBuf()->adc_value[4]>>8);//br;

     miscdata.data[15] = (uint8_t)(getAdcBuf()->adc_value[4]>>8);//br;
     miscdata.data[16] = (uint8_t)(getAdcBuf()->adc_value[4]>>8);//br;*/



    miscdata.data[i++] = (uint8_t)(getAdcBuf()->adc_value[5]>>8);//hea
    miscdata.data[i++] =(uint8_t)( getAdcBuf()->adc_value[5]);
    miscdata.data[i++] = (uint8_t)(getSensor()->tst>>8);//spo;
    miscdata.data[i++] = (uint8_t)(getSensor()->tst);//p;


    miscdata.data[i++] = (uint8_t)(*getSpo());//spo;
    miscdata.data[i++] = (uint8_t)(*(getSpo()+1));//p;
    miscdata.data[i++] = (uint8_t)(getSensor()->temperature>>8);//t;
    miscdata.data[i++] = (uint8_t)(getSensor()->temperature);//t;

    miscdata.data[i++] = (uint8_t)(getSensor()->br>>8);//br;
    miscdata.data[i++] = (uint8_t)(getSensor()->br);//br;
    miscdata.data[i++] = (uint8_t)(getSensor()->vbat);//br;
    __kfifo_in(&ecgfifo, &miscdata, 1);//
    adc_start();
    //HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcbuf, 3);
}


void TIMER3_IRQHandler(void)
{
    if(SET == timer_interrupt_flag_get(TIMER3, TIMER_INT_FLAG_UP))
    {
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
        HAL_TIM_PeriodElapsedCallback();
    }
}




//    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_10, ADC_SAMPLETIME_15);//ta
//    adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_11, ADC_SAMPLETIME_15);//tb
//    adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_13, ADC_SAMPLETIME_15);//vbat
//    adc_regular_channel_config(ADC0, 3, ADC_CHANNEL_14, ADC_SAMPLETIME_15);//spo
//    adc_regular_channel_config(ADC0, 4, ADC_CHANNEL_15, ADC_SAMPLETIME_15);//br
//    adc_regular_channel_config(ADC0, 5, ADC_CHANNEL_4, ADC_SAMPLETIME_15);//hea







