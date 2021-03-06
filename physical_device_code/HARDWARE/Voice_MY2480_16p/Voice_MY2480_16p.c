#include "Voice_MY2480_16p.h"

//初始化PF5到PF1为输出口.并使能这两个口的时钟		    
//MY2480 IO初始化
void MY2480_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //使能PF端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1;				 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOF, &GPIO_InitStructure);					 //根据设定参数初始化
	 GPIO_SetBits(GPIOF,GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1); //输出高

}

/**
*@ 起床号播放
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
*@ 火灾警报播放
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
*@ 熄灯号播放
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
*@ 音量+
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
*@ 音量-
*/
void volumeSub(void)
{
	MYIO5 = 0;
	MYIO4 = 0;
	MYIO3 = 0;
	MYIO2 = 0;
	MYIO1 = 0;
}



