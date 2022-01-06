#include "ADS129x.h"
#include "main.h"
#include "sys.h"

//#include "sd_card.h"
#include "myQueue.h"



/////////////////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�����ӿƼ�-���
//�Ա� https://shop119364888.taobao.com
//��������:2021/07/01
//�汾��V1.0
//��Ȩ���У�����ؾ���
/////////////////////////////////////////////////////////////////////////////////////////////


extern QueueInfo *UART_Queue;//���л�����ָ��


/////////////////////////////////////////////////////////////////////////////////////////////
//оƬ�ṹ��
_ADS129x_info ADS129x_info;
//ϵͳ����״̬
u8 work_state;		//����״̬
//оƬ��
u32 cannle[8];	//�洢8��ͨ��������g
int32_t	p_Temp[8];	//���ݷ��ͻ���
u8 data_to_send[50];//���ڷ��ͻ���
volatile u8 ads129x_Cache[27];		//129x	���ݻ�����
volatile u8 ads129x_read_flag=0;	//DRDY��ȡ��ɱ�־
volatile u8 lead_off_flag; //���������־
unsigned char ads_num=1;//ads1299Ƭѡ��1��оƬ1��2��оƬ2



#if ADS129X_DEVICE == ADS1299
u8 ADS129x_REG_BUF[ADS129X_REGNUM]=   //ads1299�Ĵ�������
{
    0x3e,//ID		ads1299   xx11 1110
    0x95,//CONFIG1	CONFIG1	1001 0110 ��CLKʱ����� 0X91 8KSPS  0X92 4K 0X93 2K 0X94 1K 0X95 500SPS   0X96 250SPS
    0xD0,//CONFIG2	1101 0000 �ڲ������ź� 	������ֵ(VREFP�CVREFN)/2400=��3.75mV ����24 ʵ�ʲ⵽�ĵ�ѹ��90mV  Ȩֵ 4500000uV/8838606 = 0.509uV
    0xEC,//CONFIG3	1110  1100 ���ڲ���׼��ѹ4.5V  �����������Ŵ���, ,�ڲ�1/2vref��׼���ر����Ȳ��������ȵ���������
    0x00,//LOFF			//�رյ������������
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
#if 1		//ADS1299
#if USE_SQUARE_TESTDATA==1 	//ʹ�÷�������
    0x65,//CH1SET		��һ���� ���� 			6-24 5-12 4-8 3-6 2-4 1-2 0-1
    0x65,//CH2SET	 	�ڶ����� ����ģʽ		0-��ͨ���� 1-�����· 5-�ڲ�����
    0x65,//CH3SET		0x61 �����· ����24
    0x65,//CH4SET		0x60 ��ͨ���� ����24	 0x65 �������� ����24
    0x65,//CH5SET	//1299-4  4-8ͨ��Ĭ��0x61
    0x65,//CH6SET
    0x65,//CH7SET
    0x65,//CH8SET
#elif 	USE_NOISE_TESTDATA==1	//�ڲ���������
    0x61,//CH1SET		��һ���� ���� 			6-24 5-12 4-8 3-6 2-4 1-2 0-1
    0x61,//CH2SET	 	�ڶ����� ����ģʽ		0-��ͨ���� 1-�����· 5-�ڲ�����
    0x61,//CH3SET		0x61 �����· ����24
    0x61,//CH4SET		0x60 ��ͨ���� ����24	 0x65 �������� ����24
    0x61,//CH5SET	//1299-4  4-8ͨ��Ĭ��0x61
    0x61,//CH6SET
    0x61,//CH7SET
    0x61,//CH8SET
#else
    0x60,//CH1SET		��һ���� ���� 			6-24 5-12 4-8 3-6 2-4 1-2 0-1
    0x60,//CH2SET	 	�ڶ����� ����ģʽ		0-��ͨ���� 1-�����· 5-�ڲ�����
    0x60,//CH3SET		0x61 �����· ����24
    0x60,//CH4SET		0x60 ��ͨ���� ����24 0x65 �������� ����24
    0x60,//CH5SET	//1299-4  4-8ͨ��Ĭ��0x61
    0x60,//CH6SET
    0x60,//CH7SET
    0x60,//CH8SET
#endif
#else	//ADS1299-4
#if USE_SQUARE_TESTDATA==1 	//ʹ�÷�������
    0x65,//CH1SET		��һ���� ���� 			6-24 5-12 4-8 3-6 2-4 1-2 0-1
    0x65,//CH2SET	 	�ڶ����� ����ģʽ		0-��ͨ���� 1-�����· 5-�ڲ�����
    0x65,//CH3SET		0x61 �����· ����24
    0x65,//CH4SET		0x60 ��ͨ���� ����24	 0x65 �������� ����24
#elif 	USE_NOISE_TESTDATA==1	//�ڲ���������
    0x61,//CH1SET		��һ���� ���� 			6-24 5-12 4-8 3-6 2-4 1-2 0-1
    0x61,//CH2SET	 	�ڶ����� ����ģʽ		0-��ͨ���� 1-�����· 5-�ڲ�����
    0x61,//CH3SET		0x61 �����· ����24
    0x61,//CH4SET		0x60 ��ͨ���� ����24	 0x65 �������� ����24
#else
    0x60,//CH1SET		��һ���� ���� 			6-24 5-12 4-8 3-6 2-4 1-2 0-1
    0x60,//CH2SET	 	�ڶ����� ����ģʽ		0-��ͨ���� 1-�����· 5-�ڲ�����
    0x60,//CH3SET		0x61 �����· ����24
    0x60,//CH4SET		0x60 ��ͨ���� ����24 0x65 �������� ����24
#endif
    0x61,//CH5SET	//1299-4  4-8ͨ��Ĭ��0x61
    0x61,//CH6SET
    0x61,//CH7SET
    0x61,//CH8SET
#endif
/////////////////////////////////////////////////////////////////////////////////
    0x00,//RLD_SENSP	//FF	ʹ�ö�Ӧͨ���������� //00	������������
    0x00,//RLD_SENSN	//FF	ʹ�ö�Ӧͨ����������	//00	������������
    0x00,//LOFF_SENSP		//�ص���������
    0x00,//LOFF_SENSN		//�ص���������
    0x00,//LOFF_FLIP		//�������䵼����������
    0x00,//LOFF_STATP		//�����������ź�״̬��ֻ����
    0x00,//LOFF_STATN		//�������为�ź�״̬��ֻ����
    0x00,//GPIO		0000 0000	����͵�ƽ
    0x00,//MISC1	�üĴ����ṩ��SRB1����·�ɵ�����ͨ����������Ŀ���
    0x00,//MISC2	�˼Ĵ��������Ա�����ʹ�á�
    0x00,//CONFIG4	0000 0000	//����ת�� ���õ�������Ƚ���
};
#endif






void ads1299NoSet(unsigned char num)
{
    ads_num = num;
}


//ADS129x��IO�ڳ�ʼ��
void ADS129x_IO_Init(void)
{
//		GPIO_InitTypeDef 	GPIO_InitStructure;
//		EXTI_InitTypeDef 	EXTI_InitStructure;
//		NVIC_InitTypeDef	NVIC_InitStructure;
//
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
//
//	#if ADS129X_SPI == ADS129x_USE_SPI1
//		SPI1_Init();//��ʼ��SPI����
//	#elif	ADS129X_SPI == ADS129x_USE_SPI2
//		SPI2_Init();//��ʼ��SPI����
//	#endif
//
//		//CS
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
////		//RESRT
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
////		GPIO_Init(GPIOA, &GPIO_InitStructure);
////		//START
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
////		GPIO_Init(GPIOA, &GPIO_InitStructure);
////		//CLKSEL
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
////		GPIO_Init(GPIOA, &GPIO_InitStructure);

//		//DRDY	//����ʱ�ߵ�ƽ���ɼ�ʱ�͵�ƽ��Ч
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//		//DRDY�жϳ�ʼ��
//	  EXTI_ClearITPendingBit(EXTI_Line0);//����жϱ�־
//  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);//ѡ��ܽ�
//  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;						 //ѡ���ж���·
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	 //����Ϊ�ж����󣬷��¼�����
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;						 //�ⲿ�ж�ʹ��
//  	EXTI_Init(&EXTI_InitStructure);
//
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ѡ���ж�ͨ��
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//�����ȼ�
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//ʹ���ⲿ�ж�ͨ��
//  	NVIC_Init(&NVIC_InitStructure);
//
//		EXTI->IMR &= ~(ADS129X_DRDY_LINE);//�����ⲿ�ж�
//
//		ADS129X_CS=1;
}


void EXTI10_15_IRQHandler(void)
{


    if(  exti_interrupt_flag_get(EXTI_15)==1)//���ݽ����ж�
    {
        if(ads_num == 2)
        {
            ads1299NoSet(1);


            //__disable_irq();
            //////////////////////////////////////////////////////////////////
            //��n���ֽڵ�����,�浽������  //STAT CH1 CH2 ...
            ADS129x_Read_Data();//��ȡ����
            ///////////////////////////////////////////////////////////////////
            if(work_state == SEND_UART || work_state == SEND_BULE)//����/���ڷ���
            {
                //�������������л�����
                if(queue_data_push(UART_Queue,ads129x_Cache+3,ADS129x_info.Ads129x_Data_Move,ADS129x_info.Ads129x_Write_Num))
                {

                }
            }
            //			   else if(work_state == SEND_WIFI )
            //			   {
            //					   if(queue_data_push(WIFI_Info->WIFI_Queue ,ads129x_Cache+3 ,ADS129x_info.Ads129x_Data_Move,ADS129x_info.Ads129x_Write_Num))
            //					   {

            //					   }
            //			   }
            //// 			   else if(work_state == WRITE_TF )
            //// 			   {
            ////
            //// 			   }
            //			   else
            //			   {

            //			   }
            /////////////////////////////////////////////////////////////////////////////////////////////////

        }


        exti_interrupt_flag_clear(EXTI_15); //���LINE�ϵ��жϱ�־λ
        //__enable_irq();
    }
}

void EXTI1_IRQHandler(void)
{


    if(  exti_interrupt_flag_get(EXTI_1)==1)//���ݽ����ж�
    {
       // if(ads_num == 2)
        {
            ads1299NoSet(1);
            ADS129x_Read_Data();//��ȡ����
            ///////////////////////////////////////////////////////////////////
            if(work_state == SEND_UART || work_state == SEND_BULE)//����/���ڷ���
            {
                //�������������л�����
                if(queue_data_push(UART_Queue,ads129x_Cache+3,ADS129x_info.Ads129x_Data_Move,ADS129x_info.Ads129x_Write_Num))
                {

                }
            }

        }
        exti_interrupt_flag_clear(EXTI_1); //���LINE�ϵ��жϱ�־λ
        //__enable_irq();
    }
}

//��ȡx�ֽ����� //STAT CH1 CH2 ...
__INLINE void ADS129x_Read_Data()//72Mʱ���º�����ʱ��Լ10us  8Mʱ���� ������ʱ��Լ 100us
{
    u8 i;

    chip_sel(ads_num,0);

    for(i=0; i<ADS129X_DATANUM; i++)
    {
        ads129x_Cache[i]=ADS129x_SPI_ReadWriteByte(0X00);

    }

    chip_sel(ads_num,1);

    //ADS129X_CS=1;
#if ADS129X_USE_TRIANGLE_TESTDATA ==1 //ͨ��1ʹ��������������
    ads129x_Cache[3]=0;//ͨ��1
    ads129x_Cache[4]=0;
    ads129x_Cache[5]=ads129x_test_flag++;
#endif
    ads129x_read_flag=1;//DRDY�ж�
}


///////////////////////////////////////////////////////////////////////////////////////////
//��������
//ͨ��SPI��ADS129xͨ��
__INLINE u8 ADS129x_SPI_ReadWriteByte(u8 com)
{
    return spi_TransmitReceive(com);

    /*#if ADS129X_SPI == ADS129x_USE_SPI1
    	return SPI1_ReadWriteByte(com);
    #elif	ADS129X_SPI == ADS129x_USE_SPI2
    	return SPI2_ReadWriteByte(com);
    #endif*/
}
//д����
void ADS129x_Send_CMD(u8 data)
{
    //ADS129X_CS=0;
    chip_sel(ads_num,0);
    delay_us(10);
    ADS129x_SPI_ReadWriteByte(data);
    delay_us(10);
    // ADS129X_CS=1;
    chip_sel(ads_num,1);
}
//��д�Ĵ���
//������� ����д����/Ҫ��д�Ĵ�������/����ָ��
void ADS129x_WR_REGS(u8 reg,u8 len,u8 *data)
{
    u8 i;
    // ADS129X_CS=0;
    chip_sel(ads_num,0);
    delay_us(10);
    ADS129x_SPI_ReadWriteByte(reg);
    delay_us(10);
    ADS129x_SPI_ReadWriteByte(len-1);
    if(reg&ADS129X_WREG) //д
    {
        for(i=0; i<len; i++)
        {
            delay_us(10);
            ADS129x_SPI_ReadWriteByte(*data);
            data++;
        }
    }
    else if(reg&ADS129X_RREG)//��
    {
        for(i=0; i<len; i++)
        {
            delay_us(10);
            *data = ADS129x_SPI_ReadWriteByte(0);
            data++;
        }
    }
    delay_us(10);
    chip_sel(ads_num,1);
    //ADS129X_CS=1;
}


//�� ADS129x_REG_BUF �����е�����д��Ĵ���
u8 ADS129x_WRITE_REGBUFF(void)
{
    u8 i,res=0;
    u8 ADS129x_REG_Cache[ADS129X_REGNUM];	//�Ĵ������ݻ���


    ADS129x_WR_REGS(ADS129X_WREG|0,ADS129X_REGNUM,ADS129x_REG_BUF);//д�Ĵ���
    delay_ms(1);
    ADS129x_WR_REGS(ADS129X_RREG|0,ADS129X_REGNUM,ADS129x_REG_Cache);//�ض�
    delay_ms(1);
////////////////////////////////////////////////////////////////
    ADS129X_DEBUG("Ҫд�ļĴ�������:\r\n");
    for(i=0; i<ADS129X_REGNUM; i++	)	//Ҫд�ļĴ�������
        ADS129X_DEBUG("%d %x\r\n",i,ADS129x_REG_BUF[i]);
    ADS129X_DEBUG("�ض��ļĴ�������:\r\n");
//////////////////////////////////////////////////////////
    for(i=0; i<ADS129X_REGNUM; i++	)	//���REGNUM���Ĵ���
    {
        ADS129X_DEBUG("%d %x\r\n",i,ADS129x_REG_Cache[i]); //�ض���������
        if(ADS129x_REG_BUF[i] != ADS129x_REG_Cache[i])
        {
#if ADS129X_DEVICE == ADS1298
            if(i!=0 && i!=18 && i!=19 && i != 20)	//ID ���������GPIO���,�����ж�
                res=1;
            else
                continue;
#elif ADS129X_DEVICE == ADS1299
            if(i!=0 && i!=18 && i!=19 && i != 20 	//ID ���������GPIO���,�����ж�
                    && i!=9 && i!=10 && i!=11 && i != 12 	) //1299-4 9 10 11 12 ����0��ǰ�涼���ö��˵Ļ������Ҳ�����ж���
                res=1;
            else
                continue;
#elif ADS129X_DEVICE == ADS1292
            if(i!= 0 && i!=8 && i != 11)	//0 8 ��11��ID ���������GPIO���
                res=1;
            else
                continue;
#endif
        }
    }
    if(res == 0)
        ADS129X_DEBUG("�Ĵ�����ʼ���ɹ�\r\n");
    else
        ADS129X_DEBUG("�Ĵ�����ʼ��ʧ��\r\n");
    return res;
}


u8 ADS129x_REG_Init(void)
{
    u8 i;
    u8 ADS129x_REG_Cache[ADS129X_REGNUM];
///////////////////////////////////////////////////////////////////////////
    ADS129x_Send_CMD(ADS129X_SDATAC);//����ֹͣ������ȡ��������
    delay_ms(100);
    ADS129x_Send_CMD(ADS129X_WAKEUP);//����ģʽ
    delay_ms(100);
    ADS129x_Send_CMD(ADS129X_RESET);//��λ
    delay_ms(1000);
    ADS129x_Send_CMD(ADS129X_SDATAC);//����ֹͣ������ȡ��������
    delay_ms(100);
//		ADS129x_WR_REGS(ADS129X_RREG|0,ADS129X_REGNUM,ADS129x_REG_Cache);//��Ĭ�ϼĴ���
//		ADS129X_DEBUG("��Ĭ�ϼĴ�������:\r\n");
//		for(i=0;i<ADS129X_REGNUM;i++	)	//���Ĵ�������
//		{
//				ADS129x_DEBUG("%d %x\r\n",i,REG_Cache[i]);
//		}
    return ADS129x_WRITE_REGBUFF();//д�Ĵ���
}



//��ʼ��
//����cmd�� 0�����򴮿ڷ��� / 1 wifi����/ 2 TF���洢

void ADS129x_ReInit(u8 cmd)
{
    u8 res;
    u8 RL_temp=0;

    //��ʼ��1299
//		Main_printf("Init ads129x ");
//		//ADS129x_IO_Init();	//��ʼ��ads129x
/////////////////////////////////////////////////////////////////////////////
//		RL_temp=(!KE_1);
//		Main_printf("�������� %x:\r\n",RL_temp);

    ADS129x_REG_BUF[13] = RL_temp; //������������
    ADS129x_REG_BUF[14] = RL_temp;
///////////////////////////////////////////////////////////////////////////
    while(ADS129x_REG_Init())//��ʼ���Ĵ���
    {
        // Main_printf(" err");
        delay_ms(500);
        //LED1=!LED1;//DS0��˸
    }
    //Main_printf(" ok\r\n");
    ADS129x_Send_CMD(ADS129X_RDATAC);
    ADS129x_Send_CMD(ADS129X_START);	//���Ϳ�ʼ����ת������Ч������START���ţ�
    delay_ms(500);
///////////////////////////////////////////////////////////////////////////
    ADS129x_info.Ads129x_Use_Cannle = ADS129X_USE_CANNLE*ADS129X_CASCADE_NUM; //ͨ��*����
    ADS129x_info.Ads129x_Data_Move = 	ADS129x_info.Ads129x_Use_Cannle*3; //һ�β�������N�ֽ���Ч���� 3*ͨ����

//		if(cmd==0)//����/����
//		{
//				ADS129x_info.Ads129x_Write_Num = UART_QUEUE_LENGTH/ADS129x_info.Ads129x_Data_Move ; //����N����һ����
//		}
//		else if(cmd==1)//WiFi
//		{
//				ADS129x_info.Ads129x_Write_Num = WIFI_QUEUE_LENGTH/ADS129x_info.Ads129x_Data_Move ; //����N����һ����
//		}
//		else if(cmd==2)//TF��
//		{
//				//ADS129x_info.Ads129x_Write_Num = SD_QUEUE_LENGTH/ADS129x_info.Ads129x_Data_Move ; //����N����һ����
//		}
    //  Main_printf("���� %d ͨ������\r\n",ADS129x_info.Ads129x_Use_Cannle);
    //  Main_printf("һ�β������� %d �ֽ�\r\n",ADS129x_info.Ads129x_Data_Move);
    //  Main_printf("���� %d ����һ����\r\n",ADS129x_info.Ads129x_Write_Num);
}




/*���ܣ��Ѳɵ���3���ֽ�ת���з���32λ�� */
int32_t get_volt(u32 num)
{
    int32_t change;
    change = num;
    change <<= 8;
    change >>= 8;
    return change;
}

