/*
*********************************************************************************************************
*
*	模块名称 : APP_ESP8266驱动层应用
*	文件名称 : app_esp8266.c
*	版    本 : V1.0
*	说    明 : ESP8266驱动层应用
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01 Liujingtao  正式发布
*
*	Copyright (C), 2013-2014, 沈阳恒德医疗器械研发有限公司 ShenYang HengDe Medical Instruments Co.，Ltd.
*
*********************************************************************************************************
*/
#ifndef __APP_ESP8266_H_
#define __APP_ESP8266_H_
#define ESP_BUF_SIZE  64
typedef struct 
{
	 char User_ESP8266_Apssid[ESP_BUF_SIZE];			   //要连接的热点的名称
	 char User_ESP8266_ApPwd[ESP_BUF_SIZE] ;	   //要连接的热点的密钥
	 char User_Server_IP[ESP_BUF_SIZE];		//要连接的服务器的 IP
	 char User_Server_Port[ESP_BUF_SIZE];			  //要连接的服务器的端口
     unsigned char update;
	 unsigned char status;
}esp8266_params_stru;

void Demo_esp8266(void);
void esp8266_process(void);
esp8266_params_stru *GetEspStatus(void);

#endif


