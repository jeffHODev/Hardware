#ifndef CONFIG_H
#define CONFIG_H
typedef  unsigned char	uchar; 
typedef  unsigned int	uint;
typedef  unsigned long	ulong; 
/**********�̵�����������******************/
	
//===========ʱ��FLAG===================== 
unsigned char  T10ms_flag;                //10ms���ڰ���ɨ��  
unsigned char  T100ms_flag; 
unsigned char  T500ms_flag;
unsigned char  T1s_flag;	
unsigned char  T10s_flag;	
unsigned char   deal_flag=0;               //������ϱ�־ 		
unsigned char  onetimes_flag=0;	       //һ�β�����־
unsigned char  over_time_flag=0;		   //��ʱ��־	  
//======================ʱ�����===============	
unsigned  char  T1s=0;	                   //1����
unsigned  char  T500ms=0; 
unsigned  char  T100ms=0;	
unsigned  char  T10ms=0;	  
unsigned int   T10s=0;                     //10���� 
//=====================��������================	 
unsigned  char   alarm_count=0;         //û�а�ʱ�ﵽ���񱨾�
unsigned  char   ww,gw,sw,bw,qw=0;
unsigned  char   BX_count=0;	  
//====================================
unsigned  char     alarm=0;
unsigned  char     samp_process=0;       //��������
unsigned  char     peak=0;               //���ֵ
unsigned  char     valley=0;
unsigned  char     Freq=0;               //����
//unsigned  char     MB=0;
unsigned  char     XYMB[2];
unsigned  char     step=0;
unsigned  char     cycle=0;               //���� 
//unsigned  char     cycle1=0;              //����1
//unsigned  char     cycle2=0;              //����2
unsigned  char     changdu=0;	   
unsigned  char     time1=0;	            //��һ�ε����ֵ
unsigned  char     time2=0;				//�ڶ��ε����ֵ
unsigned  char     samp_count=0;         //���μ���
unsigned  char     samp_count2=0;        //���μ���
unsigned  char     samp_data=0;          //��������	  
unsigned  char     trig_count=0;         //��������Ϊ0
unsigned int      Pulse_count=0;        //������
unsigned int      SPO2=0;               //Ѫ�����Ͷ� 
unsigned int      sum=0; 	
unsigned int      RS232_cnt=0;	        //���ڼ���
unsigned int      times=0;		
unsigned int      F_IRLED=0;            //�����Ƶ��
unsigned int      F_IRLED_max=0;        //�����Ƶ�����
unsigned int      F_IRLED_min=0;        //�����Ƶ����С
unsigned int      F_IRLED_err=0;        //�����Ƶ�ʸı���
unsigned int      F_max=0;              //���Ƶ��
unsigned int      F_min=0;              //��СƵ��
unsigned int      F_max2=0;             //���Ƶ��
unsigned int      F_min2=0;             //��СƵ��
unsigned int      F_change=0;           //Ƶ�ʸı�
unsigned int      F_change2=0;          //Ƶ�ʸı�
unsigned int      F_back=0;             //����Ƶ��	  
unsigned int      F_RLED=0;             //���Ƶ��
unsigned int      F_RLED_max=0;         //���Ƶ�����
unsigned int      F_RLED_min=0;         //���Ƶ����С
unsigned int      F_RLED_err=0;         //���Ƶ�ʸı���
//=====================================	  
unsigned  char     MB_buf[3];       //�仯Ƶ��
unsigned  char     SPO2_buf[2];     //�仯Ƶ�� 
unsigned  char     store_buf[100];  //�������Ϊ100
unsigned  char     T_buf[3];        //�������Ϊ100
unsigned int      filter_buf[16]; 
unsigned int      filter_SPO2[16]; 
unsigned int      UART_R_buf[200];   //�洢200����������
unsigned int      UART_IR_buf[200];  //�洢200���������� 
float   R=0;                         //����õ���Rֵ	  
//�����ӵı���
unsigned char    tt,tttt;
unsigned char    MB_max=0;
unsigned char    MB_min=0;	  
unsigned char    time3=0;		       //�ڶ��ε����ֵ
unsigned int     changdu1=0;           //���¶����
unsigned char    diuqi=20;
unsigned int     temp=0;
//key
unsigned char    qiehuan_flg=0;
unsigned char    lock_flg=0;         //��Ļ������
//2014.10.20�����
unsigned char       page=0; 	  //�˵�ҳ��
unsigned char       ALM_flg=0;
unsigned char       beep_flg=0;
unsigned char       bluetooth_flg=0;
unsigned char       Sounds_flg=0;
unsigned char       SPO2HI=0;      //Ѫ������
unsigned char       SPO2LO=0;      //Ѫ������
unsigned char       PRHI=0;        //��������
unsigned char       PRLO=0;        //��������
unsigned char       SPO2HI_buf[3];      //Ѫ������	 
unsigned char       SPO2LO_buf[3];      //Ѫ������
unsigned char       PRHI_buf[3];        //��������
unsigned char      PRLO_buf[3];        //��������
unsigned char      flash_flg=0;							  //��˸��־	
char               key_tree=0;  //������	
unsigned char      key_flag=0;  //������־   

char               shift=0;        //��λ  
char               add_dec=0;      //�Ӽ�  	
unsigned  char     fengming_flg=0; //������־
unsigned  char     fengming_cnt=0; //��������	   
unsigned  char     finger_flg=0; //����ָ��־
unsigned  char     nofinger_cnt=0; //��������	
unsigned  char     shanshuo_cnt=0; //��������	
///===STM32������========
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

//--------------------------TP����------------------------------


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
unsigned char  device_address=0;        //�豸�ţ�һ���ֽ� 
unsigned char  Uart3_Get_Flag=0;
unsigned char  Uart3_Get_Data=0;
unsigned char rec;

//���ڱ�������
unsigned char RS232BUSY=0; //232æµ����
unsigned char RS232cnt=0;  //232����		  
unsigned char RX_status;
unsigned char incept[9]; //���ڽ������ݻ���
unsigned char n=0;         //��ʱ���� 
unsigned char  DAT_SEL=0;  //������ѯ
unsigned char  send_canshu_flag=0;
//unsigned char  Power_Left=50,Power_Right=50;	     //����Ĭ�Ϲ���Ϊ50%

unsigned int in_cnt[9];                //2013-3-10�����
char send_buf[TBUF_SIZE],cbuf[TBUF_SIZE];
//

unsigned short ad[10],ss=0,sss=10,DO=0,DO0=0xffff,cc[8],rs485_buf[20];

char ips[40][30]={0},ipv[40][16]={0},ipv2[40]={0},ipt[40]={0},ipt2[40]={0},ipi=0,inp=255;
char pw[4],pw1[4]={0},ki=0,k0=0;

//���в���
unsigned char ir_code[40]={0},ir_i=0,ir_sp=0;
unsigned short ir=0,ir_bz=0x10;
unsigned char sj[800]={0};
unsigned char x0[8]={0},x1[8]={0},y0[8]={0},y1[8]={0};
unsigned int  vv[18],v[18],vset[18],ad_fx[18];
unsigned short sample[18];
unsigned short vt[7]={0,0,0,0,0,0,0};
int MBAP=0;
unsigned short bps=96;

static u8 mymac[6] ={0x54, 0x55, 0x58, 0x10, 0x00, 0x24};	  //�ҵĻ���
static u8 myip[4] ={192, 168, 1, 200};						  //����ENC28J60
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


