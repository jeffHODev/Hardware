#include "filter.h"
#include "sys.h"
#define N  11  

uint32_t filter(uint32_t adc)  

{  

   uint32_t value_buf[N];  

   char count,i,j,temp;  

   for ( count=0;count<N;count++)  

   {  

      value_buf[count] = adc;  

      delay_ms(10);  

   }  

   for (j=0;j<N-1;j++)  

   {  

      for (i=0;i<N-j;i++)  

      {  

         if ( value_buf[i]>value_buf[i+1] )  

         {  

            temp = value_buf[i];  

            value_buf[i] = value_buf[i+1];   

             value_buf[i+1] = temp;  

         }  

      }  

   }  

  return value_buf[(N-1)/2];  

}   


uint32_t filter2(uint32_t adc)  

{  

  
  static uint32_t value_buf[N];  

   static unsigned char count,i,j,temp;  

   if(count <N)
   	{

	   count++;
      value_buf[count] = adc;   
	  return adc;
   }
   else
   	{

	count = 0;
   for (j=0;j<N-1;j++)	
   
	 {	
   
		for (i=0;i<N-j;i++)  
   
		{  
   
		   if ( value_buf[i]>value_buf[i+1] )  
   
		   {  
   
			  temp = value_buf[i];	
   
			  value_buf[i] = value_buf[i+1];   
   
			   value_buf[i+1] = temp;  
   
		   }  
   
		}  
   
	 }	
   
	return value_buf[(N-1)/2];	

   }

  

}   
/*****************************************************
*函数名称：kalman_filter
*函数功能：ADC_滤波
*入口参数：ADC_Value
*出口参数：kalman_adc
*****************************************************/
unsigned long kalman_filter(unsigned long ADC_Value)
{
    float x_k1_k1,x_k_k1;
    static float ADC_OLD_Value;
    float Z_k;
    static float P_k1_k1;

    static float Q = 0.0001;//Q：规程噪声，Q增大，动态响应变快，收敛稳定性变坏
    static float R = 0.005; //R：测试噪声，R增大，动态响应变慢，收敛稳定性变好
    static float Kg = 0;
    static float P_k_k1 = 1;

    float kalman_adc;
    static float kalman_adc_old=0;
    Z_k = ADC_Value;
    x_k1_k1 = kalman_adc_old;

    x_k_k1 = x_k1_k1;
    P_k_k1 = P_k1_k1 + Q;

    Kg = P_k_k1/(P_k_k1 + R);

    kalman_adc = x_k_k1 + Kg * (Z_k - kalman_adc_old);
    P_k1_k1 = (1 - Kg)*P_k_k1;
    P_k_k1 = P_k1_k1;

    ADC_OLD_Value = ADC_Value;
    kalman_adc_old = kalman_adc;

    return kalman_adc;
}

