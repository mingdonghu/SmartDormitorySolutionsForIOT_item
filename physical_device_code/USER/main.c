#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"

#include "timer.h"
#include "usart3.h"
#include "gizwits_product.h" 

//function statement
void UART_test(void);


/* 用户区当前设备状态结构体*/
dataPoint_t currentDataPoint;	//在gizwits_product.c中已定义
 
//WIFI连接状态
//wifi_sta 0: 断开
//         1: 已连接
//u8 wifi_sta=0;


//协议初始化
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MS系统定时
	usart3_init(9600);//WIFI初始化
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));	//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
}


//主函数
int main(void)
{		
 	int key;
	int32_t ret = 0;
	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	Gizwits_Init();         //协议初始化
	
	printf("\r\n--------机智云IOT-LED接入实验----------\r\n");
	printf("KEY1:AirLink连接模式\t KEY0:SoftAP连接模式\t KEY_UP:复位\r\n\r\n");	 
 	
//轮询
while(1)
{
	userHandle();//用户采集
	
	gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
	
	key = KEY_Scan(0);
	
	if(key == KEY1_PRES){//KEY1 按键
		printf("WIFI 进入 AirLink 连接模式\r\n");
		ret = gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link 模式接入
		printf("ret = %d Bytes\r\n", ret);
		printf("IS OK\r\n");
	}else if(key == KEY0_PRES){// KEY0 按键
		printf("WIFI 进入SoftAP 连接模式\r\n");
		ret = gizwitsSetMode(WIFI_SOFTAP_MODE);
		printf("ret = %d Bytes\r\n", ret);
		printf("IS OK\r\n");
	}else if(key == WKUP_PRES){//KEY_UP 按键
		printf("WIFI 复位，请重新配置连接\r\n");
		ret = gizwitsSetMode(WIFI_RESET_MODE);//WIFI 复位
		printf("ret = %d Bytes\r\n", ret);
		printf("IS OK\r\n");
	}
	delay_ms(200);
	LED1=!LED1;



}	
	
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


