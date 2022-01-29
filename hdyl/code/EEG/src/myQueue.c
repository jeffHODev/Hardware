#include "myQueue.h"
#include "malloc.h"
#include <string.h>
//#include "usart.h"	 


/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2021/02/01
//版本：V1.0
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
//QueueInfo存储当前队列的一些信息，
//data为动态申请的连续的队列空间，
//front指向队列头，
//rear为队列尾部，
//capacity为队列可容纳的大小。
/////////////////////////////////////////////////////////////////////////////////////////////
//其中使用QueueInfo存储当前队列的一些信息，data为动态申请的连续的队列空间，
//front指向队列头，rear为队列尾部，capacity为队列可容纳的大小。初始化时，
//将front与rear都置为0。由于是循环使用队列空间，当逐渐入队capacity个元素时，
//此时front超过了队列容量，需要将其重置到0位置，这样将无法判断当前队列是满还是空。
//一种解决办法是，仅使用capacity-1个空间进行存储，始终保持front与rear之间存在
//不小于1个可用空间，此方法与链表的头节点有异曲同工之妙。

/* Recyle Queue Operator Push and Pop Sketch
 * Queue Size: 4
 * Capacity  : 7
 * front       rear          front       rear       rear   front
 *   |          |              |          |          |       |
 *   1  2  3  4[ ][ ][ ]    [ ]1  2  3  4[ ][ ]    4[ ][ ][ ]1  2  3
 *          (1)                    (2)                   (3)
 *
 * PUSH : 5  放入5
 * front          rear       front          rear      rear front
 *   |             |           |             |          |    |
 *   1  2  3  4  5[ ][ ]    [ ]1  2  3  4  5[ ]    4  5[ ][ ]1  2  3
 *          (1+)                   (2+)                  (3+)
 *
 * POP  : 取出
 *    front    rear             front    rear       rear      front
 *      |       |                 |       |          |          |
 *   [ ]2  3  4[ ][ ][ ]    [ ][ ]2  3  4[ ][ ]    4[ ][ ][ ][ ]2  3
 *          (1-)                    (2-)                  (3-)
 * */
 
 //对于同样容量为7，大小为4的循环队列，有以上三种情况。所以当判断队列是否为空、
 //或者是否有可用空间时，切勿直接判断front与rear的大小。因此，当进行入队和出队时，
 //也要针对不同情况进行处理。每次入队时，将元素覆盖在rear处，并将rear后移一位，
 //注意判断队列为空还是满，并且保证其不大于capacity。出队则从队头删除，只需将front向后移动即可。
///////////////////////////////////////////////////////////////////////////////////////////// 
 

//循环队列初始化 容量 长度
QueueInfo *queue_init(u32 size,u32 length)//循环队列初始化 
{
		u32 i;
		QueueInfo *q;
		u8 res=0;
    if (size == 0) return NULL;	
	
    q = (QueueInfo*)mymalloc(sizeof(QueueInfo)); //队列结构
		if(q == NULL)	return NULL; //内存申请失败
	
		q->carry_num=0;//搬运计数
		q->length = length; //队列容量
		q->front = q->rear = 0; //初始化时，将front与rear都置为0。
		q->capacity = size; //队列大小
		q->databuf = (u8 **)mymalloc( q->capacity *4);//申请动态内存空间，队列容量 32位单片机，指针占用4字节空间
		if(q->databuf == NULL)	
		{	
				myfree(q);//释放内存
				return NULL; //内存申请失败
		}

		for(i=0;i<q->capacity;i++) //队列容量
		{		
				*(q->databuf+i) = (u8*)mymalloc(q->length); //申请 n 个大小为 length 的数组 
				if(*(q->databuf+i) == NULL)
				{			
						res=1;
						break;
				}
				memset( *(q->databuf + i),0,length);//清0	
				//Main_printf(" (q->databuf)+%d  %x ",i, ((q->databuf)+i)   );	 //指针数组  指针是32位的
				//Main_printf("*((q->databuf)+%d)  %x ",i, *((q->databuf)+i)   );	//数组指向的地址
		}
		
		if(res)//内存申请失败
		{
				while(i) 
				{
						myfree(*(q->databuf+i));
						i--;
				}
				myfree(q->databuf);//释放内存
				myfree(q);//释放内存
				return NULL;
		}
		
		return q;//返回初始化的队列指针		
}

u8 queue_Deinit(QueueInfo *q)//循环队列注销
{
		u16 i;
		if(q==NULL)		return 1; //指针无效
		for(i=0;i<q->capacity;i++)
		{
				myfree(*(q->databuf+i));//释放内存
		}
		myfree(q->databuf);//释放内存
		myfree(q);//释放内存
		return 0;
}


//数组的前端添加项
//每次入队时，将元素覆盖在rear处，并将rear后移一位，
//注意判断队列为空还是满，并且保证其不大于capacity。
int8_t queue_push(QueueInfo *q, u8 *cache,u8 length) 
{	
		u16 i;
    if (q==NULL) return -1; // need queue
    if ((q->rear+1) % q->capacity == q->front) //队列满了
		{
				q->front = (q->front+1) % q->capacity;//队头自增，删除最先进入的数据
				//return 0;//满了
		}
		memcpy( *(q->databuf + q->rear),cache,length);//搬运  N 个字节			

    q->rear = (q->rear+1) % q->capacity; //取数据，队头自增，存数据，队尾自增	
    return 1;    // return push count 		
}

//从数组的后端移除项
//队列取数据
int8_t queue_pop(QueueInfo *q,u8 *sendbuf,u16 length)//从队列搬运 n个字节 至发送缓冲区
{	
	  if (q==NULL) return -1; // need queue
	  if (q->front==q->rear) return 0;//队列为空
	
		memcpy(sendbuf,*(q->databuf + q->front),length);//copy n 个字节
	
    q->front = (q->front+1) % q->capacity;//取数据，队头自增，存数据，队尾自增
    return 1; //取到数据
}


//检测队列是否有数据
//1 有数据 0 没有数据
int8_t serch_queue_data(QueueInfo *q)
{
		if (q==NULL) return -1; //need queue
	  if (q->front==q->rear) return 0;//队列为空
		else return 1;//有数据
}



//队列清空
int8_t queue_clear(QueueInfo *q)
{
		u16 i;

		if(q == NULL)	return -1; //need queue
		q->carry_num=0;
		q->front = q->rear = 0; //初始化时，将front与rear都置为0。
		for(i=0;i<q->capacity;i++) //队列容量
		{		
				memset( *(q->databuf + i),0,q->length);//清0	
		}
		return 1;
}


//队列的插入，顺次向队列缓存区搬运数据 
//参数：队列指针，数据来源，要搬运的数据长度，要搬运的次数
int8_t queue_data_push(QueueInfo *q, u8 *cache,u8 length,u16 times) 
{	
		u16 i;
    if (q==NULL) return -1; // need queue
    if ((q->rear+1) % q->capacity == q->front) //队列满了
		{
				q->front = (q->front+1) % q->capacity;//队头自增，删除最先进入的数据
				//Main_printf("m");
				//return 0;//满了
		}
		
		memmove( *(q->databuf + q->rear)+ q->carry_num *length,cache,length);//搬运  length 个字节
	
		q->carry_num++;//数据搬运次数计数			
		if(q->carry_num == times) //搬运N次数据
		{
				q->carry_num=0;
				q->rear = (q->rear+1) % q->capacity; //取数据，队头自增，存数据，队尾自增	
				return 1; //搬运完成一帧数据
		}
		else
				return 0; 
}

