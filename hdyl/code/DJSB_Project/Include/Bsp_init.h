
#define SWITCH_PERIOD		        1.5*60*100

#define ELE_PERIOD		        ELE_TIME*60*100
#define ELE_TIME				1.5
#define MIN_CURRENT      0.85
#define MAX_CURRENT      2.5
#define RS      0.2

#define Refvalue 1.22												// ��϶�ο�Դ��ѹ
#define Ntc_Num  126												// �¶ȴ��������·�Χ
#define FIR_NUM  25													// �˲�����
#define MUSIC_NUM1  3												// ��СֵΪ2
#define MUSIC_NUM2  3												// ��СֵΪ2
#define MUSIC_NUM3  5												// ��СֵΪ2
#define MUSIC_NUM4  7												// ��СֵΪ2
#define MUSIC_NUM5  5												// ��СֵΪ2
#define MUSIC_NUM6  5												// ��СֵΪ2
#define BUTTON_FILTER_TIME     20						// 50ms
#define BUTTON_LONG_TIME        200000					// 1500ms
#define CHARGE_LONG_TIME        20000					// 1500ms
#define FCHARGE_LONG_TIME       40000					// 1500ms

#define BATTER_DS								3.5*2				// �ŵ�����е�ѹ���ڸ�ֵ�ػ�
#define BATTER_01								3.411*2				// 01%����	�޵���
#define BATTER_30								3.663*2				// 30%����	һ���̵�
#define BATTER_60								3.872*2				// 60%����	�����̵�
#define BATTER_90								4.075*2				// 90%����	�����̵�

#define BATTER_ALARM							3.75*2				// 90%����	�����̵�
#define MAX_VBAT                                8.35
#define MIN_VBAT                                2

#define MAX_TEMP                50					// ��߿ɵ���¶�
#define MIN_TDSVALUE            10         // ��ѹ��ֵ����10Vֹͣ���

#define Key_Pump	P10											// �������
#define Key_Set     P00											// ���ð���
#define Charge_Set  P01											// ���������
//#define Stdby_Set   P03											// �������
#define DJ_Con			P05											// ������

#define CLK_Con			P11											// ָʾ���ź�
#define RCK_Con			P12											// ʹ�ܿ���
#define Clear_Con		P13											// ָʾ������
#define DAT_Con			P14											// ָʾ������

#define Pow_Con			P15											// ��Դ��������

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

extern uint8_t Ele_Flage;										// ��ʼ����־
extern uint8_t Set_Time;										// ���õ��ʱ�� 3-4-5����
extern uint8_t Speak_Flage;									// ������      0 �ر�  1 ����   2 ������  3 ��ʾ��  4 ������
extern uint8_t LED_Con;											// LEDָʾ��
extern uint16_t Ele_Time;										// ���ʱ��
extern uint16_t All_Use_Time;								// �ۼ�ʹ�ô���
extern uint16_t Temp_Alarm_Num;							// �¶ȱ�����ֵ
extern uint16_t TDS_Alarm_Num;							// TDS������ֵ
extern uint8_t Led_RamG[3];								  // �̹�ROM
extern uint8_t Led_RamR[3];								  // ���ROM


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