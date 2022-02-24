#include "spo.h"
#include "bsp.h"
#include "config.h"
#define  cishu   16      //�˲��Ĵ���  8
#define  Snum    200     //�洢�Ĵ���  200
#define  rogncuo 25      //�ݴ�Ĵ��� 
unsigned char d_up=0;
unsigned char d_dn=0;

/***********************************************************
��������:data_proc
��������:���ݴ���
��ڲ���:dat
���ڲ���:��
************************************************************/
void  data_proc(unsigned int temp)
{
    unsigned int x;
    x=temp;
    qw=bw=sw=gw=0;
    if(x>=1000)do
        {
            qw++;
            x=x-1000;
        }
        while(x>999);
    if(x>=100)do
        {
            bw++;
            x=x-100;
        }
        while(x>99);
    if(x>=10)do
        {
            sw++;
            x=x-10;
        }
        while(x>9);
    if(x>=1)do
        {
            gw++;
            x=x-1;
        }
        while(x>0);
}

//��200����֪�������в���
//ADC_tab ����
void  cal_RLED(void)
{
    unsigned char  i;
    F_RLED_max=F_RLED_min=UART_R_buf[0];  //��ó�ʼֵ,������һ������ֵ
    for(i=1; i<Snum; i++)
    {
        if(F_RLED_max<UART_R_buf[i])
        {
            F_RLED_max=UART_R_buf[i];
            //peak=i;
        }
        else if(F_RLED_min>UART_R_buf[i])
        {
            F_RLED_min=UART_R_buf[i];
            //valley=i;
        }
    }
    F_RLED=F_RLED_min+(F_RLED_max-F_RLED_min)/2; //ƽ��Ƶ��
    F_RLED_err=F_RLED_max-F_RLED_min;	          //���
}
//��200����֪�������в���
//ADC_tab ������
void  cal_IRLED(void)
{
    unsigned char  i;
    F_IRLED_max=F_IRLED_min=UART_IR_buf[0];  //��ó�ʼֵ,������һ������ֵ
    for(i=1; i<Snum; i++)
    {
        if(F_IRLED_max<UART_IR_buf[i])
        {
            F_IRLED_max=UART_IR_buf[i];
            //peak=i;
        }
        else if(F_IRLED_min>UART_IR_buf[i])
        {
            F_IRLED_min=UART_IR_buf[i];
            //valley=i;
        }
    }
    F_IRLED=F_IRLED_min+(F_IRLED_max-F_IRLED_min)/2; //ƽ��Ƶ��
    F_IRLED_err=F_IRLED_max-F_IRLED_min;	          //���
}
//��160����֪�������в���
//ADC_tab	  LED
//ͨ��ADC�������ݼ���Ƶ��
void  cal_F_IRLED(void)
{
    unsigned char    i;
    unsigned int     samp;
    unsigned char    count=0;              //����
    samp=F_IRLED_min+(F_IRLED_max-F_IRLED_min)*7/10;	   //70%����
//��-��=551-483��������1.5HZ����������̫���˲ⲻ׼��
    d_up=(F_IRLED_max-F_IRLED_min)/7; //����׼60HZ����
    d_dn=(F_IRLED_max-F_IRLED_min)/7;
//�������Χ������
    for(i=0; i<Snum ; i++)
    {
        if((UART_IR_buf[i]<=(samp+d_up))&&((UART_IR_buf[i]+d_dn)>=samp))  //�ݴ�Χ����2
        {
            store_buf[count]=i;   //0-49	  ��¼��ǰ��Ӧ��ֵ
            count++;
        }
        if(count>99)
            break;                //����99��������
    }
//Ƶ���ݴ����
    for(i=0; i<count; i++)
    {
        cycle=store_buf[i]-store_buf[0];
        if(cycle>rogncuo)
            break;              //����ѭ��
    }
    time1=i;             //��һ�ε��ݴ�ֵ
    for(i=time1; i<count; i++)
    {
        cycle=store_buf[i]-store_buf[time1];
        if(cycle>rogncuo)
            break;              //�ڶ��ε��ݴ�ֵ
    }
    time2=i;
    for(i=time2; i<count; i++)
    {
        cycle=store_buf[i]-store_buf[time2];
        if(cycle>rogncuo)
            break;              //�ڶ��ε��ݴ�ֵ
    }
    time3=i;
    changdu=store_buf[time2]-store_buf[0];
    changdu1=store_buf[time3]-store_buf[time1];
    changdu=(changdu+changdu1)>>1;             //������������
    Freq=7200/(unsigned int)changdu;          //����1min�ӵ�������
}
//=========================================
//Ѫ������
//=========================================
void  deal_spo2(void)
{
    //���
    cal_RLED();       //����ɼ������е������Сֵ
    //�����
    cal_IRLED();      //����ɼ������е������Сֵ
    cal_F_IRLED();    //����Ƶ��
    R=((float)F_RLED_err/(float)F_RLED)/((float)F_IRLED_err/(float)F_IRLED);
    //SPO2=110-15*R;    if(R<=30)
    if(R<=5)
        //SPO2=110-R*3.6;
        SPO2=101-R*2.0;
    else
        SPO2=0;

    if(SPO2>99)
        SPO2=99;          //Ѫ�����ֵΪ99%

}
//������ʼ��
void  canshu_init(void)
{
    unsigned char j=0;
    for(j=0; j<cishu; j++)
        filter_buf[j]=70;
}
//������ʼ��
void  canshu_init2(void)
{
    unsigned char j=0;
    for(j=0; j<cishu; j++)
        filter_SPO2[j]=99;
}
void  canshu_init3(void)
{
    unsigned char j=0;
    for(j=0; j<cishu; j++)
        filter_buf[j]=0;
}

//����ƽ���˲�
//����ȥ��ֵ��ƽ��
void  Slide_filter(void)
{
    unsigned char i,j;
    sum=0;
    for(j=0; j<(cishu-1); j++)			 //����ֵ����
    {
        i=j+1;
        filter_buf[j]=filter_buf[i];
    }
    filter_buf[cishu-1]=Freq;	          //��ǰֵ�������
    //������ɻ����˲�����
    MB_max=MB_min=filter_buf[0];
    for(i=1; i<cishu; i++)
    {
        if(MB_max<filter_buf[i])
        {
            MB_max=filter_buf[i];
        }
        else if(MB_min>filter_buf[i])
        {
            MB_min=filter_buf[i];
        }
    }
    for(j=0; j<cishu; j++)              //���
    {
        sum+=filter_buf[j];
    }
    sum=sum-MB_max;
    sum=sum-MB_min;
    //�������ȥ��ֵ����
    XYMB[1]=sum/14;
    if(XYMB[1]>160)                          //��������������
        XYMB[1]=160;
}
//����ƽ���˲�
void  Slide_filter_spo2(void)
{
    unsigned char i,j;
    sum=0;
    for(j=0; j<(cishu-1); j++)			 //����ֵ����
    {
        i=j+1;
        filter_SPO2[j]=filter_SPO2[i];
    }
    filter_SPO2[cishu-1]=SPO2;      //��ǰֵ�������
    for(j=0; j<cishu; j++)          //���
    {
        sum+=filter_SPO2[j];
    }
    XYMB[0]=sum>>4;
}

//���ݲɼ�����
void  samp_deal(void)
{
    //������ڲɼ����ݣ�����һ�βɼ�û�����,Ҳû�з�������
    //�ɼ�����
    static uint32_t systick_10ms;
    if((HAL_GetTick()-systick_10ms)>=10)
    {
        systick_10ms = HAL_GetTick();
        if(step==0)
        {
            light_red();		 //�����
            //Pulse_count=ADC10_samp(0x02); //ͨ��2��������ѹ�ɼ�
            Pulse_count=getAdcBuf()->adc_value[3]; //ͨ��ADC12_10��������ѹ�ɼ�
            Pulse_count=Pulse_count/4;
            UART_R_buf[RS232_cnt]=Pulse_count;
        }
        else
        {
            light_Ired();		 //�������
            //Pulse_count=ADC10_samp(0x02); //ͨ��2��������ѹ�ɼ�
            Pulse_count=getAdcBuf()->adc_value[3]; //ͨ��ADC12_10��������ѹ�ɼ�
            Pulse_count=Pulse_count/4;
            UART_IR_buf[RS232_cnt]=Pulse_count;
        }
        if(diuqi>0)
        {
            diuqi--;
            RS232_cnt=0;
        }
        else
            RS232_cnt++;		  //���һ�βɼ�,����ɼ��������10ms����Ϊ��ʱ��
        if(RS232_cnt>=Snum)
        {
            onetimes_flag=1;
            RS232_cnt=0;
        }

    }

}
void  no_finger_check(void)
{
    if(F_IRLED_max<F_IRLED_min+50)
    {
        finger_flg=0;
        canshu_init3();
        nofinger_cnt=1;
    }
    else
    {
        finger_flg=1;
        if(nofinger_cnt==1)
        {
            canshu_init();            //������ʼ��
            canshu_init2();           //������ʼ��
            nofinger_cnt=0;
        }
    }
}

void spo_proc()
{
    samp_deal();   //��ʱ����
    //keypro();	   //
    if(onetimes_flag==1)
    {
        step++;
        if(step==1)
        {
            light_Ired();		//�������
        }
        if(step>=2)
        {
            light_stop();		  //ֹͣ
            deal_spo2();			  //Ѫ������
            Slide_filter();		  //�����˲�
            Slide_filter_spo2();   //�˳�Ѫ��
            no_finger_check();     //����ָ���
            data_proc(XYMB[0]);		  //Ѫ�����Ͷ�
            SPO2_buf[1]=sw;
            SPO2_buf[0]=gw;
            data_proc(XYMB[1]);		  //����
            MB_buf[2]=bw;
            MB_buf[1]=sw;
            MB_buf[0]=gw;
            if(nofinger_cnt == 1)
            	{
            	XYMB[0] = 0;
				XYMB[1] = 0;

			}
            //Send_char1(0x68);	  //���ڷ�����ͷ
            //  Send_char1(XY);		//���ڷ���Ѫ��
            //  Send_char1(MB);		//���ڷ�������
            // Send_char1(0x16); 	  //���ڷ�����β 20:14:04:04:24:59   δ���ӣ��������ӣ��Ƴ�����

            light_red();			  //�����
            step=0;
        }
        diuqi=80;
        RS232_cnt=0;
        onetimes_flag=0;		  //һ�����
    }

}
uint8_t *getSpo(void)
{
    return XYMB;
}


