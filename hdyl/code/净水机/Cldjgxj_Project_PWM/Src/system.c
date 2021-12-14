/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "system.h"
#include "filter.h"
#include "bsp_led.h"
#include "feeprom.h"
#include "pid.h"
#include "tim.h"

KEYMODE_TypeDef Key_Now_Mode;				// 按键参数
KEYMODE_TypeDef Key_Last_Mode;			// 按键参数
// 系统参数
uint8_t Power_Mode=3;								// 电源模式 0 关机  1 开机显示画面  2 正常运行  3
uint16_t MSecond=0;					 				// 系统运行毫秒
uint8_t  Second=0;					 				// 系统运行秒 
uint8_t  Minute=0;									// 系统运行分钟
uint16_t Hour=0;										// 系统运行小时 
uint8_t  MS_Flage=0;								// 10ms标志位
uint8_t Hour_Flage=0;								// 1小时标志
uint8_t  Second_flage=0;						// 秒标志
uint8_t  Speak_Mode=0;							// 蜂鸣器鸣叫模式 0 不响  1 响一声  2 响两声
uint8_t JS_VA_Time=0;								// 进水阀开启累计时间
// 运行参数
uint8_t ElE_Work_Mode=0;						// 电解工作模式
uint8_t ELE_Strength=1;							// 电解强度 0 净水 1 弱碱性  2 中  3 强
uint8_t SET_Ele_Value=100;					// 设置电解电压
uint8_t TLP0501_Value=0;						// TLP0501设置值
float Flow_Now_Value=0;							// 当前水流量值
float Flow_Now_Fre=0;								// 当前频率值
float Last_Float_Flow=0;						// 上次用水量
double Fanxi_Float_Flow=0;					// 累计反洗流量
double Toatl_Float_Flow=0;					// 累计用水流量

__IO uint8_t LLJ_Rest_Time=0;				// 复位流量计延时时间
__IO uint8_t LLJ_Start_Flage=0;			// 流量计启动标志位
__IO uint8_t LLJ_End_Flage=0;				// 流量计启动标志位
__IO uint16_t LLJ_Now_Value=0;			// 流量计当前值

uint8_t Fanxi_Flage=0;							// 反洗标志
uint8_t Auto_Fanxi_Flage=0;					// 自动反洗标志
uint8_t Fanxi_Mode=0;								// 正在反冲洗模式
uint16_t Fanxi_Delay_Time=0;				// 反洗延时时间  当用户使用后30分钟后反洗
uint16_t Fanxi_Continue_Time=0;			// 反洗持续时间
uint16_t Chongxi_Continue_Time=0;		// 冲洗持续时间
uint8_t Fanxi_Step=0;								// 反洗步骤
uint8_t Queshi_Flage=0;							// 缺水标志
uint16_t Queshui_Delay_Time=0;			// 缺水延时时间
uint16_t Fanxi_Num=0;								// 反洗完成次数
uint16_t Fanxi_Value_Table[5]={500,400,300,200,100};
uint16_t Auto_Fanxi_Value=200;			// 自动反洗流量值
/***********************************************************************************
 * 函数名：delay_us
 * 描述  ：us延时
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
void delay_us(uint32_t nCount)
{
	uint32_t i,j;
	for(i=0;i<nCount;i++)
	{
  	for(j=0;j<10;j++)
		{
			__ASM("NOP");
		}
	}
}
/***********************************************************************************
 * 函数名：delay_ms
 * 描述  ：ms延时
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
void delay_ms(uint32_t nCount)
{
	uint32_t i;
	for(i=0;i<nCount;i++)
	{
		delay_us(1000);
	}  	
}
/***********************************************************************************
 * 函数名：void Button_Value_Read(void)
 * 描述  ：进入工作模式
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
void Button_Value_Read(void)
{
  static uint16_t key_ele_time,key_res_time,key_val_open_time,key_val_close_time;
	
	//KEY_ELE
  if( KEY_ELE_Read==GPIO_PIN_RESET)										// 键值判断													
  {		
    if(Key_Now_Mode.KEY_ELE_MODE==KEY_ELE_HOLD)				//		
    {
      Key_Now_Mode.KEY_ELE_MODE=KEY_ELE_HOLD;					
    }
    else if(Key_Now_Mode.KEY_ELE_MODE==KEY_ELE_DOWN)	// 前一次是按键按下
    {
      if( key_ele_time++>BUTTON_LONG_TIME)						// 检测到长按键
      {
        Key_Now_Mode.KEY_ELE_MODE=KEY_ELE_HOLD;				// 
      }		
    }
    else 																							// 前一次没有按键按下
    {
      if( key_ele_time++>BUTTON_FILTER_TIME)					// 按键通过滤波检测
      {
        Key_Now_Mode.KEY_ELE_MODE=KEY_ELE_DOWN;				// 
				Speak_Flage=2;																//
      }
    }	
  }
  else
  {
    Key_Now_Mode.KEY_ELE_MODE=KEY_ELE_UP;							// 按键抬起
    key_ele_time=0;                                   // 
  }	
	//KEY_RES
  if( KEY_RES_Read==GPIO_PIN_RESET)										// 键值判断													
  {		
    if(Key_Now_Mode.KEY_RES_MODE==KEY_RES_HOLD)				//		
    {
      Key_Now_Mode.KEY_RES_MODE=KEY_RES_HOLD;					
    }
    else if(Key_Now_Mode.KEY_RES_MODE==KEY_RES_DOWN)	// 前一次是按键按下
    {
      if( key_res_time++>BUTTON_LONG_TIME)						// 检测到长按键
      {
        Key_Now_Mode.KEY_RES_MODE=KEY_RES_HOLD;				// 				
      }		
    }
    else 																							// 前一次没有按键按下
    {
      if( key_res_time++>BUTTON_FILTER_TIME)					// 按键通过滤波检测
      {
        Key_Now_Mode.KEY_RES_MODE=KEY_RES_DOWN;				// 
				Speak_Flage=2;
      }
    }	
  }
  else
  {
    Key_Now_Mode.KEY_RES_MODE=KEY_RES_UP;							// 按键抬起
    key_res_time=0;                                  	// 
  }	
	//KEY_VAL
  if( TEST_VAL_Read==GPIO_PIN_RESET)									// 阀开信号											
  {		
		key_val_close_time=0;															// 关阀延时清零
    if(Key_Now_Mode.KEY_VAL_MODE==KEY_VAL_OPEN)				// 开阀状态	
    {
      Key_Now_Mode.KEY_VAL_MODE=KEY_VAL_OPEN;					// 开阀
			JS_VA_SET(1);																		// 设置进水阀打开	
    }
    else if( key_val_open_time++>HIGH_PRESS_OPEN_TIME)// 检测到长按键
		{
       Key_Now_Mode.KEY_VAL_MODE=KEY_VAL_OPEN;				// 
			 JS_VA_SET(1);																	// 设置进水阀打开	
    }
		else {Key_Now_Mode.KEY_VAL_MODE=KEY_VAL_NULL;}		// 阀信号未确定
  }
  else																								// 阀关信号
  {
		key_val_open_time=0;															// 开发延时清零
    if(Key_Now_Mode.KEY_VAL_MODE==KEY_VAL_CLOSE)			// 	
    {
			 Key_Now_Mode.KEY_VAL_MODE=KEY_VAL_CLOSE;				// 关阀状态
			 if(ElE_Work_Mode==0&&Fanxi_Step==0)						// 客户没有使用水设备也未在反洗
			 {
					JS_VA_SET(0);																// 设置进水阀关闭			 
			 }		
    }
    else if(key_val_close_time++>HIGH_PRESS_CLOSE_TIME)// 检测到关阀信号
		{
       Key_Now_Mode.KEY_VAL_MODE=KEY_VAL_CLOSE;				// 关阀
			 if(ElE_Work_Mode==0&&Fanxi_Step==0)						// 客户没有使用水设备也未在反洗
			 {
					JS_VA_SET(0);																// 设置进水阀关闭			 
			 }
    }
		else {Key_Now_Mode.KEY_VAL_MODE=KEY_VAL_NULL;}		// 阀信号未确定
  }	
}

/***********************************************************************************
 * 函数名：void User_Work_Set(void)
 * 描述  ：用户设置模式
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
void User_Work_Set(void)
{
	// 调节电解模式
	if(Key_Now_Mode.KEY_ELE_MODE!=Key_Last_Mode.KEY_ELE_MODE)
	{
		Key_Last_Mode.KEY_ELE_MODE=Key_Now_Mode.KEY_ELE_MODE;
		if(Key_Now_Mode.KEY_ELE_MODE==KEY_ELE_DOWN&&LLJ_Start_Flage==0&&Fanxi_Step==0)		// 制水后不能调整挡位
		{
			if(ELE_Strength++>2)
			{
				ELE_Strength=0;
			}	
			HAL_FLASH_Unlock();																							// Flash 解锁
			EE_WriteVariable(VirtAddVarTab[1],ELE_Strength); 								// 保存电解强度
			HAL_FLASH_Lock();																								// Flash 锁定
		}
		Standby_Time_Delay=STANDBY_TIME;
	}
	// 反冲洗
	if(Key_Now_Mode.KEY_RES_MODE!=Key_Last_Mode.KEY_RES_MODE)
	{
		Key_Last_Mode.KEY_RES_MODE=Key_Now_Mode.KEY_RES_MODE;
		if(Key_Now_Mode.KEY_RES_MODE==KEY_RES_DOWN)
		{
			if(Fanxi_Mode==1)																								// 关闭三通阀酸水阀			
			{
				LED_ZS=0;	LED_DJ=0; LED_CX=0;										
				Fanxi_Mode=0;SS_VA_SET(0);ST_VA_SET(0);															
			}
		}		
		if(Key_Now_Mode.KEY_RES_MODE==KEY_RES_HOLD&&LLJ_Start_Flage==0&&Queshi_Flage==0)		// 制水后不能反冲洗
		{
			if(Fanxi_Mode==1)	{	Fanxi_Mode=0;}
			else {	Fanxi_Flage=1;	}	
			Speak_Flage=6;																									// 开始反冲洗
		}
		Standby_Time_Delay=STANDBY_TIME;
	}
} //end

/***********************************************************************************
 * 函数名：void Run_Work_Mode(void)
 * 描述  ：用户工作模式
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
void Run_Work_Mode(void)
{	
	uint16_t value=0;
	
	switch(ElE_Work_Mode)
	{
		case 0:	// 待机模式
						if(LLJ_Start_Flage==1&&Fanxi_Mode==0)	// 水轮有动作并且不在反洗过程中
						{
							ElE_Work_Mode=1;			// 进入制水模式
							JS_VA_SET(1);					// 设置进水阀打开	
							LED_ZS=1;							// 制水指示灯点亮
						}
						break;
		case 1:	// 判断滤芯状态
						if(LED_PP>0&&LED_C1>0&&LED_UDF>0&&LED_C2>0)		// 滤芯正常
						{		
							if(ELE_Strength!=0)		// 开启电解
							{
								ELE_CONA_SET(1);		// 电解制取碱水 继电器正极吸合
								ELE_CONB_SET(0);		// 电解制取碱水
								ST_VA_SET(0);				// 关闭三通阀
								SS_VA_SET(1);				// 打开酸水阀											
								LED_DJ=1;						// 电解指示灯点亮		
								delay_ms(50);				// 延时
							}		
							Speak_Flage=3;				// 开始电解
						}
						else	{Speak_Flage=4;}	// 未电解
						ElE_Work_Mode=2;				// 进入制水模式						
						break;
		case 2:	// 电解模式		
						HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); 		// 打开PWM
						if(ELE_Strength!=0)			// 非净水模式
						{
							Start_Cur_PID(ELE_Strength);// 开启电解电压	
						}						
						ElE_Work_Mode=3;				// 进入制水模式							
						break;
		case 3:	// 电解中等待用户关闭
						if(LLJ_Start_Flage==0)	// 水轮不转停止制水
						{
							Stop_Cur_PID();				// 将电解电压调低		
							HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
							ElE_Work_Mode=4;			// 停止制水
							delay_ms(50);					// 延时
						}
						break;
		case 4:	// 停止制水
						value=(uint16_t)Toatl_Float_Flow;
						HAL_FLASH_Unlock();												// Flash 解锁
						EE_WriteVariable(VirtAddVarTab[3],value);	// 累计水量保存
						HAL_FLASH_Lock();													// Flash 锁定
						ELE_CONA_SET(0);				// 停止电解
						ELE_CONB_SET(0);				// 电解制取碱水			
						ST_VA_SET(0);						// 关闭三通阀
						SS_VA_SET(0);						// 关闭酸水阀				
						LED_ZS=0;								// 制水指示灯点亮
						LED_DJ=0;								// 电解指示灯点亮
						ElE_Work_Mode=0;				// 进入制水模式
						break;
		default : // 
						break;
	}
} //end




// ADC获取系统运行参数
void ADC_Read_System_Value(void)
{
	uint8_t i;
	static uint8_t init=0;
	static float ele_value,ele_current,sys_value;
	static double ele_current_zero;
	double ele_value_sum=0,ele_current_sum=0,sys_value_sum=0;	// 电解电压 电解电流 设备电压
	double res;
	
	for(i=0;i<ADC_FITER;i++)
	{
		ele_current_sum+=ADC_Value[i][0];
		ele_value_sum+=ADC_Value[i][1];
		sys_value_sum+=ADC_Value[i][2];
	}
	ele_current_sum/=ADC_FITER;
	ele_value_sum/=ADC_FITER;
	sys_value_sum/=ADC_FITER;
	
	// ADC数据初始化	采样100个周期 1000ms
	if(init<VALUE_FILTER_NUM)
	{
		init++;
		ele_current+=ele_current_sum;
		ele_value+=ele_value_sum;
		sys_value+=sys_value_sum;
		if(init==VALUE_FILTER_NUM) 
		{
			ele_current/=VALUE_FILTER_NUM;
			ele_value/=VALUE_FILTER_NUM;
			sys_value/=VALUE_FILTER_NUM;
			ele_current_zero=ele_current;
			ele_value=(ele_value*(3.3/4095.0))/(1.2/11.2);
			ele_current=0;
			sys_value=(sys_value*(3.3/4095.0))/(1.2/11.2);
		}
		return;
	}
	// 计算电解电压
	res=(ele_value_sum*(3.3/4095.0))/(1.2/11.2);
	ele_value=ele_value-ele_value/VALUE_FILTER_NUM+res/VALUE_FILTER_NUM;
	ELE_Value=ele_value;
	// 标定电解电流
	// 计算电解电流
	if(ElE_Work_Mode==0&&Fanxi_Step==0&&Standby_Time_Delay==0)	// 在待机模式下标定电流
	{
		ele_current_zero=ele_current_zero-ele_current_zero/VALUE_FILTER_NUM+ele_current_sum/VALUE_FILTER_NUM;// 重新标定电流
	}
	if(ele_current_sum<ele_current_zero) ele_current_sum=ele_current_zero-ele_current_sum;
	else ele_current_sum=0;
	res=ele_current_sum*(3.3/4095.0);
	res/=0.185;
	ele_current=ele_current-ele_current/VALUE_FILTER_NUM+res/VALUE_FILTER_NUM;
	ELE_Current=ele_current;
	// 计算系统电压
	res=(sys_value_sum*(3.3/4095.0))/(1.2/11.2);
	sys_value=sys_value-sys_value/VALUE_FILTER_NUM+res/VALUE_FILTER_NUM;
	SYS_Value=sys_value;
}

/***********************************************************************************
 * 函数名：void Test_Water_Flow(void)
 * 描述  ：测试水速
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
void Test_Water_Flow(void)		
{
	static uint8_t last_flage;
	double hz;
	
	// 统计累计用水量
	if(last_flage!=LLJ_Start_Flage)						// 检测第一次流量计打开状态
	{
		last_flage=LLJ_Start_Flage;							// 保存状态
		if(LLJ_Start_Flage!=0)									// 流量计启动
		{
			Last_Float_Flow=0;										// 清除上次流量
			Fanxi_Delay_Time=FANXI_DELAY_TIME;		// 当用户使用后一定时间后才开始反洗
		}
	}
	// 检测流量计是否开启
	if(LLJ_Start_Flage==0) 										// 流量计关闭
	{
		Flow_Now_Value=0;												// 水流速
		Flow_Now_Fre=0;													// 频率为0
		return;
	}
	if(LLJ_Now_Value==0) return ;							// 保护
	// 计算流速对应的频率
	hz=1.0/((double)LLJ_Now_Value/100000.0);	// 计算出当前频率
	// 计算当前流速
	Flow_Now_Fre=hz;													// 当前水轮频率
	Flow_Now_Value=hz/36.0;										// 当前水流速
	// 当前用水量
	Last_Float_Flow+=Flow_Now_Value/60.0;			// 当前用水量
	// 累计用水量
	Toatl_Float_Flow+=Flow_Now_Value/60.0;		// 当前用水量
	// 累计反洗水量
	Fanxi_Float_Flow+=Flow_Now_Value/60.0;		// 累计反洗水量
}

/***********************************************************************************
 * 函数名：void Back_Wash_Oper(void)
 * 描述  ：反清洗
 * 输入  ：无
 * 输出  : 获取到数据返回1 未获取数据则返回0
 * 调用  ：外部调用 AA 0A 06 84 05 12 34 56 78 85 BB
**********************************************************************************/
void Back_Wash_Oper(void)
{
	uint8_t strength=3;
	
	// 该值根据TDS调整
	if(Fanxi_Float_Flow>Auto_Fanxi_Value&&Queshi_Flage==0)	// 不缺水状态下才能反洗
	{
		Fanxi_Float_Flow=0;
		Auto_Fanxi_Flage=1;
	}
	// 最大TDS
	switch(Fanxi_Step)
	{
		case 0: // 等待清洗模式
						if(Auto_Fanxi_Flage==1&&Fanxi_Delay_Time==0)	// 清洗标志置位
						{
							Fanxi_Continue_Time=FANXI_CON_TIME;		// 反洗持续3分钟
							Fanxi_Mode=1;													// 反洗标志
							Auto_Fanxi_Flage=0;										// 
							Fanxi_Flage=0;												// 复位该标志
							Fanxi_Step=1;													// 进入清洗模式
							ST_VA_SET(1);													// 打开三通阀
							JS_VA_SET(1);													// 设置进水阀打开	
						}	
						if(Fanxi_Flage==1)											// 手动反洗
						{
							Fanxi_Continue_Time=FANXI_CON_TIME;		// 反洗持续3分钟
							Fanxi_Mode=1;													// 反洗标志
							Auto_Fanxi_Flage=0;										// 
							Fanxi_Flage=0;												// 复位该标志
							Fanxi_Step=1;													// 进入清洗模式		
							ST_VA_SET(1);													// 打开三通阀
							JS_VA_SET(1);													// 设置进水阀打开	
						}
						break;
		case 1:	// 打开电磁阀
						if(Probe_PP&&Probe_C1&&Probe_UDF&&Probe_C2)		// 滤芯正常
						{			
							ELE_CONA_SET(0);											// 反洗过程中
							ELE_CONB_SET(1);											// 反洗过程中	继电器负极吸合
							delay_ms(50);													// 延时
							LED_DJ=1;															// 电解指示灯点亮		
						}
						LED_CX=1;																// 冲洗					
						SS_VA_SET(1);														// 打开酸水阀
						Fanxi_Step=2;														// 进入制水模式
						break;
		case 2:	// 开始电解
						HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); 		// 打开PWM
						if(Probe_PP&&Probe_C1&&Probe_UDF&&Probe_C2)		// 滤芯正常
						{
							Start_Cur_PID(strength);							// 开启电解电压							
						}
						Fanxi_Step=3;														// 进入制水模式	
						break;
		case 3:	// 清洗中等待用户关闭
						if(LLJ_Start_Flage==1)									// 水轮旋转停止清洗
						{
							Stop_Cur_PID();												// 将电解电压调低		
							HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
							delay_ms(50);													// 延时
							Chongxi_Continue_Time=CHONGXI_CON_TIME;	// 冲洗持续时间2分钟
							Fanxi_Step=4;													// 停止制水
						}
						if(Fanxi_Continue_Time==0)							// 反洗结束
						{
							Stop_Cur_PID();												// 将电解电压调低		
							HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
							delay_ms(50);													// 延时
							Chongxi_Continue_Time=CHONGXI_CON_TIME;	// 冲洗持续时间2分钟
							Fanxi_Step=4;													// 停止制水
						}
						if(Fanxi_Mode==0)												// 用户手动关闭反洗
						{
							Stop_Cur_PID();												// 将电解电压调低	
							HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);							
							delay_ms(50);													// 延时
							Chongxi_Continue_Time=CHONGXI_CON_TIME;	// 冲洗持续时间2分钟
							Fanxi_Step=4;													// 停止制水						
						}
						break;
		case 4:	// 冲洗废水
						ELE_CONA_SET(0);												// 停止电解
						ELE_CONB_SET(0);												// 停止电解
						LED_DJ=0;																// 电解指示灯熄灭
						if(LLJ_Start_Flage==1)									// 水轮旋转停止清洗
						{					
							ST_VA_SET(0);													// 关闭三通阀
							Fanxi_Step=5;													// 停止制水
						}
						if(Chongxi_Continue_Time==0)						// 反洗结束
						{					
							ST_VA_SET(0);													// 关闭三通阀
							delay_ms(50);													// 延时
							Fanxi_Step=5;													// 停止制水
						}	
						if(Fanxi_Mode==0)												// 用户手动关闭反洗
						{
							ST_VA_SET(0);													// 关闭三通阀
							delay_ms(50);													// 延时
							Fanxi_Step=5;													// 停止制水
						}
						break;
		case 5:	// 停止冲洗
						Fanxi_Num++;Fanxi_Float_Flow=0;					// 反洗流量清零
						HAL_FLASH_Unlock();															// Flash 解锁
						EE_WriteVariable(VirtAddVarTab[2],Fanxi_Num);		// 读取累计反洗次数
						HAL_FLASH_Lock();																// Flash 锁定
						ELE_CONA_SET(0);												// 停止电解
						ELE_CONB_SET(0);												// 电解制取碱水									
						SS_VA_SET(0);														// 关闭酸水阀		
						ST_VA_SET(0);														// 关闭三通阀
						Fanxi_Mode=0;														// 停止反洗	
						LED_ZS=0;																// 制水指示灯点亮
						LED_DJ=0;																// 电解指示灯点亮
						LED_CX=0;																// 冲洗
						Fanxi_Step=0;														// 
						Fanxi_Mode=0;														//
						break;
		default : // 
						break;
	}	
}


void Read_System_Info(void)
{
	uint16_t state=0,flow=0;
	
	EE_ReadVariable(VirtAddVarTab[0], &state);					// 读取设备编码
	if(state==0xa5a5)
	{
		EE_ReadVariable(VirtAddVarTab[1], &state);				// 读取设备编码
		if(state>3) state=1;ELE_Strength=state;						// 默认弱碱
		EE_ReadVariable(VirtAddVarTab[2], &flow);					// 读取累计反洗次数
		Fanxi_Num=flow;																		// 反洗次数
		EE_ReadVariable(VirtAddVarTab[3], &flow);					// 读取累计水量
		Toatl_Float_Flow=flow;														// 累计水量
	}
	else
	{
		ELE_Strength=1;Fanxi_Num=0;												//
		EE_WriteVariable(VirtAddVarTab[0],0xa5a5);				// 保存电解强度
		EE_WriteVariable(VirtAddVarTab[1],ELE_Strength); 	// 保存电解强度
		EE_WriteVariable(VirtAddVarTab[2],Fanxi_Num);			// 读取累计反洗次数
		EE_WriteVariable(VirtAddVarTab[3],0);							// 清除累计水量
	}
}




// end
