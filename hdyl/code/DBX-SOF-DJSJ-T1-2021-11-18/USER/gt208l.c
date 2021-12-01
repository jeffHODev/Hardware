/******************** (C) COPYRIGHT 2014 ʢԪ���ӹ����� **************************
 * �ļ���  ��DS3231.c
 * ����    ��IIC Ӧ�ú���         
 * ʵ��ƽ̨��STM32�ֳֻ�������
 * Ӳ������: 
 * ��汾  ��ST3.0.0
 * ����    ��С��
 * ��Ŀ˵������������Ŀ
**********************************************************************************/
#include "iic.h"
#include "delay.h" 

__IO KEYMODE_TypeDef Key_Now_Mode;
uint8_t Key_Code_Value=0;
/********************************************************************************
 * ��������I2C_GPIO_Config		 												
 * ����  ��I2C1 I/O����	   														
 * ����  ����																	
 * ���  ����																	
 * ����  ���ڲ�����																
*********************************************************************************/

void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	IIC_SCL(1); IIC_SDA(0);IIC_WP(1);
  /* GPIOA Periph clock enable */
  /* Configure PA8 and PA9 in output pushpull mode */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	delay_ms(10);
  I2C_Stop();
}
/********************************************************************************
 * ��������I2C_Start									
 * ����  ��I2C1��ʼ����										
 * ����  ����																	
 * ���  ����																	
 * ����  ���ڲ�����																
*********************************************************************************/
bool I2C_Start(void)
{	
	uint16_t timeout=I2C_TIME_OUT;
	
  IIC_SCL(1);
  IIC_SDA(1);	 
  delay_us(10);
	while(--timeout && SDA_read==0)
	{
		delay_us(10);
	}
	IIC_SDA(0);									// 
	delay_us(10);	
	IIC_SCL(0);									// 
	if(timeout==0) return FALSE;
	else return TRUE;
}
/********************************************************************************
 * ��������2C_Stop						
 * ����  ��I2C1ֹͣ����										
 * ����  ����																	
 * ���  ����																	
 * ����  ���ڲ�����																
*********************************************************************************/
void I2C_Stop(void)
{
	IIC_SDA(0);
	IIC_SCL(0);
	delay_us(5);
  IIC_SCL(1);
	delay_us(5);
  IIC_SDA(1);
}
/********************************************************************************
 * ��������I2C_Ack				
 * ����  ��I2C1 ACKӦ��							
 * ����  ����																	
 * ���  ����																	
 * ����  ���ڲ�����																
*********************************************************************************/
void I2C_Ack(void)
{	
	IIC_SCL(0);
  IIC_SDA(0);
  delay_us(10);
  IIC_SCL(1);
  delay_us(10);
  IIC_SCL(0);
}
/********************************************************************************
 * ��������I2C_NoAck				
 * ����  ��I2C1 NoAckӦ��							
 * ����  ����																	
 * ���  ����																	
 * ����  ���ڲ�����																
*********************************************************************************/
void I2C_NoAck(void)
{	
	IIC_SCL(0);
  IIC_SDA(1);
  delay_us(10);
  IIC_SCL(1);
  delay_us(10);
  IIC_SCL(0);
}

/********************************************************************************
 * ��������I2C_WaitAck		
 * ����  ���ȴ�AckӦ��					
 * ����  ����																	
 * ���  ����																	
 * ����  ���ڲ�����																
*********************************************************************************/
bool I2C_WaitAck(void) 	 		//����Ϊ:=1��ACK,=0��ACK
{
  uint16_t timeout=I2C_TIME_OUT;

  IIC_SCL(1);
  delay_us(10);
  while(--timeout && SDA_read)						// �ȴ�����Ӧ�� 
  {
    delay_us(10);
  }
	IIC_SCL(0);
	delay_us(10);
  if(timeout==0) return FALSE;	
	else return TRUE; 
}

/********************************************************************************
 * ��������I2C_SendByte		
 * ����  ��I2C ����һ���ֽ�					
 * ����  ����																	
 * ���  ����																	
 * ����  ���ڲ�����																
*********************************************************************************/
void I2C_SendByte(uint8_t SendByte) //���ݴӸ�λ����λ//
{
  uint8_t i=8;
  
	IIC_SCL(0);
	delay_us(5);
  while(i--)
  {
    if(SendByte&0x80)	IIC_SDA(1); 			 
    else IIC_SDA(0); 		  
    SendByte<<=1;
    delay_us(5);
    IIC_SCL(1);
    delay_us(5);
    IIC_SCL(0);
		delay_us(5);
  }
}
/********************************************************************************
 * ��������I2C_ReceiveByte	
 * ����  ��I2C ����һ���ֽ�					
 * ����  ����																	
 * ���  ����																	
 * ����  ���ڲ�����																
*********************************************************************************/
uint8_t I2C_ReceiveByte(void)  			//���ݴӸ�λ����λ
{ 
  uint8_t i=8;
  uint8_t ReceiveByte=0;

  IIC_SDA(1); 			
  while(i--)
  {     
    IIC_SCL(1);
    delay_us(10);
    ReceiveByte<<=1; 
    if(SDA_read)	ReceiveByte++;
		delay_us(10);
		IIC_SCL(0);
  }
  return ReceiveByte;
}

/*************************************************************************************
 *	��������:	void EEP_Write_Date( uint16_t address, uint8_t date )		
 *	�������:	
 *	�������:	��������:	ָ����ַд����
 *	��		�ã��ⲿ����		
*************************************************************************************/
bool I2C_Write( uint8_t address,uint8_t num,uint8_t *temp )
{
    // 
    if(!I2C_Start()){I2C_Stop(); return FALSE;}	  	// ��ʼ����
    I2C_SendByte(Write_GT208);										// ��������Ӳ����ַ
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	  // �ȴ�Ӧ���ź�
    
    I2C_SendByte(address); 													// ���õ���ʼ��ַ    
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	 	// �ȴ�Ӧ���ź�
    // 
    for(;num>0;num--)																// ѭ��д��������
    {
			I2C_SendByte(*temp++);												// д������
			if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	// �ȴ�Ӧ���ź�
    }
    // 
    I2C_Stop(); 						// ����ֹͣ�ź�
    return TRUE;						// ���ز����ɹ���ʶ
}

/*************************************************************************************
 *	��������:	uint8_t EEP_Read_Dates( uint8_t address )	ָ����ַ��ȡ����ֽ�
 *	�������:	uint8_t address ��ȡ���ݵĵ�ַ   uint8_t n��ȡ���ݵĸ��� 
 *	�������:	uint8_t *temp ��ȡ����λ��
 *	��������:	ͻ��ʽ��ȡEEPROM
*************************************************************************************/
bool I2C_Read( uint8_t address,uint8_t num, uint8_t *temp )
{
    if(!I2C_Start()){I2C_Stop(); return FALSE;}	  	// ��ʼ����
    I2C_SendByte(Write_GT208);											// ��������Ӳ����ַ
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	 	// �ȴ�Ӧ���ź�
    
    I2C_SendByte(address); 													// ���õ���ʼ��ַ       
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	 	// �ȴ�Ӧ���ź�
    
    if(!I2C_Start()){I2C_Stop(); return FALSE;}	  	// ��ʼ����
    I2C_SendByte(Read_GT208);												// ��������Ӳ����ַ
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	 	// �ȴ�Ӧ���ź�
    
    for(;num>0;num--)																// ��ȡN���ֽ�
    {	
        *temp++ = I2C_ReceiveByte();			        	// ��ȡһ���ֽ�
        if(num==1)  I2C_NoAck();										// ���һ�����ݷ���NOACK	
        else 	  I2C_Ack();				        					// ��ȡ��һ�����ݷ���ACK			
    }
    
    I2C_Stop(); 																		// ����ֹͣ�ź�
    return TRUE;
}


uint8_t Init_GT208L(void)
{
  uint8_t idcode;
	uint8_t setcmd[]={0x0B,0x55,0x00};
  uint8_t setvalue[]={0x01,0x05,0x20};
	//uint8_t sensitivity[]={0x03,0x03,0x3F,0x03,0x3F,0x3F,0x3F,0x3F};		
	//  uint8_t sensitivity[]={0x0F,0x0a,0x0F,0x0F,0x0f,0x0f,0x0F,0x0f};		// High 0x3f   Low  0x02
		
	 uint8_t sensitivity[]={0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a};		// Low  0x3f     High 0x02	4 5 7
		
  I2C_GPIO_Config();
  // RED ID
  I2C_Read(CHIP_ID_REG,0x01,&idcode);
  // SET
  I2C_Write(TOUCH_CHANNEL_EN,sizeof(setcmd),setcmd);
  I2C_Write(GENERAL1_CON_REG,sizeof(setvalue),setvalue);
	I2C_Write(SENSITIVITY_REG,sizeof(sensitivity),sensitivity);		
	// IDLE time set
	// 2018-07-18
	setcmd[0]=0x08;
	I2C_Write(0x3D,1,&setcmd[0]);
	
	return idcode;

}

uint8_t GT208L_Read_Key(void)
{
  uint8_t keycode=0xff;
  
  if(I2C_Read(TOUCH_OUTPUT_REG,0x01,&keycode)==FALSE) return 0; 

  return keycode;

}

/***********************************************************************************
 * ��������Button_Read(void)
 * ����  ����ȡ��������
 * ����  ����
 * ���  ����
************************************************************************************/uint8_t Button_Read(void)
{			
  // �����˲�ʱ���ʱ
  static uint32_t Key_Time_PH=0;	      // �������ʱ��		PH   �趨����
  static uint32_t Key_Time_TEM=0;	      // �������ʱ��		TM   �趨����
  static uint32_t Key_Time_START=0;	    // �������ʱ��		START   �趨����	
	uint8_t keycode;
  
	keycode=GT208L_Read_Key();										// Ӳ��
	Key_Code_Value=keycode;												// 
	// PH��������
  if(keycode&0x02)															// SET��������
  {		
    if(Key_Now_Mode.KEY_PH_MODE==KEY_PH_HOLD)		// ������
    {
			Key_Now_Mode.KEY_PH_MODE=KEY_PH_HOLD;			//
    }
    else if(Key_Now_Mode.KEY_PH_MODE==KEY_PH_DOWN)    // ǰһ���ǰ�������
    {
      if(Key_Time_PH++>BUTTON_LONG_TIME)				// ��⵽������
      {
        Key_Now_Mode.KEY_PH_MODE=KEY_PH_HOLD;		// 
      }		
    }
    else 																				// ǰһ��û�а�������
    {
      if(Key_Time_PH++>BUTTON_FILTER_TIME)			// ����ͨ���˲����
      {
        Key_Now_Mode.KEY_PH_MODE=KEY_PH_DOWN;		// 
				Standby_Time=0;		
      }
    }
  }
  else
  {
    Key_Now_Mode.KEY_PH_MODE=KEY_PH_UP;					// ����̧��
    Key_Time_PH=0;
  }
	// TM��������
  if(keycode&0x01)															// SET��������
  {		
    if(Key_Now_Mode.KEY_TEM_MODE==KEY_TEM_HOLD)		// ������
    {
			Key_Now_Mode.KEY_TEM_MODE=KEY_TEM_HOLD;			//
    }
    else if(Key_Now_Mode.KEY_TEM_MODE==KEY_TEM_DOWN)    // ǰһ���ǰ�������
    {
      if(Key_Time_TEM++>BUTTON_LONG_TIME)				// ��⵽������
      {
        Key_Now_Mode.KEY_TEM_MODE=KEY_TEM_HOLD;		// 
      }		
    }
    else 																				// ǰһ��û�а�������
    {
      if(Key_Time_TEM++>BUTTON_FILTER_TIME)			// ����ͨ���˲����
      {
        Key_Now_Mode.KEY_TEM_MODE=KEY_TEM_DOWN;		// 
				Standby_Time=0;				
      }
    }	
  }
  else
  {
    Key_Now_Mode.KEY_TEM_MODE=KEY_TEM_UP;					// ����̧��
    Key_Time_TEM=0;
  }
	// START��������
  if(keycode&0x08)															// SET��������
  {		
    if(Key_Now_Mode.KEY_START_MODE==KEY_START_HOLD)		// ������
    {
			Key_Now_Mode.KEY_START_MODE=KEY_START_HOLD;			//
    }
    else if(Key_Now_Mode.KEY_START_MODE==KEY_START_DOWN)    // ǰһ���ǰ�������
    {
      if(Key_Time_START++>BUTTON_LONG_TIME)				// ��⵽������
      {
        Key_Now_Mode.KEY_START_MODE=KEY_START_HOLD;		// 
      }		
    }
    else 																				// ǰһ��û�а�������
    {
      if(Key_Time_START++>BUTTON_FILTER_TIME)			// ����ͨ���˲����
      {
        Key_Now_Mode.KEY_START_MODE=KEY_START_DOWN;		// 
				Standby_Time=0;
      }
    }	
  }
  else
  {
    Key_Now_Mode.KEY_START_MODE=KEY_START_UP;					// ����̧��
    Key_Time_START=0;
  }	
	
	return keycode;
}
