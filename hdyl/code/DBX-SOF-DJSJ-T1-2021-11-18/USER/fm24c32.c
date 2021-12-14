/******************** (C) COPYRIGHT 2014 ʢԪ���ӹ����� **************************
 * �ļ���  ��FM24C256.c
 * ����    ��IIC Ӧ�ú���         
 * ʵ��ƽ̨��STM32�ֳֻ�������
 * Ӳ������: 
 * ��汾  ��ST3.0.0
 * ����    ��С��
 * ��Ŀ˵������������Ŀ
**********************************************************************************/
#include "iic.h"
#include "delay.h" 

/* �����������ı���	*/
// EEPROM����Ϊ32768 BYTES
/***********************************EEPROM�洢�滮*************************************/ 
/***********************************0-99�ֽڴ���豸������Ϣ**************************/ 
/*		��ַ  0-9  	  :	�豸���к�		  	  [5 BYTE]				��2����											*/
/*		��ַ  10-19  	:	��ʼ����־		  	  [5 BYTE]				��2����											*/
/*		��ַ  20-29  	:	�豸��ǰRFID		  	[5 BYTE]				��2����											*/
/*		��ַ  30-39  	:	�豸���ϴ����¼		[5 BYTE]				��2����											*/
/*		��ַ  40-49  	:	�豸���ϴ����¼		[5 BYTE]				��2����											*/
/*		��ַ  50-59  	:	�豸���ϴ����¼		[5 BYTE]				��2����											*/
/***********************************100-199ADC�������ݶ�*******************************/ 

/********************************************************************************
 * ��������EEP_Write_Date( uint16_t address, uint8_t date )	
 * ����  ��ָ����ַд����
 * ����  ����																	
 * ���  ����																	
 * ����  ���ⲿ����			
*********************************************************************************/
uint8_t EEP_Write_Date( uint16_t address, uint8_t date )			// ָ����ַд����
{
	IIC_WP(0);
	delay_us(10);
	if(!I2C_Start()){I2C_Stop(); return 0;}	  			// ��ʼ����
	I2C_SendByte(Write_EE);													// ��������Ӳ����ַ
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	  		// �ȴ�Ӧ���ź�
	I2C_SendByte(address/256);						 					// ���ø���ʼ��ַ 
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	  		// �ȴ�Ӧ���ź�
	I2C_SendByte(address%256); 											// ���õ���ʼ��ַ    
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	  		// �ȴ�Ӧ���ź�
	I2C_SendByte(date);															// д������
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 			// �ȴ�Ӧ���ź�
	I2C_Stop(); 																		// ����ֹͣ�ź�
	IIC_WP(1);
	delay_ms(20);
	return 1;
}

/*************************************************************************************
 *	��������:	void EEP_Write_Date( uint16_t address, uint8_t date )					// ָ����ַд����
 *	�������:	
 *	�������:	��������:	ָ����ַд����
 *	��		�ã��ⲿ����		
*************************************************************************************/
uint8_t EEP_Write_Dates( uint16_t address,uint16_t num, uint8_t *pdate )		// ָ����ַд����
{
	IIC_WP(0);
	delay_us(10);
	if(!I2C_Start()){I2C_Stop(); return 0;}	  			// ��ʼ����
	I2C_SendByte(Write_EE);													// ��������Ӳ����ַ
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	  		// �ȴ�Ӧ���ź�
	I2C_SendByte(address/256);						 					// ���ø���ʼ��ַ 
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 			// �ȴ�Ӧ���ź�
	I2C_SendByte(address%256); 											// ���õ���ʼ��ַ    
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 			// �ȴ�Ӧ���ź�
	for(;num>0;num--)																// ѭ��д��������
	{
		I2C_SendByte(*pdate++);												// д������
		if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 		// �ȴ�Ӧ���ź�
	}	
	I2C_Stop(); 																		// ����ֹͣ�ź�
	IIC_WP(1);	
	delay_ms(20);																		// ��ʱ
	
	return 1;																				// ���ز����ɹ���ʶ
}

/*************************************************************************************
 *	��������:	void EEP_Write_Date( uint16_t address, uint8_t date )		// ָ����ַд����
 *	�������:	
 *	�������:	��������:	ָ����ַд����
 *	��		�ã��ⲿ����		
*************************************************************************************/
uint8_t EEP_Write_NULL( uint16_t address,uint16_t num )		// ָ����ַд����
{
	IIC_WP(0);
	delay_us(10);
	if(!I2C_Start()){I2C_Stop(); return 0;}	  			// ��ʼ����
	I2C_SendByte(Write_EE);													// ��������Ӳ����ַ
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	  		// �ȴ�Ӧ���ź�
	I2C_SendByte(address/256);						 					// ���ø���ʼ��ַ 
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 			// �ȴ�Ӧ���ź�
	I2C_SendByte(address%256); 											// ���õ���ʼ��ַ    
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 			// �ȴ�Ӧ���ź�
	for(;num>0;num--)																// ѭ��д��������
	{
		I2C_SendByte(0X00);														// д������
		if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 		// �ȴ�Ӧ���ź�
	}	
	I2C_Stop(); 																		// ����ֹͣ�ź�
	delay_ms(20);
	IIC_WP(1);	
	return 1;																				// ���ز����ɹ���ʶ
}
/*************************************************************************************
 *	��������:	uint8_t EEP_Read_Date( uint8_t address )   ָ����ַ��ȡһ���ֽ�
 *	�������:	uint8_t ms
 *	�������:
 *	��������:	ͻ��ʽ��ȡEEPROM
*************************************************************************************/
uint8_t EEP_Read_Date( uint16_t address )						//ͻ��ʽ��ȡEEPROM
{
	uint8_t dat;
	
	if(!I2C_Start()){I2C_Stop(); return 0;}	  				// ��ʼ����
	I2C_SendByte(Write_EE);														// ��������Ӳ����ַ
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// �ȴ�Ӧ���ź�
	I2C_SendByte(address/256);						 						// ���ø���ʼ��ַ 
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// �ȴ�Ӧ���ź�
	I2C_SendByte(address%256); 												// ���õ���ʼ��ַ       
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// �ȴ�Ӧ���ź�
	if(!I2C_Start()){I2C_Stop(); return 0;}	  				// ��ʼ����
	I2C_SendByte( Read_EE );													// �����豸Ӳ����ַ�Ͷ��ź�
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// �ȴ�Ӧ���ź�
	dat = I2C_ReceiveByte();													// ��ȡ�ֽ�
	I2C_NoAck();																			// ���һ�����ݷ���NOACK	
	I2C_Stop(); 																			// ����ֹͣ�ź�
	return( dat );																		// ��������
}

/*************************************************************************************
 *	��������:	uint8_t EEP_Read_Dates( uint8_t address )	ָ����ַ��ȡ����ֽ�
 *	�������:	uint8_t address ��ȡ���ݵĵ�ַ   uint8_t n��ȡ���ݵĸ��� 
 *	�������:	uint8_t *temp ��ȡ����λ��
 *	��������:	ͻ��ʽ��ȡEEPROM
*************************************************************************************/
uint8_t EEP_Read_Dates( uint16_t address,uint16_t num,uint8_t *temp )//ͻ��ʽ��ȡEEPROM
{
	if(!I2C_Start()){I2C_Stop(); return 0;}	  				// ��ʼ����
	I2C_SendByte(Write_EE);														// ��������Ӳ����ַ
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// �ȴ�Ӧ���ź�
	I2C_SendByte(address/256);						 						// ���ø���ʼ��ַ 
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// �ȴ�Ӧ���ź�
	I2C_SendByte(address%256); 												// ���õ���ʼ��ַ       
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// �ȴ�Ӧ���ź�
	if(!I2C_Start()){I2C_Stop(); return 0;}	  				// ��ʼ����
	I2C_SendByte( Read_EE );													// �����豸Ӳ����ַ�Ͷ��ź�
	if(!I2C_WaitAck()){I2C_Stop(); return 0;}	 				// �ȴ�Ӧ���ź�
	for(;num>0;num--)																	// ��ȡN���ֽ�
	{
		*temp++ = I2C_ReceiveByte();										// ��ȡһ���ֽ�
		if(num==1)  I2C_NoAck();												// ���һ�����ݷ���NOACK	
		else 	  I2C_Ack();															// ��ȡ��һ�����ݷ���ACK			
	}
	I2C_Stop(); 																			// ����ֹͣ�ź�
	return 1;
}

/******************** (C) COPYRIGHT 2014 ʢԪ���ӹ����� **************************/
