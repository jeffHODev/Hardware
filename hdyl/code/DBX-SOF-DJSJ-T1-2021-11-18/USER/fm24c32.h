#ifndef __fm24c256_H
#define	__fm24c256_H

#include "stm32f10x.h"

/* �궨��	*/

#define I2C_TIME_OUT    4000					// 
#define Write_EE 				0xa0		 			// EEprom Write
#define Read_EE 				0xa1					// EEprom Read


// ��������
#define Option_Flage 0xA5A5						// ��ʼ����־
#define SERIAL_NUM_ADDR  0						// ���к�
#define OPER_FLAGE_ADDR  5						// ��ʼ����־��ַ
#define SLAVE_CODE_ADDR  10						// �豸ͨѶ��ַ�����ַ
#define SLAVE_BAUD_ADDR  15						// �豸ͨѶ������	
#define CONTROL_MODE_ADDR  20					// �豸����ģʽ
#define LED_BRIGHT_ADDR  25						// �豸��ʾ���� LED_KEY_Bright
// ADC����
#define ADC_BASES_ADDR   100					// ADC������ַ
#define ADC_SIZE_VALUE   20						// ADC������ֵ��С
#define ADC_FILTER_ADDR  0						// ADC�˲�����
#define ADC_RANGE_ADDR   5						// ADC����ֵ
#define ADC_ZERO_ADDR    10						// ADC���У׼ֵ
#define ADC_FULL_ADDR    15						// ADC������У׼ֵ
// DAC����
#define DAC_BASES_ADDR   300					// DAC������ַ
#define DAC_SIZE_VALUE   10						// DAC������ֵ��С
#define DAC_RANGE_ADDR   0						// DAC����ֵ
#define DAC_FULL_ADDR    5						// DAC������У׼ֵ
// FLUX����
#define FLUX_BASES_ADDR  500					// FLUX������ַ
#define FLUX_SIZE_VALUE  100					// FLUX������ֵ��С
#define FLUX_INADEE_ADDR       0			// ��ǰ��������ʹ�ܱ�־��ַ
#define FLUX_UNIT_ADDR         5			// ��λ�趨
#define FLUX_SPOT_ADDR				10			// ��������С����
#define FLUX_SHOW_ACC_ADDR		15			// �Ƿ���ʾ�����ۻ�ֵ
#define FLUX_FILTER_ADDR  		20			// ADC�˲�����
#define FLUX_RANGE_MULT_ADDR  25			// �����ƷŴ���											1-1000
#define FLUX_TIME_AUTO_ADDR		30			// �Զ�����ģʽ��������������ʱ��  		0-255	S
#define FLUC_TIME_RUN_ADDR		35			// �������������е�ʱ�����				
#define FLUX_VALUE_AUTO_ADDR	40			// �Զ�ģʽ�����������з���ֵ         0-255 N
#define FLUX_RANGE_ADDR				45			// �����Ƶ�ǰ������������ֵ						0-5000
#define FLUX_ALARM_VALUE_ADDR	50			// ���ƫ���ֵ
#define FLUX_VALUE_SET_ADDR		55			// �����趨ֵ
#define FLUX_DAC_FULL_ADDR		60			// DAC������
#define FLUX_ADC_ZERO_ADDR		65			// ADC0������ֵ
#define FLUX_ADC_FULL_ADDR		70			// ADC������֮
#define FLUX_MAX_FLOW_ADDR		75			// ��������趨ֵ
#define FLUX_TIME_DOWN_ADDR		80			// ��������ʱ


/* Ӳ����������	*/
// IIC������
#define IIC_WP(a)	do{if (a)\
										GPIO_SetBits(GPIOB,GPIO_Pin_13);\
										else\
										GPIO_ResetBits(GPIOB,GPIO_Pin_13);}while(0)
// ��ȡIIC������
#define SDA_read    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)
// IICʱ����
#define IIC_SCL(a)	do{if (a)\
										GPIO_SetBits(GPIOB,GPIO_Pin_14);\
										else\
										GPIO_ResetBits(GPIOB,GPIO_Pin_14);}while(0)
// IIC������
#define IIC_SDA(a)	do{if (a)\
										GPIO_SetBits(GPIOB,GPIO_Pin_15);\
										else\
										GPIO_ResetBits(GPIOB,GPIO_Pin_15);}while(0)

//typedef enum {FALSE = 0, TRUE = !FALSE} bool;	
										
/* ��������	*/
void I2C_GPIO_Config(void);
uint8_t EEP_Read_Dates( uint16_t address,uint16_t num,uint8_t *temp );				// ͻ��ʽ��ȡEEPROM
uint8_t EEP_Read_Date( uint16_t address );																	// ͻ��ʽ��ȡEEPROM
uint8_t EEP_Write_Dates( uint16_t address,uint16_t num, uint8_t *pdate );		// ָ����ַд����
uint8_t EEP_Write_Date( uint16_t address, uint8_t date );									// ָ����ַд����
uint8_t EEP_Write_NULL( uint16_t address,uint16_t num );										// ָ����ַд����
#endif

/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/

