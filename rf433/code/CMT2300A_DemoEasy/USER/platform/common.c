#include "common.h"
#include <string.h>

#include "gpio_defs.h"
#include "lcd12864.h"

void no_optimize(const void* p_param)
{
}

void Common_Init(void)
{
}

void GPIO_Config(void)
{ 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    
    SET_GPIO_IN(KEY1_GPIO);
    SET_GPIO_IN(KEY2_GPIO);
    SET_GPIO_IN(KEY3_GPIO);
    SET_GPIO_IN(KEY4_GPIO);
    SET_GPIO_IN(KEY5_GPIO);
    
    SET_GPIO_IN(CMT_GPIO1_GPIO);
    SET_GPIO_IN(CMT_GPIO2_GPIO);
    SET_GPIO_IN(CMT_GPIO3_GPIO);
    SET_GPIO_IN(CMT_GPIO4_GPIO);
}



void set_u16_to_buf(u8 buf[], u16 dat16)
{
    buf[0] = (u8)dat16;
    buf[1] = (u8)(dat16 >> 8);
}

u16 get_u16_from_buf(const u8 buf[])
{
    u16 dat16 = 0;
    dat16  = buf[0];
    dat16 |= ((u16)buf[1]) << 8;
    return dat16;
}

void set_u32_to_buf(u8 buf[], u32 dat32)
{
    buf[0] = (u8)dat32;
    buf[1] = (u8)(dat32 >> 8);
    buf[2] = (u8)(dat32 >> 16);
    buf[3] = (u8)(dat32 >> 24);
}

u32 get_u32_from_buf(const u8 buf[])
{
    u32 dat32 = 0;
    dat32  = buf[0];
    dat32 |= ((u32)buf[1]) << 8;
    dat32 |= ((u32)buf[2]) << 16;
    dat32 |= ((u32)buf[3]) << 24;
    return dat32;
}

void views_print_line(u8 nLine, const char* str)
{
    static u8 buf[32];
    
    memset(buf, ' ', sizeof(buf));
    memcpy(buf, str, strlen(str));
    buf[21] = 0;

    //lcd12864_update_data(0);
    //lcd12864_display_string_6x8(nLine, 0, buf);
    //lcd12864_update_data(1);
}

