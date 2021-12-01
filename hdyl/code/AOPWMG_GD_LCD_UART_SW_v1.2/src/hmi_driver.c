#include "hmi_driver.h"

#define TX_8(P1) SEND_DATA((P1)&0xFF)  //发送单个字节
#define TX_8N(P,N) SendNU8((uint8 *)P,N)  //发送N个字节
#define TX_16(P1) TX_8((P1)>>8);TX_8(P1)  //发送16位整数


static uint8 _checksum;


void SEND_DATA(uint8 c)
{
	_checksum +=c;
	SendChar(c);
}

void BEGIN_CMD()
{
	_checksum=0;
	TX_8(0xaa);
}

void END_CMD()
{
	SendChar(_checksum);
}



void DelayMS(unsigned int n) 
{
	int i,j;  
	for(i = n;i>0;i--)
		for(j=1000;j>0;j--) ; 
}

void SendStrings(uchar *str)
{
	while(*str)
	{
		TX_8(*str);
		str++;
	}
}

void SendNU8(uint8 *pData,uint16 nDataLen)
{
	uint16 i = 0;
	for (;i<nDataLen;++i)
	{
		TX_8(pData[i]);
	}
}

uint8 GetStrLen(uchar *str)
{
	uint8 i=0;
	while(*str)
	{
		str++;
		i++;
	}
	return i;
}

///////////////////////////////////////////////
//基本指令控制函数

void SetConnect()
{
	BEGIN_CMD();
	TX_8(0x03);
	TX_8(0x48);
	END_CMD();
}

void SetReset(void)
{
	BEGIN_CMD();
	TX_8(0x08);
	TX_8(0x4a);
	TX_8(0x52);
	TX_8(0x65);
	TX_8(0x73);
	TX_8(0x65);
	TX_8(0x74);
	END_CMD();
}

void Display_Image(uint16 x,uint16 y,uint8 image_id)
{
	BEGIN_CMD();
	TX_8(0x08);
	TX_8(0x49);
	TX_16(x);
	TX_16(y);
	TX_8(image_id);
	END_CMD();
}

void DisplayCut_Image(uint16 image_x,uint16 image_y,uint16 image_w, uint16 image_h,uint8  image_id)
{
	BEGIN_CMD();
	TX_8(0x0c);
	TX_8(0x4f);
	TX_16(image_x);
	TX_16(image_y);
	TX_16(image_w);
	TX_16(image_h);
	TX_8(image_id);
	END_CMD();
}

void GUI_CleanScreen(uint16 color)
{
	BEGIN_CMD();
	TX_8(0x05);
	TX_8(0x43);
	TX_16(color);
	END_CMD();
}

void Display_String(uint16 x, uint16 y,uint8 DisplayType,uint8 FontSize,uint8 ImageNo,uint16 BackColor,uint16 ForeColor, uchar *strings )
{
	uint8 len;
	len = GetStrLen(strings);
	BEGIN_CMD();
	TX_8(len+14);//len
	TX_8(0x53);
	TX_16(x);
	TX_16(y);
	TX_8(DisplayType);
	TX_8(FontSize);
	TX_8(ImageNo);
	TX_16(BackColor);
	TX_16(ForeColor);
	SendStrings(strings);
	END_CMD();
}

void Display_Message(uint8 FontSize,uint8 time,uchar *strings )
{
	uint8 len;
	len = GetStrLen(strings);
	BEGIN_CMD();
	TX_8(len+5);//len
	TX_8(0x4d);
	TX_8(FontSize);
	TX_8(time);
	SendStrings(strings);
	END_CMD();
}


void GUI_Rectangle(uint16 x0, uint16 y0, uint16 x1,uint16 y1 ,uint16 ForeColor)
{
	BEGIN_CMD();
	TX_8(0x0d);
	TX_8(0x52);
	TX_16(x0);
	TX_16(y0);
	TX_16(x1);
	TX_16(y1);
	TX_16(ForeColor);
	END_CMD();
}

void GUI_RectangleFill(uint16 x0, uint16 y0, uint16 x1,uint16 y1  ,uint16 ForeColor)
{
	BEGIN_CMD();
	TX_8(0x0d);
	TX_8(0x46);
	TX_16(x0);
	TX_16(y0);
	TX_16(x1);
	TX_16(y1);
	TX_16(ForeColor);
	END_CMD();
}

void GUI_Line(uint16 x0, uint16 y0, uint16 x1, uint16 y1,uint16 ForeColor)
{
	BEGIN_CMD();
	TX_8(0x0d);
	TX_8(0x4c);
	TX_16(x0);
	TX_16(y0);
	TX_16(x1);
	TX_16(y1);
	TX_16(ForeColor);
	END_CMD();
}

void SetBackLight(uint8 light_level)
{
	BEGIN_CMD();
	TX_8(0x04);
	TX_8(0x42);
	TX_8(light_level);
	END_CMD();
}

void SetTouchPaneOption(uint8 enbale)
{
	BEGIN_CMD();
	TX_8(0x04);
	TX_8(0x51);
	TX_8(enbale);
	END_CMD();
}

void CalibrateTouchPane(void)
{
	BEGIN_CMD();
	TX_8(0x03);
	TX_8(0x50);
	END_CMD();
}

void TestTouchPane(uint8 enbale)
{
	BEGIN_CMD();
	TX_8(0x04);
	TX_8(0x54);
	TX_8(enbale);
	END_CMD();
}

///////////////////////////////////////////////
//控件控制函数

void SetPage(uint8 page_id)
{
	BEGIN_CMD();     //head
	TX_8(4);			//lenght
	TX_8(0x70);		//cmd
	TX_8(page_id);	//data
	END_CMD();  	//checksum
}

void GetPage(void)
{
	BEGIN_CMD();
	TX_8(3);
	TX_8(0x71);
	END_CMD();
}


void SetLableValueColor(uint8 page_id,uint8 control_id,uint16 BackColor,uint16 ForeColor,uchar *str)
{
	uint8 len;
	len = GetStrLen(str);
	//	len = strlen(str);
	BEGIN_CMD();
	TX_8(len+9);
	TX_8(0x7d);
	TX_8(control_id);
	TX_8(page_id);
	TX_16(ForeColor);
	TX_16(BackColor);
	SendStrings(str);
	END_CMD();
}

void SetLableValue(uint8 page_id,uint8 control_id,uchar *str)
{
	uint8 len;
	len = GetStrLen(str);
	//	len = strlen(str);
	BEGIN_CMD();
	TX_8(len+5);
	TX_8(0x74);
	TX_8(control_id);
	TX_8(page_id);
	SendStrings(str);
	END_CMD();
}

void SetNumberValue(uint8 page_id,uint8 control_id,uint16 number)
{
	BEGIN_CMD();
	TX_8(7);
	TX_8(0x6e);
	TX_8(control_id);
	TX_8(page_id);
	TX_16(number);
	END_CMD();
}

void SetEditValueColor(uint8 page_id,uint8 control_id,uint16 BackColor,uint16 ForeColor,uchar *str)
{
	uint8 len;
	len = GetStrLen(str);
	BEGIN_CMD();
	TX_8(len+9);
	TX_8(0x7e);
	TX_8(control_id);
	TX_8(page_id);
	TX_8(ForeColor);
	TX_8(BackColor);
	SendStrings(str);
	END_CMD();
}

void SetEditValue(uint8 page_id,uint8 control_id,uchar *str)
{
	uint8 len;
	len = GetStrLen(str);
	BEGIN_CMD();
	TX_8(len+5);
	TX_8(0x65);
	TX_8(control_id);
	TX_8(page_id);
	SendStrings(str);
	END_CMD();
}

void GetEditValue(uint8 page_id,uint8 control_id)
{
	BEGIN_CMD();
	TX_8(5);
	TX_8(0x75);
	TX_8(control_id);
	TX_8(page_id);
	END_CMD();
}

void GetTouchEditValue(uint8 page_id,uint8 control_id)
{
	BEGIN_CMD();
	TX_8(5);
	TX_8(0x7c);
	TX_8(control_id);
	TX_8(page_id);
	END_CMD();
}


void SetProgressbarValue(uint8 page_id,uint8 control_id,uint8 value)
{
	BEGIN_CMD();
	TX_8(6);
	TX_8(0x6f);
	TX_8(control_id);
	TX_8(page_id);
	TX_8(value);
	END_CMD();
}

void SetCheckboxValue(uint8 page_id,uint8 control_id,uint8 value)
{
	BEGIN_CMD();
	TX_8(6);
	TX_8(0x63);
	TX_8(control_id);
	TX_8(page_id);
	TX_8(value);
	END_CMD();
}


void GetCheckboxValue(uint8 page_id,uint8 control_id)
{
	BEGIN_CMD();
	TX_8(5);
	TX_8(0x64);
	TX_8(control_id);
	TX_8(page_id);
	END_CMD();
}

void SetCircleGaugeValue(uint8 page_id,uint8 control_id,uint16 value)
{
	BEGIN_CMD();
	TX_8(7);
	TX_8(0x7a);
	TX_8(control_id);
	TX_8(page_id);
	TX_16(value);
	END_CMD();
}

void SetBarGaugeValue(uint8 page_id,uint8 control_id,uint16 value)
{
	BEGIN_CMD();
	TX_8(7);
	TX_8(0x61);
	TX_8(control_id);
	TX_8(page_id);
	TX_16(value);
	END_CMD();
}

void SetWaterGaugeValue(uint8 page_id,uint8 control_id,uint16 value)
{
	BEGIN_CMD();
	TX_8(7);
	TX_8(0x77);
	TX_8(control_id);
	TX_8(page_id);
	TX_16(value);
	END_CMD();
}


void SetThermometerValue(uint8 page_id,uint8 control_id,uint16 value)
{
	BEGIN_CMD();
	TX_8(7);
	TX_8(0x6d);
	TX_8(control_id);
	TX_8(page_id);
	TX_16(value);
	END_CMD();
}


void SetBatteryValue(uint8 page_id,uint8 control_id,uint16 value)
{
	BEGIN_CMD();
	TX_8(7);
	TX_8(0x79);
	TX_8(control_id);
	TX_8(page_id);
	TX_16(value);
	END_CMD();
}

void SetWaveformValue(uint8 page_id,uint8 control_id,uint8 channelNo,uint8 value)
{
	BEGIN_CMD();
	TX_8(7);
	TX_8(0x66);
	TX_8(control_id);
	TX_8(page_id);
	TX_8(channelNo);
	TX_8(value);
	END_CMD();
}

void WaveformDataClear(uint8 page_id,uint8 control_id)
{
	BEGIN_CMD();
	TX_8(5);
	TX_8(0x76);
	TX_8(control_id);
	TX_8(page_id);
	END_CMD();
}

void WaveformDataInsert(uint8 page_id,uint8 control_id,uint8 channelNo,uint8 value)
{
	BEGIN_CMD();
	TX_8(7);
	TX_8(0x6a);
	TX_8(control_id);
	TX_8(page_id);
	TX_8(channelNo);
	TX_8(value);
	END_CMD();
}

void WaveformDataRefress(uint8 page_id,uint8 control_id,uint8 channelNo)
{
	BEGIN_CMD();
	TX_8(6);
	TX_8(0x6b);
	TX_8(control_id);
	TX_8(page_id);
	TX_8(channelNo);
	END_CMD();
}


void GetSliderValue(uint8 page_id,uint8 control_id)
{
	BEGIN_CMD();
	TX_8(5);
	TX_8(0x68);
	TX_8(control_id);
	TX_8(page_id);
	END_CMD();
}

void SetSliderValue(uint8 page_id,uint8 control_id,uint8 value)
{
	BEGIN_CMD();
	TX_8(6);
	TX_8(0x5f);
	TX_8(control_id);
	TX_8(page_id);
	TX_8(value);
	END_CMD();
}


//===============================



