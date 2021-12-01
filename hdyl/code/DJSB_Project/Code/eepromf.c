#include "N76E003.h"
#include "Common.h"
#include "eepromf.h"
#include "Delay.h"
#include "Bsp_init.h"
#include "SFR_Macro.h"
#include "Function_define.h"

volatile unsigned char xdata page_buffer[24];


UINT8 Read_APROM_BYTE(UINT16 code *u16_addr)
{
	UINT8 rdata;
	rdata = *u16_addr>>8;
	return rdata;
}

/*****************************************************************************************************************
write_DATAFLASH_BYTE :
user can copy all this subroutine into project, then call this function in main.
******************************************************************************************************************/		
void Write_DATAFLASH_BYTE(void)
{
	unsigned char looptmp=0;
	unsigned int u16_addrl_r;
	
//Check page start address
	u16_addrl_r=(ADDR_BASE/128)*128;
//Save APROM data to XRAM0
//	for(looptmp=0;looptmp<0x80;looptmp++)
//	{
//		RAMtmp = Read_APROM_BYTE((unsigned int code *)(u16_addrl_r+looptmp));
//		page_buffer[looptmp]=RAMtmp;
//	}
// Modify customer data in XRAM
	page_buffer[FLASH_ALLUSED_LOW&0x7f]  = All_Use_Time%256;
	page_buffer[FLASH_ALLUSED_HIGH&0x7f] = All_Use_Time/256;
	page_buffer[FLASH_TEMPALA_LOW&0x7f]  = Temp_Alarm_Num%256;
	page_buffer[FLASH_TEMPALA_HIGH&0x7f] = Temp_Alarm_Num/256;
	page_buffer[FLASH_TDSALAR_LOW&0x7f]  = TDS_Alarm_Num%256;
	page_buffer[FLASH_TDSALAR_HIGH&0x7f] = TDS_Alarm_Num/256;
	
//Erase APROM DATAFLASH page
		IAPAL = u16_addrl_r&0xff;
		IAPAH = (u16_addrl_r>>8)&0xff;
		IAPFD = 0xFF;
	  set_IAPEN; 
		set_APUEN;
    IAPCN = 0x22; 		
 		set_IAPGO; 
		
//Save changed RAM data to APROM DATAFLASH
		set_IAPEN; 
		set_APUEN;
	  IAPCN = 0x21;
		for(looptmp=0;looptmp<24;looptmp++)
		{
			IAPAL = (u16_addrl_r&0xff)+looptmp;
      IAPAH = (u16_addrl_r>>8)&0xff;
			IAPFD = page_buffer[looptmp];
			set_IAPGO;			
		}
		clr_APUEN;
		clr_IAPEN;
}	

void Read_EEprom(void)
{
	All_Use_Time=((Read_APROM_BYTE(FLASH_ALLUSED_HIGH)<<8)+Read_APROM_BYTE(FLASH_ALLUSED_LOW));				// 累计使用次数
	Temp_Alarm_Num=((Read_APROM_BYTE(FLASH_TEMPALA_HIGH)<<8)+Read_APROM_BYTE(FLASH_TEMPALA_LOW));			// 温度报警数�?
	TDS_Alarm_Num=((Read_APROM_BYTE(FLASH_TDSALAR_HIGH)<<8)+Read_APROM_BYTE(FLASH_TDSALAR_LOW	));;		// TDS报警数�?
	if(TDS_Alarm_Num==0xffff||Temp_Alarm_Num==0xffff)
	{
		All_Use_Time=0;Temp_Alarm_Num=0;TDS_Alarm_Num=0;		
		Write_DATAFLASH_BYTE();
	
		//Led_Display();
		delay_ms(500);
	}
}



