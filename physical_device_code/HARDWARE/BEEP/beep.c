#include "beep.h"

//��������ʼ��
void BEEP_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOB�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //BEEP-->GPIOB.8 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�Ϊ 50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure); //���ݲ�����ʼ�� GPIOB.8
	GPIO_ResetBits(GPIOB,GPIO_Pin_8); //��� 0���رշ��������
}

