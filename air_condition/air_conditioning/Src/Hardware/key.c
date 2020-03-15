#include "key.h"
extern unsigned char KeyNum;
unsigned char KeyStatus;
void key_scan()
{
	if(KeyNum&0x03)
	{
		if(KeyNum&0x02)
			KeyStatus = 0x01;//key1 �̰�
		else
			KeyStatus = 0x02;	//key1 ����		
	}
	else if(KeyNum&0x0c)
	{
		if(KeyNum&0x08)
			KeyStatus = 0x03;//key2 �̰�
		else
			KeyStatus = 0x04;			
	}
	else 	if(KeyNum&0x30)
	{
		if(KeyNum&0x20)
			KeyStatus = 0x05;//key3 �̰�
		else
			KeyStatus = 0x06;			
	}
	else 	if(KeyNum&0xc0)
	{
		if(KeyNum&0x80)
			KeyStatus = 0x07;//key4 �̰�
		else
			KeyStatus = 0x08;			
	}
}

unsigned char GetKeyNum()
{
	return KeyStatus;
}
void SetKeyNum(unsigned char clr)
{
	KeyStatus = KeyStatus & clr;
	
}