#ifndef __MALLOC_H
#define __MALLOC_H
#include "sys.h"






#ifndef NULL
#define NULL 0
#endif

//C8T6
//	FLASH 64K ������
//	SRAM	20K

//ZET6
//	FLASH 512K ������
//	SRAM	64K


//���������ڴ��
#define SRAMIN	0	//�ڲ��ڴ��
#define SRAMEX  1	//�ⲿ�ڴ��

#define SRAMBANK 	2	//����֧�ֵ�SRAM����.	



//�ڴ�����趨.
#define MEM_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM_MAX_SIZE					12*1024  						//�������ڴ�
#define MEM_ALLOC_TABLE_SIZE	MEM_MAX_SIZE/MEM_BLOCK_SIZE 	//�ڴ���С
 


		 
//�ڴ���������
struct _m_mallco_dev
{
	void (*init)(void);				//��ʼ��
	u8 (*perused)(void);		  	//�ڴ�ʹ����
	u8 	*membase;					//�ڴ�� 
	u16 *memmap; 					//�ڴ����״̬��
	u8  memrdy; 					//�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev;	//��mallco.c���涨��




void mymemset(void *s,u8 c,u32 count);	//�����ڴ�
void mymemcpy(void *des,void *src,u32 n);//�����ڴ�     
void mem_init(void);					 //�ڴ�����ʼ������(��/�ڲ�����)
u32 mem_malloc(u32 size);		 		//�ڴ����(�ڲ�����)
u8 mem_free(u32 offset);		 		//�ڴ��ͷ�(�ڲ�����)
u8 mem_perused(void);					//���ڴ�ʹ����(��/�ڲ�����)  
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(void *ptr);  				//�ڴ��ͷ�(�ⲿ����)
void *mymalloc(u32 size);				//�ڴ����(�ⲿ����)
void *myrealloc(void *ptr,u32 size);	//���·����ڴ�(�ⲿ����)
#endif






