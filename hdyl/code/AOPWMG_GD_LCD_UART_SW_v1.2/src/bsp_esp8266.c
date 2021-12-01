/**
  ******************************************************************************
  * 文件名程: bsp_esp8266.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: WiFi(ESP8266)底层驱动实现
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "bsp_esp8266.h"
#include "string.h"
#if CPU ==ST
extern UART_HandleTypeDef huart2;
#endif
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
static char * itoa( int value, char * string, int radix );

 uint8_t ucTcpClosedFlag=0;

/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 初始化ESP8266用到的GPIO引脚
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */


/**
  * 函数功能: 配置 ESP8266 USART 的 NVIC 中断优先级
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */




/**
  * 函数功能: 格式化输出，类似于C库中的printf，但这里没有用到C库
  * 输入参数: USARTx 串口通道，这里只用到了串口2，即USART2
  *		        Data   要发送到串口的内容的指针
  *			      ...    其他参数
  * 返 回 值: 无
  * 说    明：典型应用 USART2_printf( USART2, "\r\n this is a demo \r\n" );
  *            		     USART2_printf( USART2, "\r\n %d \r\n", i );
  *            		     USART2_printf( USART2, "\r\n %s \r\n", j );
  */
void USART_printf(UART_HandleTypeDef * USARTx, char * Data, ... )
{
	const char *s;
	int d;   
	char buf[16];
	unsigned char tmp;
	va_list ap;
	va_start(ap, Data);
	while ( * Data != 0 )     // 判断是否到达字符串结束符
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':	
					tmp = 0x0d;
					HAL_UART_Transmit(&huart2,&tmp,1,5);//回车符
					Data ++;
				break;
				case 'n':
					tmp = 0x0a;
					HAL_UART_Transmit(&huart2,&tmp,1,5);//换行符	
					Data ++;
				break;
				default:
					Data ++;
				break;
			}			 
		}
		else if ( * Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //字符串
					s = va_arg(ap, const char *);
					for ( ; *s; s++) 
					{
						HAL_UART_Transmit(USARTx,(uint8_t *)s,1,1);
						//while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
					}				
					Data++;				
				break;
					
				case 'd':			
					//十进制
					d = va_arg(ap, int);					
					itoa(d, buf, 10);					
					for (s = buf; *s; s++) 
					{
						//USART_SendData(USARTx,*s);
						HAL_UART_Transmit(USARTx,(uint8_t *)s,1,1);
						//while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
					}					
					Data++;				
				break;
					
				default:
					Data++;				
				break;				
			}		 
		}		
		else HAL_UART_Transmit(USARTx,(uint8_t *)(Data++),1,1);//USART_SendData(USARTx, *Data++);
		//while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TXE ) == RESET );
	}
}

/**
  * 函数功能: 将整形数据转换成字符串
  * 输入参数: radix =10 表示10进制，其他结果为0
  *           value 要转换的整形数
  *           buf 转换后的字符串
  *           radix = 10
  * 返 回 值: 无
  * 说    明：被USART_printf()调用
  */
static char * itoa( int value, char *string, int radix )
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;
	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}
	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}
	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';
		/* Make the value positive. */
		value *= -1;
	}
	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;
		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}
	/* Null terminate the string. */
	*ptr = 0;
	return string;
} /* NCL_Itoa */




unsigned char res;

/**
  * 函数功能: 停止使用ESP8266
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void  ESP8266_stop( void )
{
	ESP8266_RST_LOW_LEVEL();
	#if CPU == ST
    __HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
    __HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);	//使能空闲中断    
	//HAL_UART_Receive_IT(&huart2, &res, 1);	  // 重新注册一次，要不然下次收不到了
#endif
  //  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
    //HAL_UART_DMAStop(&huart2,modbus_usr.RS485_RX_BUFF,MODBUS_BSIZE);	
}
void  ESP8266_start( void )
{
	ESP8266_Rst (  );
		#if CPU == ST
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);	//使能空闲中断    

	HAL_UART_Receive_IT(&huart2, &res, 1);	  // 重新注册一次，要不然下次收不到了
    __HAL_UART_CLEAR_FLAG(&huart2,UART_IT_RXNE);
    __HAL_UART_CLEAR_FLAG(&huart2,UART_IT_IDLE);
 #endif
  //  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
    //HAL_UART_DMAStop(&huart2,modbus_usr.RS485_RX_BUFF,MODBUS_BSIZE);	
}

/**
  * 函数功能: 重启ESP8266模块
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：被ESP8266_AT_Test调用
  */
void ESP8266_Rst ( void )
{
#if 0
	 ESP8266_Cmd("AT+RST", "OK", "ready", 2500);   	
#else
	#if CPU == ST
	 HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, GPIO_PIN_RESET);
	 HAL_Delay(500); 
	 HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, GPIO_PIN_SET);
#endif	
#endif
}

/**
  * 函数功能: 对ESP8266模块发送AT指令
  * 输入参数: cmd，待发送的指令
  *           reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
  *           waittime，等待响应的时间
  * 返 回 值: 1，指令发送成功
  *           0，指令发送失败
  * 说    明：无
  */
bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime )
{    
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //从新开始接收新的数据包

	ESP8266_Usart ( "%s\r\n", cmd );

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //不需要接收数据
		return true;
	
	HAL_Delay(waittime);                 //延时
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

	PC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
	//printf("%s->%s\n",cmd,strEsp8266_Fram_Record .Data_RX_BUF);
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}

/**
  * 函数功能: 对ESP8266模块进行AT测试启动
  * 输入参数: 无
  * 返 回 值: 1，选择成功
  *           0，选择失败
  * 说    明：无
  */
bool ESP8266_AT_Test ( void )
{
	char count=0;
	
	ESP8266_RST_HIGH_LEVEL();	
	//HAL_Delay(1000);
	while(count<3)
	{
		if(ESP8266_Cmd("AT","OK",NULL,5)) return 1;
		ESP8266_Rst();
    HAL_Delay(1);
		++count;
	}
	return 0;
}

/**
  * 函数功能: 选择ESP8266模块的工作模式
  * 输入参数: enumMode，工作模式
  * 返 回 值: 1，选择成功
  *           0，选择失败
  * 说    明：无
  */
bool ESP8266_Net_Mode_Choose ( ENUM_Net_ModeTypeDef enumMode )
{
	bool result=0;
	char count=0;
	while(count<3)
	{
		switch ( enumMode )
		{
			case STA:
				result=ESP8266_Cmd ( "AT+CWMODE=1", "OK", "no change", 2 ); 
			break;
			case AP:
				result=ESP8266_Cmd ( "AT+CWMODE=2", "OK", "no change", 2 ); 
			break;
			case STA_AP:
				result=ESP8266_Cmd ( "AT+CWMODE=3", "OK", "no change", 2 ); 
			break;
			default:
				result=false;
			break;
		}
		if(result) return result;
		++count;
	}
	return 0;
}

/**
  * 函数功能: ESP8266模块连接外部WiFi
  * 输入参数: pSSID，WiFi名称字符串
  *           pPassWord，WiFi密码字符串
  * 返 回 值: 1，连接成功
  *           0，连接失败
  * 说    明：无
  */
bool ESP8266_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [120];
	char count=0;
	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	while(count<3)
	{
		if(ESP8266_Cmd(cCmd,"OK",NULL,5))return 1;
		++count;
	}
	return 0;	
}

/**
  * 函数功能: ESP8266模块创建WiFi热点
  * 输入参数: pSSID，WiFi名称字符串
  *           pPassWord，WiFi密码字符串
  *           enunPsdMode，WiFi加密方式代号字符串
  * 返 回 值: 1，创建成功
  *           0，创建失败
  * 说    明：无
  */
bool ESP8266_BuildAP ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode )
{
	char cCmd [120];
	char count=0;
	sprintf ( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode );
	while(count<3)
	{
		if(ESP8266_Cmd(cCmd,"OK",0,2))return 1;
		++count;
	}
	return 0;	
}

/**
  * 函数功能: ESP8266模块启动多连接
  * 输入参数: enumEnUnvarnishTx，配置是否多连接
  * 返 回 值: 1，配置成功
  *           0，配置失败
  * 说    明：无
  */
bool ESP8266_Enable_MultipleId ( ControlStatus enumEnUnvarnishTx )
{
	char cStr [20];
	char count=0;
	sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	while(count<3)
	{
		if(ESP8266_Cmd(cStr,"OK",0,2))return 1;
		++count;
	}
	return 0;		
}

/**
  * 函数功能: ESP8266模块连接外部服务器
  * 输入参数: enumE，网络协议
  *           ip，服务器IP字符串
  *           ComNum，服务器端口字符串
  *           id，模块连接服务器的ID
  * 返 回 值: 1，连接成功
  *           0，连接失败
  * 说    明：无
  */
bool ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

  switch (  enumE )
  {
		case enumTCP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );
		  break;
		
		case enumUDP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );
		  break;
		
		default:
			break;
  }

  if ( id < 5 )
    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  else
	  sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

	return ESP8266_Cmd ( cCmd, "OK", "ALREAY CONNECT", 3 );
	
}

/**
  * 函数功能: ESP8266模块开启或关闭服务器模式
  * 输入参数: enumMode，开启/关闭
  *           pPortNum，服务器端口号字符串
  *           pTimeOver，服务器超时时间字符串，单位：秒
  * 返 回 值: 1，操作成功
  *           0，操作失败
  * 说    明：无
  */
bool ESP8266_StartOrShutServer ( ControlStatus enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );

		return ( ESP8266_Cmd ( cCmd1, "OK", 0, 2 ) &&
						 ESP8266_Cmd ( cCmd2, "OK", 0, 2 ) );
	}
	
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

		return ESP8266_Cmd ( cCmd1, "OK", 0, 2 );
	}
}

/**
  * 函数功能: 获取ESP8266 的连接状态，较适合单端口时使用
  * 输入参数: 无
  * 返 回 值: 2，获得ip
  *           3，建立连接
  *           4，失去连接
  *           0，获取状态失败
  * 说    明：无
  */
uint8_t ESP8266_Get_LinkStatus ( void )
{
	if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 2 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:2\r\n" ) )
			return 2;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:3\r\n" ) )
			return 3;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:4\r\n" ) )
			return 4;		
	}
	return 0;
}

/**
  * 函数功能: 获取ESP8266 的端口（Id）连接状态，较适合多端口时使用
  * 输入参数: 无
  * 返 回 值: 端口（Id）的连接状态，低5位为有效位，分别对应Id5~0，某位若置1表该Id建立了连接，若被清0表该Id未建立连接
  * 说    明：无
  */
uint8_t ESP8266_Get_IdLinkStatus ( void )
{
	uint8_t ucIdLinkStatus = 0x00;
	
	
	if ( ESP8266_Cmd ( "AT+CIPSTATUS", "OK", 0, 2 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:0," ) )
			ucIdLinkStatus |= 0x01;
		else 
			ucIdLinkStatus &= ~ 0x01;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:1," ) )
			ucIdLinkStatus |= 0x02;
		else 
			ucIdLinkStatus &= ~ 0x02;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:2," ) )
			ucIdLinkStatus |= 0x04;
		else 
			ucIdLinkStatus &= ~ 0x04;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:3," ) )
			ucIdLinkStatus |= 0x08;
		else 
			ucIdLinkStatus &= ~ 0x08;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:4," ) )
			ucIdLinkStatus |= 0x10;
		else 
			ucIdLinkStatus &= ~ 0x10;	
	}
	return ucIdLinkStatus;
}

/**
  * 函数功能: 获取ESP8266 的 AP IP
  * 输入参数: pApIp，存放 AP IP 的数组的首地址
  *           ucArrayLength，存放 AP IP 的数组的长度
  * 返 回 值: 1，获取成功
  *           0，获取失败
  * 说    明：无
  */
uint8_t ESP8266_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength )
{
	char uc;
	
	char *pCh;
	
	
  ESP8266_Cmd ( "AT+CIFSR", "OK", 0, 2 );
	
	pCh = strstr ( (strEsp8266_Fram_Record .Data_RX_BUF), "APIP,\"" );
	
	if ( pCh )
		pCh += 6;
	
	else
		return 0;
	
	for ( uc = 0; uc < ucArrayLength; uc ++ )
	{
		pApIp [ uc ] = * ( pCh + uc);
		
		if ( pApIp [ uc ] == '\"' )
		{
			pApIp [ uc ] = '\0';
			break;
		}
	}
	return 1;
}

/**
  * 函数功能: 配置ESP8266模块进入透传发送
  * 输入参数: 无
  * 返 回 值: 1，配置成功
  *           0，配置失败
  * 说    明：无
  */
bool ESP8266_UnvarnishSend ( void )
{
	
	if ( ! ESP8266_Cmd ( "AT+CIPMODE=1", "OK", 0, 2 ) )
		return false;
	return  ESP8266_Cmd ( "AT+CIPSEND", "OK", ">", 2 );
	
}

/**
  * 函数功能: 配置ESP8266模块退出透传模式
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void ESP8266_ExitUnvarnishSend ( void )
{
	HAL_Delay(1);
	
	ESP8266_Usart ( "+++" );
	
	HAL_Delay(2);	
}

/**
  * 函数功能: ESP8266模块发送字符串
  * 输入参数: enumEnUnvarnishTx，声明是否已使能了透传模式
  *           pStr，要发送的字符串
  *           ucId，哪个ID发送的字符串
  *           ulStrLength，要发送的字符串的字节数
  * 返 回 值: 1，发送成功
  *           0，发送失败
  * 说    明：无
  */
bool ESP8266_SendString ( ControlStatus enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;
	
		
	if(enumEnUnvarnishTx )
	{
		ESP8266_Usart ( "%s", pStr );
		
		bRet = true;		
	}

	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
		
		ESP8266_Cmd ( cStr, "> ", 0, 2 );

		bRet = ESP8266_Cmd ( pStr, "SEND OK", 0, 2 );
  }
	
	return bRet;

}

/**
  * 函数功能: ESP8266模块接收字符串
  * 输入参数: enumEnUnvarnishTx，声明是否已使能了透传模式
  * 返 回 值: 接收到的字符串首地址
  * 说    明：无
  */
char * ESP8266_ReceiveString ( ControlStatus enumEnUnvarnishTx )
{
	char * pRecStr = 0;
		
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag );
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	
	if ( enumEnUnvarnishTx )
		pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	else 
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}
	return pRecStr;	
}




void ESP8266_USART_INT_FUN(void)
{
	uint8_t ucCh;
		uint8_t err,result;
	
	#if CPU==ST
	if(HAL_UART_GetError(&huart2)) 
		err=1;//检测到噪音、帧错误或校验错误
	else 
		err=0;

	
	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE)  != RESET )
	{
	   HAL_UART_Receive_IT(&huart2, &res, 1); 	  // 重新注册一次，要不然下次收不到了
		ucCh  = res;
		if ( strEsp8266_Fram_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) )                       	// 预留一个字节写结束符
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ]  = ucCh;

	}
	 	 
	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE)  == SET)                                         //	数据帧接收完毕
	{
        strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
		HAL_UART_Receive_IT(&huart2, &res, 1); 	  // 重新注册一次，要不然下次收不到了
		ucCh = res;
		ucTcpClosedFlag = strstr(strEsp8266_Fram_Record .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
		
  }
	#else
	
	#endif
}


/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
