#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"

#include "timer.h"
#include "usart3.h"
#include "gizwits_product.h" 

/* 用户区当前设备状态结构体*/
dataPoint_t currentDataPoint;
 
//WIFI连接状态
//wifi_sta 0: 断开
//         1: 已连接
u8 wifi_sta=0;
 
//协议初始化
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MS系统定时
  usart3_init(9600);//WIFI初始化
  memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));	//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
}



void UART_test(void)
{
	u16 t;  
	u16 len;

	if(USART_RX_STA&0x8000){					   
		len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
		printf("\r\n您发送的消息为:\r\n\r\n");
		for(t=0;t<len;t++){
			USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
		}
		printf("\r\n\r\n");//插入换行
		USART_RX_STA=0;
			
	}else{
		printf("请输入数据,以回车键结束\n");
		LED1=!LED1;//闪烁LED1[green],提示系统正在运行.
		delay_ms(100); 	
	}
}


int main(void)
{		
 	int key;
	u8 wifi_con=0;//记录wifi连接状态 1:连接 0:断开
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口

	Gizwits_Init();         //协议初始化
	printf("--------机智云接入实验----------\r\n");
	printf("KEY1:AirLink连接模式\t KEY_UP:复位\r\n\r\n");	 
 	
//轮询
while(1)
{
#if 0
	//UART_test();
	LED0 = !LED0;
	delay_ms(300);
	LED1 = !LED1;
	delay_ms(300);
#endif
		if(wifi_con!=wifi_sta)
		 {
			 wifi_con=wifi_sta;
			 wifi_con ? printf("connect"): printf("close");
		 }
		 
		 
		if(currentDataPoint.valueLED == 0x01)
		{
				LED0 = 0;
		}else
		{
				LED0 = 1;
		}
		 
		
		gizwitsHandle(&currentDataPoint);//协议处理
		
		key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1按键
		{
			printf("WIFI进入AirLink连接模式\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
		}			
		if(key==WKUP_PRES)//KEY_UP按键
		{  
			printf("WIFI复位，请重新配置连接\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位 
			wifi_sta=0;//标志wifi已断开
		}
		delay_ms(200);

}	
	
}

