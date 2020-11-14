#include "register.h"
#include "Protocol_C.h"

REG_val_stru register_map;
REG_STATUS_stru reg_status;


unsigned char array_comp(unsigned char *p1,unsigned char *p2,unsigned char len)
{
	unsigned char i;
	unsigned char result;
	result = 0;
	for(i=0;i<len;i++)
	{
		if(p1[i]!=p2[i])
		{
			result = 1;
			break;
		}
		else
			result = 0;
	}
	return result;
}

unsigned int RegisterAddrCal(unsigned int p,unsigned char num)
{
	static uint32_t x1,x12,x4,x44,xe,xf;
	static unsigned char flag;
	if(flag == 0)
	{
		flag = 1;
		x1 = x1_ADDR_START;
		x12 = x1_ADDR_SIZE;
		x4 = x1_ADDR_SIZE+x12_ADDR_SIZE;
		x44 = x1_ADDR_SIZE+x12_ADDR_SIZE+x4_ADDR_SIZE;
		xe = x1_ADDR_SIZE+x12_ADDR_SIZE+x4_ADDR_SIZE+x44_ADDR_SIZE;
		xf = x1_ADDR_SIZE+x12_ADDR_SIZE+x4_ADDR_SIZE+x44_ADDR_SIZE+xE_ADDR_SIZE;		
	}
	switch(num)
	{
		case 1:return p-x1;
		case 2:return p+x12-x12_ADDR_START;		
		case 3:return p+x4-x4_ADDR_START;
		case 4:return p+x44-x44_ADDR_START;
		case 5:return p+xe-xE_ADDR_START;
		case 6:return p+xf-xF_ADDR_START;
		case 7:return p+x1;
		case 8:return p+x12_ADDR_START-x12;	
		case 9:return p+x4_ADDR_START-x4;
		case 10:return p+x44_ADDR_START-x44;
		case 11:return p+xE_ADDR_START-xe;
		case 12:return p+xF_ADDR_START-xf;		
	}

}
unsigned char *ReadRegister(unsigned int p)
{
   unsigned int tmp;
   tmp = 0xffff;
	if(p>=x1_ADDR_START&&p<x12_ADDR_START)
	{
	    tmp = RegisterAddrCal(p,1);
		
	}
	else if(p>=x12_ADDR_START&&p<x4_ADDR_START)
	{
		tmp = RegisterAddrCal(p,2);
	}
	else if(p>=x4_ADDR_START&&p<x44_ADDR_START)
	{
		tmp =RegisterAddrCal(p,3);
	}
	else if(p>=x44_ADDR_START&&p<xE_ADDR_START)
	{
		tmp = RegisterAddrCal(p,4);
	}
	else if(p>=xE_ADDR_START&&p<xF_ADDR_START)
	{
		tmp = RegisterAddrCal(p,5);
	}
	else if(p>=xF_ADDR_START&&p<xF_ADDR_START+15)
	{
		tmp = RegisterAddrCal(p,6);
	}
	else
		tmp = 0xffff;
	if(tmp != 0xffff)
		return &(register_map.value[2*tmp]);
	else
		return NULL;

}
unsigned char ReadOnlyAddr(unsigned int p)
{
 
	if(RegisterAddrCal(p,7)<=0x100a||RegisterAddrCal(p,8)==0x1202||RegisterAddrCal(p,9)==0x4001||RegisterAddrCal(p,9)==0x400a||
		(RegisterAddrCal(p,10)>=0x4600&&RegisterAddrCal(p,10)<=0x460f)||RegisterAddrCal(p,12)>=0xf006||RegisterAddrCal(p,12)>=0xf00b||
		(RegisterAddrCal(p,11)>=0xe000&&RegisterAddrCal(p,11)<=0xe00f))
		return 1;
	else 
		return 0;
}
unsigned char WriteRegisteSet(unsigned int p,unsigned char *reg,unsigned char len)
{
   unsigned int tmp;
   unsigned char i,result;
   if(p>=x1_ADDR_START&&p<x12_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,1);
	   
   }
   else if(p>=x12_ADDR_START&&p<x4_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,2);
   }
   else if(p>=x4_ADDR_START&&p<x44_ADDR_START)
   {
	   tmp =RegisterAddrCal(p,3);
   }
   else if(p>=x44_ADDR_START&&p<xE_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,4);
   }
   else if(p>=xE_ADDR_START&&p<xF_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,5);
   }
   else if(p>=xF_ADDR_START&&p<xF_ADDR_START+15)
   {
	   tmp = RegisterAddrCal(p,6);
   }


	else
		tmp = 0xffff;
	if(tmp != 0xffff)
	{   

		
		for(i = 0;i<len;i++)
		{
		    if(ReadOnlyAddr(i+2*tmp)==1)
				continue;
			register_map.value[i+2*tmp] = reg[i];
			//register_map.value[i+2*tmp+1] = reg[i];
		}	 
		
		reg_status.wrStatus = 1;
		reg_status.regAddr = p;
		reg_status.len = len;
		result = 0;
	}
	else
	{
		reg_status.wrStatus = 0;
		reg_status.regAddr = p;
		reg_status.len = len;
		result = 1;

	}
		
		return result;

}
void WriteOneRegister(unsigned int p,unsigned int regVal)
{
   unsigned int tmp;
   tmp = 0xffff;
   
   if(p>=x1_ADDR_START&&p<x12_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,1);
	   
   }
   else if(p>=x12_ADDR_START&&p<x4_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,2);
   }
   else if(p>=x4_ADDR_START&&p<x44_ADDR_START)
   {
	   tmp =RegisterAddrCal(p,3);
   }
   else if(p>=x44_ADDR_START&&p<xE_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,4);
   }
   else if(p>=xE_ADDR_START&&p<xF_ADDR_START)
   {
	   tmp = RegisterAddrCal(p,5);
   }
   else if(p>=xF_ADDR_START&&p<xF_ADDR_START+15)
   {
	   tmp = RegisterAddrCal(p,6);
   }


	else
		tmp = 0xffff;

	if(tmp != 0xffff)
	{   

		
		register_map.value[tmp*2] = (u8)(regVal>>8);	 
		register_map.value[tmp*2+1] = (u8)(regVal);			
	}

}
unsigned int uchar2uint(unsigned char *p)//0   0 1   1  2 3   2  4 5
{
    unsigned int tmp;
	tmp = ((u16)(p[0])<<8)+p[1];
	return tmp;
}

REG_STATUS_stru *readRigsterStatus()//��ȡ�Ĵ���״̬�������д����������Ӧ�Ĳ�������
{
	return &reg_status;
}
void equip_bind(unsigned char *p)
{
    unsigned int i,j;
	unsigned char result;
	result = 0;
	

        for(i=0;i<register_map.bindCount;i++)
        {
            for(j=i;j<8;j++)
            {
				if(array_comp(p,register_map.value+j*8,8) == 0)
				{
					result = 1;
					break;
				}
				else
					result = 0;
			}
			if(result == 1)
				break;
		}
		if(result == 0)
		{
			if(register_map.bindCount<200)
			{
				register_map.bindCount = register_map.bindCount+1;
				memcpy(&register_map.value[register_map.bindCount*8+0x4010],p,8);
			}
	     }
		

}
unsigned char equip_bind_analy(unsigned char *p)
{
    unsigned int i,j;
	unsigned char result;
	result = 0;
	

        for(i=0;i<register_map.bindCount;i++)
        {
            for(j=i;j<8;j++)
            {
				if(array_comp(p,register_map.value+j*8,8) == 0)
				{
					result = 1;
					break;
				}
				else
					result = 0;
			}
			if(result == 1)
				break;
		}
		return result;

}

void register_init()
{
    unsigned char *q;
		WriteOneRegister(0xf001,0x0001);//01H�����ܼ�������� 0H���ܼ��������
		WriteOneRegister(0xf002,0);//10-1000min����	
		WriteOneRegister(0xf003,0);//10-120min����
		WriteOneRegister(0xf004,0);//0-60s����;��������ģ�鼤�ʱʱ��
		WriteOneRegister(0xf005,0);//����ģ�鹦������
		//								01H��10%���������
		//								02H��20%���������
		//								03H��30%���������
		//								04H��40%���������
		//								05H��50%���������
		//								06H��60%���������
		//								07H��70%���������
		//								08H��80%���������
		//								09H��90%���������
		//								10H�����������
		WriteOneRegister(0xf006,0);//0-100 ��Ӧ 0%-100%����
		WriteOneRegister(0xf007,0);//�ڵ��ַ��2�ֽ�
		WriteOneRegister(0xf008,0);//
		WriteOneRegister(0xf009,0);//
		WriteOneRegister(0xf00a,2);//���ؽڵ��ַ��2�ֽ�LORA�����ַ
		q = ReadRegister(0xf00a);
		WriteOneRegister(0xf00b,((u16)(q[0])<<8)+q[1]);//LORA���������ַ,��0xf00a��ͬ
		WriteOneRegister(0xf00c,0);//ũҵ�ɼ������ն��봫����ͨ�ŵ�Modbus������
		//������(2400/4800/9600)0x00-2400;0x01-4800;
		//0x02-9600;0x03-115200

		WriteOneRegister(0xf00d,0);//ũҵ�ɼ������ն���������ӣ����üĴ�����Modbus������
		//������(2400/4800/9600)0x00-2400;0x01-4800;
		//0x02-9600;0x03-115200
		WriteOneRegister(0xf00e,0);//�����ϱ�
		WriteOneRegister(0x1001,55);//�����ϱ�
		WriteOneRegister(0x1006,57);//�����ϱ�
		WriteOneRegister(0x1202,3);//�����ϱ�
		WriteOneRegister(0x1204,30);//�����ϱ�
		//WriteOneRegister(0x1006,57);//�����ϱ�
}
REG_val_stru *getRegAddr()
{
	return &register_map;
}
void loratestInit()
{
	unsigned char *q;
	  if(LORA_TX == 0)
		{
			WriteOneRegister(0xf00a,2);//���ؽڵ��ַ��2�ֽ�LORA�����ַ
			q = ReadRegister(0xf00a);
			WriteOneRegister(0xf00b,((u16)(q[0])<<8)+q[1]);//LORA���ص�ַ
			WriteOneRegister(0x1202,3);//Ŀ���ַ
			WriteOneRegister(0x1204,30);//�ŵ�		
		}
		else
		{
			WriteOneRegister(0xf00a,1);//���ؽڵ��ַ��2�ֽ�LORA�����ַ
			q = ReadRegister(0xf00a);
			WriteOneRegister(0xf00b,((u16)(q[0])<<8)+q[1]);//LORA���ص�ַ
			WriteOneRegister(0x1202,2);//Ŀ���ַ
			WriteOneRegister(0x1204,30);//�ŵ�			
		}

}

