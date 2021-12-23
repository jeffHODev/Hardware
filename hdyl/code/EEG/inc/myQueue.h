#ifndef __myQueue_H
#define __myQueue_H	 
#include "main.h"



//push()方法可以在数组的末属添加一个或多个元素
//shift()方法把数组中的第一个元素删除
//unshift()方法可以在数组的前端添加一个或多个元素
//pop()方法把数组中的最后一个元素删除


//循环队列 结构体声明	
typedef struct {
    u32 front, rear;			//front指向队列头，rear为队列尾部
    u32 capacity,length;	//队列大小，容量
		u16 carry_num;				//数据搬运次数，n次打包
	
	  u8 **databuf;					//二级指针 动态内存申请
}QueueInfo;	
 

/////////////////////////////////////////////////////////////////////

//外部函数
QueueInfo *queue_init(u32 size,u32 length);//循环队列初始化 
u8 queue_Deinit(QueueInfo *q);//循环队列注销

int8_t queue_push(QueueInfo *q, u8 *cache,u8 length) ;//数据放入队列
int8_t queue_pop(QueueInfo *q,u8 *sendbuf,u16 length);//从队列搬运 n个字节 至发送缓冲区
int8_t queue_clear(QueueInfo *q);//从队列搬运 n个字节 至发送缓冲区

int8_t serch_queue_data(QueueInfo *q);//检测队列是否有数据

int8_t queue_data_push(QueueInfo *q, u8 *cache,u8 length,u16 times);//队列插入数据


#endif


 
 
 
 
 
 
 
 

