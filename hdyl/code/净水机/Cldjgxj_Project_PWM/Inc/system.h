#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
typedef enum
{
  KEY_NULL=0,	
  KEY_ELE_UP,
  KEY_ELE_DOWN,
	KEY_ELE_HOLD,
  KEY_RES_UP,
  KEY_RES_DOWN,
	KEY_RES_HOLD,
  KEY_VAL_NULL,
	KEY_VAL_OPEN,
  KEY_VAL_CLOSE,
}KEY_ENUM;

typedef struct
{
  uint8_t KEY_ELE_MODE;
  uint8_t KEY_RES_MODE;
  uint8_t KEY_VAL_MODE;
}
KEYMODE_TypeDef;	

#define BUTTON_FILTER_TIME      5      		// 20ms		����ȥ����
#define BUTTON_LONG_TIME        200	 	 		// 1500ms	������
#define HIGH_PRESS_OPEN_TIME		300				// ��ˮ������ʱ
#define HIGH_PRESS_CLOSE_TIME		10				// ��ˮ���ر���ʱ

#define LLJ_REST_TIME           100				// �����Ƹ�λʱ��
#define VALUE_FILTER_NUM				100				// �˲�����
#define STANDBY_TIME						180     	// ������ʱʱ��180S
#define LLJ_FILTER_NUM 					10				// �����ƶ�������ʱ��
#define MAX_FILTER_FAIL_NUM     5					// ��о����ȡʧ�ܴ���	

//#define PP_FILTER_TIME					24*30*6		// 6���� 24*30*6
//#define C1_FILTER_TIME					24*30*12	// 12����
//#define UDF_FILTER_TIME					24*30*24	// 24����
//#define C2_FILTER_TIME					24*30*12	// 12����
#define PP_FILTER_TIME					3600	
#define C1_FILTER_TIME					7200	
#define UDF_FILTER_TIME					21600	
#define C2_FILTER_TIME					7200	

#define FANXI_DELAY_TIME        30 				// ��ϴ��ʱʱ��
#define FANXI_CON_TIME					3					// ��ϴ����3����
#define CHONGXI_CON_TIME				2					// ��ϴ����2����

// in put
#define KEY_RES_Read			HAL_GPIO_ReadPin(KEY_RES_GPIO_Port,KEY_RES_Pin)	
#define KEY_ELE_Read			HAL_GPIO_ReadPin(KEY_ELE_GPIO_Port,KEY_ELE_Pin)	
#define TEST_VAL_Read			HAL_GPIO_ReadPin(TEST_VA_GPIO_Port,TEST_VA_Pin)	
// out put set
#define ELE_CONA_SET(a)		do{if (a)\
													HAL_GPIO_WritePin(ELE_CONA_GPIO_Port,ELE_CONA_Pin,GPIO_PIN_SET);\
													else\
													HAL_GPIO_WritePin(ELE_CONA_GPIO_Port,ELE_CONA_Pin,GPIO_PIN_RESET);}while(0)
#define ELE_CONB_SET(a)			do{if (a)\
													HAL_GPIO_WritePin(ELE_CONB_GPIO_Port,ELE_CONB_Pin,GPIO_PIN_SET);\
													else\
													HAL_GPIO_WritePin(ELE_CONB_GPIO_Port,ELE_CONB_Pin,GPIO_PIN_RESET);}while(0)
#define TDS_N_SET(a)			do{if (a)\
													HAL_GPIO_WritePin(TDS_N_GPIO_Port,TDS_N_Pin,GPIO_PIN_SET);\
													else\
													HAL_GPIO_WritePin(TDS_N_GPIO_Port,TDS_N_Pin,GPIO_PIN_RESET);}while(0)
#define TDS_P_SET(a)			do{if (a)\
													HAL_GPIO_WritePin(TDS_P_GPIO_Port,TDS_P_Pin,GPIO_PIN_SET);\
													else\
													HAL_GPIO_WritePin(TDS_P_GPIO_Port,TDS_P_Pin,GPIO_PIN_RESET);}while(0)
#define SPI2_NSS_SET(a)			do{if (a)\
													HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port,SPI2_NSS_Pin,GPIO_PIN_SET);\
													else\
													HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port,SPI2_NSS_Pin,GPIO_PIN_RESET);}while(0)
#define LED_SET(a)				do{if (a)\
													HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);\
													else\
													HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);}while(0)
#define RF_A_SET(a)				do{if (a)\
													HAL_GPIO_WritePin(RF_A_GPIO_Port,RF_A_Pin,GPIO_PIN_SET);\
													else\
													HAL_GPIO_WritePin(RF_A_GPIO_Port,RF_A_Pin,GPIO_PIN_RESET);}while(0)
#define RF_B_SET(a)				do{if (a)\
													HAL_GPIO_WritePin(RF_B_GPIO_Port,RF_B_Pin,GPIO_PIN_SET);\
													else\
													HAL_GPIO_WritePin(RF_B_GPIO_Port,RF_B_Pin,GPIO_PIN_RESET);}while(0)
#define TM_SCK_SET(a)				do{if (a)\
													HAL_GPIO_WritePin(TM_SCK_GPIO_Port,TM_SCK_Pin,GPIO_PIN_SET);\
													else\
													HAL_GPIO_WritePin(TM_SCK_GPIO_Port,TM_SCK_Pin,GPIO_PIN_RESET);}while(0)
#define TM_DIN_SET(a)				do{if (a)\
													HAL_GPIO_WritePin(TM_DIN_GPIO_Port,TM_DIN_Pin,GPIO_PIN_SET);\
													else\
													HAL_GPIO_WritePin(TM_DIN_GPIO_Port,TM_DIN_Pin,GPIO_PIN_RESET);}while(0)
#define ST_VA_SET(a)				do{if (a)\
													HAL_GPIO_WritePin(ST_VA_GPIO_Port,ST_VA_Pin,GPIO_PIN_SET);\
													else\
													HAL_GPIO_WritePin(ST_VA_GPIO_Port,ST_VA_Pin,GPIO_PIN_RESET);}while(0)
#define SS_VA_SET(a)				do{if (a)\
													HAL_GPIO_WritePin(SS_VA_GPIO_Port,SS_VA_Pin,GPIO_PIN_SET);\
													else\
													HAL_GPIO_WritePin(SS_VA_GPIO_Port,SS_VA_Pin,GPIO_PIN_RESET);}while(0)
#define JS_VA_SET(a)				do{if (a)\
													HAL_GPIO_WritePin(JS_VALUE_GPIO_Port,JS_VALUE_Pin,GPIO_PIN_SET);\
													else\
													HAL_GPIO_WritePin(JS_VALUE_GPIO_Port,JS_VALUE_Pin,GPIO_PIN_RESET);}while(0)
	
/***************************ϵͳ��������*****************************/
extern KEYMODE_TypeDef Key_Now_Mode;		// ��������
extern KEYMODE_TypeDef Key_Last_Mode;		// ��������
// ϵͳ����
extern uint8_t Power_Mode;					// ��Դģʽ 0 �ػ�  1 ������ʾ����  2 ��������  3
extern uint16_t MSecond;					 	// ϵͳ���к���
extern uint8_t  Second;					 		// ϵͳ������ 
extern uint8_t  Minute;							// ϵͳ���з���
extern uint16_t Hour;								// ϵͳ����Сʱ 
extern uint8_t  MS_Flage;						// 10ms��־λ
extern uint8_t Hour_Flage;					// 1Сʱ��־
extern uint8_t Second_flage;				// ���־
extern uint8_t  Speak_Mode;					// ����������ģʽ 0 ����  1 ��һ��  2 ������
extern uint8_t ELE_Strength;				// ���ǿ�� 0 ��ˮ 1 ������  2 ��  3 ǿ
extern uint8_t SET_Ele_Value;				// ���õ���ѹ
extern float Flow_Now_Value;				// ��ǰ����ֵ
extern float Flow_Now_Fre;					// ��ǰƵ��ֵ
extern float Last_Float_Flow;				// �ϴ���ˮ��				
extern double Toatl_Float_Flow;			// �ۼ���ˮ��				
extern uint8_t Queshi_Flage;				// ȱˮ��־
extern uint16_t Queshui_Delay_Time;	// ȱˮ��ʱʱ��					
extern double Fanxi_Float_Flow;			// �ۼƷ�ϴ����
													
extern uint8_t TLP0501_Value;							// TLP0501����ֵ

extern uint16_t Fanxi_Delay_Time;					// ��ϴ��ʱʱ��  ���û�ʹ�ú�30���Ӻ�ϴ
extern uint16_t Fanxi_Continue_Time;			// ��ϴ����ʱ��  ��ϴ3����
extern uint16_t Chongxi_Continue_Time;		// ��ϴ����ʱ��  ��ϴ1����
extern uint8_t Fanxi_Mode;								// ���ڷ���ϴģʽ
extern uint16_t Fanxi_Num;								// ��ϴ��ɴ���
extern __IO uint8_t LLJ_Rest_Time;				// ��λ��������ʱʱ��
extern __IO uint8_t LLJ_Start_Flage;			// ������������־λ
extern __IO uint8_t LLJ_End_Flage;				// ������������־λ
extern __IO uint16_t LLJ_Now_Value;				// �����Ƶ�ǰֵ
extern uint16_t Fanxi_Value_Table[5];			// ��ϴ����ֵ
extern uint16_t Auto_Fanxi_Value;					// �Զ���ϴ����ֵ
/***************************ϵͳ��������*****************************/	
void delay_us(uint32_t nCount);			//
void delay_ms(uint32_t nCount);			//
void Speak_Work_Mode(void);					//
void Button_Value_Read(void);				//
void User_Work_Set(void);						// �û�����ģʽ
void Run_Work_Mode(void);						// �豸����ģʽ
void Test_Water_Flow(void);					//
void ADC_Read_System_Value(void);		//
uint8_t EM4305_Read_Data(uint8_t page);
uint8_t EM4305_Write_Data(uint8_t page,uint8_t data[4]);
void User_Work_Set(void);						//
void System_Read_Fax(void);					//
void Back_Wash_Oper(void);					//
void Read_System_Info(void);				//
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif											
											
#endif





