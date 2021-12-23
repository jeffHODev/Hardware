#ifndef __myQueue_H
#define __myQueue_H	 
#include "main.h"



//push()���������������ĩ�����һ������Ԫ��
//shift()�����������еĵ�һ��Ԫ��ɾ��
//unshift()���������������ǰ�����һ������Ԫ��
//pop()�����������е����һ��Ԫ��ɾ��


//ѭ������ �ṹ������	
typedef struct {
    u32 front, rear;			//frontָ�����ͷ��rearΪ����β��
    u32 capacity,length;	//���д�С������
		u16 carry_num;				//���ݰ��˴�����n�δ��
	
	  u8 **databuf;					//����ָ�� ��̬�ڴ�����
}QueueInfo;	
 

/////////////////////////////////////////////////////////////////////

//�ⲿ����
QueueInfo *queue_init(u32 size,u32 length);//ѭ�����г�ʼ�� 
u8 queue_Deinit(QueueInfo *q);//ѭ������ע��

int8_t queue_push(QueueInfo *q, u8 *cache,u8 length) ;//���ݷ������
int8_t queue_pop(QueueInfo *q,u8 *sendbuf,u16 length);//�Ӷ��а��� n���ֽ� �����ͻ�����
int8_t queue_clear(QueueInfo *q);//�Ӷ��а��� n���ֽ� �����ͻ�����

int8_t serch_queue_data(QueueInfo *q);//�������Ƿ�������

int8_t queue_data_push(QueueInfo *q, u8 *cache,u8 length,u16 times);//���в�������


#endif


 
 
 
 
 
 
 
 

