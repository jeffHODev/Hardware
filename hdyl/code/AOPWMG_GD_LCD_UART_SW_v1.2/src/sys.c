#include "sys.h"
#include "stdio.h"
#if CPU ==ST
extern UART_HandleTypeDef huart1;
#endif
static tick_stru tick_usr;

struct __FILE
{
    int handle;
};
FILE __stdout;

int fputc(int ch, FILE *f)
{
    unsigned char tmp;
    tmp =(unsigned char )ch;
    //HAL_UART_Transmit(&huart3, &tmp, 1, 500);
	#if CPU ==ST
    HAL_UART_Transmit(&huart1, &tmp, 1, 500);
	#else
	   ;//i=0;
 //  while(i<len)
	 {
		 usart_data_transmit(USART0, tmp);
	  while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
	 }
	#endif
    return ch;
}

unsigned char registerTick(unsigned char TickNo,uint32_t ms,unsigned char status,unsigned char mode)
{


    if(mode == 0|| tick_usr.TickTime[TickNo] ==0)
    {
        if(tick_usr.TickTime[TickNo] == 0)
        {
            tick_usr.TickTime[TickNo] = ms;
            tick_usr.TickTimeTmp[TickNo] = HAL_GetTick();
            tick_usr.TickStatus[TickNo] = 0;
            tick_usr.TickNum[TickNo] = 1;
			tick_usr.Tickmode[TickNo] = status;
            return 0 ;
        }
        else
        {
          if( tick_usr.Tickmode[TickNo] !=status&&status!=0)
          {
		  tick_usr.TickTime[TickNo] = ms;
		  tick_usr.TickTimeTmp[TickNo] = HAL_GetTick();
		  tick_usr.TickStatus[TickNo] = 0;
		  tick_usr.TickNum[TickNo] = 1;
		  tick_usr.Tickmode[TickNo] = status;
		  return 0;

		  }
		  else
		return 1;

		}

    }
    else //定时器复位
    {  
        
       
        tick_usr.TickTime[TickNo] = 0;
        tick_usr.TickStatus[TickNo] = 0;
        tick_usr.TickNum[TickNo] = 0;
        tick_usr.TickTimeTmp[TickNo] =HAL_GetTick();
		//tick_usr.Tickmode[TickNo] = 0;

        return 2;
    }

}
unsigned char GetTickNum(unsigned char TickNo)
{
   return  tick_usr.TickNum[TickNo];
}
unsigned char GetTickStatus(unsigned char TickNo)
{
   return  tick_usr.Tickmode[TickNo];
}
unsigned char GetTickResult(unsigned char TickNo)
{
    return  tick_usr.TickStatus[TickNo];
}
unsigned char setTickResult(unsigned char TickNo,unsigned char oper)
{
      tick_usr.TickStatus[TickNo] = oper;
}
void TickPro()
{
    unsigned char i;
    for(i = 0 ; i<TICK_SIZE; i++)
    {
        if(tick_usr.TickNum[i]==1 )
        {
            if((HAL_GetTick()-tick_usr.TickTimeTmp[i])>=tick_usr.TickTime[i])
            {
                tick_usr.TickStatus[i] = 1;
            }
        }
    }
}

uint32_t delay;
extern uint32_t sys_tick;
void delay_ms(uint32_t count)
{
    uint32_t tmp;
	
    tmp = sys_tick;
    while ((sys_tick-tmp)<count)
		;
}
uint32_t HAL_GetTick()
{
	return sys_tick;
}
void systick_config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U)){
        /* capture error */
        while (1){
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}
/***********************************************************************************
 * 函数名：delay_us
 * 描述  ：us延时
 * 输入  ：无
 * 输出  ：无
************************************************************************************/
void delay_us(uint32_t nCount)
{
	uint32_t i,j;
	for(i=0;i<nCount;i++)
	{
  	for(j=0;j<10;j++)
		{
			__ASM("NOP");
		}
	}
}
