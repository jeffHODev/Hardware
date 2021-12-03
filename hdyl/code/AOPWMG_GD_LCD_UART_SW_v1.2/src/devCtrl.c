#include "devCtrl.h"
#include "sys.h"
#if CPU == ST
extern TIM_HandleTypeDef htim4;
#endif
/***************************************************************
                  功能：控制电磁阀
**************************************************************/
void EleSwCtrl(unsigned char devNO,unsigned char ctrl)
{

    FlagStatus pinstate;
//	static unsigned char status_flag=0;
    if(ctrl  == ON)
    {
        pinstate = SET;

    }
    else
    {
        pinstate = RESET;
    }

  #if CPU == ST
    switch(devNO)
    {
    case 1:
        HAL_GPIO_WritePin(CVALUE1_GPIO_Port, CVALUE1_Pin, pinstate);
        break;
    case 2:
        HAL_GPIO_WritePin(CVALUE2_GPIO_Port, CVALUE2_Pin, pinstate);
        break;
    case 3:
        HAL_GPIO_WritePin(CVALUE3_GPIO_Port, CVALUE3_Pin, pinstate);
        break;
    case 4:
        HAL_GPIO_WritePin(CVALUE4_GPIO_Port, CVALUE4_Pin, pinstate);
        break;
    case 5:
        HAL_GPIO_WritePin(CVALUE5_GPIO_Port, CVALUE5_Pin, pinstate);
        break;
    default:
        HAL_GPIO_WritePin(CVALUE1_GPIO_Port, CVALUE1_Pin, pinstate);
        HAL_GPIO_WritePin(CVALUE2_GPIO_Port, CVALUE2_Pin, pinstate);
        HAL_GPIO_WritePin(CVALUE3_GPIO_Port, CVALUE3_Pin, pinstate);
        HAL_GPIO_WritePin(CVALUE4_GPIO_Port, CVALUE4_Pin, pinstate);
        HAL_GPIO_WritePin(CVALUE5_GPIO_Port, CVALUE5_Pin, pinstate);
        break;

    }
	#else 

    switch(devNO)
    {
    case 1:
        gpio_bit_write(CVALUE1_GPIO_Port, CVALUE1_Pin, pinstate);
        break;
    case 2:
        gpio_bit_write(CVALUE2_GPIO_Port, CVALUE2_Pin, pinstate);
        break;
    case 3:
        gpio_bit_write(CVALUE3_GPIO_Port, CVALUE3_Pin, pinstate);
        break;
    case 4:
        gpio_bit_write(CVALUE4_GPIO_Port, CVALUE4_Pin, pinstate);
        break;
    case 5:
			gpio_bit_write(CVALUE5_GPIO_Port, CVALUE5_Pin, pinstate);
        break;
    default:
        gpio_bit_write(CVALUE1_GPIO_Port, CVALUE1_Pin, pinstate);
        gpio_bit_write(CVALUE2_GPIO_Port, CVALUE2_Pin, pinstate);
        gpio_bit_write(CVALUE3_GPIO_Port, CVALUE3_Pin, pinstate);
        gpio_bit_write(CVALUE4_GPIO_Port, CVALUE4_Pin, pinstate);
        gpio_bit_write(CVALUE5_GPIO_Port, CVALUE5_Pin, pinstate);
        break;

    }

	#endif 
	
	

}

/***************************************************************
                  功能：控制电机
**************************************************************/

void DcMotorCtrl(unsigned char motorNo,uint32_t pwm)
{
#if CPU == ST

    switch(motorNo)
    {
    case 1:
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, pwm);
        break;
    case 2:
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, pwm);
        break;
    case 3:
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, pwm);
        break;
    default:
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, pwm);
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, pwm);
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, pwm);		
        break;	
    }
#else 
    switch(motorNo)
    {
    case 1:
        timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_0, pwm);
        break;
    case 2:
        timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_1, pwm);
        break;
    case 3:
        timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_2, pwm);
        break;
    default:
        timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_0, pwm);
        timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_1, pwm);
        timer_channel_output_pulse_value_config(TIMER3, TIMER_CH_2, pwm);		
        break;	
    }


	#endif 	
}

void EleCtrl(unsigned char dir,unsigned char ctrl)
{
    FlagStatus pinstate;
    if(ctrl  == ON)
    {
        pinstate = SET;

    }
    else
    {
        pinstate = RESET;
    }
    #if CPU == ST
	if(dir == FORWARD)
	{
	  HAL_GPIO_WritePin(Ele_ConA_GPIO_Port, Ele_ConA_Pin, pinstate);
	  HAL_Delay(20);
		HAL_GPIO_WritePin(Ele_ConB_GPIO_Port, Ele_ConB_Pin, !pinstate);

	}
	else if(dir == BACKWARD)
	{
	  HAL_GPIO_WritePin(Ele_ConA_GPIO_Port, Ele_ConA_Pin, !pinstate);
	  delay_ms(20);
	  HAL_GPIO_WritePin(Ele_ConB_GPIO_Port, Ele_ConB_Pin, pinstate);

	}
	else
	{
	HAL_GPIO_WritePin(Ele_ConA_GPIO_Port, Ele_ConA_Pin, 1);
	HAL_Delay(20);
	HAL_GPIO_WritePin(Ele_ConB_GPIO_Port, Ele_ConB_Pin, 1);

	}
	#else

	if(dir == FORWARD)
	{
	  gpio_bit_write(Ele_ConA_GPIO_Port, Ele_ConA_Pin, pinstate);
	  delay_ms(20);
		gpio_bit_write(Ele_ConB_GPIO_Port, Ele_ConB_Pin, !pinstate);

	}
	else if(dir == BACKWARD)
	{
	  gpio_bit_write(Ele_ConA_GPIO_Port, Ele_ConA_Pin, !pinstate);
	  delay_ms(20);
	  gpio_bit_write(Ele_ConB_GPIO_Port, Ele_ConB_Pin, pinstate);

	}
	else
	{
	gpio_bit_write(Ele_ConA_GPIO_Port, Ele_ConA_Pin, 1);
	delay_ms(20);
	gpio_bit_write(Ele_ConB_GPIO_Port, Ele_ConB_Pin, 1);

	}

	#endif
}
/***************************************************************
                 功能：控制蜂鸣器
**************************************************************/

void BeepCtrl()
{

}

