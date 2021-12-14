#include "cmd_queue.h"
#include "cmd_process.h"

typedef struct _QUEUE
{
	qsize _head;
	qsize _tail;
	qdata _data[QUEUE_MAX_SIZE];	
}QUEUE;

static QUEUE que = {0,0,0};
static qsize cmd_pos = 0;
static qdata _length=0;
static uint8_t cmd_backup=0,cmd_length=0;


void queue_reset()
{
	que._head = que._tail = 0;
	cmd_pos =  0;
}

void queue_push(qdata _data)
{
	qsize pos = (que._head+1)%QUEUE_MAX_SIZE;
	if(pos!=que._tail)//非满状态
	{
		que._data[que._head] = _data;
		que._head = pos;
	}
}

static void queue_pop(qdata* _data)
{
	if(que._tail!=que._head)//非空状态
	{
		*_data = que._data[que._tail];
		que._tail = (que._tail+1)%QUEUE_MAX_SIZE;
	}
}

static qsize queue_size()
{
	return ((que._head+QUEUE_MAX_SIZE-que._tail)%QUEUE_MAX_SIZE);
}

qsize queue_find_cmd(qdata *buffer,qsize buf_len)
{
	qsize cmd_size = 0;
	qdata _data = 0;
	cmd_length = CMD_DATA_LENGTH-1;
	while(queue_size()>0)
	{
		//取一个数据
		queue_pop(&_data);

		if(cmd_pos==0&&_data != NOTIFY_TOUCH_BUTTON &&_data !=  NOTIFY_TOUCH_CHECKBOX&&_data !=NOTIFY_TOUCH_SLIDER
		    &&_data !=  NOTIFY_GET_EDIT&&_data !=NOTIFY_GET_PAGE&&_data !=  NOTIFY_GET_CHECKBOX&&_data !=NOTIFY_GET_SLIDER
		    &&_data !=NOTIFY_TOUCH_EDIT&&_data !=NOTIFY_GET_TOUCH_EDIT)
		    continue;

		if(cmd_pos<buf_len)//防止缓冲区溢出
		{
			if(cmd_pos ==0)
				{
					cmd_backup = _data;
				}
				
				if(cmd_pos ==(CMD_DATA_LENGTH-1))
				{
					 if(cmd_backup == NOTIFY_GET_EDIT ||cmd_backup == NOTIFY_GET_TOUCH_EDIT)
				 			cmd_length = CMD_DATA_LENGTH+ _data-1;
				 			
				}
	
				if(cmd_pos > (CMD_MAX_SIZE-1))
				{
					cmd_pos =0;
					cmd_backup = 0;
				}
			
				buffer[cmd_pos++] = _data;
		}


		//μ?μ?íê??μ???
		if(cmd_pos>cmd_length)
		{
			cmd_size = cmd_pos;
			cmd_pos = 0;
			return cmd_size;
		}
	}

	return 0;//没有形成完整的一帧
}




