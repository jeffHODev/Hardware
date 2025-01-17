#include "Protocol_C.h"
#include "crc.h"
#include "register.h"
#include "loraHW.h"
#include "sensor.h"
#include "rtc.h"
#include "nbiotHAL.h"
#include "modbus.h"
#include "nbiotHW.h"
#include "gateway.h"
#include "nbiotHAL.h"
#include "loraHAL.h"
extern unsigned char MODE_STAUS;
unsigned char txmode;

void SendPayloadPack(LORAHW_stru *loraNo,unsigned char ackMode);
unsigned char autoReportMode(void);
/******************************************************************************
数据结构：
        地址    设备地址编码          功能码     起始地址      字节计数      数组0定义         数组0数值 ...crc1       crc2
bytes   16        1              1       2        1          2             2           2      2
*******************************************************************************/
unsigned char ackmode;
Proctocol_C_stru protocolC;
unsigned char dev_code;
Proctocol_C_stru *protocolCstru(void)
{
    return &protocolC;
}
unsigned char *ackmodeStatus(void)
{
    return &ackmode;
}
void ProctocolInit()
{
    unsigned char *p;
    if(ROLE == GATEWAY)
    {
        p=ReadRegister(0x4006);//发送端源地址
        memcpy(protocolC.SrcAddr,p,ADDR_SIZE);//源地址
        p=ReadRegister(0x1200);//发送端源地址
        memcpy(protocolC.DesAddr,p,ADDR_SIZE);//源地址

    }
    else
    {
        p=ReadRegister(0xf007);//发送端源地址
        memcpy(protocolC.SrcAddr,p,ADDR_SIZE);//源地址
        p=ReadRegister(0x1200);//发送端源地址
        memcpy(protocolC.DesAddr,p,ADDR_SIZE);//源地址
    }



}
/****************************************************
功能：根据功能码判断数据长度
****************************************************/
unsigned char getPackLen(unsigned char  *p)
{
    /*#define  gateway_Rcmd	 			  0x17    //读数据
    #define  pc_cmd 					  0x18    //写数据
    #define  PC_readFrame				  0x30    //读网关数据帧
    #define  auto_report_cmd 			  0x51    //终端自动上报*/

    unsigned int len;
    len = 0;
    if(ROLE != GATEWAY)//终端设备
    {
        if(*getDataSrc() == DAT_FROM_LORA)//终端正常只能接收lora数据
        {
            if(p[17]  == pc_cmd||p[17]  == auto_report_cmd)
            {
                len = p[PAYLOAD_LEN_INDEX];//payload长度
                len = len + 2*ADDR_SIZE+1+1;//
                len = len+START_ADDR_SIZE + 1+4;
            }
            else if(p[17]  == gateway_Rcmd)
            {
                len =  2*ADDR_SIZE+1+1;//
                len = len+START_ADDR_SIZE + 1+4;
            }
            else if(p[17] !=PC_readFrame)//错误功能码
            {
                len = 0;
            }
        }
        else if(*getDataSrc() == DAT_FROM_NBIOT||*getDataSrc() == DAT_FROM_PC)//终端带有nbiot或者其他接口
        {
            /*#define  gateway_Rcmd	 			  0x17    //读数据
            #define  pc_cmd 					  0x18    //写数据
            #define  PC_readFrame				  0x30    //读网关数据帧
            #define  auto_report_cmd 			  0x51    //终端自动上报*/
            if(p[17]  == pc_cmd||p[17]  == auto_report_cmd)
            {
                len = p[PAYLOAD_LEN_INDEX];//payload长度
                len = len + 2*ADDR_SIZE+1+1;//
                len = len+START_ADDR_SIZE + 1+4;
            }
            else if(p[17]  == gateway_Rcmd)
            {
                len =  2*ADDR_SIZE+1+1;//
                len = len+START_ADDR_SIZE + 1+4;
            }
            else if(p[17] !=PC_readFrame)//
                len = 0;

        }

    }
    else  //*********************网关******************************
    {
        if(*getDataSrc() == DAT_FROM_LORA)//接收终端的数据，lora只能接收终端数据
        {
            if(p[17]  == pc_cmd||p[17]  == auto_report_cmd||p[17]  == PC_readFrame)
            {
                len = p[PAYLOAD_LEN_INDEX];//payload长度
                len = len + 2*ADDR_SIZE+1+1;//
                len = len+START_ADDR_SIZE + 1+4;
            }
            else
            {
                len = 0;//
            }


            /* unsigned int regaddr;
             regaddr = uchar2uint(&p[18]);
             if(regaddr!= 0x4001)
             {
                 len = p[PAYLOAD_LEN_INDEX];//payload长度
                 len = len + 2*ADDR_SIZE+1+1;//
                 len = len+START_ADDR_SIZE + 1+4;
             }
             else
             {
                 len =  2*ADDR_SIZE+1+1;//
                 len = len+START_ADDR_SIZE + 1+4;
             }*/
        }
        else if(*getDataSrc() == DAT_FROM_NBIOT||*getDataSrc() == DAT_FROM_PC||*getDataSrc() == DAT_FROM_RELAY)
        {
            /*#define  gateway_Rcmd	 			  0x17    //读数据
            #define  pc_cmd 					  0x18    //写数据
            #define  PC_readFrame				  0x30    //读网关数据帧
            #define  auto_report_cmd 			  0x51    //终端自动上报*/
            if(p[17]  == pc_cmd||p[17]  == auto_report_cmd)//p[17]  == PC_readFrame||
            {
                len = p[PAYLOAD_LEN_INDEX];//payload长度
                len = len + 2*ADDR_SIZE+1+1;//
                len = len+START_ADDR_SIZE + 1+4;
            }
            else if(p[17]  == gateway_Rcmd||p[17]  == PC_readFrame)
            {
                len =  2*ADDR_SIZE+1+1;//
                len = len+START_ADDR_SIZE + 1+4;
            }
            else
                len = 0;

        }

    }

    return len;

}

/*result = 8;//数据校验正确
result = 3;//功能码错误,数据包格式正确
result = 7;//错误应答数据包，数据包无起始地址
result = 6;//功能码不对，且为读命令数据包格式
#define  gateway_Rcmd	 			  0x17    //读数据
#define  pc_cmd 					  0x18    //写数据
#define  PC_readFrame				  0x30    //读网关数据帧
#define  auto_report_cmd 			  0x51    //终端自动上报*/
void packAnaly(unsigned char  *p,unsigned char result,unsigned int len)
{
    unsigned int payload_len;
    protocolC.devCode = p[2*ADDR_SIZE];
    protocolC.funCode = p[2*ADDR_SIZE+1];
    if(result == 8||result == 3||result == 6)
    {
        protocolC.startAddr[0]=p[2*ADDR_SIZE+1+1];
        protocolC.startAddr[1]=p[2*ADDR_SIZE+1+1+1];
    }
    else//错误应答数据包，数据包无起始地址
    {
        protocolC.startAddr[0]=0;
        protocolC.startAddr[1]=0;
    }

    if(ROLE != GATEWAY)
    {
        if(result == 6||result == 3)
        {
            protocolC.dataType = 2;//应答错误帧
            protocolC.funCode = 0x17;
            protocolC.payloadLenL = 0;
            protocolC.payloadLenH = 0;
            payload_len = protocolC.payloadLenL ;//payload长度
            memcpy(protocolC.payload,&p[ADDR_SIZE+ADDR_SIZE+1+1],payload_len);
        }
        else if(result == 7 )
        {
            protocolC.dataType = 1;//应答错误帧
            protocolC.payloadLenL = 1;
            protocolC.payloadLenH = 0;
            payload_len =protocolC.payloadLenL ;//payload长度
            memcpy(protocolC.payload,&p[ADDR_SIZE+ADDR_SIZE+1+1],payload_len);

        }
        else if(result == 8)
        {
            protocolC.dataType = 0;//应答错误帧
            protocolC.payloadLenL = p[PAYLOAD_LEN_INDEX];
            protocolC.payloadLenH = 0;
            payload_len =protocolC.payloadLenL ;//payload长度
            if(len<payload_len)
            {
                payload_len = len - 2*ADDR_SIZE - 1 - 1;
                payload_len =payload_len - START_ADDR_SIZE -1;
                payload_len = payload_len - 4;
                //memcpy(protocolC.payload,&p[ADDR_SIZE+ADDR_SIZE+1+1+2+1],payload_len);
            }

            memcpy(protocolC.payload,&p[ADDR_SIZE+ADDR_SIZE+1+1+2+1],payload_len);

        }


    }
    else   /*****************************网关*************************************************/
    {
        /*result = 8;//数据校验正确
        result = 3;//功能码错误,数据包格式正确
        result = 7;//错误应答数据包，数据包无起始地址
        result = 6;//功能码不对，且为读命令数据包格式
        #define  gateway_Rcmd	 			  0x17    //读数据
        #define  pc_cmd 					  0x18    //写数据
        #define  PC_readFrame				  0x30    //读网关数据帧
        #define  auto_report_cmd 			  0x51    //终端自动上报*/

        if(result == 6||result == 3)
        {
            protocolC.dataType = 2;//应答错误帧
            protocolC.funCode = 0x17;
            protocolC.payloadLenL = 0;
            protocolC.payloadLenH = 0;
            payload_len = protocolC.payloadLenL ;//payload长度
            memcpy(protocolC.payload,&p[ADDR_SIZE+ADDR_SIZE+1+1],payload_len);

        }
        else if(result == 7 )
        {
            // protocolC.funCode = 0x17;
            protocolC.dataType = 1;//应答错误帧
            protocolC.payloadLenL = 1;
            protocolC.payloadLenH = 0;
            payload_len =protocolC.payloadLenL ;//payload长度
            memcpy(protocolC.payload,&p[ADDR_SIZE+ADDR_SIZE+1+1],payload_len);

        }
        else if(result == 8)
        {
            protocolC.dataType = 0;//应答错误帧
            protocolC.payloadLenL = p[PAYLOAD_LEN_INDEX];
            protocolC.payloadLenH = 0;
            payload_len =protocolC.payloadLenL ;//payload长度
            if(len<payload_len)
            {
                payload_len = len - 2*ADDR_SIZE - 1 - 1;
                payload_len =payload_len - START_ADDR_SIZE -1;
                payload_len = payload_len - 4;
                //memcpy(protocolC.payload,&p[ADDR_SIZE+ADDR_SIZE+1+1+2+1],payload_len);
            }

            memcpy(protocolC.payload,&p[ADDR_SIZE+ADDR_SIZE+1+1+2+1],payload_len);

        }


    }
    /* if(result <6)
     {
         if(p[2*ADDR_SIZE+1] !=gateway_Rcmd)
         {
             protocolC.payloadLenL = p[PAYLOAD_LEN_INDEX];
             protocolC.payloadLenH = 0;
             payload_len =p[PAYLOAD_LEN_INDEX] ;//payload长度
             //payload_len =payload_len<<8 ;//payload长度
             //payload_len =payload_len + p[PAYLOAD_LEN_INDEX+1] ;//payload长度
             memcpy(protocolC.payload,&p[ADDR_SIZE+ADDR_SIZE+1+1+2+1],payload_len);
         }
         else
         {
             if(ROLE != GATEWAY)
             {
                 protocolC.payloadLenH = 0;
                 protocolC.payloadLenL = p[PAYLOAD_LEN_INDEX];
                 payload_len =p[PAYLOAD_LEN_INDEX] ;//payload长度
                 //  payload_len =payload_len<<8 ;//payload长度
                 //  payload_len =payload_len + p[PAYLOAD_LEN_INDEX+1] ;//payload长度

             }
             else
             {
                 if(*getDataSrc() == DAT_FROM_LORA)
                 {
                     protocolC.payloadLenL = p[PAYLOAD_LEN_INDEX];
                     protocolC.payloadLenH = 0;
                     payload_len =p[PAYLOAD_LEN_INDEX] ;//payload长度
                     //payload_len =payload_len<<8 ;//payload长度
                     //payload_len =payload_len + p[PAYLOAD_LEN_INDEX+1] ;//payload长度
                     memcpy(protocolC.payload,&p[ADDR_SIZE+ADDR_SIZE+1+1+2+1],payload_len);
                 }
                 else if(*getDataSrc() == DAT_FROM_NBIOT||*getDataSrc() == DAT_FROM_PC||*getDataSrc() == DAT_FROM_RELAY)
                 {
                     protocolC.payloadLenH = 0;
                     protocolC.payloadLenL = p[PAYLOAD_LEN_INDEX];
                     payload_len =p[PAYLOAD_LEN_INDEX] ;//payload长度
                     //  payload_len =payload_len<<8 ;//payload长度
                     //  payload_len =payload_len + p[PAYLOAD_LEN_INDEX+1] ;//payload长度


                 }
             }

         }
     }*/


}

/***************************************************
功能：数据包协议解析
参数：p待解析数据包
      len: 为0代表数据不合法或者有错误功能码
      result:0为解析成功数据；4为需要转发数据非本机数据
 ****************************************************/
unsigned char protocolCAnaly(unsigned char  *p)
{
    unsigned int calCRC;
    unsigned int recCRC;
    unsigned int len;
    unsigned char result;
    result = 0;

    //memcpy(protocolC.SrcAddr,&(p[ADDR_SIZE]),ADDR_SIZE);//源地址
    len = getPackLen(p);
    if(len<=16&&len>0)//数据小于等于16为非法数据
    {
        return 5;
    }
    else if(len == 0)//功能码错误或者是
    {
        len = p[PAYLOAD_LEN_INDEX];//payload长度
        len = len + 2*ADDR_SIZE+1+1;//
        len = len+START_ADDR_SIZE + 1+4;

        calCRC=CRC_Compute(p,len-2);//计算所接收数据的CRC
        recCRC=p[len-1]|(((u16)p[len-2])<<8);//接收到的CRC(低字节在前，高字节在后)
        if(calCRC!=recCRC||(calCRC == 0))//CRC校验错误
        {

            len =  2*ADDR_SIZE+1+1;//
            len = len+START_ADDR_SIZE + 1+4;

            calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-4-ADDR_SIZE-ADDR_SIZE);
            recCRC=p[len-3]|(((u16)p[len-4])<<8);//接收到的CRC(低字节在前，高字节在后)
            if(calCRC==recCRC&&calCRC!=0)//负载数据校验正确
            {
                memcpy(protocolC.DesAddr,p,ADDR_SIZE); //目标地址
                result = 6;//功能码不对，且为读命令数据包格式
                packAnaly(p,result,len);
                if(array_comp(protocolC.DesAddr,protocolC.SrcAddr,ADDR_SIZE) == 0)//判断地址是否一致
                {
                    result = 0;//本机数据
                    WriteRegisteSet(0x1200,p+ADDR_SIZE,8);//发送端源地址
                    memcpy(protocolC.DesAddr,p+ADDR_SIZE,ADDR_SIZE); //目标地址
                }
                else
                {
                    result = 4;//非本机数据
                    WriteRegisteSet(0x1200,p,8);
                    memcpy(protocolC.DesAddr,p,ADDR_SIZE); //发送端目的地址

                }
            }
            else
            {
                len =  2*ADDR_SIZE+1+1;//
                len = len+ 1+4;

                calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-4-ADDR_SIZE-ADDR_SIZE);
                recCRC=p[len-3]|(((u16)p[len-4])<<8);//接收到的CRC(低字节在前，高字节在后)
                if(calCRC==recCRC&&calCRC!=0)//负载数据校验正确
                {
                    memcpy(protocolC.DesAddr,p,ADDR_SIZE); //目标地址
                    result = 7;//错误应答数据包，数据包无起始地址
                    packAnaly(p,result,len);
                    if(array_comp(protocolC.DesAddr,protocolC.SrcAddr,ADDR_SIZE) == 0)//判断地址是否一致
                    {
                        result = 0;//本机数据
                        WriteRegisteSet(0x1200,p+ADDR_SIZE,8);//发送端源地址
                        memcpy(protocolC.DesAddr,p+ADDR_SIZE,ADDR_SIZE); //目标地址
                    }
                    else
                    {
                        result = 4;//非本机数据
                        WriteRegisteSet(0x1200,p,8);
                        memcpy(protocolC.DesAddr,p,ADDR_SIZE); //发送端目的地址

                    }
                }
                else
                    result = 2;//数据错误
            }
        }
        else if(calCRC==recCRC&&calCRC!=0)//CRC校验正确，整个数据包校验正确
        {
            memcpy(protocolC.DesAddr,p,ADDR_SIZE); //目标地址
            calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-4-ADDR_SIZE-ADDR_SIZE);
            recCRC=p[len-3]|(((u16)p[len-4])<<8);//接收到的CRC(低字节在前，高字节在后)
            if(calCRC==recCRC)//负载数据校验正确
            {
                if(len > 25)
                    result = 8;
                else
                    result = 3;//功能码错误，数据包格式正确
                packAnaly(p,result,len);
                if(array_comp(protocolC.DesAddr,protocolC.SrcAddr,ADDR_SIZE) == 0)//判断地址是否一致
                {
                    result = 0;//本机数据
                    WriteRegisteSet(0x1200,p+ADDR_SIZE,8);//发送端源地址
                    memcpy(protocolC.DesAddr,p+ADDR_SIZE,ADDR_SIZE); //目标地址
                }
                else
                {
                    result = 4;//非本机数据
                    WriteRegisteSet(0x1200,p,8);
                    memcpy(protocolC.DesAddr,p,ADDR_SIZE); //发送端目的地址

                }
            }
        }
    }
    else
    {
        calCRC=CRC_Compute(p,len-2);//计算所接收数据的CRC
        recCRC=p[len-1]|(((u16)p[len-2])<<8);//接收到的CRC(低字节在前，高字节在后)
        if(calCRC!=recCRC||(calCRC == 0))//CRC校验错误
        {
            result = 2;//数据校验错误
        }
        if(calCRC==recCRC&&calCRC!=0)//CRC校验正确，整个数据包校验正确
        {
            memcpy(protocolC.DesAddr,p,ADDR_SIZE); //目标地址
            calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-4-ADDR_SIZE-ADDR_SIZE);
            recCRC=p[len-3]|(((u16)p[len-4])<<8);//接收到的CRC(低字节在前，高字节在后)
            if(calCRC==recCRC)//负载数据校验正确
            {
                result = 8;//数据校验正确
                packAnaly(p,result,len);
                if(array_comp(protocolC.DesAddr,protocolC.SrcAddr,ADDR_SIZE) == 0)//判断地址是否一致
                {
                    result = 0;//本机数据
                    WriteRegisteSet(0x1200,p+ADDR_SIZE,8);//发送端源地址
                    memcpy(protocolC.DesAddr,p+ADDR_SIZE,ADDR_SIZE); //目标地址
                }
                else
                {
                    result = 4;//非本机数据
                    WriteRegisteSet(0x1200,p,8);
                    memcpy(protocolC.DesAddr,p,ADDR_SIZE); //发送端目的地址

                }
            }
            else
            {
                result = 1;//负载数据校验错误
            }
        }
    }
    /* calCRC=CRC_Compute(p,len-2);//计算所接收数据的CRC
     recCRC=p[len-1]|(((u16)p[len-2])<<8);//接收到的CRC(低字节在前，高字节在后)
     if(calCRC!=recCRC||(calCRC == 0))//CRC校验正确，整个数据包校验正确
     {
         unsigned int tmp;

         if((p[PAYLOAD_LEN_INDEX]+2)>len)
             result = 7;
         else
         {



             calCRC=CRC_Compute(p,len-p[PAYLOAD_LEN_INDEX]-2);//计算所接收数据的CRC
             recCRC=p[len-p[PAYLOAD_LEN_INDEX]-1]|(((u16)p[len-p[PAYLOAD_LEN_INDEX]-2])<<8);//接收到的CRC(低字节在前，高字节在后)
         }

         if(calCRC==recCRC&&calCRC!=0)//CRC校验正确，整个数据包校验正确
         {
             len = len - p[PAYLOAD_LEN_INDEX];
             result = 8;
         }
         else
         {
             calCRC=CRC_Compute(p,len-p[PAYLOAD_LEN_INDEX]-2-START_ADDR_SIZE);//计算所接收数据的CRC
             recCRC=p[len-p[PAYLOAD_LEN_INDEX]-1-START_ADDR_SIZE]|(((u16)p[len-p[PAYLOAD_LEN_INDEX]-2-START_ADDR_SIZE])<<8);//接收到的CRC(低字节在前，高字节在后)
             if(calCRC==recCRC)//CRC校验正确，整个数据包校验正确
             {
                 len = len - p[PAYLOAD_LEN_INDEX]-START_ADDR_SIZE;
             }
             result = 6;
         }

     }
     if(calCRC==recCRC&&calCRC!=0)//CRC校验正确，整个数据包校验正确
     {
         memcpy(protocolC.DesAddr,p,ADDR_SIZE); //目标地址
         calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-4-ADDR_SIZE-ADDR_SIZE);
         recCRC=p[len-3]|(((u16)p[len-4])<<8);//接收到的CRC(低字节在前，高字节在后)
         if(calCRC==recCRC)//目的地址正确以及数据校验正确
         {
             packAnaly(p,result);
             if(array_comp(protocolC.DesAddr,protocolC.SrcAddr,ADDR_SIZE) == 0)//判断地址是否一致
             {
                 result = 0;//本机数据
                 WriteRegisteSet(0x1200,p+ADDR_SIZE,8);//发送端源地址
                 memcpy(protocolC.DesAddr,p+ADDR_SIZE,ADDR_SIZE); //目标地址
             }
             else
             {
                 result = 4;//非本机数据
                 WriteRegisteSet(0x1200,p,8);
                 memcpy(protocolC.DesAddr,p,ADDR_SIZE); //发送端目的地址

             }
         }
         else
         {
             result = 1;//目的地址校验错误
         }
     }
     else
     {
         calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-4-ADDR_SIZE-ADDR_SIZE);//计算所接收数据的CRC
         recCRC=p[len-3]|(((u16)p[len-4])<<8);//接收到的CRC(低字节在前，高字节在后)
         if(calCRC==recCRC)//16字节地址校验不正确
         {

             packAnaly(p,result);

             result = 2;
         }
         else
         {
             result = 3;
         }

     }*/
    if(result == 4)//转发数据
    {
        if(ROLE == GATEWAY)//设备为网关，做数据转发
        {
            if(equip_bind_analy(protocolC.DesAddr)==0)//本机绑定终端
            {
                LORAHW_stru loraNo;
                loraNo.loraNo = 0;
                loraNo.mode =  TransmitMode;
                if(protocolC.startAddr[0]==0&&protocolC.startAddr[1]==0)
                    ;
                else
                {
                    SendLoraData(&loraNo,p);
                    txmode = 1;
                }
            }

        }

    }

    /*if(result == 1)
    {
       ackmode = 4;
       LORAHW_stru loraNo;
       loraNo.loraNo = 0;
       loraNo.mode =  TransmitMode;
       SendPayloadPack(&loraNo,4);
    }*/
    return result;
}
unsigned char getTxmode()
{
    return txmode;
}
void dev_params_pack(unsigned int start_addr,unsigned char sensors_prams,unsigned char len,unsigned char dataSrc)
{
//         unsigned int reg_addr;
    unsigned char j,datatype;
    unsigned char *p;
    if(sensors_prams<=rh)
        datatype = SIGN_DOT1;
    else
        datatype = UNSIGNED;
    if(sensors_prams == auto_report_time)
    {
        len = AUTO_REPORT_SIZE;
        sensors_prams = temperature;

    }
    data_in_flash_stru *data_in_f;
    //reg_addr = ReadRegisterOffsetAddr(start_addr);
    if(dataSrc == 0)
        p=ReadRegister(start_addr);//
    else
        p = readDataFrame();

    if(start_addr == 0x4001)
    {
        unsigned int count;
        unsigned char q[2];
        count = p[0];
        count = count << 8;
        count = count + p[1];
        if(count >= DATA_MAX_COUNT)
        {
            count = DATA_MAX_COUNT;


        }
        q[0] = (unsigned char)(count >> 8);
        q[1] =(unsigned char) count ;
        for(j=0; j<len; j=j+4)
        {

            unsigned char k;
            k = j/4;
            if(k%2)
            {

                if(sensors_prams == temperature)
                    protocolC.payload[j] =rh;
                else if(sensors_prams == rh)
                    protocolC.payload[j] = temperature;
                else
                    protocolC.payload[j] = sensors_prams;
            }
            else
            {
                protocolC.payload[j] = sensors_prams;// 0 1 2 3	4 5 6 7 8 9 10 11
            }

            protocolC.payload[j+1] = datatype;
            memcpy(&protocolC.payload[j+2],q,2);
        }
    }
    else
    {
        for(j=0; j<len; j=j+4)
        {

            unsigned char k;
            k = j/4;
            if(k%2)
            {

                if(sensors_prams == temperature)
                    protocolC.payload[j] =rh;
                else if(sensors_prams == rh)
                    protocolC.payload[j] = temperature;
                else
                    protocolC.payload[j] = sensors_prams;
            }
            else
            {
                protocolC.payload[j] = sensors_prams;// 0 1 2 3   4 5 6 7 8 9 10 11
            }

            protocolC.payload[j+1] = datatype;
            memcpy(&protocolC.payload[j+2],p+j/2,2);
        }

    }


}

void get_equipmentData()
{
    //enum FUNC_CODE fun_code;
    unsigned int start_addr;
    unsigned char sensor_params;
    unsigned char i;
    unsigned char len;

    dev_code = protocolC.devCode;
    start_addr =protocolC.startAddr[0];
    start_addr =start_addr<<8;
    start_addr =start_addr+protocolC.startAddr[1];
    len =  protocolC.payloadLenH;
    len =  len<<8;
    len =  len+ protocolC.payloadLenL;

    for(i=0; i<len; i++)
    {
        if(start_addr == reg_soil_temp_5||start_addr == reg_soil_temp_4||start_addr == reg_soil_temp_3||
                start_addr == reg_soil_temp_2||start_addr == reg_soil_temp_1)
            sensor_params = temperature;
        else if(start_addr == reg_soil_rh_5||start_addr == reg_soil_rh_4||start_addr == reg_soil_rh_3||
                start_addr == reg_soil_rh_2||start_addr == reg_soil_rh_1)
            sensor_params = rh;
        else if(start_addr == reg_clock_ym||start_addr == reg_clock_dh||start_addr == reg_clock_ms)
        {
            if(start_addr == reg_clock_ym)
                sensor_params = year_month;
            if(start_addr == reg_clock_dh)
                sensor_params = day_hour;
            if(start_addr == reg_clock_ms)
                sensor_params = min_sec;
        }
        else if(start_addr == reg_quantity)
        {
            sensor_params = quantity;
        }
        else if(start_addr == 0x4001)
        {
            sensor_params = 0x7a;
        }
        else if(start_addr >= reg_period_set&&start_addr <= reg_period_sec)
        {
            sensor_params = auto_report_time;
        }

        if(dev_code ==soilRH_DEV)
        {
            if(ROLE !=GATEWAY)
                dev_params_pack(start_addr,sensor_params,len,0);
        }
        else  if(dev_code ==soilCON_DEV)
            ;
        else  if(dev_code ==co2_DEV);
        else  if(dev_code ==con_DEV);
        else  if(dev_code ==sunShine_DEV);
        else  if(dev_code ==pressure_DEV);
        else  if(dev_code ==flow_DEV);
        else  if(dev_code ==gateway_DEV)
        {
            if(ROLE ==GATEWAY)
                dev_params_pack(start_addr,sensor_params,len,0);
        }
        /*else if(sensor_params== quantity)
        {
              reg_addr = start_addr-reg_quantity;
        			p=ReadRegister(start_addr);//
        			for(j=0;j<len;j=j+4)
        			{
        				protocolC.payload[j] = sensor_params;
        				protocolC.payload[j+1] = UNSIGNED;
        				memcpy(&protocolC.payload[j+2],p+reg_addr+j/2,2);
        				index =index +2;
        			}
        	dev_params_pack(start_addr,sensor_params,len);
        }
        else if(sensor_params >=year_month&&sensor_params<=min_sec)
        {
        reg_addr = start_addr-reg_clock_ym;
        p=ReadRegister(start_addr);//
        for(j=0;j<len;j=j+4)
        {
        	protocolC.payload[j] = sensor_params++;
        	protocolC.payload[j+1] = UNSIGNED;
        	memcpy(&protocolC.payload[j+2],p+reg_addr+j/2,2);
        	index =index +2;
        }
        dev_params_pack(start_addr,sensor_params,len);

        }*/
        /*if(sensor_params == auto_report_time)
        {
        	len = 40;
        	sensor_params = temperature;
            dev_params_pack(start_addr,sensor_params,len);
        		reg_addr = start_addr-reg_soil_temp_5;
        		if(start_addr >= reg_soil_temp_5&&start_addr <= reg_soil_rh_1)
        		{
        			len = 40;
        			sensor_params = temperature;
        			p=ReadRegister(start_addr);//
        			for(j=0;j<len;j=j+4)
        			{
        				unsigned char k;
        				k = j/4;
        				if(k%2)
        				{

        					if(sensor_params == temperature)
        					protocolC.payload[j] =rh;
        					else
        					protocolC.payload[j] = temperature;
        				}
        				else
        				{
        					protocolC.payload[j] = sensor_params;// 0 1 2 3   4 5 6 7 8 9 10 11
        				}
        				protocolC.payload[j+1] = SIGN_DOT1;
        				memcpy(&protocolC.payload[j+2],p+reg_addr+j/2,2);
        				index =index +2;
        				if(j>=40)//0 8 16 24 32
        					break;

        			}
        		}
        }*/

    }

}
void SendPayloadPack(LORAHW_stru *loraNo,unsigned char ackMode)
{
    unsigned char p[256];
    unsigned int calCRC;
    unsigned int len,len_payload;
    unsigned char i;
    i = 0;
    len = 0;
    memcpy(p,protocolC.DesAddr,ADDR_SIZE);//8字节目的地址
    i=i+ADDR_SIZE;
    len = len +ADDR_SIZE;
    memcpy(p+i,protocolC.SrcAddr,ADDR_SIZE);	//8字节源地址
    i=i+ADDR_SIZE;
    len = len +ADDR_SIZE;
    p[i++] = protocolC.devCode;
    p[i++] = protocolC.funCode;
    len = len +2;
    if(ackMode == 0)
    {
        memcpy(p+i,protocolC.startAddr,2);
        i=i+2;
        len = len +2;
        p[i++] =protocolC.payloadLenL;
        len_payload = protocolC.payloadLenL;
        len = len +1;
        if(ROLE == GATEWAY)
        {
            if(((protocolC.devCode == gateway_DEV)&&(protocolC.funCode == gateway_Rcmd||protocolC.funCode == pc_cmd))||*getDataSrc() == DAT_FROM_LORA)
            {
                memcpy(p+i,protocolC.payload,len_payload );
                i = i+len_payload;
                len =len+len_payload;
            }
            else
            {

            }
        }
        else
        {
            // if(protocolC.devCode != gateway_DEV&&protocolC.funCode == gateway_Rcmd)
            {
                memcpy(p+i,protocolC.payload,len_payload );
                //len = protocolC.payloadLenH;
                // len = len <<8;
                i = i+len_payload;
                len =len+len_payload;
            }
        }
    }
    else if(ackMode == 1)
    {
        p[i++]  = 0x01;//功能码错误
        len = len + 1;
    }
    else if(ackMode == 2)
    {
        p[i++]  = 0x02;//addr错误
        len = len + 1;
    }
    else if(ackMode == 3)
    {
        p[i++]  = 0x03;//数据个数错误
        len = len + 1;
    }
    else if(ackMode == 4)
    {
        p[i++]  = 0x04;//多个寄存器错误
        len = len + 1;
    }
    else if(ackMode == 5)
    {
        p[i++]  = 0x05;//预留
        len = len + 1;
    }
    if(ackMode == 0)
    {
        calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-ADDR_SIZE-ADDR_SIZE);//计算所接收数据的CRC
        p[i++] =(u8)(calCRC>>8);
        p[i++] =(u8)(calCRC);
        calCRC=CRC_Compute(p,len+2);//计算所接收数据的CRC
        p[i++] =(u8)(calCRC>>8);
        p[i++] =(u8)(calCRC);
    }
    else
    {
        len = 19;
        calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-ADDR_SIZE-ADDR_SIZE);//计算所接收数据的CRC
        p[i++] =(u8)(calCRC>>8);
        p[i++] =(u8)(calCRC);
        calCRC=CRC_Compute(p,len+2);//计算所接收数据的CRC
        p[i++] =(u8)(calCRC>>8);
        p[i++] =(u8)(calCRC);
    }

    unsigned char *nbiot_enable,*data_save;
    if(ROLE == GATEWAY)
    {
        nbiot_enable = ReadRegister(0x45f5);//nbiot 485推肥使能
        data_save = ReadRegister(0x45f6);//节点数据存储与否
    }



    unsigned char *datasrc;
    datasrc = getDataSrc();
    if(*datasrc== DAT_FROM_RELAY)//数据转发
    {
        LoraSendPayloadPackTx(p,len+4);
        txmode = 1;
    }
    else if(*datasrc== DAT_FROM_NBIOT)
    {
        if(ROLE == GATEWAY)//网关
        {
            if(protocolC.devCode == soilRH_DEV)//数据走nbiot
            {
                LoraSendPayloadPackTx(p,len+4);
                txmode = 1;
            }
            else
            {
                if(Get_Network_status()==SIMCOM_NET_OK)
                    SnedToNbiot(p,len+4);
                txmode = 2;
            }
        }
        else //终端
        {
            if(nbiot_enable[1]==1)
            {
                //if(protocolC.funCode == auto_report_cmd||protocolC.devCode == gateway_DEV)
                {
                    if(Get_Network_status()==SIMCOM_NET_OK)
                        SnedToNbiot(p,len+4);
                    txmode = 2;
                }
            }
        }
    }
    else if(*datasrc==DAT_FROM_LORA)
    {
        if(ROLE == GATEWAY)//网关
        {
            //if(protocolC.funCode == auto_report_cmd||protocolC.devCode == gateway_DEV)//数据走nbiot
            {
                if(Get_Network_status()==SIMCOM_NET_OK)
                    SnedToNbiot(p,len+4);
                txmode = 2;
            }
        }
        else //终端
        {
            if(nbiot_enable[1]==1)
            {
                //if(protocolC.funCode == auto_report_cmd||protocolC.devCode == gateway_DEV)
                {
                    if(Get_Network_status()==SIMCOM_NET_OK)
                        SnedToNbiot(p,len+4);
                    txmode = 2;
                }
            }
            else
            {
                //if(protocolC.funCode == auto_report_cmd||protocolC.devCode == gateway_DEV)
                {
                    LoraSendPayloadPackTx(p,len+4);
                    txmode = 1;
                }
            }
        }
    }
    else if(*datasrc== DAT_FROM_PC||nbiot_enable[1]==2)//数据来自推肥系统
    {
        if(data_save[1]==0)//实时传输
            RS485_SendData(p, len+4,1);
        txmode = 3;
    }
    *datasrc = 0;
//    else if(*datasrc== DAT_FROM_NBIOT||*datasrc==DAT_FROM_LORA)//数据来自nbiot
//    {
//			  if(ROLE == GATEWAY||nbiot_enable[1]==1)//网关
//				{
//					if(protocolC.funCode == auto_report_cmd||protocolC.devCode == gateway_DEV)//数据走nbiot
//					{
//					if(Get_Network_status()==SIMCOM_NET_OK)
//							SnedToNbiot(p,len+4);
//					}
//					else
//				}
//				else if(ROLE != GATEWAY)
//				{
//					if(nbiot_enable[1]==1)
//					{
//						if(protocolC.funCode == auto_report_cmd)
//						{
//							if(Get_Network_status()==SIMCOM_NET_OK)
//									SnedToNbiot(p,len+4);
//						}
//
//					}
//					else
//					{
//						if(*datasrc==DAT_FROM_LORA)
//							LoraSendPayloadPackTx(p,len+4);
//					}
//				}

//    }
//    else if(*datasrc== DAT_FROM_PC||nbiot_enable[1]==2)//数据来自推肥系统
//    {
//        if(data_save[1]==0)//实时传输
//            RS485_SendData(p, len+4,1);
//    }
//
//    else if((*datasrc== DAT_FROM_LORA||protocolC.funCode == auto_report_cmd)&&ROLE != GATEWAY)//数据来自lora
//    {
//        LoraSendPayloadPackTx(p,len+4);
//    }
//    *datasrc = 0;

}


void SendLoraData(LORAHW_stru *loraNo,unsigned char *q)
{
    unsigned char p[280];
    unsigned int calCRC;
    unsigned int len,i;
    i = 0;
    len = 0;
    memcpy(p,protocolC.DesAddr,ADDR_SIZE);
    i=i+ADDR_SIZE;
    len = len +ADDR_SIZE;
    memcpy(p+i,protocolC.SrcAddr,ADDR_SIZE);
    i=i+ADDR_SIZE;
    len = len +ADDR_SIZE;
    p[i++] = protocolC.devCode;
    p[i++] = protocolC.funCode;
    len = len +2;
    //if(protocolC.startAddr[0]!=0&&protocolC.startAddr[1]!=0)
    {
        memcpy(p+i,protocolC.startAddr,2);
        i=i+2;
        len = len +2;

    }

    // *(p++) =protocolC.payloadLenH;
    p[i++] =protocolC.payloadLenL;
    len = len +1;
    if(protocolC.devCode != gateway_DEV&&protocolC.funCode != gateway_Rcmd)
    {
        memcpy(p+i,protocolC.payload,protocolC.payloadLenL );
        len = len +protocolC.payloadLenL;
        i = i+protocolC.payloadLenL;
    }

    /* else if(ackMode == 1)
     {
    	*(p++)  = 0x01;//功能码错误
    	len = len + 1;
     }
     else if(ackMode == 2)
     {
    	*(p++)  = 0x02;//功能码错误
    	len = len + 1;
     }
     else if(ackMode == 3)
     {
    	*(p++)  = 0x03;//功能码错误
    	len = len + 1;
     }
     else if(ackMode == 4)
     {
    	*(p++)  = 0x04;//功能码错误
    	len = len + 1;
     }
     else if(ackMode == 5)
     {
    	*(p++)  = 0x05;//功能码错误
    	len = len + 1;
     }*/
    calCRC=CRC_Compute(&p[ADDR_SIZE+ADDR_SIZE],len-ADDR_SIZE-ADDR_SIZE);//计算所接收数据的CRC
    p[i++] =(u8)(calCRC>>8);
    p[i++] =(u8)(calCRC);
    calCRC=CRC_Compute(p,len+2);//计算所接收数据的CRC
    p[i++] =(u8)(calCRC>>8);
    p[i++] =(u8)(calCRC);
    unsigned char *fameStatus;
    fameStatus = modbusFrameStatus();
    if(*fameStatus== 2)//数据来自串口
    {
        *fameStatus = 0;
        RS485_SendData(p, len+4,1);

    }
    else
    {
        LoraSendPayloadPackTx(p,len+4);

    }

}
static unsigned char dataToDestFLag=0;
unsigned char *getDataSrc()
{
    return &dataToDestFLag;
}
void setDataSrc(unsigned char p)
{
    dataToDestFLag = p;
}

void WrRead_equipment(LORAHW_stru *loraNo)
{
    unsigned int len;
    unsigned int addr;
    unsigned char tmp[256],i;
    unsigned char *data_save,*nbiot_enable;
    data_save = ReadRegister(0x45f6);//节点数据存储与否
    nbiot_enable = ReadRegister(0x45f5);//nbiot 485推肥使能

    len = 0;
    ackmode = 0;
    //if((protocolC.devCode <=flow_DEV&&protocolC.devCode>0)||protocolC.devCode == gateway_DEV)
    {
        if(protocolC.funCode == gateway_Rcmd||protocolC.funCode == pc_cmd||protocolC.funCode == auto_report_cmd||
                protocolC.funCode == PC_readFrame)//判断功能码是否正确
        {
            len = protocolC.payloadLenH;  //数据长度
            len = len<<8;
            len = len+ protocolC.payloadLenL;  //数据长度
            addr = protocolC.startAddr[0];//起始寄存器地址
            addr = addr << 8;//起始寄存器地址
            addr = addr+protocolC.startAddr[1];//起始寄存器地址

            if(((addr+len)<=x1_ADDR_END||(addr+len)<=x12_ADDR_END||
                    (addr+len)<=x4_ADDR_END||(addr+len)<=x44_ADDR_END||(addr+len)<=xE_ADDR_END
                    ||(addr+len)<=xF_ADDR_END))//判断寄存器地址是否正确
                ackmode = 0;
            else
                ackmode = 2;//地址错误
            if(addr == 0)
                ackmode = 1;//功能码错误
            if(len<PAYLOAD_MIN_LEN||len>PAYLOAD_MAX_LEN)//判断数据长度是否正确
                ackmode = 3;//数据长度错误
            if(ackmode != 0)
                protocolC.funCode = gateway_Rcmd;//数据有错误，功能码改为gateway_Rcmd ，0x17


            /* p[i++]  = 0x01;//功能码错误
             p[i++]  = 0x02;//addr错误
             p[i++]  = 0x03;//数据个数错误
             p[i++]  = 0x04;//多个寄存器错误
            */

            if(protocolC.dataType == 1)//作为网关时接收到终端的错误应答数据
            {
                switch(protocolC.payload[0])
                {
                case 1:
                    ackmode = 1;
                    break;
                case 2:
                    ackmode = 2;
                    break;
                case 3:
                    ackmode = 3;
                    break;
                case 4:
                    ackmode = 4;
                    break;
                }
            }


            if(protocolC.funCode == gateway_Rcmd)//读寄存器
            {
                if(ackmode == 0)
                    get_equipmentData();//获取设备数据
                if(ROLE != GATEWAY)
                {
                    ; //;if(ackmode == 0)
                    //   get_equipmentData();//获取设备数据
                }
                else //******************网关数据处理************************************
                {
                    if(*getDataSrc() == DAT_FROM_LORA&&(ackmode == 0))//
                    {
                        if(data_save[1]==1||(Get_Network_status()!=SIMCOM_NET_OK)&&nbiot_enable[1]!=1)//节点数据存储
                        {
                            for(i = 0; i<len/4; i++) //负载数据提取，数据从2开始，因为数组定义中前两个字节分别是名称和格式
                            {
                                memcpy(tmp+i*2,&(protocolC.payload[i*4+2]),2);
                            }
                            WriteRegisteSet(addr,tmp,len/2); //写入寄存器数据
                            if(Get_Network_status()!=SIMCOM_NET_OK)
                                DataFrameSave();	 //保存上报数据
                            ParamsSave(0);
                        }
                    }
                }
                SendPayloadPack(loraNo, ackmode);//向网关返回数据或者发送到终端
            }
            else if(protocolC.funCode == pc_cmd||protocolC.funCode == PC_readFrame)//写寄存器
            {


                for(i = 0; i<len/4; i++) //负载数据提取，数据从2开始，因为数组定义中前两个字节分别是名称和格式
                {
                    memcpy(tmp+i*2,&(protocolC.payload[i*4+2]),2);
                }
                if((addr!=0x4610)&&(addr!=0x4611))//非绑定终端地址操作
                {
                    if(ackmode == 0)
                    {
                        if(ROLE == GATEWAY)//************网关数据处理*************************
                        {
                            if(addr != 0x4001)
                            {
                                //gateway_DEV
                                // soilRH_DEV
                                if(protocolC.devCode == gateway_DEV)//网关数据写入
                                {
                                    if(WriteRegisteSet(addr,tmp,len/2)==0) //写入寄存器数据
                                    {

                                        equip_set();//与硬件相关的写操作
                                        if(addr != 0x4002)
                                            SendPayloadPack(loraNo, ackmode);//向网关返回数据或者发送到终端
                                    }
                                    else
                                    {
                                        SendPayloadPack(loraNo, 5);//向网关返回数据或者发送到终端
                                    }
                                }
                                else if(protocolC.devCode == soilRH_DEV)//网关数据写入
                                {
                                    if(addr != 0x4002)
                                        SendPayloadPack(loraNo, ackmode);//向网关返回数据或者发送到终端
                                }

                            }
                            else
                            {
                                if(ROLE == GATEWAY)
                                {
                                    dev_params_pack(addr,rain,len,0);
                                    SendPayloadPack(loraNo, ackmode);//
                                }
                            }
                        }
                        else
                        {
                            //gateway_DEV
                            // soilRH_DEV
                            if(protocolC.devCode == soilRH_DEV)//网关数据写入
                            {
                                if(WriteRegisteSet(addr,tmp,len/2)==0) //写入寄存器数据
                                {

                                    equip_set();//与硬件相关的写操作
                                    if(addr != 0x4002)
                                        SendPayloadPack(loraNo, ackmode);//向网关返回数据或者发送到终端
                                }
                                else
                                {
                                    SendPayloadPack(loraNo, 5);//向网关返回数据或者发送到终端
                                }
                            }
                            /* else if(protocolC.devCode == soilRH_DEV)//网关数据写入
                             {
                                 if(addr != 0x4002)
                                     SendPayloadPack(loraNo, ackmode);//向网关返回数据或者发送到终端
                             }*/

                        }
                    }



                }
                else//终端地址
                {
                    if(ROLE == GATEWAY)
                    {
                        if(addr==0x4610)//增加终端
                        {
                            if(equip_bind_analy(tmp)==1)
                            {
                                equip_bind(tmp);
                            }

                        }
                        if(addr==0x4611)//删除终端
                        {
                            if(equip_bind_analy(tmp)==0)
                            {
                                equip_bind_delete(tmp);
                            }

                        }

                    }

                }
                ParamsSave(0);


            }
            else if(protocolC.funCode == auto_report_cmd||autoReportMode()==1)//自动上报
            {
                if((ROLE != GATEWAY)&&(ackmode == 0))
                    get_equipmentData();//获取设备数据

                if(data_save[1]==1||(Get_Network_status()!=SIMCOM_NET_OK&&nbiot_enable[1]!=1))//节点数据存储
                {
                    if(ackmode == 0)
                        DataFrameSave();
                }

                SendPayloadPack(loraNo, ackmode);//向网关返回数据或者发送到终端

            }
        }
        else//功能码不正确
        {
            ackmode = 1;
            protocolC.funCode = gateway_Rcmd;
//        if(ROLE != GATEWAY)
//            get_equipmentData();//获取设备数据
            SendPayloadPack(loraNo, ackmode);//向网关返回数据
        }
    }

}
unsigned char autoReportMode()
{
    unsigned char *tmp;
    tmp = ReadRegister(0xf001);
    return tmp[1];
}
void SnedToNbiot(unsigned char *p,unsigned int len)
{
    getHeartStatus(1);

    //tmp = ((u16)protocolC.payloadLenH)<<8+protocolC.payloadLenL;
    Nbiot_SendData(p,len);

}
loraModuleTimeout_stru loraModuleTimeout;
void wirelessTimoutStart(unsigned char p)
{
    if(p == 1)
    {
        loraModuleTimeout.tickFlag = 1;
        loraModuleTimeout.tickCount = HAL_GetTick();

    }
    else
    {
        loraModuleTimeout.tickFlag = 2;
        loraModuleTimeout.tickCount = HAL_GetTick();

    }


}
unsigned char wirelessTimoutProcess()
{
    static uint32_t timeoutCount;
    unsigned char result;
    result = 1;
    if(loraModuleTimeout.tickFlag == 1)
    {

        timeoutCount = uchar2uint( ReadRegister(0xf004));
        loraModuleTimeout.tickFlag = 2;
        loraModuleTimeout.tickCount = HAL_GetTick();
    }
    if(loraModuleTimeout.tickFlag == 2)
    {
        if((HAL_GetTick()-loraModuleTimeout.tickCount) >= timeoutCount*1000)
        {
            loraModuleTimeout.tickFlag =0;
            result = 0;
            loraModuleTimeout.tickCount = HAL_GetTick();
        }
        else
        {
            result = 1;
        }
    }
//    if(loraModuleTimeout.tickFlag == 0)
//    {
//          loraModuleTimeout.tickFlag =0;
//          result = 1;
//    }
    return result;
}

void PCDevice_Modbus_Process()
{
    RS485_Service();//终端模式应答modbus命令

}


uint32_t process_period;

unsigned char  equipmentProcess(unsigned char p)
{
    
    // process_period =HAL_GetTick();

    if((HAL_GetTick()-process_period)>=EQUIP_PRO_PERIOD)
    {
        if(p ==0)
        {
            if(ROLE != GATEWAY)
                 getRH_T();
        }
        getvbat();
        getGPS();
        process_period =HAL_GetTick();
    }
    getRTC();

    if(ROLE != GATEWAY)
    {
        static unsigned char result;
       // result = 0;
        if(autoReportMode())
        {
            unsigned char tt;
            if(getRtcStatus()->RtcWakeUP == 1)
            {

                wirelessTimoutStart(1);
                getRtcStatus()->RtcWakeUP = 0;


            }
            if(wirelessTimoutProcess() == 0)//休眠时间到主动上报
            {
                result = 0;
                result = EnterStop();//休眠
                // while(getRtcStatus()->RtcWakeUP == 0)
                ;
            }
            else
            {
                if( result == 0)
                {
                    autoreport();
                    result = 1;

                }
                ;//等待接收命令
            }


            return result;
        }
    }
    if(ROLE == GATEWAY)
    {
        if(Get_Network_status()==SIMCOM_NET_OK)
        {
            unsigned char *p;
            p = getHeartStatus(0);
            if(*p == 1)//发送心跳
            {
                getHeartStatus(1);
                protocolC.devCode = gateway_DEV;
                protocolC.startAddr[0] = 0x46;
                protocolC.startAddr[1] = 0x12 ;
                protocolC.payloadLenH =0x00;
                protocolC.payloadLenL = 0;

                LORAHW_stru loraNo;
                loraNo.loraNo = 0;
                loraNo.mode =  TransmitMode;
                get_equipmentData();//获取设备数据
                setDataSrc(DAT_FROM_NBIOT);
                SendPayloadPack(&loraNo, 0);//向网关返回数据或者发送到终端
                //getRtcStatus()->RtcWakeUP = 0;
            }
        }
        return 0;
    }

}
void test()
{
    protocolC.devCode = 0x54;
    protocolC.funCode = auto_report_cmd;
    protocolC.startAddr[0] = 0x10;
    protocolC.startAddr[1] = 0x01 ;
    protocolC.payloadLenH =0x00;
    protocolC.payloadLenL = 40;
    LORAHW_stru loraNo;
    loraNo.loraNo = 0;
    loraNo.mode =  TransmitMode;
    get_equipmentData();//获取设备数据
    setDataSrc(DAT_FROM_LORA);
    SendPayloadPack(&loraNo, 0);//向网关返回数据或者发送到终端
}
void autoreport(void)
{
    protocolC.devCode = soilRH_DEV;
    protocolC.funCode = auto_report_cmd;
    protocolC.startAddr[0] = 0x10;
    protocolC.startAddr[1] = 0x01 ;
    protocolC.payloadLenH =0x00;
    protocolC.payloadLenL = 40;
    LORAHW_stru loraNo;
    loraNo.loraNo = 0;
    loraNo.mode =  TransmitMode;
    get_equipmentData();//获取设备数据
    setDataSrc(DAT_FROM_LORA);
    SendPayloadPack(&loraNo, 0);//向网关返回数据或者发送到终端

}
