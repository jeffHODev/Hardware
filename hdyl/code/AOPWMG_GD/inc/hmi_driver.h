/*! 
 *  \file hmi_driver.h
 *  \brief 串口屏驱动文件
 *  \version 1.0
 *  \date 2012-2017
 *  \copyright hunda tech
 */

#ifndef _HMI_DRIVER_
#define _HMI_DRIVER_

#define CMD_MAX_SIZE 65        /*!<单条指令大小，根据需要调整，尽量设置大一些*/
#define QUEUE_MAX_SIZE 128   /*!< 指令接收缓冲区大小，根据需要调整，尽量设置大一些*/

#include "main.h"


/*! 
 *  \brief  延时
 *  \param  n 延时时间(毫秒单位)
 */
void DelayMS(unsigned int n);

/*! 
 *  \brief  发送握手命令
 */
void SetConnect(void);
/*! 
 *  \brief  发送显示屏复位命令
 */
void SetReset(void);

/*! 
 *  \brief     图片完全显示
 *  \param  x 位置X坐标
 *  \param  y 位置Y坐标
 *  \param  image_id 图片编号
 */
void Display_Image(uint16_t x,uint16_t y,uint8_t image_id);

/*! 
 *  \brief     图片切图显示
 *  \param  image_x 切图x坐标
 *  \param  image_y 切图Y坐标
 *  \param  image_w 切图宽度
 *  \param  image_h 切图高度
 *  \param  image_id 图片编号
 */
void DisplayCut_Image(uint16_t image_x,uint16_t image_y,uint16_t image_w, uint16_t image_h,uint8_t  image_id);


/*! 
 *  \brief  清除画面
 */
void GUI_CleanScreen(uint16_t color);

/*! 
 *  \brief  设置过滤色
 *  \param  x 位置X坐标
 *  \param  y 位置Y坐标
 *  \param  DisplayType
 *  \param  FontSize 字体
 *  \param  ImageNo 背景图片编号
 *  \param  BackColor 字体背景色
 *  \param  ForeColor 字体颜色
 *  \param  strings 字符串内容
 */
void Display_String(uint16_t x, uint16_t y,uint8_t DisplayType,uint8_t FontSize,uint8_t ImageNo,uint16_t BackColor,uint16_t ForeColor, unsigned char *strings );


/*! 
 *  \brief    消息显示
 *  \param  FontSize  字符大小设置
 *  \param  time   显示延迟时间，以秒为单位
 *  \param  strings 字符串内容
 */
void Display_Message(uint8_t FontSize,uint8_t time,unsigned char *strings );

/*! 
 *  \brief      画线
 *  \param  x0 起始位置X坐标
 *  \param  y0 起始位置Y坐标
 *  \param  x1 结束位置X坐标
 *  \param  y1 结束位置Y坐标
 *  \param ForeColor 直线颜色
 */
void GUI_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t ForeColor);


/*! 
 *  \brief      画空心矩形
 *  \param  x0 起始位置X坐标
 *  \param  y0 起始位置Y坐标
 *  \param  x1 结束位置X坐标
 *  \param  y1 结束位置Y坐标
 *  \param ForeColor 矩形颜色
 */
void GUI_Rectangle(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1 ,uint16_t ForeColor);

/*! 
 *  \brief      画实心矩形
 *  \param  x0 起始位置X坐标
 *  \param  y0 起始位置Y坐标
 *  \param  x1 结束位置X坐标
 *  \param  y1 结束位置Y坐标
 *  \param ForeColor 矩形颜色
 */
void GUI_RectangleFill(uint16_t x0, uint16_t y0, uint16_t x1,uint16_t y1 ,uint16_t ForeColor);



/*! 
 *  \brief      设置背光
 *  \param light_level 背光亮度值
 */
void SetBackLight(uint8_t light_level);

/*! 
 *  \brief   触摸屏设置
 *  \param enable 触摸使能
 */
void SetTouchPaneOption(uint8_t enbale);

/*! 
 *  \brief   校准触摸屏
 */
void	CalibrateTouchPane(void);

/*! 
 *  \brief  触摸屏测试
 *  \param enable 测试使能
 */
void TestTouchPane(uint8_t enbale);




/*! 
 *  \brief      设置当前画面
 *  \param  page_id 画面ID
 */
void SetPage(uint8_t page_id);

/*! 
 *  \brief      获取当前画面
 */
void GetPage(void);


/*! 
 *  \brief     设置文本值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 *  \param  str 文本值
 */
void SetLableValue(uint8_t page_id,uint8_t control_id,unsigned char *str);
/*! 
 *  \brief     设置数字控件值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 *  \param  number数字值
 */
void SetNumberValue(uint8_t page_id,uint8_t control_id,uint16_t number);

/*! 
 *  \brief     设置编辑框控件值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 *  \param  str 文本值
 */
void SetEditValue(uint8_t page_id,uint8_t control_id,unsigned char *str);

/*! 
 *  \brief     获取编辑框控件值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 */
void GetEditValue(uint8_t page_id,uint8_t control_id);

/*! 
 *  \brief     获取触摸编辑框控件值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 */
void GetTouchEditValue(uint8_t page_id,uint8_t control_id);
/*! 
 *  \brief      设置进度值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void SetProgressbarValue(uint8_t page_id,uint8_t control_id,uint8_t value);

/*! 
 *  \brief      设置复选框值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void SetCheckboxValue(uint8_t page_id,uint8_t control_id,uint8_t value);

/*! 
 *  \brief      获取复选框值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 */
void GetCheckboxValue(uint8_t page_id,uint8_t control_id);

/*! 
 *  \brief     设置指针控件值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void SetCircleGaugeValue(uint8_t page_id,uint8_t control_id,uint16_t value);
/*! 
 *  \brief     设置标尺控件值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void SetBarGaugeValue(uint8_t page_id,uint8_t control_id,uint16_t value);
/*! 
 *  \brief     设置水位计控件值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void SetWaterGaugeValue(uint8_t page_id,uint8_t control_id,uint16_t value);
/*! 
 *  \brief     设置温度计控件值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void SetThermometerValue(uint8_t page_id,uint8_t control_id,uint16_t value);
/*! 
 *  \brief     设置电池控件值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void SetBatteryValue(uint8_t page_id,uint8_t control_id,uint16_t value);
/*! 
 *  \brief     设置曲线波形控件值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 *  \param  channelNo 曲线通道号
 *  \param  value 数值
 */
void SetWaveformValue(uint8_t page_id,uint8_t control_id,uint8_t channelNo,uint8_t value);
/*! 
 *  \brief     清空曲线波形控件
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 */
void WaveformDataClear(uint8_t page_id,uint8_t control_id);
/*! 
 *  \brief     插入曲线波形控件值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 *  \param  channelNo 曲线通道号
 *  \param  value 数值
 */
void WaveformDataInsert(uint8_t page_id,uint8_t control_id,uint8_t channelNo,uint8_t value);
/*! 
 *  \brief     刷新曲线波形控件值
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 *  \param  channelNo 曲线通道号
 */
void WaveformDataRefress(uint8_t page_id,uint8_t control_id,uint8_t channelNo);

/*! 
 *  \brief      设置滑动条
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 *  \param  value 数值
 */
void SetSliderValue(uint8_t page_id,uint8_t control_id,uint8_t value);
/*! 
 *  \brief      获取滑动条位置数据
 *  \param  page_id 页面ID
 *  \param  control_id 控件ID
 */
void GetSliderValue(uint8_t page_id,uint8_t control_id);


void SendNU8(uint8_t *pData,uint16_t nDataLen);



#endif

