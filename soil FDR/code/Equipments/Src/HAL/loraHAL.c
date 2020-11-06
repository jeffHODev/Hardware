#include "loraHAL.h"
#include "loraHW.h"
#include "Protocol_C.h"
#include "register.h"
#include "nbiotHAL.h"
#include "nbiotHW.h"
#include "modbus.h"
#include "gateway.h"

loraUart_stru loraUart;
LORAHW_stru lorahw;
LORA_Params_stru loraParams;
void ParamsInit()
{
    unsigned char *p;
	unsigned int tmp;
	ParamsSave();//��������
	p = ReadRegister(0x1205);
	if(ROLE == 1||getModeStatus()== 0x02)
	{
		NbiotUartInit();
		ServerIP_Pack(p);
		nbiot_HardwareInit(ON);
	}
	p = ReadRegister(0xf00c);
	tmp = uchar2uint(p);
	sensorModbusRate(tmp,0);
}
void EquipGateway_Process()
{  
	LORAHW_stru loraNo;
	if(loraUart.receivedFlag1 == 1)
	{	
	     if(lorahw.mode ==2)//����ģʽ�յ����ݲ���Э�����
	     {
	        loraset(6,&(loraUart.lora1RxBuffer[3]),9);
			ParamsSave();
			lorahw.mode = 0;
		 }
		 else//ʽ�յ�������Э�����
		 {
			 if(protocolCAnaly(loraUart.lora1RxBuffer)	== 0)//У��ɹ�
			 {	 
				  loraNo.loraNo = 0;
				  loraNo.mode =  TransmitMode;
				  wirelessTimoutStart(1);//�����ϱ�ģʽ��ʱ��ʱ��־λ
				  WrRead_equipment(&loraNo);//��д�Ĵ�������
			 }

		 }

		loraUart.receivedFlag1 = 0;
		  
	}
    if(ROLE == 1)//����ģʽ
    {
         unsigned char *fameStatus;
		 fameStatus = modbusFrameStatus();
		if(*fameStatus== 2)
		{
			 if(protocolCAnaly(modbusBuffer())	== 0)//У��ɹ�
			 {	 
				  loraNo.loraNo = 0;
				  loraNo.mode =  TransmitMode;
				  wirelessTimoutStart(1);//�����ϱ�ģʽ��ʱ��ʱ��־λ
				  WrRead_equipment(&loraNo);//��д�Ĵ�������
				  
			 }	
			 *fameStatus = 0;
		}
		if(NbiotFrameStatus()== 1)
		{
			 if(protocolCAnaly(NbiotFrameBuffer())	== 0)//У��ɹ�
			 {	 
				  loraNo.loraNo = 0;
				  loraNo.mode =  TransmitMode;
				  wirelessTimoutStart(1);//�����ϱ�ģʽ��ʱ��ʱ��־λ
				  WrRead_equipment(&loraNo);//��д�Ĵ������� 
			 }	
		}
		Gateway_Process();
	}
	equipmentProcess();//�豸����
	if(loraUart.receivedFlag2 == 1)
	{
		
	}	
}
