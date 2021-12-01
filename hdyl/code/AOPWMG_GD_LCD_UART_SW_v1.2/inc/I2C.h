
//#include "hmi_user_uart.h"
/*********************************************************/
//sbit     SCL=P0^1;     //I2C  Ê±ÖÓ 
//sbit     SDA=P0^2;     //I2C  Êý¾Ý 
//==============================================
#define SLA_WR	0xee//0x77

extern unsigned char ack;
void Start_I2c(void);
void Stop_I2c(void);
void  I2CSendByte(unsigned char  c);
unsigned char   I2CRcvByte(void);
void Ack_I2c(unsigned char a);

unsigned char ISendByte(unsigned char sla,unsigned char *p,unsigned char len);
unsigned char IRcvByte(unsigned char sla);

