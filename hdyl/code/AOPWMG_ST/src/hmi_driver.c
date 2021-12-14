#include "hmi_driver.h"
#include "I2C.h"




static uint8_t  sendbuf[64];
static uint8_t len;
#define TX_8(P1) ADD_DATA((P1)&0xFF)  //发送单个字节
#define TX_8N(P,N) ADDNU8((uint8_t *)P,N)  //发送N个字节
#define TX_16(P1) TX_8((P1)>>8);TX_8(P1)  //发送16位整数

void ADD_DATA(uint8_t c)
{
	sendbuf[len++]= c;
}

void ADDNU8(uint8_t *pData,uint16_t nDataLen)
{
	uint16_t i = 0;
	for (;i<nDataLen;++i)
	{
		TX_8(pData[i]);
	}
}

void SendStrings(unsigned char *str)
{
	while(*str)
	{
		TX_8(*str);
		str++;
	}
}

 void BEGIN_CMD()
{
	len=0;
}

void END_CMD()
{
	ISendByte(SLA_WR,sendbuf,len);
}


uint8_t GetStrLen(unsigned char *str)
{
	uint8_t i=0;
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
	TX_8(0x48);
	END_CMD();
}

void SetReset(void)
{
	BEGIN_CMD();
	TX_8(0x4a);
	TX_8(0x52);
	TX_8(0x65);
	TX_8(0x73);
	TX_8(0x65);
	TX_8(0x74);
	END_CMD();
}

void Display_Image(uint16_t x,uint16_t y,uint8_t image_id)
{
	BEGIN_CMD();
	TX_8(0x49);
	TX_16(x);
	TX_16(y);
	TX_8(image_id);
	END_CMD();
}

void DisplayCut_Image(uint16_t image_x,uint16_t image_y,uint16_t image_w, uint16_t image_h,uint8_t  image_id)
{
	BEGIN_CMD();
	TX_8(0x4f);
	TX_16(image_x);
	TX_16(image_y);
	TX_16(image_w);
	TX_16(image_h);
	TX_8(image_id);
	END_CMD();
}

void GUI_CleanScreen(uint16_t color)
{
	BEGIN_CMD();
	TX_8(0x43);
	TX_16(color);
	END_CMD();
}

void Display_String(uint16_t x, uint16_t y,uint8_t DisplayType,uint8_t FontSize,uint8_t ImageNo,uint16_t BackColor,uint16_t ForeColor, unsigned char *strings )
{
	uint8_t len;
	len = GetStrLen(strings);
	BEGIN_CMD();
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

void Display_Message(uint8_t FontSize,uint8_t time,unsigned char *strings )
{
	uint8_t len;
	len = GetStrLen(strings);
	BEGIN_CMD();
	TX_8(0x4d);
	TX_8(FontSize);
	TX_8(time);
	SendStrings(strings);
	END_CMD();
}


void GUI_Rectangle(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1 ,uint16_t ForeColor)
{
	BEGIN_CMD();
	TX_8(0x52);
	TX_16(x0);
	TX_16(y0);
	TX_16(x1);
	TX_16(y1);
	TX_16(ForeColor);
	END_CMD();
}

void GUI_RectangleFill(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1  ,uint16_t ForeColor)
{
	BEGIN_CMD();
	TX_8(0x46);
	TX_16(x0);
	TX_16(y0);
	TX_16(x1);
	TX_16(y1);
	TX_16(ForeColor);
	END_CMD();
}

void GUI_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t ForeColor)
{
	BEGIN_CMD();
	TX_8(0x4c);
	TX_16(x0);
	TX_16(y0);
	TX_16(x1);
	TX_16(y1);
	TX_16(ForeColor);
	END_CMD();
}

void SetBackLight(uint8_t light_level)
{
	BEGIN_CMD();
	TX_8(0x42);
	TX_8(light_level);
	END_CMD();
}

void SetTouchPaneOption(uint8_t enbale)
{
	BEGIN_CMD();
	TX_8(0x51);
	TX_8(enbale);
	END_CMD();
}

void CalibrateTouchPane(void)
{
	BEGIN_CMD();
	TX_8(0x50);
	END_CMD();
}

void TestTouchPane(uint8_t enbale)
{
	BEGIN_CMD();
	TX_8(0x04);
	TX_8(0x54);
	TX_8(enbale);
	END_CMD();
}

///////////////////////////////////////////////
//控件控制函数

void SetPage(uint8_t page_id)
{
	BEGIN_CMD();     //head
	TX_8(0x70);		//cmd
	TX_8(page_id);	//data
	END_CMD();  	//checksum
}

void GetPage(void)
{
	BEGIN_CMD();
	TX_8(0x71);
	END_CMD();
}

void SetLableValue(uint8_t page_id,uint8_t control_id,unsigned char *str)
{
	uint8_t len;
	len = GetStrLen(str);
	BEGIN_CMD();
	TX_8(0x74);
	TX_8(control_id);
	TX_8(page_id);
	SendStrings(str);
	END_CMD();
}

void SetNumberValue(uint8_t page_id,uint8_t control_id,uint16_t number)
{
	BEGIN_CMD();
	TX_8(0x6e);
	TX_8(control_id);
	TX_8(page_id);
	TX_16(number);
	END_CMD();
}

void SetEditValue(uint8_t page_id,uint8_t control_id,unsigned char *str)
{
	uint8_t len;
	len = GetStrLen(str);
	BEGIN_CMD();
	TX_8(0x65);
	TX_8(control_id);
	TX_8(page_id);
	SendStrings(str);
	END_CMD();
}

void GetEditValue(uint8_t page_id,uint8_t control_id)
{
	BEGIN_CMD();
	TX_8(0x75);
	TX_8(control_id);
	TX_8(page_id);
	END_CMD();
}

void GetTouchEditValue(uint8_t page_id,uint8_t control_id)
{
	BEGIN_CMD();
	TX_8(0x7c);
	TX_8(control_id);
	TX_8(page_id);
	END_CMD();
}


void SetProgressbarValue(uint8_t page_id,uint8_t control_id,uint8_t value)
{
	BEGIN_CMD();
	TX_8(0x6f);
	TX_8(control_id);
	TX_8(page_id);
	TX_8(value);
	END_CMD();
}

void SetCheckboxValue(uint8_t page_id,uint8_t control_id,uint8_t value)
{
	BEGIN_CMD();
	TX_8(0x63);
	TX_8(control_id);
	TX_8(page_id);
	TX_8(value);
	END_CMD();
}


void GetCheckboxValue(uint8_t page_id,uint8_t control_id)
{
	BEGIN_CMD();
	TX_8(0x64);
	TX_8(control_id);
	TX_8(page_id);
	END_CMD();
}

void SetCircleGaugeValue(uint8_t page_id,uint8_t control_id,uint16_t value)
{
	BEGIN_CMD();
	TX_8(0x7a);
	TX_8(control_id);
	TX_8(page_id);
	TX_16(value);
	END_CMD();
}

void SetBarGaugeValue(uint8_t page_id,uint8_t control_id,uint16_t value)
{
	BEGIN_CMD();
	TX_8(0x61);
	TX_8(control_id);
	TX_8(page_id);
	TX_16(value);
	END_CMD();
}

void SetWaterGaugeValue(uint8_t page_id,uint8_t control_id,uint16_t value)
{
	BEGIN_CMD();
	TX_8(0x77);
	TX_8(control_id);
	TX_8(page_id);
	TX_16(value);
	END_CMD();
}


void SetThermometerValue(uint8_t page_id,uint8_t control_id,uint16_t value)
{
	BEGIN_CMD();
	TX_8(0x6d);
	TX_8(control_id);
	TX_8(page_id);
	TX_16(value);
	END_CMD();
}


void SetBatteryValue(uint8_t page_id,uint8_t control_id,uint16_t value)
{
	BEGIN_CMD();
	TX_8(0x79);
	TX_8(control_id);
	TX_8(page_id);
	TX_16(value);
	END_CMD();
}

void SetWaveformValue(uint8_t page_id,uint8_t control_id,uint8_t channelNo,uint8_t value)
{
	BEGIN_CMD();
	TX_8(0x66);
	TX_8(control_id);
	TX_8(page_id);
	TX_8(channelNo);
	TX_8(value);
	END_CMD();
}

void WaveformDataClear(uint8_t page_id,uint8_t control_id)
{
	BEGIN_CMD();
	TX_8(0x76);
	TX_8(control_id);
	TX_8(page_id);
	END_CMD();
}

void WaveformDataInsert(uint8_t page_id,uint8_t control_id,uint8_t channelNo,uint8_t value)
{
	BEGIN_CMD();
	TX_8(0x6a);
	TX_8(control_id);
	TX_8(page_id);
	TX_8(channelNo);
	TX_8(value);
	END_CMD();
}

void WaveformDataRefress(uint8_t page_id,uint8_t control_id,uint8_t channelNo)
{
	BEGIN_CMD();
	TX_8(0x6b);
	TX_8(control_id);
	TX_8(page_id);
	TX_8(channelNo);
	END_CMD();
}


void GetSliderValue(uint8_t page_id,uint8_t control_id)
{
	BEGIN_CMD();
	TX_8(0x68);
	TX_8(control_id);
	TX_8(page_id);
	END_CMD();
}

void SetSliderValue(uint8_t page_id,uint8_t control_id,uint8_t value)
{
	BEGIN_CMD();
	TX_8(0x5f);
	TX_8(control_id);
	TX_8(page_id);
	TX_8(value);
	END_CMD();
}


//===============================



