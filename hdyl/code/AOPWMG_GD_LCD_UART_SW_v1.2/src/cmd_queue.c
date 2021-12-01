#include "cmd_queue.h"




static QUEUE que = {0,0,0};
static qsize cmd_pos = 0;
static qdata _length=0;
//extern  uint8_t buf[32];
//uint8_t *getQueue()
//{
//	return buf;
//}
void queue_reset()
{
	que._head = que._tail = 0;
	cmd_pos =  0;
}
void queue_clear()
{
	que._head = que._tail = 0;
	cmd_pos =  0;
	memset(que._data,0,QUEUE_MAX_SIZE);
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
	
	while(queue_size()>0)
	{
		//取一个数据
		queue_pop(&_data);

		if(cmd_pos==0&&_data!=HEAD)//帧头出错，跳过
		    continue;

		if(cmd_pos<buf_len)//防止缓冲区溢出
		{
			if(cmd_pos ==1)
			{
				_length = _data;
				if(_length > 63)
				{
					cmd_pos =0;
					_length =0;
				}
			}
			buffer[cmd_pos++] = _data;
		}


		//得到完整的帧
		if(_length >0 && cmd_pos>_length)
		{
			cmd_size = cmd_pos;
			cmd_pos = 0;
			
			if(!CheckSum(buffer,_length))//计算校验和
			{
				_length = 0;
				return 0;
			}
			_length = 0;
			
			return cmd_size;
		}
	}

	return 0;//没有形成完整的一帧
}


uint8 CheckSum(uint8 *buffer,uint8 n)
{
	uint8 i,chksum=0;
	for(i=0;i<n;i++)
		chksum += buffer[i];
	
	if(chksum ==  buffer[i])
		return 0xff;
	else
		return 0;
}


