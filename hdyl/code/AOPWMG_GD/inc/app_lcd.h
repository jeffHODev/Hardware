#ifndef LCD_APP_H
#define LCD_APP_H

#define STATLUS_BACK_COLOR   0x07e0
#define BACK_COLOR   0xf7be
#define FORCE_COLOR  0
#define WARN_BACK_COLOR  0xf800

typedef struct
{
	unsigned char pagenum;
	unsigned char buttonnum;
	unsigned char framenum;
	unsigned char interrupt_flag;
}ui_struct;
void lcd_touch_interrupt(void);
int lcd_boot(void);
ui_struct *getuiCallback(void);
void lcd_init(void);
void reboot_progress(unsigned char value);

#endif

