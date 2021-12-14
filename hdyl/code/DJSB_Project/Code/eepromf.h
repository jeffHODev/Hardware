
#define ADDR_BASE 						0x4700							// ´æ´¢Æ÷»ù´¡µØÖ·

#define FLASH_INIT_CODE				ADDR_BASE+0
#define FLASH_ALLUSED_LOW			ADDR_BASE+1	
#define FLASH_ALLUSED_HIGH		ADDR_BASE+2
#define FLASH_TEMPALA_LOW			ADDR_BASE+3
#define FLASH_TEMPALA_HIGH		ADDR_BASE+4
#define FLASH_TDSALAR_LOW			ADDR_BASE+5
#define FLASH_TDSALAR_HIGH		ADDR_BASE+6

void Read_EEprom(void);
void Write_DATAFLASH_BYTE(void);