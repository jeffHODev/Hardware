#include "filter.h"

#define N  11  

uint32_t filter(uint32_t adc)  

{  

   uint32_t value_buf[N];  

   char count,i,j,temp;  

   for ( count=0;count<N;count++)  

   {  

      value_buf[count] = adc;  

      HAL_Delay(10);  

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
	  return NULL;
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

