/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "system.h"
#include "filter.h"

// Page 0 : 0x46 0x44 0x42 0x58   FDBX
// Page 1 : UID
// Page 2 : PASSWORD
// Page 3 : OPT
// Page 4 :	Config
// Page 5 :	2018				��
// Page 6 :	1021				����
// Page 7 :	0000				���к�H
// Page 8 :	0001				���к�L
// Page 9 :	Config		  ����1	
// Page 10:	Config			����2
// Page 11:	Config			����3
// Page 12:	Config			����4
// Page 13:	Config			0000 �� FFFF ��
// Page 14:	RP
// Page 15:	RP

uint16_t PP_Filter_Life=0;							// 6���� 24*30*6
uint16_t C1_Filter_Life=0;							// 12����
uint16_t UDF_Filter_Life=0;							// 24����
uint16_t C2_Filter_Life=0;							// 12����

static uint8_t PP_RFID_Setp=1;					// ��ȡID���� ��Ҫ��1��ʼ����
static uint8_t C1_RFID_Setp=1;					// ��ȡID����
static uint8_t UDF_RFID_Setp=1;					// ��ȡID����
static uint8_t C2_RFID_Setp=1;					// ��ȡID����

static uint8_t PP_Read_Fail_Num=0;			// ��о��ȡʧ�ܴ���
static uint8_t C1_Read_Fail_Num=0;			// ��о��ȡʧ�ܴ���
static uint8_t UDF_Read_Fail_Num=0;			// ��о��ȡʧ�ܴ���
static uint8_t C2_Read_Fail_Num=0;			// ��о��ȡʧ�ܴ���

static uint8_t PP_RF_ID_Code[4];				// ��оID��ַ
static uint8_t C1_RF_ID_Code[4];				// ��оID��ַ
static uint8_t UDF_RF_ID_Code[4];				// ��оID��ַ
static uint8_t C2_RF_ID_Code[4];				// ��оID��ַ

/***********��од����־****************/
uint8_t PP_Write_Flage=0;								// PP����од��
uint8_t C1_Write_Flage=0;								// C1��од��
uint8_t UDF_Write_Flage=0;							// UDFод��
uint8_t C2_Write_Flage=0;								// C2од��

uint8_t Filter_Polling_Order=0;					// ��о��ѯ 0 PP��о  1 C1��о  2 UDF��о 3 C2��о

uint8_t Probe_PP=0;											// PP����о ʣ��
uint8_t Probe_C1=0;											// C1��о
uint8_t Probe_UDF=0;										// UD��о
uint8_t Probe_C2=0;											// C2��о
uint8_t LED_PP=0;												// PP����о
uint8_t LED_C1=0;												// C1��о
uint8_t LED_UDF=0;											// UD��о
uint8_t LED_C2=0;												// C2��о

__IO uint8_t Read_Filter_Flage=0;				// ��ȡ��о��־
/***********************************************************************************
 * ��������EM4305_Read_Data(uint8_t page,uint8_t data[4])
 * ����  ���ӿ��ж�ȡһҳ����
 * ����  ����
 * ���  : ��ȡ�����ݷ���1 δ��ȡ�����򷵻�0
 * ����  ���ⲿ����
**********************************************************************************/
uint8_t EM4305_Read_Data(uint8_t page)
{
	uint8_t buf[10];
	uint8_t i,err=0,temp;
	
	if(page>15) page=0;
	// ��д����
	buf[0]=0xAA;	buf[1]=0x0A;	buf[2]=0x02;	buf[3]=0x85;
	buf[4]=page;
	// У��
	for(i=1;i<5;i++)
	{
		err^=buf[i];
	}
	buf[5]=err; buf[6]=0xBB;
	// ��������
	temp=Uart1_SendData(&huart1,buf,7);
	if(temp!=1)
	{
		HAL_UART_DeInit(&huart1);
		MX_USART1_UART_Init();
	}
	
	return temp;
}
/***********************************************************************************
 * ��������EM4305_Read_Data(uint8_t page,uint8_t data[4])
 * ����  ���ӿ��ж�ȡһҳ����
 * ����  ����
 * ���  : ��ȡ�����ݷ���1 δ��ȡ�����򷵻�0
 * ����  ���ⲿ���� AA 0A 06 84 05 12 34 56 78 85 BB
**********************************************************************************/
uint8_t EM4305_Write_Data(uint8_t page,uint8_t data[4])
{
	uint8_t buf[15];
	uint8_t i,err=0,temp;

	if(page>15) page=0;
	// ��д����
	buf[0]=0xAA;	buf[1]=0x0A;	buf[2]=0x06;	buf[3]=0x84;
	buf[4]=page; 	buf[5]=data[0];	buf[6]=data[1];	buf[7]=data[2];	buf[8]=data[3];
	// У��
	for(i=1;i<9;i++)
	{
		err^=buf[i];
	}
	buf[9]=err; buf[10]=0xBB;
	// ��������
	temp=Uart1_SendData(&huart1,buf,11);
	if(temp!=1)
	{
		HAL_UART_DeInit(&huart1);
		MX_USART1_UART_Init();
	}

	return temp;
}

/***********************************************************************************
 * ��������void RF_Read_Info(uint8_t step)
 * ����  ������ģʽ
 * ����  ����
 * ���  ����
************************************************************************************/
void RFID_Read_Info(uint8_t *step)
{
	uint8_t data[4]={0,0,0,0};							// ����
	static uint8_t PP_Balanced_Bit=0;				// ����д�� 0-1
	static uint8_t C1_Balanced_Bit=0;				// ����д�� 0-1
	static uint8_t UDF_Balanced_Bit=0;			// ����д�� 0-1
	static uint8_t C2_Balanced_Bit=0;				// ����д�� 0-1
	// ��ȡ����
	switch(*step)
	{
		case 1:	// ��ȡ��ƬID
						EM4305_Read_Data(1);
						break;
		case 2:	// �жϿ�ƬID�Ƿ��ù�
						// �����оע�����ת��STEP=8
						// �����о�Ѿ�
						*step=3;
						break;
		case 3:	// ��ȡ��ʼ����־ UID^0x55AA55AA
						EM4305_Read_Data(3);
						break;
		case 4:	// �жϳ�ʼ��״̬
						*step=5;
						break;
		case 5: // ��ȡ��о����DBX
						EM4305_Read_Data(5); 
						break;
		case 6:	// ��ȡ��о���
						EM4305_Read_Data(6); 
						break;
		case 7:	// ��ȡ���ò�Ʒ����
						EM4305_Read_Data(7); 
						break;
		case 8: // ��ȡ��о����A	
						EM4305_Read_Data(8); 
						break;
		case 9:	// ��ȡ��о����B
						EM4305_Read_Data(9); 
						break;
		case 10:// �ж��Ƿ���Ҫд��																				// ���¶�ȡID
						switch(Filter_Polling_Order)															// ѭ����д
						{
							case 0:	// PP
											if(PP_Filter_Life>0)														// �����о��������
											{
												data[0]=(PP_Filter_Life-1)/256;								// ������
												data[1]=(PP_Filter_Life-1)%256;								// ������
												data[2]=data[0]^0xFF;													// У����
												data[3]=data[1]^0xFF;													// У����
												EM4305_Write_Data(8+PP_Balanced_Bit,data);		// ѭ����д8 9��������			
												if(PP_Balanced_Bit==1) PP_Balanced_Bit=0;			// ѭ����дλ
												else PP_Balanced_Bit=1;												// ѭ����дλ
											}		
											PP_Write_Flage=0;																// �Ѿ����д��
											break;
							case 1:	// C1
											if(C1_Filter_Life>0)														// �����о��������
											{
												data[0]=(C1_Filter_Life-1)/256;								// ������
												data[1]=(C1_Filter_Life-1)%256;								// ������
												data[2]=data[0]^0xFF;													// У����
												data[3]=data[1]^0xFF;													// У����
												EM4305_Write_Data(8+C1_Balanced_Bit,data);		// ѭ����д8 9��������			
												if(C1_Balanced_Bit==1) C1_Balanced_Bit=0;			// ѭ����дλ
												else C1_Balanced_Bit=1;												// ѭ����дλ
											}		
											C1_Write_Flage=0;																// �Ѿ����д��
											break;
							case 2:	// UDF
											if(UDF_Filter_Life>0)														// �����о��������
											{
												data[0]=(UDF_Filter_Life-1)/256;							// ������
												data[1]=(UDF_Filter_Life-1)%256;							// ������
												data[2]=data[0]^0xFF;													// У����
												data[3]=data[1]^0xFF;													// У����
												EM4305_Write_Data(8+UDF_Balanced_Bit,data);		// ѭ����д8 9��������			
												if(UDF_Balanced_Bit==1) UDF_Balanced_Bit=0;		// ѭ����дλ
												else UDF_Balanced_Bit=1;											// ѭ����дλ
											}			
											UDF_Write_Flage=0;															// �Ѿ����д��
											break;
							case 3:	// C2
											if(C2_Filter_Life>0)														// �����о��������
											{
												data[0]=(C2_Filter_Life-1)/256;								// ������
												data[1]=(C2_Filter_Life-1)%256;								// ������
												data[2]=data[0]^0xFF;													// У����
												data[3]=data[1]^0xFF;													// У����
												EM4305_Write_Data(8+C2_Balanced_Bit,data);		// ѭ����д8 9��������			
												if(C2_Balanced_Bit==1) C2_Balanced_Bit=0;			// ѭ����дλ
												else C2_Balanced_Bit=1;												// ѭ����дλ
											}	
											C2_Write_Flage=0;																// �Ѿ����д��
											break;
							default : break;							
						}				
						
						break;
		default : *step=1;
							break;
	}
}


/***********************************************************************************
 * ��������EM4305_Get_NewFAX(uint8_t *step)
 * ����  ���û�����ģʽ 
 * ����  ����
 * ���  ���� 0 δ��ȡ��  0xCCд���ɹ�   0xEE��о��ȡ�쳣 0xAA��о������ȡ����
************************************************************************************/
uint8_t EM4305_Get_NewFAX(uint8_t *step)					// 
{
	uint8_t err=0,num=0,i=0;
	uint8_t ecccode[4]={0x55,0xAA,0x55,0xAA}; 			// �����
	static uint8_t id_buf[4]={0x00,0x00,0x00,0x00}; // ID�뻺��
	static double ratio=0.0,testvalue,setvalue;												//
	static uint16_t read_life=0;										// ��ȡ��о����
	
	if(Usart1_NewFax[0]!=0xAA) 											// У��
	{
		return 0;
	}
	if(Usart1_NewFax[1]!=0x0A) 											// У��
	{
		return 0;
	}
	num=Usart1_NewFax[2]+4;													// ���������λ��
	if(Usart1_NewFax[num]!=0xBB)										// �жϽ�����
	{
		return 0;
	}
	// У��
	for(i=1;i<num;i++)															// У��
	{
		err^=Usart1_NewFax[i];
	}
	if(err!=0) 
	{
		return 0;
	}
	// �ж��Ƿ��д�ɹ� ��λΪ0���쳣
	if(Usart1_NewFax[3]!=0)  												// ��ȡʧ��
	{
		return 0;																			//
	}
	// ��ȡ����
	switch(*step)
	{
		case 1:	// ��ȡ��ƬID
						switch(Filter_Polling_Order)									// Ѱ����о
						{
							case 0:// PP��о
										for(i=0;i<4;i++)										  // ��ID
										{
											if(Usart1_NewFax[4+i]!=PP_RF_ID_Code[i])  break;
										}													
										break;
							case 1:// C1��о
										for(i=0;i<4;i++)										  // ��ID
										{
											if(Usart1_NewFax[4+i]!=C1_RF_ID_Code[i])  break;
										}									
										break;
							case 2:// UDF��о
										for(i=0;i<4;i++)										  // ��ID
										{
											if(Usart1_NewFax[4+i]!=UDF_RF_ID_Code[i])  break;
										}									
										break;
							case 3:// C2��о
										for(i=0;i<4;i++)										  // ��ID
										{
											if(Usart1_NewFax[4+i]!=C2_RF_ID_Code[i])  break;
										}									
										break;
							default : break;							
						}
						// �ж���оID
						if(i<4) 														 				 	// ͨ���Ա�ȷ��������о
						{						
							for(i=0;i<4;i++)														// ����ID 
							{
								id_buf[i]=Usart1_NewFax[4+i];							// ��ȡID
							}	
							*step=3;		// �жϳ�ʼ����־�Ƿ���ȷ							
						}
						else					// ��������о
						{			
							 *step=8;		// ����о��ȡ����
						}						
						break;
		case 2:	// �жϿ�ƬID�Ƿ��ù�
						// �����оע�����ת��STEP=8
						// �����о�Ѿ�
						break;
		case 3:	// ��ȡ��ʼ����־ UID^0x55AA55AA
						for(i=0;i<4;i++)												// ��ȡ��о��ʼ����־
						{
							if(Usart1_NewFax[4+i]!=(id_buf[i]^ecccode[i]))  break;
						}
						// �ж���оID
						if(i<4) 																// ����о�쳣�ǹٷ���֤
						{
							*step=1;															// ������ȡ��оID
							 return 0xEE;													// ��ȡ����
						}
						else																		// ��оͨ����֤
						{
							*step=6;															// ����оͨ������
						}
						break;
		case 4:	// �жϳ�ʼ��״̬
						*step=5;
						break;
		case 5: // ��ȡ��о����DBX
						*step=6;
						break;
		case 6:	// ��ȡ��о��� ����̿��оCC
						switch(Filter_Polling_Order)						// ����о������оID
						{
							case 0:// PP��о
										if(Usart1_NewFax[4]==0x50&&Usart1_NewFax[5]==0x50)				// ����оΪ����̿��о	
										{
											*step=8;	Probe_PP=1;																		// ��ȡ��о���� ̽�⵽PP
											for(i=0;i<4;i++)																				// �����ID
											{
												PP_RF_ID_Code[i]=id_buf[i];														// ע��ID	
											}
										}	
										else	{	*step=1;	return 0xEE;	}													// ��оע��ʧ��
										break;
							case 1:// C1��о
										if(Usart1_NewFax[4]==0x43&&Usart1_NewFax[5]==0x31)				// ����оΪ����̿��о	
										{
											*step=8;	Probe_C1=1;																		// ��ȡ��о����and̽�⵽C1																																				
											for(i=0;i<4;i++)																				// �����ID
											{
												C1_RF_ID_Code[i]=id_buf[i];														// ע��ID	
											}																						
										}			
										else	{	*step=1;	return 0xEE;	}													// ��оע��ʧ��
										break;
							case 2:// UDF��о
										if(Usart1_NewFax[4]==0x52&&Usart1_NewFax[5]==0x4F)				// ����оΪ����̿��о	
										{
											*step=8;	Probe_UDF=1;																	// ��ȡ��о����		̽�⵽UDF							
											for(i=0;i<4;i++)																				// �����ID
											{
												UDF_RF_ID_Code[i]=id_buf[i];													// ע��ID	
											}								
										}		
										else	{	*step=1;	return 0xEE;	}													// ��оע��ʧ��
										break;
							case 3:// C2��о
										if(Usart1_NewFax[4]==0x43&&Usart1_NewFax[5]==0x32)				// ����оΪ����̿��о	
										{
											*step=8;	Probe_C2=1;																		// ��ȡ��о����		
											for(i=0;i<4;i++)																				// �����ID
											{
												C2_RF_ID_Code[i]=id_buf[i];														// ע��ID	
											}									
										}		
										else	{	*step=1;	return 0xEE;	}													// ��оע��ʧ��
										break;
							default : break;							
						}			
						break;
		case 7:	// ��ȡʹ�ò�Ʒ����
						*step=8;																													// 
						break;
		case 8: // ��ȡ��о����A 4567
						if((Usart1_NewFax[4]^Usart1_NewFax[6])==0xFF&&(Usart1_NewFax[5]^Usart1_NewFax[7])==0xFF)
						{
							read_life=Usart1_NewFax[4]*256+Usart1_NewFax[5];								//
						}
						*step=9;																													// ��ȡ��о�����Ĵ���B
						break;
		case 9:	// ��ȡ��о����B 4567
						if((Usart1_NewFax[4]^Usart1_NewFax[6])==0xFF&&(Usart1_NewFax[5]^Usart1_NewFax[7])==0xFF)
						{
							if(read_life>=Usart1_NewFax[4]*256+Usart1_NewFax[5])						// �Ե���Ϊ׼
							{
								read_life=Usart1_NewFax[4]*256+Usart1_NewFax[5];							// ȡ����
							} 	
						}
						// ��оÿСʱд��һ��
						if(Hour_Flage!=0)
						{
							Hour_Flage=0;
							PP_Write_Flage=1;
							C1_Write_Flage=1;
							UDF_Write_Flage=1;
							C2_Write_Flage=1;
						}						
						// �ж��Ƿ���Ҫд��
						switch(Filter_Polling_Order)																			// ��ѯ��о
						{
							case 0:	// PP
											Probe_PP=1;
											PP_Filter_Life=read_life;																//
											testvalue=PP_Filter_Life;
											setvalue=PP_FILTER_TIME;
											ratio=testvalue/setvalue;																// 
											if(ratio>1.0/setvalue)LED_PP=(uint8_t)(ratio*6+1);			// ֻҪ��о����������ʾ��һ��
											else LED_PP=0;																					// �ľ�									
											if(PP_Write_Flage!=RESET)	{	*step=10;	}									// д����־
											else	{	*step=1;	return 0xAA;}													// ���¶�ȡID
											break;
							case 1:	// C1
											Probe_C1=1;
											C1_Filter_Life=read_life;																//
											testvalue=C1_Filter_Life;
											setvalue=C1_FILTER_TIME;
											ratio=testvalue/setvalue;																// 
											if(ratio>1.0/setvalue)LED_C1=(uint8_t)(ratio*6+1);			// ֻҪ��о����������ʾ��һ��
											else LED_C1=0;																					// �ľ�									
											if(C1_Write_Flage!=RESET)	{	*step=10;	}									// д����־
											else	{	*step=1;	return 0xAA;}													// ��д��ȡID
											break;
							case 2:	// UDF
											Probe_UDF=1;
											UDF_Filter_Life=read_life;															//
											testvalue=UDF_Filter_Life;
											setvalue=UDF_FILTER_TIME;
											ratio=testvalue/setvalue;																// 
											if(ratio>1.0/setvalue)LED_UDF=(uint8_t)(ratio*6+1);			// ֻҪ��о����������ʾ��һ��
											else LED_UDF=0;																					// �ľ�										
											if(UDF_Write_Flage!=RESET)	{	*step=10;	}								// д����־
											else	{	*step=1;	return 0xAA;}													// ��д��ȡID									
											break;
							case 3:	// C2
											Probe_C2=1;
											C2_Filter_Life=read_life;																//
											testvalue=C2_Filter_Life;
											setvalue=C2_FILTER_TIME;
											ratio=testvalue/setvalue;																// 
											if(ratio>1.0/setvalue)LED_C2=(uint8_t)(ratio*6+1);			// ֻҪ��о����������ʾ��һ��
											else LED_C2=0;																					// �ľ�										
											if(C2_Write_Flage!=RESET)	{	*step=10;	}									// д����־
											else	{	*step=1;	return 0xAA;}													// ��д��ȡID											
											break;
							default : break;					
						}		
						break;
		case 10:	// д��о
						*step=1;																													// ��λ	
						return 0xCC;
//						break;
		default : *step=1;
							break;
	}
	// ��ȡ�쳣
	return 0x01;																																// ����֤��о�˳�
}


/***********************************************************************************
 * ��������void System_Read_Filter(void)
 * ����  ���ӿ��ж�ȡһҳ����
 * ����  ����
 * ���  : ��ȡ�����ݷ���1 δ��ȡ�����򷵻�0
 * ����  ���ⲿ���� AA 0A 06 84 05 12 34 56 78 85 BB
**********************************************************************************/
void System_Read_Filter(void)																//
{
	static uint8_t last_filter,now_filter;
	static uint8_t time;
	
	// ���³����ֹģ�����޷���ȡ������о״̬
	now_filter=Filter_Polling_Order;													// ���µ�ǰ��о��
	if(last_filter!=now_filter)																// ��ǰ��о���ϴ���о��һ��
	{
		last_filter=now_filter;																	// �����ϴ���о��
		time=0;																									// ���¼���
	}
	else																											// ��ǰ��о���ϴ���о��һ��
	{
		if(time++>MAX_RETRY_FILTER_NUM)													// ����50�ο��ڵ�ǰ��о
		{
			switch(Filter_Polling_Order)													// ��о̽��
			{
				case 0:	// ��ȡPP��о����
								Probe_PP=0;LED_PP=0;												// PP̽��ʧ��
								PP_Filter_Life=0;														// PP��о��������
								Filter_Polling_Order=1;											// ��ȡ��һ����о
								break;
				case 1:	// ��ȡC1��о����
								Probe_C1=0;LED_C1=0;												// C1̽��ʧ��
								C1_Filter_Life=0;														// C1��о��������					
								Filter_Polling_Order=2;
								break;
				case 2:	// ��ȡUDF
								Probe_UDF=0;LED_UDF=0;											// UDF̽��ʧ��
								UDF_Filter_Life=0;													// UDF��о��������					
								Filter_Polling_Order=3;
								break;
				case 3:	// ��ȡC2
								Probe_C2=0;LED_C2=0;												// C2̽��ʧ��
								C2_Filter_Life=0;														// C2��о��������							
								Filter_Polling_Order=0;
								break;
				default : Filter_Polling_Order=0; 
								break;		
			}
			time=0;																								// ���¼���
		}		
	}
	// ��о���Ͷ�ȡ����
	switch(Filter_Polling_Order)															//
	{
		case 0:	// ��ȡPP��о����
						RF_A_SET(1);RF_B_SET(1);												//
		        HAL_Delay(100);
						RFID_Read_Info(&PP_RFID_Setp);									//					
						break;
		case 1:	// �ȴ�C1��о����		
						RF_A_SET(0);RF_B_SET(1);	 
		        HAL_Delay(100);											//
						RFID_Read_Info(&C1_RFID_Setp);									//
						break;
		case 2:	// �ȴ�UDF��о����	
						RF_A_SET(1);RF_B_SET(0); 
		        HAL_Delay(100);												//
						RFID_Read_Info(&UDF_RFID_Setp);									//
						break;
		case 3:	// ��ȡC2��о����
						RF_A_SET(0);RF_B_SET(0);		
         		HAL_Delay(100);										// 
						RFID_Read_Info(&C2_RFID_Setp);									//
						break;	
		default : Filter_Polling_Order=0;
							break;
	}
}

/***********************************************************************************
 * ��������void System_Read_Fax(void)
 * ����  ���ӿ��ж�ȡһҳ����
 * ����  ����
 * ���  : ��ȡ�����ݷ���1 δ��ȡ�����򷵻�0
 * ����  ���ⲿ���� AA 0A 06 84 05 12 34 56 78 85 BB
**********************************************************************************/
void System_Read_Fax(void)
{
	uint8_t station=0xFF;
	
	// ��ȡ��1֡����
	if(Usart1_Ready_Flage!=RESET)										// ��ȡ������
	{
		Usart1_Ready_Flage=0;													// �����־
	}
	else																						// ����δ�յ���Ч����
	{
		return;																				// �˳��ó���
	}
	// ����
	switch(Filter_Polling_Order)
	{
		case 0x00:// ��ǰΪPP��о
							station=EM4305_Get_NewFAX(&PP_RFID_Setp);			// ̽��							
							if(station==0)											// δ��ȷ��ȡID��Ϣ(ID���쳣)
							{
								if(PP_Read_Fail_Num++>MAX_READ_FILTER_NUM)
								{
									Probe_PP=0;LED_PP=0;						// PP̽��ʧ��
									PP_Filter_Life=0;								// PP��о��������					
									Filter_Polling_Order=1;					// ��ȡ��һ��
								}
							}
							if(station==0xEE)										// ��о״̬�쳣
							{
								Probe_PP=0;LED_PP=0;							// PP̽��ʧ��
								PP_Filter_Life=0;									// PP��о��������
								Filter_Polling_Order=1;						// ��ȡ��һ����о
							}							
							if(station==0xCC)										// ��од������
							{
								Filter_Polling_Order=1;						// ��ȡ��һ����о
								PP_Read_Fail_Num=0;								//
							}																
							if(station==0xAA)										// ��ȷ��ȡ��о����
							{
								Filter_Polling_Order=1;						// ��ȡ��һ��
								PP_Read_Fail_Num=0;								//
							}
							break;
		case 0x01:// ��ǰΪC1��о
							station=EM4305_Get_NewFAX(&C1_RFID_Setp);			//
							if(station==0)											// δ��ȷ��ȡID��Ϣ(ID���쳣)
							{
								if(C1_Read_Fail_Num++>MAX_READ_FILTER_NUM)
								{
									Probe_C1=0;LED_C1=0;						// C1̽��ʧ��
									C1_Filter_Life=0;								// C1��о��������									
									Filter_Polling_Order=2;					// ��ȡ��һ��
								}
							}
							if(station==0xEE)										// ��о״̬�쳣
							{
								Probe_C1=0;LED_C1=0;							// C1̽��ʧ��
								C1_Filter_Life=0;									// C1��о��������
								Filter_Polling_Order=2;						// ��ȡ��һ����о
							}							
							if(station==0xCC)										// ��од������
							{
								Filter_Polling_Order=2;						// ��ȡ��һ����о
								C1_Read_Fail_Num=0;								//
							}																
							if(station==0xAA)										// ��ȷ��ȡ��о����
							{
								Filter_Polling_Order=2;						// ��ȡ��һ��
								C1_Read_Fail_Num=0;								//
							}			
							break;
		case 0x02:// ��ǰΪUDF��о
							station=EM4305_Get_NewFAX(&UDF_RFID_Setp);		//
							if(station==0)											// δ��ȷ��ȡID��Ϣ(ID���쳣)
							{
								if(UDF_Read_Fail_Num++>MAX_READ_FILTER_NUM)
								{
									Probe_UDF=0;LED_UDF=0;					// UDF̽��ʧ��
									UDF_Filter_Life=0;							// UDF��о��������									
									Filter_Polling_Order=3;					// ��ȡ��һ��
								}
							}
							if(station==0xEE)										// ��о״̬�쳣
							{
								Probe_UDF=0;LED_UDF=0;						// UDF̽��ʧ��
								UDF_Filter_Life=0;								// UDF��о��������
								Filter_Polling_Order=3;						// ��ȡ��һ����о
							}							
							if(station==0xCC)										// ��од������
							{
								Filter_Polling_Order=3;						// ��ȡ��һ����о
								UDF_Read_Fail_Num=0;							//
							}																
							if(station==0xAA)										// ��ȷ��ȡ��о����
							{
								Filter_Polling_Order=3;						// ��ȡ��һ��
								UDF_Read_Fail_Num=0;							//
							}				
							break;
		case 0x03:// ��ǰΪC2��о
							station=EM4305_Get_NewFAX(&C2_RFID_Setp);			// ̽����о	
							if(station==0)											// δ��ȷ��ȡID��Ϣ(ID���쳣)
							{
								if(C2_Read_Fail_Num++>MAX_READ_FILTER_NUM)
								{
									Probe_C2=0;LED_C2=0;						// C2̽��ʧ��
									C2_Filter_Life=0;								// C2��о��������									
									Filter_Polling_Order=0;					// ��ȡ��һ��
								}
							}
							if(station==0xEE)										// ��о״̬�쳣
							{
								Probe_C2=0;LED_C2=0;							// C2̽��ʧ��
								C2_Filter_Life=0;									// C2��о��������
								Filter_Polling_Order=0;						// ��ȡ��һ����о
							}							
							if(station==0xCC)										// ��од������
							{
								Filter_Polling_Order=0;						// ��ȡ��һ����о
								C2_Read_Fail_Num=0;								//
							}																
							if(station==0xAA)										// ��ȷ��ȡ��о����
							{
								Filter_Polling_Order=0;						// ��ȡ��һ��
								C2_Read_Fail_Num=0;								//
							}			
							break;
		default : // �쳣
							Filter_Polling_Order=0x00;
							break;
	}
}



