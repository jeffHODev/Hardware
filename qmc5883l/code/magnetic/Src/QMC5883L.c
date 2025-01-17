#include "main.h"
#include "stm32f0xx_hal.h"
#include "QMC5883L.h"


/* 私有宏定义 -----------------------------------------------------------------*/
#define MAT_N  10
#define QMC5883_ADDR               0x1a 
#define fabs(x)                    (x < 0 ? -x : x)
/* 私有变量 -------------------------------------------------------------------*/
int16_t MagnetRawAd[3];
int16_t X_Buff[CALIBTATE_DATA_NUM],Y_Buff[CALIBTATE_DATA_NUM],
Z_Buff[CALIBTATE_DATA_NUM],CalibrateCnt;
  GPIO_InitTypeDef GPIO_InitStruct;
void SDA_OUT()
{
//  GPIO_InitStruct.Pin = SDA_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}
void SDA_IN()
{
//  GPIO_InitStruct.Pin = SDA_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}
void SDA_HIGH()
{
  HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
}
void SDA_LOW()
{
  HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);
}
void SCL_HIGH()
{
  HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);
}
void SCL_LOW()
{
  HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);
}
unsigned char SDA_READ()
{	
	u8 tmp;
	tmp=HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin);
	//GPIO_ReadInputDataBit(ADXL345_I2C_PORT, ADXL345_SDA);
	
	return tmp;
}
/**************************************
延时5微秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数，注意时钟过快时需要修改
当改用1T的MCU时,请调整此延时函数
**************************************/
void Delay_5us()
{
	int i;
 	for(i=0;i<6;i++);
}

/**************************************
延时5毫秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数
当改用1T的MCU时,请调整此延时函数
**************************************/
void Delay5ms()
{
	unsigned int n = 10;
	while (n--);
}
/**************************************
起始信号
**************************************/
void QMC5883_Start()
{
    SDA_OUT();
    SDA_HIGH();                    //拉高数据线
    SCL_HIGH();                    //拉高时钟线
    Delay_5us();                 //延时
    SDA_LOW();                    //产生下降沿
    Delay_5us();                 //延时
    SCL_LOW();                    //拉低时钟线
}

/**************************************
停止信号
**************************************/
void QMC5883_Stop()
{
    SDA_LOW();                    //拉低数据线
    SCL_HIGH();                    //拉高时钟线
    Delay_5us();                 //延时
    SDA_HIGH();                    //产生上升沿
    Delay_5us();                 //延时
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void QMC5883_SendACK(unsigned char ack)
{
SDA_OUT();
	if(ack==1)
		SDA_HIGH(); 
	else
		SDA_LOW(); 
    //SDA = ack;                  //写应答信号
    SCL_HIGH();                    //拉高时钟线
    Delay_5us();                 //延时
    SCL_LOW();                    //拉低时钟线
    Delay_5us();                 //延时
}

/**************************************
接收应答信号
**************************************/
unsigned char QMC5883_RecvACK()
{
  unsigned char CY;
    SCL_HIGH();                    //拉高时钟线
    Delay_5us();                //延�
    SDA_IN();
    CY = SDA_READ();                   //进位标志 读应答信号
    SCL_LOW();                    //拉低时钟线
    Delay_5us();                //延时
    return CY;
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
void QMC5883_SendByte(u8 dat)
{
    u8 i;
 SDA_OUT();
    for (i=0; i<8; i++)         //8位计数器
    {
     //   dat <<= 1;              //移出数据的最高位
		if(dat&0x80)
			SDA_HIGH(); 
		else
			SDA_LOW();
		dat <<= 1;  
       // SDA = CY;               //送数据口
        SCL_HIGH();                //拉高时钟线
        Delay_5us();             //延时
        SCL_LOW();                //拉低时钟线
        Delay_5us();             //延时

    }
    QMC5883_RecvACK();
}

/**************************************
从IIC总线接收一个字节数据
**************************************/
u8 QMC5883_RecvByte()
{
    u8 i;
    u8 dat = 0;
 SDA_OUT();
    SDA_HIGH();                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {   SDA_IN();
        dat <<= 1;
        SCL_HIGH();                //拉高时钟线
        Delay_5us();             //延时
        
	if(SDA_READ())
			dat |=0x01;             //读数据 
		else
			dat =dat;             //读数据            
       // dat |= SDA;             //读数据               
        SCL_LOW();                //拉低时钟线
        Delay_5us();             //延时
    }
    return dat;
}

void I2C_WriteByte(uint8_t Val,uint8_t  Reg, uint8_t addr)
{
	QMC5883_Start();                  //起始信号
	QMC5883_SendByte(addr);   //发送设备地址+写信号
	QMC5883_SendByte(Reg);    //内部寄存器地址，请参考中文pdf22页 
	QMC5883_SendByte(Val);       //内部寄存器数据，请参考中文pdf22页 
	QMC5883_Stop();
}
 void  I2C_ReadByte(uint8_t *RegVal,uint8_t recv_cnt,uint8_t Reg,uint8_t addr)
{
	unsigned char i;
        QMC5883_Start();                          //起始信号
	QMC5883_SendByte(addr);           //发送设备地址+写信号
	QMC5883_SendByte(Reg);                   //发送存储单元地址，从0开始	
	QMC5883_Start();                          //起始信号
	QMC5883_SendByte(addr+1);         //发送设备地址+读信号
	for(i=0;i<recv_cnt;i++)
	{
		RegVal[i]=QMC5883_RecvByte();              //读出寄存器数据

		if(i==(recv_cnt-1))
			QMC5883_SendACK(0x01);
		else
			QMC5883_SendACK(0x00);
	}

	
	QMC5883_Stop();                           //停止信号
}
/* 私有函数 -------------------------------------------------------------------*/
/******************************************************************************
* @函数名称：void MPU6050_Init(void)
* @函数描述：MPU6050传感器初始化
* @输入参数：None
* @输出参数：None
*******************************************************************************/
uint8_t QMC5883_ReadReg(uint8_t Reg)
{
    u8 RegVal = 0;
    I2C_ReadByte(&RegVal,1,Reg,QMC5883_ADDR);
    return RegVal;
}
/******************************************************************************
* @函数名称：void MPU6050_Init(void)
* @函数描述：MPU6050传感器初始化
* @输入参数：None
* @输出参数：None
*******************************************************************************/
void QMC5883_WriteReg(uint8_t Val,uint8_t Reg)
{
    I2C_WriteByte(Val,Reg, QMC5883_ADDR);
}
/******************************************************************************
* @函数名称：void QMC5883_Init(void)
* @函数描述：程序入口
* @输入参数：None
* @输出参数：None
*******************************************************************************/
uint8_t Temp;
uint8_t QMC5883_InitConfig(void)
{
	
	
	QMC5883_WriteReg(0x01, 0x0B);
	QMC5883_WriteReg(0x40, 0x20);
	QMC5883_WriteReg(0x01, 0x21);
	QMC5883_WriteReg(0x0D, 0x09);/****OSR=512,RNG=+/-2G,ODR=200Hz,MODE= continuous*******/
	//while(!TIME_CheckTimeMs(TIME_DELAY,2));///DELAY 2ms
	HAL_Delay(10);
	Temp = QMC5883_ReadReg(0x09);
	 while(Temp != 0x1D) return 1;
	
    return 0;
}
/******************************************************************************
* @函数名称：void QMC5883_GetData(float *Magnet)
* @函数描述：获取地磁数据
* @输入参数：地磁buff
* @输出参数：None
*******************************************************************************/
void QMC5883_GetData(float *Magnet)
{
    uint8_t Buff[6],i;
    uint8_t Temp;
    
    Temp = QMC5883_ReadReg(0x06);
//    if (Temp&0x06)
//    {
//    	//LOG("output data OVL or ERROR!")
//    	return;
//    }
//    else if (Temp&0x00)
//    {
//    	//LOG("output data not ready!")
//    	return;
//    }
    
    I2C_ReadByte(Buff,6,0x00,QMC5883_ADDR);
	MagnetRawAd[0] = ((int16_t)Buff[1] << 8) | Buff[0];
	MagnetRawAd[1] = ((int16_t)Buff[3] << 8) | Buff[2];
	MagnetRawAd[2] = ((int16_t)Buff[5] << 8) | Buff[4];

    for(i=0;i<3;i++)
    {
        Magnet[i] = (float)MagnetRawAd[i] / 120.f;//单位uT
		
    }
}

/* 文件结束 --------------------------------------------------------------------*/

