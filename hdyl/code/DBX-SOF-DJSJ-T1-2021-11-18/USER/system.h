#ifndef __SYSTEM_H_
#define __SYSTEM_H_
#include "stm32f10x.h"

#define Ntc_Num 126													// 温度传感器测温范围
#define AVER_SAMPLE_NUM 200									// 平均采样
#define MAX_YSTEMP			38									// 最高原水温度低于38度
#define MAX_TDS_VALUE   150									// 可电解最大TDS值
#define VERF_VALUE 			1.208								// 参考电压
#define MAX_TEMP				100									// 超温度值			
#define MAX_FANXI_VALUE 1500								// 1500ml水用于反洗
#define MAX_PREHEAT     25									// 预热温度
#define MAX_TOTAL_FLOW  2000								// 最大单次出水量 2L
#define TEST_TDS_VALUE  50									// 检测TDS出水阈值
#define TDS_TEMP_FACTOR 0.99								// TDS温度补偿系数 20摄氏度标准值
#define WATER_SPEED 		20.0								// 电机出水量 1750ml - 2000ml每分钟测算  1750/60=29   2000/60=33  按照1500ml每分钟计算 1500/60=25.0
typedef enum
{
  SENSER_NULL=0,		
  SENSER_SXG_UP,
  SENSER_SXG_DOWN,
  SENSER_SXG_HOLD,
  SENSER_SSX_UP,
  SENSER_SSX_DOWN,
  SENSER_SSX_HOLD,
  SENSER_YSX_UP,
  SENSER_YSX_DOWN,
  SENSER_YSX_HOLD,	
}SENSER_ENUM;

typedef struct
{
  uint8_t SENSER_SXG_MODE;
  uint8_t SENSER_SSX_MODE;
  uint8_t SENSER_YSX_MODE;	
}
SENSERMODE_TypeDef;

extern __IO uint16_t Yswater_Tds;						// TDS值
extern __IO uint8_t Cswater_Temp;						// 
extern __IO float Hmwater_Temp;							//
extern __IO uint8_t Yswater_Temp;						//
extern __IO uint8_t Set_Temp_Num;						// 设置出水温度
extern __IO uint8_t Set_PH_Num;							// 设置PH
extern __IO uint16_t Now_Elecr_Current;			// 当前电流
extern __IO uint8_t 	Work_Step;						// 工作步骤及模式
extern __IO uint16_t Set_ORP_Vaule;					// 设置ORP值
extern __IO uint8_t  Show_TDS_Value;				// 上电显示TDS值
extern __IO uint8_t  Standby_Mode;					// 待机模式      0 待机模式  1 正常模式
extern __IO uint8_t  Test_Filter_Flage;			// 测试滤芯 
extern __IO uint8_t  Tds_Test_Flage;				// TDS检测标志位  0 不检测 1 检测
extern __IO uint16_t Filter_Life;						// 滤芯寿命 500L

extern __IO uint8_t Set_Temp_Table[4];			// 
extern __IO uint8_t Set_PH_Table[4];				//
extern __IO uint16_t Set_ORP_Table[4];			//
extern __IO uint16_t Filter_Count;					// 当前水流速度
extern uint8_t RF_ID_Code[4];								// ID卡号
extern uint8_t New_Filter_Flage;						// 新滤芯反洗标志

void Get_Now_Temp(void);
void Get_Now_Value(void);
uint8_t Work_In_Set(void);
uint8_t Senser_Value_Read(void);
void Set_Water_Temp(void);
void Set_Value_PH(void);
void RF_Read_Info(uint8_t *step);
void EM4305_Get_NewFAX(uint8_t *step);

#endif

/******************** (C) COPYRIGHT 2013 盛元电子工作室 **************************/
