
 /*************************此部分为I2C总线的驱动程序*************************************/
//#include<reg52.h>
//#include <intrins.h>
#include "main.h"
//#include "hw_config.h"
#include "I2C.h"
//#include "LCD_Config.h"

#if CPU == ST
#define SDA_SET()		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET)
#define SDA_CLR()		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET)
#define SDA_read		HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)

#define SCL_SET()		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)
#define SCL_CLR()		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)

#else
#define SDA_SET()		gpio_bit_write(GPIOB, GPIO_PIN_15, GPIO_PIN_SET)
#define SDA_CLR()		gpio_bit_write(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET)
#define SDA_read		gpio_input_bit_get(GPIOB, GPIO_PIN_15)

#define SCL_SET()		gpio_bit_write(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)
#define SCL_CLR()		gpio_bit_write(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#endif
//#define  NOP()   _nop_()   /* 定义空指令 */
//#define  _Nop()  _nop_()   /*定义空指令*/
void _Nop()
{
	unsigned int i=500;
	while(--i);
} 

 
//sbit     SCL=P2^0;     //I2C  时钟 
//sbit     SDA=P2^1;     //I2C  数据 

unsigned char ack;                 /*应答标志位*/
   

/*******************************************************************
                     起动总线函数               
函数原型: void  Start_I2c();  
功能:     启动I2C总线,即发送I2C起始条件.  
********************************************************************/
void Start_I2c(void)
{
  SDA_SET();         /*发送起始条件的数据信号*/
  _Nop();
  SCL_SET();
  _Nop();        /*起始条件建立时间大于4.7us,延时*/
  _Nop();
  _Nop();
  _Nop();
  _Nop();    
  SDA_CLR();         /*发送起始信号*/
  _Nop();        /* 起始条件锁定时间大于4μs*/
  _Nop();
  _Nop();
  _Nop();
  _Nop();       
  SCL_CLR();       /*钳住I2C总线，准备发送或接收数据 */
  _Nop();
  _Nop();
}

/*******************************************************************
                      结束总线函数               
函数原型: void  Stop_I2c();  
功能:     结束I2C总线,即发送I2C结束条件.  
********************************************************************/
void Stop_I2c(void)
{
  SDA_CLR();      /*发送结束条件的数据信号*/
  _Nop();       /*发送结束条件的时钟信号*/
  SCL_SET();      /*结束条件建立时间大于4μs*/
  _Nop();
  _Nop();
  _Nop();
  _Nop();
  _Nop();
  SDA_SET();      /*发送I2C总线结束信号*/
  _Nop();
  _Nop();
  _Nop();
  _Nop();
}

/*******************************************************************
                 字节数据发送函数               
函数原型: void  SendByte(UCHAR c);
功能:     将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
          此状态位进行操作.(不应答或非应答都使ack=0)     
           发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************/
void  SendByte(unsigned char  c)
{
 unsigned char  BitCnt;
 
 for(BitCnt=0;BitCnt<8;BitCnt++)  /*要传送的数据长度为8位*/
    {
     if((c<<BitCnt)&0x80)SDA_SET();   /*判断发送位*/
       else  SDA_CLR();                
     _Nop();
     SCL_SET();             /*置时钟线为高，通知被控器开始接收数据位*/
      _Nop(); 
      _Nop();             /*保证时钟高电平周期大于4μs*/
      _Nop();
      _Nop();
      _Nop();         
     SCL_CLR(); 
    }
    
    _Nop();
    _Nop();
    SDA_SET();               /*8位发送完后释放数据线，准备接收应答位*/
    _Nop();
    _Nop();   
    SCL_SET();
    _Nop();
    _Nop();
    _Nop();
    if(SDA_read)ack=0;     
       else ack=1;        /*判断是否接收到应答信号*/
    SCL_CLR();
    _Nop();
    _Nop();
}

/*******************************************************************
                 字节数据接收函数               
函数原型: UCHAR  RcvByte();
功能:        用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
          发完后请用应答函数应答从机。  
********************************************************************/    
unsigned char   RcvByte()
{
  unsigned char  retc;
  unsigned char  BitCnt;
  
  retc=0; 
  SDA_SET();                       /*置数据线为输入方式*/
  for(BitCnt=0;BitCnt<8;BitCnt++)
      {
        _Nop();           
        SCL_CLR();                  /*置时钟线为低，准备接收数据位*/
        _Nop();
        _Nop();                 /*时钟低电平周期大于4.7μs*/
        _Nop();
        _Nop();
        _Nop();
        SCL_SET();                  /*置时钟线为高使数据线上数据有效*/
        _Nop();
        _Nop();
        retc=retc<<1;
        if(SDA_read)retc=retc+1;  /*读数据位,接收的数据位放入retc中 */
        _Nop();
        _Nop(); 
      }
  SCL_CLR();   
  _Nop();
  _Nop();
  return(retc);
}

/********************************************************************
                     应答子函数
函数原型:  void Ack_I2c(bit a);
功能:      主控器进行应答信号(可以是应答或非应答信号，由位参数a决定)
********************************************************************/
void Ack_I2c(unsigned char a)
{
  
  if(a==0)SDA_CLR();              /*在此发出应答或非应答信号 */
  else SDA_SET();
  _Nop();
  _Nop();
  _Nop();      
  SCL_SET();
  _Nop();
  _Nop();                    /*时钟低电平周期大于4μs*/
  _Nop();
  _Nop();
  _Nop();  
  SCL_CLR();                     /*清时钟线，钳住I2C总线以便继续接收*/
  _Nop();
  _Nop();    
}


/*******************************************************************
发送字节[命令]数据函数               
*******************************************************************/
unsigned char ISendByte(unsigned char sla,unsigned char *p,unsigned char len)
{
	unsigned char i;
   Start_I2c();              //启动总线
   SendByte(sla);            //发送器件地址
   if(ack==0)return(0);
   for(i=0;i< len;i++)
   {
   	 SendByte(*p);              //发送数据
	 p++;
   }
   if(ack==0)return(0);
   Stop_I2c();               //结束总线
   return(1);
}

/*******************************************************************
读字节数据函数               
*******************************************************************/
unsigned char IRcvByte(unsigned char sla)
{  unsigned char c;

   Start_I2c();          //启动总线
   SendByte(sla+1);      //发送器件地址
   if(ack==0)return(0);
   c=RcvByte();          //读取数据0

   Ack_I2c(1);           //发送非就答位
   Stop_I2c();           //结束总线
   return(c);
}


