#ifndef __SYSTEM_H_
#define __SYSTEM_H_
#include "stm32f10x.h"

#define Ntc_Num 126													// �¶ȴ��������·�Χ
#define AVER_SAMPLE_NUM 200									// ƽ������
#define MAX_YSTEMP			38									// ���ԭˮ�¶ȵ���38��
#define MAX_TDS_VALUE   150									// �ɵ�����TDSֵ
#define VERF_VALUE 			1.208								// �ο���ѹ
#define MAX_TEMP				100									// ���¶�ֵ			
#define MAX_FANXI_VALUE 1500								// 1500mlˮ���ڷ�ϴ
#define MAX_PREHEAT     25									// Ԥ���¶�
#define MAX_TOTAL_FLOW  2000								// ��󵥴γ�ˮ�� 2L
#define TEST_TDS_VALUE  50									// ���TDS��ˮ��ֵ
#define TDS_TEMP_FACTOR 0.99								// TDS�¶Ȳ���ϵ�� 20���϶ȱ�׼ֵ
#define WATER_SPEED 		20.0								// �����ˮ�� 1750ml - 2000mlÿ���Ӳ���  1750/60=29   2000/60=33  ����1500mlÿ���Ӽ��� 1500/60=25.0
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

extern __IO uint16_t Yswater_Tds;						// TDSֵ
extern __IO uint8_t Cswater_Temp;						// 
extern __IO float Hmwater_Temp;							//
extern __IO uint8_t Yswater_Temp;						//
extern __IO uint8_t Set_Temp_Num;						// ���ó�ˮ�¶�
extern __IO uint8_t Set_PH_Num;							// ����PH
extern __IO uint16_t Now_Elecr_Current;			// ��ǰ����
extern __IO uint8_t 	Work_Step;						// �������輰ģʽ
extern __IO uint16_t Set_ORP_Vaule;					// ����ORPֵ
extern __IO uint8_t  Show_TDS_Value;				// �ϵ���ʾTDSֵ
extern __IO uint8_t  Standby_Mode;					// ����ģʽ      0 ����ģʽ  1 ����ģʽ
extern __IO uint8_t  Test_Filter_Flage;			// ������о 
extern __IO uint8_t  Tds_Test_Flage;				// TDS����־λ  0 ����� 1 ���
extern __IO uint16_t Filter_Life;						// ��о���� 500L

extern __IO uint8_t Set_Temp_Table[4];			// 
extern __IO uint8_t Set_PH_Table[4];				//
extern __IO uint16_t Set_ORP_Table[4];			//
extern __IO uint16_t Filter_Count;					// ��ǰˮ���ٶ�
extern uint8_t RF_ID_Code[4];								// ID����
extern uint8_t New_Filter_Flage;						// ����о��ϴ��־

void Get_Now_Temp(void);
void Get_Now_Value(void);
uint8_t Work_In_Set(void);
uint8_t Senser_Value_Read(void);
void Set_Water_Temp(void);
void Set_Value_PH(void);
void RF_Read_Info(uint8_t *step);
void EM4305_Get_NewFAX(uint8_t *step);

#endif

/******************** (C) COPYRIGHT 2013 ʢԪ���ӹ����� **************************/
