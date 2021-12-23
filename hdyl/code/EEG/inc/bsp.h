#ifndef BSP_H
#define BSP_H
#include "main.h"
#define ADC_SIZE 6
#define ADC_SIZE 6
#define SPI_TX_SIZE 16
#define SPI_RX_SIZE 16


typedef struct
{
 uint32_t adc_value[ADC_SIZE];
}bsp_sru;
typedef struct
{
 unsigned char tx_array[SPI_TX_SIZE];
  unsigned char rx_array[SPI_RX_SIZE];
}spi_sru;
u8 spi_TransmitReceive(uint16_t buf);
void chip_sel(unsigned char spix_chipx,unsigned char oper);
void  light_red(void);
void  light_Ired(void);
void  light_stop(void);

bsp_sru *getAdcBuf(void);
spi_sru *getSpiBuf(void);
void SendStr(uint8_t *str);
void power_manage(unsigned char ctrl);

#endif
