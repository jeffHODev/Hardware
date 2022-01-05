#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "main.h"
#include "kfifo.h"

void protocol_init(void);
void protocol_process(void);
uint8_t protocol_data_send(uint8_t *pdata, uint16_t len, uint8_t retry);
uint8_t protocol_is_datasend_busy(void);
uint8_t *getBleUartBuf(void);
void uart3_rx_config(void);










#endif








