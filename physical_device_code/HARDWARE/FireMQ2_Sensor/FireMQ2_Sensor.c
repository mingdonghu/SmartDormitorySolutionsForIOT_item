#include "FireMQ2_Sensor.h"
#include "delay.h"
#include "Voice_MY2480_16p.h"
#include <stdio.h>
#include "common.h"

void EXTIX_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
//  EXTI_InitTypeDef EXTI_InitStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);//ʹ��PORTFʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIOF0 ����
	
	//�жϴ������ڹ��ϣ������ж�Ӧ���Ǵ�����Ӳ�����⣬�ɳ��Ե͵�ƽ�����������½��ش���
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

// //GPIOF.0  �ж����Լ��жϳ�ʼ������ �½��ش��� PF0
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource0); 

//	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�����ڵ��ⲿ�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure); 

}

//�ⲿ�ж�0������� 
void EXTI0_IRQHandler(void)
{
	
	 //delay_ms(5);//����
	GIZWITS_LOG("\r\n MQ event! \r\n");
	if(readMQ_DO == 0)	 	 
	{				 
		//user_Handel
		voice2Play();   //���־���
	}
	EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ 
	 
}
 




