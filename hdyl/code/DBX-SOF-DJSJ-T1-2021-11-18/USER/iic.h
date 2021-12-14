#ifndef __IIC_H
#define	__IIC_H

/* ����ϵͳͷ�ļ� */
#include "stm32f10x.h"

#define Write_EE 								0xa0		 			// EEprom Write
#define Read_EE 								0xa1					// EEprom Read
#define I2C_TIME_OUT            1000					// TIME OUT
#define Read_GT208 	        		0xB9					// GT208  Read
#define Write_GT208 	        	0xB8 					// GT208  Write         
#define CHIP_ID_REG             0x02
#define TOUCH_CHANNEL_EN        0x03
#define MULTI_TOUCH_SET         0x04
#define TOUCH_OUTPUT_REG        0x2A
#define GENERAL1_CON_REG        0x39    			// CLOCK 100KHz SET 0x01
#define GENERAL2_CON_REG        0x3A    			// 
#define GENERAL3_CON_REG        0x3B    			// Expiration Control Registers
#define EXPIRATION_CON_REG      0x3D    			// Expiration Control Registers
#define SENSITIVITY_REG         0x40

#define SENSER_FILTER_TIME      50
#define BUTTON_FILTER_TIME      2
#define BUTTON_LONG_TIME        100	
#define SET_CURRENT_VALUE       3000

#define Option_Flage 						0xA5A5				// ��ʼ����־
#define SERIAL_NUM_ADDR  				0							// ���к�
#define OPER_FLAGE_ADDR  				10						// ��ʼ����־��ַ
#define RFID_CODE_ADDR  				20						// ��оID
#define RFID_CURRENT_SPEED      30						// ��ʼ������
#define CLERA_FLAGE_ADDR  			40						// ��оδ��ϴ��־
#define TEST_FILTER_ADDR  			50						// ������о��־λ
#define FILTER_LIFT_ADDR  			60						// ��о����


/* Ӳ����������	*/
// ��ȡIIC������
#define SDA_read    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)
// IICʱ����
#define IIC_SCL(a)	do{if (a)\
										GPIO_SetBits(GPIOA,GPIO_Pin_10);\
										else\
										GPIO_ResetBits(GPIOA,GPIO_Pin_10);}while(0)
// IIC������
#define IIC_SDA(a)	do{if (a)\
										GPIO_SetBits(GPIOA,GPIO_Pin_9);\
										else\
										GPIO_ResetBits(GPIOA,GPIO_Pin_9);}while(0)

// IIC������
#define IIC_WP(a)		do{if (a)\
										GPIO_SetBits(GPIOA,GPIO_Pin_11);\
										else\
										GPIO_ResetBits(GPIOA,GPIO_Pin_11);}while(0)										
																			
typedef enum
{
  KEY_NULL=0,		
  KEY_START_UP,
  KEY_START_DOWN,
  KEY_START_HOLD,
	KEY_PH_UP,
	KEY_PH_DOWN,
	KEY_PH_HOLD,
	KEY_TEM_UP,
	KEY_TEM_DOWN,
	KEY_TEM_HOLD
}KEY_ENUM;

typedef struct
{
  uint8_t KEY_PH_MODE;				// ���ģʽ
  uint8_t KEY_START_MODE;		  // ����ģʽ
  uint8_t KEY_TEM_MODE;				// ˮ��ģʽ
}
KEYMODE_TypeDef;										

extern uint8_t Key_Code_Value;
extern __IO KEYMODE_TypeDef Key_Now_Mode;
		
uint8_t Init_GT208L(void);                                            
uint8_t Button_Read(void);
void I2C_Stop(void);
bool I2C_Start(void);
void I2C_Ack(void);
void I2C_NoAck(void);
bool I2C_WaitAck(void); 	 		//����Ϊ:=1��ACK,=0��ACK
void I2C_SendByte(uint8_t SendByte); //���ݴӸ�λ����λ//
uint8_t I2C_ReceiveByte(void);  			//���ݴӸ�λ����λ
uint8_t EEP_Read_Date( uint16_t address );
uint8_t EEP_Write_Date( uint16_t address, uint8_t date );			// ָ����ַд����
uint8_t EEP_Write_Dates( uint16_t address,uint16_t num, uint8_t *pdate );		// ָ����ַд����
uint8_t EEP_Read_Dates( uint16_t address,uint16_t num,uint8_t *temp );			//ͻ��ʽ��ȡEEPROM
uint8_t EEP_Write_NULL( uint16_t address,uint16_t num );		// ָ����ַд����
#endif
