#include "hmi_driver.h"
//#include "hmi_user_uart.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "stdio.h"
//#include "hw_config.h"
//#include "ulitity.h"
#include <math.h>
//#include "I2C.h"
#include "app_lcd.h"
#include "inout.h"
#include "sys.h"
#define TIME_100MS 10
#define PI 3.14
extern  uint8_t buf_cmd[LCD_BUF_SIZE];
volatile uint32_t  timer_tick_count = 0; //定时器节拍

uint8_t cmd_buffer[CMD_MAX_SIZE];
ui_struct ui_usr;
static uint8_t update_en = 0;
static unsigned char init_lcd=0;
static uint8_t page_Id_bk = 0;
static uint8_t page_Id = 0;
static uint8_t targe_Id = 0;


uint8_t   identifier,cnt;
uint8_t   command_cmd;
uint8_t   command_statu;
uint8_t   command_length;

const unsigned char String01[]="密码正确!";
const unsigned char String02[]="密码错误!";
const unsigned char String03[]="正在启动中";
const unsigned char String04[]="数据正确!";
const unsigned char String05[]="数据错误!";
unsigned char step=0;
void UpdateUI(void);

ui_struct *getuiCallback()
{
    return &ui_usr;
}
void lcd_touch_interrupt(void)
{
    uint8_t *pb;

    pb = 	buf_cmd	;
	 if(pb[INDEX_CMD+0] != PAGE_READ_CMD)//页面
	 {
    getTouch()->command = pb[INDEX_CMD+0];
    getTouch()->control_id = pb[INDEX_CMD+1];
    getTouch()->page_id = pb[INDEX_CMD+2];
    getTouch()->status = pb[INDEX_CMD+3];	 
	 }


    getTouch()->touch_flag = 1;
    switch(pb[INDEX_CMD+0])
    {
    case BUTTON_HIT:
        if(getTouch()->page_id == Main_PAGE )//main page
        {
            getTouch()->key = 0;

            step=1;
            if(getTouch()->control_id == WASH_ID||getTouch()->control_id == REBOOT_ID )
            {
                getTouch()->next_page = Main_PAGE;
                step=2;
            }
            else if(getTouch()->control_id == SETTING_ID)
            {
                step=3;
                getTouch()->next_page = SETTING_PAGE;


            }
#if DEV_TYPE == 0
            else if(getTouch()->control_id == FACTORY_ID)
            {
                // step=6;
                getTouch()->next_page = Main_PAGE;
                //  getTouch()->control_id = 0;

            }
#endif

            else
                getTouch()->next_page = Main_PAGE;

        }
        else if(getTouch()->page_id == SETTING_PAGE )//setting select page
        {
            step=4;
            getTouch()->key = 0;
            if(getTouch()->control_id == CALIBRATION_ID )
            {
                step=5;
                getTouch()->next_page = CALIBRATION_PAGE;
                // getTouch()->control_id = 0;

            }
            else if(getTouch()->control_id == SETTING_RE_ID)
            {
                step=6;
                getTouch()->next_page = Main_PAGE;
                //  getTouch()->control_id = 0;

            }
#if DEV_TYPE == 1
            else if(getTouch()->control_id == FACTORY_ID)
            {
                // step=6;
                getTouch()->next_page = SETTING_PAGE;
                //  getTouch()->control_id = 0;

            }
#endif

            else
                getTouch()->next_page = SETTING_PAGE;

        }
        else if(getTouch()->page_id == CALIBRATION_PAGE )//calibration page
        {
            step=7;
            if(getTouch()->control_id == CALIBRATION_OK_ID )
            {
                step=8;
                getTouch()->next_page = Main_PAGE;
                //getTouch()->control_id = 0;
                getTouch()->key = 0;
                getTouch()->page_id = Main_PAGE;
            }
            else if(getTouch()->control_id == CALIBRATION_RE_ID)
            {
                step=9;
                getTouch()->next_page = Main_PAGE;
                //  getTouch()->control_id = 0;
                getTouch()->key = 0;
                getTouch()->page_id = Main_PAGE;


            }
            else if(getTouch()->control_id == PH1_CAL_ID)//ph1 start
            {
                step=10;
                getTouch()->next_page = CALIBRATION_PAGE;
                getTouch()->last_ctrl_id = getTouch()->control_id;
                getTouch()->key = 0;
                //getTouch()->control_id = 0;
            }
            else if(getTouch()->control_id == PH2_CAL_ID)//ph2 start
            {
                step=11;
                getTouch()->next_page = CALIBRATION_PAGE;
                //getTouch()->control_id = 0;
                getTouch()->last_ctrl_id = getTouch()->control_id;
                getTouch()->key = 0;
            }
            else if(getTouch()->control_id == ORP_CAL_OK_ID)//ph2 start
            {
                step=11;
                getTouch()->next_page = CALIBRATION_PAGE;
                //getTouch()->control_id = 0;
                getTouch()->last_ctrl_id = getTouch()->key;
                getTouch()->key = 0;
            }

            // else if(getTouch()->control_id == ORP_CAL_ID)//orp start
            // {
            //     step=12;
            //     getTouch()->next_page = CALIBRATION_PAGE;
            // getTouch()->control_id = 0;
            //      getTouch()->last_ctrl_id = getTouch()->control_id;

            // }
            else
                getTouch()->next_page = CALIBRATION_PAGE;

        }

        break;
    case EDIT_KEY_HIT:
        if(pb[INDEX_CMD+0] == EDIT_KEY_HIT)//接收到指令
        {
            step = 13;
            getTouch()->next_page = CALIBRATION_PAGE;

            if(getTouch()->status == 0x6f)
            {
                pb[pb[INDEX_CMD-1]] = 0x00;
                strcpy(( char*)getTouch()->str,(const char*)&pb[INDEX_CMD+4]);
            }
        }

        break;
    case PAGE_READ_CMD:
        if(pb[INDEX_CMD+0] == PAGE_READ_CMD)//页面
        {
            step = 13;
            getTouch()->next_page = pb[INDEX_CMD+2];

        }

        break;
    case ENTER_HIT:
        getTouch()->next_page = CALIBRATION_PAGE;
        if(getTouch()->page_id == CALIBRATION_PAGE )
        {
            if(getTouch()->control_id==ORP_EDIT_ID)
            {
                GetTouchEditValue(CALIBRATION_PAGE,ORP_EDIT_ID);
                step=14;
                getTouch()->key = pb[INDEX_CMD+4];
                GetInOut()->update = 1;

            }
            if(getTouch()->control_id==PH1_EDIT_ID)
            {
                GetTouchEditValue(CALIBRATION_PAGE,PH1_EDIT_ID);
							step=14;
                getTouch()->key = pb[INDEX_CMD+4];
                GetInOut()->update = 1;
							 // delay_ms(100);


            }

        }
        break;



    }

    /* if(pb[INDEX_CMD+0] == 0x62)//接收button到指令
     {
         step=1;
         //ProcessMessage((PCTRL_MSG)cmd_buffer, size);//指令处理
         getTouch()->command = pb[INDEX_CMD+0];
         getTouch()->control_id = pb[INDEX_CMD+1];
         getTouch()->page_id = pb[INDEX_CMD+2];
         getTouch()->status = pb[INDEX_CMD+3];

         if( getTouch()->control_id == PH1_CAL_ID)
         {
             step=2;
             //if(pb[1]==PH1_CAL_OK_ID)
             getTouch()->control_id = pb[INDEX_CMD+1];
             getTouch()->last_ctrl_id = pb[INDEX_CMD+1];
         }
         else if( getTouch()->control_id == PH2_CAL_ID)
         {
             step=3;
             //if(pb[1]==PH2_CAL_OK_ID)
             getTouch()->control_id = pb[INDEX_CMD+1];
             getTouch()->last_ctrl_id = pb[INDEX_CMD+1];

         }
         else if( getTouch()->control_id == ORP_CAL_ID)
         {
             step=4;
             //if(pb[1]==ORP_CAL_OK_ID)
             getTouch()->control_id = pb[INDEX_CMD+1];
             getTouch()->last_ctrl_id = pb[INDEX_CMD+1];

         }

         else
         {
             step=5;
             getTouch()->control_id = pb[INDEX_CMD+1];



         }

         getTouch()->page_id = pb[INDEX_CMD+2];
         getTouch()->status = pb[INDEX_CMD+3];
         getTouch()->touch_flag = 1;
         step=6;

         if(getTouch()->page_id == Main_PAGE )
         {
             step=7;
             if(getTouch()->control_id == WASH_ID||getTouch()->control_id == REBOOT_ID )
             {
                 ;
                 step=8;
             }
             if(getTouch()->control_id == SETTING_ID)
             {
                 step=9;
                 getTouch()->page_id = SETTING_PAGE;
                 getTouch()->control_id = 0;

             }

         }
         else if(getTouch()->page_id == SETTING_PAGE )
         {
             step=10;
             if(getTouch()->control_id == CALIBRATION_ID )
             {
                 step=11;
                 getTouch()->page_id = CALIBRATION_PAGE;
                 getTouch()->control_id = 0;

             }
             if(getTouch()->control_id == SETTING_RE_ID)
             {
                 step=12;
                 getTouch()->page_id = Main_PAGE;
                 getTouch()->control_id = 0;

             }

         }
         else if(getTouch()->page_id == CALIBRATION_PAGE )
         {
             step=13;
             if(getTouch()->control_id == CALIBRATION_OK_ID )
             {
                 step=14;
                 getTouch()->page_id = Main_PAGE;
                 getTouch()->control_id = 0;

             }
             if(getTouch()->control_id == CALIBRATION_RE_ID)
             {
                 step=15;
                 getTouch()->page_id = Main_PAGE;
                 getTouch()->control_id = 0;

             }

             getTouch()->key  = pb[INDEX_CMD+4];
             if(getTouch()->control_id == CALIBRATION_OK_ID )
             {
                 step=14;
                 getTouch()->page_id = Main_PAGE;
                 getTouch()->control_id = 0;

             }

         }
         if(getTouch()->last_page_id!=getTouch()->page_id)
         {
             getTouch()->last_page_id=getTouch()->page_id;
             registerTick(LCD_TICK,0,0,1);


         }
         //GUI_CleanScreen( BACK_COLOR);
         //SetPage(getTouch()->page_id);//
     }
     else if(pb[INDEX_CMD+0] == 0x7c)//接收到指令
     {
         step = 16;
         getTouch()->control_id = pb[INDEX_CMD+1];
         getTouch()->page_id = pb[INDEX_CMD+2];
         getTouch()->status = pb[INDEX_CMD+3];
         if(getTouch()->status == 0x6f)
         {
             pb[pb[INDEX_CMD-1]] = 0x00;
             strcpy(getTouch()->str,&pb[INDEX_CMD+4]);
         }
     }

     if(getTouch()->page_id == CALIBRATION_PAGE&&(pb[INDEX_CMD+0] == 0x7b) )
     {
         step=13;
         if(getTouch()->control_id == CALIBRATION_OK_ID )
         {
             step=14;
             getTouch()->page_id = Main_PAGE;
             getTouch()->control_id = 0;

         }
         if(getTouch()->control_id == CALIBRATION_RE_ID)
         {
             step=15;
             getTouch()->page_id = Main_PAGE;
             getTouch()->control_id = 0;

         }

         getTouch()->key  = pb[INDEX_CMD+4];
         if(getTouch()->control_id == CALIBRATION_OK_ID )
         {
             step=14;
             getTouch()->page_id = Main_PAGE;
             getTouch()->control_id = 0;

         }
         getTouch()->key  = pb[INDEX_CMD+4];

     }
    */

    memset(buf_cmd,0,LCD_BUF_SIZE);



}
void lcd_init()
{


    /*清空串口接收缓冲区*/

    {
        init_lcd=0;
        queue_reset();
        SetReset();
        /*延时等待串口屏初始化完毕,必须等待500ms*/
        delay_ms(500);

        /*显示主页面*/
        SetPage(REBOOT_PAGE);//主页面Id号是4
        page_Id_bk = REBOOT_PAGE;

        SetBackLight(50);
        SetProgressbarValue(page_Id,PROGRESS_ID,10);
    }

}
void reboot_progress(unsigned char value)
{
    if(value>=100)
        value =100;
    SetProgressbarValue(REBOOT_PAGE,PROGRESS_ID,value);

}
//程序入口
int lcd_boot()
{
//    qsize  size = 0;
//    unsigned char tmp;
    static unsigned char value;


    if(value<=100)
    {
        value = value + 5;
        reboot_progress(  value);
        getTouch()->next_page = REBOOT_PAGE;
        //delay_ms(50);
        SetLableValue(page_Id_bk,START_TEXT_ID,(unsigned char *)String03);
        // Display_String(190, 100,0,0x10,0,126,0xF800, (unsigned char *)String03);
    }
    else
    {
        if(init_lcd==0)
        {
            init_lcd = 1;
            /*显示主页面*/
            SetPage(Main_PAGE);//主页面Id号是4
            page_Id_bk = Main_PAGE;
            // SetLableValue(page_Id_bk,START_TEXT_ID,(unsigned char *)String03);
            // Display_String(140, 106,0,0x10,0,126,0xF800, (unsigned char *)String03);
            getTouch()->page_id = Main_PAGE;
            getTouch()->next_page = Main_PAGE;
            getTouch()->last_page_id=getTouch()->page_id;
        }


    }

    // if(ui_usr.interrupt_flag==1&&init_lcd==1)
    // {
    //     ui_usr.interrupt_flag= 0 ;
    //     lcd_touch_interrupt();
    // queue_reset();
    // }
//	else
    {
//	     tmp = IRcvByte(SLA_WR);
//		  for(cnt =0; cnt <2; cnt++)
//		  {
//			  tmp = IRcvByte(SLA_WR);
//		  }
//		  tmp = IRcvByte(SLA_WR);

//		  tmp = IRcvByte(SLA_WR);

//		  tmp = IRcvByte(SLA_WR);

        queue_reset();

    }

    return 0;
}


/*!
 *  \brief  消息处理流程
 *  \param msg 待处理消息
 *  \param size 消息长度
 */
void ProcessMessage( PCTRL_MSG msg, uint16_t size )
{
    uint8_t cmd_type  = msg->cmd_type;//指令类型
    uint8_t control_id  = msg->control_id;//控件Id
    uint8_t page_id     = msg->page_id;   //页面Id
    uint8_t status        = msg->status;
    uint8_t key_type   = msg->key_type;
    uint8_t key_value  = msg->key_value;


    switch(cmd_type)
    {
    case NOTIFY_SET_PAGE:
        NotifySetPage(control_id);
        break;

    case NOTIFY_TOUCH_BUTTON:
        NotifyTouchButton(page_id,control_id,status,key_type,key_value);
        break;

    case NOTIFY_TOUCH_CHECKBOX:
        NotifyTouchCheckbox(page_id,control_id,status,key_type,key_value);
        break;

    case NOTIFY_TOUCH_SLIDER:
        NotifyTouchSlider(page_id,control_id,status,key_type,key_value);
        break;

    case NOTIFY_TOUCH_EDIT:
        NotifyTouchEdit(page_id,control_id,status,key_type,key_value);
        break;

    case NOTIFY_GET_EDIT:
        NotifyGetEdit((PEDIT_MSG)cmd_buffer,size-4);//去除帧头帧尾
        break;

    case NOTIFY_GET_TOUCH_EDIT:
        NotifyGetTouchEdit((PEDIT_MSG)cmd_buffer,size-4);//去除帧头帧尾
        break;

    case NOTIFY_GET_PAGE:
        NotifyGetPage(page_id,status);
        break;

    case NOTIFY_GET_CHECKBOX:
        NotifyGetCheckbox(page_id,control_id,status,key_type,key_value);
        break;

    case NOTIFY_GET_SLIDER:
        NotifyGetSlider(page_id,control_id,status,key_type,key_value);
        break;

    default:
        break;
    }
}



void UpdateUI()
{
    int i;
    int value;

    if(page_Id == Text_PAGE)
    {
        SetNumberValue(page_Id,19,20);
        SetNumberValue(page_Id,20,21);
        SetNumberValue(page_Id,21,28);
        SetNumberValue(page_Id,22,35);
        SetNumberValue(page_Id,23,46);
        SetNumberValue(page_Id,24,58);
        SetLableValue(page_Id,16,(unsigned char *)String03);
        SetLableValue(page_Id,17,(unsigned char *)String03);
        SetLableValue(page_Id,18,(unsigned char *)String03);

    }
    else if(page_Id == Waveform_PAGE)
    {
        for(i=-125; i<125; i+=1)
        {
            value = 20*sin(PI/50*i);
            SetWaveformValue(page_Id,9,2,value+119);
            SetWaveformValue(page_Id,9,1,value+59);
        }
        //WaveformDataRefress(page_Id,9,3);
    }
    else if(page_Id == Temperture_PAGE)
    {
        value =20;
        for(i=0; i<=10; i+=1)
        {
            SetThermometerValue(page_Id,9,value);
            SetNumberValue(page_Id,11,value-20);
            value +=10;
            delay_ms(200);
        }

        value =120;
        for(i=0; i<=10; i+=1)
        {
            SetThermometerValue(page_Id,9,value);
            SetNumberValue(page_Id,11,value-20);
            value -=10;
            delay_ms(200);
        }
    }
    else if(page_Id == Slider_PAGE)
    {
        value =0;
        for(i=0; i<10; i+=1)
        {
            SetProgressbarValue(page_Id,18,value);
            SetProgressbarValue(page_Id,20,value);
            value +=10;
            delay_ms(200);
        }
    }
    else if(page_Id == CircleGauge_PAGE)
    {
        value =0;
        for(i=0; i<=10; i+=1)
        {
            SetCircleGaugeValue(page_Id,37,value);
            SetBarGaugeValue(page_Id,40,value);

            value +=10;
            delay_ms(200);
        }

        value =100;
        for(i=0; i<=10; i+=1)
        {
            SetCircleGaugeValue(page_Id,37,value);
            SetBarGaugeValue(page_Id,40,value);

            value -=10;
            delay_ms(200);
        }
    }
    else if(page_Id == REBOOT_PAGE)
    {
        value =0;
        for(i=0; i<=10; i+=1)
        {
            SetBatteryValue(page_Id,44,value);
            SetWaterGaugeValue(page_Id,42,value);
            value +=10;
            delay_ms(100);
        }

        value =100;
        for(i=0; i<=10; i+=1)
        {
            SetBatteryValue(page_Id,44,value);
            SetWaterGaugeValue(page_Id,42,value);
            value -=10;
            delay_ms(100);
        }
    }
}

/*!
 *  \brief  页面切换通知
 *  \details  当前画面改变时，执行此函数
 *  \param status 切换页面是否成功
 */
void NotifySetPage(uint8_t status)
{
    //TODO: 添加用户代码
    if(status == SUCCESS)
        page_Id = page_Id_bk;

}

/*!
 *  \brief  获取当前页面Id号通知
 *  \details
 *  \param page_id 当前页面Id号
 */
void NotifyGetPage(uint8_t page_id,uint8_t status)
{
    //TODO: 添加用户代码
    if(status == SUCCESS)
        page_Id = page_id;
}

/*!
 *  \brief  按键控件通知
 *  \details  当触摸按键状态改变时，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0x55弹起，0x44按下
 *  \param type 按键功能码
 *  \param value 按键值
 */
void NotifyTouchButton(uint8_t page_id, uint8_t control_id, uint8_t  state,uint8_t type,uint8_t value)
{
    //TODO: 添加用户代码

    if(type == CHANGE_PAGE&& state == KEY_RELEASE)
    {
        page_Id = value;
        update_en = 1;
        //UpdateUI();
    }
    else if(type == ENTER&& state == KEY_RELEASE)
    {
        targe_Id = value;//目标编辑框Edit Id
        GetEditValue(page_Id,targe_Id);
    }
    else if(type == CHAR)
    {

    }
    else if(type == UPLOAD_CONTROL_ID)
    {
    }
    else if(type == CLEAR)
    {
    }

}


/*!
 *  \brief  触摸屏复选框控件状态通知
 *  \details  当复选框控件状态改变时，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0X53被选中，0x55没有选中
 */
void NotifyTouchCheckbox(uint8_t page_id, uint8_t control_id, uint8_t  state,uint8_t type,uint8_t value)
{
    //TODO: 添加用户代码
    if(state == SELECT)
        update_en = 1;
    //UpdateUI();
    //update_en = 1;
}

/*!
 *  \brief  获取复选框控件状态通知
 *  \details  当显示屏返回复选框状态时，执行此函数
 *  \param page_id 画面ID
 *  \param control_id 控件ID
 *  \param state 复选框状态：0X53被选中，0x55没有选中
 */
void NotifyGetCheckbox(uint8_t page_id, uint8_t control_id, uint8_t  state,uint8_t type,uint8_t value)
{
    //TODO: 添加用户代码
    if(state == SELECT)
    {
        update_en = 1;
    }
    //UpdateUI();
    //update_en = 1;
}

/*!
 *  \brief  滑块控件通知
 *  \details  当在触摸屏上滑动条改变时，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyTouchSlider(uint8_t page_id, uint8_t control_id, uint8_t  state,uint8_t type,uint8_t value)
{
    //TODO: 添加用户代码

    if(update_en != 1)
        SetNumberValue(page_Id,28,(uint16_t)value);
    //UpdateUI();
    //update_en = 1;
}


/*!
 *  \brief  触摸编辑框控件通知
 *  \details  当在触摸屏上滑动条改变时，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyTouchEdit(uint8_t page_id, uint8_t control_id, uint8_t  state,uint8_t type,uint8_t value)
{
    //TODO: 添加用户代码

    if(update_en != 1)
        //SetNumberValue(page_Id,28,(uint16_t)value);
        GetTouchEditValue(page_Id,control_id);

}


/*!
 *  \brief  滑块控件通知
 *  \details  获取滑块控件的数据，发送获取命令后，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyGetSlider(uint8_t page_id, uint8_t control_id, uint8_t  state,uint8_t type,uint8_t value)
{
    //TODO: 添加用户代码
    if(state == SUCCESS)
    {
        //成功获取数据
    }

    //UpdateUI();
    update_en = 1;
}

/*!
 *  \brief  获取编辑框文本数据通知
 *  \details  当向显示屏发送获取指令后，返回数据调用此函数
*  \param msg 待处理消息
 *  \param size 消息长度
 */
void NotifyGetEdit(PEDIT_MSG msg, uint16_t size)
{
    //  uint8_t cmd_type  = msg->cmd_type;//指令类型
//    uint8_t control_id  = msg->control_id;//控件ID
//    uint8_t page_id     = msg->page_id;
//    uint8_t status        = msg->status;
    /*msg->param[1]*/

    //测试密码值为1 2 3 4 对应的ASCII值是0x31 0x32 0x33 0x34

    if(msg->param[0] == 0x31 && msg->param[1] == 0x32 && msg->param[2] == 0x33 && msg->param[3] == 0x34)
    {
        Display_Message(0X18,2,(unsigned char *)String01);
    }
    else
    {
        Display_Message(0X18,2,(unsigned char *)String02);
    }

}

/*!
 *  \brief  获取触摸编辑框文本数据通知
 *  \details  当向显示屏发送获取指令后，返回数据调用此函数
*  \param msg 待处理消息
 *  \param size 消息长度
 */
void NotifyGetTouchEdit(PEDIT_MSG msg, uint16_t size)
{
//    uint8_t cmd_type  = msg->cmd_type;//指令类型
//    uint8_t control_id  = msg->control_id;//控件ID
//    uint8_t page_id     = msg->page_id;
//    uint8_t status        = msg->status;
    /*msg->param[1]*/

    //测试密码值为1 2 3 4 对应的ASCII值是0x31 0x32 0x33 0x34

    if(msg->param[0] == 0x31 && msg->param[1] == 0x32 && msg->param[2] == 0x33 && msg->param[3] == 0x34)
    {
        Display_Message(0X18,2,(unsigned char *)String04);
    }
    else
    {
        Display_Message(0X18,2,(unsigned char *)String05);
    }

}

/*!
 *  \brief  文本控件通知
 *  \details  当文本控件更新后，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 文本控件更新是否成功0x6f成功 0x65失败
 */
void NotifyLable(uint8_t page_id, uint8_t control_id,uint8_t status)
{
    //UpdateUI();
    update_en = 1;
}

/*!
 *  \brief  数字控件通知
 *  \details  当数字控件更新后，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 数字控件更新是否成功0x6f成功 0x65失败
 */
void NotifyNumber(uint8_t page_id, uint8_t control_id,uint8_t status)
{
    //UpdateUI();
    update_en = 1;
}


/*!
 *  \brief  进度条控件通知
 *  \details  调用SetProgressbarValue时，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifyProgressbar(uint8_t page_id, uint8_t control_id, uint8_t status)
{
    //TODO: 添加用户代码
}


/*!
 *  \brief  指针仪表控件通知
 *  \details  调用SetCircleGaugeValue时，执行此函数
 *  \param page_id 画面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifyCircleGauge(uint8_t page_id, uint8_t control_id, uint8_t status)
{
    //TODO: 添加用户代码
}

/*!
 *  \brief  标尺控件通知
 *  \details 调用SetBarGaugeValue时，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifyBarGauge(uint8_t page_id, uint8_t control_id, uint8_t  status)
{
    //TODO: 添加用户代码
}

/*!
 *  \brief  温度计控件通知
 *  \details  当选择控件变化时，执行此函数
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifyThermometer(uint8_t page_id, uint8_t control_id, uint8_t  status)
{
    //TODO: 添加用户代码
}

/*!
 *  \brief  水位计控件通知处理
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifyWaterGauge(uint8_t page_id, uint8_t control_id, uint8_t  status)
{
    //TODO: 添加用户代码
}

/*!
 *  \brief  电池控件通知处理
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifyBattery(uint8_t page_id, uint8_t control_id, uint8_t  status)
{
    //TODO: 添加用户代码
}


/*!
 *  \brief 更新曲线波形控件通知处理
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifySetWaveformValue(uint8_t page_id, uint8_t control_id, uint8_t  status)
{
    //TODO: 添加用户代码
}


/*!
 *  \brief 输入曲线波形控件数据通知处理
 *  \param page_id 页面ID
 *  \param control_id 控件ID
 *  \param status 值更新是否成功0x6f成功 0x65失败
 */
void NotifyWaveformDataInsert(uint8_t page_id, uint8_t control_id, uint8_t  status)
{
    //TODO: 添加用户代码
}




