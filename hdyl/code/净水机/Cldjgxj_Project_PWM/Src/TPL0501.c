#include "TPL0501.h"
#include "timer.h" 

/*******************************************************************************
* Function Name  : SPI2_SendByte(uint8_t byte)
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
uint8_t TPL_SendByte(uint8_t byte)
{
	uint8_t i,temp=0;
	
	for(i=0;i<8;i++)											// SPI
	{
		TPL0501_SCLK(0);										// 第一个上升沿读取DOUT数据
		delay_us(1);												// 延时
		if(byte&0x80)	TPL0501_DINW(1);			// 高位为1
		else TPL0501_DINW(0);								// 高位为0
		byte=byte<<1;												// 数据左移位
		delay_us(1);												// 延时
		TPL0501_SCLK(1);										// 第二个为下降沿将DIN数据写入
		delay_us(1);												// 延时
		TPL0501_SCLK(0);										// 第一个上升沿读取DOUT数据
	}
	return temp;
}


/*******************************************************************************
* Function Name  : TPL0501_SendByte(uint8_t byte)
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
uint8_t TPL0501_SendByte(uint8_t byte)
{
		TPL0501_CS(1);
		TPL0501_SCLK(1);	
		delay_us(5);	
	  TPL0501_CS(0);
		delay_us(10);
		TPL_SendByte(byte);
		delay_us(5);
		TPL0501_CS(1);
	
		return 0;
}

