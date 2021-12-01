
#define SWITCH_PERIOD		        1.5*60*100

#define ELE_PERIOD		        ELE_TIME*60*100
#define ELE_TIME				1.5
#define MIN_CURRENT      0.85
#define MAX_CURRENT      2.5
#define RS      0.2

#define Refvalue 1.22												// 带隙参考源电压
#define Ntc_Num  126												// 温度传感器测温范围
#define FIR_NUM  25													// 滤波参数
#define MUSIC_NUM1  3												// 最小值为2
#define MUSIC_NUM2  3												// 最小值为2
#define MUSIC_NUM3  5												// 最小值为2
#define MUSIC_NUM4  7												// 最小值为2
#define MUSIC_NUM5  5												// 最小值为2
#define MUSIC_NUM6  5												// 最小值为2
#define BUTTON_FILTER_TIME     20						// 50ms
#define BUTTON_LONG_TIME        200000					// 1500ms
#define CHARGE_LONG_TIME        20000					// 1500ms
#define FCHARGE_LONG_TIME       40000					// 1500ms

#define BATTER_DS								3.5*2				// 放电过程中电压低于该值关机
#define BATTER_01								3.411*2				// 01%电量	无电红灯
#define BATTER_30								3.663*2				// 30%电量	一个绿灯
#define BATTER_60								3.872*2				// 60%电量	两个绿灯
#define BATTER_90								4.075*2				// 90%电量	三个绿灯

#define BATTER_ALARM							3.75*2				// 90%电量	三个绿灯
#define MAX_VBAT                                8.35
#define MIN_VBAT                                2

#define MAX_TEMP                50					// 最高可电解温度
#define MIN_TDSVALUE            10         // 升压幅值低于10V停止电解

#define Key_Pump	P10											// 喷雾控制
#define Key_Set     P00											// 设置按键
#define Charge_Set  P01											// 充电器插入
//#define Stdby_Set   P03											// 充电已满
#define DJ_Con			P05											// 电解控制

#define CLK_Con			P11											// 指示灯信号
#define RCK_Con			P12											// 使能控制
#define Clear_Con		P13											// 指示灯清零
#define DAT_Con			P14											// 指示灯数据

#define Pow_Con			P15											// 电源控制引脚

typedef enum
{
    KEY_NULL=0,
    KEY_START_UP,
    KEY_START_DOWN,
    KEY_START_HOLD,
    KEY_HNULL=0,
    KEY_HSTART_UP=0x10,
    KEY_HSTART_DOWN=0x20,
    KEY_HSTART_HOLD=0x30,


} KEY_ENUM;
typedef enum
{
    //none // switch //none //led dia1 //pumpled green
    LED_BLUE=1,
    LED_RED=4,
    LED_GREEN=2,
    PUMP=8,
    DIA1=0x10,
    SWITCH=0x40,


} Dev;

extern uint8_t Ele_Flage;										// 开始电解标志
extern uint8_t Set_Time;										// 设置电解时长 3-4-5分钟
extern uint8_t Speak_Flage;									// 蜂鸣器      0 关闭  1 开机   2 按键音  3 提示音  4 报警音
extern uint8_t LED_Con;											// LED指示灯
extern uint16_t Ele_Time;										// 电解时间
extern uint16_t All_Use_Time;								// 累计使用次数
extern uint16_t Temp_Alarm_Num;							// 温度报警数值
extern uint16_t TDS_Alarm_Num;							// TDS报警数值
extern uint8_t Led_RamG[3];								  // 绿光ROM
extern uint8_t Led_RamR[3];								  // 红光ROM


//void Init_pwm_code(void);
void READ_BANDGAP(void);
void Send_Info(void);
void Board_gpio_init(void);
void Init_adc_read(void);
void Timer1_init(void);
void Speak_value(void);
void Set_Speak_Value(uint8_t flage);
uint8_t Button_Read(unsigned char flag);
void Led_Display(void);
void Work_In_Set(uint8_t keycode);
void display_init(void);

void Display_Process(void);
void key_reset(void);

unsigned char Charge_Process(void);
void testt(void);