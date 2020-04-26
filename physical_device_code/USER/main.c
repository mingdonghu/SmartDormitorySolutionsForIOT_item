#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "timer.h"
#include "usart3.h"
#include "gizwits_product.h" 
#include "Voice_MY2480_16p.h"
#include "PowerSensor_IM1281B.h"
#include "FireMQ2_Sensor.h"


//function statement
void UART_test(void);
void Gizwits_Init(void);


//主函数
int main(void)
{		
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //日志打印调试串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	Gizwits_Init();         //初始化机智云串口-WiFi协议硬件接口
	LCD_Init();				//TFTLCD屏初始化
	MY2480_Init();    //语音播放模块初始化
	usart2_init(4800);   //与IM1281B单相电能计量传感器通讯，默认为4800bps,USART2_TX   PA2   USART2_RX   PA3
	EXTIX_Init();       //MQ-2烟雾、可燃气体传感器响应初始化【用于火灾监测】
	
	
	POINT_COLOR = BLACK;
	LCD_Clear(WHITE);
	LCD_ShowString(30,40,210,24,24,"gizwits IOT device"); 
	LCD_ShowString(30,70,200,16,16,"WIFI mode select:");
	LCD_ShowString(30,90,200,16,16,"KEY1: AirLink mode");
 	LCD_ShowString(30,110,200,16,16,"KEY0: No mode");
	LCD_ShowString(30,130,200,16,16,"KEY_UP: Rest mode");
	
	//轮询
	while(1){
		
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
			
		userHandle();//用户采集上传
	}	
	
}


//协议初始化
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MS系统定时
	usart3_init(9600);//UART-WIFI初始化
	userInit();		//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
}



void UART_test(void)
{
	u16 t;  
	u16 len;

	if(USART_RX_STA&0x8000)
	{					   
		len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
		printf("\r\n您发送的消息为:\r\n\r\n");
		for(t=0;t<len;t++)
		{
			USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
		}
		printf("\r\n\r\n");//插入换行
		USART_RX_STA=0;	
	}
	else
	{
		printf("请输入数据,以回车键结束\n");
		LED1=!LED1;//闪烁LED1[green],提示系统正在运行.
		delay_ms(100); 	
	}
}


