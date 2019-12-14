#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "beep.h"

 
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
 		
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	BEEP_init();		//蜂鸣器初始化
 	
//轮询
while(1)
{
		//UART_test();
	LED0 = !LED0;
	delay_ms(300);
	LED1 = !LED1;
	delay_ms(300);
		
}	
	
}

