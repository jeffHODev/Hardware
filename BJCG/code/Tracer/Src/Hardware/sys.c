#include "sys.h"
#include "stdio.h"
extern UART_HandleTypeDef huart3;


struct __FILE
{
int handle;
};
FILE __stdout;

int fputc(int ch, FILE *f)
{
				//�������ݵ�����
	unsigned char tmp;
	tmp =(unsigned char )ch;
    HAL_UART_Transmit(&huart3, &tmp, 1, 500);
	return ch;
}

