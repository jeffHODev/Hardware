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

#define BUTTON_FILTER_TIME      5      		// 20ms		按键去抖动
#define BUTTON_LONG_TIME        200	 	 		// 1500ms	长按键
#define HIGH_PRESS_OPEN_TIME		300				// 进水阀打开延时
#define HIGH_PRESS_CLOSE_TIME		10				// 进水阀关闭延时

#define LLJ_REST_TIME           100				// 流量计复位时间
#define VALUE_FILTER_NUM				100				// 滤波参数
#define STANDBY_TIME						180     	// 待机延时时间180S
#define LLJ_FILTER_NUM 					10				// 流量计动作触发时常
#define MAX_FILTER_FAIL_NUM     5					// 滤芯最大读取失败次数	

//#define PP_FILTER_TIME					24*30*6		// 6个月 24*30*6
//#define C1_FILTER_TIME					24*30*12	// 12个月
//#define UDF_FILTER_TIME					24*30*24	// 24个月
//#define C2_FILTER_TIME					24*30*12	// 12个月
#define PP_FILTER_TIME					3600	
#define C1_FILTER_TIME					7200	
#define UDF_FILTER_TIME					21600	
#define C2_FILTER_TIME					7200	

#define FANXI_DELAY_TIME        30 				// 反洗延时时间
#define FANXI_CON_TIME					3					// 反洗持续3分钟
#define CHONGXI_CON_TIME				2					// 冲洗持续2分钟

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
	
/***************************系统变量声明*****************************/
extern KEYMODE_TypeDef Key_Now_Mode;		// 按键参数
extern KEYMODE_TypeDef Key_Last_Mode;		// 按键参数
// 系统参数
extern uint8_t Power_Mode;					// 电源模式 0 关机  1 开机显示画面  2 正常运行  3
extern uint16_t MSecond;					 	// 系统运行毫秒
extern uint8_t  Second;					 		// 系统运行秒 
extern uint8_t  Minute;							// 系统运行分钟
extern uint16_t Hour;								// 系统运行小时 
extern uint8_t  MS_Flage;						// 10ms标志位
extern uint8_t Hour_Flage;					// 1小时标志
extern uint8_t Second_flage;				// 秒标志
extern uint8_t  Speak_Mode;					// 蜂鸣器鸣叫模式 0 不响  1 响一声  2 响两声
extern uint8_t ELE_Strength;				// 电解强度 0 净水 1 弱碱性  2 中  3 强
extern uint8_t SET_Ele_Value;				// 设置电解电压
extern float Flow_Now_Value;				// 当前流量值
extern float Flow_Now_Fre;					// 当前频率值
extern float Last_Float_Flow;				// 上次用水量				
extern double Toatl_Float_Flow;			// 累计用水量				
extern uint8_t Queshi_Flage;				// 缺水标志
extern uint16_t Queshui_Delay_Time;	// 缺水延时时间					
extern double Fanxi_Float_Flow;			// 累计反洗流量
													
extern uint8_t TLP0501_Value;							// TLP0501设置值

extern uint16_t Fanxi_Delay_Time;					// 反洗延时时间  当用户使用后30分钟后反洗
extern uint16_t Fanxi_Continue_Time;			// 反洗持续时间  反洗3分钟
extern uint16_t Chongxi_Continue_Time;		// 冲洗持续时间  冲洗1分钟
extern uint8_t Fanxi_Mode;								// 正在反冲洗模式
extern uint16_t Fanxi_Num;								// 反洗完成次数
extern __IO uint8_t LLJ_Rest_Time;				// 复位流量计延时时间
extern __IO uint8_t LLJ_Start_Flage;			// 流量计启动标志位
extern __IO uint8_t LLJ_End_Flage;				// 流量计启动标志位
extern __IO uint16_t LLJ_Now_Value;				// 流量计当前值
extern uint16_t Fanxi_Value_Table[5];			// 反洗流量值
extern uint16_t Auto_Fanxi_Value;					// 自动反洗流量值
/***************************系统函数声明*****************************/	
void delay_us(uint32_t nCount);			//
void delay_ms(uint32_t nCount);			//
void Speak_Work_Mode(void);					//
void Button_Value_Read(void);				//
void User_Work_Set(void);						// 用户设置模式
void Run_Work_Mode(void);						// 设备运行模式
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





