#ifndef CONFIG_H
#define CONFIG_H
typedef  unsigned char	uchar; 
typedef  unsigned int	uint;
typedef  unsigned long	ulong; 
/**********继电器、蜂鸣器******************/
	
//===========时间FLAG===================== 
unsigned char  T10ms_flag;                //10ms用于按键扫描  
unsigned char  T100ms_flag; 
unsigned char  T500ms_flag;
unsigned char  T1s_flag;	
unsigned char  T10s_flag;	
unsigned char   deal_flag=0;               //处理完毕标志 		
unsigned char  onetimes_flag=0;	       //一次测量标志
unsigned char  over_time_flag=0;		   //超时标志	  
//======================时间变量===============	
unsigned  char  T1s=0;	                   //1秒钟
unsigned  char  T500ms=0; 
unsigned  char  T100ms=0;	
unsigned  char  T10ms=0;	  
unsigned int   T10s=0;                     //10秒钟 
//=====================新增报警================	 
unsigned  char   alarm_count=0;         //没有按时达到任务报警
unsigned  char   ww,gw,sw,bw,qw=0;
unsigned  char   BX_count=0;	  
//====================================
unsigned  char     alarm=0;
unsigned  char     samp_process=0;       //采样流程
unsigned  char     peak=0;               //峰谷值
unsigned  char     valley=0;
unsigned  char     Freq=0;               //报警
//unsigned  char     MB=0;
unsigned  char     XYMB[2];
unsigned  char     step=0;
unsigned  char     cycle=0;               //周期 
//unsigned  char     cycle1=0;              //周期1
//unsigned  char     cycle2=0;              //周期2
unsigned  char     changdu=0;	   
unsigned  char     time1=0;	            //第一次的序号值
unsigned  char     time2=0;				//第二次的序号值
unsigned  char     samp_count=0;         //波形计数
unsigned  char     samp_count2=0;        //波形计数
unsigned  char     samp_data=0;          //采样数据	  
unsigned  char     trig_count=0;         //触发计数为0
unsigned int      Pulse_count=0;        //脉冲检测
unsigned int      SPO2=0;               //血氧饱和度 
unsigned int      sum=0; 	
unsigned int      RS232_cnt=0;	        //串口计数
unsigned int      times=0;		
unsigned int      F_IRLED=0;            //红外光频率
unsigned int      F_IRLED_max=0;        //红外光频率最大
unsigned int      F_IRLED_min=0;        //红外光频率最小
unsigned int      F_IRLED_err=0;        //红外光频率改变量
unsigned int      F_max=0;              //最大频率
unsigned int      F_min=0;              //最小频率
unsigned int      F_max2=0;             //最大频率
unsigned int      F_min2=0;             //最小频率
unsigned int      F_change=0;           //频率改变
unsigned int      F_change2=0;          //频率改变
unsigned int      F_back=0;             //背景频率	  
unsigned int      F_RLED=0;             //红光频率
unsigned int      F_RLED_max=0;         //红光频率最大
unsigned int      F_RLED_min=0;         //红光频率最小
unsigned int      F_RLED_err=0;         //红光频率改变量
//=====================================	  
unsigned  char     MB_buf[3];       //变化频率
unsigned  char     SPO2_buf[2];     //变化频率 
unsigned  char     store_buf[100];  //最大容量为100
unsigned  char     T_buf[3];        //最大容量为100
unsigned int      filter_buf[16]; 
unsigned int      filter_SPO2[16]; 
unsigned int      UART_R_buf[200];   //存储200个串口数据
unsigned int      UART_IR_buf[200];  //存储200个串口数据 
float   R=0;                         //计算得到的R值	  
//后增加的变量
unsigned char    tt,tttt;
unsigned char    MB_max=0;
unsigned char    MB_min=0;	  
unsigned char    time3=0;		       //第二次的序号值
unsigned int     changdu1=0;           //最新定义的
unsigned char    diuqi=20;
unsigned int     temp=0;
//key
unsigned char    qiehuan_flg=0;
unsigned char    lock_flg=0;         //屏幕锁定键
//2014.10.20后添加
unsigned char       page=0; 	  //菜单页数
unsigned char       ALM_flg=0;
unsigned char       beep_flg=0;
unsigned char       bluetooth_flg=0;
unsigned char       Sounds_flg=0;
unsigned char       SPO2HI=0;      //血氧缓冲
unsigned char       SPO2LO=0;      //血氧缓冲
unsigned char       PRHI=0;        //脉搏缓冲
unsigned char       PRLO=0;        //脉搏缓冲
unsigned char       SPO2HI_buf[3];      //血氧缓冲	 
unsigned char       SPO2LO_buf[3];      //血氧缓冲
unsigned char       PRHI_buf[3];        //脉搏缓冲
unsigned char      PRLO_buf[3];        //脉搏缓冲
unsigned char      flash_flg=0;							  //闪烁标志	
char               key_tree=0;  //按键树	
unsigned char      key_flag=0;  //按键标志   

char               shift=0;        //移位  
char               add_dec=0;      //加减  	
unsigned  char     fengming_flg=0; //蜂鸣标志
unsigned  char     fengming_cnt=0; //蜂鸣计数	   
unsigned  char     finger_flg=0; //无手指标志
unsigned  char     nofinger_cnt=0; //蜂鸣计数	
unsigned  char     shanshuo_cnt=0; //蜂鸣计数	
///===STM32后增加========
#define SPK0         GPIO_ResetBits(GPIOC, GPIO_Pin_5)
#define SPK1         GPIO_SetBits(GPIOC, GPIO_Pin_5)
#define RS485_RE0    GPIO_ResetBits(GPIOA, GPIO_Pin_11)
#define RS485_RE1    GPIO_SetBits(GPIOA, GPIO_Pin_11)

#define RS485_RE20   GPIO_ResetBits(GPIOA, GPIO_Pin_12)
#define RS485_RE21   GPIO_SetBits(GPIOA, GPIO_Pin_12)

#define TBUF_SIZE   128
#define RBUF_SIZE   128

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
uint32_t ret; /* for return of the interrupt handling */
volatile TestStatus TestRx;
//ErrorStatus HSEStartUpStatus;

//--------------------------TP结束------------------------------


#define ADC1_DR_Address    ((u32)0x4001244C)
#define FLASH_ADDR		((u32)0x803f800)
u32 *pp=(u32 *)(FLASH_ADDR);
//time_t current_time;
//struct tm time_now;
//vu32 TimeDisplay;
//FATFS fs;            // Work area (file system object) for logical drive
//FIL fsrc, fdst;      // file objects
//FRESULT res;         // FatFs function common result code
//unsigned int br, bw;         // File R/W count


unsigned char addr=0,DI=0,send_len=0,ss2=0,mi=0,mi0=255;
unsigned char  device_address=0;        //设备号，一个字节 
unsigned char  Uart3_Get_Flag=0;
unsigned char  Uart3_Get_Data=0;
unsigned char rec;

//串口变量定义
unsigned char RS232BUSY=0; //232忙碌计数
unsigned char RS232cnt=0;  //232计数		  
unsigned char RX_status;
unsigned char incept[9]; //串口接收数据缓冲
unsigned char n=0;         //临时变量 
unsigned char  DAT_SEL=0;  //数据轮询
unsigned char  send_canshu_flag=0;
//unsigned char  Power_Left=50,Power_Right=50;	     //开机默认功率为50%

unsigned int in_cnt[9];                //2013-3-10后添加
char send_buf[TBUF_SIZE],cbuf[TBUF_SIZE];
//

unsigned short ad[10],ss=0,sss=10,DO=0,DO0=0xffff,cc[8],rs485_buf[20];

char ips[40][30]={0},ipv[40][16]={0},ipv2[40]={0},ipt[40]={0},ipt2[40]={0},ipi=0,inp=255;
char pw[4],pw1[4]={0},ki=0,k0=0;

//运行参数
unsigned char ir_code[40]={0},ir_i=0,ir_sp=0;
unsigned short ir=0,ir_bz=0x10;
unsigned char sj[800]={0};
unsigned char x0[8]={0},x1[8]={0},y0[8]={0},y1[8]={0};
unsigned int  vv[18],v[18],vset[18],ad_fx[18];
unsigned short sample[18];
unsigned short vt[7]={0,0,0,0,0,0,0};
int MBAP=0;
unsigned short bps=96;

static u8 mymac[6] ={0x54, 0x55, 0x58, 0x10, 0x00, 0x24};	  //我的机号
static u8 myip[4] ={192, 168, 1, 200};						  //用于ENC28J60
// base url (you can put a DNS name instead of an IP addr. if you have
// a DNS server (baseurl must end in "/"):
static u16 mywwwport = 80;   // listen port for tcp/www (max range 1-254)
static u16 mytcpport = 1300; // listen port for tcp
static u16 myudpport = 1200; // listen port for udp

#define PSTR(s) s
#define BUFFER_SIZE 1500//400
static u8 buf[BUFFER_SIZE+1];  

struct buf_st {
  unsigned int in;                                // Next In Index
  unsigned int out;                               // Next Out Index
  char buf [RBUF_SIZE];                           // Buffer
};
#endif


