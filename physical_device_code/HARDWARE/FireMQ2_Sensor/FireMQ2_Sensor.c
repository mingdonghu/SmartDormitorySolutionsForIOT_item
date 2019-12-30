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

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);//使能PORTF时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOF0 引脚
	
	//中断触发存在故障，初步判断应该是传感器硬件问题，可尝试低电平触发，而非下降沿触发
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

// //GPIOF.0  中断线以及中断初始化配置 下降沿触发 PF0
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource0); 

//	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能所在的外部中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//子优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//使能外部中断通道
//	NVIC_Init(&NVIC_InitStructure); 

}

//外部中断0服务程序 
void EXTI0_IRQHandler(void)
{
	
	 //delay_ms(5);//消抖
	GIZWITS_LOG("\r\n MQ event! \r\n");
	if(readMQ_DO == 0)	 	 
	{				 
		//user_Handel
		voice2Play();   //火灾警告
	}
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位 
	 
}
 




