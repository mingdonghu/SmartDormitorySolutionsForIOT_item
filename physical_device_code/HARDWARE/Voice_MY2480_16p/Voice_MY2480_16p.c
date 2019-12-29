#include "Voice_MY2480_16p.h"

//��ʼ��PF5��PF1Ϊ�����.��ʹ���������ڵ�ʱ��		    
//MY2480 IO��ʼ��
void MY2480_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //ʹ��PF�˿�ʱ��
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1;				 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOF, &GPIO_InitStructure);					 //�����趨������ʼ��
	 GPIO_SetBits(GPIOF,GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1); //�����

}

/**
*@ �𴲺Ų���
*/
void voice1Play(void)
{
	MYIO5 = 1;
	MYIO4 = 1;
	MYIO3 = 1;
	MYIO2 = 1;
	MYIO1 = 0;
}

/**
*@ ���־�������
*/
void voice2Play(void)
{
	MYIO5 = 1;
	MYIO4 = 1;
	MYIO3 = 1;
	MYIO2 = 0;
	MYIO1 = 1;
}

/**
*@ Ϩ�ƺŲ���
*/
void voice3Play(void)
{
	MYIO5 = 1;
	MYIO4 = 1;
	MYIO3 = 0;
	MYIO2 = 1;
	MYIO1 = 1;
}

/**
*@ ����+
*/
void volumeAdd(void)
{
	MYIO5 = 0;
	MYIO4 = 0;
	MYIO3 = 1;
	MYIO2 = 0;
	MYIO1 = 1;
}

/**
*@ ����-
*/
void volumeSub(void)
{
	MYIO5 = 0;
	MYIO4 = 0;
	MYIO3 = 0;
	MYIO2 = 0;
	MYIO1 = 0;
}



