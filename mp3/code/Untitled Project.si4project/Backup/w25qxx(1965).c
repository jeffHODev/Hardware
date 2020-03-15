#include "w25qxx.h"
#include "spi2.h"
#include "delay.h"
//#include "usart.h"
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//W25QXX��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

u16 W25QXX_TYPE=W25Q256;	//Ĭ����W25Q256

//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25Q256
//����Ϊ32M�ֽ�,����512��Block,8192��Sector 
													 
//��ʼ��SPI FLASH��IO��
void W25QXX_Init(void)
{ 
    u8 temp;
  /*  GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOF_CLK_ENABLE();           //ʹ��GPIOFʱ��
    
    //PF6
    GPIO_Initure.Pin=GPIO_PIN_6;            //PF6
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //����         
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);     //��ʼ��
    
	W25QXX_CS=1;			                //SPI FLASH��ѡ��
	SPI5_Init();		   			        //��ʼ��SPI
	SPI5_SetSpeed(SPI_BAUDRATEPRESCALER_2); //����Ϊ45Mʱ��,����ģʽ*/

	SPI2_Enable();
	W25QXX_TYPE=W25QXX_ReadID();	        //��ȡFLASH ID.
    if(W25QXX_TYPE==W25N01)                //SPI FLASHΪW25Q256
    {
        temp=W25QXX_ReadSR(3);              //��ȡ״̬�Ĵ���3���жϵ�ַģʽ
        if((temp&0X01)==0)			        //�������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ
		{
			W25QXX_CS=0; 			        //ѡ��
			SPI2_ReadWriteByte(W25X_Enable4ByteAddr);//���ͽ���4�ֽڵ�ַģʽָ��   
			W25QXX_CS=1;       		        //ȡ��Ƭѡ   
		}
    }
}  

//��ȡW25QXX��״̬�Ĵ�����W25QXXһ����3��״̬�Ĵ���
//״̬�Ĵ���1��
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
//״̬�Ĵ���2��
//BIT7  6   5   4   3   2   1   0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//״̬�Ĵ���3��
//BIT7      6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//regno:״̬�Ĵ����ţ���:1~3
//����ֵ:״̬�Ĵ���ֵ
u8 W25QXX_ReadSR(u8 regno)   
{  
	u8 byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command=W25X_WriteStatusReg1;    //��״̬�Ĵ���1ָ��
            break;
        case 2:
            command=W25X_WriteStatusReg2;    //��״̬�Ĵ���2ָ��
            break;
        case 3:
            command=W25X_WriteStatusReg3;    //��״̬�Ĵ���3ָ��
            break;
        case 4:
            command=0xC0;    //��״̬�Ĵ���3ָ��
            break;

        default:
            command=W25X_WriteStatusReg1;    
            break;
    }    
	W25QXX_CS=0;                            //ʹ������  
	SPI2_ReadWriteByte(W25X_WriteStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	SPI2_ReadWriteByte(command);            //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI2_ReadWriteByte(0Xff);          //��ȡһ���ֽ�  
	W25QXX_CS=1;                            //ȡ��Ƭѡ     
	return byte;   
	
} 



//дW25QXX״̬�Ĵ���
void W25QXX_Write_SR(u8 regno,u8 sr)   
{   
    u8 command=0;
    switch(regno)
    {
        case 1:
            command=W25X_WriteStatusReg1;    //д״̬�Ĵ���1ָ��
            break;
        case 2:
            command=W25X_WriteStatusReg2;    //д״̬�Ĵ���2ָ��
            break;
        case 3:
            command=W25X_WriteStatusReg3;    //д״̬�Ĵ���3ָ��
            break;
        default:
            command=W25X_WriteStatusReg1;    
            break;
    }   
	W25QXX_CS=0;                            //ʹ������   
	SPI2_ReadWriteByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������
	SPI2_ReadWriteByte(command);            //����дȡ״̬�Ĵ�������    
	SPI2_ReadWriteByte(sr);                 //д��һ���ֽ�  
	W25QXX_CS=1;                            //ȡ��Ƭѡ     	      
}   
//W25QXXдʹ��	
//��WEL��λ   
void W25QXX_Write_Enable(void)   
{
	W25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(W25X_WriteEnable);   //����дʹ��  
	W25QXX_CS=1;                            //ȡ��Ƭѡ     	      
} 
//W25QXXд��ֹ	
//��WEL����  
void W25QXX_Write_Disable(void)   
{  
	W25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(W25X_WriteDisable);  //����д��ָֹ��    
	W25QXX_CS=1;                            //ȡ��Ƭѡ     	      
} 

//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
//0XEF18,��ʾоƬ�ͺ�ΪW25Q256
//��ȡоƬID W25N01��ID:0XEFAA21
u32 W25QXX_ReadID(void)
{
	
	u32 Temp = 0;	  
	W25QXX_CS=0;				    
	SPI1_ReadWriteByte(W25X_JedecDeviceID);//���Ͷ�ȡID����	    
	SPI1_ReadWriteByte(0xFF); 
	Temp|=SPI1_ReadWriteByte(0xFF)<<16; 	
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	W25QXX_CS=1;				    
	return Temp;
}

 

//��ȡSPI FLASH  
//pBuffer:���ݴ洢��
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
//1.ҳ���ݶ�ȡ��13h������ָ���ڴ�ҳ�е�����ת����Data Buffer��
//2.���ݶ�ȡ��03h������Data Buffer�ж�ȡ����


void W25QXX_Read(u8* pBuffer,u32 nSector,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	W25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(W25X_PageDataRead);      //���Ͷ�ȡ����  
    if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        SPI2_ReadWriteByte((u8)((nSector)>>24));    
    }
    SPI2_ReadWriteByte((u8)((nSector)>>16));   //����24bit��ַ    
    SPI2_ReadWriteByte((u8)((nSector)>>8));   
    SPI2_ReadWriteByte((u8)nSector);  
	W25QXX_CS=1;  
	W25QXX_Wait_Busy();  
	W25QXX_CS=0; 
	SPI2_ReadWriteByte(W25X_ReadData);
	SPI2_ReadWriteByte(0xFF);

    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI2_ReadWriteByte(0XFF);    //ѭ������  
    }
	W25QXX_CS=1;  						 //ȡ��Ƭѡ  
}  





//дSPI_Flash
//Program operation����Load Program Data��Program Execute
//1.���س�������:��program data���ص�DataBuffer��
//2.����ִ�У���Data Buffer�е����ݱ�̵�ָ���������ڴ�ҳ��

//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void W25QXX_Write_Page(u8* pBuffer,u32 nSector,u16 NumByteToWrite)
{
 	u16 i;  
    W25QXX_Write_Enable();                  //SET WEL 
	W25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(W25X_PageProgram);   //Load Program Data   
    SPI2_ReadWriteByte(0);
	SPI2_ReadWriteByte(0);    
	for(i=0;i<2112;i++)
		SPI2_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	W25QXX_CS=1;                            //ȡ��Ƭѡ 
		W25QXX_Wait_Busy();					   //�ȴ�д�����
	W25QXX_CS=0; 	
	SPI2_ReadWriteByte(W25X_ProgramExecute);   
	SPI2_ReadWriteByte(0xff); 
    if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        SPI2_ReadWriteByte((u8)((nSector)>>24)); 
    }
    SPI2_ReadWriteByte((u8)((nSector)>>16)); //����24bit��ַ    
    SPI2_ReadWriteByte((u8)((nSector)>>8));   
    SPI2_ReadWriteByte((u8)nSector);   
	W25QXX_CS=1;

	W25QXX_Wait_Busy();					   //�ȴ�д�����
	W25QXX_Write_Disable() ;

	
} 








//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8* pBuffer,u32 nSector,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=2112-nSector%2112; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)
		pageremain=NumByteToWrite;//������2112�ֽ�
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,nSector,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			nSector+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>2112)pageremain=2112; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
u8 W25QXX_BUFFER[4096];		 
void W25QXX_Write(u8* pBuffer,u32 nSector,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=nSector/(4096*16*2);//������ַ  
	secoff=nSector%(4096*16*2);//�������ڵ�ƫ��
	secremain=4096*16*2-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096*16*2,4096*16*2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096*16*2,4096*16*2);//д����������  

		}else W25QXX_Write_NoCheck(pBuffer,nSector,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			nSector+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096*16*2)secremain=4096*16*2;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}
//��������оƬ		  
//�ȴ�ʱ�䳬��...
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                  //SET WEL 
    W25QXX_Wait_Busy();   
  	W25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(W25X_ChipErase);        //����Ƭ��������  
	W25QXX_CS=1;                            //ȡ��Ƭѡ     	      
	W25QXX_Wait_Busy();   				   //�ȴ�оƬ��������
}   
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ������������ʱ��:150ms
void W25QXX_Erase_Sector(u32 Dst_Addr)   
{  
	//����falsh�������,������   
 	//printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096*16*2;
    W25QXX_Write_Enable();                  //SET WEL 	
    W25QXX_CS=0;  
	SPI2_ReadWriteByte(W25X_SectorErase);   //������������ָ�� 
	SPI2_ReadWriteByte(0xFF);   //������������ָ�� 
    if(W25QXX_TYPE==W25Q256)                //�����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ
    {
        SPI2_ReadWriteByte((u8)((Dst_Addr)>>24)); 
    }
	SPI2_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    SPI2_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI2_ReadWriteByte((u8)Dst_Addr);  	
	W25QXX_CS=1;                            //ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				    //�ȴ��������

} 

//��������(Bad Block Management)
void BadBlockManagement(u16 LBA, u16 PBA)
{
	W25QXX_CS=0;
	SPI2_ReadWriteByte(W25X_SwapBlocks );
	SPI2_ReadWriteByte(LBA);
	SPI2_ReadWriteByte(PBA);
	W25QXX_CS=1;
}

//��ȡBBM���ı�(Read BBM Look Up Table)
void ReadBBMLUT(u16* LBA, u16* PBA)
{
	u16 i;	
	W25QXX_CS=0;
	SPI2_ReadWriteByte(W25X_ReadBBMLUT);
	SPI2_ReadWriteByte(0xFF);
	for(i=0;i<=19;i++)
	{
		SPI2_ReadWriteByte(LBA[i]);
		SPI2_ReadWriteByte(PBA[i]);
	}
	W25QXX_CS=1;
}

//A9h
//�ϵ��Ĭ��ECC-E=1����ʱis enabled for all Program and Read operation
void LastECCFailuraPA(u16 PA_Addr)
{
	PA_Addr*=65536;
	W25QXX_CS=0;
	SPI2_ReadWriteByte(W25X_LastECCFailPageAddress);
	SPI2_ReadWriteByte(0xFF);
	SPI2_ReadWriteByte(PA_Addr);
	W25QXX_CS=1;
}

//�����
//ʹ�ò���ָ��ǰ��������ִ��WriteEnableָ�WEL=1
//PA_Addr ҳ��ַ  0000h-FFFFh  PA[15:0];CA[11:0]
void BlockErase(u32 nSector)
{
	W25QXX_Write_Enable();             //SET WEL
	W25QXX_CS=0;
	SPI2_ReadWriteByte(W25X_BlockErase);
	SPI2_ReadWriteByte(0xFF);
    SPI2_ReadWriteByte((nSector>>16)&0xFF);
	SPI2_ReadWriteByte((nSector>>8)&0xFF);
	SPI2_ReadWriteByte((nSector)&0xFF);  //����16bitҳ��ַ       
	W25QXX_CS=1;
	W25QXX_Wait_Busy();                //�ȴ���������
}





//�ȴ�����
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(4)&0x01)==0x01);   // �ȴ�BUSYλ���
}  




//�������ģʽ
void W25QXX_PowerDown(void)   
{ 
  	W25QXX_CS=0;                            //ʹ������   
    SPI2_ReadWriteByte(W25X_PowerDown);     //���͵�������  
	W25QXX_CS=1;                            //ȡ��Ƭѡ     	      
    delay_us(3);                            //�ȴ�TPD  
}   
//����
void W25QXX_WAKEUP(void)   
{  
  	W25QXX_CS=0;                                //ʹ������   
    SPI2_ReadWriteByte(W25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB    
	W25QXX_CS=1;                                //ȡ��Ƭѡ     	      
    delay_us(3);                                //�ȴ�TRES1
}   
