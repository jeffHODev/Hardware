#ifndef __ADS129x_H
#define __ADS129x_H	 

//#include "sys.h" 
#include "myQueue.h"


///////////////////////////////////////////////////////////////////////////////
//оƬ������ѡ��
#define	ADS1298		1 
#define	ADS1299		2
#define	ADS1292		3
#define		SEND_BULE			 1 //��������
#define		SEND_WIFI			 2 //WIFI����
#define		SEND_UART			 3 //�����ڷ���
#define		WRITE_TF	 		 4 //дSD��
#define		SEND_USB			 5 //USB����
#define	ADS129x_USE_SPI1		1
#define	ADS129x_USE_SPI2		2
///////////////////////////////////////////////////////////////////////////////
//оƬ������ѡ��
#define ADS129X_DEVICE			ADS1299	 	//оƬѡ��
#define ADS129X_UART_DEBUG	0 	//�Ĵ������ڵ���	 //1 ʹ�üĴ������ڵ��� 0������
#define ADS129X_CASCADE_NUM	1		//�豸��������
#define ADS129X_USE_CANNLE	8 	//оƬʹ��N��ͨ��
//��������ѡ����ȼ� ��ݲ�>����>������
#define ADS129X_USE_TRIANGLE_TESTDATA 	0 		//ͨ��1ʹ�þ�ݲ������������� 	//1 ʹ�ò������� 0��ʹ�� �������ݣ���ʹ��ADS129X
#define ADS129X_USE_SQUARE_TESTDATA 		0 		//ʹ���ڲ��������� 				//1 ʹ�ò������� 0��ʹ�� �������ݣ�ͨ��ADS129X�ɼ�
#define	ADS129X_USE_NOISE_TESTDATA			1			//�����ڲ��̽�����
////////////////////////////////////////////////////////////////////////////////
//���ż�ͨѶ�ӿ�
#define ADS129X_DRDY_LINE			EXTI_Line0//DRDY�ж�ͨ��
#define ADS129X_SPI 					ADS129x_USE_SPI1	//SPIѡ��
#define ADS129X_DRDY					PBin(0)
#define ADS129X_CS						PAout(4)		
//	#define ADS129X_RESET 	PAout(0)
//	#define ADS129X_START		PAout(2)	
//	#define ADS129X_CLKSEL	PAout(3)	
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
typedef struct
{
		u8 Ads129x_Cascade_Num;//оƬ��������
		u8 Ads129x_Use_Cannle;//һ��оƬʹ��N��ͨ��
	
		u16 Ads129x_Data_Move; //ÿ���жϰ���N������ ADS129X_Data_Move =  Ads129x_USE_Cannle*3
		u16 Ads129x_Write_Num; //����N�� ��һ����   ADS129x_Write_Num = ADS129x_SAMPLING_POINT / Ads129x_USE_Cannle

		QueueInfo *ADS129x_Queue;//���ݻ�����ָ��
}_ADS129x_info;
extern _ADS129x_info ADS129x_info;//оƬ�ṹ
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//оƬ�Ĵ����������ж϶�ȡ���ֽ���
#if ADS129X_DEVICE == ADS1298 
	#define ADS129X_REGNUM 26			//�Ĵ�������
	#define ADS129X_DATANUM 27		//һ��DRDY�жϵ����ݶ�ȡ����
#elif ADS129X_DEVICE == ADS1299
	#define ADS129X_REGNUM 24			//�Ĵ�������
	#define ADS129X_DATANUM 27		//���ݶ�ȡ����
#elif ADS129X_DEVICE == ADS1292
	#define ADS129X_REGNUM 12			//�Ĵ�������
	#define ADS129X_DATANUM 9			//���ݶ�ȡ����
#endif
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//�����ⲿ����
extern u8 work_state;//ϵͳ����״̬
//оƬ��ر���
extern	u8 ADS129x_REG_BUF[ADS129X_REGNUM];//�Ĵ�������
extern	u8 data_to_send[50];//���ڷ��ͻ���
extern	u32 cannle[8];	//�洢8��ͨ��������g
extern	int32_t	p_Temp[8];	//���ݻ���
extern volatile u8 ads129x_Cache[27];		//129x���ݻ�����
extern volatile u8 ads129x_read_flag;	//DRDY ��ȡ���1�α�־
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//����DEBUG	
#if ADS129X_UART_DEBUG == 1 
		#define ADS129X_DEBUG(...)    Main_printf(__VA_ARGS__)
#else
		#define ADS129X_DEBUG(...)		
#endif
////////////////////////////////////////////////////////////////////////////////	

/////////////////////////////////////////////////////////////////////////////////////////////
//�����
//ע�⣺ ����Ĭ�ϴ��� RDATAC ģʽʱ��Ҫ��д�Ĵ��������ȷ���SDATAC����
//����������� RDATAC ģʽ��������ȷ��� SDATAC���Ȼ����ܽ��κ���������͵������С�
//ϵͳ����
	#define ADS129X_WAKEUP	0X02	//�Ӵ���ģʽ����
	#define ADS129X_STANDBY	0X04	//�������ģʽ
	#define ADS129X_RESET	0X06	//��λ
	#define ADS129X_START	0X08	//����ת��
	#define ADS129X_STOP	0X0A	//ֹͣת��
//���ݶ�ȡ����
	#define ADS129X_RDATAC	0X10	//�������������ݶ�ȡģʽ,Ĭ��ʹ�ô�ģʽ
	#define ADS129X_SDATAC	0X11	//ֹͣ���������ݶ�ȡģʽ
	#define ADS129X_RDATA	0X12	//ͨ�������ȡ����;֧�ֶ�����ء�
//�Ĵ�����ȡ����
	//r rrrr=Ҫ����д�ļĴ����׵�ַ	 //	n nnnn=Ҫ��д�ļĴ�������
	#define ADS129X_RREG	0X20	//��ȡ  001r rrrr(���ֽ�) 000n nnnn(2�ֽ�) 
	#define ADS129X_WREG	0X40	//д��  010r rrrr(���ֽ�) 000n nnnn(2�ֽ�)
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
int32_t get_volt(u32 num);//�Ѳɵ���3���ֽ�ת���з���32λ��

void ADS129x_IO_Init(void); //��ʼ��ADS1292����
void ADS129x_ReInit(u8 cmd);//�ظ���ʼ��

__INLINE u8 ADS129x_SPI_ReadWriteByte(u8 com);		//SPIͨ��
__INLINE void ADS129x_Read_Data(void);//72Mʱ���º�����ʱ��Լ10us  8Mʱ���� ������ʱ��Լ 100us

void ADS129x_Send_CMD(u8 data);//��������
void ADS129x_WR_REGS(u8 reg,u8 len,u8 *data);//��д�Ĵ���
u8 ADS129x_WRITE_REGBUFF(void);//���Ĵ�������д��Ĵ���
u8 ADS129x_REG_Init(void);//�Ĵ�����ʼ��

#endif


/* //Ĭ�ϼĴ���ֵ 1298
0 92
1 6
2 0
3 40
4 0
5 0
6 0
7 0
8 0
9 0
10 0
11 0
12 0
13 0
14 0
15 0
16 0
17 0
18 0
19 0
20 f
21 0
22 0
23 0
24 0
25 0
*/

/* //Ĭ�ϼĴ���ֵ 1299
0 3e
1 96
2 c0
3 60
4 0
5 61
6 61
7 61
8 61
9 61
10 61
11 61
12 61
13 0
14 0
15 0
16 0
17 0
18 0
19 0
20 f
21 0
22 0
23 0
*/

